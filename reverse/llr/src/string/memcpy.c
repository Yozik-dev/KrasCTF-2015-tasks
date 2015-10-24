#include <common.h>

void *memcpy(void *dstpp, const void *srcpp, size_t len)
{
	size_t curlen = 0;
	unsigned char *dst = (unsigned char *) dstpp;
	unsigned char *src = (unsigned char *) srcpp;

	while (curlen < len) {
		*dst++ = *src++;
		++curlen;
	}
	return dstpp;
}
