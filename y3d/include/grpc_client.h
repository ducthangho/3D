#pragma once
#include <string>
#include <atomic>
#include <mutex>
#include <grpc++/grpc++.h>
#include "yevent.grpc.pb.h"

#ifdef GRPC_CLIENT_EXPORT
#define GRPC_CLIENT_EXPORT_API __declspec( dllexport )   
#elif defined(GRPC_CLIENT_SERVICE_STATIC)
#define GRPC_CLIENT_EXPORT_API    
#else
#define GRPC_CLIENT_EXPORT_API __declspec( dllimport )   
#endif

#define DefineFunc(func,ReqType,RespType) \
	::grpc::Status func(::grpc::ClientContext* context, const ReqType& request, RespType* response){	\
		return stub_->##func (context,request,response);	\
	};	\
	\
	std::unique_ptr< ::grpc::ClientAsyncResponseReader< RespType >> Async##func (::grpc::ClientContext* context, const ReqType& request, ::grpc::CompletionQueue* cq){	\
		return stub_->Async##func (context,request,cq);	\
	}

class GRPC_CLIENT_EXPORT_API MainWorkerClient {
private:
	std::unique_ptr<y3d::YServiceMainWorker::Stub> stub_;
public:
	MainWorkerClient(std::shared_ptr<grpc::Channel> channel);

	DefineFunc(DoEvent, y3d::YEvent2, y3d::ResponseEvent2);
	DefineFunc(UpdateWorkers, y3d::YWorkerResp, y3d::ENone2);
	DefineFunc(CloseWorkerApp, y3d::YEvent2, y3d::ResponseEvent2);	
};


const std::string MainWorkerServerIP = "127.0.0.1:37001";
extern std::mutex mm_mutex;


typedef MainWorkerClient ClientType;
typedef std::atomic<ClientType*> ClientPtr;
extern ClientPtr clientPtr;
GRPC_CLIENT_EXPORT_API ClientType* getClientInstance();
GRPC_CLIENT_EXPORT_API void reset();

#define GRPC_CALL(func,...)	{	\
	auto* client = getClientInstance();	\
	if (client)	{	\
		grpc::Status status = client->##func(__VA_ARGS__);	\
		if (!status.ok()) {	\
			reset();	\
			client = getClientInstance();	\
			status = client->##func(__VA_ARGS__);	\
			if (!status.ok()) logserver::LOG("Execution of method {} failed\n",#func);	\
		}	\
	} else {	\
		logserver::LOG("Cannot connect to MainWorker server\n");	\
	};	\
}