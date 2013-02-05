#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>
#include "opt-A1.h"
#if OPT_A1
#include <asst1/foodcourt.h>
#endif

int NumBowls;  // number of food bowls

#if OPT_A1
struct foodcourt *foodcourt_create(int numBowls) {
	struct foodcourt * foodcourt;
    
    NumBowls = numBowls;
    
	foodcourt = kmalloc(sizeof(struct foodcourt));
	if (foodcourt == NULL) return NULL;
	
    foodcourt->queue_count = 0;
    
    foodcourt->waiting_creatures = q_create(1);
    
    foodcourt->bowlLocks = kmalloc(NumBowls * sizeof(struct lock *));
    if (foodcourt->bowlLocks == NULL) return NULL;
    
    //foodcourt->bowlVacants = kmalloc(NumBowls * sizeof(struct cv *));
    //if (foodcourt->bowlVacants == NULL) return NULL;
    //foodcourt->bowlOccupiers = kmalloc(NumBowls * sizeof(char));
    //if (foodcourt->bowlOccupiers == NULL) return NULL;
    int i;
    for (i = 0; i < NumBowls; ++i) {
        foodcourt->bowlLocks[i] = lock_create("bowl_lock");
        if (foodcourt->bowlLocks[i] == NULL) return NULL;
        //foodcourt->bowlVacants[i] = cv_create("bowl_cv");
        //if (foodcourt->bowlVacants[i] == NULL) return NULL;
        //foodcourt->bowlOccupiers[i] = '-';
    }
    
    foodcourt->numEaters = 0;
    
    foodcourt->currentEater = '-';
    
    //foodcourt->newComer = '-';
    
    foodcourt->foodcourt_lock = lock_create("foodcourt_lock");
    
    foodcourt->foodcourt_accessible = cv_create("foodcourt_cv");
    

    
	return foodcourt;
}

void foodcourt_enter(struct foodcourt *f, char ckind) {
    lock_acquire(f->foodcourt_lock);
    
    if (!q_empty(f->waiting_creatures)) {
        int lastElemIndex = q_getend(f->waiting_creatures) - 1;
        if (lastElemIndex < 0) lastElemIndex += q_getsize(f->waiting_creatures);
        struct creature *c = (struct creature *)q_getguy(f->waiting_creatures,lastElemIndex);
        if (c->kind == ckind) {
            c->count++;
        } else {
            struct creature *creature = creature_create();
            creature->kind = ckind;
            creature->count = 1;
            assert(q_addtail(f->waiting_creatures, creature)==0); // enqueue new comer
            f->queue_count++;
            assert(q_preallocate(f->waiting_creatures, f->queue_count+1)==0);
        }
        // put newcomer into foodcourt line.
        cv_wait(f->foodcourt_accessible, f->foodcourt_lock);
    } else { // waiting creature list is empty now
        if (f->currentEater == '-') {}
        else if (f->currentEater != ckind) {
            // put newcomer into foodcourt line.
            struct creature *creature = creature_create();
            creature->kind = ckind;
            creature->count = 1;
            assert(q_addtail(f->waiting_creatures, creature)==0);
            f->queue_count++;
            assert(q_preallocate(f->waiting_creatures, f->queue_count+1)==0);
            cv_wait(f->foodcourt_accessible, f->foodcourt_lock);
        } else {}
    }

    lock_release(f->foodcourt_lock);
}

void foodcourt_exit(struct foodcourt *f, int bowl) {
    lock_acquire(f->foodcourt_lock);
    
    f->numEaters--;
    // signal for next eater waiting in bowl cv to eat.
    //lock_acquire(f->bowlLocks[bowl-1]);
    //f->bowlOccupiers[bowl-1] = '-';
    //cv_signal(f->bowlVacants[bowl-1], f->bowlLocks[bowl-1]);
    //lock_release(f->bowlLocks[bowl-1]);
    if (f->numEaters == 0) {
        if (q_empty(f->waiting_creatures)) {
            f->currentEater = '-';
        } else {
            int i;
            struct creature *c = q_remhead(f->waiting_creatures);
            f->queue_count--;
            for (i = 0; i < c->count; ++i) {
                cv_signal(f->foodcourt_accessible, f->foodcourt_lock);
            }
            creature_destroy(c);
        }
    }
    
    lock_release(f->foodcourt_lock);
}

void foodcourt_destroy(struct foodcourt *f) {
    assert(q_empty(f->waiting_creatures));
    assert(f->queue_count==0);
    assert(f->currentEater=='-');
    assert(f->numEaters==0);
    assert(f->currentEater=='-');
    int i;
//    for (i = 0; i < NumBowls; ++i) {
//        assert(f->bowlOccupiers[i] == '-');
//    }
    q_destroy(f->waiting_creatures);
    for (i = 0; i < NumBowls; ++i) {
        lock_destroy(f->bowlLocks[i]);
        //cv_destroy(f->bowlVacants[i]);
    }
    lock_destroy(f->foodcourt_lock);
    cv_destroy(f->foodcourt_accessible);
    //kfree(f->bowlOccupiers);
    kfree(f->bowlLocks);
    //kfree(f->bowlVacants);
    kfree(f);
}

struct creature *creature_create() {
    return (struct creature*)kmalloc(sizeof(struct creature));
}

void creature_destroy(struct creature* c) {
    kfree(c);
}

#endif

