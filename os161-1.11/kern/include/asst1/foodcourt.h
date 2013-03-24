#ifndef _FOODCOURT_H_
#define _FOODCOURT_H_
#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>
#include "opt-A1.h"
#if OPT_A1
#include <queue.h>
#endif

#if OPT_A1

struct foodcourt {
    volatile struct queue *waiting_creatures; // order corresponding to the order of queue in cv.
    volatile int queue_count;
    volatile struct lock **bowlLocks; // length = NumBowls
	//volatile struct cv **bowlVacants; // length = NumBowls
    //volatile char *bowlOccupiers; // length = NumBowls
	volatile int numEaters;
	volatile char currentEater;
	//volatile char newComer;
	volatile struct lock *foodcourt_lock;
	volatile struct cv *foodcourt_accessible;
};

struct foodcourt *foodcourt_create(int);
void              foodcourt_start(struct foodcourt *, char);
void              foodcourt_end(struct foodcourt *, int);
void              foodcourt_destroy(struct foodcourt *, int);

struct creature {
    volatile char kind;
    volatile int count;
};

struct creature *creature_create();
void   creature_destroy(struct creature*);

#endif /* OPT_A1 */

#endif /* _FOODCOURT_H_*/
