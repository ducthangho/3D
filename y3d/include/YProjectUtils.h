#pragma once
#include "y3d.pb.h"
#include "yevent.pb.h"
#include <inode.h>
#include "LogClient.h"
#include "grpc_client.h"
//#include "yloader.pb.h"
////#include "yloader.grpc.pb.h"

using namespace y3d;
using namespace logserver;
//extern YSystem YSys;
//static std::vector<YEvent> received_e;
//YEvent current_e;

#include "rx.hpp"

#include <future>
//#include <pplawait.h>
//#include <pplawait.h>

void getObjInfo(INode* node, YObject* yo);
void buildGroup(INode* node, YGroup* yg);
void buildGroup(INode* node, YArea* ya);
void ObjectFromMax(YAreaList* yal);
void DoYEvent(YEvent ye);
void NewYProject(const NewProjectParam* pp, ResponseNProject* rnp);
void LoadNProject(ResponseNProject* rnp);
void DoXrefHigh(ProjectInfo* pi);
inline void xref_low(std::string project_path, std::string pname);
//void registerCB();


class MyNodeEventCB : public INodeEventCallback {
public:
	void SelectionChanged(NodeKeyTab & 	nodes);
	void release() {		
		events.complete();
		events.release();
	};
	~MyNodeEventCB() {
		LOG("~MyNodeEventCB()\n");
	}
	DWORD callBackKey;
private:	
	grpc::CompletionQueue cq;
	EventBus<y3d::YEvent2, y3d::ResponseEvent2> events;
};

extern MyNodeEventCB mcb;
inline void registerCB() {	
	auto callbackKey = GetISceneEventManager()->RegisterCallback(&mcb, 0, 0, 0);
	mcb.callBackKey = callbackKey;
	LOG("CALL BACK KEY = {}\n", callbackKey);
}

inline void unregisterCB() {
	GetISceneEventManager()->UnRegisterCallback(mcb.callBackKey);	
	mcb.release();	
	GetISceneEventManager()->ReleaseInterface();
	LOG("Unregister CALL BACK KEY = {}\n", mcb.callBackKey);
}


inline void pre_optimize(std::string oFileDir, std::string oFileName, std::string projectPath);

BOOL do_lowpoly(const ELowpoly el);
BOOL do_unwrap(const EUnwrap eu);
BOOL do_pack(const EPacking ep);