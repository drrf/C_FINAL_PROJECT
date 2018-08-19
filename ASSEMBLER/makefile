# MAKEFILE FOR ASSEMBLER PROGRAM
# Last Modified: May 8, 2018
# GLOBAL DEFINITIONS
GCC=gcc -c -g
FLAGS = -ansi -Wall -pedantic
ALLFILES = main.o arr.o ins_and_rem.o algo.o parsing.o encode.o data.o opt.o label.o en_ex.o err.o mc.o

myprog : $(ALLFILES)
	gcc -g $(FLAGS) $(ALLFILES) -o myprog

main.o : main.c myas.h
	$(GCC) $(FLAGS) main.c -o main.o

arr.o : arr.c myas.h
	$(GCC) $(FLAGS) arr.c -o arr.o

ins_and_rem.o : ins_and_rem.c myas.h
	$(GCC) $(FLAGS) ins_and_rem.c -o ins_and_rem.o

algo.o : algo.c myas.h
	$(GCC) $(FLAGS) algo.c -o algo.o

parsing.o : parsing.c myas.h
	$(GCC) $(FLAGS) parsing.c -o parsing.o

encode.o : encode.c myas.h
	$(GCC) $(FLAGS) encode.c -o encode.o

data.o : data.c myas.h
	$(GCC) $(FLAGS) data.c -o data.o

opt.o : opt.c myas.h
	$(GCC) $(FLAGS) opt.c -o opt.o

label.o : label.c myas.h
	$(GCC) $(FLAGS) label.c -o label.o

en_ex.o : en_ex.c myas.h
	$(GCC) $(FLAGS) en_ex.c -o en_ex.o

err.o : err.c myas.h
	$(GCC) $(FLAGS) err.c -o err.o

mc.o : mc.c myas.h
	$(GCC) $(FLAGS) mc.c -o mc.o


run:
	./myprog < in1.txt > out1.txt
	./myprog < int2.txt > out2.txt

gdb:
	gdb ./myprog

clean:
	rm -f *.o
