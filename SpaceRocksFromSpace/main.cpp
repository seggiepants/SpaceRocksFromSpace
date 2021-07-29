#include <cstdlib>
#include <time.h>

#define BACKEND_PGE 0
#define BACKEND_SDL2 1

#define BACKEND BACKEND_SDL2

#if BACKEND == BACKEND_PGE
#include "BackendPGE.h"
#endif

#if BACKEND == BACKEND_SDL2
#include "BackendSDL2.h"
#endif

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
        std::srand((unsigned int)time(nullptr));
        if (game.Construct("Space Rocks From Space", SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            game::SceneMenu* sceneMenu = new game::SceneMenu();
            game.Start(sceneMenu);
            delete sceneMenu;
        }
        return 0;
    }
