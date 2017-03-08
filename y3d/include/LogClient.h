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
#include "common.h"
#include "prettyprint.hpp"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "ylogservice.grpc.pb.h"


#ifdef LOG_EXPORT
#define LOGSERVICE_API __declspec( dllexport )   
#elif defined(LOG_SERVICE_STATIC)
#define LOGSERVICE_API    
#else
#define LOGSERVICE_API __declspec( dllimport )   
#endif



const std::string address = "127.0.0.1:39393";
extern std::mutex mm_mutex;

class LOGSERVICE_API LogClient {
private:
	std::unique_ptr<y3d::LogService::Stub> stub_;
public:
	LogClient(std::shared_ptr<grpc::Channel> channel);

	bool log(const std::string& logMsg);
};

extern std::mutex lock_setServerAddress;


typedef std::atomic<LogClient*> LogClientPtr;
extern LogClientPtr logClientPtr;
LOGSERVICE_API LogClient* getLogClientInstance();
extern HANDLE GetProcessHandle(const wchar_t *process_name, DWORD dwAccess);
extern bool IsProcessIsRunning(const wchar_t *process_name);
extern std::string _logServerTerminalAddress;
extern std::wstring TerminalName;
extern std::string FileName(const std::string& str);
extern std::mutex checkProcessRunning;

namespace logserver {

	inline void SetLogServerTerminalAdress(std::string address) {
		//logServerTerminalAddress.compare_exchange_strong(address, address);
		//logServerTerminalAddress = &address;
		//std::lock_guard<std::mutex> lock(lock_setServerAddress);
		std::lock_guard<std::mutex> lock(checkProcessRunning);
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
		return std::to_string(Number);
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


	#define PRINTER(name) printer(#name, (name))

	template <typename T>
	std::string printer(char *name, T value) {
		fmt::MemoryWriter w;
		w.write(" {0} = {1}", name, value);
		return std::string(w.c_str());
	}

	inline std::string const& to_string(std::string const& s) { return s; }

	template<typename... Args>
	inline std::string stringer(Args const&... args)
	{
		std::string result;
		using ::to_string;
		using std::to_string;
		int unpack[]{ 0, (result += to_string(args), 0)... };
		static_cast<void>(unpack);
		return result;
	}

	template<typename... Args>
	inline std::string stringerWithDelim(char* delim,Args const&... args)
	{
		std::string result;
		using ::to_string;
		using std::to_string;		
		int unpack[]{ 0, (result += to_string(args)+delim, 0)... };
		static_cast<void>(unpack);
		return result;
	}

	template <typename... Args>
	inline void Print(const Args&  ... args) {
		//fmt::MemoryWriter w;
		//w.write(format_str, args...);
		std::string a = stringer( args...);
		LogClient* logClient = getLogClientInstance();		
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}
static std::string delim = " ";

template <typename T>
inline void setDelim(T str) {
	delim = str;
}


#define EXPAND(x) x
#define VAR(x) out.write("{} = {}{}",#x,x,delim);
#define DBG(...)	{	\
	fmt::MemoryWriter out;\
	out.write("FILE: \"{}\": LINE {}: ",__FILE__,__LINE__);	\
	FOR_EACH(VAR, __VA_ARGS__);	\
	LOG(out.c_str());	\
	}
	
	

	template <typename... Args>
	inline void PrintWithDelim(char* delim,const Args&  ... args) {
		//fmt::MemoryWriter w;
		//w.write(format_str, args...);
		std::string a = stringerWithDelim(delim,args...);
		LogClient* logClient = getLogClientInstance();
		if (!logClient->log(a)) {
			logClient = getLogClientInstance();
			logClient->log(a);
		}
	}

	//inline void test() {
	//	fmt::MemoryWriter os;
	//	std::map<int, int> n;
	//	n[0] = 1;
	//	n[1] = 0;
	//	n[3] = 5;
	//	std::vector<int> vv = { 1,2,3,4,5 };
	//	int arr[] = { 5,4,3,2,1 };
	//	std::string s = fmt::format("\n Testing vv ={}\n", vv);
	//	std::set<int> ss{ 1,2,3,4,5,0,57 };
	//	std::map<std::string, int> m;
	//	m["hello"] = 1;
	//	m["a"] = 0;
	//	m["good"] = 5;
	//	//os << pretty_print_array(arr,5);
	//	//LOG(s);
	//	os.write("\n Testing arr ={}\n", pretty_print_array(arr, 5));
	//	//LOG(s);
	//	os.write("\n Testing n ={}\n", n);
	//	//LOG(s);
	//	os.write("\n Testing ss ={}\n", ss);
	//	os.write("\n Testing vv ={}\n", vv);
	//	os.write("\n Testing m ={}\n", m);
	//	
	//	//s = fmt::format("\n Testing ss ={}\n", ss);
	//	//s = os.str();
	//	LOG(os.c_str());
	//}
}