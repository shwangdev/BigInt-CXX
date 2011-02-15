SRC=src/
CC=g++
all:
	$(CC) $(SRC)bigint.cpp -I${SRC} -o bigint
clean:
	rm -rf *.o bigint
