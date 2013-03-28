#ifndef _COREMAP_H_
#define _COREMAP_H_

#include "opt-A3.h"

#if OPT_A3   

#include <types.h>
struct coremap {
    vaddr_t vaddr;
    paddr_t paddr;
    char occupied;    
    int length;
};

void vm_bootstrap(void);
paddr_t vm_getppages(int npages);
vaddr_t alloc_kpages(int npages);
void free_kpages(vaddr_t addr);

#endif

#endif /* _COREMAP_H_ */
