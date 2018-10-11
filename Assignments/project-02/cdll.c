#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdll.h"

/* just a convenience */
void fatal( char * msg )
{
	printf("%s\n",msg);
	exit(EXIT_FAILURE);
}


/* ----------------------------------------------------------------------------
	initList:

*/
void initList(CDLL *list, int (*compare)(void*, void*), void (*print)(void*, int),
			  void (*freeData)(void *))
{
	list->head = NULL; //Initalizing CDLL components
	list->compare = compare;
	list->print = print;
	list->freeData = freeData;
}

/* ----------------------------------------------------------------------------
*/
void insertAtTail(CDLL *list, void *data)
{
	CDLL_NODE *newNode = malloc(sizeof(CDLL_NODE)); //Malloc new CDLL_NODE
	newNode->data = data;
	if(list->head == NULL){ //If no nodes in list
		newNode->prev = newNode; //Have head reference itself
		newNode->next = newNode;
		list->head = newNode;
	}
	else{
		CDLL_NODE *previous = list->head->prev; //Otherwise, modify next and previous to account for added nodes
		newNode->prev = previous;
		newNode->next = list->head;
		previous->next = newNode;
		list->head->prev = newNode;
	}
}



/* ----------------------------------------------------------------------------
	deleteNode:
*/
CDLL_NODE * deleteNode(CDLL *list, CDLL_NODE *deadNode, int direction )
{
	if(deadNode->data == NULL)
		return NULL;
	
	CDLL_NODE *prevNode = deadNode->prev;
	CDLL_NODE *nextNode = deadNode->next;
	if(deadNode == prevNode && deadNode == nextNode){ //If one node remaining, just remove the deadNode
		list->freeData(deadNode->data);
		free(deadNode);
		return NULL;
	}
	
	if(deadNode == list->head) //Set head node
		list->head = nextNode;
	prevNode->next = nextNode; //Reset next and previous references
	nextNode->prev = prevNode;
	
	list->freeData(deadNode->data);
	free(deadNode);
	if(direction == CLOCKWISE)
		return nextNode;
	else
		return prevNode;
}



/* ----------------------------------------------------------------------------
	printList:

*/

void printList( CDLL list, int direction, int mode )
{
	CDLL_NODE * curr = list.head; //Print head first
	list.print(curr->data, mode);
	if(direction == CLOCKWISE)
		curr = curr->next;
	else
		curr = curr->prev;
	while(curr != list.head){
		list.print(curr->data, mode);
		if(direction == CLOCKWISE)
			curr = curr->next;
		else
			curr = curr->prev;
	}
		
	/* Y O U R   C O D E    H E R E */
}



/* ----------------------------------------------------------------------------
	searchList:

	Scan list until you find a node that contains the data value passed in.
	If found return that pointer - otherwise return NULL
*/
CDLL_NODE * searchList( CDLL list, void * target )
{
	CDLL_NODE *curr = list.head;
	while(list.compare(curr->data, target) != 0){
		curr = curr->next;
		if(curr == list.head)
			return NULL;
	}
	return curr;
	/* Y O U R   C O D E    H E R E */
}
