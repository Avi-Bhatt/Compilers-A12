/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2025
* Author: Avinish Bhattarai,Nikita Chaudhari
* Professors: Sarah Khan
************************************************************
#
#
#echo -e "\t==================================================="
#echo -e "\t|                 NIVLang Logo                    |"
#echo -e "\t==================================================="
#echo -e "\tNNNNNNNN        NNNNNNNNVVVVVVVV           VVVVVVVV"
#echo -e "\tN:::::::N       N::::::NV::::::V           V::::::V"
#echo -e "\tN::::::::N      N::::::NV::::::V           V::::::V"
#echo -e "\tN:::::::::N     N::::::NV::::::V           V::::::V"
#echo -e "\tN::::::::::N    N::::::N V:::::V           V:::::V "
#echo -e "\tN:::::::::::N   N::::::N  V:::::V         V:::::V  "
#echo -e "\tN:::::::N::::N  N::::::N   V:::::V       V:::::V   "
#echo -e "\tN::::::N N::::N N::::::N    V:::::V     V:::::V	   "
#echo -e "\tN::::::N  N::::N:::::::N     V:::::V   V:::::V	   "
#echo -e "\tN::::::N   N:::::::::::N      V:::::V V:::::V	   "
#echo -e "\tN::::::N    N::::::::::N       V:::::V:::::V	   "
#echo -e "\tN::::::N     N:::::::::N        V:::::::::V		   "
#echo -e "\tN::::::N      N::::::::N         V:::::::V		   "
#echo -e "\tN::::::N       N:::::::N          V:::::V		   "
#echo -e "\tN::::::N        N::::::N           V:::V		   "
#echo -e "\tNNNNNNNN         NNNNNNN            VVV			   "
#echo -e "\t==================================================="
#echo -e "\t|             Lightweight & Fast                  |"
#echo -e "\t==================================================="
/*
/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 303
* Assignment: A12.
* Date: feb 01 2025
* Professor: Sarah Khan
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Sarah KHan
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */
 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
 *          factor, and operational mode ('f', 'a', 'm')
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value:
 *   Pointer to Buffer structure
 ************************************************************
 */
BufferPointer readerCreate(niv_int size, niv_int increment, niv_char mode) {
	if (size <= 0) size = READER_DEFAULT_SIZE;
	if (increment < 0) increment = READER_DEFAULT_INCREMENT;
	if (mode != MODE_FIXED && mode != MODE_ADDIT && mode != MODE_MULTI) return niv_INVALID;

	BufferPointer readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer) return niv_INVALID;

	readerPointer->content = (niv_string)malloc(size);
	if (!readerPointer->content) {
		free(readerPointer);
		return niv_INVALID;
	}

	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	readerPointer->flags.isEmpty = 1;
	readerPointer->flags.isFull = 0;
	readerPointer->flags.isRead = 0;
	readerPointer->flags.isMoved = 0;
	readerPointer->positions.read = 0;
	readerPointer->positions.wrte = 0;
	readerPointer->positions.mark = 0;

	for (niv_int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}

	readerPointer->numReaderErrors = 0;
	return readerPointer;
}




/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*   readerPointer (pointer to Buffer Reader)
************************************************************
*/
BufferPointer readerAddChar(BufferPointer readerPointer, niv_char ch) {
	if (!readerPointer || !readerPointer->content) return niv_INVALID;

	/* Check if buffer is full */
	if (readerPointer->positions.wrte >= readerPointer->size) {
		niv_int newSize = 0;
		niv_string tempReader = NULL;

		/* Handle different buffer modes */
		switch (readerPointer->mode) {
		case MODE_FIXED:
			return niv_INVALID; /* No resizing in fixed mode */

		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;
			break;

		case MODE_MULTI:
			newSize = readerPointer->size * 2; /* Double size */
			break;

		default:
			return niv_INVALID;
		}

		/* Ensure new size does not exceed max size */
		if (newSize > READER_MAX_SIZE) return niv_INVALID;

		/* Reallocate memory */
		tempReader = (niv_string)realloc(readerPointer->content, newSize);
		if (!tempReader) return niv_INVALID; /* Memory allocation failed */

		/* Update buffer properties */
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags.isMoved = 1;
	}

	/* Add character to buffer */
	readerPointer->content[readerPointer->positions.wrte++] = ch;

	/* Update histogram */
	readerPointer->histogram[(niv_int)ch]++;

	/* Update flags */
	readerPointer->flags.isEmpty = 0;
	if (readerPointer->positions.wrte >= readerPointer->size) {
		readerPointer->flags.isFull = 1;
	}

	return readerPointer;
}



/***********************************************************
*Function name : readerClear
* Purpose : Clears the buffer reader
* Parameters :
	*readerPointer = pointer to Buffer Reader
	* Return value :
*Boolean value about operation success
* ***********************************************************
*/
niv_boolean readerClear(BufferPointer const readerPointer) {
	if (!readerPointer) return niv_FALSE;
	readerPointer->positions.wrte = 0;
	readerPointer->positions.read = 0;
	readerPointer->positions.mark = 0;
	readerPointer->flags.isEmpty = 1;
	readerPointer->flags.isFull = 0;
	return niv_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Boolean value about operation success
************************************************************
*/
niv_boolean readerFree(BufferPointer const readerPointer) {
	if (!readerPointer) return niv_FALSE;
	free(readerPointer->content);
	free(readerPointer);
	return niv_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Boolean value about operation success
************************************************************
*/
niv_boolean readerIsFull(BufferPointer const readerPointer) {
	if (!readerPointer) return niv_FALSE;
	return readerPointer->flags.isFull;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Boolean value about operation success
************************************************************
*/
niv_boolean readerIsEmpty(BufferPointer const readerPointer) {
	if (!readerPointer) return niv_FALSE;
	return readerPointer->flags.isEmpty;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*   Boolean value about operation success
************************************************************
*/
niv_boolean readerSetMark(BufferPointer const readerPointer, niv_int mark) {
	if (!readerPointer || mark < 0 || mark >= readerPointer->positions.wrte) return niv_FALSE;
	readerPointer->positions.mark = mark;
	return niv_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.

*************************************************************
*/
niv_int readerPrint(BufferPointer const readerPointer) {
	if (!readerPointer || !readerPointer->content) return -1;
	niv_int count = 0;
	niv_char c = readerGetChar(readerPointer);
	while (count < readerPointer->positions.wrte) {
		if (c < 32 || c > 126) c = '?';  // Replace invalid characters with '?'
		printf("%c", c);
		count++;
		c = readerGetChar(readerPointer);
	}
	return count;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
*   a specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*   Number of chars read and put in buffer.
************************************************************
*/
niv_int readerLoad(BufferPointer readerPointer, FILE* const fileDescriptor) {
	if (!readerPointer || !fileDescriptor) return -1;
	niv_int size = 0;
	niv_char c;
	while ((c = (niv_char)fgetc(fileDescriptor)) != EOF) {
		if (!readerAddChar(readerPointer, c)) break;
		size++;
	}
	return size;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Boolean value about operation success.
************************************************************
*/
niv_boolean readerRecover(BufferPointer const readerPointer) {
	if (!readerPointer) return niv_FALSE;
	readerPointer->positions.read = 0;
	readerPointer->positions.mark = 0;
	return niv_TRUE;
}

 

/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Boolean value about operation success.
************************************************************
*/
niv_boolean readerRestore(BufferPointer const readerPointer) {
	if (!readerPointer) return niv_FALSE;
	readerPointer->positions.read = readerPointer->positions.mark;
	return niv_TRUE;
}

/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Char in the getC position.
************************************************************
*/
niv_char readerGetChar(BufferPointer const readerPointer) {
	if (!readerPointer || readerPointer->positions.read >= readerPointer->positions.wrte) return READER_TERMINATOR;
	return readerPointer->content[readerPointer->positions.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*   Position of string char.
************************************************************
*/
niv_string readerGetContent(BufferPointer const readerPointer, niv_int pos) {
	if (!readerPointer || pos < 0 || pos >= readerPointer->positions.wrte) return NULL;
	return readerPointer->content + pos;
}


/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   The read position offset.
************************************************************
*/
niv_int readerGetPosRead(BufferPointer const readerPointer) {
	if (!readerPointer) return -1;
	return readerPointer->positions.read;
}
/*
***********************************************************
* Function name: readerRetract
* Purpose: Retract the buffer read position by one character
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Boolean value about operation success
************************************************************
*/
niv_boolean readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer) return niv_FALSE;
	if (readerPointer->positions.read > 0) {
		readerPointer->positions.read--;
		return niv_TRUE;
	}
	return niv_FALSE;
}

/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Write position
************************************************************
*/
niv_int readerGetPosWrte(BufferPointer const readerPointer) {
	if (!readerPointer) return -1;
	return readerPointer->positions.wrte;
}

/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Mark position.
************************************************************
*/
niv_int readerGetPosMark(BufferPointer const readerPointer) {
	if (!readerPointer) return -1;
	return readerPointer->positions.mark;
}

/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Size of buffer.
************************************************************
*/
niv_int readerGetSize(BufferPointer const readerPointer) {
	if (!readerPointer) return -1;
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   The Buffer increment.
************************************************************
*/
niv_int readerGetInc(BufferPointer const readerPointer) {
	if (!readerPointer) return -1;
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Operational mode.
************************************************************
*/
niv_char readerGetMode(BufferPointer const readerPointer) {
	if (!readerPointer) return '\0';
	return readerPointer->mode;
}

/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
************************************************************
*/
niv_void readerPrintStat(BufferPointer const readerPointer) {
	if (!readerPointer) return;
	for (niv_int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0)
			printf("B[%c]=%d, ", i, readerPointer->histogram[i]);
	}
	printf("\n");
}


/*
***********************************************************
* Function name: readerGetNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   Number of errors.
************************************************************
*/
niv_int readerGetNumErrors(BufferPointer const readerPointer) {
	if (!readerPointer) return -1;
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerCalcChecksum
* Purpose: Calculates the checksum of the reader (8 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   [None]
************************************************************
*/
niv_void readerCalcChecksum(BufferPointer readerPointer) {
	if (!readerPointer) return;
	readerPointer->checksum = 0;
	for (niv_int i = 0; i < readerPointer->positions.wrte; i++) {
		readerPointer->checksum ^= readerPointer->content[i];
	}
}

/*
***********************************************************
* Function name: readerPrintFlags
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*   [None]
************************************************************
*/
niv_boolean readerPrintFlags(BufferPointer readerPointer) {
	if (!readerPointer) return niv_FALSE;
		printf("isEmpty: %d\n", readerPointer->flags.isEmpty);
	printf("isFull: %d\n", readerPointer->flags.isFull);
	printf("isRead: %d\n", readerPointer->flags.isRead);
	printf("isMoved: %d\n", readerPointer->flags.isMoved);
	return niv_TRUE;
}
