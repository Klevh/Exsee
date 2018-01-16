#!/bin/makefile

SRC2=../exsee.c
OFILES2=$(SRC2:.o=.c)
SRC=$(wildcard *.c)
OFILES=$(SRC:.o=.c)
EXEC=a.out
LDFLAGS=-g
CFLAGS=-Wall -Wextra -ansi -pedantic
CC=gcc
CLEAN=*.o *~
all:$(EXEC)
$(EXEC):$(OFILES) $(OFILES2)
	$(CC) $^ -o $@ $(LDFLAGS)
../%.o:../%.c
	$(CC) -c $< $(CFLAGS)
%.o:%.c
	$(CC) -c $< $(CFLAGS)
clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
