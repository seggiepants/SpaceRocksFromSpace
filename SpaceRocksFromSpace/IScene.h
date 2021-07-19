#pragma once
#ifndef __ISCENE_H__
#define __ISCENE_H__

#include "IRenderer.h"

namespace jam
{
    class IScene
    {
    public:
        virtual void Construct(int screenWidth, int screenHeight) = 0;
        virtual void Draw(jam::IRenderer*) = 0;
        virtual void Update(float dt) = 0;
    };
}
#endif

