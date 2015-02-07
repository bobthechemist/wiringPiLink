#/bin/sh

gcc -fpic -g -c -Wall -I /opt/Wolfram/WolframEngine/10.0/SystemFiles/IncludeFiles/C/ wiringpilink.c
gcc -shared -Wl,-soname,libwiringpi.so.1 -o libwiringpi.so.1.0.1 wiringpilink.o -lc -lwiringPi -lpthread
