#pragma once
#pragma once

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


class AbstractTestService : public YServiceTest::Service {
public:
	virtual void Initialize(void* codegen, void* gli) = 0;//This function used initialize static variable needed to run grpc in dll mode. See Service.cpp for an example
	Status MTest1(ServerContext* context, const EmptyParam* request, EmptyParam* reply) = 0;
	Status MTest2(ServerContext* context, const StringParam* request, EmptyParam* reply) = 0;
	Status GenerateInterfaceInfo(ServerContext* context, const EmptyParam* request, EmptyParam* reply) = 0;
};

class YServiceTestImpl final : public AbstractTestService {
public:
	virtual void Initialize(void* codegen, void* gli) override;//This function used initialize static variable needed to run grpc in dll mode. See Service.cpp for an example
	Status MTest1(ServerContext* context, const EmptyParam* request, EmptyParam* reply) override;
	Status MTest2(ServerContext* context, const StringParam* request, EmptyParam* reply) override;
	Status GenerateInterfaceInfo(ServerContext* context, const EmptyParam* request, EmptyParam* reply) override;
	~YServiceTestImpl() override;
};

typedef AbstractTestService* ServiceTestPtr;
typedef std::shared_ptr<AbstractTestService> ServiceTestSharedPtr;

typedef ServiceTestPtr(APIENTRY *GetServiceTestImplFunc)();
typedef void(APIENTRY *ReleaseObjectTestFunc)(ServiceTestPtr);
typedef ServiceTestSharedPtr(APIENTRY *GetServiceTestPtrFunc)();


#ifndef _USE_DYNAMIC_LOADED_DLL_
YSERVICE_API ServiceTestSharedPtr APIENTRY getServiceTestPtr();
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	YSERVICE_API ServiceTestPtr APIENTRY getServiceTestImpl();
	YSERVICE_API void APIENTRY releaseObjectTest(ServiceTestPtr obj);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif