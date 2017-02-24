#include <string>
#include <iostream>
#include <memory>
#include <string>
#include "LogClient.h"
#include <grpc++/grpc++.h>
#include "ylogservice.grpc.pb.h"
#include <windows.h>
#include <Tlhelp32.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

LogClient::LogClient(std::shared_ptr<Channel> channel) :stub_(y3d::LogService::NewStub(channel))
{
}

//std::string ExePath() {
//	TCHAR buffer[MAX_PATH];
//	GetModuleFileName(NULL, buffer, MAX_PATH);
//	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
//	return std::string(buffer).substr(0, pos);
//}

std::mutex lock_setServerAddress;
std::mutex checkProcessRunning;
std::string _logServerTerminalAddress = "F:\\WorkSpace\\3D\\MaxNet\\Y3D\\x64\\Release\\LogServer.exe";
std::atomic<std::string*> logServerTerminalAddress = &_logServerTerminalAddress;

bool LogClient::log(const std::string& logMsg)
{
	y3d::LogMessage messageSend;
	messageSend.set_message(logMsg);
	y3d::LogMessage messageRec;
	ClientContext context;
	Status status = stub_->Log(&context, messageSend, &messageRec);
	if (status.ok()) {
		return true;
	}
	else
	{	
		//LogClient* oldPtr = logClientPtr.exchange(nullptr);
		//delete oldPtr;
		LogClient* oldPtr = logClientPtr;
		if (!logClientPtr.compare_exchange_strong(oldPtr, nullptr))
			delete oldPtr;
		std::cout << "Log function return error: " << status.error_code() << ": " << status.error_message() << std::endl;
		//bool isProcessRunning = true;
		//{			
		//	isProcessRunning = IsProcessIsRunning(L"LogServer.exe");
		//}
		{
			std::lock_guard<std::mutex> lock(checkProcessRunning);
			if (!IsProcessIsRunning(L"LogServer.exe")) {
				std::string cmd;
				{
					std::lock_guard<std::mutex> lock(lock_setServerAddress);
					cmd = "start " + _logServerTerminalAddress;
				}				
				int result = system(cmd.c_str());
			}
		}
		return false;
	}
}

std::mutex mm_mutex;
LogClientPtr logClientPtr = nullptr;

LogClient* getLogClientInstance() {
	LogClient* tmp = logClientPtr.load();
	if (tmp == nullptr) {
		//LogClient* abc = new LogClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
		//if (!logClientPtr.compare_exchange_strong(nullptr, abc)) {
		//	delete abc;
		//}
		std::lock_guard<std::mutex> lock(mm_mutex);
		tmp = logClientPtr.load();
		if (tmp == nullptr) {
			tmp = new LogClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
			logClientPtr.store(tmp);
		}
	}
	return tmp;
}

bool IsProcessIsRunning(const wchar_t * process_name)
{
	HANDLE hProcess = GetProcessHandle(process_name, PROCESS_QUERY_INFORMATION);
	if (hProcess == 0 || hProcess == INVALID_HANDLE_VALUE)
	{
		std::cout << "Server terminal is not running"<<std::endl;
		return false;
	}
	else
	{
		std::cout << "Server terminal is running"<<std::endl;
		return true;
	}
}

HANDLE GetProcessHandle(const wchar_t *process_name, DWORD dwAccess)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;


	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Failed to create process snapshot!";
		return INVALID_HANDLE_VALUE;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		std::cerr << "Process32First() failed\n";
		return INVALID_HANDLE_VALUE;
	}

	do
	{
		if (wcscmp(pe32.szExeFile, process_name) == 0)
			return OpenProcess(dwAccess, 0, pe32.th32ProcessID);

	} while (Process32Next(hProcessSnap, &pe32));
}
