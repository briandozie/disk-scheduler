/*	
	Author: Brian Dozie Chong Ochulor
	Creation Date: 23 April 2022
	Modification Date: 8 May 2022
	Purpose: Contains helper functions to perform seek time calculations
*/

#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

/* function to swap an element with the last index in array */
void swap(int* requestList, int size, int seekIdx)
{
    int temp;
    
    temp = requestList[size - 1];
    requestList[size - 1] = requestList[seekIdx];
    requestList[seekIdx] = temp;
}

/* function to determine the closest disk request based on current position */
int minSeekIndex(int cur, int* requestList, int size)
{
    int minSeekIdx, minSeekTime, seekTime, i;

    minSeekIdx = 0;
    minSeekTime = abs(cur - requestList[minSeekIdx]);
    
    for(i = 1; i < size; i++)
    {
        seekTime = abs(cur - requestList[i]);
    
        if(seekTime < minSeekTime)
        {
            minSeekIdx = i;
            minSeekTime = seekTime;
        }
    }

    return minSeekIdx;
}

/* function to determine the furthest disk request based on current position */
int maxSeekIndex(int cur, int* requestList, int size)
{
    int maxSeekIdx, maxSeekTime, seekTime, i;
    
    maxSeekIdx = 0;
    maxSeekTime = abs(cur - requestList[maxSeekIdx]);

    for(i = 1; i < size; i++)
    {
        seekTime = abs(cur - requestList[i]);
    
        if(seekTime > maxSeekTime)
        {
            maxSeekIdx = i;
            maxSeekTime = seekTime;
        }
    }

    return maxSeekIdx;
}

/* function to determine the closest disk request (towards larger disks)
based on current position */
int minSeekIndexBig(int cur, int i, int* requestList, int size)
{
    int minSeekIdx, minSeekTime, seekTime;
    
    minSeekIdx = i;
    minSeekTime = abs(cur - requestList[minSeekIdx]);

    for(i = i + 1; i < size; i++)
    {
        if(requestList[i] > cur)
        {
            seekTime = abs(cur - requestList[i]);
        
            if(seekTime < minSeekTime)
            {
                minSeekIdx = i;
                minSeekTime = seekTime;
            }
        }
    }

    return minSeekIdx;
}

/* function to determine the closest disk request (towards smaller disks)
based on current position */
int minSeekIndexSmall(int cur, int i, int* requestList, int size)
{
    int minSeekIdx, minSeekTime, seekTime;
    
    minSeekIdx = i;
    minSeekTime = abs(cur - requestList[minSeekIdx]);

    for(i = i + 1; i < size; i++)
    {
        if(requestList[i] < cur)
        {
            seekTime = abs(cur - requestList[i]);
        
            if(seekTime < minSeekTime)
            {
                minSeekIdx = i;
                minSeekTime = seekTime;
            }
        }
    }

    return minSeekIdx;
}