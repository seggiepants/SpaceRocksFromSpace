#include "ResourceManagerDC.h"
#ifdef __DREAMCAST__

namespace jam
{
	ResourceManagerDC::ResourceManagerDC()
	{

	}

	ResourceManagerDC::~ResourceManagerDC()
	{

	}

	void ResourceManagerDC::Clear()
	{

	}

	IAudio* ResourceManagerDC::GetAudio(std::string)
	{
		return nullptr;
	}

	IFont* ResourceManagerDC::GetFont(std::string)
	{
		return nullptr;
	}

	IImage* ResourceManagerDC::GetImage(std::string)
	{
		return nullptr;
	}

	bool ResourceManagerDC::HasAudio(std::string)
	{
		return false;
	}

	bool ResourceManagerDC::HasFont(std::string)
	{
		return false;
	}

	bool ResourceManagerDC::HasImage(std::string)
	{
		return false;
	}

	void ResourceManagerDC::PreloadAudio(std::string)
	{

	}

	void ResourceManagerDC::PreloadFont(std::string key, IFont* font)
	{

	}

	void ResourceManagerDC::PreloadImage(std::string)
	{

	}

	IFont* ResourceManagerDC::LoadTTFFont(std::string fileName, int pointSize)
	{
		return nullptr;
	}

}
#endif