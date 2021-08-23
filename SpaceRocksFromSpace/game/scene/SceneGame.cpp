#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include "../GameAssets.h"
#include "../../jam/IAudio.h"
#include "../../jam/IImage.h"
#include "../../jam/IResourceManager.h"
#include "../../jam/Rect.h"
#include "SceneGame.h"
#include "SceneHighScoreEntry.h"
#include "../../jam/SceneManager.h"
#include "../../jam/Shared.h"
#include "../../Utility.h"

namespace game
{
	#define SHOT_DELAY  0.2
	const int UFO_POINTS = 50;

	SceneGame::SceneGame()
	{
		this->ship = nullptr;
		this->ufo = nullptr;
		this->nextScene = nullptr;
		this->screenWidth = this->screenHeight = 0;
		this->shotWait = 0;
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

		for (std::vector<game::Particle*>::iterator iter = this->particles.begin(); iter != this->particles.end(); iter++)
		{
			delete (*iter);
			*iter = nullptr;
		}
		this->particles.clear();
	}

	void SceneGame::Construct(int screenWidth, int screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->level = 0;
		this->score = 0;
		this->gameTime = 0.0;

		if (!jam::backEnd->ResourceManager()->HasAudio(SOUND_NEXTLEVEL))
		{
			jam::backEnd->ResourceManager()->PreloadAudio(SOUND_NEXTLEVEL);
		}

		if (!jam::backEnd->ResourceManager()->HasAudio(SOUND_GAMEOVER))
		{
			jam::backEnd->ResourceManager()->PreloadAudio(SOUND_GAMEOVER);
		}

		/*
		if (!jam::backEnd->ResourceManager()->HasImage(IMG_PLASMA_BG))
		{
			jam::backEnd->ResourceManager()->PreloadImage(IMG_PLASMA_BG);
		}
		*/

		if (this->ship != nullptr)
		{
			delete this->ship;
			this->ship = nullptr;
		}
		this->ship = new Ship();
		this->ship->Construct(this->screenWidth, this->screenHeight);

		if (this->ufo != nullptr)
		{
			delete this->ufo;
			this->ufo = nullptr;
		}
		this->ufo = new UFO();

		this->nextScene = (IScene*)this;
		
		this->NextLevel();
	}

	void SceneGame::Draw(jam::IRenderer* render)
	{
		const int SCORE_RESERVE = 50;
		const int BORDER = 8.0;

		int width, height;
		jam::rgb bg(0, 0, 0, 255);
		jam::rgb fg(255, 255, 255, 255);

		game::VectorFont* vFont1 = static_cast<game::VectorFont*>(jam::backEnd->ResourceManager()->GetFont("vfont1"));
		game::VectorFont* vFont2 = static_cast<game::VectorFont*>(jam::backEnd->ResourceManager()->GetFont("vfont2"));

		render->Clear(bg);
		//render->DrawImage(jam::backEnd->ResourceManager()->GetImage(IMG_PLASMA_BG), 0, 0);
		render->GetScreenSize(&width, &height);
		this->screenWidth = width;
		this->screenHeight = height;
		std::ostringstream s;
		s << "SCORE " << std::to_string(this->score);
		std::string scoreDisplay = s.str();
		int tw, th;
		vFont1->MeasureText(scoreDisplay, &tw, &th);
		vFont1->DrawText(render, scoreDisplay, BORDER, BORDER + th, fg);
		std::ostringstream lvl;
		lvl << "LEVEL " << std::to_string(this->level);
		std::string levelDisplay = lvl.str();
		vFont1->MeasureText(levelDisplay, &tw, &th);
		vFont1->DrawText(render, levelDisplay,this->screenWidth - tw - BORDER, BORDER + th, fg);

		for (std::vector<game::Particle*>::iterator iter = this->particles.begin(); iter != this->particles.end(); iter++)
		{
			(*iter)->Draw(render);
		}

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

		this->ufo->Draw(render);

		this->ship->Draw(render);

		if (this->gameState == GameState::PAUSE)
		{
			int w, h;
			jam::Rect r;
			std::string pauseMessage = "PAUSE";
			vFont2->MeasureText(pauseMessage, &w, &h);
			int x, y;
			x = (this->screenWidth - w - (BORDER * 2)) / 2;
			if (x < 0)
				x = 0;
			y = (this->screenHeight - h - (BORDER * 2)) / 2;
			if (y < 0)
				y = 0;
			render->FillRect(x, y, x + w + (2 * BORDER), y + h + (2 * BORDER), fg);
			vFont2->DrawText(render, pauseMessage, x + BORDER, y + h + BORDER, bg);
		}
		else if (this->gameState == GameState::GAME_OVER || this->gameState == GameState::NEXT_LEVEL)
		{
			int w, h;
			vFont2->MeasureText(this->message, &w, &h);
			int x = (this->screenWidth - w) / 2;
			render->FillRect(0, this->messageY - h - BORDER, this->screenWidth, this->messageY + BORDER, fg);
			vFont2->DrawText(render, this->message, x, this->messageY, bg);
		}
	}

	void SceneGame::GameOver()
	{
		game::VectorFont* vFont2 = static_cast<game::VectorFont*>(jam::backEnd->ResourceManager()->GetFont("vfont2"));
		this->message = "GAME OVER";
		int w, h;
		vFont2->MeasureText(this->message, &w, &h);
		this->messageTargetY = this->screenHeight / 2;
		this->messageY = this->screenHeight + h;
		this->gameState = game::GameState::GAME_OVER;
		jam::backEnd->ResourceManager()->GetAudio(SOUND_GAMEOVER)->Play();
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

			if (btn == jam::JoystickButton::START)
			{
				this->TogglePause();
			}

			if (btn == jam::JoystickButton::SELECT)
			{
				this->ReturnToMenu();
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
			this->keyUp = true;
		}
		if (key == jam::key::KEY_S || key == jam::key::KEY_DOWN)
		{
			this->keyDown = true;
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
			this->keyUp = false;
		}
		if (key == jam::key::KEY_S || key == jam::key::KEY_DOWN)
		{
			this->keyDown = false;
		}
		if (key == jam::key::KEY_SPACE || key == jam::key::KEY_ENTER)
		{
			this->keyA = false;
		}
		if (key == jam::key::KEY_P)
		{
			this->TogglePause();
		}
		if (key == jam::key::KEY_ESCAPE)
		{
			this->ReturnToMenu();
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

	void SceneGame::NextLevel()
	{
		const int MIN_ROCKS = 2;
		const float MIN_DIST_SQUARED = 1600;
		game::VectorFont* vFont2 = static_cast<game::VectorFont*>(jam::backEnd->ResourceManager()->GetFont("vfont2"));
		this->level++;
		std::ostringstream s;
		s << "LEVEL " << std::to_string(this->level);
		this->message = s.str();
		int w, h;
		vFont2->MeasureText(this->message, &w, &h);
		this->messageTargetY = this->screenHeight / 2;
		this->messageY = this->screenHeight + h;		

		this->joyA = this->joyDown = this->joyLeft = this->joyRight = this->joyUp = false;
		this->keyA = this->keyDown = this->keyLeft = this->keyRight = this->keyUp = false;
		this->ClearObjects();
		for (int i = 0; i < MIN_ROCKS + this->level; i++)
		{
			game::Rock* rock = new Rock();
			rock->SetPosition(rndf(this->screenWidth), rndf(this->screenHeight));
			float dx = this->ship->GetX() - rock->GetX();
			float dy = this->ship->GetY() - rock->GetY();
			while (dx * dx + dy * dy < MIN_DIST_SQUARED)
			{
				rock->SetPosition(rndf(this->screenWidth), rndf(this->screenHeight));
				dx = this->ship->GetX() - rock->GetX();
				dy = this->ship->GetY() - rock->GetY();
			}
			this->rocks.push_back(rock);
		}

		this->ship->SetPosition(this->screenWidth / 2.0, this->screenHeight / 2.0);
		this->ship->SetHeading(0.00);
		
		this->gameState = game::GameState::NEXT_LEVEL;
		jam::backEnd->ResourceManager()->GetAudio(SOUND_NEXTLEVEL)->Play();
	}

	jam::IScene* SceneGame::NextScene()
	{
		return this->nextScene;
	}

	void SceneGame::ReturnToMenu()
	{
		SceneHighScoreEntry* enterHighScore = (game::SceneHighScoreEntry*) jam::SceneManager::Instance()->GetScene("highScoreEntry");
		if (enterHighScore != nullptr)
		{
			enterHighScore->Construct(this->screenWidth, this->screenHeight);
			if (enterHighScore->IsNewHighScore(this->gameTime, this->score, this->level))
			{
				this->nextScene = enterHighScore;
			}
			else
			{
				this->nextScene = jam::SceneManager::Instance()->GetScene("menu");
				if (this->nextScene != nullptr)
					this->nextScene->Construct(this->screenWidth, this->screenHeight);
			}
		}
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
			jam::IAudio* sample = jam::backEnd->ResourceManager()->GetAudio(SOUND_ZAP);
			if (sample != nullptr)
			{
				sample->Play();
			}
		}
	}

	void SceneGame::TogglePause()
	{
		if (this->gameState == GameState::PAUSE || this->gameState == GameState::PLAY)
		{
			if (this->gameState == GameState::PAUSE)
				this->gameState = GameState::PLAY;
			else
				this->gameState = GameState::PAUSE;
			jam::IAudio* sample = jam::backEnd->ResourceManager()->GetAudio(SOUND_PAUSE);
			if (sample != nullptr)
			{
				sample->Play();
			}
		}
	}

	void SceneGame::Update(float dt)
	{
		const int SPLIT_COUNT = 2;
		const float SPLIT_SCALE_1 = 15.0;
		const float SPLIT_SCALE_2 = 25.0;
		const int POINTS_ROCKS_SPLIT_1 = 10;
		const int POINTS_ROCKS_SPLIT_2 = 20;
		const int POINTS_ROCKS_DESTROY = 50;

		const float MESSAGE_SPEED = 200.0;
		const float MESSAGE_TIMEOUT_MAX = 0.5;

		if (this->gameState == game::GameState::PAUSE)
			return;
		else if (this->gameState == game::GameState::NEXT_LEVEL || this->gameState == game::GameState::GAME_OVER)
		{
			if (this->messageY > this->messageTargetY)
			{
				this->messageY -= MESSAGE_SPEED * dt;
				if (this->messageY <= this->messageTargetY)
				{
					this->messageY = this->messageTargetY;
					this->messageTimeout = MESSAGE_TIMEOUT_MAX;
					if (this->gameState == game::GameState::GAME_OVER)
						this->messageTimeout *= 2;
				}
			}
			else
			{
				this->messageTimeout -= dt;
				if (this->messageTimeout <= 0.0)
				{
					this->messageTimeout = 0.0;
					if (this->gameState == game::GameState::NEXT_LEVEL)
					{
						this->gameState = game::GameState::PLAY;
					}
					else if (this->gameState == game::GameState::GAME_OVER)
					{
						this->ReturnToMenu();
					}
				}
			}
			return; 
		}
		this->gameTime += dt;

		for (std::vector<game::Shot*>::iterator iter = this->shots.begin(); iter != this->shots.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		for (std::vector<game::Rock*>::iterator iter = this->rocks.begin(); iter != this->rocks.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		for (std::vector<game::Particle*>::iterator iter = this->particles.begin(); iter != this->particles.end(); iter++)
		{
			(*iter)->Update(this, dt);
		}

		this->ufo->Update(this, dt);

		// Shots break rocks or ufo
		for (std::vector<game::Shot*>::iterator shotIter = this->shots.begin(); shotIter != this->shots.end(); shotIter++)
		{
			game::Shot* shot = *shotIter;
			if (shot->IsDeleted())
				continue;

			float x1, y1, x2, y2;
			shot->GetCollisionLine(&x1, &y1, &x2, &y2);
			if (this->ufo->Collide_Line(x1, y1, x2, y2))
			{
				shot->SetDeleted();
				this->ufo->Hit();
				this->score += UFO_POINTS;
				jam::backEnd->ResourceManager()->GetAudio(game::SOUND_EXPLOSION)->Play();
				continue;
			}
			
			for (std::vector<game::Rock*>::iterator iter = this->rocks.begin(); iter != this->rocks.end(); iter++)
			{
				game::Rock* rock = *iter;
				if (rock->IsDeleted())
					continue;
				if (rock->Collide_Line(x1, y1, x2, y2))
				{
					shot->SetDeleted();
					float scale = rock->GetScale();
					rock->SetDeleted();
					if (scale >= SPLIT_SCALE_1)
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
						if (scale >= SPLIT_SCALE_2)
						{
							this->score += POINTS_ROCKS_SPLIT_1;
						}
						else
						{
							this->score += POINTS_ROCKS_SPLIT_2;
						}
					}
					else
					{
						this->score += POINTS_ROCKS_DESTROY;
					}
					break;
				}
			}
		}

		// Delete old Particles.
		for (int i = this->particles.size() - 1; i >= 0; i--)
		{
			game::Particle* particle= this->particles[i];
			if (particle->IsDeleted())
			{
				this->particles.erase(this->particles.begin() + i);
				delete particle;
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

		// Did the ship and ufo collide?
		if (!hit && !this->ufo->IsDeleted())
		{
			float x1, y1, x2, y2;
			for (int i = 0; i < ship->GetScreenPoints(); i++)
			{
				this->ship->GetScreenLine(i, &x1, &y1, &x2, &y2);
				if (ufo->Collide_Line(x1, y1, x2, y2))
				{
					this->ufo->Hit();
					hit = true;
					break;
				}
			}
		}


		if (hit)
		{
			if (this->ship->Hit())
			{
				const float lifeTime = 0.5; 
				const float speed = 64.0;
				for (float rad = 0.0; rad < M_PI * 2.0; rad += (M_PI / 20.0))
				{
					game::Particle* particle = new game::Particle();
					particle->Construct(this->ship->GetX(), this->ship->GetY(), rad, speed + ((rand() % 100)/5.0), lifeTime);
					this->particles.push_back(particle);
				}
			}
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

		if (this->keyUp || this->joyUp)
		{
			this->ship->Thrust();
			// Add an exhaust particle.
			const float particle_angle = (30.0 * M_PI) / 180.0;
			const float particle_speed = 450.0;
			const float particle_lifetime = 0.05;

			Particle* p = new Particle();
			float x, y, heading;
			ship->GetThrustPosition(&x, &y, &heading);
			heading += rndf(particle_angle) - (particle_angle / 2.0);
			p->Construct(x, y, heading, particle_speed, particle_lifetime);
			this->particles.push_back(p);
		}

		if (this->keyDown || this->joyDown)
		{
			this->ship->Teleport();
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

		if (this->ship->IsDeleted())
		{
			this->GameOver();
		}
		else if (this->rocks.size() == 0)
		{
			this->NextLevel();
		}
	}
}