//You only test case, should be similar to mallocdrv.c
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//include your code
#include "mymalloc.h"
//
////replace malloc here with the appropriate version of mymalloc
#define MALLOC mymalloc
////replace free here with the appropriate version of myfree
#define FREE myfree
//
typedef struct PERSON{
	int age;
	int height; //In inches
	int room;
} PERSON;

int main()
{
	int *test = (int *) malloc(sizeof(int));
	*test = 9;
	int *arr;
	arr = (int *)malloc(5 * sizeof(int));
	for(int i = 0; i < 5; i++)
		arr[i] = i * 2;
	PERSON *alan = malloc(sizeof(PERSON));
	alan->age = 19;
	alan->height = 67;
	alan->room = 903;
	PERSON *erica = malloc(sizeof(PERSON));
	erica->age = 3;
	erica->height = 30;
	erica->room = 0;
	printf("USING REGULAR MALLOC AND FREE: \n");
	printf("Value of test : %d \n", *test);
	for(int i = 0; i < 5; i++)
		printf("Value of arr[%d]: %d \n", i, arr[i]);
	printf("Value of person Alan: age = %d, height = %d, room = %d \n", alan->age, alan->height, alan->room);
	printf("Value of person Erica: age = %d, height = %d, room = %d \n", erica->age, erica->height, erica->room);
	free(alan);
	int *test2 = (int *) malloc(sizeof(int));
	*test2 = 3;
	printf("Value of test2 : %d \n", *test2);
	free(erica);
	PERSON *random = malloc(sizeof(PERSON));
	random->age = 4;
	random->height = 5;
	random->room = 7;
	printf("Value of person random: age = %d, height = %d, room = %d \n", random->age, random->height, random->room);
	free(test);
	free(random);
	free(arr);
	free(test2);
	printf("\n");

	printf("USING WORST-FIT MYMALLOC AND MYFREE: \n");
	printf("Initial value of SBRK = %p\n", sbrk(0));
	int *TEST = (int *) MALLOC(sizeof(int));
	*TEST = 9;
	int *ARR;
	ARR = (int *)MALLOC(5 * sizeof(int));
	for(int i = 0; i < 5; i++)
		ARR[i] = i * 2;
	PERSON *ALAN = MALLOC(sizeof(PERSON));
	ALAN->age = 19;
	ALAN->height = 67;
	ALAN->room = 903;
	PERSON *ERICA = MALLOC(sizeof(PERSON));
	ERICA->age = 3;
	ERICA->height = 30;
	ERICA->room = 0;
	printf("Value of test : %d \n", *TEST);
	for(int i = 0; i < 5; i++)
		printf("Value of arr[%d]: %d \n", i, ARR[i]);
	printf("Value of person Alan: age = %d, height = %d, room = %d \n", ALAN->age, ALAN->height, ALAN->room);
	printf("Value of person Erica: age = %d, height = %d, room = %d \n", ERICA->age, ERICA->height, ERICA->room);
	printf("ALAN pointer = %p\n", ALAN);
	FREE(ALAN);
	int *TEST2 = (int *) MALLOC(sizeof(int));
	*TEST2 = 3;
	printf("TEST2 pointer = %p\n", TEST2);
	printf("Value of test2 : %d\n", *TEST2);
	printf("ERICA pointer = %p\n", ERICA);
	FREE(ERICA);
	PERSON *RANDOM = MALLOC(sizeof(PERSON));
	RANDOM->age = 4;
	RANDOM->height = 5;
	RANDOM->room = 7;
	printf("RANDOM pointer = %p\n", RANDOM);
	printf("Value of person random: age = %d, height = %d, room = %d \n", RANDOM->age, RANDOM->height, RANDOM->room);
	FREE(TEST);
	FREE(RANDOM);
	FREE(ARR);
	FREE(TEST2);
	printf("Final value of SBRK = %p\n", sbrk(0));
	return 0;
}
