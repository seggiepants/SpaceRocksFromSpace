#pragma once
#ifndef __UFO_H__
#define __UFO_H__
#include "IEntity.h"
#include <vector>
#include "Point2D.h"

namespace game
{
    class UFO :
        public jam::IEntity
    {
	public:
		UFO();
		~UFO();
		void Draw(jam::IRenderer* render);
		void Update(jam::IScene* scene, float dt);
		bool Collide_Line(float x1, float y1, float x2, float y2);
		void Hit();
		bool IsDeleted();
		float GetHeading() { return this->heading; };
		void Respawn(int screenWidth, int screenHeight);
		void SetHeading(float heading);
		void SetPosition(float x, float y);
	protected:
		bool alive;
		float heading;
		float waitTime;
		std::vector<jam::Point2Df> model;
		std::vector<jam::Point2Df> transformed;
    };
}
#endif