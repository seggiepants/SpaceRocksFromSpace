#pragma once
#include <olcPixelGameEngine.h>
#include "IRenderer.h"

namespace jam
{
    class RendererPGE :
        public IRenderer
    {
    public:
        void Init(olc::PixelGameEngine* app);
        void Clear(rgb bg);
        void DrawLine(int x1, int y1, int x2, int y2, rgb color);
        void GetScreenSize(int* width, int* height);
    protected:
        olc::PixelGameEngine* app;
    };
}

