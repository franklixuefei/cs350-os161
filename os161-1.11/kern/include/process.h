#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "opt-A2.h"

#if OPT_A2
#include <types.h>
#include <synch.h>
#include <thread.h>
#include <array.h>

#define MAX_FORKED_PROCESSES 70

struct process {
    int exitcode;
    pid_t pid;
    int active; // init to 0
    struct array *children;
    struct semaphore *processSem;
    int parentWaiting; // init to 0
};

extern struct process process_table[MAX_FORKED_PROCESSES];

#endif
#endif
