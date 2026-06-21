#include <stdio.h>
#include <stdlib.h>

#include "myStringLib.h"

int my_strlen(const char * const str1)
{
	int counter = 0; 
	int i = 0;
	
	for(i = 0; str1[i] != '\0'; i++)
		counter++;

	return counter;
}

int my_strcmp(const char * const str1, const char * const str2)
{
	int i;
	int j;
	int counter1 = 0;
	int counter2 = 0;

	// Criteria 1: Check their lengths.  If not the same length, not equal
	for(i = 0; str1[i] != '\0'; i++)
	{
		counter1++;
	}

	for(i = 0; str2[i] != '\0'; i++)
	{
		counter2++;
	}

	// Criteria 2: Check contents char-by-char
	if(counter1 == counter2) 
	{
		for(i = 0; str1[i] != '\0'; i++) // Check 1 - For error
		{
			if(str1[i] != str2[i])
			{
				return 0; // Returning a value exits the ENTIRE function
			}
		}

		return 1;
	}

	else
	{
		return 0;
	}
}


int my_strcmpOrder(const char * const str1, const char * const str2)
{
	int i;
	
	for(i = 0; ((str1[i] != '\0') || str2[i] != '\0'); i++)
	{
		if(str1[i] > str2[i])
			return 1;
		

		else if(str1[i] < str2[i])
			return 0;
	}

	return -1; 

}


char *my_strcat(const char * const str1, const char * const str2){

	char *z = NULL;
	int i, j;
	int counter = 0;
	
	// Measuring length of both strings, terminating AT NULL
	for(i = 0; str1[i] != '\0'; i++) 
		counter++;

	for(j = 0; str2[j] != '\0'; j++) 
		counter++;
		
	// Allocating space in memory with found length, adding back two NULL spaces
	z = malloc((counter+1) * (sizeof(char)));

	// Combine both strings into array
	for(i = 0; str1[i] != '\0'; i++) // Loop terminates AT NULL
	{
		z[i] = str1[i];
	}

	for(j = 0; str2[j] != '\0'; j++)
	{
		z[i+j] = str2[j];
	}

	// Add back NULL spaces
	z[counter] = '\0';

	return z;
}


