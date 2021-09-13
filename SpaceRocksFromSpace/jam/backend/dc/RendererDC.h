#pragma once
#ifndef __RENDERER_DC_H__
#define __RENDERER_DC_H__
#include "../../IRenderer.h"
#ifdef __DREAMCAST__

#include <kos.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace jam
{
	class RendererDC : public IRenderer
	{
	public:
		RendererDC();
		~RendererDC() override;
		void Clear(jam::rgb bg);
		void DrawImage(jam::IImage* img, int x, int y);
		void DrawLine(int x1, int y1, int x2, int y2, jam::rgb color);
		void DrawPoint(int x, int y, jam::rgb color);
		void DrawPolygon(float x, float y, std::vector<jam::Point2Df>* points, jam::rgb color);
		void DrawSubImage(IImage* img, int screenX, int screenY, int x, int y, int w, int h);
		void FillRect(int x1, int y1, int x2, int y2, jam::rgb color);
		void GetScreenSize(int* width, int* height);
	};
}
#endif
#endif