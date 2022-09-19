/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Contains the main method for the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileIO.h"
#include "scheduler.h"
#include "macros.h"

int main(int argc, char* argv[])
{
    char filename[MAX_FILENAME_LENGTH];
	int total, cur, prev, size, exit;
	int* requestList;

	exit = FALSE;
	
	do
	{
		printf("\nDisk Scheduler Simulation: ");
		scanf("%10s", filename); /* get user input fo filename */

		if(strcmp(filename, "QUIT") != 0)
		{
			if(readFile(filename, &total, &cur, &prev, &requestList, &size))
			{
				printf("\nFor %s:\n", filename);

				/* perform seek time calculations */
				printf("FCFS: %d\n", FCFS(cur, requestList, size));
				printf("SSTF: %d\n", SSTF(cur, requestList, size));
				printf("SCAN: %d\n", SCAN(total, cur, prev, requestList, size));
				printf("C-SCAN: %d\n", CSCAN(total, cur, prev, requestList, size));
				printf("LOOK: %d\n", LOOK(total, cur, prev, requestList, size));
				printf("C-LOOK: %d\n", CLOOK(total, cur, prev, requestList, size));

				free(requestList); /* free heap memory */
			}
		}
		else
		{
			exit = TRUE;
		}
	} while(!exit);
	
	printf("\n~ Program terminated ~\n");
	return 0;
}