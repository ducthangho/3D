#pragma once
#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotocd.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")
#endif

#include "y3d.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"


#ifdef YSERVICE_EXPORT
#define YSERVICE_API __declspec( dllexport )   
#else
#define YSERVICE_API __declspec( dllimport )   
#endif

//using namespace grpc;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using namespace y3d;

// Somewhere in the global scope :/


class AbstractService : public YServiceMaxTools::Service {
public:
	virtual void Initialize(void* codegen, void* gli) = 0;//This function used initialize static variable needed to run grpc in dll mode. See Service.cpp for an example
	virtual Status MakeNode4Edit(ServerContext* context, const Make4TestParam* request, ResultReply* reply) = 0;
	virtual Status RenameObject(ServerContext* context, const RenameParam* request, ResultReply* reply) = 0;
	virtual Status BakeNormal(ServerContext* context, const ENormal* enm, ResultReply* reply) = 0;
	virtual Status BatchOptimize(ServerContext* context, const BatchOptimizeParam* bp, ResultReply* reply) = 0;
	virtual Status GetObjectFromMax(ServerContext* context, const EmptyParam* ep, YAreaList* yal) = 0;
	virtual Status DoAction(ServerContext* context, grpc::ServerReaderWriter<YEvent, YEvent>* stream) = 0;
	virtual Status DoStreamClient(ServerContext* context, grpc::ServerReader<YEvent>* stream, ResponseEvent* re) = 0;
	virtual Status DoEvent(ServerContext* context, const YEvent* ye, ResponseEvent* re) = 0;
	virtual Status CloneObject(ServerContext* context, const ::y3d::EmptyParam* request, ::y3d::ResultReply* response) = 0;
	virtual Status Shutdown(ServerContext* context, const ::y3d::EmptyParam* request, ::y3d::ResultReply* response) = 0;
	virtual Status NewProject(ServerContext* context, const ::y3d::NewProjectParam* request, ResponseNProject* response) = 0;
	virtual Status LoadProject(ServerContext* context, const ::y3d::ProjectInfo* request, ::y3d::ResponseNProject* response) = 0;

	virtual Status Init4Test(ServerContext* context, const InitTestParam* request, InitTestResponse* reply) = 0;
};

class YServiceImpl final : public AbstractService {
public:
	virtual void Initialize(void* codegen, void* gli) override;//This function used initialize static variable needed to run grpc in dll mode. See Service.cpp for an example
	Status MakeNode4Edit(ServerContext* context, const Make4TestParam* request, ResultReply* reply) override;
	Status RenameObject(ServerContext* context, const RenameParam* request, ResultReply* reply) override;
	Status BakeNormal(ServerContext* context, const ENormal* enm, ResultReply* reply) override;
	Status BatchOptimize(ServerContext* context, const BatchOptimizeParam* bp, ResultReply* reply) override;
	Status GetObjectFromMax(ServerContext* context, const EmptyParam* ep, YAreaList* yal) override;
	Status DoAction(ServerContext* context, grpc::ServerReaderWriter<YEvent, YEvent>* stream) override;
	Status DoStreamClient(ServerContext* context, grpc::ServerReader<YEvent>* stream, ResponseEvent* re) override;
	Status DoEvent(ServerContext* context, const YEvent* ye, ResponseEvent* re) override;
	Status CloneObject(::grpc::ServerContext* context, const ::y3d::EmptyParam* request, ::y3d::ResultReply* response) override;	
	Status Shutdown(::grpc::ServerContext* context, const ::y3d::EmptyParam* request, ::y3d::ResultReply* response) override;
	Status NewProject(ServerContext* context, const ::y3d::NewProjectParam* request, ::y3d::ResponseNProject* response) override;
	Status LoadProject(ServerContext* context, const ::y3d::ProjectInfo* request, ::y3d::ResponseNProject* response) override;

	Status Init4Test(ServerContext* context, const InitTestParam* request, InitTestResponse* reply) override;
	~YServiceImpl() override;
};

typedef AbstractService* ServicePtr;
typedef std::shared_ptr<AbstractService> ServiceSharedPtr;

typedef ServicePtr(APIENTRY *GetServiceImplFunc)();
typedef void(APIENTRY *ReleaseObjectFunc)(ServicePtr);
typedef ServiceSharedPtr(APIENTRY *GetServicePtrFunc)();

#ifndef _USE_DYNAMIC_LOADED_DLL_
YSERVICE_API ServiceSharedPtr APIENTRY getServicePtr();
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	YSERVICE_API ServicePtr APIENTRY getServiceImpl();
	YSERVICE_API void APIENTRY releaseObject(ServicePtr obj);	
#ifdef __cplusplus
}
#endif // __cplusplus
#endif