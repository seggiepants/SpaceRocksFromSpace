#pragma once
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__
#include <vector>
#include "IAudio.h"
#include "IScene.h"
#include "Point2D.h"
#include "Ship.h"
#include "Shot.h"
#include "Rock.h"
#include "VectorFont.h"
namespace game
{
class SceneGame :
    public jam::IScene
{
public:
    SceneGame();
    ~SceneGame();
    void Construct(int screenWidth, int screenHeight);
    void Draw(jam::IRenderer* render);
    void GetScreenSize(int* screenWidth, int* screenHeight);
    void JoystickButtonDown(int id, jam::JoystickButton btn);
    void JoystickButtonUp(int id, jam::JoystickButton btn);
    void JoystickMove(int id, int dx, int dy);
    void KeyDown(uint8_t key);
    void KeyUp(uint8_t key);
    void MouseMove(int x, int y);
    void MouseClick(jam::MouseButton button, int x, int y);
    jam::IScene* NextScene();
    void Update(float dt);
protected:
    void ClearObjects();
    void Shoot();
    VectorFont* vFont;
    int screenWidth;
    int screenHeight;
    IScene* nextScene;
    std::vector<game::Rock*> rocks;
    std::vector<game::Shot*> shots;
    std::vector<jam::Point2Df> lifeIcon;
    game::Ship* ship;
    float shotWait;
    int score;
    bool joyA, joyUp, joyDown, joyLeft, joyRight;
    bool keyA, keyUp, keyDown, keyLeft, keyRight;
    jam::IAudio* zap;
    jam::IAudio* teleport;
    jam::IAudio* explosion;
};

}
#endif