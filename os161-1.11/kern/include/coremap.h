#ifndef _COREMAP_H_
#define _COREMAP_H_

#include "opt-A3.h"

#if OPT_A3   

#include <types.h>

extern struct coremap* coremap_table;


struct coremap {
    vaddr_t vaddr;
    paddr_t paddr;
    char occupied;    
    int length;
    pid_t t_pid;
    void* addrSpace;
};

void vm_bootstrap(void);
paddr_t vm_getppages(int npages, vaddr_t vaddr);
paddr_t getppages(unsigned long npages, vaddr_t vaddr);
vaddr_t alloc_kpages(int npages);
void free_kpages(vaddr_t addr);
int disableReadForPte (vaddr_t vaddr,void* addrS, paddr_t paddr); /* report inserted pte back to pte */
void vm_shutdown();
int enableReadForPte (vaddr_t vaddr,void* addrS, paddr_t paddr); /* report inserted pte back to pte */

void cormap_free(void* addrSpace);

int coremap_insert(paddr_t paddr, vaddr_t vaddr, void* addrSpace);
#endif

#endif /* _COREMAP_H_ */
