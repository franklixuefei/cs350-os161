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



int sys_read(int fd, void *buf, size_t buflen, int32_t *retval){
    if (fd >= MAX_OPENED_FILES ) {
        *retval = EBADF;
        return -1;
    }
    if (fd < 0) {
        *retval = EBADF;
        return -1; 
    }


    int result =0;
    if (fd < 3 && ((curthread->files[fd] == NULL))) {

        struct vnode *vn =NULL;

        pid_t notUsed = 0;
        char *console = NULL;
        console = kstrdup("con:");
        vn = kmalloc (sizeof(struct vnode));
        result = vfs_open(console,O_RDONLY,&vn);
        if (result){
            *retval = (int32_t)notUsed;
            return -1;
        }
        curthread->files[0] = files_create(kstrdup("con:"),vn, O_RDONLY);

        vn = kmalloc (sizeof(struct vnode));
        kfree(console);
        console = kstrdup("con:");
        result = vfs_open(console,O_WRONLY,&vn);
        if (result){
            *retval = (int32_t)notUsed;
            return -1;
        }
        curthread->files[1] = files_create(kstrdup("con:"),vn, O_WRONLY);



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




    if((int)buf <= 0 || (int)(buf + buflen) <= 0 || (int)(buf + buflen) >= 0x80000000) {
        *retval = EFAULT;
        return -1;
    }

    struct uio copyUIO;
    void *kerBuffer = kmalloc(sizeof (buflen));
    struct files *file = curthread->files[fd];


    mk_kuio(&copyUIO, kerBuffer, buflen, file->offset, UIO_READ);
    result = VOP_READ(file->vn, &copyUIO);
    result = copyout(kerBuffer,buf, buflen);
    

    *retval = buflen - copyUIO.uio_resid;
    file->offset = *retval;
    return 0;


    /* 
       if (fd >= MAX_OPENED_FILES ) {
     *retval = EBADF;
     return -1;
     }
     if (fd < 0) {
     *retval = EBADF;
     return -1; 
     }

     struct uio copyUIO;
     int result;
     void *kerBuffer = kmalloc (sizeof (buflen));
     struct files *file = curthread->files[fd];
     if (file == NULL) {
     *retval = EBADF;
     return -1; 
     }
     copyin(buf, kerBuffer, buflen);
     mk_kuio(&copyUIO, kerBuffer, buflen, file->offset, UIO_READ);
     result = VOP_READ(file->vn, &copyUIO);
    //    if (result == 0) {
    // check if VOP_READ done
    //    }else{
     *retval = buflen - copyUIO.uio_resid;
     file->offset = *retval;
     kfree(kerBuffer);
    //    }


    switch (fd){
    case 0:
    {
    int i =0;
    for (i = 0; i < buflen; i++) {
    ((char*)buf)[i] = getch();
    }
     *retval = buflen;
     *retval = 

     }
     break;
     case 1: // fall though read nothing
     case 2: // fall though read nothing 
     break;
     default:
     {
     struct files *file = curthread->files[fd];
     if (file == NULL) {
     *retval = EBADF;
     return -1; 
     }
     if (((unsigned int)buf) < 0x80000000) {
     *retval = EFAULT;
     return -1; 
     }
     if (((unsigned int)(buf + buflen)) < 0x80000000 ) {
     *retval = EFAULT;
     return -1; 
     }
     mk_kuio(&copyUIO, buf, buflen+1, file->offset, UIO_READ);
     result = VOP_READ(file->vn, &copyUIO);
     if (result == 0) {
    // check if VOP_READ done
    }else{
     *retval = buflen - copyUIO.uio_resid;
     file->offset += *retval;
     }
     }
     }
     */
}






#endif
