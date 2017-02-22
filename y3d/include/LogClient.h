#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <string>
#include <memory>
#include <atomic>
#include <mutex>

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

namespace logserver {
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

	template <typename T, typename... Args>
	inline void LOG(T t, const Args & ... args) {
		fmt::MemoryWriter w;
		w.write(std::to_string(t), args...);
		LogClient* logClient = getLogClientInstance();
		auto a = w.str();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}
}

