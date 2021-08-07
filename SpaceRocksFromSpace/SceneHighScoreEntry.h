#pragma once
#ifndef __SCENE_HIGH_SCORE_ENTRY_H__
#define __SCENE_HIGH_SCORE_ENTRY_H__
#include <string>
#include "IScene.h"
#include "VectorFont.h"

namespace game
{
    class SceneHighScoreEntry :
        public jam::IScene
    {
    public:
        SceneHighScoreEntry();
        ~SceneHighScoreEntry();
        void Construct(int screenWidth, int screenHeight);
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
        game::VectorFont* vFont;
        int screenWidth, screenHeight;
        jam::IScene* nextScene;
        std::string entryCharacters;
        std::string initials;
        int charIndex, joyDX, joyDY;
    };

}

#endif