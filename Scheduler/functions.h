/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Header file for functions.c
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/* Functions declarations */
void swap(int* requestList, int size, int seekIdx);
int minSeekIndex(int cur, int* requestList, int size);
int maxSeekIndex(int cur, int* requestList, int size);
int minSeekIndexBig(int cur, int i, int* requestList, int size);
int minSeekIndexSmall(int cur, int i, int* requestList, int size);

#endif