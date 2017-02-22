#include <string>
#include <iostream>
#include <memory>
#include <string>
#include "LogClient.h"
#include <grpc++/grpc++.h>
#include "ylogservice.grpc.pb.h"
#include <windows.h>

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
		LogClient* oldPtr = logClientPtr.exchange(nullptr);
		//delete oldPtr->stub_.get();
		delete oldPtr;
		std::cout << "Log function return error: " << status.error_code() << ": " << status.error_message()<< std::endl;
		char* cmd = "start F:\\WorkSpace\\3D\\MaxNet\\Y3D\\x64\\Release\\LogServer.exe";
		int result = system(cmd);
		//y3d::LogMessage messageSend;
		//messageSend.set_message(logMsg);
		//y3d::LogMessage messageRec;
		//ClientContext context;
		//stub_->Log(&context, messageSend, &messageRec);
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