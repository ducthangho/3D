#pragma once
#include "y3d.pb.h"
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
