#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include "SceneGame.h"
#include "SceneManager.h"
#include "Shared.h"
#include "Utility.h"

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
		this->zap = nullptr;
		this->teleport = nullptr;
		this->explosion = nullptr;
		this->lifeIcon.push_back({ -6, 6 });
		this->lifeIcon.push_back({ 0, 3 });
		this->lifeIcon.push_back({ 6, 6 });
		this->lifeIcon.push_back({ 0, -6 });
	}

	SceneGame::~SceneGame()
	{
		this->ClearObjects();
		this->lifeIcon.clear();
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
		this->score = 0;
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
		this->zap = jam::backEnd->ResourceManager()->GetAudio("assets/sound/laser.wav");
		this->teleport = jam::backEnd->ResourceManager()->GetAudio("assets/sound/teleport.wav");
		this->explosion = jam::backEnd->ResourceManager()->GetAudio("assets/sound/explosion.wav");

	}

	void SceneGame::Draw(jam::IRenderer* render)
	{
		const int SCORE_RESERVE = 50;
		int width, height;
		jam::rgb bg(0, 0, 0, 255);
		jam::rgb fg(255, 255, 255, 255);
		render->Clear(bg);
		render->GetScreenSize(&width, &height);
		this->screenWidth = width;
		this->screenHeight = height;
		std::ostringstream s;
		s << "SCORE " << std::to_string(this->score);
		std::string scoreDisplay = s.str();
		int tw, th;
		this->vFont->MeasureText(scoreDisplay, &tw, &th);
		this->vFont->DrawText(render, scoreDisplay, 8, 8 + th, fg);
		for (std::vector<game::Shot*>::iterator iter = this->shots.begin(); iter != this->shots.end(); iter++)
		{
			(*iter)->Draw(render);
		}

		int x = th;
		int y = th * 2;
		int px = this->lifeIcon[this->lifeIcon.size() - 1].x;
		int py = this->lifeIcon[this->lifeIcon.size() - 1].y;
		int cx, cy;
		jam::rgb white{ 255, 255, 255, 255 };
		for (int j = 0; j < this->ship->GetLives(); j++)
		{
			for (int i = 0; i < this->lifeIcon.size(); i++)
			{
				cx = this->lifeIcon[i].x;
				cy = this->lifeIcon[i].y;
				render->DrawLine(px + x, py + y, cx + x, cy + y, white);
				px = cx;
				py = cy;
			}
			x += th;
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
			this->nextScene = jam::SceneManager::Instance()->GetScene("menu");
			if (this->nextScene != nullptr)
				this->nextScene->Construct(this->screenWidth, this->screenHeight);
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
			this->zap->Play();
		}
	}

	void SceneGame::Update(float dt)
	{
		const int SPLIT_COUNT = 2;
		for (std::vector<game::Shot*>::iterator iter = this->shots.begin(); iter != this->shots.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		for (std::vector<game::Rock*>::iterator iter = this->rocks.begin(); iter != this->rocks.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		// Shots break rocks
		for (std::vector<game::Shot*>::iterator shotIter = this->shots.begin(); shotIter != this->shots.end(); shotIter++)
		{
			game::Shot* shot = *shotIter;
			if (shot->IsDeleted())
				continue;
			for (std::vector<game::Rock*>::iterator iter = this->rocks.begin(); iter != this->rocks.end(); iter++)
			{
				game::Rock* rock = *iter;
				if (rock->IsDeleted())
					continue;
				float x1, y1, x2, y2;
				shot->GetCollisionLine(&x1, &y1, &x2, &y2);
				if (rock->Collide_Line(x1, y1, x2, y2))
				{
					shot->SetDeleted();
					float scale = rock->GetScale();
					rock->SetDeleted();
					if (scale >= 10.0)
					{
						float minSpeed = rock->GetSpeed() * 1.25;
						for (int j = 0; j < SPLIT_COUNT; j++)
						{
							game::Rock* newRock = new game::Rock();
							float rx, ry;
							rock->GetPosition(&rx, &ry);
							rx += rndf(scale) - (scale / 2.0);
							ry += rndf(scale) - (scale / 2.0);
							newRock->SetPosition(rx, ry);
							newRock->SetScale(scale / 2.0);
							newRock->SetHeading(rndf(M_PI * 2.0));
							float newSpeed = rndf(1.0) / 15.0;
							while (newSpeed < minSpeed)
								newSpeed *= 1.1;
							newRock->SetSpeed(newSpeed);
							this->rocks.push_back(newRock);
						}
						if (scale >= 15.0)
						{
							this->score += 10;
						}
						else
						{
							this->score += 20;
						}
					}
					else
					{
						this->score += 50;
					}
					break;
				}
			}
		}

		// Delete old Rocks.
		for (int i = this->rocks.size() - 1; i >= 0; i--)
		{
			game::Rock* rock = this->rocks[i];
			if (rock->IsDeleted())
			{
				this->rocks.erase(this->rocks.begin() + i);
				delete rock;
			}
		}

		// Rocks hit the ship
		bool hit = false;
		for (std::vector<game::Rock*>::iterator rockIter = this->rocks.begin(); rockIter != this->rocks.end(); rockIter++)
		{
			game::Rock* rock = *rockIter;
			float x1, y1, x2, y2;
			for (int i = 0; i < ship->GetScreenPoints(); i++)
			{
				this->ship->GetScreenLine(i, &x1, &y1, &x2, &y2);
				if (rock->Collide_Line(x1, y1, x2, y2))
				{
					hit = true;
					break;
				}
			}
			if (hit)
				break;
		}
		if (hit)
		{
			this->ship->Hit();
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