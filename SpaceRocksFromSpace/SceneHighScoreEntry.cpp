#include "SceneHighScoreEntry.h"
#include "SceneManager.h"
#include "Shared.h"
#include "VectorFont.h"

namespace game
{

	SceneHighScoreEntry::SceneHighScoreEntry()
	{
		this->vFont = new VectorFont();
		this->entryCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	}

	SceneHighScoreEntry::~SceneHighScoreEntry()
	{
		delete this->vFont;
		this->vFont = nullptr;
	}

	void SceneHighScoreEntry::Construct(int screenWidth, int screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->nextScene = this;

	}

	void SceneHighScoreEntry::Draw(jam::IRenderer* render)
	{
		const float BORDER = 8.0;
		const float SCALE = 2.0;
		int x, y, w, h, charW, charH;
		jam::rgb white = { 255, 255, 255, 255 };
		jam::rgb black = { 0, 0, 0, 255 };
		render->Clear(black);
		std::string message;
		message = "NEW HIGH SCORE";
		this->vFont->MeasureText(message, &w, &h, SCALE, SCALE);
		y = BORDER + h;
		x = (this->screenWidth - w) / 2;
		if (x < 0) x = 0;
		this->vFont->DrawText(render, message, x, y, white, SCALE, SCALE);

		message = "ENTER YOUR INITIALS";
		this->vFont->MeasureText(message, &w, &h, SCALE, SCALE);
		y += BORDER + h;
		x = (this->screenWidth - w) / 2;
		if (x < 0) x = 0;
		this->vFont->DrawText(render, message, x, y, white, SCALE, SCALE);

		message = "W";
		this->vFont->MeasureText(message, &charW, &charH, SCALE, SCALE);
	}

	void SceneHighScoreEntry::GetScreenSize(int* screenWidth, int* screenHeight)
	{
		*screenWidth = this->screenWidth;
		*screenHeight = this->screenHeight;
	}

	void SceneHighScoreEntry::Update(float dt)
	{

	}

	void SceneHighScoreEntry::JoystickButtonDown(int id, jam::JoystickButton btn)
	{

	}

	void SceneHighScoreEntry::JoystickButtonUp(int id, jam::JoystickButton btn)
	{

	}

	void SceneHighScoreEntry::JoystickMove(int id, int dx, int dy)
	{

	}

	void SceneHighScoreEntry::KeyDown(uint8_t key)
	{

	}

	void SceneHighScoreEntry::KeyUp(uint8_t key)
	{
		if (key == jam::key::KEY_ESCAPE)
		{
			this->nextScene = jam::SceneManager::Instance()->GetScene("menu");
			this->nextScene->Construct(this->screenWidth, this->screenHeight);
		}
	}

	void SceneHighScoreEntry::MouseMove(int x, int y)
	{

	}

	void SceneHighScoreEntry::MouseClick(jam::MouseButton button, int x, int y)
	{

	}

	jam::IScene* SceneHighScoreEntry::NextScene()
	{
		return this->nextScene;
	}
}