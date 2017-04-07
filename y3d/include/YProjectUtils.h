#pragma once
#include "y3d.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"
#include <inode.h>
//#include "yloader.pb.h"
////#include "yloader.grpc.pb.h"

using namespace y3d;
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
		if (nodes.Count() > 0) {
			for (int i = 0; i < nodes.Count(); i++)
			{
				auto n = NodeEventNamespace::GetNodeByKey(nodes[i]);
				if (n == NULL) continue;
				if (n->Selected()) {
					auto client = y3d::YServiceMainWorker::NewStub(grpc::CreateChannel("127.0.0.1:37001", grpc::InsecureChannelCredentials()));
					YEvent ye;
					//ESelect es;
					//es.set_name(ws2s(n->GetName()));
					//es.set_isolate(false);
					ye.mutable_select()->set_name(ws2s(n->GetName()));
					ye.mutable_select()->set_isolate(false);
					grpc::ClientContext context;
					//y3d::ResponseEvent rep;
					//Status* status;
					// thay = async
					grpc::CompletionQueue cq_;
					client->AsyncDoEvent(&context, ye, &cq_);

					//auto status = client->DoEvent(&context, ye, &rep);


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


inline void registerCB() {
	auto mcb = new MyNodeEventCB();
	GetISceneEventManager()->RegisterCallback(mcb, 0, 0, 0);
}