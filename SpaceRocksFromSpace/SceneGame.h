#pragma once
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__
#include <vector>
#include "IAudio.h"
#include "IScene.h"
#include "Particle.h"
#include "Point2D.h"
#include "Ship.h"
#include "Shot.h"
#include "Rock.h"
#include "VectorFont.h"
namespace game
{

    enum class GameState
    {
        PLAY = 0
        , PAUSE
        , NEXT_LEVEL
        , GAME_OVER
    };

    class SceneGame :
        public jam::IScene
    {
    public:
        SceneGame();
        ~SceneGame();
        void Construct(int screenWidth, int screenHeight);
        void Draw(jam::IRenderer* render);
        void GameOver();
        void GetScreenSize(int* screenWidth, int* screenHeight);
        void JoystickButtonDown(int id, jam::JoystickButton btn);
        void JoystickButtonUp(int id, jam::JoystickButton btn);
        void JoystickMove(int id, int dx, int dy);
        void KeyDown(uint8_t key);
        void KeyUp(uint8_t key);
        void MouseMove(int x, int y);
        void MouseClick(jam::MouseButton button, int x, int y);
        void NextLevel();
        jam::IScene* NextScene();
        void ReturnToMenu();
        void TogglePause();
        void Update(float dt);
    protected:
        void ClearObjects();
        void Shoot();
        VectorFont* vFont;
        int screenWidth;
        int screenHeight;
        float gameTime;
        IScene* nextScene;
        std::vector<game::Rock*> rocks;
        std::vector<game::Shot*> shots;
        std::vector<jam::Point2Df> lifeIcon;
        std::vector<game::Particle*> particles;
        game::Ship* ship;
        float shotWait;
        int score;
        bool joyA, joyUp, joyDown, joyLeft, joyRight;
        bool keyA, keyUp, keyDown, keyLeft, keyRight;
        GameState gameState;
        float messageY, messageTargetY, messageTimeout;
        std::string message;
        int level;
    };

}
#endif