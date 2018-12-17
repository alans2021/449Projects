// This head file just do the definitation, all implementation will be in corresponding c file. 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
//define the malloc and my free
//
typedef struct M_NODE
{
	struct M_NODE *next;
	struct M_NODE *prev;
	int data;
} M_NODE;

M_NODE *head;
M_NODE *tail;
	
void * mymalloc(int size);
void myfree(void *ptr);

// define other function you may need for the linked list
