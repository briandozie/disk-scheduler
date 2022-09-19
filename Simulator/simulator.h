/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022 
	Purpose: Header file for simulator.c
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "macros.h"

/* global variables */
extern Buffer1* buffer1;
extern Buffer2* buffer2;
extern pthread_mutex_t lock1;
extern pthread_mutex_t lock2;
extern pthread_mutex_t lockDone;
extern pthread_cond_t calcResult;
extern pthread_cond_t readResult;
extern pthread_cond_t isEmpty2;
extern int threadDone[NUM_THREADS];
extern int isFull1;
extern int isFull2;
extern int quit;

#endif