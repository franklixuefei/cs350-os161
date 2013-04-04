#include "opt-A3.h"
#if OPT_A3   

#include <coremap.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <vm.h>
#include <uw-vmstats.h>
#include <thread.h>
#include <curthread.h>
#include <swapfile.h>

static paddr_t start, end;
static int32_t num_entries;
struct coremap* coremap_table;

/* for testing */
void
dumpCoreMap()
{
    kprintf ("[%s]\t%s\t:\t%d\tvalue: %d\n", __FILE__ , __PRETTY_FUNCTION__, __LINE__, 0);
    int i =0;
    for (i = 0; i < num_entries; i++) {
        if (coremap_table[i].occupied == 1) {
            kprintf("entry[%d]:\t %p \t %p \t %p\n", i, coremap_table[i].vaddr, coremap_table[i].addrSpace, coremap_table[i].paddr);
        }
    }
}


int
coremap_insert(paddr_t paddr, vaddr_t vaddr, void* addrSpace)
{
    struct addrspace *addrs = (struct addrspace* )addrSpace;
    int i =0;
    for (i = 0; i < num_entries; i++) {
        if (coremap_table[i].occupied == 0) {
            coremap_table[i].addrSpace = addrs;
            coremap_table[i].paddr = paddr;
            coremap_table[i].vaddr = vaddr;
            coremap_table[i].occupied = 1;
            return 0;
        }
    }
    return -1;
}

void
vm_bootstrap(void)
{

    assert(coremap_table == NULL);
    assert((start&PAGE_FRAME)==start);
    assert((end&PAGE_FRAME)==end);

    u_int32_t ramsize = mips_ramsize();
    num_entries = ramsize/PAGE_SIZE;
    u_int32_t i;

    coremap_table = (struct coremap*)kmalloc(num_entries*sizeof(struct coremap));

    ram_getsize(&start, &end);

    num_entries = (end-start)/PAGE_SIZE;

    for (i = 0; i < num_entries; i++) {
        coremap_table[i].occupied = 0;
        coremap_table[i].length = 0;
        coremap_table[i].t_pid = 0;
        coremap_table[i].addrSpace = NULL;
        coremap_table[i].paddr = 0;
        coremap_table[i].vaddr = 0;
    }
}


void
vm_shutdown()
{
    kfree(coremap_table);
}

int
page_get_rr_victim()
{
    int victim;
    static unsigned int next_page_victim = 0;
    // 0x8badf00d indicates that these pages belong to kernel and should not be swapped
    while(coremap_table[next_page_victim].addrSpace == NULL || 
            coremap_table[next_page_victim].occupied == 0 || 
            coremap_table[next_page_victim].vaddr == 0x8badf00d ) {

        next_page_victim = (next_page_victim + 1) % num_entries;
    }
    victim = next_page_victim;
    next_page_victim = (next_page_victim + 1) % num_entries;
    return victim;
}



paddr_t getppages(unsigned long npages, vaddr_t vaddr) 
{
	int spl;
	paddr_t addr;
	spl = splhigh(); // TODO maybe use a lock instead?
	addr = coremap_table? vm_getppages(npages, vaddr) : ram_stealmem(npages);
	splx(spl);
	return addr;
}
paddr_t vm_getppages(int npages, vaddr_t vaddr)
{

    static unsigned int nextSearchIndex = 0;
    if (npages > num_entries) {
        return 0;
    }
    u_int32_t paddr = 0;
    u_int32_t size = npages * PAGE_SIZE;
    
    if (start + size > end) {
        return 0; 
    }
    u_int32_t i = 0;
    u_int32_t j, k;
    while (i < num_entries) {
        if (coremap_table[i].occupied == 0) {
            for (j=0; j < num_entries && j < npages; j++) {
                if (coremap_table[i+j].occupied != 0) 
                    break;
            }
            if (j == npages) { /* found a place to fit */
                paddr = i * PAGE_SIZE + start;
                for (k=0; k < npages; k++) {
                    coremap_table[i+k].occupied = 1;
                    coremap_table[i+k].length = npages-k;
                    coremap_table[i+k].t_pid = curthread->pid; 
                    // 0x8badf00d will indicate these pages belongs to kernel and shouldnt be swapped
                    if (vaddr != 0x8badf00d) {
                        coremap_table[i+k].vaddr = vaddr+k*PAGE_SIZE;
                    }else{
                        coremap_table[i+k].vaddr = vaddr;
                    }
                    coremap_table[i+k].paddr = paddr;
                    coremap_table[i+k].addrSpace = (void*)curthread->t_vmspace;
//                    kprintf("addr: %p\n", coremap_table[i+k].addrSpace);
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
        int targetPage = page_get_rr_victim();
        int result = swapOut(coremap_table[targetPage].vaddr, (struct addrspace*)(coremap_table[targetPage].addrSpace));
        if (result) {
            return result;
        }
        coremap_table[targetPage].occupied = 0;
        coremap_table[targetPage].vaddr =  0;
        coremap_table[targetPage].length =  0;
        coremap_table[targetPage].addrSpace = 0;
        coremap_table[targetPage].paddr = 0;
        coremap_table[targetPage].t_pid = 0;
        
        //dumpCoreMap();
        return vm_getppages(npages, vaddr); /* now, see if we have a place to fit */
    }
    return paddr;
}

vaddr_t 
alloc_kpages(int npages)
{
    paddr_t pa;
    // 0x8badf00d will indicate these pages belongs to kernel and shouldnt be swapped
    pa = getppages(npages, 0x8badf00d);
    if (pa == 0) {
        return 0;
    }
    return PADDR_TO_KVADDR(pa);
}

void
free_kpages(vaddr_t addr)
{
        paddr_t paddr = KVADDR_TO_PADDR(addr);
        if (paddr > start) {
            int index = (paddr - start) / PAGE_SIZE;
            assert(coremap_table[index].t_pid == curthread->pid);
            int length = coremap_table[index].length;
            int i;
            for (i = index; i < length + index; ++i) {
                coremap_table[i].vaddr = 0;
                coremap_table[i].paddr = 0;
                coremap_table[i].occupied = 0;
                coremap_table[i].length = 0;
                coremap_table[i].t_pid = 0;
                coremap_table[i].addrSpace = NULL;
            }
        }
}

void
coremap_free(void* addrs)
{
    struct addrspace* addrSpace = (struct addrspace *)addrs;
    int i;
    for (i = 0; i < num_entries; ++i) {
        if (coremap_table[i].addrSpace == addrSpace && coremap_table[i].vaddr != 0x8badf00d) {
            coremap_table[i].vaddr = 0;
            coremap_table[i].paddr = 0;
            coremap_table[i].occupied = 0;
            coremap_table[i].length = 0;
            coremap_table[i].t_pid = 0;
            coremap_table[i].addrSpace = NULL;
        }
    }
    swapTableFree(addrSpace);
}

#endif
