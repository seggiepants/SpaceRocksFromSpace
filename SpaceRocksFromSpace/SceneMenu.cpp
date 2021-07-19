#include <cstdlib>
#include "SceneMenu.h"

namespace game
{
	#define rndf(n) (float(rand())/float((RAND_MAX)) * n)

	SceneMenu::SceneMenu()  
	{		
		this->screenWidth = this->screenHeight = 0;
		this->maxStarDepth = 0.0;
		this->vFont = new VectorFont();
		this->menuText = new std::vector<std::string>();
		this->stars = new std::vector<game::Star>();
	}

	SceneMenu::~SceneMenu() 
	{
		this->menuText->clear();
		delete this->menuText;
		this->menuText = nullptr;

		this->stars->clear();
		delete this->stars;
		this->stars = nullptr;

		delete this->vFont;
		this->vFont = nullptr;
	}

	void SceneMenu::Construct(int screenWidth, int screenHeight)
	{
		const int numStars = 4000;
		this->menuText->clear();
		this->menuText->push_back("PLAY");
		this->menuText->push_back("HIGH SCORES");
		this->menuText->push_back("EXIT");

		this->stars->clear();
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		this->maxStarDepth = (screenWidth > screenHeight ? screenHeight : screenWidth) / 2.0;
		for (int i = 0; i < numStars; i++)
		{
			this->stars->push_back({ 0.0, 0.0, 0.0, 0.0 });
		}
	}

	void SceneMenu::Draw(jam::IRenderer* render)
	{
		jam::rgb bg(0, 0, 0, 255);
		render->Clear(bg);
		int x1, y1, x2, y2;
		int width, height;
		jam::rgb color;

		render->GetScreenSize(&width, &height);
		/*
		for (int i = 0; i < 100; i++)
		{
			color.r = rand() % 255;
			color.g = rand() % 255;
			color.b = rand() % 255;
			color.a = 255;

			x1 = rand() % width;
			y1 = rand() % height;
			x2 = rand() % width;
			y2 = rand() % height;

			render->DrawLine(x1, y1, x2, y2, color);
		}
		*/

		float sx, sy, px, py;
		float halfW, halfH;
		render->GetScreenSize(&this->screenWidth, &this->screenHeight);
		halfW = this->screenWidth / 2.0;
		halfH = this->screenHeight / 2.0;
		color.r = color.g = color.b = color.a = 255;
		for (std::vector<game::Star>::iterator iter = this->stars->begin(); iter != this->stars->end(); iter++)
		{
			sx = (*iter).x - halfW;
			sy = (*iter).y - halfH;
			px = (sx / (*iter).zPrevious) + halfW;
			py = (sy / (*iter).zPrevious) + halfH;
			sx = (sx / (*iter).z) + halfW;
			sy = (sy / (*iter).z) + halfH;
			render->DrawLine((int)sx, (int)sy, (int)px, (int)py, color);
		}

		color.r = color.g = color.b = 255;
		std::string msg = "SPACE ROCKS";
		this->vFont->MeasureText(msg, &width, &height, 2.0, 2.0);
		sx = (float)((this->screenWidth - width) / 2);
		sy = 32 + height;
		this->vFont->DrawText(render, msg, (int)sx, (int)sy, color, 2.0, 2.0);
		msg = "FROM SPACE";
		this->vFont->MeasureText(msg, &width, &height, 2.0, 2.0);
		sx = (float)((this->screenWidth - width) / 2);
		sy += height;
		this->vFont->DrawText(render, msg, (int)sx, (int)sy, color, 2.0, 2.0);
		// Find the biggest.
		sy += height * 2;
		int maxWidth = 0 , maxHeight = 0;
		for (std::string menu : *this->menuText)
		{
			this->vFont->MeasureText(menu, &width, &height);
			maxWidth = width > maxWidth ? width : maxWidth;
			maxHeight = height > maxHeight ? height: maxHeight;
		}
		sx = (float)((this->screenWidth - maxWidth) / 2);
		int border = 4;
		int idx = 0;
		jam::rgb black;
		black.r = black.g = black.b = 0;
		black.a = 255;
		jam::rgb white;
		white.r = white.g = white.b = white.a = 255;
		for (std::string menu : *this->menuText)
		{
			if (idx == 0)
			{
				render->FillRect(sx - border, sy - border, sx + maxWidth + (2 * border), sy + maxHeight + (2 * border), white);
				this->vFont->DrawText(render, menu, sx, sy + maxHeight - border, black);
			}
			else
			{
				render->FillRect(sx - border, sy - border, sx + maxWidth + (2 * border), sy + maxHeight + (2 * border), black);
				this->vFont->DrawText(render, menu, sx, sy + maxHeight - border, white);
			}
			sy += maxHeight + border;
			idx++;
		}
		/*
		this->vFont->DrawText(render, "0123456789", 10, 32, color);
		this->vFont->DrawText(render, "ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 10, 52, color);
		this->vFont->DrawText(render, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", 10, 92, color, 1.0, 2.0);
		*/

	}
	
	void SceneMenu::Update(float dt)
	{
		const float starSpeed = 10.0;
		float x, y, z;

		// Repopulate stars out of bounds. Update ones in bounds.
		for (std::vector<game::Star>::iterator iter = this->stars->begin(); iter != this->stars->end(); iter++)
		{
			if ((*iter).z <= 1.0)
			{
				z = 0.0;
				while (z <= 1.0)
				{
					x = rndf(this->screenWidth);
					y = rndf(this->screenHeight);
					z = rndf(this->maxStarDepth);
				}
				(*iter).x = x;
				(*iter).y = y;
				(*iter).z = (*iter).zPrevious = z;
			}
			else
			{
				(*iter).zPrevious = (*iter).z;
				(*iter).z -= dt * starSpeed;
			}
		}
	}
}