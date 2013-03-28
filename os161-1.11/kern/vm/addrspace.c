#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <vm.h>
#include <vfs.h>
#include <kern/unistd.h>
#include "pt.h"
#include "opt-A3.h"
#include <vnode.h>
#include <uw-vmstats.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <elf.h>
/*
 * Note! If OPT_DUMBVM is set, as is the case until you start the VM
 * assignment, this file is not compiled or linked or in any way
 * used. The cheesy hack versions in dumbvm.c are used instead.
 */
#if OPT_A3   

struct addrspace* as_previous = NULL;


#endif
struct addrspace * 
as_create(char* programName)
{
	struct addrspace *as = kmalloc(sizeof(struct addrspace));
	if (as==NULL) {
		return NULL;
	}

	/*
	 * Initialize as needed.
	 */
#if OPT_A3   
        char *copyProgramName = kstrdup(programName);
        vfs_open(copyProgramName, O_RDONLY, &(as->elf_file_vnode));
       // kfree(copyProgramName);
        as->progName = copyProgramName;

        /* these will be kmalloc'ed in as_define_region and as_define_stack */
        as->pt_data = NULL;
        as->pt_code = NULL;
        as->pt_stack = NULL;

        as->as_vbase1 = 0;
	as->as_npages1 = 0;
	as->as_vbase2 = 0;
	as->as_npages2 = 0;

        //This is known here so initialize it here.
        //as->pt_stack = kmalloc(sizeof(struct Pte)*VM_STACKPAGES);
        
#endif
	return as;
}




void
as_destroy(struct addrspace *as)
{
	/*
	 * Clean up as needed.
         *
         *TODO: NEED  TO FREE core map stuff!
         *
         *
         *
	 */
#if OPT_A3
    unsigned int i;

    for (i = 0; i < as->as_npages1; ++i) {
        if (as->pt_code[i]) {
            pte_destroy(as->pt_code[i]);
        }
    }
    kfree(as->pt_code);
    
    for (i = 0; i < as->as_npages2; ++i) {
        if (as->pt_data[i]) {
            pte_destroy(as->pt_data[i]);
        }
    }
    kfree(as->pt_data);

    for (i = 0; i < VM_STACKPAGES; ++i) {
        if (as->pt_stack[i]) {
            pte_destroy(as->pt_stack[i]);
        }
    }
    kfree(as->pt_stack);


#endif
	kfree(as);
}



int
as_copy(struct addrspace *old, struct addrspace **ret)
{
	struct addrspace *newas;

	newas = as_create(old->progName);
	if (newas==NULL) {
		return ENOMEM;
	}

	/*
	 * Write this.
	 */
#if OPT_A3   
        unsigned int i;
	newas->as_vbase1 = old->as_vbase1;
	newas->as_npages1 = old->as_npages1;
	newas->as_vbase2 = old->as_vbase2;
	newas->as_npages2 = old->as_npages2;
        memmove(newas->pt_code, old->pt_code, old->as_npages1* sizeof(struct Pte *));
        /* FIXME*/
        for (i = 0; i < old->as_npages1; ++i) {
            memmove(newas->pt_code[i], old->pt_code[i], sizeof(struct Pte));
        } 
        memmove(newas->pt_data, old->pt_data, old->as_npages2* sizeof(struct Pte *));
        for (i = 0; i < old->as_npages2; ++i) {
            memmove(newas->pt_data[i], old->pt_data[i], sizeof(struct Pte));
        }
        memmove(newas->pt_stack, old->pt_stack, VM_STACKPAGES * sizeof(struct Pte *));
        for (i = 0; i < VM_STACKPAGES; ++i) {
            memmove(newas->pt_stack[i], old->pt_stack[i], sizeof(struct Pte));
        }

#else

	(void)old;
	
#endif
	*ret = newas;
	return 0;
}
void
as_activate(struct addrspace *as)
{
	/*
	 * Write this.
	 */
#if OPT_A3     
    if (as != as_previous) {
    	int i, spl;

	as_previous = as;
        vmstats_inc(VMSTAT_TLB_INVALIDATE);
	spl = splhigh();

	for (i=0; i<NUM_TLB; i++) {
		TLB_Write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
	}

	splx(spl);
    }

#else

	(void)as;  // suppress warning until code gets written
#endif
}

/*
 * Set up a segment at virtual address VADDR of size MEMSIZE. The
 * segment in memory extends from VADDR up to (but not including)
 * VADDR+MEMSIZE.
 *
 * The READABLE, WRITEABLE, and EXECUTABLE flags are set if read,
 * write, or execute permission should be set on the segment. At the
 * moment, these are ignored. When you write the VM system, you may
 * want to implement them.
 */
int
as_define_region(struct addrspace *as, vaddr_t vaddr, size_t sz,
		 int readable, int writeable, int executable)
{
	/*
	 * Write this.
	 */
#if OPT_A3   
	size_t npages; 

	/* Align the region. First, the base... */
	sz += vaddr & ~(vaddr_t)PAGE_FRAME;
	vaddr &= PAGE_FRAME;

	/* ...and now the length. */
	sz = (sz + PAGE_SIZE - 1) & PAGE_FRAME;

	npages = sz / PAGE_SIZE;

	if (as->as_vbase1 == 0) {
		as->as_vbase1 = vaddr;
		as->as_npages1 = npages;
                as->pt_code = kmalloc(sizeof(struct Pte *)*npages);
                if (as->pt_code == NULL) return ENOMEM;
                unsigned int i = 0;
                for (i = 0; i < npages; i++) {
                    as->pt_code[i] = kmalloc(sizeof(struct Pte));
                    if (!as->pt_code[i]) return ENOMEM;
                    as->pt_code[i]->flag = readable | writeable | executable;
                    as->pt_code[i]->valid = 0;
                }
		return 0;
	}
	if (as->as_vbase2 == 0) {
		as->as_vbase2 = vaddr;
		as->as_npages2 = npages;
                as->pt_data = kmalloc(sizeof(struct Pte *)*npages);
                if (as->pt_data == NULL) return ENOMEM;
                unsigned int i = 0;
                for (i = 0; i < npages; i++) {
                    as->pt_data[i] = kmalloc(sizeof(struct Pte));
                    if (!as->pt_data[i]) return ENOMEM;
                    as->pt_data[i]->flag = readable | writeable | executable;
                    as->pt_data[i]->valid = 0;
                }
		return 0;
	}

	/*
	 * Support for more than two regions is not available.
	 */
	kprintf("vm: Warning: too many regions\n");
	return EUNIMP;
#else

	(void)as;
	(void)vaddr;
	(void)sz;
	(void)readable;
	(void)writeable;
	(void)executable;
	return EUNIMP;
#endif
}

int
as_prepare_load(struct addrspace *as)
{
	/*
	 * Write this.
	 */

	(void)as;
	return 0;
}

int
as_complete_load(struct addrspace *as)
{
	/*
	 * Write this.
	 */

	(void)as;
	return 0;
}

int
as_define_stack(struct addrspace *as, vaddr_t *stackptr)
{
	/*
	 * Write this.
	 */

#if OPT_A3   

        if (as->pt_stack == NULL) {
                as->pt_stack = kmalloc(sizeof(struct Pte *)*VM_STACKPAGES);
                if (as->pt_stack == NULL) return ENOMEM;
                int i;
                for (i = 0; i < VM_STACKPAGES; i++) {
                    as->pt_stack[i] = kmalloc(sizeof(struct Pte));
                    as->pt_stack[i]->flag =  PF_R | PF_W;
                    as->pt_stack[i]->valid = 0;
                }
	}
#else

	(void)as;

#endif
	/* Initial user-level stack pointer */
	*stackptr = USERSTACK;
	
	return 0;
}



/*  
 * Dumb MIPS-only "VM system" that is intended to only be just barely
 * enough to struggle off the ground. You should replace all of this
 * code while doing the VM assignment. In fact, starting in that
 * assignment, this file is not included in your kernel!
 
*/
 //under dumbvm, always have 48k of user stack 

/*
static
paddr_t
getppages(unsigned long npages)
{
	int spl;
	paddr_t addr;
	spl = splhigh();
	addr = ram_stealmem(npages);
	splx(spl);
	return addr;
}
*/





