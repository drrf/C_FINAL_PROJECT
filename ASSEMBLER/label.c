/*
 *          File: label.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 *          NOTE: In this file have my version to Self-referenaitl Structures
 *                from the book "THE C PROGRAMMING LANGUAGE 2nd edition" (p.139)
 * ----------------------------------------------------------------
 */

#include "myas.h"
#define dummy_head -99

/* addtree: add a node with w at or below p */
tnode *addtree (tnode *p, char *w, theDATA d)
{
    int cond;
	theDATA * d_ptr = &d;

    if (p == NULL) {    /* a new word has arrived */
        p = talloc();   /* make a new node */
        p->word = strdup(w);
        p->count = ONE;
		
		/*  IF THE DATA NOT EMPTY GET DATA */
		if (d.IC_DC != EMPTY){	
			if (d.IC_DC == DATA_NODE){ /* if this data line */
				p->ic_dc = DATA_NODE;
				tEnterData(p,d);
			} else {
				p->ic_dc = d.IC_DC;
				p->adr = d.tab -> TEMP_IC;
			}
		} else { /* dummy head */
			p->ic_dc = dummy_head;
			p->adr = dummy_head;
			p->ex = dummy_head;
		}

        p-> left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == FALSE) { /* THE LABEL REPEATED */
		d.tab -> E_ERR += err_in_word(pERR_ID_wrd_doubleL,d_ptr,w);
        p->count++;     /* repeated word */
    } else if (cond < ZERO)  /* less than into left subtree */
        p->left = addtree(p->left, w, d);
    else                /* greater than into right subtree */
        p->right = addtree(p->right, w, d);
    return p;
}

/* UPDATE THE DATA BY EX/EN IF NEED */
int tEnterData (tnode *p, theDATA d)
{
	int switch_n, sum = ZERO;
	
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
			p->ex = ZERO; 
	}

 	return sum;
}

/* FIND LABEL ON THE TREE LABELS */
int findLabel (tnode *p, char *w, theDATA d, int temp_ic)
{
	int cond, sum = ZERO;
	int final_ic = d.tab -> FINAL_IC;
	theDATA * d_ptr = &d;

	if (p == NULL) {
		d.tab -> E_ERR += err_in_word(pERR_ID_wrd_miss_L,d_ptr,w);
		return ERR_NUMBER;
	} else if ((cond = strcmp(w, p->word)) == ZERO){
		sum = p->adr;
		if (p->adr == EX_NODE){ /* found ex label */
			write_ex_file(w,temp_ic,d);
			return EX_ADR;
		} else if (p->ic_dc == DATA_NODE) {
			sum += final_ic;
			return sum+START_LINE_100;
		} else 
			;
        return sum+START_LINE_100;    /* found label+100 */
    }else if (cond < ZERO)  /* less than into left subtree */
        return findLabel(p->left, w, d, temp_ic);
    else                /* greater than into right subtree */
        return findLabel(p->right, w, d, temp_ic);
}

/* talloc: make a tnode */
tnode *talloc(void)
{
    return (tnode *) malloc(sizeof(tnode));
}

/* make a duplicate of s */
char *strdup (char *s)
{
	char *p;
	p = (char *) malloc (strlen(s)+ONE); /* +1 for '\0' */
	if (p != NULL)
		strcpy(p,s);
	return p;
}

/* USE ONLY FOR TESTIG TO SEE WHAT ENTER TO THE TREE */
/* treeprint: in-order print of tree p */
void treeprint(tnode *p, int ic)
{
    if (p != NULL) {
        treeprint(p->left, ic);
		if (p->ic_dc == DATA_NODE && p->ex != EX_NODE){
			p->adr += START_LINE_100 + ic;
		} else if (p->ic_dc == 	OPT_NODE){
			p->adr += START_LINE_100;
		} else
			;

		if (p->adr != dummy_head) /* dummy head */
			printf( "NAME = %s\t || ADR = %4d || IC/DC = %4d || EX = %d || TIME = %d\n", p->word, p->adr, p->ic_dc, p->ex, p-> count );
        
        treeprint(p->right, ic);
    }
}
