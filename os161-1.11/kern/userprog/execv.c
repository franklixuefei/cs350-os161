#include "opt-A2.h"

#if OPT_A2
#include <kern/errno.h>
#include <types.h>
#include <thread.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <lib.h>
#include <vnode.h>
#include <uio.h>
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
#include <kern/unistd.h>


int
sys_execv(const char* program, char **args, pid_t *retval)
{
    if (program <= 0x0 || program >= 0x80000000 || program == 0x40000000 || args <= 0x0 || args >= 0x80000000 || args == 0x40000000) {
        *retval = EFAULT;
        return -1;
    }
    int i = 0;
    while(args[i] != NULL) {
        if (args[i] <= 0x0 || args[i] >= 0x80000000 || args[i] == 0x40000000) {
            *retval = EFAULT;
            return -1;
        }
        i++;
    }
    
    if (strlen(program) == 0) {
        *retval = EINVAL;
        return -1;
    }
    
    struct vnode *v;
    vaddr_t entrypoint, stackptr;

    int result;
    int argc;
    int padding;
    
    /* Open the file. */
    result = vfs_open(program, O_RDONLY, &v);
    if (result) {
        return result;
    }
    
    /* If current thread's address space is not disposed,  dispose it. */
    if (curthread->t_vmspace != NULL) {
        as_destroy(curthread->t_vmspace);
    }
    
    /* We should be a new thread. */
    // assert(curthread->t_vmspace == NULL);
    
    /* Create a new address space. */
    curthread->t_vmspace = as_create(program);
    if (curthread->t_vmspace==NULL) {
        vfs_close(v);
        return ENOMEM;
    }
    
    /* Activate it. */
    as_activate(curthread->t_vmspace);
    
    /* Load the executable. */
    result = load_elf(v, &entrypoint);
    if (result) {
        /* thread_exit destroys curthread->t_vmspace */
        vfs_close(v);
        return result;
    }
    
    /* Done with the file now. */
    vfs_close(v);
    
    /* Define the user stack in the address space */
    result = as_define_stack(curthread->t_vmspace, &stackptr);
    if (result) {
        /* thread_exit destroys curthread->t_vmspace */
        return result;
    }
    
    
    int j;
    while(args[i] != NULL) {
        argc++;
        i++;
    }
    
    if (argc > MAX_ARGS) {
        *retval = E2BIG;
        return -1;
    }
    
    stackptr -= (argc + 1) * sizeof(char*);
    char**arg_ptrs = (char**)stackptr;
    for(i = 0; i < argc; ++i){
        j = 0;
        while(args[i][j] != '\0') {
            j++;
        }
        j++; // including '\0'
        stackptr -= j;
        arg_ptrs[i] = (char *) stackptr;
        copyoutstr((const char*)args[i], (userptr_t)arg_ptrs[i], j, 0);
    }
    padding = stackptr % 8;
    stackptr -= padding; // make the stackptr aligned.
    /* Warp to user mode. */
    md_usermode(argc,(userptr_t)arg_ptrs,stackptr, entrypoint);
    
    /* md_usermode does not return */
    panic("md_usermode returned\n");
    return EINVAL;
}
#endif
