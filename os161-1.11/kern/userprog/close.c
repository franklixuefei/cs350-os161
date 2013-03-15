
#include "opt-A2.h"

#if OPT_A2
#include <types.h>
#include <lib.h>
#include <kern/errno.h>
#include <thread.h>
#include <curthread.h>
#include <vnode.h>
#include <uio.h>
#include <syscall.h>
#include <kern/unistd.h>
#include <vfs.h>
#include <files.h>


int sys_close(int fd,  int32_t *retval) {
    if (fd >= MAX_OPENED_FILES || fd < 3) {
        *retval = EBADF; 
        kprintf("invalid fd!\n");
        return  -1;
    }
    if (curthread->files[fd] == NULL) {
        *retval = EBADF; 
        kprintf("file does not exist!\n");
        return  -1;
    }
    vfs_close(curthread->files[fd]->vn);
    //files_destroy(curthread->files[fd]);
    curthread->files[fd] = NULL;
    return 0;
}
#endif
