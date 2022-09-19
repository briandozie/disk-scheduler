##
 # File: Makefile
 # File Created: 23 April 2022
 # Author: Brian Dozie Chong Ochulor
 # -----
 # Last Modified: 8 May 2022
 # Modified By: Brian Dozie Chong Ochulor
 # -----
 # Purpose: A makefile for the the disk scheduling simulation
##

CFLAGS = -Wall -Werror -c -g
OBJS = scheduler.o simulator.o fileIO.o functions.o
CC = gcc
EXEC = simulator

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) -lpthread

simulator.o : simulator.c fileIO.h scheduler.h macros.h
	$(CC) $(CFLAGS) simulator.c

fileIO.o : fileIO.c fileIO.h macros.h
	$(CC) $(CFLAGS) fileIO.c

scheduler.o : scheduler.c scheduler.h macros.h functions.h simulator.h
	$(CC) $(CFLAGS) scheduler.c

functions.o : functions.c functions.h
	$(CC) $(CFLAGS) functions.c

clean:
	rm -f $(EXEC) $(OBJS)