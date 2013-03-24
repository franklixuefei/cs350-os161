#include "opt-A2.h"

#if OPT_A2

#include "queue.h"
#include <types.h>
#include <synch.h>

#define MAX_CHILD_PROCESSES_COUNT 10
#define MAX_FILE_ABLE_HOLD_COUNT 10

struct pidStore{
    
    struct queue* usedPIDs;
    struct lock* usedPIDsLock;
    
    pid_t currentPID;
    struct lock* currentPIDLock;
};

pid_t
safeGetPID ();

void
safeReturnPID(pid_t usedPID);



#endif
#endif
