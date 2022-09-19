/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Header file for scheduler.c
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

/* Functions declarations */
int FCFS(int cur, int* requestList, int size);
int SSTF(int cur, int* requestList, int size);
int SCAN(int total, int cur, int prev, int* requestList, int size);
int CSCAN(int total, int cur, int prev, int* requestList, int size);
int LOOK(int total, int cur, int prev, int* requestList, int size);
int CLOOK(int total, int cur, int prev, int* requestList, int size);

#endif