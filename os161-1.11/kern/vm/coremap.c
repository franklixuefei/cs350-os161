#include <coremap.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <vm.h>
#include <uw-vmstats.h>

static paddr_t start, end;
static int32_t num_entries;
static struct coremap* coremap_table;

void
vm_bootstrap(void)
{
    //vmstats_init();
//    ram_getsize(&start, &end);
    assert(coremap_table == NULL);
    assert((start&PAGE_FRAME)==start);
    assert((end&PAGE_FRAME)==end);
    /*
    num_entries = (end - start) / (PAGE_SIZE+sizeof(struct coremap));
    int remainder = (end - start) % (PAGE_SIZE+sizeof(struct coremap));
    coremap_table = (struct coremap*)PADDR_TO_KVADDR(start);
    start += num_entries * sizeof(struct coremap);
    // do we have to update the start paddr to an int?????
    assert(start + num_entries * PAGE_SIZE+remainder == end);
    */
    u_int32_t ramsize = mips_ramsize();
    num_entries = ramsize/PAGE_SIZE;
    u_int32_t i;

    coremap_table = (struct coremap*)kmalloc(num_entries*sizeof(struct coremap));

    ram_getsize(&start, &end);

    num_entries = (end-start)/PAGE_SIZE;


    for (i = 0; i < num_entries; i++) {
        coremap_table[i].occupied = 0;
        coremap_table[i].length = 0;
    }
}

paddr_t
getppages(unsigned long npages)
{
	int spl;
	paddr_t addr;
	spl = splhigh();
//	addr = ram_stealmem(npages);
	addr = coremap_table? vm_getppages(npages) : ram_stealmem(npages);
	splx(spl);
	return addr;
}
paddr_t
vm_getppages(int npages)
{
    u_int32_t paddr = 0;
    u_int32_t size = npages * PAGE_SIZE;
    
    if (start + size > end) {
        return 0; // FIXME
    }
    u_int32_t i = 0;
    u_int32_t j, k;
    while (i < num_entries) {
        if (coremap_table[i].occupied == 0) {
            for (j=0; j < num_entries && j < npages; j++) {
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
    pa = getppages(npages);
    if (pa == 0) {
        return 0; // not sure!!!!!
    }
    return PADDR_TO_KVADDR(pa);
}

void
free_kpages(vaddr_t addr)
{
	 //nothing 
    
	(void)addr;
}


