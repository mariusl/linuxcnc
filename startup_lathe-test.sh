#!/bin/bash
# startup script for linuxcnc in RIP mode

. ~/dev/linuxcnc-dev/scripts/rip-environment 
linuxcnc ~/dev/linuxcnc-dev/configs/lathe_test/lathe-test.ini
 
