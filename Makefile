CC=g++ -std=c++0x

CFLAGS=-Wall -Werror -I ./include


all: main

main: bin/huffman.o bin/main.o src/huffman.h
	$(CC) $(CFLAGS) bin/huffman.o bin/main.o -o huffman

bin/huffman.o: src/huffman.cpp src/huffman.h
	mkdir -p bin
	$(CC) $(CFLAGS) -c src/huffman.cpp -o bin/huffman.o

bin/main.o: src/main.cpp src/huffman.h
	mkdir -p bin
	$(CC) $(CFLAGS) -c src/main.cpp -o bin/main.o

clean:
	rm -f *.o ./bin/*.o huffman *~
	rmdir bin

.PHONY: all clean