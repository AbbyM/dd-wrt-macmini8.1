#if __ARM_ARCH < 6
#include <string.h>

char *strcpy(char *restrict dest, const char *restrict src)
{
	__stpcpy(dest, src);
	return dest;
}
#endif
