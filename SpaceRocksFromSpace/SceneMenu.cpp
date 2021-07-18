#include <cstdlib>
#include "SceneMenu.h"

namespace game
{
	SceneMenu::SceneMenu()  
	{

	}

	SceneMenu::~SceneMenu() 
	{
	}

	void SceneMenu::Draw(jam::IRenderer* render)
	{
		jam::rgb bg(0, 0, 128, 255);
		render->Clear(bg);
		int x1, y1, x2, y2;
		int width, height;
		jam::rgb color;

		render->GetScreenSize(&width, &height);

		for (int i = 0; i < 100; i++)
		{
			color.r = rand() % 255;
			color.g = rand() % 255;
			color.b = rand() % 255;
			color.a = 255;

			x1 = rand() % width;
			y1 = rand() % height;
			x2 = rand() % width;
			y2 = rand() % height;

			render->DrawLine(x1, y1, x2, y2, color);
		}
	}
	
	void SceneMenu::Update(float dt)
	{
	}
}