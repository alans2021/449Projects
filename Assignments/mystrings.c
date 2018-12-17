//Look for strings of printable characters 
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Must enter an input file");
		exit( EXIT_FAILURE );
	}
	
	FILE * infile = fopen( argv[1], "rt" );
	if(!infile)
	{
		fprintf(stderr, "Can't open %s for read.\n", argv[1]);
		exit( EXIT_FAILURE );
	}
	
	fseek(infile, 0, SEEK_END); // seek to end of file
	int size = ftell(infile); // get current file pointer
	fseek(infile, 0, SEEK_SET); // seek back to beginning of file
	char *buffer = calloc(size, 1);

	fread(buffer, 1, size, infile);
	char * print = (char *) calloc(4, 1);
	int printInd = 0;
	int printSize = 0;
	int i;
	for(i = 0; i < size; i++){
		//printf("%d %c\n", buffer[i], buffer[i]);
		//printf("Sizeof print: %d printSize: %d\n", sizeof(print), printSize);
		if(buffer[i] >= 32 && buffer[i] <= 126){
			if(printSize >= sizeof(print)){
				print = (char *) realloc(print, printSize * 2);
			}
			print[printInd] = buffer[i];
			printSize++;
			printInd++;
		}
		else{
			if(printSize >= 4)
				printf("%s\n", print);
			printInd = 0;
			printSize = 0;
			free(print);
			print = (char *)calloc(4, 1);
		}
	}
	free(print);
	free(buffer);
	fclose(infile);
	return(0);
	
//udUmWGuoaEwkVFpNvktEcjILPgFZU



}

