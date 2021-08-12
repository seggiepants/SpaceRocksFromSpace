#pragma once

#ifndef __SCENE_HIGH_SCORE_LIST_H__
#define __SCENE_HIGH_SCORE_LIST_H__

#include "3rdParty/json/json.hpp"
#include "IScene.h"
#include "Rect.h"

namespace game
{
    class SceneHighScoreList :
        public jam::IScene
    {
    public:
        SceneHighScoreList();
        ~SceneHighScoreList();
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
        IScene* NextScene();
    private:
        void ReturnToMenu();
        IScene* nextScene;
        int screenWidth, screenHeight;
        nlohmann::json highScores;
        jam::Rect button;
    };
}

#endif