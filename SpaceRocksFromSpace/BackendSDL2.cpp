#include "BackendSDL2.h"
#include <iostream>
#include "RendererSDL2.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace jam
{
    BackendSDL2::BackendSDL2()
    {
        this->currentScene = nullptr;
        this->window = nullptr;
        this->renderer = nullptr;

    }

    BackendSDL2::~BackendSDL2()
    {
        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }

        if (this->renderer != nullptr)
        {
            SDL_DestroyRenderer(this->renderer);
            this->renderer = nullptr;
        }

        if (this->window != nullptr)
        {     
            SDL_DestroyWindow(this->window);
            this->window = nullptr;
        }

        //Quit SDL subsystems
        SDL_Quit();
    }

    bool BackendSDL2::Construct(std::string title, int screenWidth, int screenHeight)
    {
		//Initialization flag
		bool success = true;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
		{
			std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Create window
			this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
			if (this->window == NULL)
			{
				std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
				SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);

				/* Clear the entire screen to our selected color. */
				SDL_RenderClear(this->renderer);
			}

			if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) < 0) {
				std::cout << "Sound mixer could not be initialized: \"" << SDL_GetError() << "\"" << std::endl;
				success = false;
			}

			if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0) {
				std::cout << "Sound mixer could not be opened: \"" << SDL_GetError() << "\"" << std::endl;
				success = false;
			}


			if (TTF_Init() < 0) {
				std::cout << "Font system could not be initialized : \"" << TTF_GetError() << "\"" << std::endl;
				success = false;
			}
		}

        this->render = nullptr;

		return success;
    }

    void BackendSDL2::Start(IScene* scene)
    {
        Uint64 previous, current;

        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }
        RendererSDL2* render = new RendererSDL2();
        render->Init(this->window, this->renderer);
        this->render = (IRenderer*)render;

        this->currentScene = scene;
        int screenWidth, screenHeight;
        this->render->GetScreenSize(&screenWidth, &screenHeight);
        this->currentScene->Construct(screenWidth, screenHeight);

        current = SDL_GetPerformanceCounter();
        while (this->currentScene != nullptr) {
            previous = current;
            current = SDL_GetPerformanceCounter();
            Uint64 ticks = current - previous;
            float dt = (float)ticks / (float)SDL_GetPerformanceFrequency();
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    this->currentScene = nullptr;
                }
            }
            if (this->currentScene != nullptr)
            {
                this->currentScene->Update(dt);
                this->currentScene->Draw(this->render);
                /*
                if (!globals.currentScene->running()) {
                    globals.state = globals.currentScene->getNextState();
                    NextScene();
                }
                */
                SDL_RenderPresent(this->renderer);
            }
        }
    }

    bool BackendSDL2::OnUserCreate()
    {
        return true;
    }

    bool BackendSDL2::OnUserUpdate(float dt)
    {
        if (this->currentScene != nullptr)
        {
            this->currentScene->Update(dt);
            if (this->render != nullptr)
            {
                this->currentScene->Draw((jam::IRenderer*)this->render);
            }
        }
        return true;
    }

}