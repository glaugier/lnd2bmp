all : standardMake

standardMake:
	gcc -std=c99 -o bin/lnd2bmp -Wall -g lnd2bmp.c

clean:
	rm -f bin/*
