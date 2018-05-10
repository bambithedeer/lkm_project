# lkm_project

run make to build lkm

sudo insmod test.ko will install the lkm
sudo rmmod test will remove the lkm

use tail -f /var/log/syslog to check status of lkm

gcc user.c -o user will compile test code

tail -f /var/log/syslog will print out kernel output

to test module run ./user "key" "value"

key is the entry position and the value is an integer value

the key/value pair will be added to the linked list stored in kernel memory
