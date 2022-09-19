/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Contains the main method to run the Simulator program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "scheduler.h"
#include "simulator.h"
#include "macros.h"
#include "fileIO.h"

Buffer1* buffer1; /* struct for buffer 1 */
Buffer2* buffer2; /* struct for buffer 2 */
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER; /* mutex lock for buffer 1 */
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER; /* mutex lock for buffer 2 */
pthread_mutex_t lockDone = PTHREAD_MUTEX_INITIALIZER; /* mutex lock for threadDone array */
pthread_cond_t calcResult = PTHREAD_COND_INITIALIZER; /* condition variable to calculate result */
pthread_cond_t readResult = PTHREAD_COND_INITIALIZER; /* condition variable to read result */
pthread_cond_t isEmpty2 = PTHREAD_COND_INITIALIZER; /* condition variable to represent empty buffer 2 */
int threadDone[NUM_THREADS]; /* array to keep track of threads that have completed their cycle */
int isFull1; /* boolean variable to represent the state of buffer 1 */
int isFull2; /* boolean variable to represent the state of buffer 2 */
int quit; /* boolean variable to determine if the program should quit */

int main(void)
{
    int i, readSuccess;
    char filename[MAX_FILENAME_LENGTH];
    pthread_t threads[NUM_THREADS];

    quit = FALSE;
    
    /* array of function pointers to the various scheduling algorithms */
    void (*algorithm_ptr[6])() = {FCFS, SSTF, SCAN, CSCAN, LOOK, CLOOK};

    /* allocating memory for buffer 1 and 2 */
    buffer1 = (Buffer1*)malloc(sizeof(Buffer1));
    buffer2 = (Buffer2*)malloc(sizeof(Buffer2));

    /* creating the threads */
    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, (void*) (*algorithm_ptr[i]), NULL);
    }

    do
    {
        /* reset the status of program for new file read */
        pthread_mutex_lock(&lock1);
        /* indicate that buffer1 is not filled */
        buffer1->requestList = NULL;
        buffer1->size = 0;
        isFull1 = FALSE; 
        pthread_mutex_unlock(&lock1);

        pthread_mutex_lock(&lock2);
        isFull2 = FALSE; /* indicate that buffer2 is not filled */
        pthread_mutex_unlock(&lock2);

        pthread_mutex_lock(&lockDone);
        for(i = 0; i < NUM_THREADS; i++)
        {
            threadDone[i] = FALSE;
        }
        pthread_mutex_unlock(&lockDone);

        readSuccess = FALSE;
        
        /* get input filename from user */
        printf("\nDisk Scheduler Simulation: ");
        scanf("%10s", filename);
        
        if(strcmp(filename, "QUIT") != 0)
        {
            /* read file and store contents to buffer */
            pthread_mutex_lock(&lock1);
            readSuccess = readFile(filename);
            pthread_mutex_unlock(&lock1);

            if(readSuccess)
            {     
                printf("\nFor %s:\n", filename);
                
                /* signal threads to peform disk scheduling calculations */
                for(i = 0; i < NUM_THREADS; i++)
                {
                    pthread_cond_signal(&calcResult);
                }
                
                for(i = 0; i < NUM_THREADS; i++)
                {
                    pthread_mutex_lock(&lock2);
                    while(!isFull2) /* check if buffer 2 is filled before reading */
                    {
                        pthread_cond_wait(&readResult, &lock2);
                    }
                    printf("%s: %d\n", buffer2->algorithmName, buffer2->result);

                    /* indicate that buffer 2 is empty */
                    strcpy(buffer2->algorithmName, "");
                    buffer2->result = 0;
                    isFull2 = FALSE; 
                    
                    pthread_cond_signal(&isEmpty2); /* signal threads to indicate buffer 2 is empty */
                    pthread_mutex_unlock(&lock2);
                }

                free(buffer1->requestList); /* free heap memory */
            }
        }
        else
        {
            quit = TRUE;

            pthread_mutex_lock(&lock1);
            isFull1 = TRUE; /* indicate that buffer 1 is full */
            pthread_mutex_unlock(&lock1);

            /* signal threads to wakeup and terminate */
            for(i = 0; i < NUM_THREADS; i++)
            {
                pthread_cond_signal(&calcResult);
            }
            
            printf("\n");
            for(i = 0; i < NUM_THREADS; i++) /* wait for all threads to terminate */
            {
                assert(!pthread_join(threads[i], NULL));
                printf("Thread %ld has terminated\n", threads[i]);
            }
        }
    } while(!quit);
    
    /* free heap memory */
    free(buffer1);
    free(buffer2);

    printf("\n~ Program terminated ~\n");
	return 0;
}