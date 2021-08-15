#include <iostream>
#include <SDL2\SDL_image.h>>
#include "ImageSDL2.h"
#include "RendererSDL2.h"

namespace jam
{
	ImageSDL2::ImageSDL2()
	{
		this->texture = nullptr;
	}

	ImageSDL2::~ImageSDL2()
	{
		if (this->texture != nullptr)
		{
			SDL_DestroyTexture(this->texture);
			this->texture = nullptr;
		}
	}

	bool ImageSDL2::Construct(jam::IRenderer* render, std::string fileName)
	{
		this->texture = ((jam::RendererSDL2*)render)->LoadTexture(fileName);
		return false;
	}

	int ImageSDL2::GetWidth()
	{
		SDL_Rect r;
		if (this->texture != nullptr)
		{

			SDL_QueryTexture(this->texture, NULL, NULL, &r.w, &r.h);
			return r.w;
		}
		else
			return 0;
	}

	SDL_Texture* ImageSDL2::GetHandle()
	{
		return this->texture;
	}

	int ImageSDL2::GetHeight()
	{
		SDL_Rect r;
		if (this->texture != nullptr)
		{

			SDL_QueryTexture(this->texture, NULL, NULL, &r.w, &r.h);
			return r.h;
		}
		else
			return 0;
	}
}