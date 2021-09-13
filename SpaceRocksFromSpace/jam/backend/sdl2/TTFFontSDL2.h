#pragma once
#ifndef __TTF_FONT_SDL2_H__
#define __TTF_FONT_SDL2_H__

#include <string>
#include <SDL2/SDL_ttf.h>
#include "../../IFont.h"
#include "../../IRenderer.h"
#include "../../RGB.h"

namespace jam
{

    class TTFFontSDL2 :
        public IFont
    {
    public:
        TTFFontSDL2();
        ~TTFFontSDL2();
        bool Construct(std::string fileName, int pointSize);
        void DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color);
        void MeasureText(std::string message, int* width, int* height);
    protected:
        TTF_Font* ttfFont;
    };
}

#endif