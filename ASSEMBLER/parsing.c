/*
 *          File: parsing.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

static const char * dataSaveWord [] = {
    "entry", "extern", "data", "string",
};

static const char * optSaveWord [] = {
    "mov", "cmp", "add", "sub",
	"not", "clr", "lea", "inc",
	"dec", "jmp", "bne", "red",
	"prn", "jsr", "rts", "stop",
};

static const char * regSaveWord [] = {
    "r0", "r1", "r2", "r3",
	"r4", "r5", "r6", "r7",
};


int FirstCheckChar (char * oneLine, theDATA * d_ptr)
{
	char ch;
	int len = strlen(oneLine);

	/* CHECK ERROR IN THE FIRST CHAR */
	ch = oneLine[ZERO];
	if ( !isalpha(ch) && ch != '.' ){
		d_ptr->tab -> E_ERR += err_print(pERR1_ID_START_LINE,d_ptr,ch);
		return ONE;
	}

	/* CHECK ERROR IN THE LAST CHAR */
	ch = oneLine[len-ONE];
	if ( !isalpha(ch) && !isdigit(ch) && ch != '\"' && ch != ')' ){
		d_ptr->tab -> E_ERR += err_print(pERR1_ID,d_ptr,ch);
		return ONE;
	}

	return 0;
}

/* START READ LINE & BREAK TO WORD */
theDATA ReadLine (char oneLine [MAX_LINE], int line, theDATA d)
{
	int POINT = ZERO, D_POINT = ZERO, STRING = ZERO,
		BRACKET = ZERO, COMMA = ZERO;
    int ind = ZERO, cpWord = ZERO, err_temp = ZERO, c_case, ch;
	int len = strlen(oneLine);
	
	char oneWord [MAX_WORD];
	static int digitArr [MAX_WORD]; /* STATIC FOR USE OUT THIS FUNC */

	/* SAVE DATA FROM R1 */
	void * d_ptr = &d;

	/* COPY AND RESET DATA */
	d.LineNum = line;
	d.LINE = &oneLine[ZERO];
	d.NUM_ARR = &digitArr[ZERO];
	d.WORDS = d.NUM = d.LABEL = d.IC_DC = d.REG = ZERO;
	d.OPT_DATA = EMPTY_AND_BLOCK; /* EMPTY OPT OR DATA FOR NOW */
	d.ENCODING[ZERO] = '\0';

	/* JUMP OVER COMMENT & EMPTY LINE */
    ch = oneLine[ind];
    if (ch == ';' || strlen(oneLine) < ONE){
		d.LineNum = EMPTY;
		return d;
	}

    /* ERROR IN THE FIRST CHAR */
	if(FirstCheckChar(oneLine, d_ptr) == ONE)
		return d;

	/* SAVE ERR TEMP FOR FOUND CHANGE */
	err_temp = d.tab -> E_ERR;
	
    /* CHECK FOR ERR AND CUT FOR WORDS */
    while (ind < len){
		/* IF FOUND ONE NEW ERR IN LINE */
		if (err_temp != d.tab -> E_ERR)
			return d;
		
		ch = oneLine[ind++];
        oneWord[cpWord++] = (char)ch;
		
        switch(c_case = case_FUNC_ReadLine((char)ch)) {
            case b_ALPHA :
            case b_DIGIT :
			case b_NUMBER_WORK :
                break;
            case b_POINT :
                POINT++;
                break;
            case b_LABEL_END :
                D_POINT++;
                break;
            case b_BRACKET :
				if (STRING == FALSE){
					if (ch == '('){
						BRACKET++;
						
						/* CHECK IF THERE CLOSE */
						ch = oneLine[len-ONE];
						if (ch == ')') {
							d.ENCODING[d.WORDS++] = (char) BRECKT_N; 
						} else {
							d.tab -> E_ERR +=
							err_in_word
							(pERR_ID_wrd_CLOSE,d_ptr,&oneLine[--ind]);
						}
					} else if (ch == ')' && ind == len)
						oneWord[cpWord-ONE] = '\0'; /* CLOSE ONE WORD */
					  else
						d.tab -> E_ERR += err_print(pERR1_ID,d_ptr,ch);
				}
                break;
            case b_ARR_STR :
                STRING++;

				/* CHECK IF THERE CLOSE */
				ch = oneLine[len-ONE];
				if (ch == '\"')
					;
				else {
					d.tab -> E_ERR += 
					err_in_word(pERR_ID_wrd_CLOSE,d_ptr,&oneLine[--ind]);

				}

                break;
            case b_COMMA :
				COMMA++;
				if (STRING == FALSE){
					d.ENCODING[d.WORDS++] = (char) COMMA_N;

					/*if (COMMA > ONE  && WordArr[d.WORDS-TWO] != DIGIT_A )
						d.tab -> E_ERR += err_print(pERRduble_ID,d,ch);*/
				}		
                break;
            case b_SPACE :
                if (STRING == FALSE){
                    oneWord[cpWord-ONE] = '\0'; /* CLOSE ONE WORD */
					
					if (return_sum(d_ptr,oneWord) == ERR_ENCODE){
						/* printf("-> %s\n", oneWord); */
						return d;
					}

                    cpWord = ZERO;
                    d.WORDS++;
                }
                break;
            default :
				d.tab -> E_ERR += err_print(pERRchar_ID,d_ptr,ch);
        } /* END OF SWITCH */

			if ( d.WORDS+FIRST == FIRST && (c_case == b_NUMBER_WORK ||
				 c_case == b_BRACKET || c_case == b_ARR_STR || 
				 c_case == b_COMMA))
				d.tab -> E_ERR += err_print(pERR1_ID,d_ptr,ch);
			
			/* CHECK DOUBLE LATTER ERR */
			if ( STRING == FALSE && (D_POINT > ONE ||
				 POINT > ONE || BRACKET > ONE) ) {
				d.tab -> E_ERR += err_print(pERRduble_ID,d_ptr,ch);
			}
        } /* END OF WHILE */

        /* ADD SUM TO LAST WORD */
		oneWord[cpWord] = '\0'; 
        return_sum(d_ptr,oneWord);
	
	checkENCODEdata(d);
	
	return d;
}

int case_FUNC_ReadLine (char ch)
{
	int c_case = EMPTY;
	
	if (isalpha(ch))
    	c_case = b_ALPHA; /* 'A-Z' && 'a-z' */
    else if (isdigit(ch))
        c_case = b_DIGIT; /* '0-9' */
    else if (ch == '.')
        c_case = b_POINT;
    else if ( ch == '-' || ch == '+' || ch == '#' )
        c_case = b_NUMBER_WORK;
    else if ( ch == ':' )
        c_case = b_LABEL_END;
    else if ( ch == '(' || ch == ')')
        c_case = b_BRACKET;
    else if (ch == '\"')
        c_case = b_ARR_STR;
    else if (ch == ',')
        c_case = b_COMMA;
    else if (ch == ' ' || ch == '\t' || ch == '\n')
        c_case = b_SPACE;
    else {
        c_case = b_FALSE;
	}

	return c_case;
}

int case_FUNC_return_sum (char * Str)
{
	int c_case = EMPTY;
	int len = strlen(Str);
	char c0 = Str[ZERO], c1 = Str[ONE], cEnd = Str[len-ONE];
	
	

	if (c0 == '.' && isalpha(cEnd)){
        c_case = b_DATA;
    } else if (isdigit(c0) || c0 == '#' || c0 == '-' || c0 == '+') {
        c_case = b_NUMBER_WORK;
    } else if (c0 == '\"' && cEnd == '\"') {
        c_case = b_ARR_STR;
    } else if (isalpha(c0) && cEnd == ':') {
        c_case = b_LABEL_END;
    } else if (c0 == 'r' && isdigit(c1) && len < THREE ) {
    	c_case = b_REG;
    } else if (isalpha(c0) && (isalpha(cEnd) || isdigit(cEnd))) {
    	c_case = b_OPT;
    } else if (c0 == ',' || c0 == '(' || c0 == ' ') {
        c_case = b_SPACE; 
    } else {
        c_case = b_FALSE; 
    }

	return c_case;
}

int return_sum (theDATA * d, char Str [MAX_WORD])
{
    int wordCunter = d -> WORDS;

    int len = strlen(Str);
	int c_case = ZERO;
	char c0 = Str[ZERO], cEnd = Str[len-ONE];

	int d_data; /* ENTER DATA FROM DIGIT */
	int IC = IC_ID_LINE, DC = DC_ID_LINE;
    
    int sum = ERR_ENCODE;

	/* RETURN THE CASE */
	c_case = case_FUNC_return_sum(Str);
	if (c_case == b_FALSE){
		d -> ENCODING[wordCunter] = (char) sum;
		return sum;
	}
	
	switch(c_case) {
		case b_DATA : /* DATA LINE FOUND CHECK WHICH! */
			sum = wordsToData(Str+ONE); /* SKIP ON POINT '.' */
			d -> ENCODING[wordCunter] = (char) sum;

			if (d -> IC_DC <= EMPTY) {
				if (sum != ERR_ENCODE) {
					d -> IC_DC = DC;
					d -> OPT_DATA = sum;
				} else {
					d -> tab -> E_ERR += err_in_word(pERR_ID_wrd_data,d,Str);
				}
			} else {
				d -> tab -> E_ERR += err_in_word(pERR_ID_wrd_double,d,Str);
			}
        	break;
		case b_NUMBER_WORK : /* DIGIT FOUND ENTER TO ARRAY! */
			sum = d -> ENCODING[wordCunter] = (char) DIGIT_A;
			if (isdigit(c0) && isdigit(cEnd)){		
				d -> NUM_ARR[d -> NUM] = d_data = atoi(Str);
				d -> NUM++;
			} else {
				d -> NUM_ARR[d -> NUM] = d_data = return_num(Str,d);
				d -> NUM++;
			}	
        	break;
		case b_ARR_STR : /* STRING FOUND COPY STRING! */
			sum = d -> ENCODING[wordCunter] = (char) STRING_A;
			d -> STRING = (char *) malloc (len);
			strncpy(d -> STRING,Str,len);
			d -> STRING[len] = '\0';
        	break;
		case b_LABEL_END : /* NEW LABEL FOUND SAVE THE LABEL! */
			Str[len-ONE] = '\0';
			/* CHECK IF NOT ONE OF THE SAVED WORD */
			if ( wordsToOpt(Str) == LABEL_NL ){
				sum = d -> ENCODING[wordCunter] = (char) LABEL_N;
				strncpy(d -> NEW_LABEL,Str,len);
			} else {
				d -> tab -> E_ERR += err_in_word(pERR_ID_wrd_Saved,d,Str);
			}
        	break;
		case b_REG : /* REGISTER FOUND! */
			sum = wordsToReg(Str);
			if (sum != ERR_ENCODE){
				d -> ENCODING[wordCunter] = (char) sum;
				d -> REG_ARR [d -> REG++] = sum - REGISTER_N;
			} else {
				; /* ADD REG ERR */ 
			}
        	break;
		case b_OPT : /* FOUND WORD GO CHECK! */
			sum = wordsToOpt(Str);	
			d -> ENCODING[wordCunter] = (char) sum;

			if (sum == ERR_ENCODE){
				d -> tab -> E_ERR += err_in_word(pERR_ID_wrd_Saved,d,Str);
			} else if (sum == LABEL_NL) {
				strncpy(d -> LABEL_ARR[d -> LABEL],Str,len);
				d -> LABEL_ARR[d -> LABEL++][len] = '\0';
			} else { 
				if (d -> IC_DC >= EMPTY &&
					d -> OPT_DATA == EMPTY_AND_BLOCK) { 
						d -> IC_DC = IC;
						d -> OPT_DATA = sum;
				} else
					d -> tab -> E_ERR += err_in_word(pERR_ID_wrd_double,d,Str);
			}
        	break;
		case b_SPACE : /* SKIP ON CHAR ',' */
			sum = COMMA_N;
			return_sum (d,Str+ONE);
        	break;
		default :
			if (strlen(Str) > ZERO)
				d -> tab -> E_ERR += err_in_word(0,d,Str);
			else
				return ZERO;
	}

    return sum;
}

/* RETURN DIGIT DATA FROM CHAR TO INT */
int return_num (char num [MAX_WORD], theDATA * d)
{
	int sum = ZERO;
	int len = strlen(num);
	char c0 = num[ZERO], cEnd = num[len-ONE];

	if (len < TWO && !isdigit(c0) ){
		d->tab -> E_ERR += err_print(pERR1_ID,d,c0);
	}

	if ( ( isdigit(c0) || c0 == '-' ) && isdigit(cEnd) )
		return sum = atoi(num);
	else{
		if ( ( c0 == '#' || c0 == '+' ) && isdigit(cEnd) )
			return return_num (num+ONE,d);
		else{
			/* ERR = TRUE;*/
			return 0;
		}
	}
}

int wordsToData ( char Str [MAX_WORD] )
{

    int sum = ERR_ENCODE;

            if (!strcmp(Str,dataSaveWord[d_ent]))
                sum = EN_N;
            else if (!strcmp(Str,dataSaveWord[d_ext]))
                sum = EX_N;
            else if (!strcmp(Str,dataSaveWord[d_data]))
                sum = DIGIT_N;
            else if (!strcmp(Str,dataSaveWord[d_str]))
                sum = STRING_N;
            else
                ;
    
    return sum;
}

int wordsToReg ( char Str [MAX_WORD] )
{

    int sum = ERR_ENCODE;

            if (!strcmp(Str,regSaveWord[reg_r0]))
                sum = REGISTER_N + reg_r0;
            else if (!strcmp(Str,regSaveWord[reg_r1]))
                sum = REGISTER_N + reg_r1;
            else if (!strcmp(Str,regSaveWord[reg_r2]))
                sum = REGISTER_N + reg_r2;
            else if (!strcmp(Str,regSaveWord[reg_r3]))
                sum = REGISTER_N + reg_r3;
			else if (!strcmp(Str,regSaveWord[reg_r4]))
                sum = REGISTER_N + reg_r4;
			else if (!strcmp(Str,regSaveWord[reg_r5]))
                sum = REGISTER_N + reg_r5;
			else if (!strcmp(Str,regSaveWord[reg_r6]))
                sum = REGISTER_N + reg_r6;
			else if (!strcmp(Str,regSaveWord[reg_r7]))
                sum = REGISTER_N + reg_r7;
            else
                ;
    return sum;
}

int wordsToOpt (char Str [MAX_WORD] )
{

    int sum = ERR_ENCODE;

    if (!strcmp(Str,optSaveWord[V_MOV]))
        sum = V_MOV;
    else if (!strcmp(Str,optSaveWord[V_CMP]))
        sum = V_CMP;
    else if (!strcmp(Str,optSaveWord[V_ADD]))
        sum = V_ADD;
    else if (!strcmp(Str,optSaveWord[V_SUB]))
        sum = V_SUB;
    else if (!strcmp(Str,optSaveWord[V_NOT]))
        sum = V_NOT;
    else if (!strcmp(Str,optSaveWord[V_CLR]))
        sum = V_CLR;
    else if (!strcmp(Str,optSaveWord[V_LEA]))
        sum = V_LEA;
    else if (!strcmp(Str,optSaveWord[V_INC]))
        sum = V_INC;
    else if (!strcmp(Str,optSaveWord[V_DEC]))
        sum = V_DEC;
    else if (!strcmp(Str,optSaveWord[V_JMP]))
        sum = V_JMP;
    else if (!strcmp(Str,optSaveWord[V_BNE]))
        sum = V_BNE;
    else if (!strcmp(Str,optSaveWord[V_RED]))
        sum = V_RED;
    else if (!strcmp(Str,optSaveWord[V_PRN]))
        sum = V_PRN;
    else if (!strcmp(Str,optSaveWord[V_JSR]))
        sum = V_JSR;
    else if (!strcmp(Str,optSaveWord[V_RTS]))
        sum = V_RTS;
    else if (!strcmp(Str,optSaveWord[V_STOP]))
        sum = V_STOP;
    else if (wordsToReg(Str) == ERR_ENCODE && wordsToData(Str) == ERR_ENCODE)
         sum = LABEL_NL;
	else
		;

    return sum;
}
