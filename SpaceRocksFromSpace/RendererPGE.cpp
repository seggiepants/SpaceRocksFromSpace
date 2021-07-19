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
