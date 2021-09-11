#include <iostream>
#include <SDL2/SDL_image.h>
#include "BackendSDL2.h"
#include "ImageSDL2.h"
#include "../../Shared.h"


namespace jam
{
	ImageSDL2::ImageSDL2()
	{
		this->texture = 0;
		this->width = 0;
		this->height = 0;
	}

	ImageSDL2::~ImageSDL2()
	{
		if (this->texture != 0)
		{
		  glDeleteTextures(1, &this->texture);
			this->texture = 0;
		}
	}

	bool ImageSDL2::Construct(std::string fileName)
	{
		this->texture = 0;
		this->texture = this->LoadTexture(fileName, &this->width, &this->height);
		return this->texture != 0;
	}

  	bool ImageSDL2::Construct(SDL_Surface* surface)
	{
		this->texture = 0;
		this->texture = this->LoadTexture(surface, &this->width, &this->height);
		return this->texture != 0;
	}


	int ImageSDL2::GetWidth()
	{
	  return this->width;
	}

	GLuint ImageSDL2::GetHandle()
	{
		return this->texture;
	}

	int ImageSDL2::GetHeight()
	{
	  return this->height;
	}

  GLuint ImageSDL2::LoadTexture(std::string fileName, int* w, int* h)
	{
	  SDL_Surface* image = IMG_Load(fileName.c_str());
	  GLuint texture;
	  if (image == nullptr)
		{
			std::cerr << "Unable to load image \"" << fileName << "\" SDL Error: " << IMG_GetError() << std::endl;
		}

	  *w  = image->w;
	  *h = image->h;
	  glGenTextures(1, &texture);
	  glBindTexture(GL_TEXTURE_2D, texture);

	  int mode = GL_RGBA;
	  if (image->format->BytesPerPixel == 4)
	    {
	      mode = GL_RGBA;
	    }
	  glTexImage2D(GL_TEXTURE_2D, 0, mode, *w, *h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	  SDL_FreeSurface(image);

	  return texture;
	}
  GLuint ImageSDL2::LoadTexture(SDL_Surface* image, int* w, int* h)
	{
	  GLuint texture;
	  if (image == nullptr)
		{
			std::cerr << "Invalid surface passed to function ImageSDL2::LoadTexture(SDL_Surface*, int*, intI) " << std::endl;
		}

	  *w  = image->w;
	  *h = image->h;
	  glGenTextures(1, &texture);
	  glBindTexture(GL_TEXTURE_2D, texture);

	  int mode = GL_RGBA;
	  if (image->format->BytesPerPixel == 4)
	    {
	      mode = GL_RGBA;
	    }
	  glTexImage2D(GL_TEXTURE_2D, 0, mode, *w, *h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	  return texture;
	}

}
