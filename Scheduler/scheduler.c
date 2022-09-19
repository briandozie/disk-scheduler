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
#include "scheduler.h"
#include "macros.h"
#include "functions.h"

/* function to perform FCFS disk scheduling algorithm */
int FCFS(int cur, int* requestList, int size)
{
    int i;
    int totalMoves = 0;

    for(i = 0; i < size; i++)
    {
        totalMoves += abs(cur - requestList[i]);
        cur = requestList[i];
    }

    return totalMoves;
}

/* function to perform SSTF disk scheduling algorithm */
int SSTF(int cur, int* requestList, int size)
{
    int totalMoves = 0;
    int minSeekIdx;
    int* requestListCopy;

    /* copy the array to avoid modifying the original imported array */
    requestListCopy = (int*)malloc(size * sizeof(int));
    memcpy(requestListCopy, requestList, size * sizeof(int));

    do
    {
        /* get the index of the closest request */
        minSeekIdx = minSeekIndex(cur, requestListCopy, size);
        totalMoves += abs(cur - requestListCopy[minSeekIdx]);
        cur = requestListCopy[minSeekIdx];

        /* swap the current disk position with the last element in array */
        swap(requestListCopy, size, minSeekIdx);
        
        /* resize the array */
        size--;
        requestListCopy = realloc(requestListCopy, size * sizeof(int));
    } while(size > 0);

    free(requestListCopy);
    return totalMoves;
}

/* function to perform SCAN disk scheduling algorithm */
int SCAN(int total, int cur, int prev, int* requestList, int size)
{
    int minSeekIdx, i, done, totalMoves;
    int* requestListCopy;

    totalMoves = 0;
    done = FALSE;
    i = 0;

    /* copy the array to avoid modifying the original imported array */
    requestListCopy = (int*)malloc(size * sizeof(int));
    memcpy(requestListCopy, requestList, size * sizeof(int));

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

                /* resize the array */
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

                /* resize the array */
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

        /* resize the array */
        size--;
        requestListCopy = realloc(requestListCopy, size * sizeof(int));
    }

    free(requestListCopy);
    return totalMoves;
}

/* function to perform CSAN disk scheduling algorithm */
int CSCAN(int total, int cur, int prev, int* requestList, int size)
{
    int minSeekIdx, i, done, totalMoves;
    int* requestListCopy;

    totalMoves = 0;
    done = FALSE;
    i = 0;

    /* copy the array to avoid modifying the original imported array */
    requestListCopy = (int*)malloc(size * sizeof(int));
    memcpy(requestListCopy, requestList, size * sizeof(int));

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

                /* resize the array */
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

                /* resize the array */
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

        /* resize the array */
        size--;
        requestListCopy = realloc(requestListCopy, size * sizeof(int));
    }

    free(requestListCopy);
    return totalMoves;
}

/* function to perform LOOK disk scheduling algorithm */
int LOOK(int total, int cur, int prev, int* requestList, int size)
{
    int minSeekIdx, i, done, totalMoves;
    int* requestListCopy;

    totalMoves = 0;
    done = FALSE;
    i = 0;

    /* copy the array to avoid modifying the original imported array */
    requestListCopy = (int*)malloc(size * sizeof(int));
    memcpy(requestListCopy, requestList, size * sizeof(int));

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

                /* resize the array */
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
        
        /* resize the array */
        size--;
        requestListCopy = realloc(requestListCopy, size * sizeof(int));
    }

    free(requestListCopy);
    return totalMoves;
}

/* function to perform CLOOK disk scheduling algorithm */
int CLOOK(int total, int cur, int prev, int* requestList, int size)
{
    int minSeekIdx, i, done, totalMoves, maxSeekIdx;
    int* requestListCopy;

    totalMoves = 0;
    done = FALSE;
    i = 0;

    /* copy the array to avoid modifying the original imported array */
    requestListCopy = (int*)malloc(size * sizeof(int));
    memcpy(requestListCopy, requestList, size * sizeof(int));

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

                /* resize the array */
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

                /* resize the array */
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

                    /* resize the array */
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

        /* resize the array */
        size--;
        requestListCopy = realloc(requestListCopy, size * sizeof(int));
    }

    free(requestListCopy);
    return totalMoves;
}       