#pragma once
#ifndef __RESOURCE_MANAGER_SDL2_H__
#define __RESOURCE_MANAGER_SDL2_H__
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_mixer.h>
#include <SDL2\SDL_ttf.h>
#include <string>
#include <unordered_map>
#include "AudioSDL2.h"
#include "IResourceManager.h"
namespace jam
{
    class ResourceManagerSDL2 :
        public IResourceManager
    {
	public:
		~ResourceManagerSDL2();
		/*
		int GetAudio(std::string path);
		olc::Font* GetFont(std::string path, int size);
		olc::Sprite* GetImage(std::string path);
		*/
		void Clear();
		IAudio* GetAudio(std::string);
		void GetFont(std::string, int size);
		void GetImage(std::string);
		bool HasAudio(std::string);
		bool HasFont(std::string, int size);
		bool HasImage(std::string);
		void PreloadAudio(std::string);
		void PreloadFont(std::string, int size);
		void PreloadImage(std::string);
		static IResourceManager* Instance() {
			if (!instance) {
				instance = new ResourceManagerSDL2();
			}
			return instance;
		}
	private:
		ResourceManagerSDL2();
		static ResourceManagerSDL2* instance;
		std::unordered_map<std::string, AudioSDL2*> audio;
		std::unordered_map<std::string, SDL_Texture*> image;
		std::unordered_map<std::string, TTF_Font*> font;
    };
}

#endif