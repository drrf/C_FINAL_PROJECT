/*
 *          File: err.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: asmbler
 * ----------------------------------------------------------------
 */

 #include "myas.h"

/* NOT VALID BE THERE */
 int pERRchar (char c, int line)
 {
     fprintf(stderr, "ERROR: IN LINE %d, THIS LETTER: \"%c\" IS NOT VALID THERE!\n", line, c);

     return 0;
 }

/* NOT VALID START LINE OR BE IN THE FIRST WORD */
 int pERR1st (int ERR, char c, int line)
 {
     fprintf(stderr, "ERROR: IN LINE %d, THIS LETTER: \"%c\" IS NOT VALID THERE!\n", line, c);

     return 0;
 }

 /* NOT MORE THEN ONE-TIME LETTER */
 int pERRduble (char c, int line)
 {
     fprintf(stderr, "ERROR: IN LINE %d, ONE OF THIS LETTER: \"%c\" IS NOT VALID THERE!\n", line, c);

     return 0;
 }

 /* ONE CLOSER MISS */
 int pERRmiss (char c, int line)
 {
     fprintf(stderr, "ERROR: IN LINE %d, ONE CLOSER IS MISS!\n", line);

     return 0;
 }

  /* CAN'T ENCODE THIS WORD */
 int pERRword (char c [MAX_WORD], int line)
 {
     fprintf(stderr, "ERROR: IN LINE %d, CAN'T ENCODE \"%s\" THIS WORD!\n", line, c);

     return 0;
 }