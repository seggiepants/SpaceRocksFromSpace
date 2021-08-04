#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "GameAssets.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "Shared.h"

namespace game
{	
	#define SHOT_DELAY  0.1
	#define JOY_MOVE_DELAY 0.2

	SceneMenu::SceneMenu()  
	{		
		this->screenWidth = this->screenHeight = 0;
		this->maxStarDepth = 0.0;
		this->vFont = new VectorFont();
		this->menuText = new std::vector<std::pair<std::string, jam::Rect>>();
		this->stars = new std::vector<game::Star>();
		this->nextScene = nullptr;
		this->joyA = this->oldJoyA = false;
		this->joyUp = this->oldJoyUp = false;
		this->joyDown = this->oldJoyDown = false;
		this->joyMoveTimeout = 0;
		this->bgMusic = nullptr;
		this->click = nullptr;
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
		this->menuText->push_back(std::pair<std::string, jam::Rect>("PLAY", { 0, 0, 0, 0 }));
		this->menuText->push_back(std::pair<std::string, jam::Rect>("HIGH SCORES", { 0, 0, 0, 0 }));
		this->menuText->push_back(std::pair<std::string, jam::Rect>("EXIT", { 0, 0, 0, 0 }));

		this->stars->clear();
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		this->maxStarDepth = (screenWidth > screenHeight ? screenHeight : screenWidth) / 2.0;
		for (int i = 0; i < numStars; i++)
		{
			this->stars->push_back({ 0.0, 0.0, 0.0, 0.0 });
		}
		this->menuIndex = 0;
		this->nextScene = (IScene*)this;

		this->click = jam::backEnd->ResourceManager()->GetAudio(SOUND_SELECT);
		this->bgMusic = jam::backEnd->ResourceManager()->GetAudio("assets/sound/main_menu.wav");
		this->bgMusic->Play();
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
		for (std::pair<std::string, jam::Rect> menu : *this->menuText)
		{
			this->vFont->MeasureText(menu.first, &width, &height);
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
		for (std::vector<std::pair<std::string, jam::Rect>>::iterator menu = this->menuText->begin(); menu != this->menuText->end(); menu++)
		{
			menu->second.x1 = sx - border;
			menu->second.y1 = sy - border;
			menu->second.x2 = sx + maxWidth + (2 * border);
			menu->second.y2 = sy + maxHeight + (2 * border);
			if (idx == this->menuIndex)
			{
				render->FillRect(menu->second.x1, menu->second.y1, menu->second.x2, menu->second.y2, white);
				this->vFont->DrawText(render, menu->first, sx, sy + maxHeight, black);
			}
			else
			{
				render->FillRect(menu->second.x1, menu->second.y1, menu->second.x2, menu->second.y2, black);
				this->vFont->DrawText(render, menu->first, sx, sy + maxHeight, white);
			}
			sy += maxHeight + border * 3; // two for this item + one for the top of the next.
			idx++;
		}
		/*
		this->vFont->DrawText(render, "0123456789", 10, 32, color);
		this->vFont->DrawText(render, "ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 10, 52, color);
		this->vFont->DrawText(render, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", 10, 92, color, 1.0, 2.0);
		*/
	}

	void SceneMenu::GetScreenSize(int* screenWidth, int* screenHeight) 
	{ 
		*screenWidth = this->screenWidth; 
		*screenHeight = this->screenHeight; 
	}

	void SceneMenu::JoystickButtonDown(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->oldJoyA = this->joyA;
				this->joyA = true;
			}
			else if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->oldJoyUp = this->joyUp;
				this->joyUp = true;
			}
			else if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->oldJoyDown = this->joyDown;
				this->joyDown = true;
			}
		}
	}

	void SceneMenu::JoystickButtonUp(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->oldJoyA = this->joyA;
				this->joyA = false;
				if (this->oldJoyA != this->joyA)
				{
					this->MenuSelect(this->menuText->at(this->menuIndex).first);
				}
			}
			else if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->oldJoyUp = this->joyUp;
				this->joyUp = false;
				if (this->oldJoyUp != this->joyUp)
				{
					this->menuIndex--;
					if (this->menuIndex < 0)
						this->menuIndex = 0;
				}
			}
			else if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->oldJoyDown = this->joyDown;
				this->joyDown = false;
				if (this->oldJoyDown != this->joyDown)
				{
					this->menuIndex++;
					if (this->menuIndex >= this->menuText->size())
						this->menuIndex = this->menuText->size() - 1;
				}
			}

		}
		
	}

	void SceneMenu::JoystickMove(int id, int dx, int dy)
	{
		if (this->joyMoveTimeout <= 0.0)
		{
			if (dy != 0)
			{
				if (dy < 0)
				{
					this->menuIndex--;
					if (this->menuIndex < 0)
						this->menuIndex = 0;
				}
				else if (dy > 0)
				{
					this->menuIndex++;
					if (this->menuIndex >= this->menuText->size())
						this->menuIndex = this->menuText->size() - 1;
				}
				this->joyMoveTimeout = JOY_MOVE_DELAY;
			}
		}
	}

	void SceneMenu::KeyDown(uint8_t key)
	{
		// Nothing yet
	}

	void SceneMenu::KeyUp(uint8_t key)
	{
		if (key == jam::key::KEY_UP)
		{
			this->menuIndex--;
			if (this->menuIndex < 0)
				this->menuIndex = 0;

		}
		else if (key == jam::key::KEY_DOWN)
		{
			this->menuIndex++;
			if (this->menuIndex >= this->menuText->size())
			{
				this->menuIndex = this->menuText->size() - 1;
			}
		}
		else if (key == jam::key::KEY_ENTER || key == jam::key::KEY_SPACE || key == jam::key::KEY_RETURN)
		{
			std::string menuItem = this->menuText->at(this->menuIndex).first;
			this->MenuSelect(menuItem);
		}
	}

	void SceneMenu::MenuSelect(std::string menuItem)
	{
		this->click->Play();
		if (menuItem == "PLAY")
		{
			this->nextScene = jam::SceneManager::Instance()->GetScene("game");
			if (this->nextScene != nullptr)
				this->nextScene->Construct(this->screenWidth, this->screenHeight);
			this->bgMusic->Stop();
		}
		else if (menuItem == "HIGH SCORES")
		{
		}
		else if (menuItem == "EXIT")
		{
			this->nextScene = jam::SceneManager::Instance()->GetScene("exit");
			if (this->nextScene != nullptr)
				this->nextScene->Construct(this->screenWidth, this->screenHeight);
		}
	}

	void SceneMenu::MouseMove(int x, int y)
	{

	}

	void SceneMenu::MouseClick(jam::MouseButton button, int x, int y)
	{
		if (button == jam::MouseButton::LEFT)
		{
			int index = 0;
			for(std::vector<std::pair<std::string, jam::Rect>>::iterator item = this->menuText->begin(); item != this->menuText->end(); item++)
			{
				jam::Rect r = (*item).second;
				if (x > r.x1 && x <= r.x2 && y >= r.y1 && y <= r.y2)
				{
					this->menuIndex = index;
					this->MenuSelect((*item).first);
				}
				index++;
			}
		}

	}

	jam::IScene* SceneMenu::NextScene()
	{
		return this->nextScene;
	}
	
	void SceneMenu::Update(float dt)
	{
		const float starSpeed = 10.0;
		float x, y, z;
		if (this->joyMoveTimeout > 0)
		{
			this->joyMoveTimeout -= dt;
			if (this->joyMoveTimeout < 0.0)
				this->joyMoveTimeout = 0.0;
		}

		
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