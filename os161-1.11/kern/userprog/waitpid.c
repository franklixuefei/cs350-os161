/*
 * getpid by Frank Li
 */
#include "opt-A2.h"

#if OPT-A2
#include <syscall.h>
#include <process.h>
#include <thread.h>
#include <curthread.h>
#include <types.h>

int
sys_waitpid(pid_t pid, int *status, int options, pid_t *retval)
{
    int res, exitcode, i, numchildren, child_pid;
    if (pid == 0 || options != 0) {
        *retval = EINVAL;
        return -1;
    }
    struct process child_process = process_table[(int)pid];
    struct process cur_process = process_table[(int)(curthread->pid)];
    if (!process.exitcode || status == NULL) {
        // means process not found or status validity
        *retval = EFAULT;
        return -1;
    }
    
    numchildren = array_getnum(cur_process.children);
    for (i = 0; i < numchildren; ++i) {
        child_pid = (int)array_getguy(cur_process.children, i);
        if (child_pid == pid) {
            *status = child_process.exitcode;
            array_remove(process_table[(int)(curthread->pid)].children, i);
            if (process.active == 0) {
                *retval = pid;
            } else {
                process_table[(int)pid].parentWaiting = 1;
                thread_sleep((vaddr_t *)child_pid);
                
                *retval = pid;
            }
            
            return 0;
        }
    }
    *retval = EINVAL;
    return -1;
}
#endif

