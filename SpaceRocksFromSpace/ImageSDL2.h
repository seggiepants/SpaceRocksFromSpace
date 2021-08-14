#pragma once
#ifndef __IMAGE_SDL2_H__
#define __IMAGE_SDL2_H__

#include <string>
#include <SDL2\SDL.h>
#include "IImage.h"
#include "IRenderer.h"

namespace jam
{
    class ImageSDL2 :
        public IImage
    {
        ImageSDL2();
        ~ImageSDL2();
        bool Construct(jam::IRenderer* render, std::string fileName);
        int GetWidth();
        SDL_Texture* GetHandle();
        int GetHeight();
    protected:
        SDL_Texture* LoadSprite(SDL_Renderer* render, std::string fileName);
        int width;
        int height;
        int bpp;
        SDL_Texture* sprite;
    };

}

#endif