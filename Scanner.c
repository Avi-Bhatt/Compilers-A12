/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: NIVLang Team
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: startScanner, tokenizer, nextState, nextClass,
*                funcID, funcIL, funcFPL, funcSL, funcKW,
*                funcCMT, funcErr, printToken, printScannerData
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <assert.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global objects - variables */
extern BufferPointer stringLiteralTable;	/* String literal table */
niv_int line;								/* Current line number of the source code */
extern niv_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern niv_int stateType[NUM_STATES];
extern niv_string keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern niv_int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */

niv_int startScanner(BufferPointer psc_buf) {
	/* Start histogram */
	for (niv_int i = 0; i < NUM_TOKENS; i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS;
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(niv_void) {
	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	niv_char c;			/* input symbol */
	niv_int state = 0;	/* initial state of the FSM */
	niv_int lexStart;	/* start offset of a lexeme in the input char buffer (array) */
	niv_int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	niv_int lexLength;	/* token length */
	niv_int i;			/* counter */
	niv_char nextChar;	/* for look-ahead */

	/* Starting lexeme */
	niv_string lexeme;	/* lexeme (to check the function) */
	lexeme = (niv_string)malloc(VID_LEN * sizeof(niv_char));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) { /* endless loop broken by token returns */
		c = readerGetChar(sourceBuffer);

		// Skip whitespace and line terminators
		if (c == SPC_CHR || c == TAB_CHR) {
			continue;
		}

		if (c == NWL_CHR) {
			line++;
			continue;
		}

		// Defensive programming
		if (c == EOS_CHR || c == EOF_CHR) {
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = (c == EOS_CHR) ? SEOF_0 : SEOF_255;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		}

		/* Part 1: Implementation of token driven scanner */
		switch (c) {
			/* Cases for symbols */
		case SCL_CHR:
			currentToken.code = EOS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LPR_CHR:
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RPR_CHR:
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LBR_CHR:
			currentToken.code = LBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RBR_CHR:
			currentToken.code = RBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case COM_CHR:
			currentToken.code = COM_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

			/* Arithmetic operators */
		case ADD_CHR:
			currentToken.code = ADD_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		case SUB_CHR:
			currentToken.code = SUB_T;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		case MUL_CHR:
			currentToken.code = MUL_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

		case DIV_CHR:
			/* Check for comment (/*) */
			nextChar = readerGetChar(sourceBuffer);
			if (nextChar == '*') {
				/* Process comment */
				niv_char last = '\0';
				while (1) {
					c = readerGetChar(sourceBuffer);
					if (c == EOF_CHR || c == EOS_CHR) {
						/* Unterminated comment */
						currentToken.code = ERR_T;
						strcpy(currentToken.attribute.errLexeme, "Unterminated comment");
						scData.scanHistogram[currentToken.code]++;
						return currentToken;
					}

					if (c == NWL_CHR)
						line++;

					if (last == '*' && c == '/') {
						break;  /* End of comment found */
					}

					last = c;
				}

				/* Return comment token */
				currentToken.code = CMT_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
			else {
				/* Regular division operator */
				readerRetract(sourceBuffer);
				currentToken.code = DIV_T;
				currentToken.attribute.arithmeticOperator = OP_DIV;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}

			/* Assignment and equality operators */
		case ASS_CHR:
			nextChar = readerGetChar(sourceBuffer);
			if (nextChar == '=') {
				currentToken.code = EQU_T;
				currentToken.attribute.relationalOperator = OP_EQ;
				scData.scanHistogram[currentToken.code]++;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = ASS_T;
				scData.scanHistogram[currentToken.code]++;
			}
			return currentToken;

			/* Relational operators */
		case LES_CHR:
			nextChar = readerGetChar(sourceBuffer);
			if (nextChar == '=') {
				currentToken.code = LEQ_T;
				currentToken.attribute.relationalOperator = OP_LE;
				scData.scanHistogram[currentToken.code]++;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = LES_T;
				currentToken.attribute.relationalOperator = OP_LT;
				scData.scanHistogram[currentToken.code]++;
			}
			return currentToken;

		case GRT_CHR:
			nextChar = readerGetChar(sourceBuffer);
			if (nextChar == '=') {
				currentToken.code = GEQ_T;
				currentToken.attribute.relationalOperator = OP_GE;
				scData.scanHistogram[currentToken.code]++;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = GRT_T;
				currentToken.attribute.relationalOperator = OP_GT;
				scData.scanHistogram[currentToken.code]++;
			}
			return currentToken;

		case '!':
			nextChar = readerGetChar(sourceBuffer);
			if (nextChar == '=') {
				currentToken.code = NEQ_T;
				currentToken.attribute.relationalOperator = OP_NE;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
			/* If not !=, process as a normal character */
			readerRetract(sourceBuffer);
			break;

			/* Check for comment (#) in NIVLang */
		case '#':
			/* Process comment until end of line */
			while (1) {
				c = readerGetChar(sourceBuffer);
				if (c == NWL_CHR || c == EOF_CHR || c == EOS_CHR) {
					if (c == NWL_CHR)
						line++;
					break;
				}
			}

			/* Return comment token */
			currentToken.code = CMT_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

			/* String literal processing */
		case QOT_CHR:
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);

			/* Read until closing quote or EOL/EOF */
			while (1) {
				c = readerGetChar(sourceBuffer);
				if (c == QOT_CHR || c == NWL_CHR || c == EOF_CHR || c == EOS_CHR) {
					break;
				}
			}

			if (c != QOT_CHR) {
				/* Unterminated string */
				readerRetract(sourceBuffer);
				lexEnd = readerGetPosRead(sourceBuffer);
				lexLength = lexEnd - lexStart;
				lexemeBuffer = readerCreate((niv_int)lexLength + 2, 0, MODE_FIXED);
				readerRestore(sourceBuffer);
				for (i = 0; i < lexLength; i++)
					readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
				readerAddChar(lexemeBuffer, READER_TERMINATOR);
				lexeme = readerGetContent(lexemeBuffer, 0);
				currentToken = funcErr(lexeme);
				readerRestore(lexemeBuffer);
				return currentToken;
			}

			/* Process the string literal */
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((niv_int)lexLength + 2, 0, MODE_FIXED);
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			currentToken = funcSL(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		}

		/* Part 2: Process letters and digits (identifiers, keywords, numbers) */

		lexStart = readerGetPosRead(sourceBuffer) - 1;
		readerSetMark(sourceBuffer, lexStart);

		/* Process identifiers and keywords (start with letter or underscore) */
		if (isalpha(c) || c == UND_CHR) {
			/* Consume all characters that can be part of an identifier */
			while (1) {
				nextChar = readerGetChar(sourceBuffer);
				if (!(isalnum(nextChar) || nextChar == UND_CHR)) {
					readerRetract(sourceBuffer);
					break;
				}
			}

			/* Extract the lexeme */
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((niv_int)lexLength + 2, 0, MODE_FIXED);
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);

			/* Check if it's a keyword */
			for (i = 0; i < KWT_SIZE; i++) {
				if (!strcmp(lexeme, keywordTable[i])) {
					currentToken.code = KWRD_T;
					currentToken.attribute.keywordIndex = i;
					scData.scanHistogram[currentToken.code]++;
					readerRestore(lexemeBuffer); /* Clear lexeme buffer */
					return currentToken;
				}
			}

			/* If we're here, it's a regular identifier */
			currentToken.code = VID_T;
			strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
			currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
			scData.scanHistogram[currentToken.code]++;
			readerRestore(lexemeBuffer); /* Clear lexeme buffer */
			return currentToken;
		}

		/* Process numbers (integer and float literals) */
		if (isdigit(c)) {
			niv_boolean isFloat = niv_FALSE;

			/* Consume all digits */
			while (1) {
				nextChar = readerGetChar(sourceBuffer);
				if (!isdigit(nextChar)) {
					if (nextChar == DOT_CHR) {
						isFloat = niv_TRUE;
						/* Consume the dot and continue */
						nextChar = readerGetChar(sourceBuffer);
						if (!isdigit(nextChar)) {
							/* Invalid float - must have digits after decimal point */
							readerRetract(sourceBuffer);
							break;
						}

						/* Continue consuming digits after the decimal point */
						while (1) {
							nextChar = readerGetChar(sourceBuffer);
							if (!isdigit(nextChar)) {
								readerRetract(sourceBuffer);
								break;
							}
						}
						break;
					}
					else {
						readerRetract(sourceBuffer);
						break;
					}
				}
			}

			/* Extract the lexeme */
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((niv_int)lexLength + 2, 0, MODE_FIXED);
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);

			/* Process numeric literal - important fix here */
			if (strchr(lexeme, '.') != NULL) {
				/* This is a floating-point literal */
				currentToken = funcFPL(lexeme);
			}
			else {
				/* This is an integer literal */
				currentToken = funcIL(lexeme);
			}

			readerRestore(lexemeBuffer); /* Clear lexeme buffer */
			return currentToken;
		}

		/* If we get here, it's an invalid token */
		currentToken.code = ERR_T;
		currentToken.attribute.errLexeme[0] = c;
		currentToken.attribute.errLexeme[1] = EOS_CHR;
		scData.scanHistogram[currentToken.code]++;
		return currentToken;
	} //while
} // tokenizer

/*
 ************************************************************
 * Get Next State
 ***********************************************************
 */

niv_int nextState(niv_int state, niv_char c) {
	niv_int col;
	niv_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
 ***********************************************************
 */

niv_int nextClass(niv_char c) {
	/*    [A-z], [0-9],    _,    ",    /,  EOF,    #,    .,   OP, OTHER
			L(0),  D(1), U(2), Q(3), S(4), E(5), C(6), P(7), O(8),  X(9) */

	if (c == EOS_CHR || c == EOF_CHR)
		return SEOF;

	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return LETTER;

	if (c >= '0' && c <= '9')
		return DIGIT;

	if (c == UND_CHR)
		return UNDERSCORE;

	if (c == QOT_CHR)
		return QUOTE;

	if (c == DIV_CHR)
		return SLASH;

	if (c == '#') /* Comment indicator for NIVLang */
		return COMMENT;

	if (c == DOT_CHR)
		return DOT;

	if (c == ADD_CHR || c == SUB_CHR || c == MUL_CHR ||
		c == ASS_CHR || c == LES_CHR || c == GRT_CHR)
		return OPERATOR;

	return OTHER;
}

/*
 ************************************************************
 * Acceptance State Function ID
 ***********************************************************
 */

Token funcID(niv_string lexeme) {
	Token currentToken = { 0 };

	/* First, check if it's a keyword */
	for (niv_int j = 0; j < KWT_SIZE; j++) {
		if (strcmp(lexeme, keywordTable[j]) == 0) {
			currentToken.code = KWRD_T;
			currentToken.attribute.keywordIndex = j;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		}
	}

	/* It's a variable identifier */
	currentToken.code = VID_T;
	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	scData.scanHistogram[currentToken.code]++;

	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function IL
 ***********************************************************
 */

Token funcIL(niv_string lexeme) {
	Token currentToken = { 0 };
	niv_long tlong;

	/* Check if the number has a decimal point - handle as float if so */
	if (strchr(lexeme, '.') != NULL) {
		/* This is a floating-point number */
		return funcFPL(lexeme);
	}

	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		currentToken = funcErr(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (niv_int)tlong;
		}
		else {
			currentToken = funcErr(lexeme);
		}
	}
	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function FPL
 ***********************************************************
 */
Token funcFPL(niv_string lexeme) {
	Token currentToken = { 0 };
	niv_float tfloat;

	/* Convert string to float */
	tfloat = (niv_float)atof(lexeme);

	/* Check if the float is within valid range */
	if (tfloat >= FLT_MIN && tfloat <= FLT_MAX || tfloat == 0.0) {
		currentToken.code = FPL_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.floatValue = tfloat;
	}
	else {
		currentToken = funcErr(lexeme);
	}

	return currentToken;
}

/*
************************************************************
 * Acceptance State Function SL
 ***********************************************************
 */

Token funcSL(niv_string lexeme) {
	Token currentToken = { 0 };
	niv_int i = 0, len = (niv_int)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = ERR_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		currentToken.code = ERR_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function KW
 ***********************************************************
 */

Token funcKW(niv_string lexeme) {
	Token currentToken = { 0 };
	niv_int kwindex = -1;

	for (niv_int j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, keywordTable[j]))
			kwindex = j;

	if (kwindex != -1) {
		currentToken.code = KWRD_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.keywordIndex = kwindex;
	}
	else {
		/* If not a keyword, it's a regular identifier */
		currentToken = funcID(lexeme);
	}

	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function COM
 ***********************************************************
 */

Token funcCMT(niv_string lexeme) {
	Token currentToken = { 0 };
	niv_int i = 0, len = (niv_int)strlen(lexeme);

	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
	}

	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function Error
 ***********************************************************
 */

Token funcErr(niv_string lexeme) {
	Token currentToken = { 0 };
	niv_int i = 0, len = (niv_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

niv_void printToken(Token t) {
	extern niv_string keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case KWRD_T:
		printf("KWRD_T\t\t%s\n", keywordTable[t.attribute.keywordIndex]);
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (niv_int)t.attribute.contentString);
		printf("%s\n", readerGetContent(stringLiteralTable, (niv_int)t.attribute.contentString));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case ASS_T:
		printf("ASS_T\n");
		break;
	case ADD_T:
		printf("ADD_T\n");
		break;
	case SUB_T:
		printf("SUB_T\n");
		break;
	case MUL_T:
		printf("MUL_T\n");
		break;
	case DIV_T:
		printf("DIV_T\n");
		break;
	case LES_T:
		printf("LES_T\n");
		break;
	case GRT_T:
		printf("GRT_T\n");
		break;
	case EQU_T:
		printf("EQU_T\n");
		break;
	case NEQ_T:
		printf("NEQ_T\n");
		break;
	case LEQ_T:
		printf("LEQ_T\n");
		break;
	case GEQ_T:
		printf("GEQ_T\n");
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
niv_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}