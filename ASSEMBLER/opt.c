/*
 *          File: opt.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */
#include "myas.h"

/* CHECK OPT AND RETURN IC */
int checkOPT(theDATA d)
{
	int sum_ic = 0;

	sum_ic = d.LABEL + d.REG + d.NUM; /* HOW MUCH IC IN THIS LINE */

	if (sum_ic > MAX_IC_IN_LINE){
		printf("ERROR: FOUND %d IC IN ONE LINE!\n", sum_ic);
		return 0;
	} else{
		if (d.REG < 2)
			printf("FOUND %d IC IN THIS LINE!\n", sum_ic);
		else
			printf("FOUND %d IC IN THIS LINE!\n", sum_ic = sum_ic-1);
	}

	return sum_ic;
}

int optLine (theDATA d, int r)
{
	int i = 0, x = 0, switch_n = 0;
	int sum_ic = 0, temp_ic = d.tab -> IC, des = 0, sur = 0;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;

	/* TAKE THE DES */
	des = returnAddress(d.ENCODING[d.WORDS]);
	sur = returnAddress(d.ENCODING [x = GIVE_ME_NUM (d,x)]);
	printf("NEW des FOUND: %d \n", des);
	printf("NEW sur FOUND: %d \n", sur);

	switch_n = (int)d.ENCODING[i];

	if (switch_n == LABEL_N && r == R_ONE){
		printf("NEW LABEL FOUND: %s \n", d.NEW_LABEL);
		temp_l = d.NEW_LABEL;
		TEMP_T = addtree (TEMP_T, temp_l, d);
	} else if (switch_n == LABEL_N && r != R_ONE)
		switch_n = (int)d.ENCODING[i+1];
	else
		;

	if (r==R_ONE){ /* IF WHO CALL THIS FUNC IS R1 */
		d.tab -> IC += sum_ic = checkOPT(d)+1; /* HOW MUCH IC IN THIS LINE */
		return 0;
	} else {
		sum_ic = checkOPT(d);
	}

	/* THIS IS R2 SECTION */
	switch(switch_n) {
		case V_MOV :
			if (des == adr_1 || des == adr_3)
				DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_CMP :
			if (des == adr_0 || des == adr_1 || des == adr_3)
				DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_ADD :
			if (des == adr_1 || des == adr_3)
				DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_SUB :
			if (des == adr_1 || des == adr_3)
				DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_NOT :
			if (d.LABEL == 1 || d.REG == 1)
				DES_only(d,sum_ic,des);
	    	break;
		case V_CLR :
			if (d.LABEL == 1 || d.REG == 1)
				DES_only(d,sum_ic,des);
	    	break;
		case V_LEA :
			if (d.LABEL == 1 || d.REG == 1)
				if (des == adr_1 || des == adr_3)
					DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_INC :
			if (d.LABEL == 1 || d.REG == 1)
				if (des == adr_1 || des == adr_3)
					DES_only(d,sum_ic,des);
	    	break;
		case V_DEC :
			if (d.LABEL == 1 || d.REG == 1)
				if (des == adr_1 || des == adr_3)
					DES_only(d,sum_ic,des);
	    	break;
		case V_JMP :
	    	if (sum_ic == 1 && (d.NUM == 0))
				DES_only(d,sum_ic,des);
			else if (sum_ic > 1 /* && (d.NUM == 0)*/)
				optJUMP(d,sum_ic);
			else
				;
			break;
		case V_BNE :
	    	if (sum_ic == 1 && (d.NUM == 0))
				DES_only(d,sum_ic,des);
			else if (sum_ic > 1  /* && (d.NUM == 0)*/)
				optJUMP(d,sum_ic);
			else
				;
	    	break;
		case V_RED :
			if (d.LABEL == 1 || d.REG == 1)
				DES_only(d,sum_ic,des);
	    	break;
		case V_PRN :
			if (d.NUM == 1 || d.LABEL == 1 || d.REG == 1)
				DES_only(d,sum_ic,des);
	    	break;
		case V_JSR :
			if (sum_ic == 1 && (d.LABEL == 1 || d.REG == 1))
				DES_only(d,sum_ic,des);
			else if (sum_ic >= 2 /* && (d.NUM == 0)*/)
				optJUMP(d,sum_ic);
			else
				;
	    	break;
		case V_RTS :
			noDES_noSUR(d,sum_ic);
	    	break;
		case V_STOP :
			noDES_noSUR(d,sum_ic);
	    	break;
		default :
	    	printf("FOUND ERR IN OPT SWITCH!\n");
	} /* END OF SWITCH */


	/* IF IC DON'T CHANGE WE HAVE ERR */
	if (temp_ic == d.tab -> IC){
		printf("FOUND ERR IN OPT, IC DON'T CHANGE!\n");
		/* exit(0);*/
	}

	printf("IC = %d\n", d.tab -> IC);

	puts("");

	return 0;
}

int noDES_noSUR (theDATA d, int ic)
{
	int words = 0; int mc = 0;
	words = d.WORDS; /* HOW MUCH WORD IN THIS LINE */
	
	if (ic > 1 || words > 2){
		printf("ERR: IN no DEC no SUR FOUND %d IC AND %d WORDS!\n", ic, words);
		return 0;
	}

	d.MC_ARR[d.tab -> IC++] = mc = d.OPT_DATA * bit_OPT;
	/* printf("\n\t IC = %d, DC = %d\n\n", d.tab -> IC, d.tab -> DC);
	exit(0);*/
	return ic+1;
}

int DES_only (theDATA d, int ic, int des)
{
	int words = 0;  int mc1 = 0, mc2 = 0;
	words = d.WORDS; /* HOW MUCH WORD IN THIS LINE */

	if ( ic > 1 || words > 3 ){
		printf("ERR: IN DES only FOUND %d IC AND %d WORDS!\n", ic, words);
		return 0;
	}

	/* TAKE DATA FOR DES */
	if (des == adr_0){
		mc2 = d.NUM_ARR[0] * BIT_3;
	} else if (des == adr_1) {
		mc2 = -1;
	} else if (des == adr_3) {
		mc2 = d.REG_ARR[0] * BIT_3; /* MAYBE NEED +1 FOR R0, PRINT 0000 LINE */
	} else {
		;
	}
	
	mc1 = (d.OPT_DATA * bit_OPT) + (des * bit_DES);

	d.MC_ARR[d.tab -> IC++] = mc1;
	d.MC_ARR[d.tab -> IC++] = mc2;
	printf("MC1 CODE IS: %d\n", mc1);
	printf("MC2 CODE IS: %d\n", mc2);

	return ic+1;
}

int DES_and_SUR (theDATA d, int ic, int des, int sur)
{
	int words = 0, mc1 = 0, mc2 = 0, mc3 = 0;
	words = d.WORDS; /* HOW MUCH WORD IN THIS LINE */

	if ( ic < 1 || ic > 2){
		printf("ERR: IN DEC_and_SUR FOUND %d IC AND %d WORDS!\n", ic, words);
		return 0;
	}
	
	mc1 = (d.OPT_DATA * bit_OPT) + (des * bit_DES) + (sur * bit_SUR);

	if (des == adr_3 && sur == adr_3){ /* IF FOUND TWO REGISTER */
		mc2 += d.REG_ARR[0] * BIT_9;
		mc2 += d.REG_ARR[1] * BIT_3;
	} else {
		if (sur == adr_0)
			mc3 = d.NUM_ARR[0] * BIT_3;
		else if (sur == adr_1)
			mc3 = -1;
		else if (sur == adr_3)
			mc3 += d.REG_ARR[0] * BIT_3;
		else
			;

		if (des == adr_0)
			mc2 = d.NUM_ARR[0] * BIT_3;
		else if (des == adr_1)
			mc2 = -1;
		else if (des == adr_3)
			mc2 += d.REG_ARR[0] * BIT_9;
		else
			;
	}

	d.MC_ARR[d.tab -> IC++] = mc1;
	d.MC_ARR[d.tab -> IC++] = mc2;

	printf("MC1 CODE IS: %d\n", mc1);
	printf("MC2 CODE IS: %d\n", mc2);
	if (des != adr_3 || sur != adr_3){
		printf("MC3 CODE IS: %d\n", mc3);
		d.MC_ARR[d.tab -> IC++] = mc3;
	}

	return ic+1;
}

int optJUMP (theDATA d, int ic)
{
	int words = 0, mc1 = 0, mc2 = 0, mc3 = 0, mc4 = 0;
	int x = 0, y = 0, z = 0;
	int label = 0;
	int final_ic = d.tab -> FINAL_IC;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;

	words = d.WORDS; /* HOW MUCH WORD IN THIS LINE */

	if ( ic < 2 || ic > 3){
		printf("ERR: IN optJUMP FOUND %d IC AND %d WORDS!\n", ic, words);
		return 0;
	}

	label = returnAddress(d.ENCODING [x += GIVE_ME_NUM (d,x)]);
	y = returnAddress(d.ENCODING [x += GIVE_ME_NUM (d,x)]);
	z = returnAddress(d.ENCODING [x += GIVE_ME_NUM (d,x)]);

	printf("L CODE IS: %d\n", label);
	printf("Y CODE IS: %d\n", y);
	printf("Z CODE IS: %d\n", z);

	if (label == adr_1){
		mc1 = ((d.OPT_DATA * bit_OPT) + (y * bit_P1) + (z * bit_P2)) + BIT_4;
		temp_l = d.LABEL_ARR[0];
		mc2 = findLabel (TEMP_T,temp_l,final_ic);
		if (y == adr_3 && z == adr_3){ /* IF FOUND TWO REGISTER */
			mc3 += d.REG_ARR[0] * BIT_9;
			mc3 += d.REG_ARR[1] * BIT_3;
		} else {
			if (y == adr_0)
				mc4 = d.NUM_ARR[0] * BIT_3;
			else if (y == adr_1){
				temp_l = d.LABEL_ARR[2];
				printf("temp_l 2 = %s\n", temp_l);
				mc4 = findLabel (TEMP_T,temp_l,final_ic);
				printf("my = %d\n",d.IC_DC);
					
			} else if (y == adr_3)
				mc4 += d.REG_ARR[0] * BIT_3;
			else
				;

			if (z == adr_0)
				mc3 = d.NUM_ARR[0] * BIT_3;
			else if (z == adr_1){
				temp_l = d.LABEL_ARR[1];
				printf("temp_l 1 = %s\n", temp_l);
				mc3 = findLabel (TEMP_T,temp_l,final_ic);
			} else if (z == adr_3)
				mc3 += d.REG_ARR[0] * BIT_9;
			else
				;
		}
	} else
		;

	d.MC_ARR[d.tab -> IC++] = mc1;
	d.MC_ARR[d.tab -> IC++] = mc2;
	d.MC_ARR[d.tab -> IC++] = mc3;
	printf("MC1 CODE IS: %d\n", mc1);
	printf("MC2 CODE IS: %d\n", mc2);
	printf("MC3 CODE IS: %d\n", mc3);
	if (y != adr_3 || z != adr_3){
		printf("MC4 CODE IS: %d\n", mc4);
		d.MC_ARR[d.tab -> IC++] = mc4;
	}

	return ic+1;
}

int GIVE_ME_NUM (theDATA d, int x)
{
	int i = 0, j = 0;
	int R = REGISTER_N;
	
	for (i = x; i <= d.WORDS; i++){
		j = (int)d.ENCODING[i];
		if (j == -1 || j == DIGIT_A || (j >= R+reg_r0 && j <= R+reg_r7) ){
			x = i;
			break;
		}
	}

	return x;
}

/* RETURN ADDRESS METHOD */
int returnAddress (int x)
{
	int R = REGISTER_N;

	if (x == DIGIT_A){
		x = adr_0;
	} else if (x == -1) {
		x = adr_1;
	} else if (x >= R+reg_r0 && x <= R+reg_r7) {
		x = adr_3;
	} else {
		x = -1;
	}

	return x;
}
