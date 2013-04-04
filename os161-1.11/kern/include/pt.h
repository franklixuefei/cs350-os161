#ifndef _PT_H_
#define _PT_H_

#include "opt-A3.h"

#if OPT_A3

#include <types.h>
#include <addrspace.h>

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
int          pageFaultHandler(vaddr_t vaddr, int faulttype, struct Pte *pte, int segNum);
int          allocZeroedPage(vaddr_t vaddr, struct Pte *pte, int segNum);
int          calculate_segment(struct addrspace *as, vaddr_t vaddr, int* rValue);
int          loadPageFromElf(vaddr_t vaddr, struct Pte* pte, int segNum);
int          load_segment(struct vnode *v, off_t offset, vaddr_t vaddr, size_t memsize, size_t filesize, int is_executable);

int calculate_segment (struct addrspace *as, vaddr_t vaddr, int* rValue);
#endif
#endif /* _PT_H_ */


