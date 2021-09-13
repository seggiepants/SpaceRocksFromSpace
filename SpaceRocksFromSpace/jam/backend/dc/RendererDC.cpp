#ifdef __DREAMCAST__
#include "RendererDC.h"

namespace jam
{
	RendererDC::RendererDC()
	{
		
	}

	RendererDC::~RendererDC()
	{
	}

	void RendererDC::Clear(jam::rgb bg)
	{
	}

	void RendererDC::DrawImage(jam::IImage* img, int x, int y)
	{
	}

	void RendererDC::DrawLine(int x1, int y1, int x2, int y2, jam::rgb color)
	{
	}

	void RendererDC::DrawPoint(int x, int y, jam::rgb color)
	{
	}

	void RendererDC::DrawPolygon(float x, float y, std::vector<jam::Point2Df>* points, jam::rgb color)
	{
	}

	void RendererDC::DrawSubImage(IImage* img, int screenX, int screenY, int x, int y, int w, int h)
	{
	}

	void RendererDC::FillRect(int x1, int y1, int x2, int y2, jam::rgb color)
	{
	}

	void RendererDC::GetScreenSize(int* width, int* height)
	{
	}
}
#endif