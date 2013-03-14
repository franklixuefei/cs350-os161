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
    struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;
    int argc;
    char**argv;
    
	/* Open the file. */
	result = vfs_open(program, O_RDONLY, &v);
	if (result) {
		return result;
	}
    
	/* We should be a new thread. */
	assert(curthread->t_vmspace == NULL);
    
	/* Create a new address space. */
	curthread->t_vmspace = as_create();
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
    
    
    
    
	/* Warp to user mode. */
	md_usermode(0 /*argc*/, NULL /*userspace addr of argv*/,
                stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}
#endif

