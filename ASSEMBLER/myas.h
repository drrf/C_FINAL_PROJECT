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
#define ERR_NUMBER -999
#define EMPTY 0
#define FALSE 0
#define TRUE 1
#define FIRST 1
#define R_ONE 1
#define R_TWO 2
#define BITS_SIZE_14 14

/* DEFINE MAX LENGTH */
#define MAX_FILES 10        /* MAX FILE THAT PROGRAM CAN INPUT IN 1 RUN */
#define MAX_NAME 1024       /* MAX FILE NAME PATH */
#define MAX_ENCODING 40     /* MAX ENCODING IN ONE LINE */
#define MAX_LABEL 32        /* MAX LABEL NAME LENGTH */
#define MAX_L_IN_LINE 3 	/* MAX LABEL NAME IN LINE */
#define MAX_IC_IN_LINE 3 	/* MAX LABEL NAME IN LINE */
#define MAX_REG_IN_LINE 2 	/* MAX REGISTER IN LINE */
#define MAX_WORD 99         /* MAX WORD LENGTH & IN ONE LINE */
#define MAX_LINE 512        /* MAX LINE LENGTH & LINES IN FILES */
#define MAX_CODE 99999	    /* MAX CODE IN ONE .AS FILE */

/* DEFINE FOR FILES WORK */
#define FILE_FORMAT_LEN 3
#define FILE_AS ".as"
#define FILE_OB ".ob"
#define FILE_ENT ".ent"
#define FILE_EXT ".ext"

/* MAJIC NUMBER TO ENCODING THE LINE  */
#define DC_ID_LINE  -1
#define IC_ID_LINE  1
#define LABEL_NL -1		/* LABEL IN LINE */
#define LABEL_N -100	/* START LINE LABEL */
#define COMMA_N -111
#define BRECKT_N -125
#define ERR_ENCODE -127

/* MAJIC NUMBER TO ENCODING DATA LINE */
#define EN_N -99
#define EX_N -98
#define DIGIT_N -10
#define STRING_N -15
#define DIGIT_A 100
#define STRING_A 101
#define REGISTER_N 114

/* NUMBER FOR ADR */
#define EX_ADR 1
#define OPT_NODE 1
#define DATA_NODE -1
#define EX_NODE -1
#define EN_NODE -111 /* USE ONE TIME REMOVE BETTER REMOVE THIS ! */
#define START_LINE_100 100

/* SIMPLE THE CODE FROM NUNBERS */
enum NUM_ID { ZERO, ONE, TWO, THREE, FOUR, FIVE };

enum data_c { d_ent, d_ext, d_data, d_str };

/* MAJIC NUMBER TO ENCODING OPT LINE */
enum opt_c { V_MOV, V_CMP, V_ADD, V_SUB, V_NOT, V_CLR, V_LEA, V_INC, V_DEC, V_JMP, V_BNE, V_RED, V_PRN, V_JSR, V_RTS, V_STOP };
/* THE OPT FROM: 0 [00000000000000] TO 960 [00001111000000] */


enum reg_c { reg_r0, reg_r1, reg_r2, reg_r3, reg_r4, reg_r5, reg_r6, reg_r7 };

/* BITFIELD */
enum bit_c { bit_ERA = 1, bit_DES = 4, bit_SUR = 16, bit_OPT = 64, bit_P1 = 4096, bit_P2 = 1024 };
enum BITS_F { BIT_3 = 4, BIT_4 = 8, BIT_9 = 256 };

/* ADDRESS METHOD */
enum address { adr_0_NUM, adr_1_LAB, adr_2_JMP, adr_3_REG };

/* ERR_NUM_ID FOR SWITCH CASE */
enum ERR_NUM {	
				/* ERR FOR arr.c */
				pERR_ID_OPEN_F, pERR_ID_F_EMPTY, pERR_ID_MAX_LINE,

				/* ERR FOR ONE CHAR */
				pERR_ID, pERR1_ID_START_LINE, pERR1_ID, pERRchar_ID, 					pERRduble_ID,

				/* ERR FOR ONE WORD */
				pERR_ID_wrd_data, pERR_ID_wrd_opt, pERR_ID_wrd_double,
				pERR_ID_wrd_Saved, pERR_ID_wrd_CLOSE, pERR_ID_wrd_doubleL,
				pERR_ID_wrd_miss_L, 
				

				/* ERR FOR ONE LINE */
				pERR_ID_line_IC_DC, pERR_ID_line_IC_MAX, pERR_ID_line_EXTRA,
				pERR_ID_line_DATA, pERR_ID_line_OPT };

/* ReadLine ID FOR SWITCH CASE IN parsing.c FILE */
enum ReadLine { b_FALSE, b_ALPHA, b_DIGIT, b_POINT,
				b_NUMBER_WORK, b_LABEL_END, b_BRACKET,
				b_ARR_STR, b_COMMA, b_SPACE,
				b_DATA, b_REG, b_OPT };

/* THE LINE TYPEDEF */
typedef struct AS_FILE {
    int SLen;                 /* ARRAY LENGTH */
    int SLines;              /* NUMBER OF LINE */
    int * SLineArr;         /* ARRAY OF POINTER TO LINES */
    char * Code;	       /* POINTER TO CODE TEXT */
	char * FileN;	       /* POINTER TO FILE NAME */
} AS_DATA;

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

/* STRUCT TO SAVE AND READ, IC & DC & LABEL */
typedef struct T {
	tnode * T_ROOT;		/* ROOT OF THE LABEL TREE */
	tnode * T_PTR;		/* POINTER TO LABEL TREE */

	char * FNAME;       /* POINTER TO THE FILE NAME WITHOUT ".as" */
	int * MC_ARR;		/* POINTER TO ARRAY MC (DC + IC) */

    int TEMP_IC;		/* IC COUNTER TEMP FOR FIRST RUN */
	int FINAL_IC;		/* FINAL IC FOR FINAL RUN */
	int DC;				/* DC COUNTER */
	int FINAL_DC;		/* FINAL DC FOR FINAL RUN */
	int MC; 			/* MC = IC_TEMP + DC */
	int E_ERR;			/* COUNTER OF ERR TO BLOCK OUTPUT FILE */
} TABLE;

/* THE LINE TYPEDEF */
typedef struct ONE_CODE_LINE {
	int LineNum;					/* THE LINE NUMBER IN "FILE.AS" CODE */
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
	int * MC_ARR;					/* POINTER TO MC DATA HERE */
	
	TABLE * tab;					/* POINTER TO TABLE */
} theDATA;

/* arr.c */
int cpToArr (char[]);

/* ins_and_rem */
TABLE installT (char *);
int remove_files (char *);

/* algo.c */
int algo1 (AS_DATA);
theDATA algo2 (AS_DATA, theDATA, int);
int algo3 (theDATA,int);

/* parsing.c */
theDATA ReadLine (char[],int,theDATA);
int FirstCheckChar (char *,theDATA *);
int case_FUNC_ReadLine (char);
int case_FUNC_return_sum (char *);
int return_sum (theDATA *,char[]);
int return_num (char[],theDATA *);
int wordsToData (char[]);
int wordsToReg (char[]);
int wordsToOpt (char[]);

/* encode.c */
int checkENCODEdata (theDATA);
int checkCountNum (theDATA, int, int);
int checkIfHaveNum (theDATA,int);
int GIVE_ME_NUM (theDATA,int);

/* label.c */
tnode *addtree (tnode *, char *, theDATA);
tnode *talloc(void);
void treeprint (tnode *, int);
char *strdup (char *);
int findLabel (tnode *, char *, theDATA, int);
int tEnterData (tnode *, theDATA );

/* en_ex.c */
int write_en_file (tnode *, char *, theDATA);
int write_ex_file (char *, int, theDATA);

/* data.c */
int dataLine (theDATA,int);
int dataDigit (theDATA,int,int);
int dataString (theDATA,int,int);

/* opt.c */
int checkOPT(theDATA);
int optLine (theDATA,int);
int noDES_noSUR (theDATA,int);
int DES_only (theDATA,int,int);
int DES_and_SUR (theDATA,int,int,int);
int optJUMP (theDATA,int);
int ADD_R_LBAEL_ADR (int);
int returnAddress (int);

/* mc.c */
int MC_to_FILE (TABLE);
char * INT_to_MC (char *,int);
char * my_strrev(char *);

/* err.c */
int err_in_arr (int,char*);
void err_in_algo (int,char*,int);
void err_open_print(theDATA * d);
int err_print(int,theDATA*,char);
int err_in_word(int,theDATA*,char*);
int err_in_line(int,theDATA*,char*);
