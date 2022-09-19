/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Contains functions to perform file IO operations
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileIO.h"
#include "macros.h"
#include "simulator.h"

/* read input file and store contentes in buffer1 */
int readFile(char* filename)
{
    int success;
    FILE* file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening the file");
        success = FALSE;
    }
    else
    {
        int num, nRead, discarded, count;
        int* requests;
        count = 0;
        discarded = 0;

        /* read first value for number of cylinders */
        nRead = fscanf(file, "%d", &num);
        
        if(nRead == 1 && num > 0)
        {
            /* creating initial buffer array to store requests of size num */
            int* requestBuffer = (int*)malloc(num * sizeof(int));
            requestBuffer[0] = num;
            count++;

            /* read the rest of the content into buffer array */
            while(fscanf(file, "%d", &num) != EOF)
            {
                if(num >= 0 && num < requestBuffer[0])
                {
                    requestBuffer[count] = num;
                    count++;
                }
                else
                {
                    printf("\nInvalid disk position read: %d", num);
                    discarded++;
                }
                
            }

            /* display total values discarded if any */
            if(discarded > 0)
            {
                printf("\nDiscarded %d invalid value(s) from %s\n", discarded, filename);
            }

            /* check if there is the minumum amount of valid values
               read to run the program */
            if(count >= 4)
            {
                /* create a dynamically allocated array based on count size */
                requests = (int*)malloc(count * sizeof(int));
                memcpy(requests, requestBuffer, count * sizeof(int));
                
                /* store size of requests, store requestList in struct */
                buffer1->size = count;
                buffer1->requestList = requests;

                isFull1 = TRUE; /* indicate that buffer 1 is filled */
                success = TRUE; /* indicate that read operation succeeded */
            }
            else /* less than minimum amount of valid values read */
            {
                printf("Error: Incorrect file format\n");
                success = FALSE;
            }

            /* free heap memory */
            free(requestBuffer);
        }
        else if(nRead == EOF) /* empty file */
        {
            printf("Error: File is empty\n");
            success = FALSE;
        }
        else /* incorrect file format */
        {
            printf("Error: Incorrect file format\n");
            success = FALSE;
        }

        if (ferror(file)) /* error while reading */
		{
			perror("Error reading from file");
            success = FALSE;
		}

		fclose(file); /* close file */
    }

    return success;
}
