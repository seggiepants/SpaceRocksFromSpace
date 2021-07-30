#ifndef __RESOURCE_MANAGER_PGE_H__
#define __RESOURCE_MANAGER_PGE_H__
#pragma once
#include <unordered_map>
#include <olcPixelGameEngine.h>
#include <olcPGEX_Sound.h>
#include <olcPGEX_TTF.h>
#include "IResourceManager.h"

namespace jam
{
    class ResourceManagerPGE :
        public IResourceManager
    {
    public:
		~ResourceManagerPGE();
		/*
		int GetAudio(std::string path);
		olc::Font* GetFont(std::string path, int size);
		olc::Sprite* GetImage(std::string path);
		*/
		void Clear();
		bool HasAudio(std::string);
		bool HasFont(std::string, int size);
		bool HasImage(std::string);
		void PreloadAudio(std::string);
		void PreloadFont(std::string, int size);
		void PreloadImage(std::string);
		void PlayAudio(std::string);
		void StopAudio();
		static IResourceManager* Instance() {
			if (!instance) {
				instance = new ResourceManagerPGE();
			}
			return instance;
		}
    protected:
		ResourceManagerPGE();
		static ResourceManagerPGE* instance;
		std::unordered_map<std::string, int> audio;
		std::unordered_map<std::string, olc::Sprite*> image;
		std::unordered_map<std::string, olc::Font*> font;

    };
}

#endif
