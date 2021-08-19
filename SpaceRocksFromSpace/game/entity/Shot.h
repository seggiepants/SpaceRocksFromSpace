#pragma once
#ifndef __SHOT_H__
#define __SHOT_H__
#include "../../jam/IEntity.h"
#include "../../jam/RGB.h"

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
        void SetDeleted() { this->alive = false; };
        void SetHeading(float ang);
        void SetPosition(float x, float y);
        void GetCollisionLine(float* x1, float* y1, float* x2, float* y2) { *x1 = this->oldX; *y1 = this->oldY; *x2 = this->x; *y2 = this->y; }
    protected:
        float oldX, oldY, vx, vy, age;
        bool alive, visible;
        jam::rgb color;
    };
}

#endif