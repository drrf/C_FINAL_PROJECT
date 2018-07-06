/* myas.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* FLAGS */
#define FALSE 0
#define TRUE 1
#define FIRST 1
#define ONE 1
#define TWO 2

#define MAX_FILES 10        /* MAX FILE THAT PROGRAM CAN INPUT IN 1 RUN */
#define MAX_NAME 10         /* MAX FILE NAME LENGTH */
#define MAX_WORD 99         /* MAX WORD LENGTH & IN ONE LINE */
#define MAX_LINE 999       /* MAX LINE LENGTH & LINES IN FILES */
#define MAX_CODE 9999	    /* MAX CODE IN ONE .AS FILE */
#define AS 258              /* THE VAULE OF ".as" -> need double check! */

/* THE OPT FROM: 0 [00000000000000] TO 960 [00001111000000] */
#define mov_V  0             /* 0 THE VAULE OF "mov" */
#define cmp_V  64            /* 1 THE VAULE OF "mov" */
#define add_V  128           /* 2 THE VAULE OF "mov" */
#define sub_V  192           /* 3 THE VAULE OF "mov" */
#define not_V  256           /* 4 THE VAULE OF "mov" */
#define clr_V  320           /* 5 THE VAULE OF "mov" */
#define lea_V  384           /* 6 THE VAULE OF "mov" */
#define inc_V  448           /* 7 THE VAULE OF "mov" */
#define dec_V  512           /* 8 THE VAULE OF "mov" */
#define jmp_V  576           /* 9 THE VAULE OF "mov" */
#define bne_V  640           /* 10 THE VAULE OF "mov" */
#define red_V  704           /* 11 THE VAULE OF "mov" */
#define prn_V  768           /* 12 THE VAULE OF "mov" */
#define jsr_V  832           /* 13 THE VAULE OF "mov" */
#define rts_V  896           /* 14 THE VAULE OF "mov" */
#define stop_V 960           /* 15 THE VAULE OF "mov" */

/* MAJIC NUMBER TO ENCODING THE LINE  */
#define LABEL_N -100
#define REGISTER_N 114
#define DIGIT_N 100
#define STRING_N 200
#define COMMA_N -101
#define ERR_ENCODE -9999

/* THE LINE TYPEDEF */
typedef struct Lines {
    int SLen;                 /* ARRAY LENGTH */
    int SLines;              /* NUMBER OF LINE */
    int * SLineArr;         /* ARRAY OF POINTER TO LINES */
    char * Code;	       /* POINTER TO CODE TEXT */
} SLine;

/* THE LINE TYPEDEF */
typedef struct CODE {
	char * LINE;       			/* POINTER TO ONE LINE OF THE CODE */
	int * ENCODE;      			/* POINTER TO ARRAY OF ENCODE CODE */
    int WORDS;                  /* NUMBER OF WORDS IN LINE */
	char NEW_LABEL [MAX_WORD];  /* DATA NEW LABEL */
	char STRING [MAX_WORD];		/* DATA STRING */
} theDATA;

typedef struct T {
    int IC;                
    int DC;                
} TABLE;

typedef struct IC {
    int ERA;                /* -1 == EX || 0 == N || 1 == EN */
    int OPT;                /* CMD FROM: 0 [00000000000000] TO 960 [00001111000000] */
} DATA_IC;

typedef struct DC {
    int VAULE;                /* THE VAULE OF DATA */
} DATA_DC;


/* arr.c */
int cpToArr (int,char[]);

/* r1.c */
int r1 (SLine);
int breakToWords (char[], int);
int return_sum (theDATA *,char[],int[]);
int wordsToData (int,char[],int[]);
int wordsToOpt (int,char[],int[]);

/* r1.c */
int dataRead (theDATA);

/* err.c */
int pERRchar (char,int);
int pERR1st (int,char,int);
int pERRduble (char,int);
int pERRmiss (char,int);
int pERRword (char[],int);


