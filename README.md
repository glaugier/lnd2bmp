lnd2bmp
=======

A program that allows to generate a bitmap image from a landscape stored as a semicolumn-separated csv file.

Usage
-----

### Compilation

Open a terminal, change directory to where the root of this file, and type `make`.
Compilation should occur, generating `lnd2bmp` executable in the local `bin/` directory.

Alternatively, this can be compiled by hand, typing this minimal command line:

```shell
gcc lnd2bmp.c -o bin/lnd2bmp
```

### Creating bitmap from csv landscapes

The program works in command line interface and take a single argument:
the name (and path) of the `.csv` file.
For instance

```shell
	./lnd2bmp path/to/my/landscape.csv
```

Will create landscape.bmp into bmp/ (directory will be created if needed).


Troubleshooting
---------------

 + Might work only on Linux, due to mkdir and file permission.
 + Sometimes, the resulting bitmap file has a shorter extension name, such as ".bm". Might come from memory allocation.

TODO
----

In addition to solving the problems listed above:

	+ Clean the unused variables
	+ Run memory leaks checks and patch the leaks.
	+ Colours
		- Automatic palette generation
		- Allow to pass custom palettes
		- Allow to work with more than 3 states (4+ colours)

