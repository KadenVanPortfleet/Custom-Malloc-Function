// MallocTest3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define MAX_CAP 1024

const int KERNEL_HEAP_SIZE = 0x8000;

unsigned int KERNEL_HEAP_START;
unsigned int KERNEL_HEAP_END = KERNEL_HEAP_START + KERNEL_HEAP_SIZE;

const unsigned int* hstart = (unsigned int*)&KERNEL_HEAP_START,
* hend = (unsigned int*)&KERNEL_HEAP_END;

bool* freeChecker;

int checkChar = 2;
int checkCharEnd = 3;

void heapClear()
{
	unsigned char* p = (unsigned char*)hstart;
	for (int i = 0; i < 1000; i++)
	{
		p[i] = 0; //CLEAR HEAP!
	}
}
void* malloc2(size_t size)
{

	char* result = (char*)hstart;
	result[0] = checkCharEnd;
	int iterator = 0;
	int freeCount = 0;
	//printf("\nINDEX 17: %d", result[33]); //Debug line for weird anomolay. The heap initilizes with "-128" value byte at index 17... 
	while (true)
	{
		while (true)
		{
			if (result[iterator] == checkCharEnd && result[iterator + 1] == checkChar)
			{
				iterator += 2;
				continue;
			}
			else if (result[iterator] == checkCharEnd && result[iterator + 1] != checkChar)
			{
				break;
			}
			else
			{
				iterator++;
			}
		}
		printf("\nIndex: %d", iterator);

		for (int i = 2; i <= size + 2; i++) //We need 2 extra byte spaces to mark the beginning and end of a section. freeCount is the string size including the null PLUS the 1 terminator.
		{									//freeCount = size + 1
			if (result[iterator + i] != checkChar)
			{
				freeCount++;
				printf("\n%d", iterator + i);
			}
		}
		printf("\nFree: %d", freeCount);
		if (freeCount == size + 1)
		{
			printf("\nFree space was found at index: %d", iterator);
			result[iterator + 1] = checkChar; //Mark begining of section.
			//result[iterator + 2] = char('a'); //Debug write test to make sure pointers are correct.
			//result[iterator + 3] = 0; //Debug write test to make sure pointers are correct -> Terminating 0.
			result[iterator + freeCount + 1] = checkCharEnd; //Mark end of section.
			result = result + iterator + 2; //Change pointer to start at beggining of section.
			return result;
		}
		else
		{
			freeCount = 0;
			iterator++;
			continue;
		}
	}
	result = NULL;
	return result;
}


void* memset2(void* str, int c, size_t n)
{
	unsigned char* p = (unsigned char*)str;

	for (int i = 0; i < n; i++)
	{
		
		if (p[i] != checkCharEnd)
		{
			p[i] = c;
		}
		else
		{
			printf("\nMEMSET ERROR : Memory Was Set for Greater Than Allocated Space! Only allocated space cleared...");
			return NULL;
		}
	}
	return NULL;
	
	
}

char* strncpy2(char* dest, const char* src, size_t n)
{
	int i = 0;
	while (n > 0)
	{
		if (dest[i+1] != checkCharEnd) //This caps the amount you can put into a certain heap spot. This allows for a zero to terminate. Any value greater than what was allocated will get cropped to fit.
		{
			dest[i] = src[i];
			n--;
			i++;
		}
		else
		{
			printf("\nCOPY ERROR : Memory Space too Small! Cropping input to fit...");
			return NULL;
		}
	}
	return dest;
}

void free2(void* ptr)
{
	unsigned char* p = (unsigned char*)ptr;
	printf("\nFREEING MEMORY AT INDEX %d", p - (unsigned char*)hstart);
	p[-1] = 0;
	int i = 0;
	while (p[i] != checkCharEnd)
	{
		p[i] = 0;
		i++;
	}
	printf("\nFinal deleteion: %d", p[i]);
	p[i] = 0;
}

long os_heap_avail()
{
	int counting = 0;
	long count = 1; //Start at one because heap has initial byte.
	char* p = (char*)hstart;
	for (int i = 1; i < 1000; i++) // Start at index [1] to skip control byte at beginning of heap.
	{
		if (counting == 1)
		{
			count++;
		}
		if (p[i] == 2)
		{
			count++; //Add one for beginning control byte. Ending control byte is added by third if below.
			counting = 1;
		}
		else if (p[i] == 3)
		{
			counting = 0;
		}
		
	}
	return count;
}

int main()
{
	printf("---Starting!---");

	heapClear();
	char* ptr = (char*)malloc2(10);
	if (ptr != NULL)
	{
		memset2(ptr, '\0', 10); // initialize the array to '\0'
		char name[10] = "DUOS V1.1"; // create a c-string
		strncpy2(ptr, name, strlen(name)); // copy the c-string to the dynamic array
		printf("\n%s", ptr);
		//free2(ptr); // release the memory
	}

	char* ptr2 = (char*)malloc2(10);
	if (ptr2 != NULL)
	{
		memset2(ptr2, '\0', 10); // initialize the array to '\0'
		char name[12] = "DUOS V1.2"; // create a c-string
		strncpy2(ptr2, name, strlen(name)); // copy the c-string to the dynamic array
		printf("\n%s", ptr2);
		//free2(ptr2); // release the memory
	}

	char* ptr3 = (char*)malloc2(10);
	if (ptr3 != NULL)
	{
		memset2(ptr3, '\0', 10); // initialize the array to '\0'
		char name[10] = "DUOS V1.3"; // create a c-string
		strncpy2(ptr3, name, strlen(name)); // copy the c-string to the dynamic array
		printf("\n%s", ptr3);
		//free2(ptr3); // release the memory
	}

	free2(ptr2);

	char* ptr4 = (char*)malloc2(5);
	if (ptr4 != NULL)
	{
		memset2(ptr4, '\0', 10); // initialize the array to '\0'
		char name[10] = "DUOS V1.4"; // create a c-string
		strncpy2(ptr4, name, strlen(name)); // copy the c-string to the dynamic array
		printf("\n%s", ptr4);
		//free2(ptr3); // release the memory
	}

	char* ptr5 = (char*)malloc2(4);
	if (ptr5 != NULL)
	{
		memset2(ptr5, '\0', 10); // initialize the array to '\0'
		char name[10] = "DUOS V1.4"; // create a c-string
		strncpy2(ptr5, name, strlen(name)); // copy the c-string to the dynamic array
		printf("\n%s", ptr5);
		//free2(ptr3); // release the memory
	}


	printf("\n");						//MASTER DEBUG! Shows first 100 byte values in int for the heap.
	char* final = (char*)hstart;
	for (int i = 0; i < 1000; i++)
	{
		printf(" %d", final[i]);
	}

	printf("\n%d", os_heap_avail());
	

	printf("\n---Done!---");
}

