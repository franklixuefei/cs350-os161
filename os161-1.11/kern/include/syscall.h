#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include "opt-A2.h"
#include <types.h>
#include <syscall.h>
#include <machine/trapframe.h>

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);

#if OPT_A2
// more stuff here
void sys__exit(int exitcode);
int sys_getpid(pid_t *retval);
int sys_waitpid(pid_t pid, int *status, int options, pid_t *retval);
int sys_open(const char *filename, int flags, pid_t *retval);
int sys_close(int fd, pid_t *retval);
int sys_fork(struct trapframe *tf, pid_t *retval);
int sys_read(int fd, void *buf, size_t buflen, pid_t *retval);
int sys_write(int fd, const void *buf, size_t nbytes, pid_t *retval);
int sys_execv(const char* program, char **args, pid_t *retval);
#endif

#endif /* _SYSCALL_H_ */
