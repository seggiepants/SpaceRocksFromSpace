#pragma once
#ifndef __IMAGE_SDL2_GL_H__
#define __IMAGE_SDL2_GL_H__

#include <string>
#include <SDL2/SDL.h>
#include "../../IImage.h"
#include "../../IRenderer.h"

namespace jam
{
    class ImageSDL2 :
        public IImage
    {
    public:
        ImageSDL2();
        ~ImageSDL2();
        bool Construct(std::string fileName);
      bool Construct(SDL_Surface* source);
        int GetWidth();
      GLuint GetHandle();
        int GetHeight();
    protected:
      GLuint LoadTexture(std::string fileName, int* width, int* height);
      GLuint LoadTexture(SDL_Surface* image, int* width, int* height);
        int width;
        int height;
        GLuint texture;
    };

}

#endif
