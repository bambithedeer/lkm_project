# lkm_project

run make to build lkm

sudo insmod test.ko will install the lkm
sudo rmmod test will remove the lkm

use tail -f /var/log/syslog to check status of lkm

gcc user.c -o user will compile test code
