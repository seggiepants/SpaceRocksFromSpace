#include <iomanip>
#include <sstream>
#include "SceneHighScoreList.h"
#include "../../jam/BitmapFont.h"
#include "../../jam/Configuration.h"
#include "../GameAssets.h"
#include "../../jam/SceneManager.h"
#include "../VectorFont.h"
#ifdef KIOSK_MODE
#include "../KioskHelper.h"
#endif

namespace game
{
    SceneHighScoreList::SceneHighScoreList()
    {
    }

    SceneHighScoreList::~SceneHighScoreList()
    {
    }

    void SceneHighScoreList::Construct(int screenWidth, int screenHeight)
    {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->nextScene = this;
#ifdef KIOSK_MODE
        this->highScores = nlohmann::json::parse(KioskHelper::Instance()->GetHighScores());
#else
        std::string filePath = jam::Configuration::GetDataPath();
        filePath = jam::Configuration::PathJoin(filePath, game::HIGHSCORE_FILENAME);
        this->highScores = jam::Configuration::LoadJsonFile(filePath);
#endif

    }

    void SceneHighScoreList::Draw(jam::IRenderer* render)
    {
        const float BORDER = 8.0;
        const std::string BUTTON_MSG = "BACK";
        int x, y, w, h;

        game::VectorFont* vFont1 = static_cast<game::VectorFont*>(jam::backEnd->ResourceManager()->GetFont("vfont1"));
        //jam::BitmapFont* vFont1 = static_cast<jam::BitmapFont*>(jam::backEnd->ResourceManager()->GetFont("nova16pt"));
        game::VectorFont* vFont2 = static_cast<game::VectorFont*>(jam::backEnd->ResourceManager()->GetFont("vfont2"));
        //jam::IFont* vFont2 = jam::backEnd->ResourceManager()->GetFont("Kenney32pt");

        render->GetScreenSize(&this->screenWidth, &this->screenHeight);
        vFont1->MeasureText(BUTTON_MSG, &w, &h);
        this->button.x = this->screenWidth - (3 * BORDER) - w;
        this->button.w = w + (2 * BORDER);
        this->button.y = this->screenHeight - (3 * BORDER) - h;
        this->button.h = h + (2 * BORDER);

        jam::rgb white = { 255, 255, 255, 255 };
        jam::rgb black = { 0, 0, 0, 255 };
        render->Clear(black);
        std::string message;
        message = "HIGH SCORE";
        vFont2->MeasureText(message, &w, &h);
        y = BORDER + h;
        x = (this->screenWidth - w) / 2;
        if (x < 0) x = 0;
        vFont2->DrawText(render, message, x, y, white);
        y += BORDER * 2;
        x = BORDER;
        render->DrawLine(x, y, this->screenWidth - (2 * BORDER), y, white);
        y += BORDER / 2;
        int xInitials, xScore, xLevel, xTime, xStep;
        vFont1->MeasureText("M", &w, &h);
        xStep = (this->screenWidth - (2 * BORDER) - (4 * w)) / 4;
        xInitials = BORDER + (4 * w);
        xScore = xInitials + xStep;
        xLevel = xScore + xStep;
        xTime = xLevel + xStep;
        vFont1->DrawText(render, "NAME", xInitials, y + h, white);
        vFont1->DrawText(render, "SCORE", xScore, y + h, white);
        vFont1->DrawText(render, "LEVEL", xLevel, y + h, white);
        vFont1->DrawText(render, "TIME", xTime, y + h, white);
        y += h + BORDER / 2;
        render->DrawLine(x, y, this->screenWidth - (2 * BORDER), y, white);
        y += BORDER / 2;
        int index = 0;

        array_list* scores = json_object_get_array(json_object_object_get(this->highScores, "scores"));
        for (size_t i = 0; i < array_list_length(scores); i++)
        {
            json_object* entry = (json_object*)array_list_get_idx(scores, i);
            index++;
            std::ostringstream ss;
            ss << std::setfill('0') << std::setw(2) << index << ".";
            vFont1->DrawText(render, ss.str(), BORDER, y + h, white); // Index

            std::string temp;
            const char* initials = json_object_get_string(json_object_object_get(entry, "initials"));
            temp = initials;
            vFont1->DrawText(render, temp, xInitials, y + h, white); // Initials

            ss.str("");
            int score = json_object_get_int(json_object_object_get(entry, "score"));
            ss << score;
            vFont1->DrawText(render, ss.str(), xScore, y + h, white); // Score

            ss.str("");
            int level = json_object_get_int(json_object_object_get(entry, "level"));
            ss << level;
            vFont1->DrawText(render, ss.str(), xLevel, y + h, white); // Level

            double gameTime = json_object_get_double(json_object_object_get(entry, "gameTime"));
            int seconds = (int)ceil(gameTime);
            int minutes = (seconds - (seconds % 60)) / 60;
            seconds = seconds % 60;
            ss.str("");
            ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
            vFont1->DrawText(render, ss.str(), xTime, y + h, white); // Game Time

            y += h + BORDER;
        }
        render->FillRect(this->button.x, this->button.y, this->button.x + this->button.w, this->button.y + this->button.h, white);
        vFont1->DrawText(render, BUTTON_MSG, this->button.x + BORDER, this->button.y + this->button.h - BORDER, black);
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
        if (btn == jam::JoystickButton::A || btn == jam::JoystickButton::X || btn == jam::JoystickButton::B)
        {
            this->ReturnToMenu();
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
            this->ReturnToMenu();
        }
    }

    void SceneHighScoreList::MouseMove(int x, int y)
    {

    }

    void SceneHighScoreList::MouseClick(jam::MouseButton button, int x, int y)
    {
        if (button == jam::MouseButton::LEFT)
        {
            if (x >= this->button.x && x < this->button.x + this->button.w && y >= this->button.y && y < this->button.y + this->button.h)
            {
                this->ReturnToMenu();
            }
        }
    }

    jam::IScene* SceneHighScoreList::NextScene()
    {
        return this->nextScene;
    }

    void SceneHighScoreList::ReturnToMenu()
    {
        IScene* next = jam::SceneManager::Instance()->GetScene("menu");
        if (next != nullptr)
        {
            jam::backEnd->ResourceManager()->GetAudio(game::SOUND_SELECT)->Play();
            next->Construct(this->screenWidth, this->screenHeight);
            this->nextScene = next;
        }
    }
}
