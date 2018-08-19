/*
 *          File: ins_and_rem.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* CRATE THE TABLE */
TABLE installT (char * FILE_NAME)
{
	tnode *root;
	theDATA MY_DATA;
	int len = strlen(FILE_NAME);
	char str [] = "_dummy_head_";
    const char s[TWO] = " ";
	char *token;
	
	TABLE MY_TAB = {ZERO,ZERO,ZERO,ZERO,ZERO}; /* RESET DATA HERE */

	/* static becuase use outside of this func */
	static char des[MAX_NAME];	

	/* ADD DUMMY HEAD TO THE LABEL TREE */
	MY_DATA.IC_DC = ZERO;
	root = NULL;
	token = strtok(str, s);
	root = addtree(root, token,MY_DATA);
	
	MY_TAB.T_ROOT = root;

	/* REMOVE .AS FROM FILE NAME */
	des[ZERO] = '\0';
	strncat(des, FILE_NAME, len-FILE_FORMAT_LEN);
	MY_TAB.FNAME = des;
	
	/* REMOVE OLDS FILES IF EXIST */
	remove_files(MY_TAB.FNAME);

	return MY_TAB;
}

/* REMOVE THE THREE FILE (ob/ent/ext) */
int remove_files (char * File)
{
	char R_F_NAME [MAX_NAME];

	strcpy(R_F_NAME, File);
	strcat(R_F_NAME, FILE_OB);
	remove(R_F_NAME);
	strcpy(R_F_NAME, File);
	strcat(R_F_NAME, FILE_ENT);
	remove(R_F_NAME);
	strcpy(R_F_NAME, File);
	strcat(R_F_NAME, FILE_EXT);
	remove(R_F_NAME);

	return 0;
}
