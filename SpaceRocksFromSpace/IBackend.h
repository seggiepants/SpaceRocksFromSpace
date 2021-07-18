#pragma once

#ifndef __IBACKEND_H__
#define __IBACKEND_H__

#include <string>
#include "IRenderer.h"
#include "IScene.h"

namespace jam
{
    class IBackend
    {
    public:
        IBackend();
        virtual bool Construct(std::string title, int screenWidth, int screenHeight) = 0;                
        virtual void Start(IScene* scene) = 0;
        virtual bool OnUserCreate() = 0;
        virtual bool OnUserUpdate(float dt) = 0;        
    protected:
        IScene* currentScene;
        IRenderer* render;
    };
}
#endif

