/*
 * =====================================================================================
 *
 *       Filename:  pidStore.c
 *
 *    Description:  manage PIDs for Processes
 *
 *        Version:  1.0
 *        Created:  02/17/2013 12:32:00 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yongxin
 *   Organization:
 *
 * =====================================================================================
 */

#include "opt-A2.h"

#if OPT_A2

#include <stdlib.h>
#include <types.h>
#include <pidStore.h>
#define MAX_AMOUNT_PID 100

struct pidStore *myStore = NULL;

lock* storeBusy = lock_create("Store Busy");

pid_t
safeGetPID(){
    lock_acquire(storeBusy);
    
    if (myStore == NULL) {
        myStore = kmalloc(sizeof(pidStore));
        myStore->usedPIDs = q_create(MAX_AMOUNT_PID);
        myStore->usedPIDsLock = lock_create("used pid lock");
        myStore->currentPID = 0;
        myStore->currentPIDLock = lock_create("current PID count");
    }
    
    
    
    
    pid_t newPID = -1;
    lock_acquire(myStore->usedPIDsLock);
    if (!q_empty(myStore->usedPIDs)) {
        pidStore* usePID = NULL;
        usedPID = q_remhead(myStore->usedPIDs);
        lock_release(myStore->usedPIDsLock);
        newPID = *usedPIDs;
        kfree(usedPID);
    }else{
        lock_release(myStore->usedPIDsLock);
        lock_acquire(myStore->currentPIDLock);
        newPID = currentPID;
        currentPID++;
        lock_release(myStore->currentPIDLock);
    }
    lock_release(storeBusy);
    return newPID;
    
}


void
safeReturnPID(pid_t usedPID){
    
    lock_acquire(storeBusy);
    
    lock_acquire(myStore->currentPIDLock);
    if (myStore->currentPID -1 == usedPID) {
        myStore->currentPID --;
        lock_release(myStore->currentPIDLock);
    }else{
        lock_release(myStore->currentPIDLock);
        pid_t * aboutToInsertPID = kmalloc(sizeof(pid_t));
        *aboutToInsertPID = usePID;
        lock_acquire(myStore->usedPIDsLock);
        q_addtail(myStore->usedPIDs, aboutToInsertPID);
        lock_release(myStore->usedPIDsLock);
    }
    lock_release(storeBusy);
}


#endif
