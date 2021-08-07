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
			return std::string(wPath.begin(), wPath.end());
		}
		else
		{
			std::string appPath = Configuration::GetAppPath();
			if (appPath[appPath.size() -1] != '\\' && appPath[appPath.size() -1] != '/')
				appPath.push_back('\\');
			std::ostringstream ss;
			ss << appPath.c_str() << game::COMPANY << "\\" << game::PRODUCT << game::VERSION << "\\";
			return std::string(ss.str());
		}
#elif defined(OS_UNIX)
		std::string appPath = Configuration::GetAppPath();
		if (appPath[appPath.size() - 1] != '\\' && appPath[appPath.size() - 1] != '/')
			appPath.push_back('\\');
		std::ostringstream ss;
		ss << appPath.c_str() << game::COMPANY << "\\" << game::PRODUCT << game::VERSION << "\\";
		return std::string(ss.str());

#elif defined(OS_UNKOWN)
		std::string appPath = Configuration::GetAppPath();
		if (appPath[appPath.size() - 1] != '\\' && appPath[appPath.size() - 1] != '/')
			appPath.push_back('\\');
		std::ostringstream ss;
		ss << appPath.c_str() << game::COMPANY << "\\" << game::PRODUCT << game::VERSION << "\\";
		return std::string(ss.str());
#endif
	}
}