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
#include "pt.h"
#include <elf.h>

int
tlb_get_rr_victim()
{
    int victim;
    static unsigned int next_victim = 0;
    victim = next_victim;
    next_victim = (next_victim + 1) % NUM_TLB;
    return victim;
}

int
vm_fault(int faulttype, vaddr_t faultaddress)
{
//	vaddr_t vbase1, vtop1, vbase2, vtop2, stackbase, stacktop;
	paddr_t paddr;
	int i, res, errNum, hasPageFault = 0;
	u_int32_t ehi, elo;
	struct addrspace *as;
	int spl; 
        struct Pte * faultPte = NULL;
	
        spl = splhigh();
    
	faultaddress &= PAGE_FRAME;
    
	DEBUG(DB_VM, "dumbvm: fault: 0x%x\n", faultaddress);
    
	switch (faulttype) {
            case VM_FAULT_READONLY:
                /* if this is a readonly violation - check if the flag in pte contains writeable or not... */
                errNum = probePte(faultaddress, &faultPte, &hasPageFault); // TODO probePte needs more detailed work.
                if (errNum) {
                    splx(spl);
                    return errNum;
                }
                if ((faultPte->flag & PF_R) && (faultPte->flag & PF_W)) { /* ...if yes, then turn on the dirty bit for paddr and write back to TLB... */

                    if (!hasPageFault) vmstats_inc(VMSTAT_TLB_RELOAD);
                    paddr = faultPte->frameNum + (faultaddress % PAGE_SIZE);  
                    assert((paddr & PAGE_FRAME)==paddr); // TODO need reviewing
                    paddr |= TLBLO_VALID | TLBLO_DIRTY;
                    vmstats_inc(VMSTAT_TLB_FAULT);
                    res = TLB_Probe((u_int32_t)faultaddress, 0);
                    vmstats_inc(VMSTAT_TLB_FAULT_REPLACE);
                    TLB_Write(faultaddress, paddr, res);
                } else { /* ...if no, just gracefully terminate the process  */
                    splx(spl);
                    return EFAULT; /* TODO: maybe kill_curthread */
                }
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
    
        errNum = probePte(faultaddress, &faultPte, &hasPageFault); // TODO probePte needs more detailed work.
        if (errNum) {
            splx(spl);
            return errNum;
        }

        if (!hasPageFault) vmstats_inc(VMSTAT_TLB_RELOAD);
        
        paddr = faultPte->frameNum + (faultaddress % PAGE_SIZE);     
        /* make sure it is page-aligned */
        assert((paddr & PAGE_FRAME)==paddr); // TODO need reviewing

        /* we need to first probe the TLB to ensure there is no dup ehi */

        res = TLB_Probe((u_int32_t)faultaddress, 0);

        /* if probe fails, use round-robin algm */
        if (res < 0) i = tlb_get_rr_victim();
        else i = res;

        vmstats_inc(VMSTAT_TLB_FAULT);
        
        if (faultPte->flag & PF_W) {
            paddr |= TLBLO_VALID | TLBLO_DIRTY;
        }else{
            paddr |= TLBLO_VALID;
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
