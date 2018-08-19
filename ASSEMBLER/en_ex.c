/*
 *          File: en_ex.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* WRITE THE ADDRESS OF ENTRY LABEL IN ".ent" FILE */
int write_en_file (tnode *p, char * label, theDATA d)
{
	int adr;
	char F_NAME [MAX_NAME];
    FILE *fp_EN;
	
	/* UPDATE FILE NAME */
	strcpy(F_NAME, d.tab -> FNAME);
	strcat(F_NAME, FILE_ENT);
	fp_EN = fopen(F_NAME, "a");

	/* RETURN LABEL FROM THE TREE AND PRINT */
	adr = findLabel(p,label,d,EMPTY);
	fprintf(fp_EN, "%s \t %d\n", label, adr);

	fclose(fp_EN);

	return 0;
}

/* WRITE THE ADDRESS OF EXTREN LABEL IN ".ext" FILE */
int write_ex_file (char * label, int adr, theDATA d)
{
	char F_NAME [MAX_NAME];
    FILE *fp_EX;
	
	/* UPDATE FILE NAME */
	strcpy(F_NAME, d.tab -> FNAME);
	strcat(F_NAME, FILE_EXT);
	fp_EX = fopen(F_NAME, "a");

	fprintf(fp_EX, "%s \t %d\n", label, adr+START_LINE_100);

	fclose(fp_EX);

	return 0;
}
