/*
 * =====================================================================================
 *
 *       Filename:  pt.c
 *
 *    Description:  page table
 *
 *        Version:  1.0
 *        Created:  03/24/2013 02:16:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *   Organization:  :
 *
 * =====================================================================================
 */


#include "opt-A3.h"

#if OPT_A3   



#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <addrspace.h>
#include <uw-vmstats.h>
#include <uio.h>
#include <elf.h>
#include <vnode.h>
#include "pt.h"
#include "coremap.h"

struct Pte *
pte_create() 
{
    struct Pte * p;
    p = kmalloc(sizeof(struct Pte)); 
    return p;
}

void
pte_destroy(struct Pte* pte)
{
    kfree(pte);
}

int
calculate_segment (struct addrspace *as, vaddr_t vaddr, int* rValue){

	vaddr_t vbase1, vtop1, vbase2, vtop2, stackbase, stacktop;
        assert(as->as_vbase1 != 0);
	assert(as->as_npages1 != 0);
	assert(as->as_vbase2 != 0);
	assert(as->as_npages2 != 0);
        /* make sure they are page-aligned */
	assert((as->as_vbase1 & PAGE_FRAME) == as->as_vbase1);
	assert((as->as_vbase2 & PAGE_FRAME) == as->as_vbase2);
        
        vbase1 = as->as_vbase1;
	vtop1 = vbase1 + as->as_npages1 * PAGE_SIZE;
	vbase2 = as->as_vbase2;
	vtop2 = vbase2 + as->as_npages2 * PAGE_SIZE;
	stackbase = USERSTACK - VM_STACKPAGES * PAGE_SIZE;
	stacktop = USERSTACK;
    
	if (vaddr >= vbase1 && vaddr < vtop1) {
            *rValue = PT_CODE;
            return 0;
        }
	else if (vaddr >= vbase2 && vaddr < vtop2) {
            *rValue = PT_DATA;
            return 0;
        
	}
	else if (vaddr >= stackbase && vaddr < stacktop) {
             *rValue = PT_STACK;
             return 0;
	}
	else {
	    return EFAULT;
	}

}

    
int
probePte (vaddr_t vaddr , struct Pte **rPte, int* hasPageFault) 
{
    struct addrspace *as;
    int segNum = -1;
    int errCode = -1;
    int res;
    as = curthread->t_vmspace;
    if (as == NULL) {
        return EFAULT;
    }

    errCode = calculate_segment(as, vaddr, &segNum);
    if (errCode) {
        return errCode;
    }

    switch (segNum){
        case PT_CODE:
            *rPte = as->pt_code[(vaddr - as->as_vbase1)/PAGE_SIZE];
            break;
        case PT_DATA:
            *rPte = as->pt_data[(vaddr - as->as_vbase2)/PAGE_SIZE];
            break;
        case PT_STACK:
            *rPte = as->pt_stack[(USERSTACK - vaddr)/PAGE_SIZE];
            break;
        default:
            return EFAULT;
    }

    if ((*rPte)->valid == 0) {
        if (segNum == PT_STACK) {
            res = pageFaultHandler(vaddr, LOAD_FROM_STACK, rPte, segNum);
        } else {
            res = pageFaultHandler(vaddr, LOAD_FROM_ELF, rPte, segNum);
        }
        if (res) return res;
        *hasPageFault = 1;
    } else if ((((*rPte)->flag) & PF_R) == 0) { 
        res = pageFaultHandler(vaddr, LOAD_FROM_SWAP, rPte, segNum);
        if (res) return res;
        *hasPageFault = 1;
    } else {
        *hasPageFault = 0;
    }
    return 0;
}


int
insertPte (vaddr_t vaddr, paddr_t paddr, int segNum, struct Pte **pte) /* report inserted pte back to pte */
{
    struct addrspace *as;

    as = curthread->t_vmspace;
    if (as == NULL) {
        return EFAULT;
    }
    switch(segNum) {
        case PT_CODE:
            *pte = as->pt_code[(vaddr - as->as_vbase1)/PAGE_SIZE]; 
            break;
        case PT_DATA:
            *pte = as->pt_data[(vaddr - as->as_vbase2)/PAGE_SIZE];
            break;
        case PT_STACK:
            *pte = as->pt_stack[(USERSTACK - vaddr)/PAGE_SIZE];
            break;
        default:
            return EFAULT;
    }
    if (*pte == NULL) {
         return EFAULT;
    }
    (*pte)->valid = 1;
    (*pte)->frameNum = paddr & PAGE_FRAME; // TODO need revision
    return 0;
}

int 
pageFaultHandler(vaddr_t vaddr, int faulttype, struct Pte **pte, int segNum)
{
    int res = 0;
    switch(faulttype) {
        case LOAD_FROM_ELF:
            vmstats_inc(VMSTAT_PAGE_FAULT_DISK);
            vmstats_inc(VMSTAT_ELF_FILE_READ);
            res = loadPageFromElf(vaddr, pte, segNum);
            break;
        case LOAD_FROM_SWAP:
            vmstats_inc(VMSTAT_PAGE_FAULT_DISK);
            vmstats_inc(VMSTAT_SWAP_FILE_READ);
            //res = loadPageFromSwap();
            break;
        case LOAD_FROM_STACK:
            vmstats_inc(VMSTAT_PAGE_FAULT_ZERO);
            res = allocZeroedPage(vaddr, pte, segNum);
            break;
        default:
            return EINVAL;
    }
    if (res) return res;
    return 0;
}

int
allocZeroedPage(vaddr_t vaddr, struct Pte** pte, int segNum) // refer to load_segment
{
    struct uio u;
    int result;
    paddr_t paddr;

    u.uio_iovec.iov_ubase = (userptr_t)vaddr;
    u.uio_iovec.iov_len = PAGE_SIZE; // length of the memory space
    u.uio_resid = PAGE_SIZE; // amount to actually read
    u.uio_offset = 0;
    u.uio_segflg = UIO_USERISPACE;
    u.uio_rw = UIO_READ;
    u.uio_space = curthread->t_vmspace;
    
    result = uiomovezeros(PAGE_SIZE, &u);

    if (result) return result;
    
    paddr = vm_getppages(1);
    
    /* Now, insert page table entry */
    
    result = insertPte(vaddr, paddr, segNum, pte);
    if (result) return result;

    /* Now, give control back to vm_fault for updating TLB */
    
    return 0;
    
}

int
loadPageFromElf(vaddr_t vaddr, struct Pte** pte, int segNum)
{ 
        Elf_Ehdr eh;   /* Executable header */
	Elf_Phdr ph;   /* "Program header" = segment header */
	int result, i, pageOffset, vbase1, vbase2;
	struct uio ku;
        paddr_t paddr;
        struct vnode* v = curthread->t_vmspace->elf_file_vnode;
	/*
	 * Read the executable header from offset 0 in the file.
	 */

        paddr = vm_getppages(1);
	
        mk_kuio(&ku, &eh, sizeof(eh), 0, UIO_READ);
	result = VOP_READ(v, &ku);
	if (result) {
		return result;
	}

	if (ku.uio_resid != 0) {
		/* short read; problem with executable? */
		kprintf("ELF: short read on header - file truncated?\n");
		return ENOEXEC;
	}

        if (eh.e_ident[EI_MAG0] != ELFMAG0 ||
	    eh.e_ident[EI_MAG1] != ELFMAG1 ||
	    eh.e_ident[EI_MAG2] != ELFMAG2 ||
	    eh.e_ident[EI_MAG3] != ELFMAG3 ||
	    eh.e_ident[EI_CLASS] != ELFCLASS32 ||
	    eh.e_ident[EI_DATA] != ELFDATA2MSB ||
	    eh.e_ident[EI_VERSION] != EV_CURRENT ||
	    eh.e_version != EV_CURRENT ||
	    eh.e_type!=ET_EXEC ||
	    eh.e_machine!=EM_MACHINE) {
		return ENOEXEC;
	}



        for (i=0; i<eh.e_phnum; i++) {
		off_t offset = eh.e_phoff + i*eh.e_phentsize;
		mk_kuio(&ku, &ph, sizeof(ph), offset, UIO_READ);

		result = VOP_READ(v, &ku);
		if (result) {
			return result;
		}

		if (ku.uio_resid != 0) {
			/* short read; problem with executable? */
			kprintf("ELF: short read on phdr - file truncated?\n");
			return ENOEXEC;
		}

		switch (ph.p_type) {
		    case PT_NULL: /* skip */ continue;
		    case PT_PHDR: /* skip */ continue;
		    case PT_MIPS_REGINFO: /* skip */ continue;
		    case PT_LOAD: 
                        vbase1 = curthread->t_vmspace->as_vbase1;
                        vbase2 = curthread->t_vmspace->as_vbase2;
                        switch(segNum) {
                            case PT_CODE:
                                pageOffset = ph.p_offset + (vaddr - vbase1);
                                break;
                            case PT_DATA:
                                pageOffset = ph.p_offset + (vaddr - vbase2);
                                break;
                            case PT_STACK:
                                return EFAULT;
                                break;
                            default:
                                return EFAULT;
                        }
                        break;
		    default:
			kprintf("loadelf: unknown segment type %d\n", 
				ph.p_type);
			return ENOEXEC;
		}
                result = load_segment(curthread->t_vmspace->elf_file_vnode, pageOffset, vaddr, PAGE_SIZE, PAGE_SIZE,ph.p_flags & PF_X);
               	if (result) {
			return result;
		}
                /* Now, insert page table entry */
                result = insertPte(vaddr, paddr, segNum, pte);
                if (result) return result;
                
                /* Now, give control back to vm_fault for updating TLB */
	}
        return 0;
}




#endif
