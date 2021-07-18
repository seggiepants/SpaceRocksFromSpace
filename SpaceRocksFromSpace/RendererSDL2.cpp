#include "RendererSDL2.h"

namespace jam
{
    void RendererSDL2::Init(SDL_Window* window, SDL_Renderer* renderer)
    {
        this->window = window;
        this->renderer = renderer;
    }
    void RendererSDL2::Clear(rgb bg)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(this->renderer, bg.r, bg.g, bg.b, bg.a);
        SDL_RenderClear(this->renderer);
        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    }
    void RendererSDL2::DrawLine(int x1, int y1, int x2, int y2, rgb color)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(this->renderer, x1, y1, x2, y2);
        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    }

    void RendererSDL2::GetScreenSize(int* width, int* height)
    {
        SDL_GetWindowSize(this->window, width, height);
    }
}