#pragma once
#ifndef __SHIP_H__
#define __SHIP_H__

#include <vector>
#include "Point2D.h"
#include "IRenderer.h"
#include "IScene.h"
#include "IEntity.h"

namespace game
{
    class Ship :
        public jam::IEntity
    {
    public:
        Ship();
        ~Ship();
        void Construct(int screenWidth, int screenHeight);
        void Draw(jam::IRenderer* render);
        void Update(jam::IScene* scene, float dt);
        bool IsDeleted() { return false; };
        void Rotate(int dir);
        void SetPosition(float x, float y);
    protected:
        int rotateDir;
        int screenWidth, screenHeight;
        float heading, moveSpeed;
        std::vector<jam::Point2Df> model;
        std::vector<jam::Point2Df> screenModel;
    };
}
#endif