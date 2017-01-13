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
#include "xnormal.pb.h"
#include "xml_format.h"
#include <fstream>
#include <google/protobuf/text_format.h>

//#include<TCHAR.H> // Implicit or explicit include
#ifdef __cplusplus
extern "C"
{
#endif

#define WM_ToStrRIGGER_CALLBACK WM_USER+4764


//using yproto::ObjItem;
//using yproto::NumFaceRange;
//using yproto::ObjList;

//using yproto::YPrepare;
//
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
//using namespace grpc;
using namespace y3d;
//using namespace y3d::yservices::
using namespace xnormal;

//extern "C" {
//	void testMeo(int a, int b);
//}

typedef std::function<void()> FuncType;
typedef struct {
	FuncType fn;
	std::shared_ptr<std::promise<int>> ret;
} FunctionTask;
static tbb::concurrent_queue< FunctionTask > fn_q;


inline std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

//std::mutex m;
//std::condition_variable cv;
//bool ready = false;



/*template<typename... Args, typename Lambda>
std::pair< void(*)(void*, Args...), std::decay_ToStr<Lambda> > voidify(Lambda&& l) {
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
PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_ToStrRIGGER_CALLBACK, (UINT_PTR)function, (UINT_PTR)p);
}
//*/


inline void PostCallback(void(*funcPtr)(UINT_PTR), UINT_PTR param)
{
	PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_ToStrRIGGER_CALLBACK, (UINT_PTR)funcPtr, (UINT_PTR)param);
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
	PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_ToStrRIGGER_CALLBACK, (UINT_PTR)(executeWrapper), (UINT_PTR)0);
	waitForReturn(ft.ret);
}

inline std::shared_ptr<std::promise<int>> InvokeAsync(FuncType&& fn) {
	std::promise<int> pr;
	//std::shared_ptr< std::promise<int> > ptr(new std::promise<int>());
	FunctionTask ft = { fn, std::make_shared<std::promise<int>>() };
	fn_q.push(ft);
	PostMessage(GetCOREInterface()->GetMAXHWnd(), WM_ToStrRIGGER_CALLBACK, (UINT_PTR)(executeWrapper), (UINT_PTR)0);
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

inline void getSelNodeTab(INodeTab& i_nodeTab) {
	auto* ip = GetCOREInterface();
	for (int i = 0; i < ip->GetSelNodeCount(); i++)
	{
		auto* node = ip->GetSelNode(i);
		i_nodeTab.AppendNode(node);
	}
}

class YServiceImpl final : public Tools::Service {

	Status MakeNode4Edit(ServerContext* context, const Make4TestParam* request,
		ResultReply* reply) override
	{
		Invoke([request]() -> void {
			INodeTab nRef;
			GetSceneNodes(nRef, nullptr);
			auto* ip = GetCOREInterface();

			auto oname = s2ws(request->oname());
			auto n = ip->GetINodeByName(oname.c_str());
			
			INodeTab nt1;
			INodeTab nt2;
			INodeTab nt3;
			INodeTab nt4;
			getSelNodeTab(nt1);
			ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, NODE_COPY, &nt2, &nt2);
			ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, NODE_COPY, &nt3, &nt3);
			ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, NODE_COPY, &nt4, &nt4);
			std::wstring tmp = n->GetName();
			auto lowStr = tmp+L"_low";
			auto hiStr = tmp + L"_high";
			auto cageStr = tmp + L"_cage";
			nt2[0]->SetName(lowStr.c_str());
			nt3[0]->SetName(hiStr.c_str());
			nt4[0]->SetName(cageStr.c_str());
			//auto* nameMaker = ip->NewNameMaker();
			//GetCOREInterface13()->SetNameSuffixLength(2);
			//std::map<MSTR, int> mnames;
			//ExecuteMAXScriptScript(L"yms.pre_optimize 'a' 'b'");
		});
		//waitForReturn(ret);
		return Status::OK;
	}

	Status RenameObject(ServerContext* context, const RenameParam* request,	ResultReply* reply) override
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
			ExecuteMAXScriptScript(L"yms.pre_optimize 'a' 'b'");
			//ip->max
			//y3d::setting::xnormal::Settings s;
			//std::string hiPoly = "D:\\teapot_hi.FBX";
			//std::string loPoly = "D:\\teapot_lo.obj";
			//createDefaultSettings(&s, hiPoly, loPoly, "D:\\teapot.png");
			//setSize(s, 1024, 1024);

			//bakeNormal(s,"D:\\example.xml");
		});
		reply->set_message("DDDD");
		//reply->message.set_message("ddd");
		waitForReturn(ret);
		return Status::OK;
		// ... (pre-existing code)
	}

	Status BakeNormal(ServerContext* context, const ENormal* enm, ResultReply* reply) override
	{
		//ENormal e = *enm;
		if (enm->has_normal_xnormal()) {
			Settings& s = (Settings&)enm->normal_xnormal();
			createDefaultSettings(&s, enm->highpoly(), enm->lowpoly(), enm->out_tex());
			setSize(s, enm->tex_size(), enm->tex_size());
			bakeNormal(s);
		}
		//a.has_normal_xnormal
		return Status::OK;
	}

	Status BatchOptimize(ServerContext* context, const BatchOptimizeParam* bp,	ResultReply* reply) override
	{
		Invoke([bp]() -> void {
			std::string tmp = "yms.pre_optimize";
			char buf[1000];
			sprintf(buf, "%s \"%s\" \"%s\"", tmp.c_str(), bp->folder().c_str(), bp->filename().c_str());
			auto cmd = s2ws(buf);
			//bp->folder.a
			//ExecuteMAXScriptScript(L"yms.pre_optimize 'a' 'b'");
			//mprintf(L"Command is %s\n",cmd.c_str());
			ExecuteMAXScriptScript(cmd.c_str());
		});
		return Status::OK;
	}
};

#ifdef __cplusplus
}
#endif