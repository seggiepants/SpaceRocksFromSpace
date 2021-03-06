#pragma once
#ifndef __SHIP_H__
#define __SHIP_H__

#include <vector>
#include "../../jam/Point2D.h"
#include "../../jam/IRenderer.h"
#include "../../jam/IScene.h"
#include "../../jam/IEntity.h"

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
        void GetCanonPosition(float* x, float* y, float* heading);
        void GetThrustPosition(float* x, float* y, float* heading);
        int GetLives() { return this->lives; };
        int GetScreenPoints() { return this->screenModel.size(); }
        void GetScreenLine(int index, float* x1, float* y1, float* x2, float* y2);
        bool Hit();
        bool IsDeleted() { return this->lives <= 0; };
        void Rotate(int dir);
        void SetPosition(float x, float y);
        void SetHeading(float heading) { this->heading = heading; };
        void Teleport();
        bool Thrust();
    protected:
        int lives;
        int rotateDir;
        int screenWidth, screenHeight;
        float heading, moveSpeed;
        float vx, vy;
        std::vector<jam::Point2Df> model;
        std::vector<jam::Point2Df> screenModel;
        float invincibleTime;
        bool toggle;
        float teleportWait;
    };
}
#endif