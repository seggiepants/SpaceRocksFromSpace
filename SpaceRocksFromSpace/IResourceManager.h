#pragma once
#ifndef __IRESOURCE_MANAGER_H__
#define __IRESOURCE_MANAGER_H__
#include <string>
#include "IAudio.h"
#include "IFont.h"

namespace jam
{
    class IResourceManager
    {
	public:
		IResourceManager() {};
		~IResourceManager() {};
		//int GetAudio(std::string path);
		//olc::Font* GetFont(std::string path, int size);
		//olc::Sprite* GetImage(std::string path);
		virtual void Clear() = 0;
		virtual IAudio* GetAudio(std::string) = 0;
		virtual IFont* GetFont(std::string) = 0;
		virtual void GetImage(std::string) = 0;
		virtual bool HasAudio(std::string) = 0;
		virtual bool HasFont(std::string) = 0;
		virtual bool HasImage(std::string) = 0;
		virtual void PreloadAudio(std::string) = 0;
		virtual void PreloadFont(std::string key, IFont* font) = 0;
		virtual void PreloadImage(std::string) = 0;
		//static IResourceManager* Instance() = 0;
    };
}
#endif
