#include "mymalloc.h"

void * mymalloc(int size)
{
	if (head == NULL){ //No nodes exist yet
		M_NODE *newNode = sbrk(0); //Reference to first node of linked list
		head = newNode; //Head and tail both reference here
		tail = newNode;
		sbrk(sizeof(M_NODE)); //Allocate space for data, next, and prev
		newNode->next = NULL;
		newNode->prev = NULL;
		int data = size + sizeof(M_NODE); //data is the size argument plus sizeof(Node)
		data = data << 1; //Multiply by 2
		data = data | 1; //Add 1, indicates data is being used
		newNode->data = data;
		int * blockstart = sbrk(0); //Pointer that returns location of where allocated data starts
		sbrk(size); //Adjust sbrk
		return blockstart;
	}
	else{
		int maxsize = size + sizeof(M_NODE); //Maxsize at first is the size argument plus node overhead
		int * blockstart = NULL;
		M_NODE *curr = head;
		M_NODE *toAlloc = curr;
		while(curr != NULL){ //For mallocing in existing nodes
			int flag = curr->data;	
			if (flag % 2 == 0){ //Indicates free space avaiable
				flag = flag >> 1;
				if(flag >= maxsize){ //If data in flag greater than maxsize, save that location in blockstart
					toAlloc = curr; //toAlloc is curr, but may change if there's larger space anywhere else
					maxsize = flag; //Adjust maxsize
					blockstart = (int *)(curr + 1);
				}
			}
			curr = curr->next;
		}
		
		if(blockstart == NULL){ //Means need new node
			int dat = size + sizeof(M_NODE); //Similar to creating the first node
			dat = dat << 1;
			dat = dat | 1;
			M_NODE * newNode = sbrk(0);
			sbrk(sizeof(M_NODE));
			newNode->data = dat; //Adjust tail, also make sure next and prev go to right references
			tail->next = newNode;
			newNode->prev = tail;
			newNode->next = NULL;
			tail = newNode;
			blockstart = sbrk(0);		
			sbrk(size);
		}
		else{ //Means just set flag to 1
			int dat = toAlloc->data;
			dat += 1;
			toAlloc->data = dat;
		}
		return blockstart;

	}
	

}

void myfree(void *ptr)
{
	M_NODE * overhead = (M_NODE *) (ptr - sizeof(M_NODE)); //Reference to node before this pointer, storing basic information
	M_NODE *previous = overhead->prev; 
	M_NODE *nextN = overhead->next;
	int data = overhead->data;
	data = data & 0xFFFFFFFE; //Set flag to 0, now available for use
	int size = data / 2;
	overhead->data = data;
	if(previous != NULL){
		int prevdata = previous->data;
		if(prevdata % 2 == 0){ //If previous block also free, coalesce
			int prevsize = prevdata / 2; //Add sizes together
			prevsize = prevsize + size;
			prevsize = prevsize * 2;
			previous->data = prevsize; //Adjust prev and next references
			previous->next = nextN;
			if(nextN != NULL)
				nextN->prev = previous;
 			overhead = previous;
		}
		if(nextN != NULL){ //If next block also free, coalesce
			int nextdata = nextN->data;
			if(nextdata % 2 == 0){
				nextdata = (overhead->data) + nextdata;
				overhead->data = nextdata; //Add sizes together
				overhead->next = nextN->next; //Adjust prev and next references
				if(nextN->next != NULL)
					nextN->next->prev = overhead;	
			}
		}
	}
	else if (nextN != NULL){
		if((nextN->data) % 2 == 0){ //Same as previous code block, of coalescing next
			int nextsize = (nextN->data) >> 1;
			size += nextsize;
			overhead->data = (size << 1);
			overhead->next = nextN->next;
			if(nextN->next != NULL)
				nextN->next->prev = overhead;
		}
	}
	
	if(overhead->next == NULL){ //If freeing block that touches sbrk, adjust sbrk by incrementing down
		int offset = overhead->data;
		tail = overhead->prev;
		if(tail != NULL) 
			tail->next = NULL;
		else
			head = NULL;
		offset = offset >> 1;
		offset *= -1;
		sbrk(offset);
	}
	
// implementation
}
