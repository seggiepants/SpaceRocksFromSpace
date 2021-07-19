#pragma once
#include <SDL2/SDL.h>
#include "IRenderer.h"

namespace jam
{
    class RendererSDL2 :
        public IRenderer
    {
    public:
        void Init(SDL_Window* window, SDL_Renderer* renderer);
        void Clear(rgb bg);
        void DrawLine(int x1, int y1, int x2, int y2, rgb color);
        void FillRect(int x1, int y1, int x2, int y2, rgb color);
        void GetScreenSize(int* width, int* height);
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
    };

}
