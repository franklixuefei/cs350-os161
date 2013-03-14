#include "opt-A2.h"
#include <types.h>
#include <lib.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <array.h>
#include <thread.h>
#include <curthread.h>
#include <vnode.h>
#include <uio.h>
#include <files.h>
#if OPT_A2

struct files * files_create(const char* filename, struct vnode *vn, int flags) {
    struct files *file = kmalloc(sizeof(struct files));
    if (file == NULL) {
        return NULL;
    }
    file->filename = (char*)filename;
    file->vn = vn;
    file->flags = flags;
    file->offset = 0;
    file->fd = -1;
    return file;
}

void files_destroy(struct files *files) {
    if (files != NULL) {
        kfree(files->filename);
        if (files->vn->vn_refcount == 1 && files->vn->vn_refcount == 0) vnode_kill(files->vn);
        kfree(files);
    }
}


#endif
