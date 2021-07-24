#pragma once

#ifndef __BACKEND_SDL2_H__
#define __BACKEND_SDL2_H__
#include <SDL2/SDL.h>
#include "IBackend.h"

namespace jam
{
    class BackendSDL2 :
        public IBackend
    {
    public:
        BackendSDL2();
        ~BackendSDL2();
        bool Construct(std::string title, int screenWidth, int screenHeight);
        bool IsKeyDown(uint8_t code);
        bool IsKeyPressed(uint8_t code);
        bool IsKeyReleased(uint8_t code);
        void Start(IScene* scene);
        bool OnUserCreate();
        bool OnUserUpdate(float dt);
    protected:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int mouseX, mouseY;
        bool mouseBtnLeft, mouseBtnRight, oldMouseBtnLeft, oldMouseBtnRight;
    };
}
#endif