#include "ResourceManagerSDL2.h"

namespace jam
{
	ResourceManagerSDL2* ResourceManagerSDL2::instance = NULL;
	ResourceManagerSDL2::ResourceManagerSDL2() {}
	ResourceManagerSDL2::~ResourceManagerSDL2() {}
	void ResourceManagerSDL2::Clear() {}
	bool ResourceManagerSDL2::HasAudio(std::string) { return false; }
	bool ResourceManagerSDL2::HasFont(std::string, int size) { return false; }
	bool ResourceManagerSDL2::HasImage(std::string) { return false; }
	void ResourceManagerSDL2::PreloadAudio(std::string) {}
	void ResourceManagerSDL2::PreloadFont(std::string, int size) {}
	void ResourceManagerSDL2::PreloadImage(std::string) {}
	void ResourceManagerSDL2::PlayAudio(std::string) {}
	void ResourceManagerSDL2::StopAudio() {}

}