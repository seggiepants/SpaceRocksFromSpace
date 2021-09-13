#pragma once
#ifndef __DREAMCAST_PATCH_H__
#define __DREAMCAST_PATCH_H__
#include "../../jam/Shared.h"
#ifdef __DREAMCAST__
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int snprintf(char* s, size_t n, const char* format, ...);
char* _strdup(const char* str);
int vsnprintf(char* s, size_t n, const char* format, va_list arg);
int vasprintf(char** ret, const char* format, va_list ap);
int strncasecmp(const char* a, const char* b, size_t count);
#define LLONG_MAX LONG_MAX
#ifndef ssize_t
#define ssize_t size_t
#endif
#endif
#endif
