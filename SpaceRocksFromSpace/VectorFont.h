#pragma once
#ifndef __VECTOR_FONT_H__
#define __VECTOR_FONT_H__
#include "IFont.h"
#include <map>
#include <string>
#include <vector>
#include "IRenderer.h"
#include "Point2D.h"
#include "RGB.h"

namespace game
{
#ifdef DrawText
#undef DrawText
#endif
	class VectorFont : public jam::IFont
	{
	public:
		VectorFont();
		~VectorFont();
		void Construct(float scaleX, float scaleY);
		void DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color);
		void MeasureText(std::string message, int* width, int* height);
	protected:
		std::map<char, std::vector<std::vector<jam::Point2Df>>>* characters;
		float scaleX, scaleY;
	};
}
#endif
