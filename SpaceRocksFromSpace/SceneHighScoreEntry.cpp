#include "Configuration.h"
#include "SceneHighScoreEntry.h"
#include "SceneManager.h"
#include "Shared.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace game
{
	const int MAX_SCORES = 10;
	const std::string HIGHSCORE_FILENAME = "highscore.json";
		
	SceneHighScoreEntry::SceneHighScoreEntry()
	{
		this->vFont = new VectorFont();
		this->entryCharacters = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		//std::cout << "App Path: " << jam::Configuration::GetAppPath() << std::endl;
		//std::cout << "Data Path: " << jam::Configuration::GetDataPath() << std::endl;
	}

	SceneHighScoreEntry::~SceneHighScoreEntry()
	{
		delete this->vFont;
		this->vFont = nullptr;
	}

	void SceneHighScoreEntry::Construct(int screenWidth, int screenHeight)
	{
		this->initials = "   ";
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->nextScene = this;
		this->charIndex = 0;
		this->joyDX = this->joyDY = 0;
	}

	bool SceneHighScoreEntry::IsNewHighScore(float gameTime, int score, int level)
	{
		std::filesystem::path  filePath = std::filesystem::path(jam::Configuration::GetDataPath());
		filePath /= HIGHSCORE_FILENAME;

		this->gameTime = gameTime;
		this->score = score;
		this->level = level;
		nlohmann::json ret = LoadHighScore(filePath.string());
		if (ret == nullptr)
		{
			return true; // No file then yes, new high score.
		}
		int scoreCount = ret["scores"].size();
		if (scoreCount < MAX_SCORES)
		{
			return true; // Room for a new one.
		}
		for (auto& item : ret["scores"])
		{
			if (item["score"].get<int>() < this->score)
			{
				// New high score.
				return true;
			}
		}
		return false;
	}

	void SceneHighScoreEntry::Draw(jam::IRenderer* render)
	{
		const float BORDER = 8.0;
		const float SCALE = 2.0;
		int x, y, w, h, charW, charH;
		std::vector<jam::Point2Df> triangleUp;
		std::vector<jam::Point2Df> triangleDown;


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

		triangleUp.push_back({ 0.0, 0.0 });
		triangleUp.push_back({ (float)charW, 0.0 });
		triangleUp.push_back({ (float)(charW / 2.0), (float)(charW / 2.0) });

		triangleDown.push_back({ (float)(charW / 2.0), 0.0 });
		triangleDown.push_back({ 0.0, (float)(charW / 2.0) });
		triangleDown.push_back({ (float)charW, (float)(charW / 2.0) });

		int offset = (this->screenWidth - (this->initials.size() * charW * 1.5)) / 2;
		y += BORDER + h;
		render->DrawPolygon(offset + (1.5 * this->charIndex * charW), y, &triangleUp, white);
		y += (charW / 2) + BORDER;
		std::string temp;
		for (int i = 0; i < initials.size(); i++)
		{
			int x1 = offset + (1.5 * i * charW);
			temp = initials[i];
			this->vFont->DrawText(render, temp, x1, y + charH, white, SCALE, SCALE);
			render->DrawLine(x1, y + charH + BORDER, x1 + charW, y + charH + BORDER, white);
		}
		y += charH + (BORDER * 2);
		render->DrawPolygon(offset + (1.5 * this->charIndex * charW), y, &triangleDown, white);
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
		if (btn == jam::JoystickButton::A)
		{
			// Accept and next or done.
			if (this->charIndex >= this->initials.size() - 1)
			{
				this->SaveInitials();
			}
			else
			{
				this->MoveNext();
			}
		}
		else if (btn == jam::JoystickButton::DPAD_LEFT)
		{
			// Move the cursor back if index > 0
			this->MovePrev();
		}
		else if (btn == jam::JoystickButton::DPAD_RIGHT)
		{
			// Change character at current position to previous.
			this->MoveNext();
		}
		else if (btn == jam::JoystickButton::DPAD_UP)
		{
			// Change character at current position to next.
			this->PrevChar();
		}
		else if (btn == jam::JoystickButton::DPAD_DOWN)
		{
			// Move the cursor forward if index < initials.size() - 1
			this->NextChar();
		}

		else if (btn == jam::JoystickButton::START)
		{
			// Accept string as-is and save.
			this->SaveInitials();
		}
	}

	void SceneHighScoreEntry::JoystickMove(int id, int dx, int dy)
	{
		if (dx == 0 && this->joyDX < 0)
		{
			// Move Left if index >= 0
			this->MovePrev();
		}
		else if (dx == 0 && this->joyDX > 0)
		{
			// Move Right if index >= 0
			this->MoveNext();
		}
		
		if (dy == 0 && this->joyDY < 0)
		{
			// Change current index to previous character
			this->PrevChar();
		}
		else if (dy == 0 && this->joyDY > 0)
		{
			// Change current index to next character
			this->NextChar();
		}

		this->joyDX = dx;
		this->joyDY = dy;
	}

	void SceneHighScoreEntry::KeyDown(uint8_t key)
	{

	}

	void SceneHighScoreEntry::KeyUp(uint8_t key)
	{
		if (key == jam::key::KEY_ENTER || key == jam::key::KEY_RETURN)
		{
			// Accept and next or done.
			if (this->charIndex >= this->initials.size() - 1)
			{
				this->SaveInitials();
			}
			else
			{
				this->NextChar();
			}
		}
		else if (key == jam::key::KEY_LEFT)
		{
			// Move the cursor back if index > 0
			this->MovePrev();
		}
		else if (key == jam::key::KEY_RIGHT)
		{
			// Move the cursor forward if index < initials.size() - 1
			this->MoveNext();
		}
		else if (key == jam::key::KEY_UP)
		{
			// Change character at current position to previous.
			this->PrevChar();
		}
		else if (key == jam::key::KEY_DOWN)
		{
			// Change character at current position to next.
			this->NextChar();
		}
		else if (key == jam::key::KEY_ESCAPE)
		{
			// Accept string as-is and save.
			this->SaveInitials();
		}
		else if (key == jam::key::KEY_0)
		{
			this->SetChar('0');
		}
		else if (key >= jam::key::KEY_1 && key <= jam::key::KEY_9)
		{
			this->SetChar((char)(key - jam::key::KEY_1) + (int) '1');
		}
		else if (key >= jam::key::KEY_A && key <= jam::key::KEY_Z)
		{
			this->SetChar((char)(key - jam::key::KEY_A) + (int) 'A');
		}
		else if (key == jam::key::KEY_SPACE)
		{
			this->SetChar(' ');
		}

	}

	void SceneHighScoreEntry::MouseMove(int x, int y)
	{

	}

	void SceneHighScoreEntry::MouseClick(jam::MouseButton button, int x, int y)
	{

	}

	void SceneHighScoreEntry::MoveNext()
	{
		this->charIndex++;
		if (this->charIndex >= this->initials.size())
			this->charIndex = this->initials.size() - 1;
	}

	void SceneHighScoreEntry::MovePrev()
	{
		this->charIndex--;
		if (this->charIndex < 0)
			this->charIndex = 0;
	}

	void SceneHighScoreEntry::NextChar()
	{
		if (this->charIndex >= 0 && this->charIndex < this->initials.size())
		{
			char current = this->initials[this->charIndex];
			int pos = this->entryCharacters.find(current, 0);
			if (pos >= 0 && pos < this->entryCharacters.size())
			{
				pos++;
				if (pos >= this->entryCharacters.size())
					pos -= this->entryCharacters.size();
				this->initials[this->charIndex] = this->entryCharacters[pos];
			}
		}
	}

	jam::IScene* SceneHighScoreEntry::NextScene()
	{
		return this->nextScene;
	}

	void SceneHighScoreEntry::PrevChar()
	{
		if (this->charIndex >= 0 && this->charIndex < this->initials.size())
		{
			char current = this->initials[this->charIndex];
			int pos = this->entryCharacters.find(current, 0);
			if (pos >= 0 && pos < this->entryCharacters.size())
			{
				pos--;
				if (pos < 0)
					pos += this->entryCharacters.size();
				this->initials[this->charIndex] = this->entryCharacters[pos];
			}
		}

	}

	nlohmann::json SceneHighScoreEntry::LoadHighScore(std::string fileName)
	{
		std::ostringstream buffer;
		try
		{
			std::ifstream file(fileName);
			if (file.is_open())
			{
				std::string line;
				while (std::getline(file, line))
				{
					buffer << line << std::endl;
				}
				file.close();
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to read high score table " << fileName << std::endl << ex.what() << std::endl;
		}
		try
		{
			return nlohmann::json::parse(buffer.str());
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to parse high score table " << fileName << std::endl << ex.what() << std::endl;
			return nullptr;
		}
	}

	void SceneHighScoreEntry::SaveInitials()
	{
		// Save the data to high score table.
		std::filesystem::path  filePath = std::filesystem::path(jam::Configuration::GetDataPath());
		filePath /= HIGHSCORE_FILENAME;
		nlohmann::json saveData = this->LoadHighScore(filePath.string());
		bool inserted = false;
		int index = 0;
		if (!saveData["scores"].is_array())
		{
			saveData["scores"] = nlohmann::json::array();
		}
		for (auto& score : saveData["scores"])
		{
			if (score["score"].get<int>() < this->score)
			{
				break;
			}
			index++;
		}
		nlohmann::json newRow = nlohmann::json::object();
		newRow["initials"] = this->initials.c_str();
		newRow["score"] = this->score;
		newRow["level"] = this->level;
		newRow["gameTime"] = this->gameTime;

		saveData["scores"].insert(saveData["scores"].begin() + index, newRow);
		if (saveData["scores"].size() > MAX_SCORES)
		{
			saveData["scores"].erase(saveData["scores"].size() - 1);
		}

		try
		{
			std::ofstream file(filePath.string());
			if (file.is_open())
			{
				file << std::setw(4) << saveData << std::endl;
				file.close();
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to read high score table " << filePath.string() << std::endl << ex.what() << std::endl;
		}

		// Change to next screen.
		this->nextScene = jam::SceneManager::Instance()->GetScene("menu");
		this->nextScene->Construct(this->screenWidth, this->screenHeight);
	}

	void SceneHighScoreEntry::SetChar(char ch)
	{
		char tmp = toupper((int)ch);
		if (this->charIndex >= 0 && this->charIndex < this->initials.size())
		{
			int pos = this->entryCharacters.find(tmp, 0);
			if (pos >= 0 && pos < this->entryCharacters.size())
			{
				this->initials[this->charIndex] = this->entryCharacters[pos];
				this->charIndex++;
				if (this->charIndex >= this->initials.size())
					this->charIndex = this->initials.size() - 1;
			}
		}
	}
}