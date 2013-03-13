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
#include <vfs.h>
#include <files.h>

#include <kern/errno.h>

int sys_write(int fd, const void *buf, size_t nbytes, int32_t *retval)
{
    if (fd >= MAX_OPENED_FILES ) {
        *retval = EBADF;
        return -1;
    }
    if (fd < 0) {
        *retval = EBADF;
        return -1;
    }
/* 
    int res;
    console = kstrdup("console-stdin:");
    curthread->files[0] = kmalloc(sizeof(struct files));
    if (curthread->files[0] == NULL) {
        *retval = EBADF;
        return -1;
    }
    curthread->files[0]->offset = 0;
    curthread->files[0]->vn = NULL;
    res = vfs_open(console, O_RDONLY, &curthread->files[0]->vn);
    if (res) {
        *retval = res;
        return -1;
    }
    console = kstrdup("console-stdout:");
    curthread->files[1] = kmalloc(sizeof(struct files));
    if (curthread->files[1] == NULL) {
        *retval = EBADF;
        return -1;
    }
    curthread->files[1]->offset = 0;
    curthread->files[1]->vn = NULL;
    res = vfs_open(console, O_WRONLY, &curthread->files[1]->vn);
    if (res) {
        *retval = res;
        return -1;
    }
    console = kstrdup("console-stderr:");
    curthread->files[2] = kmalloc(sizeof(struct files));
    if (curthread->files[2] == NULL) {
        *retval = EBADF;
        return -1;
    }
    curthread->files[2]->offset = 0;
    curthread->files[2]->vn = NULL;
    res = vfs_open(console, O_WRONLY, &curthread->files[2]->vn);
    if (res) {
        *retval = res;
        return -1;
    }
    kfree(console);
 * */

    struct uio copyUIO;
    int result;

    switch (fd) {
        case 0:
            *retval = EBADF;
            return -1;
        case 1:
        case 2:
            {
                int i = 0;
                for (i = 0; i < nbytes; i++) {
                    putch(((char*)buf)[i]);
                }
            }
            break;
        default:
            {
                struct files *file = curthread->files[fd];
                if (file == NULL) {
                    *retval = EBADF;
                    return -1; 
                }
                /* 
                   if (((unsigned int)buf) < 0x80000000) {
                 *retval = EFAULT;
                 return -1; 

                 }
                 if (((unsigned int)(buf + nbytes)) < 0x80000000 ) {
                 *retval = EFAULT;
                 return -1; 
                 }
                 * */

                mk_kuio(&copyUIO, buf, nbytes, file->offset, UIO_WRITE);
                result = VOP_WRITE (file->vn, &copyUIO);
                if (result == 0) {
                    // check if VOP_WRITE done
                }else{
                    *retval = nbytes - copyUIO.uio_resid;
                    file->offset += *retval;
                }
            }
    }
    return 0;
}





#endif
