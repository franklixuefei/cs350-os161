#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>
#include <machine/pcb.h>
#include <addrspace.h>
#include <uw-vmstats.h>
#include <coremap.h>
#include <pt.h>
#include <vm.h> 

static struct coremapFrame *coremap = NULL;
static u_int32_t coremap_size;

static paddr_t startpmem;
static paddr_t endpmem;

void coremap_init()
{
	u_int32_t ramsize = mips_ramsize();
	u_int32_t nframes = ramsize/PAGE_SIZE;
	u_int32_t i;

	// Limit ramsize due to properties of MIPS architecture.
  if (ramsize > 508*1024*1024) {
      ramsize = 508*1024*1024;
  }

	coremap = (struct coremapFrame *)kmalloc(nframes*sizeof(struct coremapFrame));

	// Get the bounds for the remaining physical memory.
  ram_getsize(&startpmem, &endpmem);

	// Re-set num_frames now that we know the actual bounds.
  nframes = (endpmem-startpmem)/PAGE_SIZE;
	coremap_size = nframes;


	for (i = 0; i < nframes; i++)
	{
		coremap[i].pid = 0;
		coremap[i].flags = 0;
	}
}

void coremap_destroy()
{
}

paddr_t coremap_getnpages(unsigned long npages)
{
//kprintf("get %d pages\n", npages);
	unsigned long i, j;
	for (i = 0; i < coremap_size; i++)
	{
		if (coremap[i].flags == 0)
		{
			for (j = 1; j < npages; j++) 
				if (coremap[i+j].flags & CM_IN_USE) break;

			if (j == npages)
			{
				for (j = i; j < i + npages; j++)
				{
					coremap[j].pid = curthread->pid;
					coremap[j].flags = (j == i) ? CM_IN_USE : CM_IN_USE | CM_IS_SUBFRAME;
				}
				return i*PAGE_SIZE + startpmem;
			}
		}
	}
	return 0;
}

void coremap_free(paddr_t paddr)
{
	int i;
//kprintf("free paddr: %x\n",paddr);
	i = (paddr - startpmem)/PAGE_SIZE;
	
	assert(coremap[i].pid == curthread->pid);
	//assert(coremap[i].flags == CM_IN_USE);
	do
	{

		coremap[i].flags = 0;
		i = i + 1;
	} while (coremap[i].flags & CM_IS_SUBFRAME);
}

void vm_bootstrap(void)
{
	//coremap_init();
	//vmstats_init();
}

void vm_shutdown(void)
{
	vmstats_print();
}

paddr_t getppages(unsigned long npages)
{
	int spl;
	paddr_t paddr;

	spl = splhigh();
//kprintf("getppages() - get %d pages\n", npages);
	paddr =  ram_stealmem(npages);
//kprintf("getppages() - paddr %x\n", paddr);
	splx(spl);
	return paddr;
}

/* Allocate/free some kernel-space virtual pages */
vaddr_t alloc_kpages(int npages)
{
	paddr_t pa;
//kprintf("alloc_kpages() - alloc %d pages\n",npages);
	pa = getppages(npages);
	if (pa==0) {
		return 0;
	}
	return PADDR_TO_KVADDR(pa);
}

void free_kpages(vaddr_t vaddr)
{
#if abc
	int spl;

		spl = splhigh();

		coremap_free(KVADDR_TO_PADDR(vaddr));


                splx(spl);
#endif
}

