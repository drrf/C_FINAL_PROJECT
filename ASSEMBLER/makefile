# MAKEFILE FOR ASSEMBLER PROGRAM
# Last Modified: May 8, 2018
# GLOBAL DEFINITIONS
CC=gcc
GFLAGS = -g -ansi -Wall -pedantic
CFLAGS = -c -ansi -Wall -pedantic
ALLFILES = main.o arr.o ins_and_free.o r1.o r2.o parsing.o data.o opt.o label.o err.o

myprog : $(ALLFILES)
	$(CC) $(GFLAGS) $(ALLFILES) -o myprog

main.o : main.c myas.h
	$(CC) $(CFLAGS) main.c -o main.o

arr.o : arr.c myas.h
	$(CC) $(CFLAGS) arr.c -o arr.o

ins_and_free.o : ins_and_free.c myas.h
	$(CC) $(CFLAGS) ins_and_free.c -o ins_and_free.o

r1.o : r1.c myas.h
	$(CC) $(CFLAGS) r1.c -o r1.o

r2.o : r2.c myas.h
	$(CC) $(CFLAGS) r2.c -o r2.o

parsing.o : parsing.c myas.h
	$(CC) $(CFLAGS) parsing.c -o parsing.o

data.o : data.c myas.h
	$(CC) $(CFLAGS) data.c -o data.o

opt.o : opt.c myas.h
	$(CC) $(CFLAGS) opt.c -o opt.o

label.o : label.c myas.h
	$(CC) $(CFLAGS) label.c -o label.o

err.o : err.c myas.h
	$(CC) $(CFLAGS) err.c -o err.o


run:
	./myprog < in1.txt > out1.txt
	./myprog < int2.txt > out2.txt

gdb:
	gdb ./myprog

clean:
	rm -f *.o
