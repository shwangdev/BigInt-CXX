SRC=src/
BIN=bin/
CC=g++
CFLAGS=-g -O0 -da -Q
EXEC=$(BIN)bigint
SOURCE=$(SRC)bigint.cpp
INCLUDE=$(SRC)bigint.h
all:
	$(CC) $(SOURCE) -I$(SRC) $(CFLAGS) -o $(EXEC)
clean:
	rm -rf *.o $(EXEC)
