#ifndef _PT_H_
#define _PT_H_

/*
 * =====================================================================================
 *
 *       Filename:  pt.h
 *
 *    Description:  page table header
 *    
 *        Version:  1.0
 *        Created:  03/24/2013 03:37:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *   Organization:  
 *
 * =====================================================================================
 */

#include "opt-A3.h"

#if OPT_A3

#include <types.h>
#include <addrspace.h>

#define PTE_RDONLY      0
#define PTE_WRONLY      1

#define PT_CODE      0
#define PT_DATA      1
#define PT_STACK      2

struct Pte {
    short int valid;
    int32_t flag;
    paddr_t frameNum;
};

struct Pte * pte_create();
void         pte_destroy(struct Pte* pte);
int          probePte(vaddr_t vaddr, struct Pte **rPte, int *hasPageFault);
int          pageFaultHandler(vaddr_t vaddr, int faulttype, struct Pte **pte, int segNum);
int          allocZeroedPage(vaddr_t vaddr, struct Pte**pte, int segNum);
int          calculate_segment(struct addrspace *as, vaddr_t vaddr, int* rValue);
int          loadPageFromElf(vaddr_t vaddr, struct Pte** pte, int segNum);
int          load_segment(struct vnode *v, off_t offset, vaddr_t vaddr, size_t memsize, size_t filesize, int is_executable);
#endif
#endif /* _PT_H_ */


