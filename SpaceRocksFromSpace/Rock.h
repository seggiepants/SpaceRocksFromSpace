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
        void Draw(jam::IRenderer* render);
        void Update(jam::IScene* scene, float dt);
        bool IsDeleted();
        float GetHeading() { return this->heading; }
        void SetHeading(float heading) { this->heading = heading; };
        void SetPosition(float x, float y) { this->x = x; this->y = y; }
        void SetScale(float scale) { this->scale = scale; }
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