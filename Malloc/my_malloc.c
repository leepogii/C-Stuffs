#include "my_malloc.h"

/* You *MUST* use this macro when calling my_sbrk to allocate the
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you may receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif


/* our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */

metadata_t* freelist;

void* my_malloc_size_order(size_t size)
{
	size_t total = size + sizeof(metadata_t);
	ERRNO = NO_ERROR;
	if (total > SBRK_SIZE)
	{
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}
	if (freelist == NULL)
	{
		freelist = (metadata_t*) my_sbrk(SBRK_SIZE);
		if (freelist == NULL)
		{
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		freelist->size = SBRK_SIZE;
		freelist->prev = NULL;
		freelist->next = NULL;
		freelist->in_use = 0;
	}
	if (findSpace(total) == NULL)
	{
		metadata_t *newList = (metadata_t*) my_sbrk(SBRK_SIZE);
		if (newList == NULL)
		{
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		newList->size = SBRK_SIZE;
		newList->in_use = 0;
		addListSize(newList);
	} 
	metadata_t *temp = findSpace(total);
	if (temp->size < (total + sizeof(metadata_t) + 1))
	{
		removeList(temp);
		temp->in_use = 1;
		return (void*)((char*)temp + sizeof(metadata_t));
	}
	char* new_addr = (char*) temp;
	new_addr += total;
	metadata_t *new_add = (metadata_t*) new_addr;
	new_add->size = temp->size - total;
	new_add->in_use = 0;
	removeList(temp);
	addListSize(new_add);
	temp->size = total;
	temp->in_use = 1;
	return (void*)((char*)temp + sizeof(metadata_t));
}
void addListSize(metadata_t* toAdd)
{
	metadata_t *curr = freelist;
	if (freelist == NULL)
	{
		freelist = toAdd;
		toAdd->prev = NULL;
		toAdd->next = NULL;
		return;
	}
	while (curr != NULL)
	{
		if (curr->size > toAdd->size)
		{
			if (curr->prev == NULL)
			{
				toAdd->next = curr;
				curr->prev = toAdd;
				toAdd->prev = NULL;
				freelist = toAdd;
				return;
			}
			toAdd->next = curr;
			toAdd->prev = curr->prev;
			(curr->prev)->next = toAdd;
			curr->prev = toAdd;
			return;
		}
		if (curr->next == NULL)
		{
			toAdd->next = NULL;
			curr->next = toAdd;
			toAdd->prev = curr;
			return;
		}
		curr = curr->next;
	}
	return;	
}

void removeList(metadata_t* curr)
{
	if (freelist == NULL)
	{
		return;
	}
	if (curr->next != NULL && curr->prev != NULL)
	{
		(curr->next)->prev = curr->prev;
		(curr->prev)->next = curr->next;
		curr->prev = NULL;
		curr->next = NULL;
	}
	else if (curr->prev == NULL && curr->next == NULL)
	{	
		freelist = NULL;
	}
	else if (curr->prev == NULL)
	{
		freelist = curr->next;
		freelist->prev = NULL;
		curr->next = NULL;
	}
	else 	
	{
		(curr->prev)->next = NULL;
		curr->prev = NULL;
	}
	return;
}

metadata_t *findSpace(size_t size)
{
	metadata_t *curr = freelist;
	while (curr != NULL)
	{
		if (curr->size >= size)
		{
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}
void* my_malloc_addr_order(size_t size)
{
	size_t total = size + sizeof(metadata_t);
	ERRNO = NO_ERROR;
	if (total > SBRK_SIZE)
	{
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}
	if (freelist == NULL)
	{
		freelist = (metadata_t*) my_sbrk(SBRK_SIZE);
		if (freelist == NULL)
		{
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		freelist->size = SBRK_SIZE;
		freelist->prev = NULL;
		freelist->next = NULL;
		freelist->in_use = 0;
	}
	if (findSpace(total) == NULL)
	{
		metadata_t *newList = (metadata_t*) my_sbrk(SBRK_SIZE);
		if (newList == NULL)
		{
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		newList->size = SBRK_SIZE;
		newList->in_use = 0;
		addListAddr(newList);
	} 
	metadata_t *temp = findSpace(total);
	if (temp->size < (total + sizeof(metadata_t) + 1))
	{
		removeList(temp);
		temp->in_use = 1;
		return (void*)((char*)temp + sizeof(metadata_t));
	}
	char* new_addr = (char*) temp;
	new_addr += total;
	metadata_t *new_add = (metadata_t*) new_addr;
	new_add->size = temp->size - total;
	new_add->in_use = 0;
	removeList(temp);
	addListAddr(new_add);
	temp->size = total;
	temp->in_use = 1;
	return (void*)((char*)temp + sizeof(metadata_t));
}
void addListAddr(metadata_t* toAdd)
{
	metadata_t *curr = freelist;
	if (freelist == NULL)
	{
		freelist = toAdd;
		toAdd->prev = NULL;
		toAdd->next = NULL;
		return;
	}
	while (curr != NULL)
	{
		if (curr > toAdd)
		{
			if (curr->prev == NULL)
			{
				toAdd->next = curr;
				curr->prev = toAdd;
				toAdd->prev = NULL;
				freelist = toAdd;
				return;
			}
			toAdd->next = curr;
			toAdd->prev = curr->prev;
			(curr->prev)->next = toAdd;
			curr->prev = toAdd;
			return;
		}
		if (curr->next == NULL)
		{
			toAdd->next = NULL;
			curr->next = toAdd;
			toAdd->prev = curr;
			return;
		}
		curr = curr->next;
	}
	return;	
}

void my_free_size_order(void* ptr)
{
	if (ptr == NULL)
	{
		return;
	}
	metadata_t *temp = (void*)((char*)ptr - sizeof(metadata_t));
	temp->in_use = 0;
	addListSize(temp);
	merge_size(temp);
}

void my_free_addr_order(void* ptr)
{
	if (ptr == NULL)
	{
		return;
	}
	metadata_t *temp = (void*)((char*)ptr - sizeof(metadata_t));
	temp->in_use = 0;
	addListAddr(temp);
	merge_addr(temp);
}

void merge_size(metadata_t* ptr)
{
	metadata_t *curr = freelist;
	metadata_t *lp = NULL;
	metadata_t *rp = NULL;
	short l = 0;
	short r = 0;
	while (curr != NULL)
	{
		if (((void*)((char*)curr + curr->size)) == ptr)
		{
			l = 1;
			lp = curr;
		} else if (((void*)((char*)ptr + ptr->size)) == curr)
		{
			r = 1;
			rp = curr;
		}
		if (l + r == 2)
		{
			break;
		}
		curr = curr->next;
	}
	if (l + r == 0)
	{
		return;
	}
	if (l + r == 2)
	{
		removeList(rp);
		removeList(ptr);
		removeList(lp);
		lp->size += ptr->size + rp->size;
		addListSize(lp);
	} else if (l == 1)
	{
		removeList(ptr);
		removeList(lp);
		lp->size += ptr->size;
		addListSize(lp);
	} else
	{
		removeList(rp);
		removeList(ptr);
		ptr->size += rp->size;
		addListSize(ptr);
	}
}
void merge_addr(metadata_t* ptr)
{
	metadata_t *curr = freelist;
	metadata_t *lp = NULL;
	metadata_t *rp = NULL;
	short l = 0;
	short r = 0;
	while (curr != NULL)
	{
		if (((void*)((char*)curr + curr->size)) == ptr)
		{
			l = 1;
			lp = curr;
		} else if (((void*)((char*)ptr + ptr->size)) == curr)
		{
			r = 1;
			rp = curr;
		}
		if (l + r == 2)
		{
			break;
		}
		curr = curr->next;
	}
	if (l + r == 0)
	{
		return;
	}
	if (l + r == 2)
	{
		removeList(rp);
		removeList(ptr);
		removeList(lp);
		lp->size += ptr->size + rp->size;
		addListAddr(lp);
	} else if (l == 1)
	{
		removeList(ptr);
		removeList(lp);
		lp->size += ptr->size;
		addListAddr(lp);
	} else
	{
		removeList(rp);
		removeList(ptr);
		ptr->size += rp->size;
		addListAddr(ptr);
	}
}