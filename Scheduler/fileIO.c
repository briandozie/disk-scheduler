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

/* read input file and store content in buffer1 */
int readFile(char* filename, int* total, int* cur,
    int* prev, int** inRequests, int* size)
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
        nRead = fscanf(file, "%d", total);

        if(nRead == 1 && (*total) > 0)
        {
            /* creating initial buffer array to store requests of size total */
            int* requestBuffer = (int*)malloc((*total) * sizeof(int));

            /* read cur and prev disk values from file */
            nRead = fscanf(file, "%d %d", cur, prev);

            /* check that cur and prev values are valid */
            if (nRead == 2 && (*cur) >= 0 && (*cur) < (*total)
            && (*prev) >= 0 && (*prev) < (*total))
            {
                /* read the disk request into buffer array */
                while(fscanf(file, "%d", &num) != EOF)
                {
                    if(num >=0 && num < (*total))
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
                if(count > 0)
                {
                    /* create a dynamically allocated array based on count size */
                    requests = (int*)malloc(count * sizeof(int));

                    /* copy request from buffer array to dynamic allocated array */
                    memcpy(requests, requestBuffer, count * sizeof(int));

                    /* assigning variables */
                    *inRequests = requests;
                    *size = count;

                    success = TRUE;
                }
                else /* incorrect file format */
                {
                    printf("Error: Incorrect file format\n");
                    success = FALSE;
                }                
            }
            else /* incorrect file format */
            {
                printf("Error: Incorrect file format\n");
                success = FALSE;
            }

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

        if (ferror(file))
        {
            perror("Error reading from file");
            success = FALSE;
        }

        fclose(file);
    }

    return success;
}
