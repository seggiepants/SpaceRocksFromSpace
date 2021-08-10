#include <sstream>
#include "SceneHighScoreList.h"
#include "Configuration.h"
#include "GameAssets.h"
#include "SceneManager.h";

namespace game
{
    SceneHighScoreList::SceneHighScoreList()
    {
        this->vFont = new VectorFont();
    }

    SceneHighScoreList::~SceneHighScoreList()
    {
        delete this->vFont;
        this->vFont = nullptr;
    }

    void SceneHighScoreList::Construct(int screenWidth, int screenHeight)
    {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->nextScene = this;
        this->highScores = jam::Configuration::LoadJsonFile(game::HIGHSCORE_FILENAME);

    }

    void SceneHighScoreList::Draw(jam::IRenderer* render)
    {
        const float BORDER = 8.0;
        const float SCALE = 1.0;
        const float SCALE_TITLE = 2.0;
        int x, y, w, h, charW, charH;
        render->GetScreenSize(&this->screenWidth, &this->screenHeight);

        jam::rgb white = { 255, 255, 255, 255 };
        jam::rgb black = { 0, 0, 0, 255 };
        render->Clear(black);
        std::string message;
        message = "HIGH SCORE";
        this->vFont->MeasureText(message, &w, &h, SCALE_TITLE, SCALE_TITLE);
        y = BORDER + h;
        x = (this->screenWidth - w) / 2;
        if (x < 0) x = 0;
        this->vFont->DrawText(render, message, x, y, white, SCALE_TITLE, SCALE_TITLE);
        y += BORDER * 2;
        x = BORDER;
        render->DrawLine(x, y, this->screenWidth - (2 * BORDER), y, white);
        y += BORDER / 2;
        int xInitials, xScore, xLevel, xTime, xStep;
        this->vFont->MeasureText("M", &w, &h, SCALE, SCALE);
        xStep = (this->screenWidth - (2 * BORDER) - (4 * w)) / 4;
        xInitials = BORDER + (4 * w);
        xScore = xInitials + xStep;
        xLevel = xScore + xStep;
        xTime = xLevel + xStep;
        this->vFont->DrawText(render, "NAME", xInitials, y + h, white);
        this->vFont->DrawText(render, "SCORE", xScore, y + h, white);
        this->vFont->DrawText(render, "LEVEL", xLevel, y + h, white);
        this->vFont->DrawText(render, "TIME", xTime, y + h, white);
        y += h + BORDER / 2;
        render->DrawLine(x, y, this->screenWidth - (2 * BORDER), y, white);
        y += BORDER / 2;
        int index = 0;
        for (auto& entry : this->highScores["scores"])
        {
            index++;
            std::ostringstream ss;
            ss << std::setfill('0') << std::setw(2) << index << ".";
            this->vFont->DrawText(render, ss.str(), BORDER, y + h, white, SCALE, SCALE); // Index

            std::string temp;
            temp = entry["initials"];
            this->vFont->DrawText(render, temp, xInitials, y + h, white, SCALE, SCALE); // Initials

            ss.str("");
            ss << entry["score"];
            this->vFont->DrawText(render, ss.str(), xScore, y + h, white, SCALE, SCALE); // Score

            ss.str("");
            ss << entry["level"].get<int>();
            this->vFont->DrawText(render, ss.str(), xLevel, y + h, white, SCALE, SCALE); // Level

            int seconds = (int)std::ceil(entry["gameTime"].get<float>());
            int minutes = (seconds - (seconds % 60)) / 60;
            seconds = seconds % 60;
            ss.str("");
            ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
            this->vFont->DrawText(render, ss.str(), xTime, y + h, white, SCALE, SCALE); // Game Time

            y += h + BORDER;
        }
    }

    void SceneHighScoreList::GetScreenSize(int* screenWidth, int* screenHeight)
    {
        *screenWidth = this->screenWidth;
        *screenHeight = this->screenHeight;
    }

    void SceneHighScoreList::Update(float dt)
    {

    }

    void SceneHighScoreList::JoystickButtonDown(int id, jam::JoystickButton btn)
    {

    }

    void SceneHighScoreList::JoystickButtonUp(int id, jam::JoystickButton btn)
    {
        if (btn == jam::JoystickButton::A)
        {
            IScene* next = jam::SceneManager::Instance()->GetScene("menu");
            if (next != nullptr)
            {
                next->Construct(this->screenWidth, this->screenHeight);
                this->nextScene = next;
            }
        }
    }

    void SceneHighScoreList::JoystickMove(int id, int dx, int dy)
    {

    }

    void SceneHighScoreList::KeyDown(uint8_t key)
    {

    }

    void SceneHighScoreList::KeyUp(uint8_t key)
    {
        if (key == jam::key::KEY_SPACE || key == jam::key::KEY_ENTER || key == jam::key::KEY_RETURN || key == jam::key::KEY_ESCAPE)
        {
            IScene* next = jam::SceneManager::Instance()->GetScene("menu");
            if (next != nullptr)
            {
                next->Construct(this->screenWidth, this->screenHeight);
                this->nextScene = next;
            }
        }
    }

    void SceneHighScoreList::MouseMove(int x, int y)
    {

    }

    void SceneHighScoreList::MouseClick(jam::MouseButton button, int x, int y)
    {

    }

    jam::IScene* SceneHighScoreList::NextScene()
    {
        return this->nextScene;
    }
}
