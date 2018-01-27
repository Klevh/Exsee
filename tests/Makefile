#!/bin/makefile

SRC=$(wildcard *.c)
OFILES=$(SRC:.c=.o)
EXEC=a.out
LDFLAGS=-g
CFLAGS=-Wall -Wextra -ansi -pedantic -std=c90
CC=gcc
CLEAN=*.o *~

all:$(EXEC)
$(EXEC):$(OFILES) ../exsee.o
	$(CC) $(OFILES) exsee.o -o $@ $(LDFLAGS)

../%.o:../%.c
	$(CC) -c $< $(CFLAGS)
%.o:%.c
	$(CC) -c $< $(CFLAGS)
clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
