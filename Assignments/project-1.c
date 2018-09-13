/*
  project-1.c

  MY NAME:
  MY PITT EMAIL:

  As you develop and test this file:

  use this command to compile: (you can name the executable whatever you like)
  gcc -W -Wall -Wextra -O2   project-1.c   -o project-1.exe

  use this command to execute:  (you will of course test on both input files)

  ./project-1.exe  sine-1.bin 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[])
{
  FILE * inFile= NULL;
  int isFull = 0; // Tracks if just passed end of full wave
  int count = 0; // Tracks total number of samples read 
  int max = 0; // Tracks max value of full wave
  int min = 0; // Tracks min value of full wave
  int samples = 0; // Represents number of values for a full wave
  short int prev; // Represents previous value read
  short int current; // Represents current value being read from file 

  if (argc < 2)
    {
      fprintf(stderr,"\nMust specify a binary file on the command line. Please try again.\n");
      exit(EXIT_FAILURE);
    }
  inFile = fopen(argv[1], "rb");
  if  (inFile == NULL)
    {
      fprintf(stderr,"Can't open %s for input\n", argv[1] );
      exit(EXIT_FAILURE);
    }

  while (feof(inFile) == 0){
    // Read in value from file 
    fread(&current, sizeof(current), 1, inFile);
    if (count >= 1){
      // Indicates that a new full wave is starting   	  
      if (current > 0 && prev <= 0){

        // Print information if it was end of full wave
        if (isFull == 1)
	        printf("%d\t%hi\t%d\t%d\t%d\n", count, prev, samples, max, min);
        else // set isFull to 1 since all future starts of a wave will be the end of another wave
		isFull = 1;

        //Following lines reset the value for sample
	if (prev == 0) // Include that value as part of the wave sample if prev = 0
          samples = 1;
        else 
          samples = 0; // If not, don't include that value
        
	// Reset max and min
	max = 0;
        min = 0;  
      }
    }
    
    // Set max and min values if conditions met	
    if (current > max)
      max = current;
    if (current < min)
      min = current;
    prev = current; // Have prev variable set to current value
    samples++; // Increment
    count++; // Increment
  } 

  fclose(inFile); /* after the read loop is done. close file */

  return EXIT_SUCCESS;  // this is just a zero value. In C a zero return often indicates no error
}
