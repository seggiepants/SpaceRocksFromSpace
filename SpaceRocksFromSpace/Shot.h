#pragma once
#ifndef __SHOT_H__
#define __SHOT_H__
#include "IEntity.h"
#include "RGB.h"

namespace game
{
    class Shot :
        public jam::IEntity
    {
    public:
        Shot();
        ~Shot();
        void Draw(jam::IRenderer* render);
        void Update(jam::IScene* scene, float dt);
        bool IsDeleted() { return !this->alive; };
        void SetHeading(float ang);
        void SetPosition(float x, float y);
    protected:
        float oldX, oldY, vx, vy, age;
        bool alive, visible;
        jam::rgb color;
    };
}

#endif