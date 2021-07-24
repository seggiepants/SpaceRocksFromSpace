#pragma once
#ifndef __ISCENE_H__
#define __ISCENE_H__

#include <cstdint>
#include "KeyCodes.h"
#include "IRenderer.h"

namespace jam
{

    enum MouseButton
    {
        LEFT, RIGHT
    };
    class IScene
    {
    public:
        virtual void Construct(int screenWidth, int screenHeight) = 0;
        virtual void Draw(jam::IRenderer*) = 0;
        virtual void GetScreenSize(int* screenWidth, int* screenHeight) = 0;
        virtual void Update(float dt) = 0;
        virtual void KeyDown(uint8_t key) {};
        virtual void KeyUp(uint8_t key) {};
        virtual void MouseMove(int x, int y) = 0;
        virtual void MouseClick(MouseButton button, int x, int y) = 0;
        virtual IScene* NextScene() = 0;
    };
}
#endif

