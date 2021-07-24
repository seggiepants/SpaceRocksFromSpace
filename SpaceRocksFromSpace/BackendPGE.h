#ifndef __BACKEND_PGE_H__
#define __BACKEND_PGE_H__
#pragma once
#include <olcPixelGameEngine.h>
#include <olcPGEX_Sound.h>
#include <olcPGEX_Gamepad.h>
#include <olcPGEX_TTF.h>
#include "IBackend.h"

namespace jam
{
    class BackendPGE :
        public IBackend, olc::PixelGameEngine
    {
    public:
        BackendPGE();
        ~BackendPGE();
        bool Construct(std::string title, int screenWidth, int screenHeight);
        bool IsKeyDown(uint8_t code);
        bool IsKeyPressed(uint8_t code);
        bool IsKeyReleased(uint8_t code);
        void Start(IScene* scene);
        bool OnUserCreate() override;
        bool OnUserUpdate(float dt) override;
    protected:
        olc::GPButtons JoystickButtonToPGE(JoystickButton btn);
        JoystickButton PGEToJoystickButton(olc::GPButtons btn);
        olc::vi2d oldMousePos;
        std::vector<olc::GamePad*> gamePads;
    };    
}
#endif