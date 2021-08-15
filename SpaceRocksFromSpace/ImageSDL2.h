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
    public:
        ImageSDL2();
        ~ImageSDL2();
        bool Construct(jam::IRenderer* render, std::string fileName);
        int GetWidth();
        SDL_Texture* GetHandle();
        int GetHeight();
    protected:
        int width;
        int height;
        SDL_Texture* texture;
    };

}

#endif