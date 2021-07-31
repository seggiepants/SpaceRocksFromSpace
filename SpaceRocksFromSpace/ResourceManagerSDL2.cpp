#include "ResourceManagerSDL2.h"

namespace jam
{
	ResourceManagerSDL2* ResourceManagerSDL2::instance = NULL;
	ResourceManagerSDL2::ResourceManagerSDL2() {}
	ResourceManagerSDL2::~ResourceManagerSDL2() 
	{
		this->Clear();
	}

	void ResourceManagerSDL2::Clear() 
	{
		if (instance != NULL)
		{
			for (const auto& element : instance->audio) {
				delete element.second;
			}
			instance->audio.clear();

			for (const auto& element : instance->font) {
				delete element.second;
			}
			instance->font.clear();

			for (const auto& element : instance->image) {
				delete element.second;
			}
			instance->image.clear();

			ResourceManagerSDL2* temp = instance;
			instance = NULL;
			delete temp;
		}
	}
	IAudio* ResourceManagerSDL2::GetAudio(std::string filePath) 
	{ 
		this->PreloadAudio(filePath);
		return this->audio[filePath];
	}

	void ResourceManagerSDL2::GetFont(std::string, int size) {}
	void ResourceManagerSDL2::GetImage(std::string) {}

	bool ResourceManagerSDL2::HasAudio(std::string filePath) 
	{ 
		std::unordered_map<std::string, AudioSDL2*>::const_iterator search = instance->audio.find(filePath);

		return (search != instance->audio.end());
	}

	bool ResourceManagerSDL2::HasFont(std::string, int size) { return false; }
	bool ResourceManagerSDL2::HasImage(std::string) { return false; }
	
	void ResourceManagerSDL2::PreloadAudio(std::string filePath) 
	{
		if (!instance->HasAudio(filePath))
		{
			AudioSDL2* sample = new AudioSDL2();
			bool success = sample->Load(filePath);

			if (success)
			{
				instance->audio[filePath] = sample;
			}
		}
	}

	void ResourceManagerSDL2::PreloadFont(std::string, int size) {}
	void ResourceManagerSDL2::PreloadImage(std::string) {}

}