#include "opt-A2.h"

#if OPT_A2
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <vnode.h>
#include <uio.h>
#include <syscall.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <vfs.h>
#include <files.h>

int sys_open(const char *filename, int flags, pid_t *retval) {
    if (filename == NULL) {
        return -1;
        *retval = EFAULT;
    }
//    if ((unsigned int)filename >= 0x80000000) {
//        return -1;
//        *retval = EFAULT;
//    }
    if (flags != O_RDONLY && flags != O_WRONLY &&
        flags != O_RDWR && flags != O_CREAT &&
        flags != O_EXCL && flags != O_TRUNC &&
        flags != O_APPEND) {
        kprintf("invalid flags!\n");
        return -1;
        *retval = EINVAL;
    }
    int result = -1;
    struct vnode *vn = NULL;
    char *console = NULL;
    console = kstrdup("con:");
    result = vfs_open(console, flags, &vn);
    if (result) {
        kprintf("cannot open the file!\n");
        *retval = ENOENT; //not sure about this errno
        return -1;
    }
    struct files *file = files_create(filename, vn, flags);
    if (file == NULL) {
        *retval = EFAULT;
        return  -1;
    }
    int i = 3;
    while (curthread->files[i] != NULL) {
        i++;
    }
    if (i >= MAX_OPENED_FILES) {
        kprintf("file table is full!\n");
        *retval = ENFILE;
        return -1; 
    }
    file->fd = i;
    curthread->files[i] = file;
    *retval = i;
    return 0;
}
#endif
