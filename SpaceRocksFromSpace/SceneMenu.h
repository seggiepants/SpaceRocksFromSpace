#pragma once

#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__
#include "IScene.h"

namespace game
{
    class SceneMenu : public jam::IScene //jam::IScene
    {
    public:
        SceneMenu();
        ~SceneMenu();
        void Draw(jam::IRenderer* render);
        void Update(float dt);
    };
}

#endif