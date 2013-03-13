#ifndef _FILES_H_
#define _FILES_H_

struct files {
    char *filename;
    struct vnode* vn;
    int flags;
    int offset;
    int fd;
};

struct files* files_create(const char* filename, struct vnode* vn, int flags);

void files_destroy(struct files *files);

#endif /* _FILES_H_ */
