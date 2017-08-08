#pragma once
#include "y3d.pb.h"
#include "yevent.pb.h"
#include <inode.h>
#include "LogClient.h"
#include <notify.h>
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
	using delegate_type = async_delegate<y3d::YEvent2, y3d::ResponseEvent2>::type;
public:
	//std::forward<delegate_type>( delegate_type::create<ClientType, &ClientType::AsyncDoEvent>(getClientInstance()))
	MyNodeEventCB() : client( getClientInstance()), events(DelegateList::DoEventDelegate() ) {
		LOG("MyNodeEventCB initializing...\n");		
		/*events.initialize([](auto e) {
			return e.debounce(std::chrono::milliseconds(50)) ;
		});*/
		events.initialize();
		//if (!init) {
		//	init = true;
		//	LOG("INIT first time\n");
		//	events.observable().subscribe([](auto e) {
		//		//y3d::ResponseEvent2& reply = e.first;

		//		LOG("On next {} \n", 1);
		//	}, [](auto e) {
		//		LOG("On error \n");
		//	}, []() {
		//		LOG("On completed\n");
		//	});
		//}
	}
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
	//rxcpp::observable< std::pair<y3d::ResponseEvent2, grpc::Status> > stream;
	//rxcpp::observable<y3d::YEvent2> stream;
	ClientType* client;
	EventBus<y3d::YEvent2, y3d::ResponseEvent2,5000> events;
};

extern MyNodeEventCB* mcb;
inline void registerCB() {
	mcb = new MyNodeEventCB();	
	auto callbackKey = GetISceneEventManager()->RegisterCallback(mcb, 0, 0, 0);	
	mcb->callBackKey = callbackKey;
	LOG("CALL BACK KEY = {}\n", callbackKey);
}

inline void unregisterCB() {
	GetISceneEventManager()->UnRegisterCallback(mcb->callBackKey);
	mcb->release();
	//GetISceneEventManager()->ReleaseInterface();
	LOG("Unregister CALL BACK KEY = {}\n", mcb->callBackKey);
	delete mcb;
	mcb = nullptr;
}


inline void pre_optimize(std::string oFileDir, std::string oFileName, std::string projectPath);

BOOL do_lowpoly(const ELowpoly el, bool make_temp=false);
BOOL do_unwrap(const EUnwrap eu);
BOOL do_pack(const EPacking ep);
BOOL do_isolate(const EIsolate iso);

BOOL save_test(InitTestParam lt);
BOOL load_test(InitTestParam lt);
