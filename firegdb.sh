#!/bin/bash

cd ~/cs350-os161/root/
gnome-terminal -e "bash -c \"gnome-terminal -x bash -c 'sys161 -w kernel'; exec cs350-gdb kernel;\""
