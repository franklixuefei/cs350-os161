///*
// * Sample/test code for running a user program.  You can use this for
// * reference when implementing the execv() system call. Remember though
// * that execv() needs to do more than this function does.
// */
//
//#include <types.h>
//#include <kern/unistd.h>
//#include <kern/errno.h>
//#include <lib.h>
//#include <addrspace.h>
//#include <thread.h>
//#include <curthread.h>
//#include <vm.h>
//#include <vfs.h>
//#include <test.h>
//
///*
// * Load program "progname" and start running it in usermode.
// * Does not return except on error.
// *
// * Calls vfs_open on progname and thus may destroy it.
// */
//int
//runprogram(char *progname)
//{
//	struct vnode *v;
//	vaddr_t entrypoint, stackptr;
//	int result;
//
//	/* Open the file. */
//	result = vfs_open(progname, O_RDONLY, &v);
//	if (result) {
//		return result;
//	}
//
//	/* We should be a new thread. */
//	assert(curthread->t_vmspace == NULL);
//
//	/* Create a new address space. */
//	curthread->t_vmspace = as_create();
//	if (curthread->t_vmspace==NULL) {
//		vfs_close(v);
//		return ENOMEM;
//	}
//
//	/* Activate it. */
//	as_activate(curthread->t_vmspace);
//
//	/* Load the executable. */
//	result = load_elf(v, &entrypoint);
//	if (result) {
//		/* thread_exit destroys curthread->t_vmspace */
//		vfs_close(v);
//		return result;
//	}
//
//	/* Done with the file now. */
//	vfs_close(v);
//
//	/* Define the user stack in the address space */
//	result = as_define_stack(curthread->t_vmspace, &stackptr);
//	if (result) {
//		/* thread_exit destroys curthread->t_vmspace */
//		return result;
//	}
//
//	/* Warp to user mode. */
//	md_usermode(0 /*argc*/, NULL /*userspace addr of argv*/,
//		    stackptr, entrypoint);
//
//	/* md_usermode does not return */
//	panic("md_usermode returned\n");
//	return EINVAL;
//}
//

/*
 * Sample/test code for running a user program.  You can use this for
 * reference when implementing the execv() system call. Remember though
 * that execv() needs to do more than this function does.
 */
#include "opt-A2.h"
#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */
int
runprogram(char *progname, char ** args)
{
    struct vnode *v;
    vaddr_t entrypoint, stackptr;
    int result;
    
#if OPT_A2
    vaddr_t *arg_ptrs[MAX_ARGS];
    int argc;
    char**argv;
#endif
    
    /////////////////////////////////////////////////////////////
    //Testing
    ////////////////////////////////////////////////////////////
    
    //kprintf("Size of args * size of(char *): %d\n",sizeof(args) * sizeof(char *));
    //kprintf("nargs = %d\n",nargs);
    
    //int i = 0;
    //for(i = 0; i < nargs; i++)
    //kprintf("args[%d] = %s\n",i,args[i]);
    
    
    ////////////////////////////////////////////////////////////
    
    /* Open the file. */
    result = vfs_open(progname, O_RDONLY, &v);
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
    
#if OPT_A2
    i = 0;
    int j;
    while(argv[i] != NULL) {
        argc++;
    }
    
    if (argc > MAX_ARGS) {
        *retval = E2BIG;
        return -1;
    }
    
    stackptr -= (argc + 1) * sizeof(char*);
    
	for(i = 0; i < argc; ++i){
        j = 0;
		while(argv[i][j] != '\0') {
            j++;
        }
		stackptr -= j;
		arg_ptrs[i] = (char *) stackptr;
		memcpy(arg_ptrs[i],argv[i], j);
	}
	arg_ptrs[i] = NULL; // insert a NULL at the end.
    int remainder = stackptr % 8;
	if(remainder != 0) stackptr -= remainder; // make the stackptr aligned.
    /* Warp to user mode. */
    md_usermode(argc,(userptr_t)arg_ptrs,stackptr, entrypoint);
    
    
#else
    
    /* Warp to user mode. */
    md_usermode(0 /*argc*/, NULL /*userspace addr of argv*/,
                stackptr, entrypoint);
    
#endif
    
    /* md_usermode does not return */
    panic("md_usermode returned\n");
    return EINVAL;
    
}
