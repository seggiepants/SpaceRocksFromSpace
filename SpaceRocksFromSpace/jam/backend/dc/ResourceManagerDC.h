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
		void Clear() override;
		IAudio* GetAudio(std::string) override;
		IFont* GetFont(std::string) override;
		IImage* GetImage(std::string) override;
		bool HasAudio(std::string) override;
		bool HasFont(std::string) override;
		bool HasImage(std::string) override;
		void PreloadAudio(std::string) override;
		void PreloadFont(std::string key, IFont* font) override;
		void PreloadImage(std::string) override;
		IFont* LoadTTFFont(std::string fileName, int pointSize) override;
    };
}
#endif
#endif