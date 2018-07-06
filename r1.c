/*
 *          File: r1.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: asmbler
 * ----------------------------------------------------------------
 */

#include "myas.h"

int ERR = 0;

int r1 (SLine S)
{
	int i = 0, sum = 0;
	char FileArr [MAX_CODE];
	char oneLine [MAX_LINE];

	/* COPY CODE FROM S TO FileArr */
	strncpy(FileArr,S.Code,S.SLen+1);

	/* FOR TESTING 
    printf("%s\n", FileArr+5);
	printf("%c\n", FileArr[5]);
	printf("%d\n", sizeof(S));
	return 0;*/

	for (i = 0; i < S.SLines-1; i++){
    	strncpy(oneLine, FileArr+S.SLineArr[i], S.SLineArr[i+1]-1); /* -1 remove '\n' */
    	oneLine[(S.SLineArr[i+1]-S.SLineArr[i]-1)] = '\0';          /* add EOF */

		/* START BREAK TO WORD */
		sum = breakToWords(oneLine,i+1);

		if (sum == 0) {
			puts("");
    		printf("%d) %s\n", i+1, oneLine);
    		puts("");
		}
	}

	if (ERR == FALSE)
		printf("\nGOOD FOUND 0 ERR IN THE CODE!\n");
	else
		printf("\nFOUND ERR IN THE CODE CAN'T CONTINUE!\n");

    printf("\n****************************************\n");
    printf("there is %d bytes in this array!\n", S.SLen);
    printf("there is %d lines in this array!\n", S.SLines-1); 
    printf("****************************************\n");

    puts("");
	
	return 0;

}

int breakToWords (char oneLine [MAX_LINE], int i)
{
	int POINT = 0, D_POINT = 0, STRING = 0, QUOTA = 0;
    int c, c_case = 0, ind = 0, cpWord = 0, sum = 0;
	int len = strlen(oneLine);

	char oneWord [MAX_WORD];
    int WordArr [MAX_WORD];

	/* SAVE DATA FROM R1 */
	theDATA d;
	void * d_ptr = &d;

	/* JUMP OVER COMMENT & EMPTY LINE */
    c = oneLine[ind];
    if (c == ';' || strlen(oneLine) < 1) return 1;

    /* ERROR IN THE FIRST CHAR */
    if ( isdigit(c) || c == '\"' || c == ':' || c == ',' || c == '(' || c == ')' || c == '#' || c == '-' || c == '+' )
    	pERR1st (ERR = TRUE,c,i+1);

    /* ERROR IN THE LAST CHAR */
    c = oneLine[len-1];
    if ( c == ':' || c == '#' || c == ',' || c == '+' || c == '-' ){
    	pERR1st (ERR = TRUE,c,i+1);
	}
	
	/* COPY LINE TO DATA */
	d.LINE = &oneLine[0];
	d.WORDS = 0;
	

    /* CHECK FOR ERR AND CUT FOR WORDS */
        while (ind < len){
			c = oneLine[ind++];
            oneWord[cpWord++] = (char)c;
            if (isalpha(c))
                c_case = 1; /* A-Z && a-z */
            else if (isdigit(c))
                c_case = 2; /* 0-9 */
            else if (c == '.')
                c_case = 3;
            else if ( c == '-' || c == '+' || c == '#' )
                c_case = 4;
            else if ( c == ':' )
                c_case = 5;
            else if ( c == '(' || c == ')')
                c_case = 6;
            else if (c == '\"')
                c_case = 7;
            else if (c == ',')
                c_case = 8;
            else if (c == ' ' || c == '\n')
                c_case = 9;
            else
                c_case = 0;

            switch(c_case) {
                case 1 : /*printf("ALPHA!\n" );*/
                    break;
                case 2 : /*printf("DIGIT!\n" );*/
                    break;
                case 3 : /*printf("POINT!\n" );*/
                    POINT++;
                    if (POINT > ONE && STRING == FALSE) pERRduble (c,i+1);
                    break;
                case 4 : /*printf("NUMBER WORK!\n" );*/
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
                    break;
                case 5 : /*printf("LABEL END!\n" );*/
                    D_POINT++;
                    if (D_POINT > ONE && STRING == FALSE) pERRduble (c,i+1);
                    break;
                case 6 :
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
					if (STRING == FALSE){
						if (c == ')' && ind == len)
							oneWord[cpWord-1] = '\0'; /* CLOSE ONE WORD */
						else
							;/* pERRword(oneWord,i+1);*/
					}
                    break;
                case 7 : /*printf("ARRAY OF STRING!\n" );*/
                    STRING++;
                    QUOTA++;
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
                    break;
                case 8 : /*printf("ARRAY OF NUMBER!\n" );*/
                    if (d.WORDS+FIRST == FIRST) pERR1st (ERR = TRUE,c,i+1);
					if (STRING == FALSE){
						WordArr[d.WORDS++] = COMMA_N;
					}
                    break;
                case 9 : /*printf("SPACE!\n" );*/
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

			d.ENCODE = &WordArr[0];

			/*for (ind = 0; ind <= d.WORDS; ind++){
				d.ENCODE[ind] = WordArr[ind];
			}*/

			/* START THE DATA FILE */
			dataRead(d);
        }

        if (sum == ERR_ENCODE){
            pERRword(oneWord,i+1);
            ERR = TRUE;
        }

	return 0;
}

int return_sum (theDATA * d, char Str [MAX_WORD], int WordArr [MAX_WORD])
{
    int wordCunter = d -> WORDS;
    int len = strlen(Str);
	char c0 = Str[0], c1 = Str[1], cEnd = Str[len-1];
    
    int sum = ERR_ENCODE;
    int label = LABEL_N;
    int r = REGISTER_N;
    int digit = DIGIT_N;
	int string = STRING_N;
    int comma = COMMA_N;
	
    if (c0 == '.' && isalpha(cEnd)){
        sum = wordsToData(wordCunter,Str,WordArr);
    } else if (isdigit(c0) || c0 == '#' || c0 == '-' || c0 == '+') {
        sum = WordArr[wordCunter] = digit;
    } else if (c0 == '\"' && cEnd == '\"') {
        sum = WordArr[wordCunter] = string;
		strncpy(d -> STRING,Str,len);
		d -> STRING[len] = '\0';
    } else if (isalpha(c0) && cEnd == ':') {
        sum = WordArr[wordCunter] = label;
		strncpy(d -> NEW_LABEL,Str,len-1);
		d -> NEW_LABEL[len-1] = '\0';
    } else if (c0 == 'r' && isdigit(c1) && len < 3 ) {
        sum = WordArr[wordCunter] = r+atoi(Str+1);
    } else if (isalpha(c0) && (isalpha(cEnd) || isdigit(cEnd))) {
        sum = wordsToOpt(wordCunter,Str,WordArr);
    } else if (c0 == ',' || c0 == '(') {
        sum = comma;
        return_sum(d,Str+1,WordArr);  /* SKIP ON CHAR ',' */
    } else {
        sum = WordArr[wordCunter] = sum;
    }

    return sum;
}

int wordsToData (int num, char Str [MAX_WORD], int arr [MAX_WORD] )
{
    char en[]       = ".entry";
    char ex[]       = ".extern";
    char data[]     = ".data";
    char string[]   = ".string";

    int sum = ERR_ENCODE;

            if (strcmp(Str,en) == 0)
                sum = -99;
            else if (strcmp(Str,ex) == 0)
                sum = -999;
            else if (strcmp(Str,data) == 0)
                sum = -10;
            else if (strcmp(Str,string) == 0)
                sum = -15;
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
        sum = mov_V;
    else if (strcmp(Str,cmp) == 0)
        sum = cmp_V;
    else if (strcmp(Str,add) == 0)
        sum = add_V;
    else if (strcmp(Str,sub) == 0)
        sum = sub_V;
    else if (strcmp(Str,not) == 0)
        sum = not_V;
    else if (strcmp(Str,clr) == 0)
        sum = clr_V;
    else if (strcmp(Str,lea) == 0)
        sum = lea_V;
    else if (strcmp(Str,inc) == 0)
        sum = inc_V;
    else if (strcmp(Str,dec) == 0)
        sum = dec_V;
    else if (strcmp(Str,jmp) == 0)
        sum = jmp_V;
    else if (strcmp(Str,bne) == 0)
        sum = bne_V;
    else if (strcmp(Str,red) == 0)
        sum = red_V;
    else if (strcmp(Str,prn) == 0)
        sum = prn_V;
    else if (strcmp(Str,jsr) == 0)
        sum = jsr_V;
    else if (strcmp(Str,rts) == 0)
        sum = rts_V;
    else if (strcmp(Str,stop) == 0)
        sum = stop_V;
    else
         sum = -1;

    arr[num] = sum;

    return sum;
}
