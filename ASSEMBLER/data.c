/*
 *          File: data.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* SWITCH TO THE RIGHT DATA LINE CASE */
int dataLine (theDATA d, int r)
{
	int i = ZERO, switch_n;
	int label = ZERO;
	int BREAK = ZERO;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;
	theDATA * d_ptr = &d;

	switch_n = (int)d.ENCODING[i];

	for (i = ZERO; i <= d.WORDS; i++){
		switch(switch_n) {
			case LABEL_N : /* LINE START WITH LABEL */
				if (r == R_ONE ){ /* IF R_ONE ADD LABEL TO THE TREE */
					switch_n = (int)d.ENCODING[i+ONE];
					if (switch_n != EX_N && switch_n != EN_N ) {
						temp_l = d.NEW_LABEL;
						TEMP_T = addtree (TEMP_T, temp_l, d);
					}
				} else
					switch_n = (int)d.ENCODING[i+ONE];
		    	break;
			case EX_N : /* LINE START WITH EXTREN */
				if (r == R_ONE){ /* IF R_ONE ADD LABEL TO THE TREE */
					switch_n = (int)d.ENCODING[++i];
					if (d.WORDS > TWO || switch_n != LABEL_NL) {
						d.tab -> E_ERR += 
						err_in_line(pERR_ID_line_EXTRA,d_ptr,d.LINE);
						break;
					} else {
						temp_l = d.LABEL_ARR[label];
						TEMP_T = addtree (TEMP_T, temp_l, d);
					}
				}
		    	break;
			case EN_N : /* LINE START WITH ENTRY */
				if (r == R_TWO){ /* IF R_TWO ADD LABEL TO THE TREE */
					switch_n = (int)d.ENCODING[++i];
					if (d.WORDS > TWO || switch_n != LABEL_NL){
						d.tab -> E_ERR += 
						err_in_line(pERR_ID_line_EXTRA,d_ptr,d.LINE);
						break;
					} else {
						temp_l = d.LABEL_ARR[label];
						write_en_file(TEMP_T, temp_l,d);
					}
				}
		    	break;
			case STRING_N : /* LINE WITH STRING */
				dataString(d,i,r);
				switch_n = BREAK;
				break;
			case DIGIT_N : /* LINE WITH DIGIT DATA */
				dataDigit(d,i,r);
				switch_n = BREAK;
				break;
			default :
		    	d.tab -> E_ERR += err_in_line(ZERO,d_ptr,d.LINE);
		} /* END OF SWITCH */

		if (switch_n == BREAK) break;		
	} /* END OF FOR */
	return 0;
}

/* COUNT THE DC AND ENTER STRING TO ARRAY */
int dataString (theDATA d, int i, int r)
{
	int j = ZERO, switch_n = ZERO;
	int strLen = ZERO;
	theDATA * d_ptr = &d;

	switch_n = (int)d.ENCODING[++i];
	
	/* IF THIS LINE NOT ONLY A STRING PRINT ERR */
	if (d.WORDS > TWO || switch_n != STRING_A)
		d.tab -> E_ERR += err_in_line(pERR_ID_line_DATA,d_ptr,d.LINE);
	else{
		strLen = strlen(d.STRING) - ONE; /* +EOF */
		while(j++ < strLen-ONE){
			if (r == R_TWO) /* IF R_TWO ENTER STRING ELSE COUNT DC */
				d.MC_ARR[d.tab -> FINAL_DC++] = (int) d.STRING[j];
			else
				d.tab -> DC++;
		}
		if (r == R_TWO)
				d.MC_ARR[d.tab -> FINAL_DC++] = (int) '\0';
		else
			d.tab -> DC++;
				
		free(d.STRING); /* IMPORTANT TO FREE THE MALLOC */
	}
	return 0;
}

/* COUNT THE DC AND ENTER DIGIT DATA TO ARRAY */
int dataDigit (theDATA d, int i, int r)
{
	int num = ZERO, numARR = ZERO;
	int switch_n, j=ZERO;
	theDATA * d_ptr = &d;

	/* IF NOT END WITH DIGIT */
	if ((switch_n = (int)d.ENCODING[d.WORDS]) != DIGIT_A)
		d.tab -> E_ERR += err_in_line(pERR_ID_line_DATA,d_ptr,d.LINE);

	numARR = d.NUM;	/* HOW MUCH DATA DIGIT IN THIS LINE */

	do {
		switch_n = (int)d.ENCODING[++i];
		if (switch_n == DIGIT_A){
			num++;
			if (r == R_TWO) /* IF R_TWO ENTER DATA ELSE COUNT DC */
				d.MC_ARR[d.tab -> FINAL_DC++] = d.NUM_ARR[j++];
			else
				d.tab -> DC++;
		} else if (switch_n == COMMA_N) {
			;
		} else { /* IF NOT DIGIT OR COMMA */
			d.tab -> E_ERR += err_in_line(pERR_ID_line_DATA,d_ptr,d.LINE);
			break;
				}
		} while (num < numARR);

	return 0;
}
