/*
 * =====================================================================================
 *
 *       Filename:  pidStore.h
 *
 *    Description:  manage PIDs for processes
 *
 *        Version:  1.0
 *        Created:  02/17/2013 12:24:19 AM
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

#include "queue.h"
#include <machine/pcb.h>
#include <synch.h>

#define MAX_CHILD_PROCESSES_COUNT 10
#define MAX_FILE_ABLE_HOLD_COUNT 10

struct pidStore{
    
    struct queue* usedPIDs;
    struct lock* usedPIDsLock;
    
    pid_t currentPID;
    struct lock* currentPIDLock;
}

pid_t
safeGetPID ();

void
safeReturnPID(pid_t usedPID);



#endif
