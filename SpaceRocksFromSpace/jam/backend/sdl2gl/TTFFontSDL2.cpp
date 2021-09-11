#include <iostream>
#include "BackendSDL2.h"
#include "../../Shared.h"
#include "TTFFontSDL2.h"
#include "ImageSDL2.h"

namespace jam
{
    TTFFontSDL2::TTFFontSDL2()
    {
        this->ttfFont = nullptr;
    }

    TTFFontSDL2::~TTFFontSDL2()
    {
        if (this->ttfFont != nullptr)
        {
            TTF_CloseFont(this->ttfFont);
            this->ttfFont = nullptr;
        }
    }

    bool TTFFontSDL2::Construct(std::string fileName, int pointSize)
    {
        this->ttfFont = TTF_OpenFont(fileName.c_str(), pointSize);
        if (this->ttfFont == nullptr)
        {
            std::cerr << "Failed to load font \"" << fileName << "\" Size " << pointSize << ". Error: \"" << SDL_GetError() << "\"" << std::endl;
            this->ttfFont = nullptr;
            return false;
        }
        return true;
    }

    void TTFFontSDL2::DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color)
    {
        SDL_Color fg = { color.r, color.g, color.b };
        SDL_Surface* ttfSurface = TTF_RenderText_Solid(this->ttfFont, message.c_str(), fg);
        ImageSDL2* ttfTex = new ImageSDL2();
	ttfTex->Construct(ttfSurface);
	render->DrawImage(ttfTex, x, y);

        SDL_FreeSurface(ttfSurface);
	delete ttfTex;
    }

    void TTFFontSDL2::MeasureText(std::string message, int* width, int* height)
    {
        TTF_SizeText(this->ttfFont, message.c_str(), width, height);
    }
}
