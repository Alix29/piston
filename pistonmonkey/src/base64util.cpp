// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// local includes
#include "base64util.h"

static const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char index_64[256] =
{
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,62, XX,XX,XX,63,
    52,53,54,55, 56,57,58,59, 60,61,XX,XX, XX,XX,XX,XX,
    XX, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,XX, XX,XX,XX,XX,
    XX,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
};

#define CHAR64(c)  (index_64[(unsigned char)(c)])


///////////////////////////////////////////////////////////////////////////
// base64_encode
char *		base64_encode(unsigned char * data, unsigned long datalen)
{
	char *		buf=0;
	int			c1=0, c2=0, c3=0, buflen=0, len=0, maxbuf=0;
	char *		binStr=0;

    if(!data || !datalen)
        return 0;

	binStr = (char *)data;
	len = datalen;

	maxbuf = len*2 + 20;
    buf = (char *)malloc(maxbuf);
	if(!buf)
		return 0;

	while(len)
	{
		c1 = (unsigned char)*binStr++;
		buf[buflen++] = basis_64[c1>>2];
		if(--len==0)
			c2 = 0;
		else
			c2 = (unsigned char)*binStr++;

		buf[buflen++] = basis_64[((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4)];
		if(len==0)
		{
			buf[buflen++] = '=';
			buf[buflen++] = '=';
			break;
		}

		if(--len==0)
			c3 = 0;
		else
			c3 = (unsigned char)*binStr++;

		buf[buflen++] = basis_64[((c2 & 0xF) << 2) | ((c3 & 0xC0) >>6)];
		if(len==0)
		{
			buf[buflen++] = '=';
			break;
		}

		--len;

		buf[buflen++] = basis_64[c3 & 0x3F];
    }

    buf[buflen]=0;

	return buf;
}


///////////////////////////////////////////////////////////////////////////
// base64_decode
unsigned char *		base64_decode(char * data, unsigned long * resultlen)
{
	char *			input=0;
    unsigned char *	output=0;
	unsigned char *	tempBuffer=0;
    int				c1=0, c2=0, c3=0, c4=0, len=0;
	unsigned char * newData=0;

	if(!data || !*data || !resultlen)
		return 0;

	tempBuffer = (unsigned char *)strdup(data);

	input = (char *)data;
	output = (unsigned char *)tempBuffer;

    while(*input)
	{
		c1 = *input++;
		if(CHAR64(c1)==XX)
			return 0;

		c2 = *input++;
		if(CHAR64(c2)==XX)
			return 0;

		c3 = *input++;
		if(c3!='=' && CHAR64(c3)==XX)
			return 0;

		c4 = *input++;
		if(c4!='=' && CHAR64(c4)==XX)
			return 0;

		*output++ = (unsigned char)((CHAR64(c1) << 2) | (CHAR64(c2) >> 4));
		++len;

		if(c3=='=')
			break;

		*output++ = (unsigned char)(((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2));
		++len;

		if(c4=='=')
			break;

		*output++ = (unsigned char)(((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4));
		++len;
    }

	newData = (unsigned char *)malloc(len);
	memcpy(newData, tempBuffer, len);

	*resultlen = len;

	free(tempBuffer);

	return newData;
}

