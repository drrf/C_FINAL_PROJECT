/*
 *          File: main.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

int main ( int argc, char *argv[] )
{
    int argv_ind = ZERO, count = ZERO;

    printf("\n****************************************");
	printf("\nThis program read ASSEMBLER files");
	printf("\nand process them to machine code\n");
	printf("****************************************\n");

	/* IF MORE ONE ARGV AND LAST THEN MAX FILES */
    if (argc-ONE > ZERO && argc <= MAX_FILES+ONE){ 
        for (argv_ind = ONE; argv_ind < argc; argv_ind++){
            if (!cpToArr (argv[argv_ind]))
				count++;
        }
    } else {
        printf("INPUT %d FILES\n", argc-ONE);
        if (argc-ONE == ZERO)
            fprintf(stderr, "THE PROGRAM NEED INPUT 1 FILE OR MORE\n");
        else
            fprintf(stderr, "CHANGE THE SETTING TO WORK ON MORE THEN %d FILES\n", MAX_FILES);
    }
	printf("\n---------------------------------------------");
	printf("\nASSEMBLER: Succses create %d from %d files!\n", count, argc-ONE);
	printf("---------------------------------------------\n");
    return 0;
}
