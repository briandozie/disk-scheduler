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

CFLAGS = -Wall -Werror -pedantic -ansi -c -g
OBJS = scheduler.o main.o fileIO.o functions.o
CC = gcc
EXEC = scheduler

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

main.o : main.c fileIO.h scheduler.h macros.h
	$(CC) $(CFLAGS) main.c

fileIO.o : fileIO.c fileIO.h macros.h
	$(CC) $(CFLAGS) fileIO.c

scheduler.o : scheduler.c scheduler.h macros.h functions.h
	$(CC) $(CFLAGS) scheduler.c

functions.o : functions.c functions.h
	$(CC) $(CFLAGS) functions.c

clean:
	rm -f $(EXEC) $(OBJS)