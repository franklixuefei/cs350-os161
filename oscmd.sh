#!/bin/bash

shopt -s expand_aliases
#alias help
alias cmdhelp="cat ~/cs350-os161/oscmd.sh"
#alias edit oscmd.sh
alias oscmd="cd ~/cs350-os161; vim oscmd.sh"
#alias os top
alias ostop="cd ~/cs350-os161/; ls"
#alias os root
alias osroot="cd ~/cs350-os161/root; ls"
#alias cd kern dir
alias oskern="cd ~/cs350-os161/os161-1.11/kern; ls"
#alias os161-1.11
alias os161="cd ~/cs350-os161/os161-1.11/; ls"
#alias userprog
alias userprog="cd ~/cs350-os161/os161-1.11/kern/userprog/; ls"
#alias make runtests a2
alias a2test="cd ~/cs350-os161/os161-1.11/a2-test-scripts/; make runtests;cd -"
#alias make runtests a3
alias a3test="cd ~/cs350-os161/os161-1.11/a3-test-scripts/; make runtests;cd -"
#alias run os
alias runos="cd ~/cs350-os161/root/;sys161 kernel;cd-"
#alias open kern.conf file
alias confkern="cd ~/cs350-os161/os161-1.11/kern/conf;vim conf.kern"
#alias open main.c file
alias editmain="cd ~/cs350-os161/os161-1.11/kern/main; vim main.c"
#alias kernel config
alias confos0="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST0; cd -"
alias confos1="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST1; cd -"
alias confos2="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST2; cd -"
alias confos3="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST3; cd -"

#alias cs350-gdb
alias gdbkernelw="cd ~/cs350-os161/root; sys161 -w kernel; cd -"
alias cs350gdb="cd ~/cs350-os161/root/; cs350-gdb kernel; cd -"
alias firegdb="cd ~/cs350-os161/;./firegdb.sh"

#alias make depend and make
alias buildos0="cd ~/cs350-os161/os161-1.11/kern/compile/ASST0;make depend;make;cd ~/cs350-os161/os161-1.11/kern/compile/ASST0;make install; cd -"
alias buildos1="cd ~/cs350-os161/os161-1.11/kern/compile/ASST1;make depend;make;cd ~/cs350-os161/os161-1.11/kern/compile/ASST1;make install; cd -"
alias buildos2="cd ~/cs350-os161/os161-1.11/kern/compile/ASST2;make depend;make;cd ~/cs350-os161/os161-1.11/kern/compile/ASST2;make install; cd -"
alias buildos3="cd ~/cs350-os161/os161-1.11/kern/compile/ASST3;make depend;make;cd ~/cs350-os161/os161-1.11/kern/compile/ASST3;make install; cd -"


