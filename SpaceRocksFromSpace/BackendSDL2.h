#pragma once

#ifndef __BACKEND_SDL2_H__
#define __BACKEND_SDL2_H__
#include <SDL2/SDL.h>
#include <vector>
#include "IBackend.h"
#include "ResourceManagerSDL2.h"

namespace jam
{

    struct JoystickInfoSDL
    {
        int id;
        bool isGamePad;
        SDL_Joystick* joystick;
        SDL_GameController* gamepad;
    };
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
        IResourceManager* ResourceManager() { return ResourceManagerSDL2::Instance(); };
    protected:
        void CloseJoysticks();
        void OpenJoysticks();
        int JoystickButtonToSDL(JoystickButton btn);
        JoystickButton SDLToJoystickButton(int btn);
        SDL_Window* window;
        SDL_Renderer* renderer;
        int mouseX, mouseY;
        bool mouseBtnLeft, mouseBtnRight, oldMouseBtnLeft, oldMouseBtnRight;        
        int numJoysticks;
        std::vector<JoystickInfoSDL*> joysticks;
    };
}
#endif