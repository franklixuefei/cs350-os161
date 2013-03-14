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
#include <vnode.h>

void
sys__exit(int exitcode) {
    // pass exitcode back into curthread.
    process_table[curthread->pid].exitcode = exitcode;
    // FIXME close all open files and free the file table entries
    int i;

//    vfs_close(curthread->files[0]->vn);
//    files_destroy(curAthread->files[0]);
   
    for (i=0; i<MAX_OPENED_FILES; ++i) {
        if (curthread->files[i]) {
            //vfs_close(curthread->files[i]->vn);
//            kprintf ("[%s]\t%s\t:\t%d\tvalue: %d\n", __FILE__ , __PRETTY_FUNCTION__, __LINE__, curthread->files[i]->vn->vn_opencount);
            if (curthread ->files[i]->vn->vn_opencount == 0){
//              kprintf ("[%s]\t%s\t:\t%d\tvalue: %d\n", __FILE__ , __PRETTY_FUNCTION__, __LINE__, -1);
//                files_destroy(curthread->files[i]);
            }
        }
    }
    process_table[(int)(curthread->pid)].active = 0;
    if (process_table[(int)(curthread->pid)].parentWaiting) {
        //thread_wakeup((vaddr_t *)curthread->pid); // for waitpid, wake up the parent
        //if (process_table[(int)(curthread->pid)].processSem == NULL) process_table[(int)(curthread->pid)].processSem = sem_create("process_sem", 0);
        
        V(process_table[(int)(curthread->pid)].processSem);
        process_table[(int)(curthread->pid)].parentWaiting = 0;
        //kprintf("")
    }
    thread_exit();
}

#endif

