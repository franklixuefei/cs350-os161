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
#alias run os
alias runos="cd ~/cs350-os161/root/;sys161 kernel"
#alias open kern.conf file
alias confkern="cd ~/cs350-os161/os161-1.11/kern/conf;vim conf.kern"
#alias open main.c file
alias editmain="cd ~/cs350-os161/os161-1.11/kern/main; vim main.c"
#alias kernel config
alias confos0="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST0"
alias confos1="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST1"
alias confos2="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST2"
alias confos3="cd ~/cs350-os161/os161-1.11/kern/conf;./config ASST3"

#alias make depend and make
alias buildos0="cd ~/cs350-os161/os161-1.11/kern/compile/ASST0;make depend;make"
alias buildos1="cd ~/cs350-os161/os161-1.11/kern/compile/ASST1;make depend;make"
alias buildos2="cd ~/cs350-os161/os161-1.11/kern/compile/ASST2;make depend;make"
alias buildos3="cd ~/cs350-os161/os161-1.11/kern/compile/ASST3;make depend;make"
#alias install
alias installos0="cd ~/cs350-os161/os161-1.11/kern/compile/ASST0;make install"
alias installos1="cd ~/cs350-os161/os161-1.11/kern/compile/ASST1;make install"
alias installos2="cd ~/cs350-os161/os161-1.11/kern/compile/ASST2;make install"
alias installos3="cd ~/cs350-os161/os161-1.11/kern/compile/ASST3;make install"

