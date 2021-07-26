#include "SceneGame.h"

namespace game
{
	#define SHOT_DELAY  0.1
	SceneGame::SceneGame()
	{
		this->joyA = this->oldJoyA = this->joyUp = this->oldJoyUp = false;
		this->joyDown = this->oldJoyDown = this->joyLeft = this->oldJoyLeft = false;
		this->joyRight = this->oldJoyRight = false;
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
		this->ClearObjects();
		for (int i = 0; i < COUNT_ROCKS; i++)
		{
			game::Rock* rock = new Rock();
			this->rocks.push_back(rock);
		}
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
	}

	void SceneGame::GetScreenSize(int* screenWidth, int* screenHeight)
	{
		*screenWidth = this->screenWidth;
		*screenHeight = this->screenHeight;
	}

	void SceneGame::JoystickButtonDown(int id, jam::JoystickButton btn)
	{
	}

	void SceneGame::JoystickButtonUp(int id, jam::JoystickButton btn)
	{
	}

	void SceneGame::JoystickMove(int id, int dx, int dy)
	{
	}

	void SceneGame::KeyDown(uint8_t key)
	{
	}

	void SceneGame::KeyUp(uint8_t key)
	{
	}

	void SceneGame::MouseMove(int x, int y)
	{
	}

	void SceneGame::MouseClick(jam::MouseButton button, int x, int y)
	{
	}

	jam::IScene* SceneGame::NextScene()
	{
		return this->nextScene;
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
	}
}