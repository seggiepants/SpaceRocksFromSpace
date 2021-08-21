#pragma once
#ifndef __RESOURCE_MANAGER_SDL2_H__
#define __RESOURCE_MANAGER_SDL2_H__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>
#include "AudioSDL2.h"
#include "../../IImage.h"
#include "ImageSDL2.h"
#include "../../IFont.h"
#include "../../IResourceManager.h"
namespace jam
{
    class ResourceManagerSDL2 :
        public IResourceManager
    {
	public:
		~ResourceManagerSDL2() override;
		/*
		int GetAudio(std::string path);
		olc::Font* GetFont(std::string path, int size);
		olc::Sprite* GetImage(std::string path);
		*/
		void Clear();
		IAudio* GetAudio(std::string);
		IFont* GetFont(std::string);
		IImage* GetImage(std::string);
		bool HasAudio(std::string);
		bool HasFont(std::string);
		bool HasImage(std::string);
		void PreloadAudio(std::string);
		void PreloadFont(std::string key, IFont* font);
		void PreloadImage(std::string);
		IFont* LoadTTFFont(std::string fileName, int pointSize);
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
		std::unordered_map<std::string, jam::ImageSDL2*> image;
		std::unordered_map<std::string, jam::IFont*> font;
    };
}

#endif
