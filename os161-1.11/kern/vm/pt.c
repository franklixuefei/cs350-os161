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
#include <pt.h>
#include <uw-vmstats.h>
#include <uio.h>
#include <elf.h>
#include <vnode.h>


int
calculate_segment (vaddr_t vaddr, int* rValue){
        assert(as->as_vbase1 != 0);
	assert(as->as_npages1 != 0);
	assert(as->as_vbase2 != 0);
	assert(as->as_npages2 != 0);
	assert((as->as_vbase1 & PAGE_FRAME) == as->as_vbase1);
	assert((as->as_vbase2 & PAGE_FRAME) == as->as_vbase2);

        
        
        vbase1 = as->as_vbase1;
	vtop1 = vbase1 + as->as_npages1 * PAGE_SIZE;
	vbase2 = as->as_vbase2;
	vtop2 = vbase2 + as->as_npages2 * PAGE_SIZE;
	stackbase = USERSTACK - VM_STACKPAGES * PAGE_SIZE;
	stacktop = USERSTACK;

	if (faultaddress >= vbase1 && faultaddress < vtop1) {
            *rValue =PT_CODE;
            return 0;
        }
	else if (faultaddress >= vbase2 && faultaddress < vtop2) {
            *rValue =PT_DATA;
            return 0;
        
	}
	else if (faultaddress >= stackbase && faultaddress < stacktop) {
             *rValue =PT_STACK;
             return 0;
	}
	else {
		return EFAULT;
	}


}

    
int
probePte (vaddr_t vaddr , struct Pte *rPte) {
    int segNum = -1;
    int errCode = -1;

    errCode = calculate_segment(vaddr, &segNum);
    if (errCode) {
        return errCode;
    }

    switch (segNum){
        case PT_CODE:
            *rPte = as->pt_code[(vaddr - as->vbase1)/PAGE_SIZE];
            break;
        case PT_DATA:
            *rPte = as->pt_code[(vaddr - as->vbase2)/PAGE_SIZE];
            break;
        case PT_STACK:
            *rPte = as->pt_code[(USERSTACK - vaddr)/PAGE_SIZE];
            break;
        default:
            return EFAULT;
    }
    return 0;
}


#endif
