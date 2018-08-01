/*
 *          File: ins_and_free.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* CRATE THE DATA */
theDATA * installD (SLine S)
{
	int SIZE_DATA = S.SLines;
	theDATA * MY_DATA;

	MY_DATA = (theDATA *) malloc (SIZE_DATA * sizeof(theDATA));
	printf("SIZEOF(MY_DATA) = %ld", sizeof(MY_DATA));
	printf(", in FILE = %s", S.FileN);
	printf(", with SIZE LINE = %d\n", S.SLines);

	return MY_DATA;
}

/* CRATE THE TABLE */
TABLE installT ()
{
	tnode *root;
	
	char str [] = "_dummy_head_";
    const char s[2] = " ";
	char *token;
	theDATA MY_DATA;
	TABLE MY_TAB = {0,0,0};

	/* ADD DUMMY HEAD TO THE LABEL TREE */
	MY_DATA.IC_DC = 0;
	root = NULL;
	token = strtok(str, s);
	root = addtree(root, token,MY_DATA);
	
	MY_TAB.T_ROOT = root;

	return MY_TAB;
}

/* FREE LINKEDLIST */
void freeList(lnode * head)
{
   lnode * tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }

}
