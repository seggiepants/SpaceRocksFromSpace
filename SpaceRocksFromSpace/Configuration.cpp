#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Configuration.h"
#ifdef OS_WIN
#include <atlbase.h>
#include <atlstr.h>
#include <comutil.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#endif

namespace jam
{

	void Configuration::CreatePathIfNotExist(std::filesystem::path& p)
	{
		std::filesystem::path fldr("");
		for (std::filesystem::path::iterator folder = p.begin(); folder != p.end(); folder++)
		{
			fldr /= *folder;
			if (!std::filesystem::exists(fldr))
			{
				try
				{
					std::filesystem::create_directory(fldr);
				}
				catch (std::exception& ex)
				{
					std::cerr << "Unable to create folder " << fldr << std::endl << ex.what() << std::endl;
					break;
				}
			}
		}
		if (!std::filesystem::exists(p))
		{
			std::cerr << "Path Not Found: " << p << std::endl;
		}
	}

	std::string Configuration::GetAppPath()
	{
#ifdef OS_WIN
		wchar_t path[MAX_PATH] = { 0 };
		GetModuleFileNameW(NULL, path, MAX_PATH);
		std::wstring ws(path);
		return std::string(ws.begin(), ws.end());
#elif defined(OS_UNIX)
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);
#elif defined(OS_UNKNOWN)
		return std::string(".");
#endif
	}

	std::string Configuration::GetDataPath()
	{
#ifdef OS_WIN		
		TCHAR szPath[MAX_PATH];
		// Get path for each computer, non-user specific and non-roaming data.
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
		{
			// Append product-specific path
			std::ostringstream ss;
			ss << "\\" << game::COMPANY << "\\" << game::PRODUCT << "\\" << game::VERSION << "\\";
			std::string targetPath = ss.str();

			// Convert char* string to a wchar_t* string.
			size_t copiedChars = 0;
			TCHAR* buffer = new TCHAR[targetPath.size() + 1];
			mbstowcs_s(&copiedChars, buffer, targetPath.size(), targetPath.c_str(), _TRUNCATE);
			PathAppend(szPath, buffer);
			delete buffer;
			std::wstring wPath(szPath);
			std::string ret = std::string(wPath.begin(), wPath.end());
			std::filesystem::path p;
			p.assign(ret.c_str());
			Configuration::CreatePathIfNotExist(p);
			return ret;
		}
		else
		{
			std::string appPath = Configuration::GetAppPath();
			if (appPath[appPath.size() -1] != '\\' && appPath[appPath.size() -1] != '/')
				appPath.push_back('\\');
			std::ostringstream ss;
			ss << appPath.c_str() << game::COMPANY << "\\" << game::PRODUCT << game::VERSION << "\\";
			return ss.str();
		}
#elif defined(OS_UNIX)
		std::ostringstream ss;
		ss << "~/.config/" << game::COMPANY << "/" << game::PRODUCT << "/" << game::VERSION << "/";
		return ss.str();

#elif defined(OS_UNKOWN)
		std::string appPath = Configuration::GetAppPath();
		if (appPath[appPath.size() - 1] != '\\' && appPath[appPath.size() - 1] != '/')
			appPath.push_back('/');
		std::ostringstream ss;
		ss << appPath.c_str() << game::COMPANY << "/" << game::PRODUCT << "/" << game::VERSION << "/";
		return std::string(ss.str());
#endif
	}

	nlohmann::json Configuration::LoadJsonFile(std::string fileName)
	{
		std::filesystem::path filePath = std::filesystem::path(jam::Configuration::GetDataPath());
		filePath /= fileName;		
		std::ostringstream buffer;
		try
		{
			std::ifstream file(filePath.string());
			if (file.is_open())
			{
				std::string line;
				while (std::getline(file, line))
				{
					buffer << line << std::endl;
				}
				file.close();
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to read file " << filePath.string() << std::endl << ex.what() << std::endl;
		}
		try
		{
			return nlohmann::json::parse(buffer.str());
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to parse file " << filePath.string() << std::endl << ex.what() << std::endl;
			return nullptr;
		}
	}

	bool Configuration::SaveJsonFile(std::string fileName, nlohmann::json data)
	{
		std::filesystem::path filePath = std::filesystem::path(jam::Configuration::GetDataPath());
		filePath /= fileName;

		try
		{
			std::ofstream file(filePath.string());
			if (file.is_open())
			{
				file << std::setw(4) << data << std::endl;
				file.close();
			}
			else
			{
				std::cerr << "File not open: " << filePath.string() << std::endl;
				return false;
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to write file " << filePath.string() << std::endl << ex.what() << std::endl;
			return false;
		}
		return true;
	}
}