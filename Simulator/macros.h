/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Contains macros for the Simulator program
*/

#ifndef MACROS_H
#define MACROS_H

#define FALSE 0 /* boolean variable for FALSE */
#define TRUE !FALSE /* boolean variable for TRUE */
#define MAX_FILENAME_LENGTH 11 /* maximum filename length (10 characters + null terminator) */
#define MAX_ALGONAME_LENGTH 7 /* maximum algorithm name length (6 characters + null terminator) */
#define NUM_THREADS 6 /* number of threads */

/* struct for buffer 1 */
typedef struct
{
    int* requestList;
    int size;
} Buffer1;

/* struct for buffer 2 */
typedef struct
{
    int result;
    char algorithmName[MAX_ALGONAME_LENGTH];
} Buffer2;

#endif