CC=gcc

all: checker.c Image.o
	$(CC) -o checker -Wall -std=gnu99 checker.c ../Image.o -D_GNU_SOURCE
Image.o: ../Image.c
	$(CC) -o ../Image.o -c -std=gnu99 -Wall ../Image.c -D_GNU_SOURCE
clean:
	rm -rf ../Image.o checker
