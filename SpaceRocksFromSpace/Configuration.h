#pragma once
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <filesystem>
#include <string>
#include "Shared.h"
#include "Version.h"

namespace jam
{
	class Configuration
	{
	public:
		static std::string GetAppPath();
		static std::string GetDataPath();
	};
}
#endif