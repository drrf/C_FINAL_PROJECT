/*
 *          File: encode.c
 *        Author: Ron F. <>
 * Last Modified: August 19, 2018
 *         Topic: ASSEMBLER
 * ----------------------------------------------------------------
 */

#include "myas.h"


int checkENCODEdata (theDATA d)
{
	int ind = ZERO, err = ZERO, temp;
	theDATA * d_ptr = &d;

	/* FOR TESTING THE ENCODE AND THE LINE
	printf("\n%d) %s\n\n", d.LineNum, d.LINE);
	printf("EN = ");
	for (ind = ZERO; ind <= d.WORDS; ind++){		
		temp = (int)d.ENCODING[ind];
		printf("%d, ", temp);
	}
	puts("");
	*/

	temp = (int)d.ENCODING[ZERO];
	for (ind = ONE; ind <= d.WORDS; ind++){
		if (temp == (int)d.ENCODING[ind] || temp == ERR_ENCODE){
			err = d.tab -> E_ERR += err_in_line(ZERO,d_ptr,d.LINE);
			break;
		} else
			temp = (int)d.ENCODING[ind];
		
		/* CHECK THE BRECK GET THREE PARAMETER AND LABEL BEFORE */
		if (temp == BRECKT_N){
			if ((int)d.ENCODING[ind-ONE] == LABEL_NL)
				checkCountNum(d,ind,THREE);
			else {
				err = d.tab -> E_ERR += err_in_line(ZERO,d_ptr,d.LINE);
				break;
			}
		}
	}

	return err;
}

/* COUNT THE LOOP FROM INDEX AND RETURN THE COUNT */
int checkCountNum (theDATA d, int ind, int numCheck)
{
	int count = ZERO;
	theDATA * d_ptr = &d;

	for (ind = ind; ind < d.WORDS; ind++){
		count++;
	}
	
	/* THE COUNT FOUND NOT THE SOME NUMBER */
	if (count != numCheck)
		d.tab -> E_ERR += err_in_line(ZERO,d_ptr,d.LINE);

	return count;
}

/* CHECK IF HAVE X, IF TRUE RETURN ONE ELSE RETURN ZERO */
int checkIfHaveNum (theDATA d, int x)
{	
	int i, j;

	for (i = ZERO; i <= d.WORDS; i++){
		j = (int)d.ENCODING[i];
		if (j == x)
			return ONE;
	}

	return 0;
}

/* RETURN THE INDEX OF THE NEXT LABEL, DIGIT OR REGISTER */
int GIVE_ME_NUM (theDATA d, int x)
{
	int i, j;
	int R = REGISTER_N;

	x++;

	for (i = x; i <= d.WORDS; i++){
		j = (int)d.ENCODING[i];
		if (j == LABEL_NL || j == DIGIT_A || (j >= R+reg_r0 && j <= R+reg_r7) ){
			x = i;
			break;
		}
	}

	return x;
}

