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
/*
 * Dining room encapsulation.
 * Operations:
 *     foodcourt_create:
 *
 *     foodcourt_set_newcomer: 
 *     
 *     
 *
 *
 */

// we dont need queue because if there are cats eating, mice will be cv'ed and they will auto
//matically enter the queue built in cv.

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
void              foodcourt_enter(struct foodcourt *, char);
void		      foodcourt_dine(struct foodcourt *, int, char);
void              foodcourt_exit(struct foodcourt *, int);
void              foodcourt_destroy(struct foodcourt *);

struct creature {
    volatile char kind;
    int count;
};

#endif /* OPT_A1 */

#endif /* _FOODCOURT_H_*/
