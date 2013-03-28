#include <coremap.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <vm.h>
#include <uw-vmstats.h>

paddr_t start, end;
int32_t num_entries;
struct coremap* coremap_table;

void
vm_bootstrap(void)
{
    vmstats_init();
    ram_getsize(&start, &end);
    num_entries = (end - start) / (PAGE_SIZE+sizeof(struct coremap));
    coremap_table = (struct coremap*)PADDR_TO_KVADDR(start);
    start += num_entires * sizeof(struct coremap);
    // do we have to update the start paddr to an int?????
    assert(start + num_entires * PAGE_SIZE == lastaddr);
    u_int32_t i;
    for (i = 0; i < num_entires; i++) {
        coremap_table[i].occupied = 0;
        coremap_table[i].length = 0;
    }
    
}

paddr_t
vm_getppages(int npages)
{
    //lock or diabling interrupt here???
    u_int32_t paddr = 0;
    u_int32_t size = npages * PAGE_SIZE;
    
    if (start + size > end) {
        return 0; // FIXME
    }
    u_int32_t i = 0;
    u_int32_t j, k;
    while (i < num_entires) {
        if (coremap_table[i].occupied == 0) {
            for (j=0; j < num_entires && j < npages, j++) {
                if (coremap_table[i+j].occupied != 0) 
                    break;
            }
            if (j == npages) {
                paddr = i * PAGE_SIZE + start;
                for (k=0; k < npages; k++) {
                    coremap_table[i+k].occupied = 1;
                    coremap_table[i+k].length = npages-k;
                }
                break;
            }
            else {
                i+=j;
            }
        }
        else {
            i+= coremap_table[i].length;
        }
    }
    if (paddr == 0) {
        kprintf("out of memory!\n");
        //do page replacement here!!!!!
    }
    return paddr;
}

vaddr_t 
alloc_kpages(int npages)
{
    paddr_t pa;
    pa = vm_getppages(npages);
    if (pa == 0) {
        return 0; // not sure!!!!!
    }
    return PADDR_TO_KVADDR(pa);
}

paddr_t get1page() {

}
