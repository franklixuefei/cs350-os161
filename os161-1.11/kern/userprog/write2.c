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
    int res;
    char* console = NULL;
    int result = 0;
    struct vnode *vn;
    struct uio copyUIO;

    if (fd < 3 && (curthread->files[fd] == NULL)) {
        pid_t notUsed = 0;
        //////////// read from console//////////////
        char *console = NULL;
        console = kstrdup("con:");
        vn = kmalloc (sizeof(struct vnode));
        result = vfs_open(console,O_RDONLY,&vn);
        if (result){
            *retval = (int32_t)notUsed;
            return -1;
        }
        curthread->files[0] = files_create(kstrdup("con:"),vn, O_RDONLY);

        ////////////// write from console//////////////
        vn = kmalloc (sizeof(struct vnode));
        kfree(console);
        console = kstrdup("con:");
        result = vfs_open(console,O_WRONLY,&vn);
        if (result){
            *retval = (int32_t)notUsed;
            return -1;
        }
        curthread->files[1] = files_create(kstrdup("con:"),vn, O_WRONLY);


        ////////////// write from console//////////////

        vn  = kmalloc (sizeof (struct vnode));
        kfree(console);
        console = kstrdup("con:");
        result = vfs_open(console,O_WRONLY,&vn);
        if (result){
            *retval = (int32_t)notUsed;
            return -1;
        }
        curthread->files[2] = files_create(kstrdup("con:"),vn, O_WRONLY);
        kfree(console);
    }

    if((int)buf <= 0 || (int)(buf + nbytes) <= 0 || (int)(buf + nbytes) >= 0x80000000) {
        *retval = EFAULT;
        return -1;
    }
    void* klebuf = kmalloc (sizeof(nbytes));
    struct files *file = curthread->files[fd];
    copyin (buf, klebuf, nbytes);
    mk_kuio(&copyUIO, klebuf, nbytes, file->offset, UIO_WRITE);
    result = VOP_WRITE (file->vn, &copyUIO);
    kfree(klebuf);



    return 0;
    /*
       int res;
       char* console = NULL;
       int result = 0;
       struct vnode *vn;

       if (fd < 3 &&   (curthread->files[fd] == NULL))  { 
       pid_t notUsed = 0;
    //////////// read from console//////////////
    char *console = NULL;
    console = kstrdup("con:");
    vn = kmalloc (sizeof(struct vnode));
    result = vfs_open(console,O_RDONLY,&vn);
    if (result){
     *retval = (int32_t)notUsed;
     return -1;
     }
     curthread->files[0] = files_create("con:",vn, O_RDONLY);

    ////////////// write from console//////////////
    vn = kmalloc (sizeof(struct vnode));
    kfree(console);
    console = kstrdup("con:");
    result = vfs_open(console,O_WRONLY,&vn);
    if (result){
     *retval = (int32_t)notUsed;
     return -1;
     }
     curthread->files[1] = files_create("con:",vn, O_WRONLY);


    ////////////// write from console//////////////

    vn  = kmalloc (sizeof (struct vnode));
    kfree(console);
    console = kstrdup("con:");
    result = vfs_open(console,O_WRONLY,&vn);
    if (result){
     *retval = (int32_t)notUsed;
     return -1;
     }
     curthread->files[2] = files_create("con:",vn, O_WRONLY);
     kfree(console);
     }

     struct uio copyUIO;
     if (file == NULL) {
     *retval = EBADF;
     return -1; 
     }
     if (((unsigned int)buf) < 0x80000000) {
     *retval = EFAULT;
     return -1; 

     }
     if (((unsigned int)(buf + nbytes)) < 0x80000000 ) {
     *retval = EFAULT;
     return -1; 
     }
     int error =0;
     error = copyin (buf, klebuf, nbytes);
     mk_kuio(&copyUIO, klebuf, nbytes, file->offset, UIO_WRITE);
     result = VOP_WRITE (file->vn, &copyUIO);
     kfree(klebuf);
     *retval = nbytes - copyUIO.uio_resid;
     file->offset += *retval;
     * */
    /*
       switch (fd) {:w

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

     }
     }
     */
}





#endif

