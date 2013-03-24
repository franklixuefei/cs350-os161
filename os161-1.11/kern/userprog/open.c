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
#include <kern/limits.h>

int sys_open(const char *filename, int flags, pid_t *retval) {
    if (filename <= 0x0 || filename >= 0x80000000 || filename == 0x40000000) {
        *retval = EFAULT;
        return -1;
    }
    if (strlen(filename) == 0) {
        *retval = EINVAL;
        return -1;
    }

    if (flags != O_RDONLY && flags != O_WRONLY && flags != O_RDWR &&
        
        flags != (O_RDONLY|O_CREAT) && flags != (O_RDONLY|O_EXCL) && flags != (O_RDONLY|O_TRUNC) && flags != (O_RDONLY|O_APPEND) &&
        flags != (O_WRONLY|O_CREAT) && flags != (O_WRONLY|O_EXCL) && flags != (O_WRONLY|O_TRUNC) && flags != (O_WRONLY|O_APPEND) &&
        flags != (O_RDWR|O_CREAT) && flags != (O_RDWR|O_EXCL) && flags != (O_RDWR|O_TRUNC) && flags != (O_RDWR|O_APPEND) &&
        
        flags != (O_RDONLY|O_CREAT|O_EXCL) && flags != (O_RDONLY|O_CREAT|O_TRUNC) && flags != (O_RDONLY|O_CREAT|O_APPEND) &&
        flags != (O_RDONLY|O_EXCL|O_TRUNC) && flags != (O_RDONLY|O_EXCL|O_APPEND) &&
        flags != (O_RDONLY|O_TRUNC|O_APPEND) &&
        
        flags != (O_WRONLY|O_CREAT|O_EXCL) && flags != (O_WRONLY|O_CREAT|O_TRUNC) && flags != (O_WRONLY|O_CREAT|O_APPEND) &&
        flags != (O_WRONLY|O_EXCL|O_TRUNC) && flags != (O_WRONLY|O_EXCL|O_APPEND) &&
        flags != (O_WRONLY|O_TRUNC|O_APPEND) &&
        
        flags != (O_RDWR|O_CREAT|O_EXCL) && flags != (O_RDWR|O_CREAT|O_TRUNC) && flags != (O_RDWR|O_CREAT|O_APPEND) &&
        flags != (O_RDWR|O_EXCL|O_TRUNC) && flags != (O_RDWR|O_EXCL|O_APPEND) &&
        flags != (O_RDWR|O_TRUNC|O_APPEND)) {
//        kprintf("invalid flags!\n");
        *retval = EINVAL;
        return -1;
    }
    struct vnode *vn = kmalloc(sizeof(struct vnode));
    int result = vfs_open(kstrdup(filename), flags, &vn);
    if (result) { 
        *retval = EFAULT;
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
