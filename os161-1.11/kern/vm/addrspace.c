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
/*
 * Note! If OPT_DUMBVM is set, as is the case until you start the VM
 * assignment, this file is not compiled or linked or in any way
 * used. The cheesy hack versions in dumbvm.c are used instead.
 */

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
	 */
#if OPT_A3   
    int i;

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

	newas->as_vbase1 = old->as_vbase1;
	newas->as_npages1 = old->as_npages1;
	newas->as_vbase2 = old->as_vbase2;
	newas->as_npages2 = old->as_npages2;

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



#endif
	(void)as;  // suppress warning until code gets written
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

	(void)as;
	(void)vaddr;
	(void)sz;
	(void)readable;
	(void)writeable;
	(void)executable;
	return EUNIMP;
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

	(void)as;

	/* Initial user-level stack pointer */
	*stackptr = USERSTACK;
	
	return 0;
}





// TODO dont forget to change me!!



#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <addrspace.h>
#include <vm.h>
#include <machine/spl.h>
#include <machine/tlb.h>

/*
 * Dumb MIPS-only "VM system" that is intended to only be just barely
 * enough to struggle off the ground. You should replace all of this
 * code while doing the VM assignment. In fact, starting in that
 * assignment, this file is not included in your kernel!
 */

/* under dumbvm, always have 48k of user stack */


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

void
vm_bootstrap(void)
{
	/* Do nothing. */
}


/* Allocate/free some kernel-space virtual pages */
vaddr_t
alloc_kpages(int npages)
{
	paddr_t pa;
	pa = getppages(npages);
	if (pa==0) {
		return 0;
	}
	return PADDR_TO_KVADDR(pa);
}


void
free_kpages(vaddr_t addr)
{
	/* nothing */
    
	(void)addr;
}

