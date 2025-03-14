/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2025
* Author: Avinish Bhattarai, Nikita Chaudhari
* Professors: Sarah Khan
************************************************************
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
************************************************************
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: 303
* Assignment: A12, A22, A32.
* Date: feb 01 2025
* Professor: Sarah Khan
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/


#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

niv_int main(int argc, char** argv) {
	niv_int i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_READER, "] - Reader\n* [",
			PGM_SCANNER, "] - Scanner\n* [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	niv_char option = argv[1][0];
	switch (option) {
	case PGM_READER:
		printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
		mainReader(argc, argv);
		break;
	case PGM_SCANNER:
		printf("%s%c%s", "\n[Option '", PGM_SCANNER, "': Starting SCANNER .....]\n\n");
		mainScanner(argc, argv);
		break;
		/*
	case PGM_PARSER:
		printf("%s%c%s", "\n[Option '", PGM_PARSER, "': Starting PARSER .....]\n\n");
		mainParser(argc, argv);
		break;
	*/
	default:
		printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
			PGM_READER, "] - Reader\n- [",
			PGM_SCANNER, "] - Scanner\n- [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Avinish Bhattarai,Nikita Chaudhari
* History/Versions: Ver S23
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

/* TO_DO: Update your logo with your own language message */

niv_void printLogo() {
	printf("\n===================================================");
	printf("\n|				  NIVLang Logo     				    |");
	printf("\n===================================================");
	printf("\nNNNNNNNN        NNNNNNNNVVVVVVVV           VVVVVVVV");
	printf("\nN:::::::N       N::::::NV::::::V           V::::::V");
	printf("\nN::::::::N      N::::::NV::::::V           V::::::V");
	printf("\nN:::::::::N     N::::::NV::::::V           V::::::V");
	printf("\nN::::::::::N    N::::::N V:::::V           V:::::V ");
	printf("\nN:::::::::::N   N::::::N  V:::::V         V:::::V	 ");
	printf("\nN:::::::N::::N  N::::::N   V:::::V       V:::::V	 ");
	printf("\nN::::::N N::::N N::::::N    V:::::V     V:::::V	 ");
	printf("\nN::::::N  N::::N:::::::N     V:::::V   V:::::V  	 ");
	printf("\nN::::::N   N:::::::::::N      V:::::V V:::::V		 ");
	printf("\nN::::::N    N::::::::::N       V:::::V:::::V		 ");
	printf("\nN::::::N     N:::::::::N        V:::::::::V		 ");
	printf("\nN::::::N      N::::::::N         V:::::::V	 	 ");
	printf("\nN::::::N       N:::::::N          V:::::V			 ");
	printf("\nN::::::N        N::::::N           V:::V			 ");
	printf("\nNNNNNNNN         NNNNNNN            VVV			 ");
	printf("\n===================================================");
	printf("\n|	Lightweight & Fast			  |					 ");
	printf("\n===================================================");
}



