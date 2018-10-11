#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ------------------------------------------------------
// HELPFUL DEBUGGING UTILITIES - NOT PART OF THE SOLUTION

void bprintInt( int n)
{
	int i;
	for (i = 31 ; i >=0  ; --i )
 	{
       if ( (n >> i) & 1)
          printf("1" );
       else
          printf("0" );
    }
}

void bprintByte( char n)
{
	int i;
	for (i = 7 ; i >=0  ; --i )
 	{
       if ( (n >> i) & 1)
          printf("1" );
       else
          printf("0" );
    }
}
void bprintShort( short n)
{
	int i;
	for (i = 15 ; i >=0  ; --i )
 	{
       if ( (n >> i) & 1)
          printf("1" );
       else
          printf("0" );
    }
}
// ------------------------------------------------------

int main( int argc, char *argv[] )
{
	FILE* txtFile, *binFile;
	int shortsReadIn=0,  _12bitValsWritten=0, _12bitValsReadBackIn;
	short s1,s2; /* the pair read each time */

	if (argc<3)
	{
		printf("usage: %s  <infilename> <outfilename> (i.e. ./a.out sine.txt sxine.bin)\n", argv[0] );
		return EXIT_FAILURE;
	}

	txtFile = fopen( argv[1], "r" );
	if (txtFile == NULL )
	{
		printf("Can't open %s for input. Program halting\n",argv[1] );
		return EXIT_FAILURE;
	}

	binFile = fopen( argv[2], "w" );
	if (binFile == NULL )
	{
		printf("Can't open %s for output. Program halting\n",argv[2] );
		return EXIT_FAILURE;
	}



	while((fscanf(txtFile, "%hi", &s1 ) == 1))  // giving you the read loop so you dont use incorrect idiom of  while !EOF
	{
		
		if(fscanf(txtFile, "%hi", &s2) == 1){		
			s1 = s1 << 4; // Shift left by 4 bits
			short inter = s2 & 0x0F00; 
			inter = inter >> 8;
			s1 = s1 | inter; //Take 4 MSB of s2 and put in lower 4 bits of s1
			fwrite(&s1, sizeof(s1), 1, binFile); 
			s2 = s2 & 0x00FF;
			char c = s2; //Write lower 8 bits of s2
			fwrite(&c, sizeof(c), 1, binFile);
			_12bitValsWritten += 2;
		}
		else{
			fwrite(&s1, sizeof(s1), 1, binFile); //Just need to write one short
			_12bitValsWritten++;
		}
			

	}

	printf("%d integers written to %s (3 bytes per 2 integers written)\nNow READING/ECHOING values from binary outfile\n",_12bitValsWritten, argv[2] );	fclose( txtFile );
	fclose( binFile );

/* READ BACK AND ECHO THE VALUES WRITTEN OUT */

	binFile = fopen( argv[2], "rb" );
	if (binFile == NULL )
	{
		printf("Can't open %s for input. Program halting\n",argv[2] );
		return EXIT_FAILURE;
	}

	while ( fread(&s1, sizeof(s1), 1, binFile) == 1 ) // giving you the correct form of the read so you don't use  bad-> while !EOF
	{
		char c;
		short inter1;
		short final1;
		short inter2;
		short final2;
		if(fread(&c, sizeof(c), 1, binFile) == 1){
			if(s1 < 0){ //If negative
				inter1 = s1 >> 4;
				final1 = inter1 | 0xF000; //Make sure upper 4 bits are all ones
			}
			else
				final1 = s1 >> 4; //Otherwise, just shift right by 4
			printf("%hi\n", final1);
			
			inter2 = s1 & 0x000F; //Take lower 4 bits, which represent 4 MSB of second integer
			if(inter2 >= 8){ //Just means MSB of second integer is 1, which is negative number
				final2 = 0xFFF0; 
				final2 = final2 | inter2; //Initialize with all one's for first 12 bits, then inter2
			}
			else
				final2 = inter2; //Otherwise, just set equal to inter2
			final2 = final2 << 8; //Shift left by 8 bits
			final2 = final2 | (c & 0x00FF); //Take the last 8 bits stored in char
			printf("%hi\n", final2);			
		}
		else{
			printf("%hi\n", s1); //Just print final remaining integer
		}
	}
	fclose( binFile );

	return EXIT_SUCCESS;
}

