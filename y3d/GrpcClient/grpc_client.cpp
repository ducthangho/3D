#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotocd.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")
#endif
#include "grpc_client.h"

std::mutex mm_mutex;
ClientPtr clientPtr = nullptr;


ClientType* getClientInstance()
{
	ClientType* tmp = clientPtr.load();
	if (tmp == nullptr) {
		//LogClient* abc = new LogClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
		//if (!logClientPtr.compare_exchange_strong(nullptr, abc)) {
		//	delete abc;
		//}
		std::lock_guard<std::mutex> lock(mm_mutex);
		tmp = clientPtr.load();
		if (tmp == nullptr) {
			//auto stub(y3d::YServiceMainWorker::NewStub(grpc::CreateChannel(address, grpc::InsecureChannelCredentials())));
			//auto channel = grpc::CreateChannel(MainWorkerServerIP, grpc::InsecureChannelCredentials());
			//printf("Connection state = %d\n", channel->GetState(false));
			tmp = new ClientType(grpc::CreateChannel(MainWorkerServerIP, grpc::InsecureChannelCredentials()));
			printf("Create connection to server %s\n", MainWorkerServerIP.c_str());			
			//tmp->swap(stub);
			clientPtr.store(tmp);
		}
	}
	return tmp;
}

void reset()
{
	ClientType* oldPtr = clientPtr.load();
	if (!clientPtr.compare_exchange_strong(oldPtr, nullptr))
		delete oldPtr;
}

MainWorkerClient::MainWorkerClient(std::shared_ptr<grpc::Channel> channel) : stub_(y3d::YServiceMainWorker::NewStub(channel))
{
}
