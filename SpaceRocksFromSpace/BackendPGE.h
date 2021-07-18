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
        void Start(IScene* scene);
        bool OnUserCreate() override;
        bool OnUserUpdate(float dt) override;
    };
}
#endif