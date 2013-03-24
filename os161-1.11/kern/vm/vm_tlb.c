#include "opt-A3.h"

#if OPT_A3   





#include <types.h>
#include <addrspace.h>
#include <vm.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <curthread.h>
#include <thread.h>
#include <lib.h>
#include <kern/errno.h>
#include <uw-vmstats.h>


int
tlb_get_rr_victim()
{
    int victim;
    vmstats_inc(VMSTAT_TLB_FAULT);
    static unsigned int next_victim = 0;
    victim = next_victim;
    next_victim = (next_victim + 1) % NUM_TLB;
    return victim;
}

int
vm_fault(int faulttype, vaddr_t faultaddress)
{
	vaddr_t vbase1, vtop1, vbase2, vtop2, stackbase, stacktop;
	paddr_t paddr;
	int i;
	u_int32_t ehi, elo;
	struct addrspace *as;
	int spl;
    
	spl = splhigh();
    
	faultaddress &= PAGE_FRAME;
    
	DEBUG(DB_VM, "dumbvm: fault: 0x%x\n", faultaddress);
    
	switch (faulttype) {
            case VM_FAULT_READONLY:
                return EFAULT;
	    case VM_FAULT_READ:
	    case VM_FAULT_WRITE:
                break;
	    default:
                 splx(spl);
                return EINVAL;
	}
    
	as = curthread->t_vmspace;
	if (as == NULL) {
		/*
		 * No address space set up. This is probably a kernel
		 * fault early in boot. Return EFAULT so as to panic
		 * instead of getting into an infinite faulting loop.
		 */
		return EFAULT;
	}
    
	/* Assert that the address space has been set up properly.
	assert(as->as_vbase1  != 0);
	assert(as->as_pbase1 != 0);
	assert(as->as_npages1 != 0);
	assert(as->as_vbase2 != 0);
	assert(as->as_pbase2 != 0);
	assert(as->as_npages2 != 0);
	assert(as->as_stackpbase != 0);
	assert((as->as_vbase1 & PAGE_FRAME) == as->as_vbase1);
	assert((as->as_pbase1 & PAGE_FRAME) == as->as_pbase1);
	assert((as->as_vbase2 & PAGE_FRAME) == as->as_vbase2);
	assert((as->as_pbase2 & PAGE_FRAME) == as->as_pbase2);
	assert((as->as_stackpbase & PAGE_FRAME) == as->as_stackpbase);
    
	vbase1 = as->as_vbase1;
	vtop1 = vbase1 + as->as_npages1 * PAGE_SIZE;
	vbase2 = as->as_vbase2;
	vtop2 = vbase2 + as->as_npages2 * PAGE_SIZE;
	stackbase = USERSTACK - DUMBVM_STACKPAGES * PAGE_SIZE;
	stacktop = USERSTACK;
    
	if (faultaddress >= vbase1 && faultaddress < vtop1) {
		paddr = (faultaddress - vbase1) + as->as_pbase1;
	}
	else if (faultaddress >= vbase2 && faultaddress < vtop2) {
		paddr = (faultaddress - vbase2) + as->as_pbase2;
	}
	else if (faultaddress >= stackbase && faultaddress < stacktop) {
		paddr = (faultaddress - stackbase) + as->as_stackpbase;
	}
	else {
		splx(spl);
		return EFAULT;
	}
    
     make sure it's page-aligned */

        Pte * faultPte = NULL;
        int errNum = -1;
        errNum = probePte(faultaddress, faultPte);
        paddr = faultPte->frameNum + (faultaddress % PAGE_SIZE);     
        assert((paddr & PAGE_FRAME)==paddr);


       i = tlb_get_rr_victim();
        if (faultPte->flag & PTE_RDONLY) {
            paddr |= TLBLO_VALID;
        }else{
            paddr |= TLBLO_VALID | TLBLO_DIRTY;
        }

       TLB_Read(&ehi, &elo, i);
       if (elo & TLBLO_VALID) {
            vmstats_inc(VMSTAT_TLB_FAULT_REPLACE);
       } else {   
            vmstats_inc(VMSTAT_TLB_FAULT_FREE);
       }

       
        DEBUG(DB_VM, "dumbvm: 0x%x -> 0x%x\n", faultaddress, paddr);
	TLB_Write(faultaddress, paddr, i);
	splx(spl);
	return 0;
}

#endif
