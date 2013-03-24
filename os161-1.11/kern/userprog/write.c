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
#include <addrspace.h>

#include <kern/errno.h>



int sys_write(int fd, const void *buf, size_t nbytes, int32_t *retval)
{
    if (fd < 0) {
        *retval = EBADF;
        return -1;
    }
    if (fd >= MAX_OPENED_FILES) {
        *retval = EBADF;
        //kprintf("invalid fd!\n");
        return  -1;
    }

    if (buf <= 0x0 || buf + nbytes <= 0x0 || buf >= 0x80000000|| buf + nbytes >= 0x80000000 || buf == 0x40000000 || buf + nbytes == 0x40000000) {
        *retval = EFAULT;
        return -1;
    }


    int result =0;
    if (fd < 3 && ((curthread->files[fd] == NULL))) {
        struct vnode *vn =NULL;
        pid_t notUsed = 0;
        char *consolein = NULL;
        consolein = kstrdup("con:");
        vn = kmalloc (sizeof(struct vnode));
        result = vfs_open(consolein,O_RDONLY,&vn);
        if (result){
            *retval = (int32_t)notUsed;
            return -1;
        }
        curthread->files[0] = files_create(kstrdup("con:"),vn, O_RDONLY);
        curthread->files[1] = files_create(kstrdup("con:"),vn, O_WRONLY);
        curthread->files[2] = files_create(kstrdup("con:"),vn, O_WRONLY);
        kfree(consolein);
    }

    struct uio copyUIO;

    switch (fd) {
        case 0:
            *retval = EBADF;
            return -1;
        case 1:
        case 2:
            {
                int i;
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

                //                void* klebuf = kmalloc (sizeof(nbytes));
                //                copyin (buf, klebuf, nbytes);
                mk_kuio(&copyUIO, (void*)buf, nbytes, file->offset, UIO_WRITE);
                result = VOP_WRITE (file->vn, &copyUIO);
                if (result) {
                    return result;
                }
                //kfree(klebuf);
                /*

                   mk_kuio(&copyUIO, buf, nbytes, file->offset, UIO_WRITE);
                   result = VOP_WRITE (file->vn, &copyUIO);
                   if (result == 0) {
                // check if VOP_WRITE done
                }else{
                }
                */
                *retval = nbytes - copyUIO.uio_resid;
                file->offset += *retval;
            }
    }
    return 0;
}





#endif
