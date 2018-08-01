/*
 *          File: mc.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */
#include "myas.h"

typedef struct MC {
    int IC;
    int DC;
    int L;
    int * code;
    char * F_NAME;
} MC;

int MC_to_FILE (MC);
char * INT_to_MC(char[],int);
char * my_strrev(char *);

int main ()
{
    MC mc;
    mc.IC = 5, mc.DC = 10, mc.L = mc.IC+mc.DC;
    char src [99] = "out.txt"; // need change the name like: "F_NAME.as" -> "F_NAME.ob"
    int arr [15] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383};

    mc.code = &arr[0];
    mc.F_NAME = &src[0];
    
    /* print DATA to file */
    MC_to_FILE(mc);

    return 0;
}

/* print DATA to New file */
int MC_to_FILE (MC mc)
{  
    FILE *f = fopen(mc.F_NAME, "w");
    char str[15];
    int i = 0, ZERO = 0;

    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "\t%d\t%d\n\n", mc.IC, mc.DC);

    for (i = 0; i < mc.L; i++){
        fprintf(f, "%d%d\t", ZERO, i+100);
        fprintf(f, "\t%s\n", INT_to_MC(str,mc.code[i]));
    }

    fclose(f);

    return 0;
}

/* COPY int MC to string */
char * INT_to_MC (char * str,int x)
{
    unsigned int mask = 1;
    char ZERO = '.';
    char ONE = '/';
    int i;
    int max = 14;
    char src[15];

    for (i = 0; i < max; i++){
        if ((x&mask) == 0)
			src[i] = ZERO;
		else
			src[i] = ONE;
		mask <<= 1;
    }
    my_strrev(src);
    src[15] = '\0';
    strcpy(str, src);

    return str;
}

/* rev string func */
char * my_strrev(char *str)
{
      char *p1, *p2;

    /* check for empty string */
      if (! str || ! *str)
            return str;

      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}