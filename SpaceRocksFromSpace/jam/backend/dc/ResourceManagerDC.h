#pragma once
#ifndef __RESOURCE_MANAGER_DC_H__
#define __RESOURCE_MANAGER_DC_H__
#include "../../IResourceManager.h"
#ifdef __DREAMCAST__
namespace jam
{
    class ResourceManagerDC :
        public IResourceManager
    {
		ResourceManagerDC();
		~ResourceManagerDC() override;
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
    };
}
#endif
#endif