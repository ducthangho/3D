#pragma once
#include <utility>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
//#define FMT_HEADER_ONLY
#include "fmt/format.h"

#define YCDEBUG 1

class Time {
public:
	Time() : time(std::chrono::steady_clock::now()) {};
	long long duration() {
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - time);
		return elapsed.count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> time;
};

inline const char *find_dll_dir()
{
	static char path[MAX_PATH];

	HMODULE dll;
	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)find_dll_dir, &dll)) {
		return 0;
	}
	GetModuleFileNameA(dll, path, sizeof path);

	char *last_slash = strrchr(path, '\\');
	if (last_slash) {
		*last_slash = 0;
	}
	return path;
}

constexpr char* LOG_FILE = "log.y3d.txt";

class LOGGER {
public:
	LOGGER(char* file = LOG_FILE) : filename(file) {
		f = std::fopen(file, "a");
	}

	void clear() {
		if (f) {
			std::remove(filename);
			f = nullptr;
		}
	}

	template <typename... Args>
	void operator()(char* format_str, const Args& ... args) {
		fmt::print(f, format_str, args...);
	}

	template <typename... Args>
	void operator()(wchar_t* format_str, const Args& ... args) {
		fmt::print(f, format_str, args...);
	}

	~LOGGER() {
		if (f) {
			std::fflush(f);
			std::fclose(f);
		}
	}
private:
	char* filename;
	std::FILE *f;
};

static LOGGER LOG;


// multi byte to wide char:
inline std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

// wide char to multi byte:
inline std::string ws2s(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
	return strTo;
}


template <typename... Args>
inline std::wstring formatWS(fmt::StringRef format_str, const Args & ... args) {
	std::string s = fmt::format(format, args...);
	return s2ws(s);
}

template <typename... Args>
inline std::wstring sprintfws(fmt::StringRef format_str, const Args & ... args) {
	std::wstring s;
	fmt::format(s, args...);
	return s2ws(s);
}

inline bool is_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

inline int dirExists(const char *path)
{
	struct stat info;

	if (stat(path, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}

