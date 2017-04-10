#pragma once
#include "y3d.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"
#include <inode.h>
#include "LogClient.h"
//#include "yloader.pb.h"
////#include "yloader.grpc.pb.h"

using namespace y3d;
using namespace logserver;
//extern YSystem YSys;
//static std::vector<YEvent> received_e;
//YEvent current_e;


void getObjInfo(INode* node, YObject* yo);
void buildGroup(INode* node, YGroup* yg);
void buildGroup(INode* node, YArea* ya);
void ObjectFromMax(YAreaList* yal);
void DoYEvent(YEvent ye);
void NewYProject(const NewProjectParam* pp, ResponseNProject* rnp);
void LoadNProject(ResponseNProject* rnp);
void DoXrefHigh(ProjectInfo* pi);
//void registerCB();

class MyNodeEventCB : public INodeEventCallback {
public:
	void SelectionChanged(NodeKeyTab & 	nodes)
	{
		LOG("zz");
		if (nodes.Count() > 0) {
			auto client = y3d::YServiceMainWorker::NewStub(grpc::CreateChannel("127.0.0.1:37001", grpc::InsecureChannelCredentials()));
			for (int i = 0; i < nodes.Count(); i++)
			{
				auto n = NodeEventNamespace::GetNodeByKey(nodes[i]);
				if (n == NULL) continue;
				if (n->Selected()) {
					
					YEvent ye;
					//ESelect es;
					//es.set_name(ws2s(n->GetName()));
					//es.set_isolate(false);
					ye.mutable_select()->set_name(ws2s(n->GetName()));
					ye.mutable_select()->set_isolate(false);
					grpc::ClientContext context;
					y3d::ResponseEvent rep;
					//Status* status;
					// thay = async
					grpc::CompletionQueue cq_;
					LOG("abc\n");
					//client->AsyncDoEvent(&context, ye, &cq_);

					auto status = client->DoEvent(&context, ye, &rep);
					LOG("xong\n");

					/*		YEvent ye;
					ESelect es;
					es.set_name(ws2s(n->GetName()));
					es.set_isolate(false);
					ye.mutable_select()->CopyFrom(es);
					received_e.clear();
					received_e.push_back(ye);*/
					//mprintf(L"Test select: %s \n", n->GetName());
					break;
				}
			}
			//MessageBoxW(NULL, n->GetName(), L"TEST", MB_OK);
		}
	}
};

extern MyNodeEventCB mcb;
inline void registerCB() {
	GetISceneEventManager()->RegisterCallback(&mcb, 0, 0, 0);
}