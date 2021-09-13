#include "dreamcast_patch.h"

#ifdef __DREAMCAST__
int snprintf(char* s, size_t n, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	sprintf(s, format, args);
	va_end(args);
	return strlen(s);
}

int vsnprintf(char* s, size_t n, const char* format, va_list arg)
{
	sprintf(s, format, arg);
	return strlen(s);
}

int vasprintf(char** ret, const char* format, va_list ap)
{
	const int BUFFER_SIZE = 512;
	(*ret) = (char*)malloc(BUFFER_SIZE);
	sprintf(*ret, format, ap);
	return strlen(*ret);
}

char* _strdup(const char* str)
{
	char* ret = (char*)malloc(strlen(str) + 1);
	strcpy(ret, str);
	ret[strlen(str)] = '\0';
	return ret;
}

int strncasecmp(const char* a, const char* b, size_t count)
{
	int lenA = strlen(a);
	int lenB = strlen(b);
	char charA, charB;
	for (int i = 0; i < (int)count; i++)
	{
		if (i >= lenA)
		{
			return -1;
		}
		if (i >= lenB)
		{
			return 1;
		}
		charA = tolower(a[i]);
		charB = tolower(b[i]);
		if (charA < charB)
			return -1;
		else if (charA > charB)
			return 1;
	}
	return 0;
}
#endif