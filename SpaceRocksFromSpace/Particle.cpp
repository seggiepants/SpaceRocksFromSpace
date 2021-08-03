#include <cmath>
#include "Particle.h"
#include "Shared.h"

namespace game
{
	Particle::Particle()
	{

	}

	Particle::~Particle()
	{

	}

	void Particle::Construct(float x, float y, float heading, float speed, float lifeTime)
	{
		this->x = x;
		this->y = y;
		this->heading = heading;
		this->lifeTime = lifeTime;
		this->speed = speed;
	}

	void Particle::Draw(jam::IRenderer* render)
	{
		int screenWidth, screenHeight;
		jam::rgb white{ 255, 255, 255, 255 };
		render->GetScreenSize(&screenWidth, &screenHeight);
		if (this->x < 0 || this->y < 0 || this->x >= screenWidth || this->y >= screenHeight)
		{
			this->lifeTime = 0.0;
		}
		else
		{
			render->DrawPoint(this->x, this->y, white);
		}
	}

	void Particle::Update(jam::IScene* scene, float dt)
	{
		this->lifeTime -= dt;
		if (this->lifeTime < 0.0)
			this->lifeTime = 0.0;

		this->x += this->speed * cos(this->heading) * dt;
		this->y += this->speed * sin(this->heading) * dt;
	}
}