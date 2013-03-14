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

int
withInRange (vaddr_t addr, vaddr_t from, vaddr_t to){
    return ((addr > from) && (addr < to));
}


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
    void *kerBuffer = kmalloc(sizeof (buflen));
    struct files *file = curthread->files[fd];
    vaddr_t insbase1from = curthread->t_vmspace->as_vbase1;
    vaddr_t insbase1to = curthread->t_vmspace->as_npages1 * PAGE_SIZE + insbase1from;

    vaddr_t datavbasefrom = curthread->t_vmspace->as_vbase2;
    vaddr_t datavbaseto = curthread->t_vmspace->as_npages2 * PAGE_SIZE + datavbaseto;

    vaddr_t stackvbasefrom = curthread->t_vmspace->as_vbase1 - PAGE_SIZE*12;
    vaddr_t stackvbaseto = curthread->t_vmspace->as_vbase1;

    if (buf < 0x8000000 &&(
            withInRange(buf, insbase1from, insbase1to) ||
            withInRange(buf, datavbasefrom, datavbaseto) ||
            withInRange(buf, stackvbasefrom, stackvbaseto))
            ) {
        *retval = EFAULT;
        return -1;  
    }

    mk_kuio(&copyUIO, kerBuffer, buflen, file->offset, UIO_READ);
    result = VOP_READ(file->vn, &copyUIO);
    if (result) {
        return result;
    }
    result = copyout(kerBuffer,buf, buflen);
    if (result) {
        return result;
    }

    *retval = buflen - copyUIO.uio_resid;
    file->offset = *retval;
    return 0;
}





#endif
