#ifndef _SWAPOPS_H_
#define _SWAPOPS_H_
#include "opt-A3.h"
#if OPT_A3   

#include <types.h>
#include <addrspace.h>

struct swapEntry {
    vaddr_t addr;
    int offset;
    struct addrspace* belongToAddrsapce;
};


int initSwapOps ();
void shutdownSwapOps ();
int swapIn (vaddr_t targetAddr, struct addrspace* targetAddrs);
int swapOut (vaddr_t targetAddr, struct addrspace* addrSpace);

void swapTableFree(struct addrspace* addrs);
#endif

#endif
