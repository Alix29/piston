#ifndef PISTONMONKEY_MEMUTIL_H_
#define PISTONMONKEY_MEMUTIL_H_

struct mem
{
	unsigned char * data;
	unsigned long 	length;
};

void * 				memfree(void * ptr);
unsigned char * 	memappend(unsigned char * start, unsigned long startLength, unsigned char * appendage, unsigned long appendageLength);

#endif /* PISTONMONKEY_MEMUTIL_H_ */
