/*
 *          File: myas.h
 *        Author: Ron F. <>
 * Last Modified: April 8, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */
/* myas.h */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* FLAGS */
#define EMPTY_AND_BLOCK -9999
#define EMPTY 0
#define FALSE 0
#define TRUE 1
#define FIRST 1
#define ONE 1
#define R_ONE 1
#define R_TWO 2
#define TWO 2
#define THREE 3


/* DEFINE MAX LENGTH */
#define MAX_FILES 10        /* MAX FILE THAT PROGRAM CAN INPUT IN 1 RUN */
#define MAX_NAME 10         /* MAX FILE NAME LENGTH */
#define MAX_ENCODING 40     /* MAX NUMBER DATA IN ONE LINE */
#define MAX_LABEL 32        /* MAX LABEL NAME LENGTH */
#define MAX_L_IN_LINE 3 	/* MAX LABEL NAME IN LINE */
#define MAX_IC_IN_LINE 3 	/* MAX LABEL NAME IN LINE */
#define MAX_REG_IN_LINE 2 	/* MAX REGISTER IN LINE */
#define MAX_WORD 99         /* MAX WORD LENGTH & IN ONE LINE */
#define MAX_LINE 82         /* MAX LINE LENGTH & LINES IN FILES */
#define MAX_CODE 9999	    /* MAX CODE IN ONE .AS FILE */
#define AS 258              /* THE VAULE OF ".as" -> need double check! */



/* MAJIC NUMBER TO ENCODING THE LINE  */
#define LABEL_N -100
#define COMMA_N -101
#define ERR_ENCODE -127

/* MAJIC NUMBER TO ENCODING DATA LINE */
#define EN_N -99
#define EX_N -98
#define DIGIT_N -10
#define STRING_N -15
#define DIGIT_A 100
#define STRING_A 101

/* MAJIC NUMBER TO ENCODING OPT LINE */
enum opt_c { V_MOV, V_CMP, V_ADD, V_SUB, V_NOT, V_CLR, V_LEA, V_INC, V_DEC, V_JMP, V_BNE, V_RED, V_PRN, V_JSR, V_RTS, V_STOP };
/* THE OPT FROM: 0 [00000000000000] TO 960 [00001111000000] */

#define REGISTER_N 114
enum reg_c { reg_r0, reg_r1, reg_r2, reg_r3, reg_r4, reg_r5, reg_r6, reg_r7 };

/* BITFIELD */
enum bit_c { bit_ERA = 1, bit_DES = 4, bit_SUR = 16, bit_OPT = 64, bit_P1 = 1024, bit_P2 = 4096};
enum BITS_F { BIT_3 = 4, BIT_4 = 8, BIT_9 = 256 };

/* ADDRESS METHOD */
enum address { adr_0, adr_1, adr_2, adr_3 };

/* THE LINE TYPEDEF */
typedef struct Lines {
    int SLen;                 /* ARRAY LENGTH */
    int SLines;              /* NUMBER OF LINE */
    int * SLineArr;         /* ARRAY OF POINTER TO LINES */
    char * Code;	       /* POINTER TO CODE TEXT */
	char * FileN;	       /* POINTER TO FILE NAME */
} SLine;

/* TREE STRUCTURE FOR LABEL */
typedef struct tnode {      /* the tree node */
    char *word;          /* points to the text */
	int adr;		 	 /* adr of label */
	int ic_dc;           /* ic or dc */
	int ex;				 /* ex yes = 1, or no */
	int count;           /* number of occurrences */
    struct tnode *left;  /* left child */
    struct tnode *right; /* right child */
} tnode;

/* LINKED LIST FOR DATA */
typedef struct lnode { 
    int data;
    struct lnode *next;
} lnode;

typedef struct T {
	lnode * DC_HEAD;
	lnode * DC_PTR;
	tnode * T_ROOT;
	tnode * T_PTR;

    int IC;
	int DC;	
	int MC; 			/* MC = IC + DC */
	int FINAL_IC;		 /* final ic */
} TABLE;

/* THE LINE TYPEDEF */
typedef struct ONE_CODE_LINE {
	int Line;						/* THE LINE NUMBER IN "FILE.AS" CODE */
	int WORDS;                  	/* NUMBER OF WORDS IN THIS LINE */

	int NUM;						/* COUNT HOW MUCH DATA DIGIT FOUND */
	int LABEL;						/* COUNT HOW MUCH LABEL FOUND */
	int REG;						/* COUNT HOW MUCH REGISTER FOUND */

	int IC_DC;						/* 0 == NULL || 1 == IC || -1 == DC */
	int OPT_DATA;					/* FOR OPT OR DATA LINE WE FOUND */

	char ENCODING [MAX_ENCODING];   /*  ARRAY OF ENCODING CODE */
	char NEW_LABEL [MAX_LABEL]; 	/* SAVE DATA NEW LABEL */
	char LABEL_ARR [MAX_L_IN_LINE][MAX_LABEL];	/* SAVE LABEL DATA HERE */
	char REG_ARR [MAX_REG_IN_LINE];	/* SAVE REGISTER DATA */

	/* THIS DATA CAN BE POINTER I'M USE THEM ONE TIME */
	int * NUM_ARR;					/* POINTER TO DIGIT DATA HERE */
	char * STRING;					/* POINTER TO STRING DATA */
	char * LINE;       				/* POINTER TO ONE LINE OF THE CODE */
	char * ONE_W;					/* ONE WORD FROM LINE */
	char * FNAME;       			/* POINTER TO THE FILE NAME */
	int * MC_ARR;					/* POINTER TO DIGIT DATA HERE */
	
	TABLE * tab;					/* POINTER TO TABLE */
} theDATA;


typedef struct IC {
    int ERA;                /* -1 == EX || 0 == N || 1 == EN */
    int OPT;                /* CMD FROM: 0 [00000000000000] TO 960 [00001111000000] */
} DATA_IC;


/* arr.c */
int cpToArr (int,char[]);

/* ins_and_free */
theDATA * installD (SLine);
TABLE installT ();
void freeList(lnode * head);

/* r1.c */
int r1 (SLine);
int WriteLine (theDATA,int);

/* r2.c */
int r2(theDATA,TABLE);
int my_data (theDATA[],int);

/* parsing.c */
theDATA ReadLine (char[],int,theDATA);
int return_sum (theDATA *,char[],int[]);
int return_num (char[]);
int wordsToData (int,char[],int[]);
int wordsToOpt (int,char[],int[]);

/* label.c */
tnode *addtree (tnode *, char *, theDATA);
tnode *talloc(void);
void treeprint (tnode *, int);
char *strdup (char *);
int findLabel (tnode *, char *, int);
int tEnterData (tnode *, theDATA );

/* data.c */
int dataLine (theDATA,int);
int dataDigit (theDATA,int);
int dataString (theDATA,int);
lnode *lalloc(void);

/* opt.c */
int checkOPT(theDATA);
int optLine (theDATA,int);
int noDES_noSUR (theDATA,int);
int DES_only (theDATA,int,int);
int DES_and_SUR (theDATA,int,int,int);
int optJUMP (theDATA,int);
int GIVE_ME_NUM (theDATA,int);
int returnAddress (int);

/* err.c */
int pERRchar (char,int);
int pERR1st (int,char,int);
int pERRduble (char,int);
int pERRmiss (char,int);
int pERRword (char[],int);
