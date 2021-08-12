#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "Collision.h"
#include "GameAssets.h"
#include "Shared.h"
#include "UFO.h"
#include "Utility.h"

namespace game
{
	const float ufoSpeed = 200.0;
	const float waitMin = 5.0;
	const float waitDelta = 5.0;

	UFO::UFO()
	{
		this->model.clear();
		this->model.push_back({ -15.0, 0.0 });
		this->model.push_back({ -5.0, 5.0 });
		this->model.push_back({ 5.0, 5.0 });
		this->model.push_back({ 15.0, 0.0 });
		this->model.push_back({ -15.0, 0.0 });
		this->model.push_back({ -10.0, -5.0 });
		this->model.push_back({ 10.0, -5.0 });
		this->model.push_back({ 15.0, 0.0 });
		this->model.push_back({ -5.0, 5.0 });
		this->model.push_back({ -5.0, 10.0 });
		this->model.push_back({ 0.0, 12.5 });
		this->model.push_back({ 5.0, 10.0 });
		this->model.push_back({ 5.0, 5.0 });

		this->transformed.clear();
		for (auto& point : this->model)
		{
			this->transformed.push_back({ point.x, point.y });
		}

		// Set the direction 
		this->SetHeading(rndf(M_PI * 2));
		this->Hit();
	}

	UFO::~UFO()
	{
		this->model.clear();
		this->transformed.clear();
	}

	bool UFO::Collide_Line(float x1, float y1, float x2, float y2)
	{
		bool ret = false;
		float xa, ya, xb, yb;
		
		if (!this->alive)
			return false;

		xa = this->transformed[0].x + this->x;
		ya = this->transformed[0].y + this->y;
		for (int i = 1; i < 8; i++)
		{
			xb = xa;
			yb = ya;
			xa = this->transformed[i].x + this->x;
			ya = this->transformed[i].y + this->y;
			ret = jam::Collision::Line_Line(xa, ya, xb, yb, x1, y1, x2, y2);
			if (ret)
				break;
		}
		if (!ret)
		{
			xa = this->transformed[8].x + this->x;
			ya = this->transformed[8].y + this->y;
			for (int i = 9; i < this->transformed.size() - 1; i++)
			{
				xb = xa;
				yb = ya;
				xa = this->transformed[i].x + this->x;
				ya = this->transformed[i].y + this->y;
				ret = jam::Collision::Line_Line(xa, ya, xb, yb, x1, y1, x2, y2);
				if (ret)
					break;
			}
		}
		return ret;
	}

	void UFO::Draw(jam::IRenderer* render)
	{
		if (this->alive)
		{
			jam::rgb white;
			white.r = white.a = 255;  white.g = white.b = 0;
			for (int i = 1; i < 8; i++)
			{
				jam::Point2Df pointTo = this->transformed[i];
				jam::Point2Df pointFrom = this->transformed[(int)i - 1];
				render->DrawLine(pointTo.x + this->x, pointTo.y + this->y, pointFrom.x + this->x, pointFrom.y + this->y, white);
			}
			for (int i = 9; i < this->transformed.size(); i++)
			{
				jam::Point2Df pointTo = this->transformed[i];
				jam::Point2Df pointFrom = this->transformed[(int)i - 1];
				render->DrawLine(pointTo.x + this->x, pointTo.y + this->y, pointFrom.x + this->x, pointFrom.y + this->y, white);
			}
		}
	}

	void UFO::Update(jam::IScene* scene, float dt)
	{
		int screenWidth, screenHeight;
		scene->GetScreenSize(&screenWidth, &screenHeight);
		if (!this->alive)
		{
			this->waitTime -= dt;
			if (this->waitTime <= 0.0)
			{
				this->waitTime = 0;
				this->Respawn(screenWidth, screenHeight);
			}
			return;
		}

		float cosHeading = cosf(this->heading);
		float sinHeading = sinf(this->heading);
		this->x += ufoSpeed * dt * cosHeading;
		this->y += ufoSpeed * dt * sinHeading;

		if (this->x < 0 || this->x >= screenWidth || this->y < 0 || this->y >= screenHeight)
		{
			this->Hit();
		}
	}

	void UFO::Hit()
	{
		this->alive = false;
		this->waitTime = waitMin + rndf(waitDelta);
	}

	bool UFO::IsDeleted()
	{
		return !this->alive;
	}

	void UFO::Respawn(int screenWidth, int screenHeight)
	{
		int edge;
		float angle;
		// Pick an edge

		edge = std::rand() % 4;
		edge = 3;
		angle = rndf(M_PI_2) + M_PI_4;
		switch (edge)
		{
			case 0: // Top
				this->y = 0;
				this->x = (rand() % (screenWidth) / 2) + (screenWidth / 4);
				this->SetHeading(angle);
				break;
			case 1: // Left
				this->y = (rand() % (screenHeight) / 2) + (screenHeight / 4);
				this->x = 0;
				this->SetHeading(angle - M_PI_2);
				break;
			case 2: // Right
				this->y = (rand() % (screenHeight) / 2) + (screenHeight / 4);
				this->x = screenWidth - 1;
				this->SetHeading(angle + M_PI_2);
				break;
			default: // Bottom
				this->y = screenHeight - 1;
				this->x = (rand() % (screenWidth) / 2) + (screenWidth / 4);
				this->SetHeading(angle + M_PI);
				break;
		}
		
		this->alive = true;
		jam::backEnd->ResourceManager()->GetAudio(game::SOUND_UFO)->Play();
	}

	void UFO::SetHeading(float heading)
	{
		this->heading = heading;

		// Update the model.
		float x, y, x1, y1, xScreen, yScreen;
		float sinAngle, cosAngle;
		sinAngle = std::sinf(M_PI); // this->heading + M_PI);
		cosAngle = std::cosf(M_PI); // this->heading + M_PI_2);
		for (int i = 0; i < this->model.size(); i++)
		{
			x = this->model[i].x;
			y = this->model[i].y;
			xScreen = ((x * cosAngle) - (y * sinAngle));
			yScreen = ((y * cosAngle) + (x * sinAngle));
			this->transformed[i].x = xScreen;
			this->transformed[i].y = yScreen;
		}
	}

	void UFO::SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
}