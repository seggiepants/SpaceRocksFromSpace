#pragma once
#ifndef __IRENDERER_H__
#define __IRENDERER_H__
#include "RGB.h"

namespace jam
{
	class IRenderer
	{
	public:
		virtual void Clear(rgb bg) = 0;
		virtual void DrawLine(int x1, int y1, int x2, int y2, rgb color) = 0;
		virtual void FillRect(int x1, int y1, int x2, int y2, rgb color) = 0;
		virtual void GetScreenSize(int* width, int* height) = 0;
	};
}

#endif