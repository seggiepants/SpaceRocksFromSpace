#define _USE_MATH_DEFINES
#include <cmath>
#include "Shot.h"

namespace game
{
#define MAX_SHOT_AGE 2.0
#define SHOT_SPEED 200.0
	Shot::Shot()
	{
		this->alive = true;
		this->visible = true;
		this->oldX = this->oldY = this->x = this->y = 0.0;
		this->vx = 0.0;
		this->vy = 1.0;
		this->age = MAX_SHOT_AGE;
		this->color.r = this->color.g = this->color.b = this->color.a = 255;
		this->color.g = this->color.b = 0;
	}

	Shot::~Shot()
	{

	}

	void Shot::SetHeading(float ang)
	{
		this->vx = cos(ang);
		this->vy = -1 * sin(ang);
	}

	void Shot::SetPosition(float x, float y)
	{
		this->x = this->oldX = x;
		this->y = this->oldY = y;
	}

	void Shot::Draw(jam::IRenderer* render)
	{
		if (this->alive && this->visible)
		{
			render->DrawLine(this->oldX, this->oldY, this->x, this->y, this->color);
		}
	}

	void Shot::Update(jam::IScene* scene, float dt)
	{
		int screenWidth, screenHeight;
		scene->GetScreenSize(&screenWidth, &screenHeight);
		if (!this->alive)
			return;

		this->oldX = this->x;
		this->oldY = this->y;

		this->x += this->vx * dt * SHOT_SPEED;
		this->y += this->vy * dt * SHOT_SPEED;

		if (this->x < 0 || this->x >= screenWidth || this->y < 0 || this->y >= screenHeight)
		{
			this->alive = false;
		}
		
		this->age -= dt;
		if (this->age <= 0.0)
		{
			this->alive = false;
		}
	}
}