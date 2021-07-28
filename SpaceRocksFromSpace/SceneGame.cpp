#include "SceneGame.h"

namespace game
{
	#define SHOT_DELAY  0.2

	SceneGame::SceneGame()
	{
		this->ship = nullptr;
		this->nextScene = nullptr;
		this->screenWidth = this->screenHeight = 0;
		this->shotWait = 0;
		this->vFont = new VectorFont();
	}

	SceneGame::~SceneGame()
	{
		this->ClearObjects();
	}

	void SceneGame::ClearObjects()
	{
		for (std::vector<game::Shot*>::iterator iter = this->shots.begin(); iter != this->shots.end(); iter++)
		{
			delete (*iter);
			*iter = nullptr;
		}
		this->shots.clear();

		for (std::vector<game::Rock*>::iterator iter = this->rocks.begin(); iter != this->rocks.end(); iter++)
		{
			delete (*iter);
			*iter = nullptr;
		}
		this->rocks.clear();
	}

	void SceneGame::Construct(int screenWidth, int screenHeight)
	{
		const int COUNT_ROCKS = 10;
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->joyA = this->joyDown = this->joyLeft = this->joyRight = this->joyUp = false;
		this->keyA = this->keyDown = this->keyLeft = this->keyRight = this->keyUp = false;
		this->ClearObjects();
		for (int i = 0; i < COUNT_ROCKS; i++)
		{
			game::Rock* rock = new Rock();
			this->rocks.push_back(rock);
		}
		if (this->ship != nullptr)
		{
			delete this->ship;
			this->ship = nullptr;
		}
		this->ship = new Ship();
		this->ship->Construct(this->screenWidth, this->screenHeight);
		this->nextScene = (IScene*)this;
	}

	void SceneGame::Draw(jam::IRenderer* render)
	{
		int width, height;
		jam::rgb bg(0, 0, 0, 255);
		render->Clear(bg);
		render->GetScreenSize(&width, &height);
		this->screenWidth = width;
		this->screenHeight = height;
		for (std::vector<game::Shot*>::iterator iter = this->shots.begin(); iter != this->shots.end(); iter++)
		{
			(*iter)->Draw(render);
		}

		for (std::vector<game::Rock*>::iterator iter = this->rocks.begin(); iter != this->rocks.end(); iter++)
		{
			(*iter)->Draw(render);
		}

		this->ship->Draw(render);
	}

	void SceneGame::GetScreenSize(int* screenWidth, int* screenHeight)
	{
		*screenWidth = this->screenWidth;
		*screenHeight = this->screenHeight;
	}

	void SceneGame::JoystickButtonDown(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->joyA = true;
			}

			if (btn == jam::JoystickButton::DPAD_LEFT)
			{
				this->joyLeft = true;
			}

			if (btn == jam::JoystickButton::DPAD_RIGHT)
			{
				this->joyRight = true;
			}

			if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->joyUp = true;
			}

			if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->joyDown = true;
			}
		}

	}

	void SceneGame::JoystickButtonUp(int id, jam::JoystickButton btn)
	{
		if (id == 0)
		{
			if (btn == jam::JoystickButton::A)
			{
				this->joyA = false;
			}

			if (btn == jam::JoystickButton::DPAD_LEFT)
			{
				this->joyLeft = false;
			}

			if (btn == jam::JoystickButton::DPAD_RIGHT)
			{
				this->joyRight = false;
			}

			if (btn == jam::JoystickButton::DPAD_UP)
			{
				this->joyUp = false;
			}

			if (btn == jam::JoystickButton::DPAD_DOWN)
			{
				this->joyDown = false;
			}
		}
	}

	void SceneGame::JoystickMove(int id, int dx, int dy)
	{
		if (id == 0)
		{
			if (dx < 0)
			{
				this->joyLeft = true;
				this->joyRight = false;
			}
			else if (dx > 0)
			{
				this->joyLeft = false;
				this->joyRight = true;
			}
			else
			{
				this->joyLeft = false;
				this->joyRight = false;
			}

			if (dy < 0)
			{
				this->joyUp = true;
				this->joyDown = false;
			}
			else if (dy > 0)
			{
				this->joyUp = false;
				this->joyDown = true;
			}
			else
			{
				this->joyUp = false;
				this->joyDown = false;
			}
		}
	}

	void SceneGame::KeyDown(uint8_t key)
	{
		if (key == jam::key::KEY_A || key == jam::key::KEY_LEFT)
		{
			this->keyLeft = true;
		}
		if (key == jam::key::KEY_D || key == jam::key::KEY_RIGHT)
		{
			this->keyRight = true;
		}
		if (key == jam::key::KEY_W || key == jam::key::KEY_UP)
		{
			this->keyLeft = true;
		}
		if (key == jam::key::KEY_S || key == jam::key::KEY_DOWN)
		{
			this->keyRight = true;
		}
		if (key == jam::key::KEY_SPACE || key == jam::key::KEY_ENTER)
		{
			this->keyA = true;
		}
	}

	void SceneGame::KeyUp(uint8_t key)
	{
		if (key == jam::key::KEY_A || key == jam::key::KEY_LEFT)
		{
			this->keyLeft = false;
		}
		if (key == jam::key::KEY_D || key == jam::key::KEY_RIGHT)
		{
			this->keyRight = false;
		}
		if (key == jam::key::KEY_W || key == jam::key::KEY_UP)
		{
			this->keyLeft = false;
		}
		if (key == jam::key::KEY_S || key == jam::key::KEY_DOWN)
		{
			this->keyRight = false;
		}
		if (key == jam::key::KEY_SPACE || key == jam::key::KEY_ENTER)
		{
			this->keyA = false;
		}
		if (key == jam::key::KEY_ESCAPE)
		{
			this->nextScene = nullptr;
		}
	}

	void SceneGame::MouseMove(int x, int y)
	{
	}

	void SceneGame::MouseClick(jam::MouseButton button, int x, int y)
	{
		if (button == jam::MouseButton::LEFT)
		{
			// Shoot
			this->Shoot();
		}
	}

	jam::IScene* SceneGame::NextScene()
	{
		return this->nextScene;
	}

	void SceneGame::Shoot()
	{
		if (shotWait <= 0.0)
		{
			float x, y, heading;
			x = y = heading = 0.0;
			this->ship->GetCanonPosition(&x, &y, &heading);
			game::Shot* shot = new game::Shot();
			shot->SetHeading(heading);
			shot->SetPosition(x, y);
			this->shots.push_back(shot);
			this->shotWait = SHOT_DELAY;
		}
	}

	void SceneGame::Update(float dt)
	{
		for (std::vector<game::Shot*>::iterator iter = this->shots.begin(); iter != this->shots.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		for (std::vector<game::Rock*>::iterator iter = this->rocks.begin(); iter != this->rocks.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		if (this->keyLeft || this->joyLeft)
		{
			this->ship->Rotate(-1);
		}
		else if (this->keyRight || this->joyRight)
		{
			this->ship->Rotate(1);
		}
		else
		{
			this->ship->Rotate(0);
		}

		if (this->shots.size() != 0)
		{
			for (int i = this->shots.size() - 1; i >= 0; i--)
			{
				Shot* shot = this->shots[i];
				if (shot->IsDeleted())
				{
					this->shots.erase(this->shots.begin() + i);
					delete shot;
					shot = nullptr;
					i--;
				}
				else
				{
					shot->Update(this, dt);
				}
			}
		}


		if (this->shotWait > 0)
		{
			this->shotWait -= dt;
			if (this->shotWait < 0)
				this->shotWait = 0;
		}
		this->ship->Update(this, dt);
		if ((this->keyA || this->joyA) && this->shotWait <= 0)
		{
			this->Shoot();
		}
	}
}