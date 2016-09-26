all : standardMake

standardMake:
	gcc -o bin/lnd2bmp -Wall -g lnd2bmp.c

clean:
	rm -f bin/*
