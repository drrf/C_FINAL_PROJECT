/*
 *          File: parsing.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

int ERR = 0;

enum ReadLine { b_ALPHA = 1, b_DIGIT = 2, b_POINT = 3, b_NUMBER_WORK = 4, b_LABEL_END = 5, b_BRACKET = 6, b_ARR_STR = 7, b_ARR_NUM = 8, b_SPACE = 9 };

/* START READ LINE & BREAK TO WORD */
theDATA ReadLine (char oneLine [MAX_LINE], int i, theDATA d)
{
	int POINT = 0, D_POINT = 0, STRING = 0, QUOTA = 0;
    int c, c_case = 0, ind = 0, cpWord = 0, sum = 0;
	int len = strlen(oneLine);
	int t = 0;
	

	char oneWord [MAX_WORD];
    int WordArr [MAX_WORD];
	static int digitArr [MAX_WORD]; /* STATIC FOR NOT REMOVE */

	/* SAVE DATA FROM R1 */
	/* theDATA z; */
	void * d_ptr = &d;

	/* JUMP OVER COMMENT & EMPTY LINE */
    c = oneLine[ind];
    if (c == ';' || strlen(oneLine) < 1){
		d.Line = 0;
		return d;
	}

    /* ERROR IN THE FIRST CHAR */
    if ( isdigit(c) || c == '\"' || c == ':' || c == ',' || c == '(' || c == ')' || c == '#' || c == '-' || c == '+' )
    	pERR1st (ERR = TRUE,c,i+1);

    /* ERROR IN THE LAST CHAR */
    c = oneLine[len-1];
    if ( c == ':' || c == '#' || c == ',' || c == '+' || c == '-' ){
    	pERR1st (ERR = TRUE,c,i+1);
	}
	
	/* COPY AND RESET DATA */
	d.Line = i;
	d.LINE = &oneLine[0];
	d.NUM_ARR = &digitArr[0];
	d.WORDS = 0; d.NUM = 0; d.LABEL = 0, d.IC_DC = 0; d.REG = 0;
	d.OPT_DATA = EMPTY_AND_BLOCK; /* EMPTY OPT OR DATA FOR NOW */
	d.ENCODING[0] = '\0';
	

    /* CHECK FOR ERR AND CUT FOR WORDS */
        while (ind < len){
			c = oneLine[ind++];
            oneWord[cpWord++] = (char)c;
            if (isalpha(c))
                c_case = b_ALPHA; /* A-Z && a-z */
            else if (isdigit(c))
                c_case = b_DIGIT; /* 0-9 */
            else if (c == '.')
                c_case = b_POINT;
            else if ( c == '-' || c == '+' || c == '#' )
                c_case = b_NUMBER_WORK;
            else if ( c == ':' )
                c_case = b_LABEL_END;
            else if ( c == '(' || c == ')')
                c_case = b_BRACKET;
            else if (c == '\"')
                c_case = b_ARR_STR;
            else if (c == ',')
                c_case = b_ARR_NUM;
            else if (c == ' ' || c == '\n')
                c_case = b_SPACE;
            else
                c_case = 0;

            switch(c_case) {
                case b_ALPHA :
                    break;
                case b_DIGIT :
                    break;
                case b_POINT :
                    POINT++;
                    if (POINT > ONE && STRING == FALSE) pERRduble (c,i+1);
                    break;
                case b_NUMBER_WORK :
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
                    break;
                case b_LABEL_END :
                    D_POINT++;
                    if (D_POINT > ONE && STRING == FALSE) pERRduble (c,i+1);
                    break;
                case b_BRACKET :
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
					if (STRING == FALSE){
						if (c == ')' && ind == len)
							oneWord[cpWord-1] = '\0'; /* CLOSE ONE WORD */
						else
							;/* pERRword(oneWord,i+1);*/
					}
                    break;
                case b_ARR_STR :
                    STRING++;
                    QUOTA++;
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
                    break;
                case b_ARR_NUM :
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
					if (STRING == FALSE){
						d.ENCODING[d.WORDS] = (char) COMMA_N;
						WordArr[d.WORDS++] = COMMA_N;
					}
                    break;
                case b_SPACE :
                    if (STRING == FALSE){
                        oneWord[cpWord-1] = '\0'; /* CLOSE ONE WORD */
                        printf("-> %s\n", oneWord );

                        if (ERR == FALSE && c != EOF)
                            sum = return_sum(d_ptr,oneWord,WordArr);
                        
                        if (sum == ERR_ENCODE){
                            pERRword(oneWord,i+1);
                            ERR = TRUE;
                        }
                        cpWord = 0;
                        d.WORDS++;
                    }
                    break;
                default :
                    ERR = TRUE;
                    pERRchar (c,i+1);
            }
        }



        /* CHECK IF THERE IS START & END OF QUOTA */
        if (STRING >= ONE && QUOTA != TWO)
            pERRmiss (c,i+1);

        /* PRINT LAST WORD */
        oneWord[cpWord] = '\0'; 
        printf("%s <-\n", oneWord );

        /* ADD SUM TO LAST WORD */
        if (ERR == FALSE && c != EOF){
            sum = return_sum(d_ptr,oneWord,WordArr);

			/* d[t].ENCODE = &WordArr[0];*/
			/* z = d[t];*/

			/* START THE DATA FILE */
			/* my_data(d,t);
			 dataRead(z);*/
        }

        if (sum == ERR_ENCODE){
            pERRword(oneWord,i+1);
            ERR = TRUE;
        }

	
	return d;
}

int return_sum (theDATA * d, char Str [MAX_WORD], int WordArr [MAX_WORD])
{
    int wordCunter = d -> WORDS;
    int len = strlen(Str);
	int c_case = 0;
	char c0 = Str[0], c1 = Str[1], cEnd = Str[len-1];
	int d_data; /* ENTER DATA FROM DIGIT */
	int IC = 1, DC = -1;
    
    int sum = ERR_ENCODE;
    int label = LABEL_N;
    int r = REGISTER_N;
    int digit = DIGIT_A;
	int string = STRING_A;
    int comma = COMMA_N;
	
    if (c0 == '.' && isalpha(cEnd)){
        c_case = 1;
    } else if (isdigit(c0) || c0 == '#' || c0 == '-' || c0 == '+') {
        c_case = 2;
    } else if (c0 == '\"' && cEnd == '\"') {
        c_case = 3;
    } else if (isalpha(c0) && cEnd == ':') {
        c_case = 4;
    } else if (c0 == 'r' && isdigit(c1) && len < 3 ) {
    	c_case = 5;
    } else if (isalpha(c0) && (isalpha(cEnd) || isdigit(cEnd))) {
    	c_case = 6;
    } else if (c0 == ',' || c0 == '(') {
        c_case = 7; 
    } else {
        sum = WordArr[wordCunter] = sum;
		d -> ENCODING[wordCunter] = (char) sum;
    }


	switch(c_case) {
		case 1 : /* DATA ENTER FOUND CHECK WHICH! */
			sum = wordsToData(wordCunter,Str,WordArr);
			d -> ENCODING[wordCunter] = (char) sum;
			if (d -> IC_DC <= EMPTY) {
					d -> IC_DC = DC;
					d -> OPT_DATA = sum;
				} else
					printf("ERR FOUND IN IC_DC!\n" );
        	break;
		case 2 : /* DIGIT FOUND ENTER TO ARRAY! */
			sum = WordArr[wordCunter] = digit;
			d -> ENCODING[wordCunter] = (char) digit;
			if (isdigit(c0) && isdigit(cEnd)){		
				d -> NUM_ARR[d -> NUM] = d_data = atoi(Str);
				d -> NUM++;
			} else {
				d -> NUM_ARR[d -> NUM] = d_data = return_num(Str);
				d -> NUM++;
			}	
        	break;
		case 3 : /* STRING FOUND COPY STRING! */
			sum = WordArr[wordCunter] = string;
			d -> ENCODING[wordCunter] = (char) string;
			d -> STRING = (char *) malloc (len);
			strncpy(d -> STRING,Str,len);
			d -> STRING[len] = '\0';
        	break;
		case 4 : /* NEW LABEL FOUND SAVE THE LABEL! */
			sum = WordArr[wordCunter] = label;
			d -> ENCODING[wordCunter] = (char) label;
			strncpy(d -> NEW_LABEL,Str,len-1);
			d -> NEW_LABEL[len-1] = '\0';
        	break;
		case 5 : /* REGISTER FOUND! */
			sum = WordArr[wordCunter] = r+atoi(Str+1);
			if (sum - r >= reg_r0 && sum - r <= reg_r7){
				d -> ENCODING[wordCunter] = (char) sum;
				d -> REG_ARR [d -> REG++] = sum - r;
			} else {
				printf("ERR FOUND IN REGISTER IS SMALL OR BIG!\n" );
			}
        	break;
		case 6 : /* CMD OR LABEL FOUND GO CHECK! */
			sum = wordsToOpt(wordCunter,Str,WordArr);
			d -> ENCODING[wordCunter] = (char) sum;
			if (sum == -1){
				strncpy(d -> LABEL_ARR[d -> LABEL],Str,len);
				d -> LABEL_ARR[d -> LABEL++][len] = '\0';
			} else { 
				if (d -> IC_DC >= EMPTY && d -> OPT_DATA == EMPTY_AND_BLOCK){ 
					d -> IC_DC = IC;
					d -> OPT_DATA = sum;
				} else {
					printf("ERR FOUND IN IC_DC!\n" );
				};
			}
        	break;
		case 7 : /* SKIP ON CHAR ',' */
			sum = comma;
        	return_sum(d,Str+1,WordArr);  
        	break;
		default :
			ERR = TRUE;
        	printf("ERR FOUND HERE!\n" );
	}

    return sum;
}

/* RETURN DIGIT DATA FROM CHAR TO INT */
int return_num (char num [MAX_WORD])
{
	int sum = 0;
	int len = strlen(num);
	char c0 = num[0], cEnd = num[len-1];

	if ( ( isdigit(c0) || c0 == '-' ) && isdigit(cEnd) )
		return sum = atoi(num);
	else{
		if ( ( c0 == '#' || c0 == '+' ) && isdigit(cEnd) )
			return return_num (num+1);
		else{
			ERR = TRUE;
			return 0;
		}
	}
}

int wordsToData (int num, char Str [MAX_WORD], int arr [MAX_WORD] )
{
    char en[]       = ".entry";
    char ex[]       = ".extern";
    char data[]     = ".data";
    char string[]   = ".string";

    int sum = ERR_ENCODE;

            if (strcmp(Str,en) == 0)
                sum = EN_N;
            else if (strcmp(Str,ex) == 0)
                sum = EX_N;
            else if (strcmp(Str,data) == 0)
                sum = DIGIT_N;
            else if (strcmp(Str,string) == 0)
                sum = STRING_N;
            else
                ;
    
    arr[num] = sum;

    puts("");

    return sum;
}

int wordsToOpt (int num, char Str [MAX_WORD], int arr [MAX_WORD] )
{
    char mov[]      = "mov";
    char cmp[]      = "cmp";
    char add[]      = "add";
    char sub[]      = "sub";
    char not[]      = "not";
    char clr[]      = "clr";
    char lea[]      = "lea";
    char inc[]      = "inc";
    char dec[]      = "dec";
    char jmp[]      = "jmp";
    char bne[]      = "bne";
    char red[]      = "red";
    char prn[]      = "prn";
    char jsr[]      = "jsr";
    char rts[]      = "rts";
    char stop[]      = "stop";

    int sum = ERR_ENCODE;

    if (strcmp(Str,mov) == 0)
        sum = V_MOV;
    else if (strcmp(Str,cmp) == 0)
        sum = V_CMP;
    else if (strcmp(Str,add) == 0)
        sum = V_ADD;
    else if (strcmp(Str,sub) == 0)
        sum = V_SUB;
    else if (strcmp(Str,not) == 0)
        sum = V_NOT;
    else if (strcmp(Str,clr) == 0)
        sum = V_CLR;
    else if (strcmp(Str,lea) == 0)
        sum = V_LEA;
    else if (strcmp(Str,inc) == 0)
        sum = V_INC;
    else if (strcmp(Str,dec) == 0)
        sum = V_DEC;
    else if (strcmp(Str,jmp) == 0)
        sum = V_JMP;
    else if (strcmp(Str,bne) == 0)
        sum = V_BNE;
    else if (strcmp(Str,red) == 0)
        sum = V_RED;
    else if (strcmp(Str,prn) == 0)
        sum = V_PRN;
    else if (strcmp(Str,jsr) == 0)
        sum = V_JSR;
    else if (strcmp(Str,rts) == 0)
        sum = V_RTS;
    else if (strcmp(Str,stop) == 0)
        sum = V_STOP;
    else
         sum = -1;

    arr[num] = sum;

    return sum;
}

