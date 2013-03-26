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

#define PTE_RDONLY      0
#define PTE_WRONLY      1

#define PT_CODE      0
#define PT_DATA      1
#define PT_STACK      2


struct Pte {

    int32_t flag;
    paddr_t frameNum;
};

struct Pte * pte_create();
void         pte_destroy(struct Pte* pte);
int          probePte(vaddr_t vaddr, struct Pte **rPte);

#endif



