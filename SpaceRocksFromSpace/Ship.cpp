#define _USE_MATH_DEFINES
#include <cmath>
#include "Ship.h"

namespace game
{

	const float rotateSpeed = 3 * M_PI;
	const float shipMoveSpeed = 300.0;

    Ship::Ship()
    {
        this->model.clear();
        this->model.push_back({ -8, 8 });
        this->model.push_back({ 0, 5 });
        this->model.push_back({ 8, 8 });
        this->model.push_back({ 0, -8 });

        this->screenModel.clear();
        for (int i = 0; i < this->model.size(); i++)
        {
            this->screenModel.push_back({ 0, 0 });
        }		
    }

    Ship::~Ship()
    {

    }

    void Ship::Construct(int screenWidth, int screenHeight)
    {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->SetPosition(screenWidth / 2.0, screenHeight / 2.0);
        this->heading = 0.00;
		this->moveSpeed = 0.0;
		this->rotateDir = 0;
    }

    void Ship::Draw(jam::IRenderer* render)
    {
        jam::rgb white;
        white.r = white.g = white.b = white.a = 255;
        int iPrevious = this->screenModel.size() - 1;
        for (int i = 0; i < this->screenModel.size(); i++)
        {
            jam::Point2Df pointTo = this->screenModel[i];
            jam::Point2Df pointFrom = this->screenModel[iPrevious];
            render->DrawLine(pointTo.x, pointTo.y, pointFrom.x, pointFrom.y, white);
            iPrevious = i;
        }
    }

	void Ship::GetCanonPosition(float* x, float* y, float* heading)
	{
		const float offset = 8.0;
		*heading = M_PI_2 - this->heading;
		*x = this->x + offset * cos(*heading);
		*y = this->y - offset * sin(*heading);
	}

	void Ship::Rotate(int dir)
	{
		if (dir < 0)
		{
			this->rotateDir = -1;
		}
		else if (dir > 0)
		{
			this->rotateDir = 1;
		}
		else
		{
			this->rotateDir = 0;
		}
	}
	
	void Ship::SetPosition(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void Ship::Update(jam::IScene* scene, float dt)
    {
		int screenWidth, screenHeight;
		scene->GetScreenSize(&screenWidth, &screenHeight);

		this->heading += this->rotateDir * rotateSpeed * dt;
		while (this->heading < 0.0)
		{
			this->heading += (M_PI * 2.0);
		}
		while (this->heading > (M_PI * 2.0))
		{
			this->heading -= (M_PI * 2.0);
		}

		float cosAngle = cosf(this->heading);
		float sinAngle = sinf(this->heading);
		this->x += this->moveSpeed * dt * cosAngle;
		this->y += this->moveSpeed * dt * sinAngle;
		float xMin, yMin, xMax, yMax, xScreen, yScreen;
		xMin = yMin = xMax = yMax = 0.0;
		for (int i = 0; i < this->model.size(); i++)
		{
			float x1 = (this->model[i].x);
			float y1 = (this->model[i].y);
			xScreen = ((x1 * cosAngle) - (y1 * sinAngle)) + this->x;
			yScreen = ((y1 * cosAngle) + (x1 * sinAngle)) + this->y;
			this->screenModel[i].x = xScreen;
			this->screenModel[i].y = yScreen;
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
}