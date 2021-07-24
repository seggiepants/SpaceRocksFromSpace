#include "BackendSDL2.h"
#include <iostream>
#include "RendererSDL2.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "KeyCodesSDL2.h"

namespace jam
{
    BackendSDL2::BackendSDL2()
    {
        this->currentScene = nullptr;
        this->window = nullptr;
        this->renderer = nullptr;
        this->mouseX = this->mouseY = 0;
        this->mouseBtnLeft = this->mouseBtnRight = this->oldMouseBtnLeft = this->oldMouseBtnRight = false;
        key::KeyInit_SDL2();        
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

    bool BackendSDL2::IsKeyDown(uint8_t code)
    {
        return this->Key[code] && !this->oldKey[code];
    }

    bool BackendSDL2::IsKeyPressed(uint8_t code)
    {
        return this->Key[code];
    }

    bool BackendSDL2::IsKeyReleased(uint8_t code)
    {
        return !this->Key[code] && this->oldKey[code];
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
            for (int i = 0; i < MAX_KEYS; i++)
            {
                this->oldKey[i] = this->Key[i];
            }
            while (SDL_PollEvent(&e))
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    this->currentScene = nullptr;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    this->Key[e.key.keysym.scancode] = true;
                    if (this->Key[e.key.keysym.scancode] != this->oldKey[e.key.keysym.scancode])
                    {
                        if (this->currentScene != nullptr)
                        {
                            this->currentScene->KeyDown(e.key.keysym.scancode);
                        }
                    }

                }
                else if (e.type == SDL_KEYUP)
                {
                    this->Key[e.key.keysym.scancode] = false;
                    if (this->Key[e.key.keysym.scancode] != this->oldKey[e.key.keysym.scancode])
                    {
                        if (this->currentScene != nullptr)
                        {
                            this->currentScene->KeyUp(e.key.keysym.scancode);
                        }
                    }
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    switch (e.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        this->oldMouseBtnLeft = this->mouseBtnLeft;
                        this->mouseBtnLeft = true;
                        break;
                    case SDL_BUTTON_RIGHT:
                        this->oldMouseBtnRight = this->mouseBtnRight;
                        this->mouseBtnRight = true;
                        break;
                    }
                }
                else if (e.type == SDL_MOUSEBUTTONUP)
                {
                    switch (e.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        this->oldMouseBtnLeft = this->mouseBtnLeft;
                        this->mouseBtnLeft = false;
                        if (this->oldMouseBtnLeft)
                        {
                            if (this->currentScene != nullptr)
                            {
                                this->currentScene->MouseClick(jam::MouseButton::LEFT, this->mouseX, this->mouseY);
                            }
                        }
                        break;
                    case SDL_BUTTON_RIGHT:
                        this->oldMouseBtnRight = this->mouseBtnRight;
                        this->mouseBtnRight = false;
                        if (this->oldMouseBtnRight)
                        {
                            if (this->currentScene != nullptr)
                            {
                                this->currentScene->MouseClick(jam::MouseButton::RIGHT, this->mouseX, this->mouseY);
                            }
                        }
                        break;
                    }
                }
                else if (e.type == SDL_MOUSEMOTION)
                {
                    this->mouseX = e.motion.x;
                    this->mouseY = e.motion.y;
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
                this->currentScene = this->currentScene->NextScene();
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