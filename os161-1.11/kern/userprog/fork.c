/*
 * getpid by Frank Li
 */
#include "opt-A2.h"

#if OPT_A2
#include <kern/errno.h>
#include <types.h>
#include <thread.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <lib.h>
#include <vnode.h>
#include <vfs.h>
#include <addrspace.h>
#include <syscall.h>
#include <test.h>
#include <synch.h>
#include <syscall.h>
#include <process.h>
#include <array.h>
#include <curthread.h>
#include <files.h>
#include <machine/trapframe.h>

int
sys_fork(struct trapframe* tf, pid_t *retval)
{
    struct thread *child = NULL;
    
    //struct addrspace *t_vmspace = NULL;
    
    struct trapframe *t_tf = NULL;
    
    int i;
    for(i = 0; i < MAX_FORKED_PROCESSES; ++i) {
        if (process_table[i].active == 0) break; // indicating not full.
    }
    
    if (i >= MAX_FORKED_PROCESSES) {
        *retval = EAGAIN;
        return -1;
    }
    
    t_tf = (struct trapframe *)kmalloc(sizeof(struct trapframe));
    
    if (t_tf == NULL) {
        *retval = ENOMEM;
        return -1;
    }
    int res;
//    res = as_copy(curthread->t_vmspace, &t_vmspace);
//    if (res) {
//        return res;
//    }
    
    memcpy(t_tf, tf, sizeof(struct trapframe));
    
    curthread->forkcalled = 1;
    res = thread_fork("child", t_tf, 0, md_forkentry, &child);
    if (res) { // fails
        return res;
    } else {
        *retval = child->pid;
    }
    
//    // copy filetable
        if (process_table[(int)(curthread->pid)].children == NULL) {
        process_table[(int)(curthread->pid)].children = array_create();
        if (process_table[(int)(curthread->pid)].children == NULL) {
            *retval = ENOMEM;
            return -1;
        }
    }
    res = array_add(process_table[(int)(curthread->pid)].children,(int *)child->pid);
    if(res) {
        return res;
    }
    
    
    return 0;
    
}
#endif

