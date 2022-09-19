/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Header file for fileIO.c
*/

#ifndef FILEIO_H
#define FILEIO_H

/* Function declarations */
int readFile(char* filename, int* total, int* cur,
    int* prev, int** inRequests, int* size);

#endif