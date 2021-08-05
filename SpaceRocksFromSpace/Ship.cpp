#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include "GameAssets.h"
#include "Ship.h"
#include "Shared.h"

namespace game
{

	const int DEFAULT_LIVES = 3;
	const float INVINCIBLE_TIME = 1.5;
	const float rotateSpeed = 2.5 * M_PI;
	const float shipMoveSpeed = 300.0;
	const float teleportDelay = 0.5;

    Ship::Ship()
    {
		this->lives = DEFAULT_LIVES;
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
		this->toggle = false;
		this->invincibleTime = 0.0;
		this->teleportWait = 0.0;
		this->vx = this->vy = 0.0;
    }

    void Ship::Draw(jam::IRenderer* render)
    {
        jam::rgb white;

		if (this->invincibleTime <= 0 || this->toggle)
		{
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
    }

	void Ship::GetCanonPosition(float* x, float* y, float* heading)
	{
		const float offset = 8.0;
		*heading = M_PI_2 - this->heading;
		*x = this->x + offset * cos(*heading);
		*y = this->y - offset * sin(*heading);
	}

	void Ship::GetThrustPosition(float* x, float* y, float* heading)
	{
		const float offset = 10.0;
		*heading = M_PI_2 + this->heading;
		*x = this->x + offset * cos(*heading);
		*y = this->y + offset * sin(*heading);
	}

	void Ship::GetScreenLine(int index, float* x1, float* y1, float* x2, float* y2)
	{
		int cur = index;
		if (index < 0 || index >= this->screenModel.size())
			index = 0;
		int prev = cur - 1;
		if (prev < 0)
			prev += this->screenModel.size();

		*x1 = this->screenModel[prev].x;
		*y1 = this->screenModel[prev].y;
		*x2 = this->screenModel[cur].x;
		*y2 = this->screenModel[cur].y;
	}

	bool Ship::Hit()
	{
		bool retVal = false;
		if (this->invincibleTime <= 0.0)
		{
			retVal = true;
			jam::backEnd->ResourceManager()->GetAudio(game::SOUND_EXPLOSION)->Play();;
			this->invincibleTime = INVINCIBLE_TIME;
			this->lives--;
			if (this->lives <= 0)
			{
				// Game Over.
			}
		}
		return retVal;
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
		this->vx = 0.0;
		this->vy = 0.0;
    }

	void Ship::Teleport()
	{
		if (this->teleportWait <= 0.0)
		{
			this->teleportWait = teleportDelay;
			this->x = (float)(std::rand() % this->screenWidth);
			this->y = (float)(std::rand() % this->screenHeight);
			this->vx = 0.0;
			this->vy = 0.0;
			jam::IAudio* audio = jam::backEnd->ResourceManager()->GetAudio(game::SOUND_TELEPORT);
			if (audio != nullptr) audio->Play();
		}
	}

	bool Ship::Thrust()
	{
		const float headingAdjust = M_PI / 2.0;
		this->moveSpeed = shipMoveSpeed;
		this->vx += cos(this->heading - headingAdjust);
		this->vy += sin(this->heading - headingAdjust);
		float magnitude = std::sqrtf(this->vx * this->vx + this->vy * this->vy);
		this->vx /= magnitude;
		this->vy /= magnitude;
		return true;
	}

    void Ship::Update(jam::IScene* scene, float dt)
    {
		const float headingAdjust = M_PI / 2.0;

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
		this->x += this->moveSpeed* dt* vx;
		this->y += this->moveSpeed* dt* vy;
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

		if (this->invincibleTime >= 0.0)
		{
			this->toggle = !this->toggle;
			this->invincibleTime -= dt;
			if (this->invincibleTime < 0.0)
				this->invincibleTime = 0.0;
		}
		if (this->moveSpeed > 0.0)
			this->moveSpeed = this->moveSpeed >= 0 ? this->moveSpeed  - (shipMoveSpeed * 0.5  * dt) : 0.0;		
		this->teleportWait -= dt;
		if (this->teleportWait < 0.0)
			this->teleportWait = 0;

    }
}