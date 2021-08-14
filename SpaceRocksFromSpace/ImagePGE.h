#pragma once
#ifndef __IMAGE_PGE_H__
#define __IMAGE_PGE_H__

#include <string>
#include <olcPixelGameEngine.h>
#include "IRenderer.h"
#include "IImage.h"

namespace jam
{
    class ImagePGE :
        public IImage
    {
    public:
        ImagePGE();
        ~ImagePGE();
        bool Construct(jam::IRenderer* render, std::string fileName);
        int GetWidth();
        olc::Decal* GetHandle();
        int GetHeight();
    protected:
        olc::Sprite* LoadSprite(std::string fileName); 
        int width;
        int height;
        int bpp;
        olc::Sprite* sprite;
        olc::Decal* decal;
    };
}

#endif