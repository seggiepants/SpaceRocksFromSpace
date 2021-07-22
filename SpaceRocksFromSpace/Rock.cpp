#define _USE_MATH_DEFINES
#include <cmath>
#include "RGB.h"
#include "Rock.h"

namespace game
{
	const float Pi = M_PI;
	const float twoPi = M_PI * 2.0;
	const float MAX_SPEED = 300;
	const float MIN_SPEED = 50;
	const float MAX_SCALE = 50.0;
	const float MIN_SCALE = 0.25;
	const int ROCK_POINTS = 8;

	Rock::Rock()
	{
		this->deleted = false;
		this->visible = true;
		this->model = new std::vector<jam::Point2Df>();
		this->screenModel = new std::vector<jam::Point2Df>();
		this->rotateSpeed = (rand() / (float)RAND_MAX) * 6.0 - 2.9; // 0 to 3 seconds clockwise or counter clockwise.
		this->moveSpeed = (rand() / (float)RAND_MAX) * (MAX_SPEED - MIN_SPEED) + MIN_SPEED;
		this->scale = (rand() / (float)RAND_MAX) * (MAX_SCALE - MIN_SCALE) + MIN_SCALE;
		this->heading = (rand() / (float)RAND_MAX) * twoPi;
		this->angle = (rand() / (float)RAND_MAX) * twoPi;
		float dAngle = twoPi / ROCK_POINTS;
		float radius = 1.0;
		for (int i = 0; i < ROCK_POINTS; i++)
		{
			float angle = dAngle * i;
			float rx = radius * (1.0 + (rand() / float(RAND_MAX))) * cos(angle);
			float ry = radius * (1.0 + (rand() / float(RAND_MAX))) * sin(angle);
			this->model->push_back({ rx, ry });
			this->screenModel->push_back({ rx, ry });
		}
	}

	Rock::~Rock()
	{
		if (this->model != nullptr)
		{
			this->model->clear();
			delete this->model;
			this->model = nullptr;
		}

		if (this->screenModel != nullptr)
		{
			this->screenModel->clear();
			delete this->screenModel;
			this->screenModel = nullptr;
		}
	}

	void Rock::Draw(jam::IRenderer* render)
	{
		if (this->visible && this->screenModel->size() > 0)
		{
			jam::rgb white;
			white.r = white.g = white.b = white.a = 255;
			int iPrevious = this->screenModel->size() - 1;
			for (int i = 0; i < this->screenModel->size(); i++)
			{
				jam::Point2Df pointTo = this->screenModel->at(i);
				jam::Point2Df pointFrom = this->screenModel->at(iPrevious);
				render->DrawLine(pointTo.x, pointTo.y, pointFrom.x, pointFrom.y, white);
				iPrevious = i;
			}

		}
	}

	void Rock::Update(jam::IScene* scene, float dt)
	{
		if (this->deleted)
			return;

		this->angle += this->rotateSpeed * dt;
		while (this->angle < 0.0)
		{
			this->angle += twoPi;
		}
		while (this->angle > twoPi)
		{
			this->angle -= twoPi;
		}

		float cosAngle = cosf(this->angle);
		float sinAngle = sinf(this->angle);
		float cosHeading = cosf(this->heading);
		float sinHeading = sinf(this->heading);
		this->x += this->moveSpeed * dt * cosHeading;
		this->y += this->moveSpeed * dt * sinHeading;

		for (int i = 0; i < this->model->size(); i++)
		{
			float x1 = (this->model->at(i).x * this->scale);
			float y1 = (this->model->at(i).y * this->scale);
			this->screenModel->at(i).x = ((x1 * cosAngle) - (y1 * sinAngle)) + this->x;
			this->screenModel->at(i).y = ((y1 * cosAngle) + (x1 * sinAngle)) + this->y;
		}
		while (this->x < 0)
			this->x += 640;
		while (this->x >= 640)
			this->x -= 640;
		while (this->y < 0)
			this->y += 480;
		while (this->y >= 480)
			this->y -= 480;
	}

	bool Rock::IsDeleted()
	{
		return this->deleted;
	}
}