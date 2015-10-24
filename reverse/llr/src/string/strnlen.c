#include <common.h>
#include <string.h>

size_t __strnlen(const char *str, size_t maxlen)
{
	size_t actual_len = strlen(str);
	if (actual_len >= maxlen)
		return maxlen;
	return actual_len;
}
