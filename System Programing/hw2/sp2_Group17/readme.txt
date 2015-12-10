-receiver.c
-sender.c

ports are hard-coded, 9001, 9002, 9003.

how to compile
gcc receiver.c -lpthread -std=c99 -o receiver
gcc sender.c -lpthread -std=c99 -o sender
then ./receiver or ./sender

NOTICE : server's ip address is hard-coded on sender.c. check '192.168.56.3' on sender.c



module/
-Makefile
-simplemodule.c

just make. then insmod.
how to input port : echo 9001 > /proc/myproc/myproc
if inserting port are not working, please rmmod and insmod again.
