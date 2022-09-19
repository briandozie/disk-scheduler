/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Contains functions to perform seek time calculations
             for the 6 different disk scheduling algorithms
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
#include "functions.h"

/* function to perform FCFS disk scheduling algorithm */
void FCFS()
{
    int i, cur, size, totalMoves;
    int* requestListCopy;
    
    while(!quit)
    {
        pthread_mutex_lock(&lock1);
        
        /* check if buffer 1 filled and thread has not completed
        its work in this cycle before proceeding */
        while(!isFull1 || threadDone[0]) 
        {
            pthread_cond_wait(&calcResult, &lock1);
        }

        /* check if thread needs to terminate */
        if(!quit)
        {
            /* copy values of buffer to local variables to avoid modifying
            original buffer values */
            cur = buffer1->requestList[1];
            size = buffer1->size - 3;
            requestListCopy = (int*)malloc(size * sizeof(int));
            memcpy(requestListCopy, buffer1->requestList + 3, size * sizeof(int));
            pthread_mutex_unlock(&lock1);
            
            totalMoves = 0;

            for(i = 0; i < size; i++)
            {
                totalMoves += abs(cur - requestListCopy[i]);
                cur = requestListCopy[i];
            }

            /* fill buffer 2 with calculation result */
            pthread_mutex_lock(&lock2);
            while(isFull2) /* check if buffer 2 is empty before filling it */
            {
                pthread_cond_wait(&isEmpty2, &lock2);
            }
            strncpy(buffer2->algorithmName, "FCFS", MAX_ALGONAME_LENGTH);
            buffer2->result = totalMoves;
            isFull2 = TRUE;
                            
            pthread_cond_signal(&readResult); /* signal parent to read result from buffer 2 */
            pthread_mutex_unlock(&lock2);

            /* set the thread status to done */
            pthread_mutex_lock(&lockDone);
            threadDone[0] = TRUE;
            pthread_mutex_unlock(&lockDone);

            free(requestListCopy); /* free heap memery */
        }
        else
        {
            pthread_mutex_unlock(&lock1);
            pthread_exit(NULL); /* terminate thread */
        }
    }
}

/* function to perform SSTF disk scheduling algorithm */
void SSTF()
{
    int totalMoves;
    int minSeekIdx, cur, size;
    int* requestListCopy;

    while(!quit)
    {
        pthread_mutex_lock(&lock1);
        
        /* check if buffer 1 filled and thread has not completed
        its work in this cycle before proceeding */
        while(!isFull1 || threadDone[1])
        {
            pthread_cond_wait(&calcResult, &lock1);
        }

        /* check if thread needs to terminate */
        if(!quit)
        {
            /* copy values of buffer to local variables to avoid modifying
            original buffer values */
            cur = buffer1->requestList[1];
            size = buffer1->size - 3;
            requestListCopy = (int*)malloc(size * sizeof(int));
            memcpy(requestListCopy, buffer1->requestList + 3, size * sizeof(int));
            pthread_mutex_unlock(&lock1);
            
            totalMoves = 0;

            do
            {
                /* get the index of the closest request */
                minSeekIdx = minSeekIndex(cur, requestListCopy, size);
                totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                cur = requestListCopy[minSeekIdx];

                /* swap the current disk position with the last element in array */
                swap(requestListCopy, size, minSeekIdx);

                size--;
                requestListCopy = realloc(requestListCopy, size * sizeof(int));
            } while(size > 0);

            /* fill buffer 2 with calculation result */
            pthread_mutex_lock(&lock2);
            while(isFull2) /* check if buffer 2 is empty before filling it */
            {
                pthread_cond_wait(&isEmpty2, &lock2);
            }
            strncpy(buffer2->algorithmName, "SSTF", MAX_ALGONAME_LENGTH);
            buffer2->result = totalMoves;
            isFull2 = TRUE;
                            
            pthread_cond_signal(&readResult); /* signal parent to read result from buffer 2 */
            pthread_mutex_unlock(&lock2);

            /* set the thread status to done */
            pthread_mutex_lock(&lockDone);
            threadDone[1] = TRUE;
            pthread_mutex_unlock(&lockDone);

            free(requestListCopy); /* free heap memery */
        }
        else
        {
            pthread_mutex_unlock(&lock1);
            pthread_exit(NULL); /* terminate thread */
        }
    }
}

/* function to perform SCAN disk scheduling algorithm */
void SCAN()
{
    int minSeekIdx, i, done, cur, size, totalMoves, prev, total;
    int* requestListCopy;

    while(!quit)
    {
        pthread_mutex_lock(&lock1);
        
        /* check if buffer 1 filled and thread has not completed
        its work in this cycle before proceeding */
        while(!isFull1 || threadDone[2])
        {
            pthread_cond_wait(&calcResult, &lock1);
        }
        
        /* check if thread needs to terminate */
        if(!quit)
        {  
            /* copy values of buffer to local variables to avoid modifying
            original buffer values */
            total = buffer1->requestList[0];
            cur = buffer1->requestList[1];
            prev = buffer1->requestList[2];
            size = buffer1->size - 3;
            requestListCopy = (int*)malloc(size * sizeof(int));
            memcpy(requestListCopy, buffer1->requestList + 3, size * sizeof(int));
            pthread_mutex_unlock(&lock1);

            totalMoves = 0;
            done = FALSE;
            i = 0;

            if(cur > prev) /* moving towards bigger number */
            {  
                while(!done)
                {
                    /* search the array for the first bigger disk request */
                    if(size > 0 && requestListCopy[i] > cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the bigger numbered disks */
                        minSeekIdx = minSeekIndexBig(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1))
                        {
                            /* no more bigger requests, reverse direction */
                            totalMoves += ((total - 1) - cur);
                            cur = (total - 1);
                            done = TRUE; 
                        }
                        else if(size == 0)
                        {
                            done = TRUE;
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }
            else /* moving towards smaller number */
            {
                while(!done)
                {
                    /* search the array for the first smaller disk request */
                    if(size > 0 && requestListCopy[i] < cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the smaller numbered disks */
                        minSeekIdx = minSeekIndexSmall(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1))
                        {
                            /* no more smaller requests, reverse direction */
                            totalMoves += cur;
                            cur = 0;
                            done = TRUE; 
                            
                        }
                        else if(size == 0)
                        {
                            done = TRUE; /* no futher disk request */
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }

            /* scan in the reverse direction */
            while(size > 0)
            {
                /* get the index of the closest disk request */
                minSeekIdx = minSeekIndex(cur, requestListCopy, size);
                totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                cur = requestListCopy[minSeekIdx];

                /* swap the current disk position with the last element in array */
                swap(requestListCopy, size, minSeekIdx);

                size--;
                requestListCopy = realloc(requestListCopy, size * sizeof(int));
            }

            /* fill buffer 2 with calculation result */
            pthread_mutex_lock(&lock2);
            while(isFull2) /* check if buffer 2 is empty before filling it */
            {
                pthread_cond_wait(&isEmpty2, &lock2);
            }
            strncpy(buffer2->algorithmName, "SCAN", MAX_ALGONAME_LENGTH);
            buffer2->result = totalMoves;
            isFull2 = TRUE;
                            
            pthread_cond_signal(&readResult); /* signal parent to read result from buffer 2 */
            pthread_mutex_unlock(&lock2);

            /* set the thread status to done */
            pthread_mutex_lock(&lockDone);
            threadDone[2] = TRUE;
            pthread_mutex_unlock(&lockDone);

            free(requestListCopy); /* free heap memery */
        }
        else
        {
            pthread_mutex_unlock(&lock1);
            pthread_exit(NULL); /* terminate thread */
        }
    }
}

/* function to perform CSAN disk scheduling algorithm */
void CSCAN()
{
    int minSeekIdx, i, done, totalMoves, cur, prev, total, size;
    int* requestListCopy;

    while(!quit)
    {
        pthread_mutex_lock(&lock1);
        
        /* check if buffer 1 filled and thread has not completed
        its work in this cycle before proceeding */
        while(!isFull1 || threadDone[3])
        {
            pthread_cond_wait(&calcResult, &lock1);
        }

        /* check if thread needs to terminate */
        if(!quit)
        {
            /* copy values of buffer to local variables to avoid modifying
            original buffer values */
            total = buffer1->requestList[0];
            cur = buffer1->requestList[1];
            prev = buffer1->requestList[2];
            size = buffer1->size - 3;
            requestListCopy = (int*)malloc(size * sizeof(int));
            memcpy(requestListCopy, buffer1->requestList + 3, size * sizeof(int));
            pthread_mutex_unlock(&lock1);

            totalMoves = 0;
            done = FALSE;
            i = 0;

            if(cur > prev) /* moving towards bigger number */
            {  
                while(!done)
                {
                    /* search the array for the first bigger disk request */
                    if(size > 0 && requestListCopy[i] > cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the bigger numbered disks */
                        minSeekIdx = minSeekIndexBig(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1))
                        {
                            /* no more bigger requests, move to other end */
                            totalMoves += (((total - 1) - cur) + (total - 1));
                            cur = 0;
                            done = TRUE; 
                        }
                        else if(size == 0)
                        {
                            done = TRUE; /* no futher disk request */
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }
            else /* moving towards smaller number */
            {
                while(!done)
                {
                    /* search the array for the first smaller disk request */
                    if(size > 0 && requestListCopy[i] < cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the smaller numbered disks */
                        minSeekIdx = minSeekIndexSmall(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1))
                        {
                            /* no more smaller requests, head moves to other end */
                            totalMoves += (cur + (total - 1));
                            cur = total - 1;
                            done = TRUE;
                        }
                        else if(size == 0)
                        {
                            done = TRUE; /* no futher disk request */
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }

            /* continue scan from other end */
            while(size > 0)
            {
                /* get the index of the closest disk request */
                minSeekIdx = minSeekIndex(cur, requestListCopy, size);
                totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                cur = requestListCopy[minSeekIdx];

                /* swap the current disk position with the last element in array */
                swap(requestListCopy, size, minSeekIdx);

                size--;
                requestListCopy = realloc(requestListCopy, size * sizeof(int));
            }

            /* fill buffer 2 with calculation result */
            pthread_mutex_lock(&lock2);
            while(isFull2) /* check if buffer 2 is empty before filling it */
            {
                pthread_cond_wait(&isEmpty2, &lock2);
            }
            strncpy(buffer2->algorithmName, "C-SCAN", MAX_ALGONAME_LENGTH);
            buffer2->result = totalMoves;
            isFull2 = TRUE;
                            
            pthread_cond_signal(&readResult); /* signal parent to read result from buffer 2 */
            pthread_mutex_unlock(&lock2);

             /* set the thread status to done */
            pthread_mutex_lock(&lockDone);
            threadDone[3] = TRUE;
            pthread_mutex_unlock(&lockDone);

            free(requestListCopy); /* free heap memery */
        }
        else
        {
            pthread_mutex_unlock(&lock1);
            pthread_exit(NULL); /* terminate thread */
        }
    }
}

/* function to perform LOOK disk scheduling algorithm */
void LOOK()
{
    int minSeekIdx, i, done, totalMoves, cur, prev, size;
    int* requestListCopy;

    while(!quit)
    {
        pthread_mutex_lock(&lock1);
        
        /* check if buffer 1 filled and thread has not completed
        its work in this cycle before proceeding */
        while(!isFull1 || threadDone[4])\
        {
            pthread_cond_wait(&calcResult, &lock1);
        }

        /* check if thread needs to terminate */
        if(!quit)
        {
            /* copy values of buffer to local variables to avoid modifying
            original buffer values */
            cur = buffer1->requestList[1];
            prev = buffer1->requestList[2];
            size = buffer1->size - 3;
            requestListCopy = (int*)malloc(size * sizeof(int));
            memcpy(requestListCopy, buffer1->requestList + 3, size * sizeof(int));
            pthread_mutex_unlock(&lock1);

            totalMoves = 0;
            done = FALSE;
            i = 0;

            if(cur > prev) /* moving towards bigger number */
            {  
                while(!done)
                {
                    /* search the array for the first bigger disk request */
                    if(size > 0 && requestListCopy[i] > cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the bigger numbered disks */
                        minSeekIdx = minSeekIndexBig(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1) || size == 0)
                        {
                            /* no more bigger requests, reverse direction */
                            done = TRUE; 
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }
            else /* moving towards smaller number */
            {
                while(!done)
                {
                    /* search the array for the first smaller disk request */
                    if(size > 0 && requestListCopy[i] < cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the smaller numbered disks */
                        minSeekIdx = minSeekIndexSmall(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1) || size == 0)
                        {
                            /* no more smaller requests, reverse direction */
                            done = TRUE; 
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }

            /* scan in the reverse direction */
            while(size > 0)
            {
                /* get the index of the closest disk request */
                minSeekIdx = minSeekIndex(cur, requestListCopy, size);
                totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                cur = requestListCopy[minSeekIdx];

                /* swap the current disk position with the last element in array */
                swap(requestListCopy, size, minSeekIdx);

                size--;
                requestListCopy = realloc(requestListCopy, size * sizeof(int));
            }

            /* fill buffer 2 with calculation result */
            pthread_mutex_lock(&lock2);
            while(isFull2) /* check if buffer 2 is empty before filling it */
            {
                pthread_cond_wait(&isEmpty2, &lock2);
            }
            strncpy(buffer2->algorithmName, "LOOK", MAX_ALGONAME_LENGTH);
            buffer2->result = totalMoves;
            isFull2 = TRUE;
                            
            pthread_cond_signal(&readResult); /* signal parent to read result from buffer 2 */
            pthread_mutex_unlock(&lock2);

             /* set the thread status to done */
            pthread_mutex_lock(&lockDone);
            threadDone[4] = TRUE;
            pthread_mutex_unlock(&lockDone);

            free(requestListCopy); /* free heap memery */
        }
        else
        {
            pthread_mutex_unlock(&lock1);
            pthread_exit(NULL); /* terminate thread */
        }  
    }
}
    
/* function to perform CLOOK disk scheduling algorithm */
void CLOOK()
{
    int minSeekIdx, i, done, totalMoves, maxSeekIdx, cur, prev, size;
    int* requestListCopy;

    while(!quit)
    {
        pthread_mutex_lock(&lock1);
        
        /* check if buffer 1 filled and thread has not completed
        its work in this cycle before proceeding */
        while(!isFull1 || threadDone[5])
        {
            pthread_cond_wait(&calcResult, &lock1);
        }

        /* check if thread needs to terminate */
        if(!quit)
        {
            /* copy values of buffer to local variables to avoid modifying
            original buffer values */
            cur = buffer1->requestList[1];
            prev = buffer1->requestList[2];
            size = buffer1->size - 3;
            requestListCopy = (int*)malloc(size * sizeof(int));
            memcpy(requestListCopy, buffer1->requestList + 3, size * sizeof(int));
            pthread_mutex_unlock(&lock1);

            totalMoves = 0;
            done = FALSE;
            i = 0;

            if(cur > prev) /* moving towards bigger number */
            {  
                while(!done)
                {
                    /* search the array for the first bigger disk request */
                    if(size > 0 && requestListCopy[i] > cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the bigger numbered disks */
                        minSeekIdx = minSeekIndexBig(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1))
                        {
                            /* no more bigger requests, move to last request on other end */
                            maxSeekIdx = maxSeekIndex(cur, requestListCopy, size);
                            totalMoves += abs(cur - requestListCopy[maxSeekIdx]);
                            cur = requestListCopy[maxSeekIdx];

                            /* swap the current disk position with the last element in array */
                            swap(requestListCopy, size, maxSeekIdx);

                            size--;
                            requestListCopy = realloc(requestListCopy, size * sizeof(int));

                            done = TRUE; 
                        }
                        else if(size == 0)
                        {
                            done = TRUE; /* no futher disk request */
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }
            else /* moving towards smaller number */
            {
                while(!done)
                {
                    /* search the array for the first smaller disk request */
                    if(size > 0 && requestListCopy[i] < cur)
                    {
                        /* search array for the closest disk request in the direction
                        of the smaller numbered disks */
                        minSeekIdx = minSeekIndexSmall(cur, i, requestListCopy, size);
                        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                        cur = requestListCopy[minSeekIdx];

                        /* swap the current disk position with the last element in array */
                        swap(requestListCopy, size, minSeekIdx);

                        size--;
                        requestListCopy = realloc(requestListCopy, size * sizeof(int));
                        i = 0;
                    }
                    else
                    {
                        if(i == (size - 1))
                        {
                            /* no more smaller requests, head moves to last request on other end */
                            maxSeekIdx = maxSeekIndex(cur, requestListCopy, size);
                            totalMoves += abs(cur - requestListCopy[maxSeekIdx]);
                            cur = requestListCopy[maxSeekIdx];

                            /* swap the current disk position with the last element in array */
                            swap(requestListCopy, size, maxSeekIdx);

                            size--;
                            requestListCopy = realloc(requestListCopy, size * sizeof(int));

                            done = TRUE;
                        }
                        else if(size == 0)
                        {
                            done = TRUE; /* no futher disk request */
                        }
                        else
                        {
                            i++;
                        }
                    }
                }
            }

            /* continue scan from other end */
            while(size > 0)
            {
                /* get the index of the closest disk request */
                minSeekIdx = minSeekIndex(cur, requestListCopy, size);
                totalMoves += abs(cur - requestListCopy[minSeekIdx]);
                cur = requestListCopy[minSeekIdx];

                /* swap the current disk position with the last element in array */
                swap(requestListCopy, size, minSeekIdx);

                size--;
                requestListCopy = realloc(requestListCopy, size * sizeof(int));
            }

            /* fill buffer 2 with calculation result */
            pthread_mutex_lock(&lock2);
            while(isFull2) /* check if buffer 2 is empty before filling it */
            {
                pthread_cond_wait(&isEmpty2, &lock2);
            }
            strncpy(buffer2->algorithmName, "C-LOOK", MAX_ALGONAME_LENGTH);
            buffer2->result = totalMoves;
            isFull2 = TRUE;
                            
            pthread_cond_signal(&readResult); /* signal parent to read result from buffer 2 */
            pthread_mutex_unlock(&lock2);

            /* set the thread status to done */
            pthread_mutex_lock(&lockDone);
            threadDone[5] = TRUE;
            pthread_mutex_unlock(&lockDone);

            free(requestListCopy); /* free heap memery */
        }
        else
        { 
            pthread_mutex_unlock(&lock1);
            pthread_exit(NULL); /* terminate thread */
        }
    }
}

        