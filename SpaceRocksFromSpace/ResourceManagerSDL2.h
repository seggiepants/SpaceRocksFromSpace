#pragma once
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
				instance = new ResourceManagerSDL2();
			}
			return instance;
		}
	private:
		ResourceManagerSDL2();
		static ResourceManagerSDL2* instance;
    };
}

