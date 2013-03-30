#ifndef _COREMAP_H_
#define _COREMAP_H_

#define CM_IN_USE 			0x0001
#define CM_IS_SUBFRAME	0x0010

struct coremapFrame {
	int pid;
	short int flags;
};

void coremap_init(void);
void coremap_destroy(void);
paddr_t coremap_getnpages(unsigned long npages);
void coremap_free(paddr_t paddr);

#endif /* _COREMAP_H_ */

