#pragma once
#ifndef __SHARED_H__
#define __SHARED_H__

#include "backend/IBackend.h"

#if defined(__UNIX__) || defined(__linux__)
	#define OS_UNIX
#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define OS_WIN
#elif defined(__DREAMCAST__)
	#define OS_DC
	#include <math.h>
	#include <stdlib.h>
	#ifndef M_PI
		#define M_PI  (3.14159265358979323846264338327950288419716939937510)
		#define M_PI_2  ((3.14159265358979323846264338327950288419716939937510)/2.0)
		#define M_PI_4  ((3.14159265358979323846264338327950288419716939937510)/4.0)
	#endif
#else
	#define OS_UNKNOWN
#endif

namespace jam
{
	extern IBackend* backEnd;
}
#endif