#define _USE_MATH_DEFINES
#include <cmath>
#include "../../jam/Collision.h"
#include "../../game/GameAssets.h"
#include "../../jam/RGB.h"
#include "Rock.h"
#include "../../jam/Shared.h"
#include "../../Utility.h"

namespace game
{
	const float Pi = M_PI;
	const float twoPi = M_PI * 2.0;
	const float MAX_SPEED = 300;
	const float MIN_SPEED = 50;
	const float MAX_SCALE = 30.0;
	const float MIN_SCALE = 10.0;
	const int ROCK_POINTS = 8;

	Rock::Rock()
	{
		this->deleted = false;
		this->visible = true;
		this->model = new std::vector<jam::Point2Df>();
		this->screenModel = new std::vector<jam::Point2Df>();
		this->rotateSpeed = rndf(20.0) - 9.9; // 0 to 3 seconds clockwise or counter clockwise.
		this->moveSpeed = rndf(MAX_SPEED - MIN_SPEED) + MIN_SPEED;
		this->scale = abs(rndf(MAX_SCALE - MIN_SCALE)) + MIN_SCALE;
		this->heading = rndf(twoPi);
		this->angle = rndf(twoPi);
		if (rand() % 2 == 1)
			this->angle *= -1.0;

		float dAngle = twoPi / ROCK_POINTS;
		float radius = 1.0;
		
		for (int i = 0; i < ROCK_POINTS; i++)
		{
			float angle = dAngle * i;
			float rx = radius * (1.0 + rndf(1.0)) * cos(angle);
			float ry = radius * (1.0 + rndf(1.0)) * sin(angle);
			this->model->push_back({ rx, ry });
			this->screenModel->push_back({ rx, ry });
		}
		this->RecalculateScreenModel();
		/*
		float cosAngle = cosf(this->angle);
		float sinAngle = sinf(this->angle);
		float cosHeading = cosf(this->heading);
		float sinHeading = sinf(this->heading);
		this->x += this->moveSpeed * dt * cosHeading;
		this->y += this->moveSpeed * dt * sinHeading;
		float xMin, yMin, xMax, yMax, xScreen, yScreen;
		xMin = yMin = xMax = yMax = 0.0;
		for (int i = 0; i < this->model->size(); i++)
		{
			float x1 = (this->model->at(i).x * this->scale);
			float y1 = (this->model->at(i).y * this->scale);
			xScreen = ((x1 * cosAngle) - (y1 * sinAngle)) + this->x;
			yScreen = ((y1 * cosAngle) + (x1 * sinAngle)) + this->y;
			this->screenModel->at(i).x = xScreen;
			this->screenModel->at(i).y = yScreen;
			if (i == 0)
			{
				xMin = xMax = xScreen;
				yMin = yMax = yScreen;
			}
			else
			{
				xMin = xScreen < xMin ? xScreen : xMin;
				xMax = xScreen > xMax ? xScreen : xMax;
				yMin = yScreen < yMin ? yScreen : yMin;
				yMax = yScreen > yMax ? yScreen : yMax;

			}
		}
		*/
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

	bool Rock::Collide_Circle_Point(float x, float y)
	{
		jam::Point2Df center{ this->x, this->y };
		jam::Point2Df point{ x, y };
		return jam::Collision::Circle_Point(&center, this->scale, &point);
	}

	bool Rock::Collide_Line(float x1, float y1, float x2, float y2)
	{
		bool ret = false;
		float xa, ya, xb, yb;
		xa = this->screenModel->at(this->screenModel->size() - 1).x;
		ya = this->screenModel->at(this->screenModel->size() - 1).y;
		for (int i = 0; i < this->screenModel->size(); i++)
		{
			xb = xa;
			yb = ya;
			xa = this->screenModel->at(i).x;
			ya = this->screenModel->at(i).y;
			ret = jam::Collision::Line_Line(xa, ya, xb, yb, x1, y1, x2, y2);
			if (ret)
				break;
		}
		return ret;
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

			// Debug draw
			/*
			float step = M_PI / 4;
			float x, y;
			float oldX, oldY;
			oldX = this->x + this->scale;
			oldY = this->y;
			jam::rgb color{ 255, 255, 0, 255 };
			for (float ang = step; ang <= M_PI * 2.0 + step; ang += step)
			{
				x = this->x + (this->scale * cos(ang));
				y = this->y + (this->scale * sin(ang));
				render->DrawLine(oldX, oldY, x, y, color);
				oldX = x;
				oldY = y;
			}
			*/

		}
	}

	void Rock::RecalculateScreenModel()
	{
		float x, y, x1, y1, xScreen, yScreen;
		float sinAngle, cosAngle;
		sinAngle = sinf(this->angle);
		cosAngle = cosf(this->angle);
		for (int i = 0; i < this->model->size(); i++)
		{
			x = this->model->at(i).x * this->scale;
			y = this->model->at(i).y * this->scale;
			xScreen = ((x * cosAngle) - (y * sinAngle)) + this->x;
			yScreen = ((y * cosAngle) + (x * sinAngle)) + this->y;
			this->screenModel->at(i).x = xScreen;
			this->screenModel->at(i).y = yScreen;
		}
	}

	void Rock::SetDeleted() 
	{
		this->deleted = true; 
		jam::backEnd->ResourceManager()->GetAudio(game::SOUND_EXPLOSION)->Play();
	}
	
	void Rock::Update(jam::IScene* scene, float dt)
	{
		int screenWidth, screenHeight;
		scene->GetScreenSize(&screenWidth, &screenHeight);
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
		float xMin, yMin, xMax, yMax, xScreen, yScreen;
		xMin = yMin = xMax = yMax = 0.0;
		for (int i = 0; i < this->model->size(); i++)
		{
			float x1 = (this->model->at(i).x * this->scale);
			float y1 = (this->model->at(i).y * this->scale);
			xScreen = ((x1 * cosAngle) - (y1 * sinAngle)) + this->x;
			yScreen = ((y1 * cosAngle) + (x1 * sinAngle)) + this->y;
			this->screenModel->at(i).x = xScreen;
			this->screenModel->at(i).y = yScreen;
			if (i == 0)
			{
				xMin = xMax = xScreen;
				yMin = yMax = yScreen;
			}
			else
			{
				xMin = xScreen < xMin ? xScreen : xMin;
				xMax = xScreen > xMax ? xScreen : xMax;
				yMin = yScreen < yMin ? yScreen : yMin;
				yMax = yScreen > yMax ? yScreen : yMax;

			}
		}
		while (xMax < 0.0)
		{
			this->x += screenWidth;
			xMax += screenWidth;
			xMin += screenWidth;
		}
		while (xMin >= screenWidth)
		{
			this->x -= screenWidth;
			xMax -= screenWidth;
			xMin -= screenWidth;
		}
		while (yMax < 0.0)
		{
			this->y += screenHeight;
			yMax += screenHeight;
			yMin += screenHeight;
		}
		while (yMin >= screenHeight)
		{
			this->y -= screenHeight;
			yMax -= screenHeight;
			yMin -= screenHeight;
		}
	}

	bool Rock::IsDeleted()
	{
		return this->deleted;
	}

}
