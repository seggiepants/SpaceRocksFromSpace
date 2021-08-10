#pragma once
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <filesystem>
#include <string>
#include "3rdParty/json/json.hpp"
#include "Shared.h"
#include "Version.h"

namespace jam
{
	class Configuration
	{
	public:
		static void CreatePathIfNotExist(std::filesystem::path& p);
		static std::string GetAppPath();
		static std::string GetDataPath();
		static nlohmann::json LoadJsonFile(std::string fileName);
		static bool SaveJsonFile(std::string fileName, nlohmann::json data);
	};
}
#endif