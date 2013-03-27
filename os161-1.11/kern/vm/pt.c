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
        /* make sure they are aligned */
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
probePte (vaddr_t vaddr , struct Pte **rPte) {
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

    if (rPte->valid == 0) {
        res = pageFaultHandler();
        if (res) {
            return res;
        }
    } else if (rPte->flag & PF_R) { 
        res = pageFaultHandler();
        if (res) {
            return res;
        }
    } else {
        return 0;
    }


    return 0;
}


int
insertPte (vaddr_t vaddr, paddr_t paddr, int segNum) 
{
    struct addrspace *as;
    struct Pte *pte;
    int err;

    as = curthread->t_vmspace;
    if (as == NULL) {
        return EFAULT;
    }
    switch(segNum) {
        case PT_CODE:
            pte = as->pt_code[(vaddr - as->as_vbase1)/PAGE_SIZE]; 
            break;
        case PT_DATA:
            pte = as->pt_data[(vaddr - as->as_vbase2)/PAGE_SIZE];
            break;
        case PT_STACK:
            pte = as->pt_stack(USERSTACK - vaddr)/PAGE_SIZE];
            break;
        default:
            return EFAULT;
    }
    if (pte == NULL) {
         return EFAULT;
    }
    pte->valid = 1;
    pte->frameNum = paddr & PAGE_FRAME;
    return 0;
}

int 
pageFaultHandler (vaddr_t vaddr, int faulttype, paddr_t *paddr, int segNum)
{
    int res;
    switch(faulttype) {
        case IN_ELF:

            res = loadPageFromElf(vaddr, paddr, segNum);
            break;
        case IN_SWAP:

            break;
        default:

   }
}

int
loadPageFromElf(vaddr_t vaddr, paddr_t *paddr, int segNum)
{
        Elf_Ehdr eh;   /* Executable header */
	Elf_Phdr ph;   /* "Program header" = segment header */
	int result, i, pageOffset, vbase1, vbase2, remainingPage;
	struct uio ku;
    
	/*
	 * Read the executable header from offset 0 in the file.
	 */

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
                        {
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
                        }
                        break;
		    default:
			kprintf("loadelf: unknown segment type %d\n", 
				ph.p_type);
			return ENOEXEC;
		}
                if (ph.p_filesz - (pageOffset - ph.p_offset) > PAGE_SIZE) { 
                    result = load_segment(curthread->t_vmspace->elf_file_vnode, ph.p_offset, ph.p_vaddr, PAGE_SIZE, PAGE_SIZE,ph.p_flags & PF_X);
                } else {
                    remainingPage = ph.p_filesz - (pageOffset - ph.p_offset);
                    result = load_segment(curthread->t_vmspace->elf_file_vnode, ph.p_offset, ph.p_vaddr, PAGE_SIZE, remainingPage, ph.p_flags & PF_X);

                }
               	if (result) {
			return result;
		}
	}
}




#endif
