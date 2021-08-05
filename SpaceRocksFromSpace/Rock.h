#pragma once
#ifndef __ROCK_H__
#define __ROCK_H__

#include <vector>
#include "IEntity.h"
#include "Point2D.h"
#include "IRenderer.h"
#include "IScene.h"

namespace game
{
    class Rock : public jam::IEntity
    {
    public:
        Rock();
        ~Rock();
        bool Collide_Line(float x1, float y1, float x2, float y2);
        bool Collide_Circle_Point(float x, float y);
        void Draw(jam::IRenderer* render);
        void Update(jam::IScene* scene, float dt);
        bool IsDeleted();
        void SetDeleted();
        float GetHeading() { return this->heading; }
        void SetHeading(float heading) { this->heading = heading; this->RecalculateScreenModel(); };
        void GetPosition(float* x, float* y) { *x = this->x; *y = this->y; }
        void SetPosition(float x, float y) { this->x = x; this->y = y; this->RecalculateScreenModel(); }
        float GetScale() { return this->scale; } 
        void SetScale(float scale) { this->scale = scale; }
        float GetSpeed() { return this->moveSpeed; }
        void SetSpeed(float speed) { this->moveSpeed = speed; }
        void RecalculateScreenModel();
    protected:
        bool deleted;
        bool visible;
        float angle;
        float heading;
        float rotateSpeed;
        float moveSpeed;
        float scale;
        std::vector<jam::Point2Df>* model;
        std::vector<jam::Point2Df>* screenModel;
    };
}

#endif