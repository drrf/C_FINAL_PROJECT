/*
 *          File: r1.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

int ERR1 = 0;

int r1 (SLine S)
{
	int line = 0, i = 0;
	char FileArr [MAX_CODE];
	char oneLine [MAX_LINE];
	int * MC; /* SAVE MC HERE */

	/* theDATA * MY_DATA = installD(S);*/
	TABLE MY_TAB = installT();
	theDATA MY_DATA;

	/* LINKLIST FOR DC (need move to installT) */
	lnode DC1 = {-1,NULL};
	lnode DC2 = {0,NULL};

	MY_TAB.DC_HEAD = &DC1;
	MY_TAB.DC_PTR = &DC2;
	MY_TAB.DC_HEAD -> next = MY_TAB.DC_PTR;
	MY_TAB.DC_HEAD = MY_TAB.DC_HEAD -> next;

	/* COPY CODE FROM S TO FileArr */
	strncpy(FileArr,S.Code,S.SLen+1);
	MY_DATA.FNAME = S.FileN;
	MY_DATA.tab = &MY_TAB;
	
	/* R1 THIS IS FIRST TIME */
	for (line = 0; line < S.SLines-1; line++){
		/* COPY EACH LINE TO oneLine, REMOVE '\n' and ADD EOF */
    	strncpy(oneLine, FileArr+S.SLineArr[line], S.SLineArr[line+1]-1);
    	oneLine[(S.SLineArr[line+1]-S.SLineArr[line]-1)] = '\0';

		/* START parsing.c FILE */
		MY_DATA = ReadLine(oneLine,line+1,MY_DATA); /* PLUS 1 FOR 0 */
		
		if (MY_DATA.Line != EMPTY) { /* EMPTY OR COMMENT */
    		printf("\n%d) %s\n\n", line+1, oneLine);
			WriteLine(MY_DATA,R_ONE);
		}
	}

	if (ERR1 == FALSE){
		printf("\nGOOD FOUND 0 ERR IN THE CODE!\n");
		
		MY_TAB.MC = MY_TAB.IC + MY_TAB.DC;
		MC = (int *) calloc (MY_TAB.MC,sizeof(int));
		MC [0] = 1;
		MC [MY_TAB.IC] = 100;
		MC [MY_TAB.DC] = 777;
		MC [MY_TAB.MC] = -999;

		MY_DATA.MC_ARR = &MC[0];
		MY_TAB.DC_PTR = MY_TAB.DC_HEAD -> next;
		
		for (i = MY_TAB.IC; i < MY_TAB.MC; i++){
			MY_DATA.MC_ARR[i] = MY_TAB.DC_PTR -> data;
			MY_TAB.DC_PTR = MY_TAB.DC_PTR -> next;	
		}
	} else {
		printf("\nFOUND ERR IN THE CODE CAN'T CONTINUE!\n");
		exit(0);	
	}

	MY_TAB.FINAL_IC = MY_TAB.IC;
	MY_TAB.IC = 0;
	strncpy(FileArr,S.Code,S.SLen+1);
	

	/* R2 THIS IS SECCEND TIME */
	for (line = 0; line < S.SLines-1; line++){
		/* COPY EACH LINE TO oneLine, REMOVE '\n' and ADD EOF */
    	strncpy(oneLine, FileArr+S.SLineArr[line], S.SLineArr[line+1]-1);
    	oneLine[(S.SLineArr[line+1]-S.SLineArr[line]-1)] = '\0';

		/* START parsing.c FILE */
		MY_DATA = ReadLine(oneLine,line+1,MY_DATA); /* PLUS 1 FOR 0 */
		
		if (MY_DATA.Line != EMPTY) { /* EMPTY OR COMMENT */
    		printf("\n%d) %s\n\n", line+1, oneLine);
			WriteLine(MY_DATA,R_TWO);
		}
	}

	printf("\n\t IC = %d, DC = %d\n\n", MY_TAB.IC, MY_TAB.DC);

		for (i = 0; i <= MY_TAB.MC; i++){	
			if (i < MY_TAB.IC)
				printf("\t\tIC %4d %d !\n", i+100, MY_DATA.MC_ARR [i]);
			else
				printf("\t\tDC %4d %d !\n", i+100, MY_DATA.MC_ARR [i]);
		}

	
	/* r2(MY_DATA,MY_TAB);*/

	treeprint(MY_TAB.T_ROOT, MY_TAB.IC);

	return 0;
}



int WriteLine (theDATA d, int r)
{
	int ind = 0, j = 0, label = 0;
	
	printf("\n=== DATA FILE in file: %s ===\n", d.FNAME);
	printf("SIZEOF(d) = %ld", sizeof(d));
	printf(", WORK ON LINE = %d, with R=%d\n", d.Line,r);
	
	if (d.IC_DC == 1)
		optLine(d,r); /* START opt.c FILE */
	else if (d.IC_DC == -1)
		dataLine(d,r); /* START data.c FILE */
	else
		printf("FOUND ERR IN DATA/OPT IF!\n");

	for (ind = 0; ind <= d.WORDS; ind++){
		j = (int)d.ENCODING[ind];

		if (j == -1){
			printf("LABEL FOUND: %s \n", d.LABEL_ARR[label++]);
		}
	}

	printf("ENCODE = ");
	for (ind = 0; ind <= d.WORDS; ind++){
				printf("%d, ", (int)d.ENCODING[ind] );
	}
	
	puts("");
puts("");

return 0;
}
