// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local includes
#include "memutil.h"


//////////////////////////////////////////////////////////////////////////
// memfree
void * memfree(void * ptr)
{
	if(ptr)
		free(ptr);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// memappend
unsigned char * memappend(unsigned char * start, unsigned long startLength, unsigned char * appendage, unsigned long appendageLength)
{
    if(!start && !appendage)
        return start;

    if(!startLength && !appendageLength)
        return start;

    if(!appendage || !appendageLength)
        return start;

    if(!start)
    {
        start = (unsigned char *)malloc(appendageLength);
        memcpy(start, appendage, appendageLength);
    }
    else
    {
        start = (unsigned char *)realloc(start, startLength+appendageLength);
        memcpy(start+startLength, appendage, appendageLength);
    }

    return start;
}
