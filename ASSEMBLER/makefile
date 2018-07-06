
# Last Modified: May 8, 2018
# GLOBAL DEFINITIONS
CC=gcc
CFLAGS = -g -ansi -Wall -pedantic

myprog : main.o arr.o r1.o data.o err.o
	$(CC) -g -ansi -Wall -pedantic main.o arr.o r1.o data.o err.o -o myprog

main.o : main.c myas.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o

arr.o : arr.c myas.h
	gcc -c -Wall -ansi -pedantic arr.c -o arr.o

r1.o : r1.c myas.h
	gcc -c -Wall -ansi -pedantic r1.c -o r1.o

data.o : data.c myas.h
	gcc -c -Wall -ansi -pedantic data.c -o data.o

err.o : err.c myas.h
	gcc -c -Wall -ansi -pedantic err.c -o err.o


run:
	./myprog < in1.txt > out1.txt
	./myprog < int2.txt > out2.txt

gdb:
	gdb ./myprog

clean:
	rm -f *.o
