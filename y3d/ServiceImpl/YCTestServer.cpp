#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotocd.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")
#endif

#include "YCTestServer.h"
#include "common.h"
#include "YLibs.h"


Status YServiceTestImpl::MTest1(ServerContext* context, const EmptyParam* request, EmptyParam* reply)
{
	Invoke([]() {
		mprintf(L"Test M1.\n");
		auto* ip = GetCOREInterface();
		INodeTab inodes;
		getSelNodeTab(inodes);
		std::string s(std::to_string(inodes.Count()));
		mprintf(s2ws(s).c_str());
		mflush();
		mprintf(L"Test M1.\n");
		log("here is test: ", inodes.Count());
		mflush();
	});
	return Status::OK;
}

void YServiceTestImpl::Initialize(void* codegen, void* gli)
{

}

YServiceTestImpl::~YServiceTestImpl()
{

}

YSERVICE_API ServiceTestSharedPtr APIENTRY getServiceTestPtr()
{
	return std::shared_ptr<AbstractTestService>(new YServiceTestImpl);
}

ServiceTestPtr APIENTRY getServiceTestImpl()
{
	return new YServiceTestImpl();
}

YSERVICE_API void APIENTRY releaseObjectTest(ServiceTestPtr obj)
{
	if (obj) delete obj;
}
