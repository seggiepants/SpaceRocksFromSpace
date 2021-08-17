#include <cstdlib>
#include <time.h>
#include "Shared.h"

#define BACKEND_PGE 0
#define BACKEND_SDL2 1

#define BACKEND BACKEND_SDL2

#if BACKEND == BACKEND_PGE
#include "BackendPGE.h"
#endif

#if BACKEND == BACKEND_SDL2
#include "BackendSDL2.h"
#endif

#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneHighScoreEntry.h"
#include "SceneHighScoreList.h"
#include "SceneMenu.h"
#include "BitmapFont.h"
#include "VectorFont.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

    int main(int argc, char** argv)
    {
#if BACKEND == BACKEND_PGE
        jam::BackendPGE game;
#endif
#if BACKEND == BACKEND_SDL2
        jam::BackendSDL2 game;
#endif
        jam::backEnd = &game;
        std::srand((unsigned int)time(nullptr));
        game::VectorFont* font = nullptr;
        game::VectorFont* font2 = nullptr;

        if (game.Construct("Space Rocks From Space", SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            font = new game::VectorFont();
            font->Construct(1.0, 1.0);
            font2 = new game::VectorFont();
            font2->Construct(2.0, 2.0);
            jam::backEnd->ResourceManager()->PreloadFont("vfont1", font);
            jam::backEnd->ResourceManager()->PreloadFont("vfont2", font2);
            jam::BitmapFont* bmpFont = new jam::BitmapFont();
            bmpFont->Construct("assets/font/NovaSquareBoldOblique16pt.json");
            jam::backEnd->ResourceManager()->PreloadFont("nova16pt", bmpFont);

            game::SceneGame* sceneGame = new game::SceneGame();
            game::SceneHighScoreEntry* sceneHighScoreEntry = new game::SceneHighScoreEntry();
            game::SceneHighScoreList* sceneHighScoreList = new game::SceneHighScoreList();
            game::SceneMenu* sceneMenu = new game::SceneMenu();
            jam::SceneManager::Instance()->AddScene("menu", sceneMenu);
            jam::SceneManager::Instance()->AddScene("game", sceneGame);
            jam::SceneManager::Instance()->AddScene("highScoreEntry", sceneHighScoreEntry);
            jam::SceneManager::Instance()->AddScene("highScoreList", sceneHighScoreList);
            jam::SceneManager::Instance()->AddScene("exit", nullptr);

            sceneHighScoreEntry->Construct(SCREEN_WIDTH, SCREEN_HEIGHT);
            game.Start(jam::SceneManager::Instance()->GetScene("menu")); // menu"));
        }
        return 0;
    }
