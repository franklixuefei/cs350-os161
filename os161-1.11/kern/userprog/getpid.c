/*
 * getpid by Frank Li
 */

#include "opt-A2.h"
#if OPT_A2
#include <syscall.h>
#include <types.h>
#include <thread.h>
#include <curthread.h>

int
sys_getpid(pid_t *retval)
{
    *retval = curthread->pid;
    return 0;
}

#endif
