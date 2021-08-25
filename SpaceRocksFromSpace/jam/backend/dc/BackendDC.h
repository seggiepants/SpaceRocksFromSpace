#pragma once
#ifndef __BACKEND_DC_H__
#define __BACKEND_DC_H__
#include "../IBackend.h"
#ifdef __DREAMCAST__

namespace jam
{

    class BackendDC :
        public IBackend
    {
    public:
        BackendDC();
        ~BackendDC() override;
        bool Construct(std::string title, int screenWidth, int screenHeight) override;
        void Start(IScene* scene) override = 0;
        bool OnUserCreate() = 0;
        bool OnUserUpdate(float dt) = 0;
        IResourceManager* ResourceManager() = 0;
    };

}

#endif
#endif