#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <string>
#include <memory>
#include <atomic>
#include <mutex>
#include <sstream>
#include <grpc++/grpc++.h>
#include "ylogservice.grpc.pb.h"
#include "common.h"
#include "fmt/format.h"


const std::string address = "127.0.0.1:39393";
extern std::mutex mm_mutex;

class LogClient {
private:
	std::unique_ptr<y3d::LogService::Stub> stub_;
public:
	LogClient(std::shared_ptr<grpc::Channel> channel);;

	bool log(const std::string& logMsg);
};

typedef std::atomic<LogClient*> LogClientPtr;
extern LogClientPtr logClientPtr;
extern LogClient* getLogClientInstance();
extern HANDLE GetProcessHandle(const wchar_t *process_name, DWORD dwAccess);
extern bool IsProcessIsRunning(const wchar_t *process_name);
extern std::string _logServerTerminalAddress;
//extern std::atomic<std::string *> logServerTerminalAddress;

extern std::mutex lock_setServerAddress;
namespace logserver {

	inline void SetLogServerTerminalAdress(std::string address) {
		//logServerTerminalAddress.compare_exchange_strong(address, address);
		//logServerTerminalAddress = &address;
		std::lock_guard<std::mutex> lock(lock_setServerAddress);
		_logServerTerminalAddress = address;
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

