/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>
#include "opt-A1.h"
////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	assert(initial_count >= 0);

	sem = kmalloc(sizeof(struct semaphore));
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	spl = splhigh();
	assert(thread_hassleepers(sem)==0);
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while (sem->count==0) {
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	spl = splhigh();
	sem->count++;
	assert(sem->count>0);
	thread_wakeup(sem);
	splx(spl);
}

////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));
	if (lock == NULL) {
		return NULL;
	}

	lock->name = kstrdup(name);
	if (lock->name == NULL) {
		kfree(lock);
		return NULL;
	}
	#if OPT_A1
	assert(curthread != NULL);
	lock->origThread = NULL;
	lock->held = 0;
	//assert(lock->origThread == NULL);
	//assert(thread_hassleepers(lock) == 0);
	#endif /* OPT_A1 */

	// add stuff here as needed
	
	return lock;
}

void
lock_destroy(struct lock *lock)
{
	//assert(lock != NULL);

	// add stuff here as needed
	#if OPT_A1
        //int spl;
	assert(lock != NULL);
	
	//spl = splhigh();
	assert(!lock->held);
    assert(!lock_do_i_hold(lock));
	assert(lock->origThread == NULL);
	//assert(thread_hassleepers(lock) == 0);
	//splx(spl);
        #endif /* OPT_A1 */
	kfree(lock->name);
	kfree(lock);
	#if OPT_A1
	lock = NULL;
	#endif /* OPT_A1 */
}

void
lock_acquire(struct lock *lock)
{
	// Write this
	#if OPT_A1
	int spl;
	assert(lock != NULL);
	spl = splhigh();
	while(lock->held && !lock_do_i_hold(lock)) {
	   thread_sleep(lock);
	}
	// either no one holds the lock or I hold it.
	assert(!lock->held || (lock->held && lock_do_i_hold(lock)));
	lock->origThread = curthread;
	lock->held = 1;
	splx(spl);
	#else
	(void)lock;  // suppress warning until code gets written
	#endif /* OPT_A1 */
}

void
lock_release(struct lock *lock)
{
	// Write this
	#if OPT_A1
	int spl;
	assert(lock != NULL);
	spl = splhigh();
	assert(lock_do_i_hold(lock));
	lock->origThread = NULL;
	lock->held = 0;
	thread_wakeup(lock);
	splx(spl);
	#else
	(void)lock;  // suppress warning until code gets written
	#endif /* OPT_A1 */
}

int
lock_do_i_hold(struct lock *lock)
{
	// Write this
	#if OPT_A1
	return lock->origThread == curthread;
	#else
	return 1;    // dummy until code gets written
	#endif /* OPT_A1 */
}

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));
	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	#if OPT_A1
	cv->count = 0;
	cv->queue = q_create(1);
	#endif
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	#if OPT_A1
	assert(cv->count==0);
	q_destroy(cv->queue);
	#endif
	kfree(cv->name);
	kfree(cv);
	#if OPT_A1
	cv = NULL;
	#endif
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	// Write this
	#if OPT_A1
	int spl;
	spl = splhigh();
	assert(cv!=NULL && lock!=NULL);
	assert(lock_do_i_hold(lock)); // not neccesary because lock_release() has this assertion inside of it.

	assert(q_addtail(cv->queue, curthread)==0); // enqueue the curthread.
    
    cv->count++;
    
    assert(q_preallocate(cv->queue, cv->count+1)==0); // realloc the queue to bigger size;
	
	
	lock_release(lock); // do this because if a thread called cv_wait, it means that a particular condition
			    // not true.
	thread_sleep(curthread);
	
	lock_acquire(lock);
	splx(spl);
	#else
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
	#endif
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	// Write this
	#if OPT_A1
	int spl;
	spl = splhigh();
	assert(cv!=NULL && lock!=NULL);
	assert(lock_do_i_hold(lock));
	assert(cv->count > 0); // ensure that there are guys awaiting.
	struct thread *front = q_remhead(cv->queue);
	cv->count--;
	thread_wakeup(front);
	splx(spl);
	#else
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
	#endif
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	#if OPT_A1
	int spl;
	spl = splhigh();
	assert(cv!=NULL && lock!=NULL);
	while(cv->count>0) {
	   cv_signal(cv, lock);
	}
	assert(cv->count == 0);
	splx(spl);
	#else
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
	#endif
}


