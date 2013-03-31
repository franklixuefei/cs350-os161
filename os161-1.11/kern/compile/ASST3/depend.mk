lhd.o: ../../dev/lamebus/lhd.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/kern/errno.h machine/bus.h machine/vm.h \
  ../../dev/lamebus/lamebus.h ../../include/uio.h ../../include/vfs.h \
  ../../dev/lamebus/lhd.h ../../include/dev.h autoconf.h
emu.o: ../../dev/lamebus/emu.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/kern/unistd.h ../../include/kern/stat.h \
  ../../include/lib.h machine/setjmp.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/array.h ../../include/uio.h \
  ../../include/vfs.h ../../include/emufs.h ../../include/vnode.h \
  ../../include/fs.h ../../dev/lamebus/emu.h machine/bus.h machine/vm.h \
  ../../dev/lamebus/lamebus.h autoconf.h
random.o: ../../dev/generic/random.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/kern/errno.h ../../include/kern/unistd.h \
  ../../include/lib.h machine/setjmp.h ../../include/uio.h \
  ../../include/vfs.h ../../dev/generic/random.h ../../include/dev.h \
  autoconf.h
rtclock.o: ../../dev/generic/rtclock.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/kern/errno.h ../../include/lib.h machine/setjmp.h \
  ../../include/clock.h opt-synchprobs.h ../../dev/generic/rtclock.h \
  autoconf.h
pseudorand.o: ../../dev/generic/pseudorand.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/uio.h \
  ../../dev/generic/pseudorand.h autoconf.h
ltrace.o: ../../dev/lamebus/ltrace.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h machine/bus.h machine/vm.h \
  ../../dev/lamebus/lamebus.h ../../dev/lamebus/ltrace.h autoconf.h
lamebus.o: ../../dev/lamebus/lamebus.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h machine/spl.h \
  ../../dev/lamebus/lamebus.h
console.o: ../../dev/generic/console.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/kern/errno.h ../../include/lib.h machine/setjmp.h \
  machine/spl.h ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../dev/generic/console.h ../../include/dev.h ../../include/vfs.h \
  ../../include/uio.h autoconf.h
lser.o: ../../dev/lamebus/lser.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h machine/spl.h machine/bus.h machine/vm.h \
  ../../dev/lamebus/lamebus.h ../../dev/lamebus/lser.h autoconf.h
beep.o: ../../dev/generic/beep.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../dev/generic/beep.h \
  autoconf.h
lrandom.o: ../../dev/lamebus/lrandom.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/uio.h machine/bus.h \
  machine/vm.h ../../dev/lamebus/lamebus.h ../../dev/lamebus/lrandom.h \
  autoconf.h
ltimer.o: ../../dev/lamebus/ltimer.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/clock.h \
  opt-synchprobs.h machine/bus.h machine/vm.h ../../dev/lamebus/lamebus.h \
  ../../dev/lamebus/ltimer.h autoconf.h
rtclock_ltimer.o: ../../dev/lamebus/rtclock_ltimer.c \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h ../../include/lib.h machine/setjmp.h \
  ../../dev/generic/rtclock.h ../../dev/lamebus/ltimer.h autoconf.h
emu_att.o: ../../dev/lamebus/emu_att.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/lamebus/lamebus.h \
  ../../dev/lamebus/emu.h autoconf.h
random_lrandom.o: ../../dev/lamebus/random_lrandom.c \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h ../../include/lib.h machine/setjmp.h \
  ../../dev/generic/random.h ../../include/dev.h \
  ../../dev/lamebus/lrandom.h autoconf.h
beep_ltimer.o: ../../dev/lamebus/beep_ltimer.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/generic/beep.h \
  ../../dev/lamebus/ltimer.h autoconf.h
ltrace_att.o: ../../dev/lamebus/ltrace_att.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/lamebus/lamebus.h \
  ../../dev/lamebus/ltrace.h autoconf.h
pseudorand_att.o: ../../dev/generic/pseudorand_att.c \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h ../../include/lib.h machine/setjmp.h \
  ../../dev/generic/random.h ../../include/dev.h \
  ../../dev/generic/pseudorand.h autoconf.h
con_lser.o: ../../dev/lamebus/con_lser.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/generic/console.h \
  ../../dev/lamebus/lser.h autoconf.h
lrandom_att.o: ../../dev/lamebus/lrandom_att.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/lamebus/lamebus.h \
  ../../dev/lamebus/lrandom.h autoconf.h
lser_att.o: ../../dev/lamebus/lser_att.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/lamebus/lamebus.h \
  ../../dev/lamebus/lser.h autoconf.h
lhd_att.o: ../../dev/lamebus/lhd_att.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/lamebus/lamebus.h \
  ../../dev/lamebus/lhd.h ../../include/dev.h autoconf.h
ltimer_att.o: ../../dev/lamebus/ltimer_att.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../dev/lamebus/lamebus.h \
  ../../dev/lamebus/ltimer.h autoconf.h
sfs_io.o: ../../fs/sfs/sfs_io.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/uio.h \
  ../../include/sfs.h ../../include/vnode.h ../../include/fs.h \
  ../../include/kern/sfs.h ../../include/dev.h
sfs_fs.o: ../../fs/sfs/sfs_fs.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/kern/errno.h ../../include/array.h \
  ../../include/bitmap.h ../../include/uio.h ../../include/dev.h \
  ../../include/sfs.h ../../include/vnode.h ../../include/fs.h \
  ../../include/kern/sfs.h ../../include/vfs.h
sfs_vnode.o: ../../fs/sfs/sfs_vnode.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/array.h ../../include/bitmap.h \
  ../../include/kern/stat.h ../../include/kern/errno.h \
  ../../include/kern/unistd.h ../../include/uio.h ../../include/dev.h \
  ../../include/sfs.h ../../include/vnode.h ../../include/fs.h \
  ../../include/kern/sfs.h
addrspace.o: ../../vm/addrspace.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/addrspace.h \
  ../../include/vm.h machine/vm.h opt-dumbvm.h opt-A3.h \
  ../../include/vfs.h ../../include/kern/unistd.h ../../include/pt.h \
  opt-A3.h ../../include/vnode.h ../../include/uw-vmstats.h machine/spl.h \
  machine/tlb.h ../../include/elf.h
cache_mips1.o: ../../arch/mips/mips/cache_mips1.S machine/asmdefs.h
exception.o: ../../arch/mips/mips/exception.S machine/asmdefs.h \
  machine/specialreg.h
lamebus_mips.o: ../../arch/mips/mips/lamebus_mips.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/kern/unistd.h ../../include/lib.h machine/setjmp.h \
  ../../include/synch.h opt-A1.h ../../include/queue.h machine/spl.h \
  machine/pcb.h ../../include/dev.h machine/bus.h machine/vm.h \
  ../../dev/lamebus/lamebus.h autoconf.h
interrupt.o: ../../arch/mips/mips/interrupt.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h machine/bus.h machine/vm.h \
  ../../dev/lamebus/lamebus.h machine/spl.h machine/pcb.h
pcb.o: ../../arch/mips/mips/pcb.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h machine/pcb.h machine/spl.h machine/switchframe.h \
  ../../include/thread.h opt-A2.h
ram.o: ../../arch/mips/mips/ram.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/vm.h machine/vm.h machine/pcb.h
spl.o: ../../arch/mips/mips/spl.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h machine/spl.h machine/specialreg.h
start.o: ../../arch/mips/mips/start.S machine/asmdefs.h \
  machine/specialreg.h
switch.o: ../../arch/mips/mips/switch.S machine/asmdefs.h
syscall.o: ../../arch/mips/mips/syscall.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/kern/errno.h ../../include/lib.h machine/setjmp.h \
  machine/pcb.h machine/spl.h machine/trapframe.h \
  ../../include/kern/unistd.h ../../include/kern/callno.h \
  ../../include/process.h opt-A2.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/thread.h ../../include/array.h \
  ../../include/syscall.h ../../include/curthread.h opt-A2.h \
  ../../include/addrspace.h ../../include/vm.h machine/vm.h opt-dumbvm.h \
  opt-A3.h
threadstart.o: ../../arch/mips/mips/threadstart.S machine/asmdefs.h
trap.o: ../../arch/mips/mips/trap.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h machine/trapframe.h machine/specialreg.h machine/pcb.h \
  machine/spl.h ../../include/vm.h machine/vm.h ../../include/thread.h \
  opt-A2.h ../../include/curthread.h ../../include/syscall.h
tlb_mips1.o: ../../arch/mips/mips/tlb_mips1.S machine/asmdefs.h \
  machine/specialreg.h
mips-setjmp.o: ../../../lib/libc/mips-setjmp.S machine/asmdefs.h
copyinout.o: ../../lib/copyinout.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h machine/pcb.h ../../include/vm.h \
  machine/vm.h ../../include/thread.h opt-A2.h ../../include/curthread.h
array.o: ../../lib/array.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/array.h
bitmap.o: ../../lib/bitmap.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/kern/errno.h ../../include/bitmap.h
queue.o: ../../lib/queue.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/queue.h
kheap.o: ../../lib/kheap.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/vm.h machine/vm.h machine/spl.h
kprintf.o: ../../lib/kprintf.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/stdarg.h \
  ../../include/lib.h machine/setjmp.h ../../include/kern/unistd.h \
  ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/vfs.h ../../include/thread.h opt-A2.h machine/pcb.h \
  machine/spl.h
kgets.o: ../../lib/kgets.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h
misc.o: ../../lib/misc.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errmsg.h \
  ../../include/lib.h machine/setjmp.h
ntoh.o: ../../lib/ntoh.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h
__printf.o: ../../../lib/libc/__printf.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/stdarg.h
snprintf.o: ../../../lib/libc/snprintf.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/stdarg.h
atoi.o: ../../../lib/libc/atoi.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h
bzero.o: ../../../lib/libc/bzero.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h
memcpy.o: ../../../lib/libc/memcpy.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
memmove.o: ../../../lib/libc/memmove.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
strcat.o: ../../../lib/libc/strcat.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
strchr.o: ../../../lib/libc/strchr.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
strcmp.o: ../../../lib/libc/strcmp.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
strcpy.o: ../../../lib/libc/strcpy.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
strlen.o: ../../../lib/libc/strlen.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
strrchr.o: ../../../lib/libc/strrchr.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
strtok_r.o: ../../../lib/libc/strtok_r.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h
init.o: ../../dev/init.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h machine/spl.h ../../include/dev.h autoconf.h
device.o: ../../fs/vfs/device.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/kern/errno.h ../../include/kern/unistd.h \
  ../../include/kern/stat.h ../../include/vnode.h ../../include/uio.h \
  ../../include/dev.h
vfscwd.o: ../../fs/vfs/vfscwd.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/kern/stat.h ../../include/lib.h machine/setjmp.h \
  ../../include/vfs.h ../../include/fs.h ../../include/vnode.h \
  ../../include/uio.h ../../include/thread.h opt-A2.h machine/pcb.h \
  ../../include/curthread.h
vfslist.o: ../../fs/vfs/vfslist.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/array.h ../../include/kern/errno.h ../../include/vfs.h \
  ../../include/vnode.h ../../include/fs.h ../../include/dev.h
vfslookup.o: ../../fs/vfs/vfslookup.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/kern/errno.h ../../include/kern/limits.h \
  ../../include/lib.h machine/setjmp.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/vfs.h ../../include/vnode.h \
  ../../include/fs.h
vfspath.o: ../../fs/vfs/vfspath.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/kern/limits.h ../../include/kern/unistd.h \
  ../../include/vfs.h ../../include/vnode.h ../../include/lib.h \
  machine/setjmp.h
vnode.o: ../../fs/vfs/vnode.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/vnode.h
devnull.o: ../../fs/vfs/devnull.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/vfs.h \
  ../../include/dev.h ../../include/uio.h
hardclock.o: ../../thread/hardclock.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h machine/spl.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/clock.h \
  opt-synchprobs.h
synch.o: ../../thread/synch.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/curthread.h \
  machine/spl.h opt-A1.h
scheduler.o: ../../thread/scheduler.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/scheduler.h \
  ../../include/thread.h opt-A2.h machine/pcb.h machine/spl.h \
  ../../include/queue.h
thread.o: ../../thread/thread.c opt-A2.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/kern/errno.h \
  ../../include/array.h machine/spl.h machine/pcb.h \
  ../../include/thread.h opt-A2.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/curthread.h \
  ../../include/scheduler.h ../../include/addrspace.h ../../include/vm.h \
  machine/vm.h opt-dumbvm.h opt-A3.h ../../include/vnode.h \
  ../../include/process.h opt-synchprobs.h ../../include/files.h \
  ../../include/kern/unistd.h ../../include/vfs.h
main.o: ../../main/main.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/kern/unistd.h ../../include/lib.h machine/setjmp.h \
  machine/spl.h ../../include/test.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/thread.h opt-A2.h machine/pcb.h \
  ../../include/scheduler.h ../../include/dev.h ../../include/vfs.h \
  ../../include/vm.h machine/vm.h ../../include/syscall.h \
  machine/trapframe.h ../../include/version.h opt-A0.h opt-A3.h \
  ../../include/swapfile.h ../../include/addrspace.h opt-dumbvm.h \
  opt-A3.h ../../include/uw-vmstats.h
menu.o: ../../main/menu.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/kern/unistd.h ../../include/kern/limits.h \
  ../../include/lib.h machine/setjmp.h ../../include/clock.h \
  opt-synchprobs.h ../../include/thread.h opt-A2.h machine/pcb.h \
  ../../include/syscall.h machine/trapframe.h ../../include/uio.h \
  ../../include/vfs.h ../../include/sfs.h ../../include/vnode.h \
  ../../include/fs.h ../../include/kern/sfs.h ../../include/test.h \
  opt-synchprobs.h opt-sfs.h opt-net.h
loadelf.o: ../../userprog/loadelf.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/uio.h \
  ../../include/elf.h ../../include/addrspace.h ../../include/vm.h \
  machine/vm.h opt-dumbvm.h opt-A3.h ../../include/thread.h opt-A2.h \
  machine/pcb.h ../../include/curthread.h ../../include/vnode.h
runprogram.o: ../../userprog/runprogram.c opt-A2.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/kern/unistd.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/addrspace.h \
  ../../include/vm.h machine/vm.h opt-dumbvm.h opt-A3.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/curthread.h \
  ../../include/vfs.h ../../include/test.h ../../include/syscall.h \
  machine/trapframe.h
uio.o: ../../userprog/uio.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/uio.h ../../include/thread.h opt-A2.h \
  machine/pcb.h ../../include/curthread.h
arraytest.o: ../../test/arraytest.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/array.h ../../include/test.h
bitmaptest.o: ../../test/bitmaptest.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/bitmap.h \
  ../../include/test.h
queuetest.o: ../../test/queuetest.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/queue.h ../../include/test.h
threadtest.o: ../../test/threadtest.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/thread.h opt-A2.h machine/pcb.h \
  ../../include/test.h
tt3.o: ../../test/tt3.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h machine/spl.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/thread.h opt-A2.h machine/pcb.h \
  ../../include/test.h opt-synchprobs.h
synchtest.o: ../../test/synchtest.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/test.h \
  ../../include/clock.h opt-synchprobs.h
malloctest.o: ../../test/malloctest.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/thread.h opt-A2.h machine/pcb.h \
  ../../include/test.h
fstest.o: ../../test/fstest.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/kern/unistd.h ../../include/lib.h machine/setjmp.h \
  ../../include/synch.h opt-A1.h ../../include/queue.h ../../include/fs.h \
  ../../include/vnode.h ../../include/vfs.h ../../include/uio.h \
  ../../include/test.h ../../include/thread.h opt-A2.h machine/pcb.h
hello.o: ../../main/hello.c ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/lib.h \
  machine/setjmp.h
foodcourt.o: ../../asst1/foodcourt.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/test.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/synch.h \
  opt-A1.h ../../include/queue.h opt-A1.h ../../include/asst1/foodcourt.h \
  opt-A1.h
getpid.o: ../../userprog/getpid.c opt-A2.h ../../include/syscall.h \
  opt-A2.h ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h machine/trapframe.h \
  ../../include/thread.h machine/pcb.h machine/setjmp.h \
  ../../include/curthread.h
waitpid.o: ../../userprog/waitpid.c opt-A2.h ../../include/syscall.h \
  opt-A2.h ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h machine/trapframe.h \
  ../../include/process.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/thread.h machine/pcb.h \
  machine/setjmp.h ../../include/array.h ../../include/curthread.h \
  ../../include/kern/errno.h machine/spl.h ../../include/kern/unistd.h
_exit.o: ../../userprog/_exit.c opt-A2.h ../../include/syscall.h opt-A2.h \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h machine/trapframe.h ../../include/thread.h \
  machine/pcb.h machine/setjmp.h ../../include/files.h \
  ../../include/vfs.h ../../include/curthread.h ../../include/process.h \
  ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/array.h ../../include/vnode.h
files.o: ../../userprog/files.c opt-A2.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/kern/unistd.h \
  ../../include/kern/errno.h ../../include/array.h ../../include/thread.h \
  opt-A2.h machine/pcb.h ../../include/curthread.h ../../include/vnode.h \
  ../../include/uio.h ../../include/files.h
open.o: ../../userprog/open.c opt-A2.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/thread.h opt-A2.h \
  machine/pcb.h ../../include/curthread.h ../../include/vnode.h \
  ../../include/uio.h ../../include/syscall.h machine/trapframe.h \
  ../../include/kern/unistd.h ../../include/kern/errno.h \
  ../../include/vfs.h ../../include/files.h ../../include/kern/limits.h
close.o: ../../userprog/close.c opt-A2.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/kern/errno.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/curthread.h \
  ../../include/vnode.h ../../include/uio.h ../../include/syscall.h \
  machine/trapframe.h ../../include/kern/unistd.h ../../include/vfs.h \
  ../../include/files.h
read.o: ../../userprog/read.c opt-A2.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/vm.h machine/vm.h ../../include/lib.h machine/setjmp.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/curthread.h \
  ../../include/vnode.h ../../include/uio.h ../../include/syscall.h \
  machine/trapframe.h ../../include/kern/unistd.h \
  ../../include/kern/errno.h ../../include/vfs.h ../../include/files.h \
  ../../include/addrspace.h opt-dumbvm.h opt-A3.h
write.o: ../../userprog/write.c opt-A2.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/thread.h opt-A2.h \
  machine/pcb.h ../../include/curthread.h ../../include/vnode.h \
  ../../include/uio.h ../../include/syscall.h machine/trapframe.h \
  ../../include/kern/unistd.h ../../include/vfs.h ../../include/files.h \
  ../../include/addrspace.h ../../include/vm.h machine/vm.h opt-dumbvm.h \
  opt-A3.h ../../include/kern/errno.h
fork.o: ../../userprog/fork.c opt-A2.h ../../include/kern/errno.h \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h ../../include/thread.h opt-A2.h machine/pcb.h \
  machine/setjmp.h machine/spl.h ../../include/lib.h \
  ../../include/vnode.h ../../include/vfs.h ../../include/addrspace.h \
  ../../include/vm.h machine/vm.h opt-dumbvm.h opt-A3.h \
  ../../include/syscall.h machine/trapframe.h ../../include/test.h \
  ../../include/synch.h opt-A1.h ../../include/queue.h \
  ../../include/process.h ../../include/array.h ../../include/curthread.h \
  ../../include/files.h ../../include/kern/unistd.h
execv.o: ../../userprog/execv.c opt-A2.h ../../include/kern/errno.h \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h ../../include/thread.h opt-A2.h machine/pcb.h \
  machine/setjmp.h machine/spl.h ../../include/lib.h \
  ../../include/vnode.h ../../include/uio.h ../../include/vfs.h \
  ../../include/addrspace.h ../../include/vm.h machine/vm.h opt-dumbvm.h \
  opt-A3.h ../../include/syscall.h machine/trapframe.h \
  ../../include/test.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h ../../include/process.h ../../include/array.h \
  ../../include/curthread.h ../../include/files.h \
  ../../include/kern/unistd.h
uw-vmstats.o: ../../vm/uw-vmstats.c opt-A3.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../include/synch.h opt-A1.h \
  ../../include/queue.h machine/spl.h ../../include/uw-vmstats.h opt-A3.h
vm_tlb.o: ../../vm/vm_tlb.c opt-A3.h ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/addrspace.h ../../include/vm.h machine/vm.h opt-dumbvm.h \
  opt-A3.h machine/spl.h machine/tlb.h ../../include/curthread.h \
  ../../include/thread.h opt-A2.h machine/pcb.h machine/setjmp.h \
  ../../include/lib.h ../../include/kern/errno.h \
  ../../include/uw-vmstats.h ../../include/pt.h ../../include/elf.h
coremap.o: ../../vm/coremap.c ../../include/coremap.h opt-A3.h \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h machine/spl.h machine/tlb.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/addrspace.h \
  ../../include/vm.h machine/vm.h opt-dumbvm.h ../../include/uw-vmstats.h \
  ../../include/thread.h opt-A2.h machine/pcb.h ../../include/curthread.h \
  ../../include/swapfile.h
swapfile.o: ../../vm/swapfile.c ../../include/swapfile.h \
  ../../include/types.h machine/types.h ../../include/kern/types.h \
  machine/ktypes.h ../../include/addrspace.h ../../include/vm.h \
  machine/vm.h opt-dumbvm.h opt-A3.h machine/tlb.h ../../include/synch.h \
  opt-A1.h ../../include/queue.h ../../include/pt.h \
  ../../include/kern/errno.h ../../include/thread.h opt-A2.h \
  machine/pcb.h machine/setjmp.h ../../include/uio.h \
  ../../include/vnode.h ../../include/coremap.h \
  ../../include/kern/unistd.h ../../include/lib.h ../../include/vfs.h
pt.o: ../../vm/pt.c opt-A3.h ../../include/types.h machine/types.h \
  ../../include/kern/types.h machine/ktypes.h ../../include/kern/errno.h \
  ../../include/lib.h machine/setjmp.h ../../include/thread.h opt-A2.h \
  machine/pcb.h ../../include/curthread.h machine/spl.h machine/tlb.h \
  ../../include/addrspace.h ../../include/vm.h machine/vm.h opt-dumbvm.h \
  opt-A3.h ../../include/uw-vmstats.h ../../include/uio.h \
  ../../include/elf.h ../../include/vnode.h ../../include/pt.h \
  ../../include/coremap.h ../../include/swapfile.h
autoconf.o: ../../compile/ASST3/autoconf.c ../../include/types.h \
  machine/types.h ../../include/kern/types.h machine/ktypes.h \
  ../../include/lib.h machine/setjmp.h ../../compile/ASST3/autoconf.h
