/*
 *          File: main.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */
#include "myas.h"

int main ( int argc, char *argv[] )
{
    int i = 0;
    int length = 0;

    printf("\n****************************************");
	printf("\nThis program read ASSEMBLER files\nand compile them to machine code\n");
	printf("****************************************\n");


    if (argc-1 > 0 && argc < MAX_FILES){
        for (i = 1; i < argc; i++){
            length = strlen(argv[i]);
            cpToArr (length,argv[i]);
        }
    } else {
        printf("INPUT %d FILES\n", argc-1);
        if (argc-1 == 0)
            printf("THE PROGRAM NEED INPUT 1 FILE OR MORE\n");
        else
            printf("CHANGE THE SETTING TO WORK ON MORE THEN %d FILES\n", MAX_FILES);
    }

    return 0;
}
