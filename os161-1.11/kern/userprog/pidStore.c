#include "opt-A2.h"

#if OPT_A2


#include <types.h>
#include <lib.h>
#include <synch.h>
#include <pidStore.h>
#define MAX_AMOUNT_PID 100

struct pidStore *myStore = NULL;

struct lock* storeBusy = NULL;

pid_t
safeGetPID(){
    if (!storeBusy) {
        storeBusy = lock_create("Store Busy");
    }
    lock_acquire(storeBusy);
    
    if (myStore == NULL) {
        myStore = kmalloc(sizeof(struct pidStore));
        myStore->usedPIDs = q_create(MAX_AMOUNT_PID);
        myStore->usedPIDsLock = lock_create("used pid lock");
        myStore->currentPID = 0;
        myStore->currentPIDLock = lock_create("current PID count");
    }
    
    
    
    
    pid_t newPID = -1;
    lock_acquire(myStore->usedPIDsLock);
    if (!q_empty(myStore->usedPIDs)) {
        struct pidStore* usedPID = NULL;
        usedPID = q_remhead(myStore->usedPIDs);
        lock_release(myStore->usedPIDsLock);
        newPID = *(pid_t *)usedPID;
        kfree(usedPID);
    }else{
        lock_release(myStore->usedPIDsLock);
        lock_acquire(myStore->currentPIDLock);
        newPID = myStore->currentPID;
        myStore->currentPID++;
        lock_release(myStore->currentPIDLock);
    }
    lock_release(storeBusy);
    return newPID;
    
}


void
safeReturnPID(pid_t usedPID){
    if (!storeBusy) {
        storeBusy = lock_create("Store Busy");
    }
    lock_acquire(storeBusy);
    
    lock_acquire(myStore->currentPIDLock);
    if (myStore->currentPID -1 == usedPID) {
        myStore->currentPID --;
        lock_release(myStore->currentPIDLock);
    }else{
        lock_release(myStore->currentPIDLock);
        pid_t * aboutToInsertPID = kmalloc(sizeof(pid_t));
        *aboutToInsertPID = usedPID;
        lock_acquire(myStore->usedPIDsLock);
        q_addtail(myStore->usedPIDs, aboutToInsertPID);
        lock_release(myStore->usedPIDsLock);
    }
    lock_release(storeBusy);
}


#endif
