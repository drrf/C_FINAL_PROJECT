/*
 *          File: err.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

 #include "myas.h"

/*** === ERR IN FILE arr.c ===  ***/	
int err_in_arr (int ERR_ID, char * F_name)
{

	fprintf(stderr, "ERROR: IN FILE %s ",  F_name);

	switch(ERR_ID){
		case pERR_ID_OPEN_F :
			fprintf(stderr, "CAN'T OPEN THE FILE!\n");
	    	break;
		case pERR_ID_F_EMPTY :
			fprintf(stderr, "THE FILE %s IS EMPTY!\n",F_name);
	    	break;
		case pERR_ID_MAX_LINE :
			fprintf(stderr, "HAVE MORE THEN %d LINES!\n", MAX_LINE);
	    	break;
		default :
	    	fprintf(stderr, "YOUR FILE HAVE ERR FIX THEM!\n");
	}

	puts("");

	return ONE;
}

/*** === ERR IN FILE algo.c ===  ***/	
void err_in_algo (int ERR_ID, char * F_name, int ERR_Count)
{

	fprintf(stderr, "ERROR: IN FILE %s ",  F_name);

	switch(ERR_ID){
		default :
	    	fprintf(stderr, "FOUND %d ERR IN THE CODE CAN'T CONTINUE UNTILL FIX THEM!\n", ERR_Count);
	}
}

/* PRINT FILE NAME AND LINE ERROR */
void err_open_print(theDATA * d)
{
	int line = d->LineNum;
	char * F_name = d->FNAME;

	fprintf(stderr, "ERROR: IN FILE \"%s\" IN LINE %d !\n\n\t",  F_name, line);
}

/*** === ERR IN FILE parsing.c ===  ***/	
int err_print(int ERR_ID, theDATA * d, char ch)
{
	int line = d->LineNum;

	/* PRINT FILE NAME AND LINE */
	err_open_print(d);
	
	switch(ERR_ID){
		case pERR1_ID_START_LINE :
			fprintf(stderr, "THIS LETTER -> \'%c\' IS NOT VALID START LINE!\n", ch);
	    	break;
		case pERR1_ID :
			fprintf(stderr, "THIS LETTER -> \'%c\' IS NOT VALID THERE!\n", ch);
	    	break;
		case pERRchar_ID :
			fprintf(stderr, "THIS LETTER -> \'%c\' IS NOT VALID IN AS FILES!\n", ch);
	    	break;
		case pERRduble_ID :
			fprintf(stderr, "DOUBLE USE IN LETTER -> \'%c\' IS NOT VALID THERE!\n", ch);
	    	break;
		default :
	    	fprintf(stderr, "ERROR: IN LINE %d\n", line);
	} /* END OF SWITCH */

	puts("");

	return ONE; /* ADD ONE TO POINTER ERR COUNTER */
}

/*** === ERR IN FILE parsing.c ===  ***/	
int err_in_word(int ERR_ID, theDATA * d, char * str)
{
	/* PRINT FILE NAME AND LINE */
	err_open_print(d);

	switch(ERR_ID){
		case pERR_ID_wrd_data :
			fprintf(stderr, "THIS WORD -> \'%s\' IS NOT VALID DATA WORD!\n", str);
	    	break;
		case pERR_ID_wrd_double :
			fprintf(stderr, "THIS WORD -> \'%s\' IS NOT VALID, DOUBLE DEFINED!\n", str);
	    	break;
		case pERR_ID_wrd_Saved :
			fprintf(stderr, "THIS WORD -> \'%s\' IS NOT VALID LABEL, BECUSE THIS SAVED WORD!\n", str);
			break;
		case pERR_ID_wrd_CLOSE :
			fprintf(stderr, "THE STATEMENT -> \' %s \', MUST HAVE MATCH CLOSE!\n", str);
	    	break;
	    case pERR_ID_wrd_doubleL :
			fprintf(stderr, "THE LABEL -> \' %s \', ALLREADY DECLARED ONE!\n", str);
	    	break;
		case pERR_ID_wrd_miss_L :
			fprintf(stderr, "THE LABEL -> \' %s \', DON'T DECLARED!\n", str);
	    	break;
		default :
	    	fprintf(stderr, "THIS WORD -> \'%s\' IS NOT VALID WORD!\n", str);
	} /* END OF SWITCH */

	puts("");

	return ONE; /* ADD ONE TO POINTER ERR COUNTER */
}

int err_in_line(int ERR_ID, theDATA * d, char * str)
{
	/* PRINT FILE NAME AND LINE */
	err_open_print(d);

	switch(ERR_ID){
		case pERR_ID_line_IC_DC :
			fprintf(stderr, "THIS LINE -> \"%s\" IS NOT opt OR data LINE!\n", str);
	    	break;
		case pERR_ID_line_IC_MAX :
			fprintf(stderr, "THIS LINE -> \"%s\" FOUND MORE THAN %d IC IN ONE LINE!\n", str, MAX_IC_IN_LINE);
	    	break;
	    case pERR_ID_line_EXTRA :
			fprintf(stderr, "THIS LINE -> \"%s\" FOUND EXTRA VALID WORDS!\n", str);
	    	break;
		case pERR_ID_line_DATA :
			fprintf(stderr, "DATA/STRING LINE -> \"%s\" FOUND VALID WORDS!\n", str);
	    	break;
		case pERR_ID_line_OPT :
			fprintf(stderr, "THIS LINE -> \"%s\" FOUND VALID OPT DES/SUR!\n", str);
	    	break;
		default :
	    	fprintf(stderr, "THIS LINE -> \'%s\' IS NOT VALID!\n", str);
	} /* END OF SWITCH */

	puts("");

	return ONE; /* ADD ONE TO POINTER ERR COUNTER */
}
