#pragma once

#include <grpc++/grpc++.h>
#include "ymax.grpc.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"
#include "maxscript/maxscript.h"
#include "tbb/concurrent_queue.h"
//#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
#include "xNormalSettings.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define WM_TRIGGER_CALLBACK WM_USER+4764

using yproto::ObjItem;
//using yproto::Point3;
//using yproto::Box3;
using yproto::NumFaceRange;
using yproto::ObjList;
using grpc::Status;
using yproto::YPrepare;

using y3d::Tools;
using y3d::RenameParam;
using y3d::ResultReply;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

//extern "C" {
//	void testMeo(int a, int b);
//}

typedef std::function<void()> FuncType;
typedef struct {
	FuncType fn;
	std::shared_ptr<std::promise<int>> ret;
} FunctionTask;
static tbb::concurrent_queue< FunctionTask > fn_q;

//std::mutex m;
//std::condition_variable cv;
//bool ready = false;



/*template<typename... Args, typename Lambda>
std::pair< void(*)(void*, Args...), std::decay_t<Lambda> > voidify(Lambda&& l) {
typedef typename std::decay<Lambda>::type Func;
return{
[](void* v, Args... args)->void {
Func* f = static_cast< Func* >(v);
(*f)(std::forward<Args>(args)...);
},
std::forward<Lambda>(l)
};
}

template<typename TCallable>
struct Wrapper
{
typedef Wrapper<TCallable> Self;

TCallable function;
void * data;

Wrapper(TCallable const & function, void * data)
: function(function), data(data)
{
// Nothing else
}

static void call(void * data)
{
Self * wrapper = reinterpret_cast<Self * >(data);
wrapper->function(wrapper->data);
}
};

void register_callback(void(*function)(void*), void * p) {
PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_TRIGGER_CALLBACK, (UINT_PTR)function, (UINT_PTR)p);
}
//*/


inline void PostCallback(void(*funcPtr)(UINT_PTR), UINT_PTR param)
{
	PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_TRIGGER_CALLBACK, (UINT_PTR)funcPtr, (UINT_PTR)param);
}

inline void executeWrapper() {
	FunctionTask ft;
	bool b = fn_q.try_pop(ft);
	if (b) {
		ft.fn();
		ft.ret->set_value(0);
	}
	else ft.ret->set_value(1);
}

inline void waitForReturn(std::shared_ptr<std::promise<int>>& ret) {
	auto future = ret->get_future();
	future.wait();

}

inline void Invoke(FuncType&& fn) {
	std::promise<int> pr;
	//std::shared_ptr< std::promise<int> > ptr(new std::promise<int>());
	FunctionTask ft = { fn, std::make_shared<std::promise<int>>() };
	fn_q.push(ft);
	PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_TRIGGER_CALLBACK, (UINT_PTR)(executeWrapper), (UINT_PTR)0);
	waitForReturn(ft.ret);
}

inline std::shared_ptr<std::promise<int>> InvokeAsync(FuncType&& fn) {
	std::promise<int> pr;
	//std::shared_ptr< std::promise<int> > ptr(new std::promise<int>());
	FunctionTask ft = { fn, std::make_shared<std::promise<int>>() };
	fn_q.push(ft);
	PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_TRIGGER_CALLBACK, (UINT_PTR)(executeWrapper), (UINT_PTR)0);
	return ft.ret;
}


inline int GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode /*=NULL*/)
{
	int i;
	if (i_currentNode == nullptr)
	{
		i_currentNode = GetCOREInterface()->GetRootNode();
	}
	else // IGame will crash 3ds Max if it is initialized with the root node.
	{
		i_nodeTab.AppendNode(i_currentNode);
	}
	for (i = 0; i < i_currentNode->NumberOfChildren(); i++)
	{
		GetSceneNodes(i_nodeTab, i_currentNode->GetChildNode(i));
	}
	return i_nodeTab.Count();
}

class YServiceImpl final : public Tools::Service {
	Status RenameObject(ServerContext* context, const RenameParam* request,
		ResultReply* reply) override
	{

		auto ret = InvokeAsync([]() -> void {
			INodeTab nRef;
			GetSceneNodes(nRef, nullptr);
			auto* ip = GetCOREInterface();

			auto* nameMaker = ip->NewNameMaker();
			GetCOREInterface13()->SetNameSuffixLength(2);
			std::map<MSTR, int> mnames;

			for (int i = 0; i < nRef.Count(); ++i) {
				auto* node = nRef[i];
				MSTR nodeName(node->GetName());
				MSTR tmp(nodeName);
				tmp.toLower();
				if (mnames.find(tmp) == mnames.end()) {
					mnames[tmp]++;
				}
				else if (mnames[tmp]++ > 0) {
					mprintf(L"Renaming %s ", nodeName);
					nameMaker->MakeUniqueName(nodeName);
					node->SetName(nodeName.data());
					tmp = nodeName;
					tmp.toLower();
					mnames[tmp]++;
					mprintf(L"to %s \n", nodeName);
				};
			}
			mprintf(L"Hello world\n");
		});
		reply->set_message("DDDD");
		//reply->message.set_message("ddd");
		waitForReturn(ret);
		return Status::OK;
		// ... (pre-existing code)
	}

	//Status SayHelloAgain(ServerContext* context, const RenameParam* request
	//	HelloReply* reply) override {
	//	std::string prefix("Hello again ");
	//	reply->set_message(prefix + request->name());
	//	return Status::OK;
	//}
};


class YPrepareServiceImpl final : public YPrepare::Service {


	Status MakeBox(ServerContext* context, const NumFaceRange* request,
		ObjList* reply) override {
		ObjItem *oItem = reply->add_objs();
		yproto::Point3 pos;
		yproto::Point3 pmin;
		yproto::Point3 pmax;
		pos.set_x(1);
		pos.set_y(2);
		pos.set_z(3);
		pmin.set_x(4);
		pmin.set_y(5);
		pmin.set_z(6);
		pmax.set_x(7);
		pmax.set_y(8);
		pmax.set_z(9);

		//yproto::Box3 bbox;
		//bbox.set_allocated_pmax(&pmax);
		//bbox.set_allocated_pmin(&pmin);
		//oItem->set_allocated_bbox(&bbox);
		//oItem->set_allocated_pos(&pos);
		//reply->add_objs();
		//mprintf(L"Finally I found you\n");
		int x = 1024;
		//auto closure = [x,pmin,pmax]() -> void {
		//	auto ip = GetCOREInterface();
		//	ip->ImportFromFile(L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\test.cap", 1);
		//	mprintf(L"Hello world %d\n", x);
		//	//auto &pmin = bbox.pmin();
		//	//auto &pmax = bbox.pmax();
		//	mprintf(L"PMIN = (%f, %f, %f)\n", pmin.x(),pmin.y(),pmin.z());
		//	mprintf(L"PMAX = (%f, %f, %f)\n", pmax.x(), pmax.y(), pmax.z());
		//};	
		//Invoke( std::forward<FuncType>(closure) );		
		auto ret = InvokeAsync([x, pmin, pmax]() -> void {
			auto ip = GetCOREInterface();
			//ip->ImportFromFile(L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\test.cap", 1);
			mprintf(L"Hello world %d\n", x);
			//auto &pmin = bbox.pmin();
			//auto &pmax = bbox.pmax();
			mprintf(L"PMIN = (%f, %f, %f)\n", pmin.x(), pmin.y(), pmin.z());
			mprintf(L"PMAX = (%f, %f, %f)\n", pmax.x(), pmax.y(), pmax.z());
			//testMeo(10, 6);
		});

		//waitForReturn();
		return Status::OK;
	}



};
#ifdef __cplusplus
}
#endif