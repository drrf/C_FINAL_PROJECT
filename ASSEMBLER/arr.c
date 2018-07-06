
/*
 *          File: arr.c
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: asmbler
 * ----------------------------------------------------------------
 */
/* copy from file to array and fix what u can */

#include "myas.h"

int cpToArr (int x, char str [MAX_NAME])
{
	SLine S;								/* SVAE ALL DATA IN THIS TYPE */
    int i = 0;
    int sum = 0;
    char * FileN = str;                     /* FILE NAME POINTER */
    FILE * fp = fopen (FileN,"r");	        /* POINTER TO FILE */
    int Flen = 0;					        /* FILE LENGTH IN BYTES */
    int NLine = 0;                          /* THE NUMBER OF THE LINES IN FILE */
    int LineArr[MAX_LINE] = {'\0'};         /* ARRAY OF POINTER TO LINES IN FILE */
    char * FileC = (char *) malloc(Flen);   /* ARRAY FOR COPY FILE CONTENT */

    for (i = x-3; i < x; i++ ){
        sum+=str[i];
    }
        
    puts("");
    /* IF SUM FOUND ASMBLER FILE */
    if (sum == AS){                         
        if( fp==NULL ) {
		    fprintf(stderr, "ERROR: Can't open the file %s!\n", str);
			return 0;
	    } else {
            fseek (fp, 0, SEEK_END);
            Flen = ftell(fp);
            rewind(fp);
            if (Flen == 0) {                /* IF  FILE IS EMPTY */
			    fprintf(stderr, "ERROR: File '%s' is empty!\n",FileN);
		    } else {
			    int c=0, space=0, InString=0;
			    FileC = (char *) realloc(FileC,Flen);
                i = 0;
                LineArr[NLine++] = i;               /* POINTER TO FIRST LINE */

                /* COPY FILE CONTENT TO ARRAY */
			    while (1) {
                    c = fgetc(fp);
				    if ( feof (fp) ){
                        FileC[i] = '\0';
					    break;
				    }

                    /* COUNT THE LINE IN FILE */
                    if (c == '\n'){
                        space = 0; InString = 0;
                        if (NLine > MAX_LINE){
                            fprintf(stderr, "YOUR FILE HAVE MORE THEN %d LINES!\n", MAX_LINE);
                            break;
                        } else {
                            LineArr[NLine++] = i+1;     /* POINTER TO NEW LINE */ 
                        } 
                    }

                    if (c == '\"') InString++;

                    if (InString == FALSE){
                        /* ADD SPACE TO ',' 
                        if (c == ','){
                            if (FileC[i-1] == ' ')
                                FileC[--i] = (char)c;
                        }*/

                        /* ADD SPACE BEFORE '(' */
                        if (c == '(' || c == ','){
                            if (FileC[i-1] != ' ')
                                FileC[i++] = ' ';  
                        }        
                    }

                    /* CHECK DOUBLE SPACE */
                    if ((c == ' ' || c == '\t') && InString == FALSE) {          
                        space++;
                    } else {
                        space = 0;
                    }

                    /* PRINT TO ARRAY */
                    if (space < 1)
                        FileC[i++] = (char)c;
                    else if ((space < 2) && FileC[i-1] != ',' && FileC[i-1] != '\n')
                        FileC[i++] = ' ';
                    else
                        ;
			    } /* END OF WHILE */
            }

            /* FOR TESTING 
            printf("%s\n", FileC);
			return 0;*/

            /* POINTER TO LAST LINE */
            LineArr[NLine++] = i+1;                        
           
            if (NLine < MAX_LINE){
                S.SLen = i;
                S.SLines = NLine;
                S.SLineArr = &LineArr[0];
		S.Code = &FileC[0];
                r1(S); 
            }  
        }
        fclose(fp);
		free(FileC);
    } else {
        fprintf(stderr, "ERROR: THE %s IS NOT ASMBLER FILE!\n", str);
    }

    return 0;
}
