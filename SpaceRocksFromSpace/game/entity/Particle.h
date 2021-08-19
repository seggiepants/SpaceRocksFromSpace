#pragma once
#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#include "../../jam/IEntity.h"
#include "../../jam/IRenderer.h"
#include "../../jam/IScene.h"

namespace game
{
    class Particle :
        public jam::IEntity
    {
    public:
        Particle();
        ~Particle();
        void Construct(float x, float y, float heading, float speed, float lifeTime);
        void Draw(jam::IRenderer* render);
        bool IsDeleted() { return lifeTime <= 0.0; };
        void SetPosition(float x, float y) { this->x = x; this->y = y; };
        void Update(jam::IScene* scene, float dt);

    protected:
        float heading;
        float lifeTime;
        float speed;
    };
}

#endif