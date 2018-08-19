/*
 *          File: mc.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* CREATE THE ".ob" FILE */
int MC_to_FILE (TABLE mc)
{  
	int i;
	char str[BITS_SIZE_14+ONE]; /* +1 to '\0' */
	char * F_NAME = mc.FNAME;
    FILE *fp_MC;

	strcat(F_NAME, FILE_OB);
	fp_MC = fopen(F_NAME, "w");

	if (fp_MC == NULL){
	    fprintf(stderr, "Error opening file %s!\n", F_NAME);
		fprintf(stderr, "EXIT PROGRAM!!\n");
	    exit(0);
	} else { 
	
	/* PRINT IC AND DC COUNT */
	fprintf(fp_MC, "\t%d\t%d\n\n", mc.FINAL_IC, mc.DC);

	/* PRINT THE LINE AND THE CODE */
	for (i = ZERO; i < mc.MC; i++){
	    fprintf(fp_MC, "%04d\t", i+START_LINE_100);
	    fprintf(fp_MC, "\t%s\n", INT_to_MC(str,mc.MC_ARR[i]));
	}
	printf("\n>>> ASSEMBLER: Succses create %s file! <<<\n\n", F_NAME);
	printf("---------------------------------------------\n");
	fclose(fp_MC);
	}
	
    return 0;
}

/* RETURN STRING OF STRANGE BINARY "./././" FROM INT */
char * INT_to_MC (char * str,int x)
{
    unsigned int mask = ONE;
    char ZERO_POINT = '.';
    char ONE_LINE = '/';
    int i;
    char src[BITS_SIZE_14+ONE]; /* +1 to '\0' */

	/* MASK TO COMPARE EACH BIT IS ON OR OFF */
    for (i = ZERO; i < BITS_SIZE_14; i++){
        if ((x&mask) == ZERO)
			src[i] = ZERO_POINT;
		else
			src[i] = ONE_LINE;
		mask <<= ONE;
    }
    my_strrev(src);
    src[BITS_SIZE_14+ONE] = '\0';
    strcpy(str, src);

    return str;
}

/* REVERSE STRING FUNCTION */
char * my_strrev(char *str)
{
      char *p1, *p2;

    /* check for empty string */
      if (! str || ! *str)
            return str;

      for (p1 = str, p2 = str + strlen(str) - ONE; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

