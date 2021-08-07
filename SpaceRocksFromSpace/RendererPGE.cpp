#include "RendererPGE.h"

namespace jam
{
	void RendererPGE::Init(olc::PixelGameEngine* app)
	{
		this->app = app;
	}

	void RendererPGE::Clear(rgb bg)
	{
		olc::Pixel p((uint8_t) bg.r, (uint8_t) bg.g, (uint8_t) bg.b, (uint8_t) bg.a);
		app->Clear(p);
	}

	void RendererPGE::DrawLine(int x1, int y1, int x2, int y2, rgb color)
	{
		olc::Pixel p((uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a);
		app->DrawLine((int32_t)x1, (int32_t)y1, (int32_t)x2, (int32_t)y2, p);
	}

	void RendererPGE::DrawPoint(int x, int y, rgb color)
	{
		olc::Pixel p((uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a);
		olc::vi2d point{ x, y };
		app->Draw(point, p);
	}

	void RendererPGE::DrawPolygon(float x, float y, std::vector<jam::Point2Df>* points, jam::rgb color)
	{
		olc::vi2d from, to;
		olc::Pixel clr((uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a);
		
		// Prime with last point to make a closed shape.
		to.x = points->at(points->size() - 1).x + x;
		to.y = points->at(points->size() - 1).y + y;
		for (int i = 0; i < points->size(); i++)
		{
			from.x = to.x;
			from.y = to.y;
			to.x = points->at(i).x + x;
			to.y = points->at(i).y + y;
			app->DrawLine(from, to, clr);
		}
	}

	void RendererPGE::FillRect(int x1, int y1, int x2, int y2, rgb color)
	{
		olc::Pixel p((uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a);
		int32_t w = abs(x2 - x1);
		int32_t h = abs(y2 - y1);
		int32_t x = x1 > x2 ? x2 : x1;
		int32_t y = y1 > y2 ? y2 : y1;
		app->FillRect(x, y, w, h, p);
	}

	void RendererPGE::GetScreenSize(int* width, int* height)
	{
		olc::vi2d size = this->app->GetWindowSize();
		*width = size.x;
		*height = size.y;
	}

}
