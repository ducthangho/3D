#pragma once


#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <condition_variable>
#include <future>

#include "tbb/concurrent_queue.h"
#include <vector>

#include <grpc++/grpc++.h>
#include "ymax.grpc.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"
#include "maxscript/maxscript.h"
#include "IGame/IGame.h"
#include "ObjectWrapper.h"
#include "3dsmaxsdk_preinclude.h"
#include <mnmesh.h>
#include <bitarray.h>
#include <mesh.h>
#include <inode.h>
#include "polyobj.h"
#include "triobj.h"
#include "patchobj.h"
#include <gfx.h>

//#include <mutex>

#include "xNormalSettings.h"
#include "xnormal.pb.h"
#include "xml_format.h"
#include <google/protobuf/text_format.h>

#include "ISceneEventManager.h"


//#define FMT_HEADER_ONLY
#include "fmt/format.h"

using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
//using namespace grpc;
using namespace y3d;
//using namespace y3d::yservices::
using namespace xnormal;
using namespace std;
using namespace fmt;

#define YCDEBUG 1

//#include <stdio.h>
//#using <System.dll>
//#using <mscorlib.dll> 
//using namespace System;
//using namespace System::IO;

class Time {
public:
	Time() : time(std::chrono::steady_clock::now()) {};
	long long duration() {
		auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - time);
		return elapsed.count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> time;
};

static std::vector<uint64_t> times(10);

inline void time(Time& t, int type) {
	times[type] += t.duration();
}


#define OBJECT_CREATION 0
#define BUILD_NORMALS 1
#define BUILD_EDGE 2
#define LOAD_NORMALS 3
#define OBJECT_SCENE_CREATE 4
#define LOAD_MESH 5
#define LOAD_UV 6
#define LOAD_SELECTION 7
#define EVAL_OBJECT 8
#define CREATE_NODE 9

#define TRIOBJ_T 1
#define POLYOBJ_T 2
#define PATCHOBJ_T 3
#define DUMMYOBJ_T 4
#define HELPEROBJ_T 5
#define CAMERA_T 6
#define LIGHT_T 7
#define GEOMETRY_T 8

#define CTL_CHARS  31
#define SINGLE_QUOTE 39

#define NO_GROUP  0
#define GROUP_HEAD 1
#define GROUP_OPEN_HEAD 2
#define GROUP_MEMBER 3
#define GROUP_OPEN_MEMBER 4

#define WM_ToStrRIGGER_CALLBACK WM_USER+4764

YSystem YSys;


std::vector<YEvent> received_e;
//YEvent current_e;

typedef std::function<void()> FuncType;
typedef struct {
	FuncType fn;
	std::shared_ptr<std::promise<int>> ret;
} FunctionTask;
static tbb::concurrent_queue< FunctionTask > fn_q;


// multi byte to wide char:
inline std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

// wide char to multi byte:
inline std::string ws2s(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
	return strTo;
}

inline void log(std::string& str) {
#ifdef YCDEBUG 
	std::wstring wstr = s2ws(str);
	mprintf(wstr.c_str());
#else
#endif
}

inline void log(std::wstring& str) {
#ifdef YCDEBUG 	
	mprintf(str.c_str());
#else
#endif
}


template <typename... Args>
inline void log(std::string& format_str, const Args & ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args);
	std::wstring wstr = s2ws(w.c_str()); // returns a C string (const char*)
	mprintf(wstr.c_str());
#else
#endif
}

template <typename... Args>
inline void log(char* format_str, const Args & ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args);
	std::wstring wstr = s2ws(w.c_str()); // returns a C string (const char*)
	mprintf(wstr.c_str());
#else
#endif
}

template <typename... Args>
inline void log(std::wstring& format_str, const Args & ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args);
	mprintf(w.c_str());
#else
#endif
}

template <typename... Args>
inline void log(wchar_t* format_str, const Args & ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args);
	mprintf(w.c_str());
#else
#endif
}


template <typename... Args>
inline std::wstring formatWS(StringRef format_str, const Args & ... args) {
	std::string s = fmt::format(format, args);
	return s2ws(s);
}

template <typename... Args>
inline std::wstring sprintfws(StringRef format_str, const Args & ... args) {
	std::wstring s;
	fmt::format(s, 1, 2, 3);
	return s2ws(s);
}


bool is_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

int dirExists(const char *path)
{
	struct stat info;

	if (stat(path, &info) != 0)
		return 0;
	else if (info.st_mode & S_IFDIR)
		return 1;
	else
		return 0;
}


inline void initSystem() {
	auto* ip = GetCOREInterface();
	auto setting_path = ip->GetDir(9);
	std::wstring yfolder;
	yfolder = setting_path;
	yfolder += L"\\y3d\\";
	YSys.set_working_folder(ws2s(yfolder));
	if (dirExists(YSys.working_folder().c_str()) == 0) {
		CreateDirectory(yfolder.c_str(), NULL);
	}
	auto path = yfolder;
	path += L"ysetting.y3d";
	//if (!is_file_exist(ws2s(path).c_str())) {
	//	ProjectInfo pi;
	//	YSys.mutable_default_info.CopyFrom(pi);
	//	PSetting ps;
	//	ps.max_recent = 5;
	//}
	fstream input(path, ios::in | ios::binary);
	if (!input) {
		ProjectInfo pi;
		YSys.mutable_default_info()->CopyFrom(pi);
		PSetting ps;
		ps.set_max_recent(5);
		YSys.mutable_default_setting()->CopyFrom(ps);
		YSys.clear_projects();
		fstream output(path, ios::out | ios::trunc | ios::binary);
		if (!YSys.SerializePartialToOstream(&output)) {
			MessageBoxW(NULL, L"Can not create setting file!", L"Error", MB_OK);
		}
	}
	else {
		if (!YSys.ParseFromIstream(&input)) {
			MessageBoxW(NULL, L"Can not read setting file!", L"Error", MB_OK);
		}
		
	}
}

inline void saveSystem() {
	std::string kk;
	kk.append(YSys.working_folder().c_str());
	kk.append("ysetting.y3d");
	//MessageBoxW(NULL, s2ws(kk).c_str(), L"sAVE", MB_OK);
	mprintf(L"aa:%s \n", s2ws(kk).c_str());
	fstream output(kk, ios::out | ios::trunc | ios::binary);
	if (!YSys.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not override setting file!", L"Error", MB_OK);
	}
}

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

inline Object* getObject(INode* node) {
	if (!node) return nullptr;
	Time t;
	auto& os = node->EvalWorldState(GetCOREInterface()->GetTime());
	time(t, EVAL_OBJECT);
	return os.obj;
}

template <class Obj>
inline Mesh& getMesh(Obj& o) {
	if (o.CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) {

		TriObject* ob = (TriObject*)o.ConvertToType(GetCOREInterface()->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));

		return ob->GetMesh();
	}
	return (Mesh&)(((TriObject&)o).GetMesh());
}

//template <class Obj>
inline int8_t getType(Object& o) {
	if (o.IsSubClassOf(polyObjectClassID)) {
		return POLYOBJ_T;
	}
	else if (o.IsSubClassOf(triObjectClassID)) {
		return TRIOBJ_T;
	}
	else if (o.IsSubClassOf(patchObjectClassID)) {
		return PATCHOBJ_T;
	}
	else if (o.SuperClassID() == CAMERA_CLASS_ID ) {
		return CAMERA_T;
	}
	else if (o.SuperClassID() == LIGHT_CLASS_ID) {
		return LIGHT_T;
	}
	else if (o.SuperClassID() == HELPER_CLASS_ID) {
		if (o.ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
			return DUMMYOBJ_T;
		return HELPEROBJ_T;
	}
	return TRIOBJ_T;
}

//template <class Obj>
inline int8_t getType(Object* o) {
	if (!o) return -1;
	if (o->IsSubClassOf(polyObjectClassID)) {
		return POLYOBJ_T;
	}
	else if (o->IsSubClassOf(triObjectClassID)) {
		return TRIOBJ_T;
	}
	else if (o->IsSubClassOf(patchObjectClassID)) {
		return PATCHOBJ_T;
	}
	else if (o->SuperClassID() == CAMERA_CLASS_ID) {
		return CAMERA_T;
	}
	else if (o->SuperClassID() == LIGHT_CLASS_ID) {
		return LIGHT_T;
	}
	else if (o->SuperClassID() == HELPER_CLASS_ID) {
		if (o->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
			return DUMMYOBJ_T;
		return HELPEROBJ_T;
	}
	return -1;
}

//template <>
inline int8_t getType(INode* o) {
	if (o == nullptr) return -1;
	return getType(o->EvalWorldState(GetCOREInterface()->GetTime()).obj);
}


//template <>
inline int8_t getType(ObjectWrapper& o) {
	if (o.GetPolyMesh()) {
		return POLYOBJ_T;
	}
	else if (o.GetTriMesh()) {
		return TRIOBJ_T;
	}
	else if (o.GetPatchMesh()) {
		return PATCHOBJ_T;
	}
	return -1;
}


//inline int8_t getSuperType(INode* o) {
//	if (o == nullptr) return -1;
//	return getSuperType(o->EvalWorldState(GetCOREInterface()->GetTime()).obj);
//}

inline int8_t getSuperType(Object* oo) {
	if (!oo) return -1;
	if (oo->SuperClassID() == GEOMOBJECT_CLASS_ID) {
		return GEOMETRY_T;
	}
	else if (oo->SuperClassID() == CAMERA_CLASS_ID) {
		return CAMERA_T;
	}
	else if (oo->SuperClassID() == LIGHT_CLASS_ID) {
		return LIGHT_T;
	}
	else if (oo->SuperClassID() == HELPER_CLASS_ID) {
		if (oo->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
			return DUMMYOBJ_T;
		return HELPEROBJ_T;
	}
	return -1;
}
//inline int GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode /*=NULL*/)
//{
//	int i;
//	if (i_currentNode == nullptr)
//	{
//		i_currentNode = GetCOREInterface()->GetRootNode();
//	}
//	else // IGame will crash 3ds Max if it is initialized with the root node.
//	{
//		i_nodeTab.AppendNode(i_currentNode);
//	}
//	for (i = 0; i < i_currentNode->NumberOfChildren(); i++)
//	{
//		GetSceneNodes(i_nodeTab, i_currentNode->GetChildNode(i));
//	}
//	return i_nodeTab.Count();
//}
//

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

inline void getObjInfo(INode* node, YObject* yo) {
	auto* oo = getObject(node);
	auto t = getSuperType(oo);

	if (t == GEOMETRY_T) {
		yo->set_otype(y3d::ObjectType::GEOMETRY);
		mprintf(L"Mesh: %s \n", node->GetName());
		ObjectWrapper ow;
		auto os = node->EvalWorldState(0);
		ow.Init(0, os);
		YMesh ym;
		ym.set_num_faces(ow.NumFaces());
		y3d::YMesh::MeshType mtype = y3d::YMesh::MeshType::YMesh_MeshType_Unknown;
		//mprintf(L"MType: %s \n", );
		auto cname = os.obj->ClassName();
		if (cname == L"Editable Mesh")
			mtype = YMesh_MeshType::YMesh_MeshType_Editable_Mesh;
		else if (cname == L"Editable Poly")
			mtype = YMesh_MeshType::YMesh_MeshType_Editable_Poly;
		else if (cname == L"Box")
			mtype = YMesh_MeshType::YMesh_MeshType_Box;
		else if (cname == L"Cone")
			mtype = YMesh_MeshType::YMesh_MeshType_Cone;
		else if (cname == L"Sphere")
			mtype = YMesh_MeshType::YMesh_MeshType_Sphere;
		else if (cname == L"GeoSphere")
			mtype = YMesh_MeshType::YMesh_MeshType_GeoSphere;
		else if (cname == L"Cylinder")
			mtype = YMesh_MeshType::YMesh_MeshType_Cylinder;
		else if (cname == L"Tube")
			mtype = YMesh_MeshType::YMesh_MeshType_Tube;
		else if (cname == L"Pyramid")
			mtype = YMesh_MeshType::YMesh_MeshType_Pyramid;
		else if (cname == L"Teapot")
			mtype = YMesh_MeshType::YMesh_MeshType_Teapot;
		else if (cname == L"Plane")
			mtype = YMesh_MeshType::YMesh_MeshType_Plane;
		//auto cname = node->clas
		ym.set_mtype(mtype);
		yo->mutable_mesh()->CopyFrom(ym);
	}
	else if (t == CAMERA_T) {
		yo->set_otype(y3d::ObjectType::CAMERA);
		mprintf(L"Cam: %s \n", node->GetName());
		auto* cam = (Camera*)oo;
		YCamera ycam;
		yo->mutable_camera()->CopyFrom(ycam);
	}
	else if (t == LIGHT_T) {
		yo->set_otype(y3d::ObjectType::LIGHT);
		auto* light = (Light*)oo;
		YLight ylight;
		yo->mutable_light()->CopyFrom(ylight);
	}
	else {
		yo->set_otype(ObjectType::OTHER);
	}
}

inline void buildGroup(INode* node, YGroup* yg) {
	if (node->NumberOfChildren() > 0) {
		for (int i = 0; i < node->NumberOfChildren(); i++)
		{
			auto child = node->GetChildNode(i);
			MSTR nodeName(child->GetName());
			if (child->IsGroupHead()) {
				auto nyg = yg->add_children();
				nyg->set_name(nodeName.ToCStr());
				buildGroup(child, nyg);
			}
			else {
				auto yo = yg->add_objs();
				yo->set_name(nodeName.ToCStr());
				getObjInfo(child, yo);
			}
		}
	}
}

inline void buildGroup(INode* node, YArea* ya) {
	if (node->NumberOfChildren() > 0) {
		for (int i = 0; i < node->NumberOfChildren(); i++)
		{
			auto child = node->GetChildNode(i);
			MSTR nodeName(child->GetName());
			if (child->IsGroupHead()) {
				auto nyg = ya->add_groups();
				nyg->set_name(nodeName.ToCStr());
				buildGroup(child, nyg);
			}
			else {
				auto yo = ya->add_objs();
				yo->set_name(nodeName.ToCStr());
				getObjInfo(child, yo);
			}
		}
	}
}


inline void ObjectFromMax(YAreaList* yal) {
	auto* ip = GetCOREInterface();
	auto ya = yal->add_areas();
	ya->set_name("Area 1");
	auto root = GetCOREInterface()->GetRootNode();
	buildGroup(root, ya);
	// switch to camera 1, Maximize Viewport Toggle, Views: Perspective User View, Viewport Visual Style Wireframe
	ExecuteMAXScriptScript(L"actionMan.executeAction 0 \"160\"");
	ExecuteMAXScriptScript(L"max vpt persp user;actionMan.executeAction 0 \"551\"");
	ip->SetViewportMax(true);
}

inline void NewYProject(const NewProjectParam* pp, ResponseNProject* rnp) {
	std::string tmp = "yms.pre_optimize";
	char buf[1000];
	std::sprintf(buf, "%s \"%s\" \"%s\"", tmp.c_str(), pp->folder().c_str(), pp->fname().c_str());
	auto cmd = s2ws(buf);
	ExecuteMAXScriptScript(cmd.c_str());
		
	YAreaList yal;
	ObjectFromMax(&yal);

	std::string path;
	path = pp->folder();
	//path += "\\y3d_" + pp->fname() + "\\yal.y3d";
	path += "\\" + pp->fname()+"_y3d\\yal.y3d";

	auto pi = YSys.add_projects();
	pi->CopyFrom(rnp->pinfo());
	saveSystem();
	fstream output(path, ios::out | ios::trunc | ios::binary);
	if (!yal.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not create List Area Object file!", L"Error", MB_OK);
	}
	rnp->mutable_yal()->CopyFrom(yal);
}

inline void LoadNProject(const NewProjectParam* pp, ResponseNProject* rnp) {
	auto* ip = GetCOREInterface();
	std::string pfolder;
	pfolder.append(pp->folder().c_str());
	pfolder += "\\"+pp->fname()+"_y3d\\";
	//pfolder.append("\\");
	std::string wpath;
	wpath.append(pfolder);
	wpath.append(pp->fname().c_str());
	wpath.append("90.max");
	std::string yal_path;
	yal_path.append(pfolder);
	yal_path.append("yal.y3d");
	if (ip->LoadFromFile(s2ws(wpath).c_str()) == 0) {
		MessageBoxW(NULL, L"Can not load working file!", L"Error", MB_OK);
	}
	else {
		fstream input(yal_path, ios::in | ios::binary);
		if (!input) {
			MessageBoxW(NULL, L"Can not load List Area Object file!", L"Error", MB_OK);
		}
		else {
			YAreaList yal;
			if (!yal.ParseFromIstream(&input)) {
				MessageBoxW(NULL, L"Can not read setting file!", L"Error", MB_OK);
			}
			else {
				rnp->mutable_yal()->CopyFrom(yal);
			}
		}
	}
}

inline void DoYEvent(YEvent ye) {
	if (ye.has_select()) {
		auto name_select = ye.select().name();
		std::string cmd = "select $" + name_select + ";";
		//MessageBoxW(NULL, s2ws(cmd).c_str(), L"TEST", MB_OK);
		ExecuteMAXScriptScript(s2ws(cmd).c_str());
	}
}

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
			/*		YEvent ye;
					ESelect es;
					es.set_name(ws2s(n->GetName()));
					es.set_isolate(false);
					ye.mutable_select()->CopyFrom(es);
					received_e.clear();
					received_e.push_back(ye);*/
					mprintf(L"Test select: %s \n", n->GetName());
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

