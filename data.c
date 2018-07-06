/*
 *          File: data.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: asmbler
 * ----------------------------------------------------------------
 */

#include "myas.h"

int dataRead (theDATA d)
{
	int ind = 0, j = 0;
	int R = REGISTER_N, R0 = 0, R7 = 7;
	printf("\n=== DATA FILE ===\n");
	printf("SIZEOF(d) = %ld\n", sizeof(d));

	for (ind = 0; ind <= d.WORDS; ind++){
		j = d.ENCODE[ind];
		if (j == LABEL_N){
			printf("LABEL FOUND: %s \n", d.NEW_LABEL);
		}

		if (j == STRING_N){
			printf("STRING FOUND: %s \n", d.STRING);
		}

		if (j == R || (j > R+R0 && j < R+R7)){
			printf("REGISTER FOUND: r%d \n", j-R);
		}
	}

	for (ind = 0; ind <= d.WORDS; ind++){
            	printf("%d, ", d.ENCODE[ind] );
	}

	puts("");
	
return 0;
}
