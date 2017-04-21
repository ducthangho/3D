#pragma once
#include "y3d.pb.h"
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
inline void xref_low(std::string project_path, std::string pname);
//void registerCB();

class MyNodeEventCB : public INodeEventCallback {
public:
	void SelectionChanged(NodeKeyTab & 	nodes);
};

extern MyNodeEventCB mcb;
inline void registerCB() {
	GetISceneEventManager()->RegisterCallback(&mcb, 0, 0, 0);
}

inline void pre_optimize(std::string oFileDir, std::string oFileName, std::string projectPath);

void do_lowpoly(const ELowpoly *el);
void do_unwrap(const EUnwrap *eu);
void do_pack(const EPacking *ep);