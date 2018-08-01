/*
 *          File: data.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* lalloc: make a lnode for DC linklist */
lnode *lalloc(void)
{
    return (lnode *) malloc(sizeof(lnode));
}

int dataLine (theDATA d, int r)
{
	int i = 0, switch_n = 0;
	int words = 0, label = 0;
	int BREAK = 0, ERR = -999;
	lnode * TEMP = d.tab -> DC_HEAD;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;

	words = d.WORDS; /* HOW MUCH WORD IN THIS LINE */

	printf("FOUND DATA LINE HERE WITH %d WORDS!\n", words+1);
	switch_n = (int)d.ENCODING[i];

	for (i = 0; i <= words; i++){
		switch(switch_n) {
			case LABEL_N :
				if (r == R_ONE){
					printf("NEW LABEL FOUND: %s \n", d.NEW_LABEL);
					temp_l = d.NEW_LABEL;
					TEMP_T = addtree (TEMP_T, temp_l, d);
				}
				switch_n = (int)d.ENCODING[i+1];
		    	break;
			case EX_N :
				if (r == R_ONE){
					printf("extern -> FOUND ");
					switch_n = (int)d.ENCODING[++i];
					if (d.WORDS > 1 || switch_n != -1) {
						printf("ERR IN DATA SWITCH!\n");
					} else {
						printf("LABEL: %s \n", d.LABEL_ARR[label]);
						temp_l = d.LABEL_ARR[label];
						TEMP_T = addtree (TEMP_T, temp_l, d);
					}
				}
		    	break;
			case EN_N :
				printf(".entry -> FOUND ");
				switch_n = (int)d.ENCODING[++i];
				if (d.WORDS > 1 || switch_n != -1){
					printf("ERR IN DATA SWITCH!\n");
				} else {
					printf("LABEL: %s \n", d.LABEL_ARR[label]);
					/* temp_l = d.LABEL_ARR[label];
					TEMP_T = addtree (TEMP_T, temp_l, d);*/
				}
		    	break;
			case STRING_N :
				if (r == R_ONE)
					dataString(d,i);
				switch_n = BREAK;
				break;
			case DIGIT_N :
				if (r == R_ONE)
					dataDigit(d,i);
				switch_n = BREAK;
				break;
			default :
		    	printf("FOUND ERR IN DATA SWITCH!\n");
		} /* END OF SWITCH */

		if (switch_n == BREAK) break;		
	} /* END OF FOR */

	if (r == R_ONE){
		printf("DC = %d\n", d.tab -> DC);


		while (TEMP!=NULL){
			printf("%d", TEMP -> data);
			TEMP = TEMP -> next;
		}
		
		puts("");
	}
	return 0;
}

int dataString (theDATA d, int i)
{
	int j = 0, switch_n = 0;
	int words = 0, strLen = 0;

	words = d.WORDS; /* HOW MUCH WORD IN THIS LINE */

	printf("string -> FOUND ");
	switch_n = (int)d.ENCODING[++i];

	if (d.WORDS > 2 || switch_n != STRING_A)
		printf("ERR IN DATA SWITCH!\n");
	else{
		strLen = strlen(d.STRING) - 1; /* +EOF */
		while(j++ < strLen-1){
			d.tab -> DC_PTR =  d.tab -> DC_PTR -> next = lalloc();
			d.tab -> DC_PTR -> data = (int) d.STRING[j];
			d.tab -> DC_PTR -> next = NULL;
			d.tab -> DC++;
		}
		d.tab -> DC_PTR =  d.tab -> DC_PTR -> next = lalloc();
		d.tab -> DC_PTR -> data = (int) '\0';
		d.tab -> DC_PTR -> next = NULL;
		d.tab -> DC++;
				
		printf("%d, %s \n", strLen, d.STRING);
		free(d.STRING); /* IMPORTANT TO FREE THE MALLOC */
	}

	if (i == words)
		printf("\nSTRING FINE!\n");
	else
		printf("\nSTRING NOT FINE!\n");

	return 0;
}

int dataDigit (theDATA d, int i)
{
	int words = 0, num = 0, numARR = 0;
	int switch_n, j=0; 
	words = d.WORDS; /* HOW MUCH WORD IN THIS LINE */
	numARR = d.NUM;	/* HOW MUCH DATA DIGIT IN THIS LINE */

	printf("data -> FOUND ");
	do {
		switch_n = (int)d.ENCODING[++i];
		if (switch_n == DIGIT_A){
			d.tab -> DC_PTR =  d.tab -> DC_PTR -> next = lalloc();
			d.tab -> DC_PTR -> data = d.NUM_ARR[j++];
			d.tab -> DC_PTR -> next = NULL;
			d.tab -> DC++;
			printf("%d", d.NUM_ARR[num++]);
		} else if (switch_n == COMMA_N)
			printf(", ");
		else
			printf("FOUND ERR IN DIGIT DATA!\n");
		} while (num < numARR);

	if (i == words)
		printf("\nDIGIT FINE!\n");
	else
		printf("\nDIGIT NOT FINE!\n");

	return 0;
}
