#pragma once

#include "fmt/format.h"
#include "ylogservice.grpc.pb.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <fstream>
#include <grpc++/grpc++.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <shlobj.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <utility>
#include <windows.h>
//#define FMT_HEADER_ONLY

#define YCDEBUG 1

const std::string address = "127.0.0.1:39393";
extern std::mutex mm_mutex;

class LogClient {
private:
	std::unique_ptr<y3d::LogService::Stub> stub_;
public:
	LogClient(std::shared_ptr<grpc::Channel> channel);;

	bool log(const std::string& logMsg);
};

extern std::mutex lock_setServerAddress;


typedef std::atomic<LogClient*> LogClientPtr;
extern LogClientPtr logClientPtr;
extern LogClient* getLogClientInstance();
extern HANDLE GetProcessHandle(const wchar_t *process_name, DWORD dwAccess);
extern bool IsProcessIsRunning(const wchar_t *process_name);
extern std::string _logServerTerminalAddress;
extern std::wstring TerminalName;

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


//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
inline std::string GetLastErrorAsString(DWORD errorMessageID)
{
	//Get the error message, if any.
	//DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

#define SYSTEM_CALL_ERROR_MSG(code,error_code,msg)	\
	code;	\
	auto error_code = GetLastError();	\
	std::wstring msg;	\
	if (error_code){	\
		msg = s2ws(fmt::format("Error : {0}. {1}\n", error_code, GetLastErrorAsString(error_code).c_str()));	\
	}

#define SYSTEM_CALL_ERROR(code,error_code)	\
	code;	\
	auto error_code = GetLastError();	\
	if (error_code){	\
		auto msg = s2ws(fmt::format("Error : {0}. {1}\n", error_code, GetLastErrorAsString(error_code).c_str()));	\
		MessageBox(NULL, msg.c_str(), L"Error", MB_OK);	\
	}

#define SYSTEM_CALL(code)	\
	code;	\
	{	\
	auto error_code = GetLastError();	\
	if (error_code){	\
		auto msg = s2ws(fmt::format("Error : {0}. {1}\n", error_code, GetLastErrorAsString(error_code).c_str()));	\
		MessageBox(NULL, msg.c_str(), L"Error", MB_OK);	\
	}	\
	}

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


namespace logserver {
	extern std::string FileName(const std::string& str);

	inline void SetLogServerTerminalAdress(std::string address) {
		//logServerTerminalAddress.compare_exchange_strong(address, address);
		//logServerTerminalAddress = &address;
		std::lock_guard<std::mutex> lock(lock_setServerAddress);
		_logServerTerminalAddress = address;
		TerminalName = s2ws(FileName(_logServerTerminalAddress)).c_str();
	}

	//expect stub_->Log(&context, messageSend, &messageRec) is thread safe
	inline void LOG(std::string a) {
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
		//logClient->log("ha ha, i got a new habit");
	};

	inline void LOG(char* a) {
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	inline void LOG(std::wstring& str) {
		std::string a = ws2s(str);
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	inline void LOG(wchar_t* str) {
		std::string a = ws2s(std::wstring(str));
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename... Args>
	inline void LOG(std::string& format_str, const Args ... args) {
		fmt::MemoryWriter w;
		w.write(format_str, args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename... Args>
	inline void LOG(char* format_str, const Args& ... args) {
		fmt::MemoryWriter w;
		w.write(format_str, args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename... Args>
	inline void LOG(std::wstring& format_str, const Args&  ... args) {
		fmt::MemoryWriter w;
		w.write(format_str, args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename... Args>
	inline void LOG(wchar_t* format_str, const Args & ... args) {
		fmt::MemoryWriter w;
		w.write(format_str, args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename T>
	inline std::string NumberToString(T Number)
	{
		std::ostringstream ss;
		ss << Number;
		return ss.str();
	}


	inline void Printf(std::string a) {
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
		//logClient->log("ha ha, i got a new habit");
	};

	inline void Printf(char* a) {
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	inline void Printf(std::wstring& str) {
		std::string a = ws2s(str);
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	inline void Printf(wchar_t* str) {
		std::string a = ws2s(std::wstring(str));
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}


	inline std::string string_format(const std::string fmt_str, ...) {
		int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
		std::string str;
		std::unique_ptr<char[]> formatted;
		va_list ap;
		while (1) {
			formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
			strcpy(&formatted[0], fmt_str.c_str());
			va_start(ap, fmt_str);
			final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
			va_end(ap);
			if (final_n < 0 || final_n >= n)
				n += abs(final_n - n + 1);
			else
				break;
		}
		return std::string(formatted.get());
	}

	template <typename... Args>
	inline void Printf(std::string& format_str, const Args ... args) {
		//fmt::MemoryWriter w;
		//w.write(format_str, args...);
		std::string a = string_format(format_str, args...);
		LogClient* logClient = getLogClientInstance();
		//auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename... Args>
	inline void Printf(char* format_str, const Args& ... args) {
		/*fmt::MemoryWriter w;
		w.write(format_str, args...);*/
		std::string a = string_format(format_str, args...);
		LogClient* logClient = getLogClientInstance();
		//auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename... Args>
	inline void Printf(std::wstring& format_str, const Args&  ... args) {
		//fmt::MemoryWriter w;
		//w.write(format_str, args...);
		std::string a = string_format(ws2s(format_str), args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	template <typename... Args>
	inline void Printf(wchar_t* format_str, const Args & ... args) {
		//fmt::MemoryWriter w;
		//w.write(format_str, args...);
		std::string a = string_format(ws2s(format_str), args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	/*template <typename T, typename... Args>
	inline void LOG(T t, const Args & ... args) {
		fmt::MemoryWriter w;
		std::string First = NumberToString(t);
		w.write(std::to_string(t), args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}*/
}