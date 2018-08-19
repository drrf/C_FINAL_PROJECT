/*
 *          File: arr.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"

/* copy from file to array and fix space to read more easily */
int cpToArr (char str [MAX_NAME])
{
	AS_DATA S;							/* SVAE ALL AS DATA IN THIS TYPE */
    int checkAS = ZERO;	    			/* CHECK ".as" FORMAT FILE */	
    int Flen = ZERO;					/* FILE LENGTH IN BYTES */
    int NLine = ZERO;                   /* THE NUMBER OF THE LINES IN FILE */
    int LineArr[MAX_LINE] = {'\0'};     /* ARRAY OF POINTER TO LINES IN FILE */
	int FName_len = strlen(str);		/* THE LENGTH OF FILE NAME */
    char * FileN = str;                 /* FILE NAME POINTER */
    FILE * fp;							/* POINTER TO FILE */
    char * FileC = (char *) malloc(Flen); /* ARRAY FOR COPY FILE CONTENT */

	/* CHECK IF ARGV INPUT FILE AIN'T ADD ".as" TO THE END */
	checkAS = memcmp(str+(FName_len-FILE_FORMAT_LEN),FILE_AS,FILE_FORMAT_LEN);
	if (checkAS){
		strcat(FileN, FILE_AS);
	}

	/* OPEN THE ".as" FILE */   
    fp = fopen (FileN,"r+");                  
    if( fp==NULL ) {
	    checkAS = err_in_arr(pERR_ID_OPEN_F,str);
    } else {
        fseek (fp, ZERO, SEEK_END);
        Flen = ftell(fp);
        rewind(fp);
        if (Flen == EMPTY) { /* IF  FILE IS EMPTY */
		    checkAS = err_in_arr(pERR_ID_F_EMPTY,FileN);
	    } else {
		    int ch=ZERO, SPACE=ZERO, InString=ZERO, ind=ZERO, ERR=ZERO;
		    FileC = (char *) realloc(FileC,Flen);
            LineArr[NLine++] = ind;               /* POINTER TO FIRST LINE */

            /* COPY FILE CONTENT TO ARRAY */
		    while (ONE) {
                ch = fgetc(fp);
			    if ( feof (fp) ){
                    FileC[ind] = '\0';
				    break;
			    }

                /* COUNT THE LINES IN FILE */
                if (ch == '\n'){
                    SPACE = InString = ZERO;
                    if (NLine > MAX_LINE){
                        err_in_arr(pERR_ID_MAX_LINE,FileN);
						ERR++;
                        break;
                    } else {
                        LineArr[NLine++] = ind+ONE;     /* ADD THE NEW LINE */ 
                    } 
                }

                /* MAKE SURE HAVE SPACE BEFORE '(' || ',' */
                if (ch == '(' || ch == ','){
                    if (FileC[ind-ONE] != ' ')
                        FileC[ind++] = ' ';  
                }        
                
                /* CHECK DOUBLE SPACE */
                if ((ch == ' ' || ch == '\t') && InString == FALSE) {          
                    SPACE++;
                } else {
                    SPACE = ZERO;
                }

                /* PRINT TO ARRAY */
                if (SPACE < ONE){
                    FileC[ind++] = (char)ch; 	/* ADD CHAR */
                }else if ((SPACE < TWO) && FileC[ind-ONE] != ',' && FileC[ind-ONE] != '\n')
                    FileC[ind++] = ' '; 		/* ADD SPACE */
                else
                    ;							/* DO NOTHING */
		    } /* END OF WHILE */

			/* ADD THE LAST LINE */
			LineArr[NLine++] = ind+ONE;

			/* IF ERR OR TO MUCH LINES DO NOTHING ELSE START ALGO */
			if (ERR!=ZERO || NLine > MAX_LINE)
				;
			else { 
		        S.SLen = ind;
		        S.SLines = NLine;
		        S.SLineArr = &LineArr[ZERO];
				S.Code = &FileC[ZERO];
				S.FileN = &str[ZERO];
				
		        checkAS = algo1(S); /* START algo.c FILE */

		    }  			
        }
	/* FREE AND CLOSE */
	free(FileC);
   	fclose(fp);
    }

	if (checkAS)
		return 1;
	else
    	return 0;
}
