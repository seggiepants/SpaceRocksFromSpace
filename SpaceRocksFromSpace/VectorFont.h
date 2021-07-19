#pragma once
#ifndef __VECTOR_FONT_H__
#define __VECTOR_FONT_H__
#include <map>
#include <string>
#include <vector>
#include "IRenderer.h"
#include "Point2D.h"
#include "RGB.h"

namespace game
{
	class VectorFont
	{
	public:
		VectorFont();
		~VectorFont();
		void DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb, float scaleX = 1.0, float scaleY = 1.0);
		void MeasureText(std::string message, int* width, int* height, float scaleX = 1.0, float scaleY = 1.0);
	protected:
		std::map<char, std::vector<std::vector<jam::Point2Df>>>* characters;
	};
}
#endif
