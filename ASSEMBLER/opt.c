/*
 *          File: opt.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* CHECK OPT LINE AND RETURN HOW MUCH IC THERE */
int checkOPT(theDATA d)
{
	int sum_ic;
	theDATA * d_ptr = &d;

	/* HOW MUCH IC IN THIS LINE */
	sum_ic = d.LABEL + d.REG + d.NUM; 

	if (sum_ic > MAX_IC_IN_LINE){
		d.tab -> E_ERR += err_in_line(pERR_ID_line_IC_MAX,d_ptr,d.LINE);
		return 0;
	} else{
		if (d.REG > ONE) /* TWO REGISTER IN LINE */
				sum_ic = sum_ic-ONE;
	}

	return sum_ic;
}

int optLine (theDATA d, int r)
{
	int ind = ZERO, x = ZERO, switch_n, temp_ic, before_temp;
	int sum_ic, des, sur;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;
	theDATA * d_ptr = &d;

	switch_n = (int)d.ENCODING[ind];

	if (switch_n == LABEL_N && r == R_ONE){
		temp_l = d.NEW_LABEL;
		TEMP_T = addtree (TEMP_T, temp_l, d);
	} else if (switch_n == LABEL_N && r != R_ONE)
		switch_n = (int)d.ENCODING[ind+ONE];
	else
		;

	/* IF R1 -> SAVE HOW MUCH IC IN THIS LINE AND RETURN */
	if (r==R_ONE){ 
		d.tab -> TEMP_IC += sum_ic = checkOPT(d)+ONE;
		return 0;
	} else {
		sum_ic = checkOPT(d);
	}

	/* TAKE THE DES AND SUR */
	des = returnAddress(d.ENCODING[d.WORDS]);
	sur = returnAddress(d.ENCODING [x = GIVE_ME_NUM (d,x)]);

	before_temp = temp_ic = d.tab -> TEMP_IC;

	/* THIS IS R2 SECTION SWITCH TO THE RIGHT OPT */
	switch(switch_n) {
		case V_MOV :
			if ((sur == adr_0_NUM || sur == adr_1_LAB || sur == adr_3_REG) &&
				(des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_CMP :
			if ((sur == adr_0_NUM || sur == adr_1_LAB || sur == adr_3_REG) &&
				(des == adr_0_NUM || des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_ADD :
			if ((sur == adr_0_NUM || sur == adr_1_LAB || sur == adr_3_REG) &&
				(des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_SUB :
			if ((sur == adr_0_NUM || sur == adr_1_LAB || sur == adr_3_REG) &&
				(des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_NOT :
			if ((des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_only(d,sum_ic,des);
	    	break;
		case V_CLR :
			if ((des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_only(d,sum_ic,des);
	    	break;
		case V_LEA :
			if ((sur == adr_1_LAB) &&
				(des == adr_1_LAB || des == adr_3_REG))
					temp_ic += DES_and_SUR(d,sum_ic,des,sur);
	    	break;
		case V_INC :
			if ((des == adr_1_LAB || des == adr_3_REG))
					temp_ic += DES_only(d,sum_ic,des);
	    	break;
		case V_DEC :
			if ((des == adr_1_LAB || des == adr_3_REG))
					temp_ic += DES_only(d,sum_ic,des);
	    	break;
		case V_JMP :
	    	if (sum_ic == ONE && (d.NUM == ZERO))
				temp_ic += DES_only(d,sum_ic,des);
			else if (sum_ic > ONE /* && (d.NUM == 0)*/)
				temp_ic += optJUMP(d,sum_ic);
			else
				;
			break;
		case V_BNE :
	    	if (sum_ic == ONE && (d.NUM == ZERO))
				temp_ic += DES_only(d,sum_ic,des);
			else if (sum_ic > ONE  && (d.NUM == ZERO))
				temp_ic += optJUMP(d,sum_ic);
			else
				;
	    	break;
		case V_RED :
			if ((des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_only(d,sum_ic,des);
	    	break;
		case V_PRN :
			if ((des == adr_0_NUM || des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_only(d,sum_ic,des);
	    	break;
		case V_JSR :
			if (sum_ic == ONE && (des == adr_1_LAB || des == adr_3_REG))
				temp_ic += DES_only(d,sum_ic,des);
			else if (sum_ic >= TWO /* && (d.NUM == 0)*/)
				temp_ic += optJUMP(d,sum_ic);
			else
				;
	    	break;
		case V_RTS :
			temp_ic += noDES_noSUR(d,sum_ic);
	    	break;
		case V_STOP :
			temp_ic += noDES_noSUR(d,sum_ic);
	    	break;
		default :
	    	d.tab -> E_ERR += err_in_line(ZERO,d_ptr,d.LINE);
	} /* END OF SWITCH */

	/* IF TEMP NOT CHANGE IS MEAN ERROR */
	if (before_temp == temp_ic)
		d.tab -> E_ERR += err_in_line(pERR_ID_line_OPT,d_ptr,d.LINE);

	return 0;
}

int noDES_noSUR (theDATA d, int ic)
{
	int mc = ZERO;
	
	/* RETURN IF FOUND TOO MUCH IC OR WORD IN LINE */
	if (ic > ZERO || d.WORDS > ONE){
		return 0;
	}

	d.MC_ARR[d.tab -> TEMP_IC++] = mc = d.OPT_DATA * bit_OPT;

	return ic+ONE;
}

int DES_only (theDATA d, int ic, int des)
{
	int mc1 = ZERO, mc2 = ZERO, labelInd = ZERO;
	int temp_ic = d.tab -> TEMP_IC;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;

	/* RETURN IF FOUND TOO MUCH IC OR WORD IN LINE */
	if ( ic > ONE || d.WORDS > THREE ){
		return 0;
	}

	temp_ic++;
	/* TAKE DATA FOR DES */
	if (des == adr_0_NUM){
		mc2 = d.NUM_ARR[ZERO] * BIT_3;
	} else if (des == adr_1_LAB) {
		/* SAVE LABEL AND FOUND THE IC */
		temp_l = d.LABEL_ARR[labelInd];
		mc2 = findLabel (TEMP_T,temp_l,d,temp_ic++);
		if (mc2 != EX_ADR)
			mc2 = ADD_R_LBAEL_ADR(mc2);
	} else if (des == adr_3_REG) {
		mc2 = d.REG_ARR[ZERO] * BIT_3;
	} else {
		;
	}
	
	mc1 = (d.OPT_DATA * bit_OPT) + (des * bit_DES);

	d.MC_ARR[d.tab -> TEMP_IC++] = mc1;
	d.MC_ARR[d.tab -> TEMP_IC++] = mc2;

	return ic+ONE;
}

int DES_and_SUR (theDATA d, int ic, int des, int sur)
{
	int mc1 = ZERO, mc2 = ZERO, mc3 = ZERO, labelInd = ZERO;
	int temp_ic = d.tab -> TEMP_IC;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;

	/* RETURN IF FOUND TOO MUCH IC OR WORD IN LINE */
	if ( ic < ONE || ic > TWO){
		return 0;
	}
	
	mc1 = (d.OPT_DATA * bit_OPT) + (des * bit_DES) + (sur * bit_SUR);

	if (des == adr_3_REG && sur == adr_3_REG){ /* IF FOUND TWO REGISTER */
		mc2 += d.REG_ARR[ZERO] * BIT_9;
		mc2 += d.REG_ARR[ONE] * BIT_3;
	} else {
		if (sur == adr_0_NUM)
			mc2 = d.NUM_ARR[ZERO] * BIT_3;
		else if (sur == adr_1_LAB) {
			/* MC2 IS LABEL ADR -> FOUND THE IC */
			temp_l = d.LABEL_ARR[labelInd++];
			mc2 = findLabel (TEMP_T,temp_l,d,temp_ic++);
			if (mc2 != EX_ADR)
				mc2 = ADD_R_LBAEL_ADR(mc2);
		} else if (sur == adr_3_REG)
			mc2 += d.REG_ARR[ZERO] * BIT_9;
		else
			;

		if (des == adr_0_NUM)
			mc3 = d.NUM_ARR[ZERO] * BIT_3;
		else if (des == adr_1_LAB) {
			/* MC3 IS LABEL ADR -> FOUND THE IC */
			temp_l = d.LABEL_ARR[labelInd];
			mc3 = findLabel (TEMP_T,temp_l,d,temp_ic++);
			if (mc3 != EX_ADR)
				mc3 = ADD_R_LBAEL_ADR(mc3);
		} else if (des == adr_3_REG)
			mc3 += d.REG_ARR[ZERO] * BIT_3;
		else
			;
	}

	d.MC_ARR[d.tab -> TEMP_IC++] = mc1;
	d.MC_ARR[d.tab -> TEMP_IC++] = mc2;

	if (des != adr_3_REG || sur != adr_3_REG){
		d.MC_ARR[d.tab -> TEMP_IC++] = mc3;
	}

	return ic+ONE;
}

int optJUMP (theDATA d, int ic)
{
	int mc1 = ZERO, mc2 = ZERO, mc3 = ZERO, mc4 = ZERO, labelInd = ZERO;
	int x = ZERO, p1, p2, label;
	int temp_ic = d.tab -> TEMP_IC;
	tnode * TEMP_T = d.tab -> T_ROOT;
	char * temp_l;

	/* RETURN IF FOUND TOO MUCH IC OR WORD IN LINE */
	if ( ic < TWO || ic > THREE){
		return 0;
	}

	if (checkIfHaveNum(d,BRECKT_N) != ONE)
		return 0;

	label = returnAddress(d.ENCODING [x += GIVE_ME_NUM (d,x)]);
	p1 = returnAddress(d.ENCODING [x = GIVE_ME_NUM (d,x)]);
	p2 = returnAddress(d.ENCODING [x = GIVE_ME_NUM (d,x)]);
	
	/* IF JUNP HAVE LABEL AS THIS SHOULD */
	if (label == adr_1_LAB){
		/* MC1 IS THE OPT CMD */
		mc1 = d.OPT_DATA * bit_OPT;
		mc1 += p1 * bit_P1;
		mc1 += (p2 * bit_P2) + BIT_4;

		temp_ic++;
		/* MC2 IS THE LABEL ADR -> FOUND THE IC */
		temp_l = d.LABEL_ARR[labelInd++];
		mc2 = findLabel (TEMP_T,temp_l,d,temp_ic++);
		if (mc2 != EX_ADR)
			mc2 = ADD_R_LBAEL_ADR(mc2);
		if (p1 == adr_3_REG && p2 == adr_3_REG){ /* IF FOUND TWO REGISTER */
			mc3 += d.REG_ARR[ZERO] * BIT_9;
			mc3 += d.REG_ARR[ONE] * BIT_3;
			temp_ic++;
		} else {
			if (p1 == adr_0_NUM)
				mc3 = d.NUM_ARR[ZERO] * BIT_3;
			else if (p1 == adr_1_LAB){
				temp_l = d.LABEL_ARR[labelInd++];
				mc3 = findLabel (TEMP_T,temp_l,d,temp_ic++);
				if (mc3 != EX_ADR)
					mc3 = ADD_R_LBAEL_ADR(mc3);	
			} else if (p1 == adr_3_REG)
				mc3 += d.REG_ARR[ZERO] * BIT_9;
			else
				;

			if (p2 == adr_0_NUM)
				mc4 = d.NUM_ARR[ZERO] * BIT_3;
			else if (p2 == adr_1_LAB){
				temp_l = d.LABEL_ARR[labelInd];
				mc4 = findLabel (TEMP_T,temp_l,d,temp_ic++);
				if (mc4 != EX_ADR)
					mc4 = ADD_R_LBAEL_ADR(mc4);	
			} else if (p2 == adr_3_REG)
				mc4 += d.REG_ARR[ZERO] * BIT_3;
			else
				;
		}
	} else
		;


	/* COPY MC TO MC ARRAY */
	d.MC_ARR[d.tab -> TEMP_IC++] = mc1;
	d.MC_ARR[d.tab -> TEMP_IC++] = mc2;
	d.MC_ARR[d.tab -> TEMP_IC++] = mc3;
	if (p1 != adr_3_REG || p2 != adr_3_REG){
		d.MC_ARR[d.tab -> TEMP_IC++] = mc4;
	}

	return ic+ONE;
}

/* ADD ADDRESS LABEL RELOCATE */
int ADD_R_LBAEL_ADR (int adr)
{
	adr = (adr * BIT_3)+TWO;	
	return adr;
}


/* RETURN ADDRESS METHOD */
int returnAddress (int x)
{
	int R = REGISTER_N;

	if (x == DIGIT_A){
		x = adr_0_NUM;
	} else if (x == LABEL_NL) {
		x = adr_1_LAB;
	} else if (x >= R+reg_r0 && x <= R+reg_r7) {
		x = adr_3_REG;
	} else {
		x = EMPTY_AND_BLOCK;
	}

	return x;
}
