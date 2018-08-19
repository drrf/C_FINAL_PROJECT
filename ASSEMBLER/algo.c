/*
 *          File: algo.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* ALGORITHM PT1 CRATE THE MC TABLE */
int algo1 (AS_DATA S)
{			
	TABLE MY_TAB = installT(S.FileN); /* IC & DC & LABEL */
	theDATA MY_DATA;				  /* FOR ONE LINE OF CODE */
	int * MC; 						  /* MC (IC & DC) */
	
	MY_DATA.FNAME = S.FileN;
	MY_DATA.tab = &MY_TAB;
	
	/* R1 -> RUN ON CODE FIRST TIME */
	MY_DATA = algo2(S,MY_DATA,R_ONE);

	/* IF NOT ERR IN R1 */
	if (MY_DATA.tab -> E_ERR == EMPTY){
		/* COPY IC & DC AND CREATE ARRAY TO SAVE DATA & OPT */
		MY_TAB.MC = MY_TAB.TEMP_IC + MY_TAB.DC;
		MY_TAB.FINAL_IC = MY_TAB.TEMP_IC;
		MY_TAB.FINAL_DC = MY_TAB.FINAL_IC;
		MY_TAB.TEMP_IC = ZERO;
		MC = (int *) calloc (MY_TAB.MC,sizeof(int));
		MY_DATA.MC_ARR = &MC[ZERO];	
		
		/* R2 -> RUN ON CODE SECONND TIME */
		MY_DATA = algo2(S,MY_DATA,R_TWO);
		
		/* OUTPUT THE ob FILE */
		if (MY_DATA.tab -> E_ERR == EMPTY){
			MY_TAB.MC_ARR = MC;
			MC_to_FILE(MY_TAB);		
		} else {
			err_in_algo(ZERO,MY_DATA.FNAME, MY_DATA.tab -> E_ERR);
			remove_files(MY_TAB.FNAME);
		}

		free(MC);
		
	} else {
		err_in_algo(ZERO,MY_DATA.FNAME, MY_DATA.tab -> E_ERR);	
	}

	/* PRINT THE TREE OF LABEL TO CHECK WHAT LABEL WE HAVE THERE */
	/* treeprint(MY_TAB.T_ROOT, MY_TAB.FINAL_IC); */
	
	if (MY_DATA.tab -> E_ERR != EMPTY)
		return -1;
	else
		return 0;
}

/* ALGORITHM PT2 READ & PARRSING ONE LINE */
theDATA algo2 (AS_DATA S, theDATA d, int r)
{
	int line = ZERO, len;
	int End_line = S.SLines-ONE;
	char oneLine [MAX_LINE];
	char FileArr [MAX_CODE];

	strncpy(FileArr,S.Code,S.SLen+ONE);

	for (line = ZERO; line < End_line; line++){
		/* COPY EACH LINE TO oneLine, REMOVE '\n' and ADD EOF */
    	strncpy(oneLine, FileArr+S.SLineArr[line], S.SLineArr[line+ONE]-ONE);

		len = (S.SLineArr[line+ONE]-S.SLineArr[line]-ONE);
		if (oneLine[len-ONE] == ' ' || oneLine[len-ONE] == '\t' ){
			oneLine[len-ONE] = '\0';
		} else {
    		oneLine[len] = '\0';
		}
		/* START parsing.c FILE */
		d = ReadLine(oneLine,line+ONE,d);  /* PLUS 1 FOR 0 */

		/* IF NOT EMPTY, COMMENT OR ERR */
		if (d.LineNum != EMPTY && d.tab -> E_ERR == EMPTY) { 
			algo3(d,r);
		}
	}

	return d;
}

/* ALGORITHM PT3 SWITCH TO OPT/DATA */
int algo3 (theDATA d, int r)
{
	char * line = d.LINE;
	theDATA * d_ptr = &d;
	
	if (d.IC_DC == IC_ID_LINE)
		optLine(d,r); /* START opt.c FILE */
	else if (d.IC_DC == DC_ID_LINE)
		dataLine(d,r); /* START data.c FILE */
	else
		d.tab -> E_ERR += err_in_line(pERR_ID_line_IC_DC,d_ptr,line);
	
	return 0;
}
