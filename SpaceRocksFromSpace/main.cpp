#include <cstdlib>
#include <time.h>
#include "Shared.h"

#define BACKEND_PGE 0
#define BACKEND_SDL2 1

#define BACKEND BACKEND_PGE

#if BACKEND == BACKEND_PGE
#include "BackendPGE.h"
#endif

#if BACKEND == BACKEND_SDL2
#include "BackendSDL2.h"
#endif

#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneHighScoreEntry.h"
#include "SceneMenu.h"

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
        if (game.Construct("Space Rocks From Space", SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            game::SceneGame* sceneGame = new game::SceneGame();
            game::SceneHighScoreEntry* sceneHighScoreEntry = new game::SceneHighScoreEntry();
            game::SceneMenu* sceneMenu = new game::SceneMenu();
            jam::SceneManager::Instance()->AddScene("menu", sceneMenu);
            jam::SceneManager::Instance()->AddScene("game", sceneGame);
            jam::SceneManager::Instance()->AddScene("highScoreEntry", sceneHighScoreEntry);
            jam::SceneManager::Instance()->AddScene("exit", nullptr);

            sceneHighScoreEntry->Construct(SCREEN_WIDTH, SCREEN_HEIGHT);
            game.Start(jam::SceneManager::Instance()->GetScene("highScoreEntry")); // menu"));
        }
        return 0;
    }
