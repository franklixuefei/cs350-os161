/*
 * getpid by Frank Li
 */
#include "opt-A2.h"

#if OPT_A2
#include <syscall.h>
#include <process.h>
#include <thread.h>
#include <curthread.h>
#include <types.h>
#include <kern/errno.h>
#include <machine/spl.h>
#include <kern/unistd.h>

int
sys_waitpid(pid_t pid, int *status, int options, pid_t *retval)
{
    int res, exitcode, i, numchildren, child_pid, spl;

    int found = 0;
    
    if (status <= 0x0 || status >= 0x80000000 || status == 0x40000000 || (int)status % sizeof(char*) != 0) {
        *retval = EFAULT;
        return -1;
    }
    if (pid <= 0 || pid > MAX_FORKED_PROCESSES || options != 0) {
        *retval = EINVAL;
        return -1;
    }
    
    if (process_table[(int)(curthread->pid)].children == NULL) { // I'm a child
        *retval = EFAULT;
        return -1;
    }
    
    struct process cur_process = process_table[(int)(curthread->pid)];
    struct process child_process = process_table[(int)pid];
    
    numchildren = array_getnum(cur_process.children);
    
    //spl = splhigh();
    for (i = 0; i < numchildren; ++i) {
        child_pid = (int)array_getguy(cur_process.children, i);
        //kprintf("passed_in_pid: %d, fetched_pid: %d \n", pid, child_pid);
        
        if (child_pid == (int)pid) {
            found = 1;
            break;
        }
        
    }
    if (found) {
        
        array_remove(process_table[(int)(curthread->pid)].children, i);
        //            if (process.active == 0) {
        //                *retval = pid;
        //            } else {
        //                process_table[(int)pid].parentWaiting = 1;
        //                thread_sleep((vaddr_t *)child_pid);
        //
        //                *retval = pid;
        //            }
        //if (child_process.active == 0) {
            //process_table[(int)pid].parentWaiting = 1;
            //if (process_table[(int)(curthread->pid)].processSem == NULL) process_table[(int)(curthread->pid)].processSem = sem_create("process_sem", 0);
        P(process_table[(int)pid].processSem);
//        kprintf("exitcode: %d\n", child_process.exitcode);
        *status = process_table[(int)pid].exitcode;
        //}
        *retval = pid;
        process_table[(int)pid].parentWaiting = 0;
        return 0;
    }
    
    *retval = EINVAL;
    return -1;
}
#endif

