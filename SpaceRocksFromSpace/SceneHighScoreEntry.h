#pragma once
#ifndef __SCENE_HIGH_SCORE_ENTRY_H__
#define __SCENE_HIGH_SCORE_ENTRY_H__
#include <string>
#include "IScene.h"
#include "3rdParty/json/json.hpp"

namespace game
{
    class SceneHighScoreEntry :
        public jam::IScene
    {
    public:
        SceneHighScoreEntry();
        ~SceneHighScoreEntry();
        void Construct(int screenWidth, int screenHeight);
        bool IsNewHighScore(float gameTime, int score, int level);
        void Draw(jam::IRenderer*);
        void GetScreenSize(int* screenWidth, int* screenHeight);
        void Update(float dt);
        void JoystickButtonDown(int id, jam::JoystickButton btn);
        void JoystickButtonUp(int id, jam::JoystickButton btn);
        void JoystickMove(int id, int dx, int dy);
        void KeyDown(uint8_t key);
        void KeyUp(uint8_t key);
        void MouseMove(int x, int y);
        void MouseClick(jam::MouseButton button, int x, int y);
        jam::IScene* NextScene();
    private:
        void NextChar();
        void PrevChar();
        void SetChar(char ch);
        void MoveNext();
        void MovePrev();
        void SaveInitials();
        int screenWidth, screenHeight;
        jam::IScene* nextScene;
        std::string entryCharacters;
        std::string initials;
        int charIndex, joyDX, joyDY;
        float gameTime;
        int level;
        int score;
    };

}

#endif