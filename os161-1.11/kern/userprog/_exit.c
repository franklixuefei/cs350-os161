/*
 * _exit by Frank Li
 */
#include "opt-A2.h"

#if OPT_A2
#include <syscall.h>
#include <thread.h>
#include <files.h>
#include <vfs.h>
#include <curthread.h>
#include <process.h>
#include <types.h>

void
sys__exit(int exitcode) {
    // pass exitcode back into curthread.
    process_table[curthread->pid].exitcode = exitcode;
    // FIXME close all open files and free the file table entries
    int i;
    for (i=0; i<MAX_OPENED_FILES; ++i) {
        if (curthread->files[i]) {
            vfs_close(curthread->files[i]->vn);
            files_destroy(curthread->files[i]);
        }
    }
    process_table[(int)(curthread->pid)].active = 0;
    if (process_table[(int)(curthread->pid)].parentWaiting) {
        thread_wakeup((vaddr_t *)curthread->pid); // for waitpid, wake up the parent
    }
    thread_exit();
}

#endif

