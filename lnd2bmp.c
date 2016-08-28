////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// PROLOGUE /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* 
This program aims to create a .bmp graphic file from a csv ";"
landscape file.
*/
 
 
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// HEADER ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#if SC_PLATFORM == SC_PLATFORM_LINUX
#include <errno.h>
#endif
#include <unistd.h> // sinon error implicit declaration of
						//function 'srandom' 'bzero'
#define max(a,b) (a>b?a:b)
#define min(a,b) (a>b?b:a)
// Todo: put this inside hk function
int *labels;
int  n_labels = 0;     /* length of the labels array */
////////////////////////////////////////////////////////////////////////////////


//using namespace std;




void countStats(int **L, int count[9], int x, int y,
				int x_size, int y_size, int NEonly);
void split_path_file(char** p, char** f, char *pf);
char *remove_ext(char* mystr, char dot, char sep);
extern char *strdup(const char *s);

typedef unsigned char byte;  /* just for convenience */

/*
* Definitions: penser a def les variables qu'on va lire dans le fichier
*/
FILE *infile, *outFile;
int x_size, y_size;  /* size of landscape */
int r, g, b;  /* rgb values of landscape */

char version[10] = "v0.02" ;

/*
 * Main() routine.
 */
int main(int argc, char **argv)
{
////////////////////////////////////////////////////////////////////////////////
/*
* 0] PREAMBULE
*/
printf("╔═══════════╗\n");
printf("║ Landscape ║\n");
printf("║ to Bitmap ║ %s\n", version);
printf("╚═══════════╝\n\n");

// Check arguments
	if( argv[1] == NULL ) {
		perror("Error: Please provide an input CSV file in the first argument.\n");
		exit(EXIT_FAILURE);
	}



	// Get input name from first arg
	char inputName[strlen(argv[1])+1];
	strcpy(inputName, argv[1]);
	// Create output name
	//~ char dirName[5] = "bmp";
	char **fileName = malloc ( (strlen(argv[1])+10)*sizeof(char) );
	char **pathName = malloc ( (strlen(argv[1])+10)*sizeof(char) );
	char *outputName = malloc ( (strlen(argv[1])+10)*sizeof(char) );
	split_path_file(&*pathName, &*fileName, argv[1]);
	char *dirPathName = malloc ( (strlen(argv[1]) + 3)*sizeof(char) );


	
	//~ // Check if dir exists
	//~ DIR* dir = opendir(dirName);
//~ if (dir)
//~ {
    //~ /* Directory exists. */
    //~ closedir(dir);
//~ }
//~ else if (ENOENT == errno)
//~ {
//~ //printf("\nmouchard\n");
    //~ /* Directory does not exist. */
//~ printf("%s does not exist, creating it.\n", dirName);
    //~ mkdir(dirName, // Needs no final "/"
			//~ S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IWUSR | S_IWGRP);
			//~ // Force permission to drwxr-xr--
//~ }
//~ else
//~ {
    //~ /* opendir() failed for some other reason. */
    //~ perror("Error while creating 'bmp/'.\n");
//~ }
	//~ outputName = strcat(dirName, "/"); // Update dirname
	dirPathName[0] = '\0';
	strcpy(dirPathName, *pathName);
	//~ dirPathName = strcat(dirPathName, dirName);
	printf("fileName: %s\n", *fileName);
	*fileName = remove_ext(*fileName, '.', '/');
//~ printf("Dirname is: %s\n", dirPathName);
	//~ strcpy(outputName, dirName);
	//~ outputName = strcat(outputName, *fileName);
	//~ outputName = strcat(outputName, ".bmp");
	//~ outputName[strlen(outputName)+1] = '\0';
//~ printf("Outputname will be: %s\n", outputName);
//~ printf("Outputname will be: %lu\n", strlen(outputName));


infile=fopen(inputName, "r"); // Ouverture du fichier infile
	if( infile == NULL ) {
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}



printf("Reading: %s\n", inputName);

// Reading the number of lines and columbs in input file
y_size = 0; x_size = 1;
int ch = 0;

do 
{
    ch = fgetc(infile);
    if(ch == '\n')
    	y_size++; // Warning! only works because generated files end with and empty line...
    if(y_size == 0 && ch == ';') // First line only
    	x_size++;
} while (ch != EOF);

// last line doesn't end with a new line!
// but there has to be a line at least before the last line
//~ if(ch != '\n' && y_size != 0) 
    //~ y_size++;
//~ 
fclose(infile); // We need to close the file and open it again later.

printf("\tDetected %d lines and %d columns in %s\n", y_size, x_size, argv[1]);


// Initialising the landscape
//int Lsize = x_size*y_size;
int i, j;
int** land=malloc(y_size*sizeof(int*));
for(i=0; i<y_size; ++i) {
	land[i]=malloc(x_size*sizeof(int));
}
infile=fopen(argv[1], "r"); // Ouverture du fichier infile
	if( infile == NULL ) {
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

// Loading the landscape
int val;
for(i=0; i<y_size; i++) {
	for(j=0; j<x_size; j++) {
		fscanf(infile,"%d;",&val);
		land[i][j] = val;
	}
}
fclose(infile);



int w=x_size, h=y_size; // TODO: homogeneize
//~ printf("1==> Outputname will be: %s with %lu chars.\n", outputName, strlen(outputName)); // Gets truncated
char *img = NULL;
//~ printf("2==> Outputname will be: %s with %lu chars.\n", outputName, strlen(outputName));// Gets truncated
int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
//~ if( img ) free( img );
img = (char *)malloc(3*w*h);
//~ printf("3==> Outputname will be: %s with %d chars.\n", outputName, (int) (sizeof(outputName)/sizeof(char))); // Gets truncated
memset(img,0,sizeof(img));

int x, y;
for(int i=0; i<w; i++)
{
    for(int j=0; j<h; j++)
{
    x=i; y=(h-1)-j;
		if(land[i][j]== 0){
			r = 34;
			g = 139;  // Forestgreen
			b = 34;
		} else if(land[i][j]==1){
			r = 255;
			g = 255; // Yellow (desaturated)
			b = 25;
		} else {
			r = 150;
			g = 150; // Just a random gray
			b = 150;
	}
    if (r > 255) r=255;
    if (g > 255) g=255;
    if (b > 255) b=255;
    img[(x+y*w)*3+2] = (unsigned char)(r);
    img[(x+y*w)*3+1] = (unsigned char)(g);
    img[(x+y*w)*3+0] = (unsigned char)(b);
}
}
//~ printf("==> Outputname will be: %s with %d chars.\n", outputName, (int) (sizeof(outputName)/sizeof(char))); // Gets even more truncated

unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
unsigned char bmppad[3] = {0,0,0};

bmpfileheader[ 2] = (unsigned char)(filesize    );
bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
bmpfileheader[ 4] = (unsigned char)(filesize>>16);
bmpfileheader[ 5] = (unsigned char)(filesize>>24);

bmpinfoheader[ 4] = (unsigned char)(       w    );
bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
bmpinfoheader[ 6] = (unsigned char)(       w>>16);
bmpinfoheader[ 7] = (unsigned char)(       w>>24);
bmpinfoheader[ 8] = (unsigned char)(       h    );
bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
bmpinfoheader[10] = (unsigned char)(       h>>16);
bmpinfoheader[11] = (unsigned char)(       h>>24);

//~ strcpy(outputName, dirName);
outputName = strcat(outputName, *fileName);
outputName = strcat(outputName, ".bmp");
outputName[strlen(outputName)+1] = '\0';
//~ printf("Outputname will be: %s\n", outputName);
//~ printf("Outputname length be: %lu\n", strlen(outputName));

printf("Ready to save %s\n", outputName);
outFile = fopen(outputName,"wb");
fwrite(bmpfileheader,1,14,outFile);
fwrite(bmpinfoheader,1,40,outFile);
for(i=0; i<h; i++)
{
    fwrite(img+(w*(h-i-1)*3),3,w,outFile);
    fwrite(bmppad,1,(4-(w*3)%4)%4,outFile);
}
fclose(outFile);




//fclose(outFile);
printf("\tSucces: statistics have been saved in: \n\t\t%s\n", outputName);

	for(i=0; i<x_size; ++i) {
		free(land[i]);
	}



free(land);


	return 0;
}  // fin du main()



void split_path_file(char** p, char** f, char *pf) {
    char *slash = pf, *next;
    while ((next = strpbrk(slash + 1, "\\/"))) slash = next;
    if (pf != slash) slash++;
    *p = strndup(pf, slash - pf);
    *f = strdup(slash);
}

char *remove_ext (char* mystr, char dot, char sep)
{
	char *retstr, *lastdot, *lastsep;

	// Error checks and allocate string.

	if (mystr == NULL)
		return NULL;
	if ((retstr = malloc ( (strlen(mystr) + 1)*sizeof(char) )) == NULL)
		return NULL;

	// Make a copy and find the relevant characters.

	strcpy (retstr, mystr);
	lastdot = strrchr (retstr, dot);
	lastsep = (sep == 0) ? NULL : strrchr (retstr, sep);

	// If it has an extension separator.

	if (lastdot != NULL) {
		// and it's before the extenstion separator.

		if (lastsep != NULL) {
			if (lastsep < lastdot) {
				// then remove it.

				*lastdot = '\0';
			}
		} else {
			// Has extension separator with no path separator.

			*lastdot = '\0';
		}
	}

	// Return the modified string.

	return retstr;
}



