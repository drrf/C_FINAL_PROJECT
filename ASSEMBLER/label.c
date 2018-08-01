/*
 *          File: label.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
*          NOTE: My version to Self-referenaitl Structures
 *                from "THE C PROGRAMMING LANGUAGE 2nd edition" (p.139)
 * ----------------------------------------------------------------
 */

#include "myas.h"
#define OPT_NODE 1
#define DATA_NODE -1
#define EX_NODE -1
#define EN_NODE -111

/* addtree: add a node with w at or below p */
tnode *addtree (tnode *p, char *w, theDATA d)
{
    int cond;

    if (p == NULL) {    /* a new word has arrived */
        p = talloc();   /* make a new node */
        p->word = strdup(w);
        p->count = 1;
		

		/*  IF THE DATA NOT EMPTY GET DATA */
		if (d.IC_DC != EMPTY){	
			if (d.IC_DC == DATA_NODE){ /* if this data line */
				p->ic_dc = DATA_NODE;
				tEnterData(p,d);
			} else {
				p->ic_dc = d.IC_DC;
				p->adr = d.tab -> IC;
			}
		} else { /* dummy head */
			p->ic_dc = -99;
			p->adr = -99;
			p->ex = -99;
		}

        p-> left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == FALSE) /* THE LABEL REPEATED */
        p->count++;     /* repeated word */
    else if (cond < 0)  /* less than into left subtree */
        p->left = addtree(p->left, w, d);
    else                /* greater than into right subtree */
        p->right = addtree(p->right, w, d);
    return p;
}

int tEnterData (tnode *p, theDATA d)
{
	int switch_n = 0, sum = 0;
	
	switch_n = d.OPT_DATA;

	switch(switch_n) {
		case EN_N :
			p->adr = EN_NODE; 
	    	break;
		case EX_N :
			p->adr = EX_NODE;
			p->ex = EX_NODE; 
	    	break;
		default :
			p->adr = d.tab -> DC;
			p->ex = 0; 
	}

 	return sum;
}

int findLabel (tnode *p, char *w, int ic)
{
	int cond, sum = 0;
	int Line = -1;

	if (p == NULL) {
		return Line;
	} else if ((cond = strcmp(w, p->word)) == 0){
		sum = p->adr;
		if (p->ic_dc == DATA_NODE)
			sum += ic;
        return sum+100;    /* found label+100 */
    }else if (cond < 0)  /* less than into left subtree */
        return findLabel(p->left, w, ic);
    else                /* greater than into right subtree */
        return findLabel(p->right, w, ic);
}

/* treeprint: in-order print of tree p */
void treeprint(tnode *p, int ic)
{
    if (p != NULL) {
        treeprint(p->left, ic);
		if (p->ic_dc == DATA_NODE && p->ex != EX_NODE){
			p->adr += 100 + ic;
		} else if (p->ic_dc == 	OPT_NODE){
			p->adr += 100;
		} else
			;

		if (p->adr != -99) /* dummy head */
			printf( "NAME = %s\t || ADR = %4d || IC/DC = %4d || EX = %d || TIME = %d\n", p->word, p->adr, p->ic_dc, p->ex, p-> count );
        
        treeprint(p->right, ic);
    }
}

/* talloc: make a tnode */
tnode *talloc(void)
{
    return (tnode *) malloc(sizeof(tnode));
}

char *strdup (char *s) /* make a duplicate of s */
{
	char *p;
	p = (char *) malloc (strlen(s)+1); /* +1 for '\0' */
	if (p != NULL)
		strcpy(p,s);
	return p;
}
