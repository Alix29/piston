#ifndef PISTONMONKEY_BASE64UTIL_H_
#define PISTONMONKEY_BASE64UTIL_H_

#define XX			127

char *		        base64_encode(unsigned char * data, unsigned long datalen);
unsigned char *		base64_decode(char * data, unsigned long * resultlen);

#endif /* PISTONMONKEY_BASE64UTIL_H_ */
