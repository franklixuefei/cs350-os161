#include "opt-A2.h"

#if OPT_A2   



#include <types.h>
#include <vm.h>
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
#include <addrspace.h>


int sys_read(int fd, void *buf, size_t buflen, int32_t *retval){
    if (fd >= MAX_OPENED_FILES  || fd < 0) {
        *retval = EBADF;
        return -1;
    }
    
    if (buf <= 0x0 || buf + buflen <= 0x0 || buf >= 0x80000000|| buf + buflen >= 0x80000000 || buf == 0x40000000 || buf + buflen == 0x40000000) {
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



    struct uio *copyUIO = kmalloc(sizeof(struct uio));
    void *kerBuffer = kmalloc(sizeof (buflen));
    struct files *file = curthread->files[fd];
    if (file == NULL) {
        *retval = EBADF;
        return -1;
    }

    mk_kuio(copyUIO, buf, buflen, file->offset, UIO_READ);
    result = VOP_READ(file->vn, copyUIO);
    if (result) {
        return result;
    }
//    result = copyout(kerBuffer,buf, buflen);
    if (result) {
        return result;
    }

    *retval = buflen - copyUIO->uio_resid;
    file->offset += *retval;
    kfree(copyUIO);
    return 0;
}





#endif
