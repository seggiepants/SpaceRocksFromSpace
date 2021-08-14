#include <iostream>
#include <SDL2\SDL_image.h>>
#include "ImageSDL2.h"

namespace jam
{
	ImageSDL2::ImageSDL2()
	{

	}

	ImageSDL2::~ImageSDL2()
	{

	}

	bool ImageSDL2::Construct(jam::IRenderer* render, std::string fileName)
	{
		//RendererSDL2* renderSDL2 = dynamic_cast<RendererSDL2*>(render);
		return false;
	}

	int ImageSDL2::GetWidth()
	{
		SDL_Rect r;
		if (this->sprite != nullptr)
		{

			SDL_QueryTexture(this->sprite, NULL, NULL, &r.w, &r.h);
			return r.w;
		}
		else
			return 0;
	}

	SDL_Texture* ImageSDL2::GetHandle()
	{
		return this->sprite;
	}

	int ImageSDL2::GetHeight()
	{
		SDL_Rect r;
		if (this->sprite != nullptr)
		{

			SDL_QueryTexture(this->sprite, NULL, NULL, &r.w, &r.h);
			return r.h;
		}
		else
			return 0;
	}

	SDL_Texture* ImageSDL2::LoadSprite(SDL_Renderer* render, std::string fileName)
	{
		SDL_Texture* texture = nullptr;

		// Load image at specified path
		texture = IMG_LoadTexture(render, fileName.c_str());
		if (texture = nullptr)
		{
			std::cerr << "Unable to load image \"" << fileName << "\" SDL Error: " << SDL_GetError() << std::endl;
		}		

		return texture;
	}
}