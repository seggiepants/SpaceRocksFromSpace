#pragma once

#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__
#include <string>
#include <vector>
#include "IScene.h"
#include "Star.h"
#include "VectorFont.h"

namespace game
{
    class SceneMenu : public jam::IScene //jam::IScene
    {
    public:
        SceneMenu();
        ~SceneMenu();
        void Construct(int screenWidth, int screenHeight);
        void Draw(jam::IRenderer* render);
        void Update(float dt);
    protected:
        VectorFont* vFont;
        std::vector<std::string>* menuText;
        std::vector<game::Star>* stars;
        int screenWidth;
        int screenHeight;
        float maxStarDepth;
    };
}

#endif