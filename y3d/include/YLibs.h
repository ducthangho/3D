#pragma once
#include <condition_variable>
#include <future>
#include <regex>
#include "tbb/concurrent_queue.h"
#include <vector>
#include <map>
#include "sdkinterface/interfaceinfo.h"

//#include <grpc++/grpc++.h>
//#include "y3d.pb.h"
//#include "ymax.pb.h"
//#include "ymax.grpc.pb.h"
//#include "yloader.pb.h"
////#include "yloader.grpc.pb.h"
//#include "xnormal.pb.h"
//#include "ysetting.pb.h"
//#include "yservice.pb.h"
//#include "yservice.grpc.pb.h"

#include "maxscript/maxscript.h"
//#include "IGame/IGame.h"
#include "ObjectWrapper.h"
#include <mnmesh.h>
#include <bitarray.h>
#include <mesh.h>
#include <maxtypes.h>
#include <inode.h>
#include "polyobj.h"
#include "triobj.h"
#include "patchobj.h"
#include <gfx.h>
#include "ICustAttribCollapseManager.h"
#include "ICustAttribContainer.h"
//#include "modstack.h"
//#include <mutex>
#include "3dsmaxUtils.h"
#include <plugapi.h>
#include "LogClient.h"

//#include "xNormalSettings.h"

#include "ISceneEventManager.h"

//#include "ServiceImpl.h"

extern TCHAR *GetString(int id);

#if MAX_RELEASE == MAX_RELEASE_R19
#define BATCH_PROOPTIMIZER_CLASS_ID Class_ID(0x5cdb0866, 0x34ed5c0e)
#endif

using namespace logserver;
//using grpc::Status;
//using grpc::Server;
//using grpc::ServerBuilder;
//using grpc::ServerContext;
//using namespace grpc;
//using namespace y3d;
//using namespace y3d::yservices::
//using namespace xnormal;
//using namespace std;
//using namespace fmt;


//#include <stdio.h>
//#using <System.dll>
//#using <mscorlib.dll> 
//using namespace System;
//using namespace System::IO;


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


inline void log(const std::string& str) {
#ifdef YCDEBUG 
	std::wstring wstr = s2ws(str);
	mprintf(wstr.c_str());
	mflush();
#else
#endif
}

inline void log(const char* str) {
#ifdef YCDEBUG 
	std::wstring wstr = s2ws(str);
	mprintf(wstr.c_str());
	mflush();
#else
#endif
}


inline void log(const std::wstring& str) {
#ifdef YCDEBUG 	
	mprintf(str.c_str());
	mflush();
#else
#endif
}

inline void log(const wchar_t* str) {
#ifdef YCDEBUG 	
	mprintf(str);
	mflush();
#else
#endif
}


template <typename... Args>
inline void log(const std::string& format_str, const Args ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args...);
	std::wstring wstr = s2ws(w.c_str()); // returns a C string (const char*)
	mprintf(wstr.c_str());
	mflush();
#else
#endif
}

template <typename... Args>
inline void log(const char* format_str, const Args& ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args...);
	std::wstring wstr = s2ws(w.c_str()); // returns a C string (const char*)
	mprintf(wstr.c_str());
	mflush();
#else
#endif
}


template <typename... Args>
inline void log(const std::wstring& format_str, const Args&  ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args...);
	mprintf(w.c_str());
	mflush();
#else
#endif
}

template <typename... Args>
inline void log(const wchar_t* format_str, const Args & ... args) {
#ifdef YCDEBUG 	
	std::wstring s = fmt::format(format_str, args...);
	mprintf(s.c_str());
	mflush();
#else
#endif
}



typedef std::function<void()> FuncType;
typedef struct {
	FuncType fn;
	std::shared_ptr<std::promise<int>> ret;
} FunctionTask;
static tbb::concurrent_queue< FunctionTask > fn_q;


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
	auto& os = node->EvalWorldState(GetCOREInterface()->GetTime());	
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

inline void Collapse(INode *node)
{
	ICustAttribCollapseManager * iCM = ICustAttribCollapseManager::GetICustAttribCollapseManager();

	Interface7 *ip = GetCOREInterface7();

	Object *oldObj = node->GetObjectRef();
	SClass_ID sc = oldObj->SuperClassID();

	theHold.Begin();
	if (theHold.Holding())
	{
		theHold.Put(new ObjManipulatorRestore);
		theHold.Put(new CollapseRestore());
		//theHold.Put(new UpdateUIRestore());
	}
	//LAM : added following to handle extension objects 8/2/00
	// NH 14 April 04: Added support for the maintaining CAs on stack collapse
	// LAM - 7/22/05 - Collapse does not affect WSMs, So we shouldn't enumerate them....
	bool ignoreBaseObjectCAs = false;
	if (iCM && iCM->GetCustAttribSurviveCollapseState())
	{
		NotifyCollapseMaintainCustAttribEnumProc2 PreNCEP(true, node);
		EnumGeomPipeline(&PreNCEP, oldObj);
	}
	else
	{
		NotifyCollapseEnumProc PreNCEP(true, node);
		EnumGeomPipeline(&PreNCEP, oldObj);
	}

	ip->StopCreating();
	ip->ClearPickMode();

	if (sc == GEN_DERIVOB_CLASS_ID)
	{
		// stack not empty, collapse stack 
		ObjectState os = oldObj->Eval(ip->GetTime());

		HoldSuspend hs;
		//LAM : modified following to handle polymesh 7/21/00
		Object *obj = os.obj->CollapseObject();
		// Now get rid of superfulous objects 		
		if (os.obj == obj)
		{
			// if we are cloning the base object, the clone will take care of the CAs
			Object *theBaseObject = oldObj->FindBaseObject();
			if (obj == theBaseObject)
				ignoreBaseObjectCAs = true;

			obj = (Object*)CloneRefHierarchy(obj);
		}
		hs.Resume();

		obj->SetSubSelState(0); // Reset the selection level to object level (in case it happens to have an edit mesh modifier
		oldObj->SetAFlag(A_LOCK_TARGET);
		node->SetObjectRef(obj);
		//ip->InvalidateObCache(node);
		node->NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);

		//LAM : added following to handle extension objects 8/2/00
		// Notify all mods and objs in the pipleine, that they have been collapsed
		// NH 14 April 04: Added support for the maintaining CAs on stack collapse
		if (iCM && iCM->GetCustAttribSurviveCollapseState())
		{
			NotifyCollapseMaintainCustAttribEnumProc2 PostNCEP(false, node, ignoreBaseObjectCAs, obj);
			EnumGeomPipeline(&PostNCEP, oldObj);
		}
		else
		{
			NotifyCollapseEnumProc PostNCEP(false, node, obj);
			EnumGeomPipeline(&PostNCEP, oldObj);
		}

		oldObj->ClearAFlag(A_LOCK_TARGET);
		oldObj->MaybeAutoDelete();

		if (theHold.Holding())
		{
			theHold.Put(new CollapseRestore(TRUE));
			theHold.Put(new ObjManipulatorRestore(TRUE));
		}
#ifdef IDS_COLLAPSE
		theHold.Accept(GetString(IDS_COLLAPSE));
#else
		theHold.Accept(_T("Collapsed"));
#endif
	}
}

inline void generateInterFacesID()
{
	/************************************************************************/
	/* show all the core interface id of 3ds max
	*/
	/************************************************************************/
	int num = NumCOREInterfaces();
	logserver::LOG("Num interface is {0}", num);

	int index = 127;
	logserver::LOG("cls");
	for (int index = 0; index < num; index++)
	{
		FPInterface* fpInterface = GetCOREInterfaceAt(index);
		FPInterfaceDesc* fpInterfaceDesc = fpInterface->GetDesc();
		MSTR internal_name = fpInterfaceDesc->internal_name;
		//logserver::LOG("internal name of interface at index {0} is {1}\n", index, internal_name);
		internal_name.toUpper();
		Interface_ID interfaceID = fpInterface->GetID();
		//logserver::LOG("internal name of interface at index {0} is {1}, interface_id is ({3},{4})\n", index, internal_name,a,b);
		ULONG a = interfaceID.PartA();
		ULONG b = interfaceID.PartB();
		std::stringstream stream;
		stream << std::hex << a;
		std::string hexa(stream.str());
		stream.str("");
		stream << std::hex << b;
		std::string hexb(stream.str());
		logserver::LOG("#define {0}_INTERFACE_ID  Interface_ID(0x{1},0x{2})\n", internal_name, hexa, hexb);
	}
}

inline std::string InterFaceID2S(Interface_ID interfaceID)
{
	std::stringstream buffer;
	ULONG a = interfaceID.PartA();
	ULONG b = interfaceID.PartB();
	std::stringstream stream;
	stream << std::hex << a;
	std::string hexa(stream.str());
	stream.str("");
	stream << std::hex << b;
	std::string hexb(stream.str());
	buffer << "Interface_ID(0x" << hexa << ",0x" << hexb << ")" << std::endl;
	return buffer.str();
	//logserver::LOG("#define {0}_INTERFACE_ID  Interface_ID(0x{1},0x{2})\n", internal_name, hexa, hexb);
}

inline void generateInterfaceFuntionsID(Interface_ID id)
{
	FPInterface* fpInterface = GetCOREInterface(id);
	FPInterfaceDesc* fpInterfaceDesc = fpInterface->GetDesc();
	MSTR internal_name = fpInterfaceDesc->internal_name;
	internal_name.toUpper();
	Tab<FPFunctionDef*> functions = fpInterfaceDesc->functions;
	auto numFunction = functions.Count();
	//define number of functions
	LOG("#define I{0}_NUMFUCNTIONS {1}\n", internal_name, numFunction);

	//LOG("- Num functions is {0}\n", numFunction);
	for (int i = 0; i < numFunction; i++)
	{
		auto f = functions[i];
		auto func_internalname = f->internal_name;
		func_internalname.toUpper();
		auto func_id = f->ID;
		//define function ID
		LOG("#define {0}_I{1} {2}\n", func_internalname, internal_name, func_id);
		//log(" + function number {0} have internal name is {1}, id is {2}\n", i, func_internalname, func_id);
	}

	Tab<FPPropDef*> props = fpInterfaceDesc->props;
	auto numProps = props.Count();
	LOG("#define I{0}_NUMPROPS {1}\n", internal_name, numProps);
	//LOG("- Num Properties is {0}\n", numProps);
	for (int i = 0; i < numProps; i++)
	{
		auto f = props[i];
		auto prop_internalname = f->internal_name;
		prop_internalname.toUpper();
		auto fucn_setterid = f->setter_ID;
		auto func_getterid = f->getter_ID;
		LOG("#define {0}_I{1}_GETTER {2}\n", prop_internalname, internal_name, func_getterid);
		LOG("#define {0}_I{1}_SETTER {2}\n", prop_internalname, internal_name, fucn_setterid);

		/*log(" + properties number {0} have internal name is {1},"
		" setterid is {2}, getterid is {3}\n", i, prop_internalname, fucn_setterid, func_getterid);*/
	}
}

extern std::map<int, std::string>  map;
inline std::map<int, std::string> generateParamType()
{
	//static std::map<int, std::string> map;
	if (!map.empty())   //wn: not thread safe
		return map;
	map[ParamType2::TYPE_PCNT_FRAC] = "ParamType2::TYPE_PCNT_FRAC";
	map[ParamType2::TYPE_WORLD] = "ParamType2::TYPE_WORLD";
	map[ParamType2::TYPE_STRING] = "ParamType2::TYPE_STRING";
	map[ParamType2::TYPE_FILENAME] = "ParamType2::TYPE_FILENAME";
	map[ParamType2::TYPE_HSV] = "ParamType2::TYPE_HSV";
	map[ParamType2::TYPE_COLOR_CHANNEL] = "ParamType2::TYPE_COLOR_CHANNEL";
	map[ParamType2::TYPE_TIMEVALUE] = "ParamType2::TYPE_TIMEVALUE";
	map[ParamType2::TYPE_RADIOBTN_INDEX] = "ParamType2::TYPE_RADIOBTN_INDEX";
	map[ParamType2::TYPE_MTL] = "ParamType2::TYPE_MTL";
	map[ParamType2::TYPE_TEXMAP] = "ParamType2::TYPE_TEXMAP";
	map[ParamType2::TYPE_BITMAP] = "ParamType2::TYPE_BITMAP";
	map[ParamType2::TYPE_INODE] = "ParamType2::TYPE_INODE";
	map[ParamType2::TYPE_REFTARG] = "ParamType2::TYPE_REFTARG";
	map[ParamType2::TYPE_INDEX] = "ParamType2::TYPE_INDEX";
	map[ParamType2::TYPE_MATRIX3] = "ParamType2::TYPE_MATRIX3";
	map[ParamType2::TYPE_PBLOCK2] = "ParamType2::TYPE_PBLOCK2";
	map[ParamType2::TYPE_POINT4] = "ParamType2::TYPE_POINT4";
	map[ParamType2::TYPE_FRGBA] = "ParamType2::TYPE_FRGBA";
	map[ParamType2::TYPE_ENUM] = "ParamType2::TYPE_ENUM";
	map[ParamType2::TYPE_VOID] = "ParamType2::TYPE_VOID";
	map[ParamType2::TYPE_INTERVAL] = "ParamType2::TYPE_INTERVAL";
	map[ParamType2::TYPE_ANGAXIS] = "ParamType2::TYPE_ANGAXIS";
	map[ParamType2::TYPE_QUAT] = "ParamType2::TYPE_QUAT";
	map[ParamType2::TYPE_RAY] = "ParamType2::TYPE_RAY";
	map[ParamType2::TYPE_POINT2] = "ParamType2::TYPE_POINT2";
	map[ParamType2::TYPE_BITARRAY] = "ParamType2::TYPE_BITARRAY";
	map[ParamType2::TYPE_CLASS] = "ParamType2::TYPE_CLASS";
	map[ParamType2::TYPE_MESH] = "ParamType2::TYPE_MESH";
	map[ParamType2::TYPE_OBJECT] = "ParamType2::TYPE_OBJECT";
	map[ParamType2::TYPE_CONTROL] = "ParamType2::TYPE_CONTROL";
	map[ParamType2::TYPE_POINT] = "ParamType2::TYPE_POINT";
	map[ParamType2::TYPE_TSTR] = "ParamType2::TYPE_TSTR";
	map[ParamType2::TYPE_IOBJECT] = "ParamType2::TYPE_IOBJECT";
	map[ParamType2::TYPE_INTERFACE] = "ParamType2::TYPE_INTERFACE";
	map[ParamType2::TYPE_HWND] = "ParamType2::TYPE_HWND";
	map[ParamType2::TYPE_NAME] = "ParamType2::TYPE_NAME";
	map[ParamType2::TYPE_COLOR] = "ParamType2::TYPE_COLOR";
	map[ParamType2::TYPE_FPVALUE] = "ParamType2::TYPE_FPVALUE";
	map[ParamType2::TYPE_VALUE] = "ParamType2::TYPE_VALUE";
	map[ParamType2::TYPE_DWORD] = "ParamType2::TYPE_DWORD";
	map[ParamType2::TYPE_bool] = "ParamType2::TYPE_bool";
	map[ParamType2::TYPE_INTPTR] = "ParamType2::TYPE_INTPTR";
	map[ParamType2::TYPE_INT64] = "ParamType2::TYPE_INT64";
	map[ParamType2::TYPE_DOUBLE] = "ParamType2::TYPE_DOUBLE";
	map[ParamType2::TYPE_BOX3] = "ParamType2::TYPE_BOX3";
	map[ParamType2::TYPE_BEZIERSHAPE] = "ParamType2::TYPE_BEZIERSHAPE";
	map[ParamType2::TYPE_FLOAT_TAB] = "ParamType2::TYPE_FLOAT_TAB";
	map[ParamType2::TYPE_INT_TAB] = "ParamType2::TYPE_INT_TAB";
	map[ParamType2::TYPE_RGBA_TAB] = "ParamType2::TYPE_RGBA_TAB";
	map[ParamType2::TYPE_POINT3_TAB] = "ParamType2::TYPE_POINT3_TAB";
	map[ParamType2::TYPE_BOOL_TAB] = "ParamType2::TYPE_BOOL_TAB";
	map[ParamType2::TYPE_ANGLE_TAB] = "ParamType2::TYPE_ANGLE_TAB";
	map[ParamType2::TYPE_PCNT_FRAC_TAB] = "ParamType2::TYPE_PCNT_FRAC_TAB";
	map[ParamType2::TYPE_WORLD_TAB] = "ParamType2::TYPE_WORLD_TAB";
	map[ParamType2::TYPE_STRING_TAB] = "ParamType2::TYPE_STRING_TAB";
	map[ParamType2::TYPE_FILENAME_TAB] = "ParamType2::TYPE_FILENAME_TAB";
	map[ParamType2::TYPE_HSV_TAB] = "ParamType2::TYPE_HSV_TAB";
	map[ParamType2::TYPE_COLOR_CHANNEL_TAB] = "ParamType2::TYPE_COLOR_CHANNEL_TAB";
	map[ParamType2::TYPE_TIMEVALUE_TAB] = "ParamType2::TYPE_TIMEVALUE_TAB";
	map[ParamType2::TYPE_RADIOBTN_INDEX_TAB] = "ParamType2::TYPE_RADIOBTN_INDEX_TAB";
	map[ParamType2::TYPE_MTL_TAB] = "ParamType2::TYPE_MTL_TAB";
	map[ParamType2::TYPE_TEXMAP_TAB] = "ParamType2::TYPE_TEXMAP_TAB";
	map[ParamType2::TYPE_BITMAP_TAB] = "ParamType2::TYPE_BITMAP_TAB";
	map[ParamType2::TYPE_INODE_TAB] = "ParamType2::TYPE_INODE_TAB";
	map[ParamType2::TYPE_REFTARG_TAB] = "ParamType2::TYPE_REFTARG_TAB";
	map[ParamType2::TYPE_INDEX_TAB] = "ParamType2::TYPE_INDEX_TAB";
	map[ParamType2::TYPE_MATRIX3_TAB] = "ParamType2::TYPE_MATRIX3_TAB";
	map[ParamType2::TYPE_PBLOCK2_TAB] = "ParamType2::TYPE_PBLOCK2_TAB";
	map[ParamType2::TYPE_POINT4_TAB] = "ParamType2::TYPE_POINT4_TAB";
	map[ParamType2::TYPE_FRGBA_TAB] = "ParamType2::TYPE_FRGBA_TAB";
	map[ParamType2::TYPE_VOID_TAB] = "ParamType2::TYPE_VOID_TAB";
	map[ParamType2::TYPE_INTERVAL_TAB] = "ParamType2::TYPE_INTERVAL_TAB";
	map[ParamType2::TYPE_ANGAXIS_TAB] = "ParamType2::TYPE_ANGAXIS_TAB";
	map[ParamType2::TYPE_QUAT_TAB] = "ParamType2::TYPE_QUAT_TAB";
	map[ParamType2::TYPE_RAY_TAB] = "ParamType2::TYPE_RAY_TAB";
	map[ParamType2::TYPE_POINT2_TAB] = "ParamType2::TYPE_POINT2_TAB";
	map[ParamType2::TYPE_BITARRAY_TAB] = "ParamType2::TYPE_BITARRAY_TAB";
	map[ParamType2::TYPE_CLASS_TAB] = "ParamType2::TYPE_CLASS_TAB";
	map[ParamType2::TYPE_MESH_TAB] = "ParamType2::TYPE_MESH_TAB";
	map[ParamType2::TYPE_OBJECT_TAB] = "ParamType2::TYPE_OBJECT_TAB";
	map[ParamType2::TYPE_CONTROL_TAB] = "ParamType2::TYPE_CONTROL_TAB";
	map[ParamType2::TYPE_POINT_TAB] = "ParamType2::TYPE_POINT_TAB";
	map[ParamType2::TYPE_TSTR_TAB] = "ParamType2::TYPE_TSTR_TAB";
	map[ParamType2::TYPE_IOBJECT_TAB] = "ParamType2::TYPE_IOBJECT_TAB";
	map[ParamType2::TYPE_INTERFACE_TAB] = "ParamType2::TYPE_INTERFACE_TAB";
	map[ParamType2::TYPE_HWND_TAB] = "ParamType2::TYPE_HWND_TAB";
	map[ParamType2::TYPE_NAME_TAB] = "ParamType2::TYPE_NAME_TAB";
	map[ParamType2::TYPE_COLOR_TAB] = "ParamType2::TYPE_COLOR_TAB";
	map[ParamType2::TYPE_FPVALUE_TAB] = "ParamType2::TYPE_FPVALUE_TAB";
	map[ParamType2::TYPE_VALUE_TAB] = "ParamType2::TYPE_VALUE_TAB";
	map[ParamType2::TYPE_DWORD_TAB] = "ParamType2::TYPE_DWORD_TAB";
	map[ParamType2::TYPE_bool_TAB] = "ParamType2::TYPE_bool_TAB";
	map[ParamType2::TYPE_INTPTR_TAB] = "ParamType2::TYPE_INTPTR_TAB";
	map[ParamType2::TYPE_INT64_TAB] = "ParamType2::TYPE_INT64_TAB";
	map[ParamType2::TYPE_DOUBLE_TAB] = "ParamType2::TYPE_DOUBLE_TAB";
	map[ParamType2::TYPE_BOX3_TAB] = "ParamType2::TYPE_BOX3_TAB";
	map[ParamType2::TYPE_BEZIERSHAPE_TAB] = "ParamType2::TYPE_BEZIERSHAPE_TAB";
	map[ParamType2::TYPE_INT_BR] = "ParamType2::TYPE_INT_BR";
	map[ParamType2::TYPE_BOOL_BR] = "ParamType2::TYPE_BOOL_BR";
	map[ParamType2::TYPE_ANGLE_BR] = "ParamType2::TYPE_ANGLE_BR";
	map[ParamType2::TYPE_PCNT_FRAC_BR] = "ParamType2::TYPE_PCNT_FRAC_BR";
	map[ParamType2::TYPE_WORLD_BR] = "ParamType2::TYPE_WORLD_BR";
	map[ParamType2::TYPE_COLOR_CHANNEL_BR] = "ParamType2::TYPE_COLOR_CHANNEL_BR";
	map[ParamType2::TYPE_TIMEVALUE_BR] = "ParamType2::TYPE_TIMEVALUE_BR";
	map[ParamType2::TYPE_RADIOBTN_INDEX_BR] = "ParamType2::TYPE_RADIOBTN_INDEX_BR";
	map[ParamType2::TYPE_INDEX_BR] = "ParamType2::TYPE_INDEX_BR";
	map[ParamType2::TYPE_RGBA_BR] = "ParamType2::TYPE_RGBA_BR";
	map[ParamType2::TYPE_BITMAP_BR] = "ParamType2::TYPE_BITMAP_BR";
	map[ParamType2::TYPE_POINT3_BR] = "ParamType2::TYPE_POINT3_BR";
	map[ParamType2::TYPE_HSV_BR] = "ParamType2::TYPE_HSV_BR";
	map[ParamType2::TYPE_REFTARG_BR] = "ParamType2::TYPE_REFTARG_BR";
	map[ParamType2::TYPE_MATRIX3_BR] = "ParamType2::TYPE_MATRIX3_BR";
	map[ParamType2::TYPE_POINT4_BR] = "ParamType2::TYPE_POINT4_BR";
	map[ParamType2::TYPE_FRGBA_BR] = "ParamType2::TYPE_FRGBA_BR";
	map[ParamType2::TYPE_ENUM_BR] = "ParamType2::TYPE_ENUM_BR";
	map[ParamType2::TYPE_INTERVAL_BR] = "ParamType2::TYPE_INTERVAL_BR";
	map[ParamType2::TYPE_ANGAXIS_BR] = "ParamType2::TYPE_ANGAXIS_BR";
	map[ParamType2::TYPE_QUAT_BR] = "ParamType2::TYPE_QUAT_BR";
	map[ParamType2::TYPE_RAY_BR] = "ParamType2::TYPE_RAY_BR";
	map[ParamType2::TYPE_POINT2_BR] = "ParamType2::TYPE_POINT2_BR";
	map[ParamType2::TYPE_BITARRAY_BR] = "ParamType2::TYPE_BITARRAY_BR";
	map[ParamType2::TYPE_MESH_BR] = "ParamType2::TYPE_MESH_BR";
	map[ParamType2::TYPE_POINT_BR] = "ParamType2::TYPE_POINT_BR";
	map[ParamType2::TYPE_TSTR_BR] = "ParamType2::TYPE_TSTR_BR";
	map[ParamType2::TYPE_COLOR_BR] = "ParamType2::TYPE_COLOR_BR";
	map[ParamType2::TYPE_FPVALUE_BR] = "ParamType2::TYPE_FPVALUE_BR";
	map[ParamType2::TYPE_DWORD_BR] = "ParamType2::TYPE_DWORD_BR";
	map[ParamType2::TYPE_bool_BR] = "ParamType2::TYPE_bool_BR";
	map[ParamType2::TYPE_INTPTR_BR] = "ParamType2::TYPE_INTPTR_BR";
	map[ParamType2::TYPE_INT64_BR] = "ParamType2::TYPE_INT64_BR";
	map[ParamType2::TYPE_DOUBLE_BR] = "ParamType2::TYPE_DOUBLE_BR";
	map[ParamType2::TYPE_BOX3_BR] = "ParamType2::TYPE_BOX3_BR";
	map[ParamType2::TYPE_BEZIERSHAPE_BR] = "ParamType2::TYPE_BEZIERSHAPE_BR";
	map[ParamType2::TYPE_FLOAT_TAB_BR] = "ParamType2::TYPE_FLOAT_TAB_BR";
	map[ParamType2::TYPE_INT_TAB_BR] = "ParamType2::TYPE_INT_TAB_BR";
	map[ParamType2::TYPE_RGBA_TAB_BR] = "ParamType2::TYPE_RGBA_TAB_BR";
	map[ParamType2::TYPE_POINT3_TAB_BR] = "ParamType2::TYPE_POINT3_TAB_BR";
	map[ParamType2::TYPE_BOOL_TAB_BR] = "ParamType2::TYPE_BOOL_TAB_BR";
	map[ParamType2::TYPE_ANGLE_TAB_BR] = "ParamType2::TYPE_ANGLE_TAB_BR";
	map[ParamType2::TYPE_PCNT_FRAC_TAB_BR] = "ParamType2::TYPE_PCNT_FRAC_TAB_BR";
	map[ParamType2::TYPE_WORLD_TAB_BR] = "ParamType2::TYPE_WORLD_TAB_BR";
	map[ParamType2::TYPE_STRING_TAB_BR] = "ParamType2::TYPE_STRING_TAB_BR";
	map[ParamType2::TYPE_FILENAME_TAB_BR] = "ParamType2::TYPE_FILENAME_TAB_BR";
	map[ParamType2::TYPE_HSV_TAB_BR] = "ParamType2::TYPE_HSV_TAB_BR";
	map[ParamType2::TYPE_COLOR_CHANNEL_TAB_BR] = "ParamType2::TYPE_COLOR_CHANNEL_TAB_BR";
	map[ParamType2::TYPE_TIMEVALUE_TAB_BR] = "ParamType2::TYPE_TIMEVALUE_TAB_BR";
	map[ParamType2::TYPE_RADIOBTN_INDEX_TAB_BR] = "ParamType2::TYPE_RADIOBTN_INDEX_TAB_BR";
	map[ParamType2::TYPE_MTL_TAB_BR] = "ParamType2::TYPE_MTL_TAB_BR";
	map[ParamType2::TYPE_TEXMAP_TAB_BR] = "ParamType2::TYPE_TEXMAP_TAB_BR";
	map[ParamType2::TYPE_BITMAP_TAB_BR] = "ParamType2::TYPE_BITMAP_TAB_BR";
	map[ParamType2::TYPE_INODE_TAB_BR] = "ParamType2::TYPE_INODE_TAB_BR";
	map[ParamType2::TYPE_REFTARG_TAB_BR] = "ParamType2::TYPE_REFTARG_TAB_BR";
	map[ParamType2::TYPE_INDEX_TAB_BR] = "ParamType2::TYPE_INDEX_TAB_BR";
	map[ParamType2::TYPE_MATRIX3_TAB_BR] = "ParamType2::TYPE_MATRIX3_TAB_BR";
	map[ParamType2::TYPE_POINT4_TAB_BR] = "ParamType2::TYPE_POINT4_TAB_BR";
	map[ParamType2::TYPE_FRGBA_TAB_BR] = "ParamType2::TYPE_FRGBA_TAB_BR";
	map[ParamType2::TYPE_TSTR_TAB_BR] = "ParamType2::TYPE_TSTR_TAB_BR";
	map[ParamType2::TYPE_ENUM_TAB_BR] = "ParamType2::TYPE_ENUM_TAB_BR";
	map[ParamType2::TYPE_INTERVAL_TAB_BR] = "ParamType2::TYPE_INTERVAL_TAB_BR";
	map[ParamType2::TYPE_ANGAXIS_TAB_BR] = "ParamType2::TYPE_ANGAXIS_TAB_BR";
	map[ParamType2::TYPE_QUAT_TAB_BR] = "ParamType2::TYPE_QUAT_TAB_BR";
	map[ParamType2::TYPE_RAY_TAB_BR] = "ParamType2::TYPE_RAY_TAB_BR";
	map[ParamType2::TYPE_POINT2_TAB_BR] = "ParamType2::TYPE_POINT2_TAB_BR";
	map[ParamType2::TYPE_BITARRAY_TAB_BR] = "ParamType2::TYPE_BITARRAY_TAB_BR";
	map[ParamType2::TYPE_CLASS_TAB_BR] = "ParamType2::TYPE_CLASS_TAB_BR";
	map[ParamType2::TYPE_MESH_TAB_BR] = "ParamType2::TYPE_MESH_TAB_BR";
	map[ParamType2::TYPE_OBJECT_TAB_BR] = "ParamType2::TYPE_OBJECT_TAB_BR";
	map[ParamType2::TYPE_CONTROL_TAB_BR] = "ParamType2::TYPE_CONTROL_TAB_BR";
	map[ParamType2::TYPE_POINT_TAB_BR] = "ParamType2::TYPE_POINT_TAB_BR";
	map[ParamType2::TYPE_IOBJECT_TAB_BR] = "ParamType2::TYPE_IOBJECT_TAB_BR";
	map[ParamType2::TYPE_INTERFACE_TAB_BR] = "ParamType2::TYPE_INTERFACE_TAB_BR";
	map[ParamType2::TYPE_HWND_TAB_BR] = "ParamType2::TYPE_HWND_TAB_BR";
	map[ParamType2::TYPE_NAME_TAB_BR] = "ParamType2::TYPE_NAME_TAB_BR";
	map[ParamType2::TYPE_COLOR_TAB_BR] = "ParamType2::TYPE_COLOR_TAB_BR";
	map[ParamType2::TYPE_FPVALUE_TAB_BR] = "ParamType2::TYPE_FPVALUE_TAB_BR";
	map[ParamType2::TYPE_VALUE_TAB_BR] = "ParamType2::TYPE_VALUE_TAB_BR";
	map[ParamType2::TYPE_DWORD_TAB_BR] = "ParamType2::TYPE_DWORD_TAB_BR";
	map[ParamType2::TYPE_bool_TAB_BR] = "ParamType2::TYPE_bool_TAB_BR";
	map[ParamType2::TYPE_INTPTR_TAB_BR] = "ParamType2::TYPE_INTPTR_TAB_BR";
	map[ParamType2::TYPE_INT64_TAB_BR] = "ParamType2::TYPE_INT64_TAB_BR";
	map[ParamType2::TYPE_DOUBLE_TAB_BR] = "ParamType2::TYPE_DOUBLE_TAB_BR";
	map[ParamType2::TYPE_BOX3_TAB_BR] = "ParamType2::TYPE_BOX3_TAB_BR";
	map[ParamType2::TYPE_BEZIERSHAPE_TAB_BR] = "ParamType2::TYPE_BEZIERSHAPE_TAB_BR";
	map[ParamType2::TYPE_RGBA_BV] = "ParamType2::TYPE_RGBA_BV";
	map[ParamType2::TYPE_POINT3_BV] = "ParamType2::TYPE_POINT3_BV";
	map[ParamType2::TYPE_HSV_BV] = "ParamType2::TYPE_HSV_BV";
	map[ParamType2::TYPE_INTERVAL_BV] = "ParamType2::TYPE_INTERVAL_BV";
	map[ParamType2::TYPE_BITMAP_BV] = "ParamType2::TYPE_BITMAP_BV";
	map[ParamType2::TYPE_MATRIX3_BV] = "ParamType2::TYPE_MATRIX3_BV";
	map[ParamType2::TYPE_POINT4_BV] = "ParamType2::TYPE_POINT4_BV";
	map[ParamType2::TYPE_FRGBA_BV] = "ParamType2::TYPE_FRGBA_BV";
	map[ParamType2::TYPE_ANGAXIS_BV] = "ParamType2::TYPE_ANGAXIS_BV";
	map[ParamType2::TYPE_QUAT_BV] = "ParamType2::TYPE_QUAT_BV";
	map[ParamType2::TYPE_RAY_BV] = "ParamType2::TYPE_RAY_BV";
	map[ParamType2::TYPE_POINT2_BV] = "ParamType2::TYPE_POINT2_BV";
	map[ParamType2::TYPE_BITARRAY_BV] = "ParamType2::TYPE_BITARRAY_BV";
	map[ParamType2::TYPE_MESH_BV] = "ParamType2::TYPE_MESH_BV";
	map[ParamType2::TYPE_POINT_BV] = "ParamType2::TYPE_POINT_BV";
	map[ParamType2::TYPE_TSTR_BV] = "ParamType2::TYPE_TSTR_BV";
	map[ParamType2::TYPE_COLOR_BV] = "ParamType2::TYPE_COLOR_BV";
	map[ParamType2::TYPE_FPVALUE_BV] = "ParamType2::TYPE_FPVALUE_BV";
	map[ParamType2::TYPE_CLASS_BV] = "ParamType2::TYPE_CLASS_BV";
	map[ParamType2::TYPE_BOX3_BV] = "ParamType2::TYPE_BOX3_BV";
	map[ParamType2::TYPE_BEZIERSHAPE_BV] = "ParamType2::TYPE_BEZIERSHAPE_BV";
	map[ParamType2::TYPE_FLOAT_TAB_BV] = "ParamType2::TYPE_FLOAT_TAB_BV";
	map[ParamType2::TYPE_INT_TAB_BV] = "ParamType2::TYPE_INT_TAB_BV";
	map[ParamType2::TYPE_RGBA_TAB_BV] = "ParamType2::TYPE_RGBA_TAB_BV";
	map[ParamType2::TYPE_POINT3_TAB_BV] = "ParamType2::TYPE_POINT3_TAB_BV";
	map[ParamType2::TYPE_BOOL_TAB_BV] = "ParamType2::TYPE_BOOL_TAB_BV";
	map[ParamType2::TYPE_ANGLE_TAB_BV] = "ParamType2::TYPE_ANGLE_TAB_BV";
	map[ParamType2::TYPE_PCNT_FRAC_TAB_BV] = "ParamType2::TYPE_PCNT_FRAC_TAB_BV";
	map[ParamType2::TYPE_WORLD_TAB_BV] = "ParamType2::TYPE_WORLD_TAB_BV";
	map[ParamType2::TYPE_STRING_TAB_BV] = "ParamType2::TYPE_STRING_TAB_BV";
	map[ParamType2::TYPE_FILENAME_TAB_BV] = "ParamType2::TYPE_FILENAME_TAB_BV";
	map[ParamType2::TYPE_HSV_TAB_BV] = "ParamType2::TYPE_HSV_TAB_BV";
	map[ParamType2::TYPE_COLOR_CHANNEL_TAB_BV] = "ParamType2::TYPE_COLOR_CHANNEL_TAB_BV";
	map[ParamType2::TYPE_TIMEVALUE_TAB_BV] = "ParamType2::TYPE_TIMEVALUE_TAB_BV";
	map[ParamType2::TYPE_RADIOBTN_INDEX_TAB_BV] = "ParamType2::TYPE_RADIOBTN_INDEX_TAB_BV";
	map[ParamType2::TYPE_MTL_TAB_BV] = "ParamType2::TYPE_MTL_TAB_BV";
	map[ParamType2::TYPE_TEXMAP_TAB_BV] = "ParamType2::TYPE_TEXMAP_TAB_BV";
	map[ParamType2::TYPE_BITMAP_TAB_BV] = "ParamType2::TYPE_BITMAP_TAB_BV";
	map[ParamType2::TYPE_INODE_TAB_BV] = "ParamType2::TYPE_INODE_TAB_BV";
	map[ParamType2::TYPE_REFTARG_TAB_BV] = "ParamType2::TYPE_REFTARG_TAB_BV";
	map[ParamType2::TYPE_INDEX_TAB_BV] = "ParamType2::TYPE_INDEX_TAB_BV";
	map[ParamType2::TYPE_MATRIX3_TAB_BV] = "ParamType2::TYPE_MATRIX3_TAB_BV";
	map[ParamType2::TYPE_POINT4_TAB_BV] = "ParamType2::TYPE_POINT4_TAB_BV";
	map[ParamType2::TYPE_FRGBA_TAB_BV] = "ParamType2::TYPE_FRGBA_TAB_BV";
	map[ParamType2::TYPE_PBLOCK2_TAB_BV] = "ParamType2::TYPE_PBLOCK2_TAB_BV";
	map[ParamType2::TYPE_VOID_TAB_BV] = "ParamType2::TYPE_VOID_TAB_BV";
	map[ParamType2::TYPE_TSTR_TAB_BV] = "ParamType2::TYPE_TSTR_TAB_BV";
	map[ParamType2::TYPE_ENUM_TAB_BV] = "ParamType2::TYPE_ENUM_TAB_BV";
	map[ParamType2::TYPE_INTERVAL_TAB_BV] = "ParamType2::TYPE_INTERVAL_TAB_BV";
	map[ParamType2::TYPE_ANGAXIS_TAB_BV] = "ParamType2::TYPE_ANGAXIS_TAB_BV";
	map[ParamType2::TYPE_QUAT_TAB_BV] = "ParamType2::TYPE_QUAT_TAB_BV";
	map[ParamType2::TYPE_RAY_TAB_BV] = "ParamType2::TYPE_RAY_TAB_BV";
	map[ParamType2::TYPE_POINT2_TAB_BV] = "ParamType2::TYPE_POINT2_TAB_BV";
	map[ParamType2::TYPE_BITARRAY_TAB_BV] = "ParamType2::TYPE_BITARRAY_TAB_BV";
	map[ParamType2::TYPE_CLASS_TAB_BV] = "ParamType2::TYPE_CLASS_TAB_BV";
	map[ParamType2::TYPE_MESH_TAB_BV] = "ParamType2::TYPE_MESH_TAB_BV";
	map[ParamType2::TYPE_OBJECT_TAB_BV] = "ParamType2::TYPE_OBJECT_TAB_BV";
	map[ParamType2::TYPE_CONTROL_TAB_BV] = "ParamType2::TYPE_CONTROL_TAB_BV";
	map[ParamType2::TYPE_POINT_TAB_BV] = "ParamType2::TYPE_POINT_TAB_BV";
	map[ParamType2::TYPE_IOBJECT_TAB_BV] = "ParamType2::TYPE_IOBJECT_TAB_BV";
	map[ParamType2::TYPE_INTERFACE_TAB_BV] = "ParamType2::TYPE_INTERFACE_TAB_BV";
	map[ParamType2::TYPE_HWND_TAB_BV] = "ParamType2::TYPE_HWND_TAB_BV";
	map[ParamType2::TYPE_NAME_TAB_BV] = "ParamType2::TYPE_NAME_TAB_BV";
	map[ParamType2::TYPE_COLOR_TAB_BV] = "ParamType2::TYPE_COLOR_TAB_BV";
	map[ParamType2::TYPE_FPVALUE_TAB_BV] = "ParamType2::TYPE_FPVALUE_TAB_BV";
	map[ParamType2::TYPE_VALUE_TAB_BV] = "ParamType2::TYPE_VALUE_TAB_BV";
	map[ParamType2::TYPE_DWORD_TAB_BV] = "ParamType2::TYPE_DWORD_TAB_BV";
	map[ParamType2::TYPE_bool_TAB_BV] = "ParamType2::TYPE_bool_TAB_BV";
	map[ParamType2::TYPE_INTPTR_TAB_BV] = "ParamType2::TYPE_INTPTR_TAB_BV";
	map[ParamType2::TYPE_INT64_TAB_BV] = "ParamType2::TYPE_INT64_TAB_BV";
	map[ParamType2::TYPE_DOUBLE_TAB_BV] = "ParamType2::TYPE_DOUBLE_TAB_BV";
	map[ParamType2::TYPE_BOX3_TAB_BV] = "ParamType2::TYPE_BOX3_TAB_BV";
	map[ParamType2::TYPE_BEZIERSHAPE_TAB_BV] = "ParamType2::TYPE_BEZIERSHAPE_TAB_BV";
	map[ParamType2::TYPE_FLOAT_BP] = "ParamType2::TYPE_FLOAT_BP";
	map[ParamType2::TYPE_INT_BP] = "ParamType2::TYPE_INT_BP";
	map[ParamType2::TYPE_BOOL_BP] = "ParamType2::TYPE_BOOL_BP";
	map[ParamType2::TYPE_ANGLE_BP] = "ParamType2::TYPE_ANGLE_BP";
	map[ParamType2::TYPE_PCNT_FRAC_BP] = "ParamType2::TYPE_PCNT_FRAC_BP";
	map[ParamType2::TYPE_WORLD_BP] = "ParamType2::TYPE_WORLD_BP";
	map[ParamType2::TYPE_COLOR_CHANNEL_BP] = "ParamType2::TYPE_COLOR_CHANNEL_BP";
	map[ParamType2::TYPE_TIMEVALUE_BP] = "ParamType2::TYPE_TIMEVALUE_BP";
	map[ParamType2::TYPE_RADIOBTN_INDEX_BP] = "ParamType2::TYPE_RADIOBTN_INDEX_BP";
	map[ParamType2::TYPE_INDEX_BP] = "ParamType2::TYPE_INDEX_BP";
	map[ParamType2::TYPE_ENUM_BP] = "ParamType2::TYPE_ENUM_BP";
	map[ParamType2::TYPE_DWORD_BP] = "ParamType2::TYPE_DWORD_BP";
	map[ParamType2::TYPE_bool_BP] = "ParamType2::TYPE_bool_BP";
	map[ParamType2::TYPE_INTPTR_BP] = "ParamType2::TYPE_INTPTR_BP";
	map[ParamType2::TYPE_INT64_BP] = "ParamType2::TYPE_INT64_BP";
	map[ParamType2::TYPE_DOUBLE_BP] = "ParamType2::TYPE_DOUBLE_BP";
	map[ParamType2::TYPE_KEYARG_MARKER] = "ParamType2::TYPE_KEYARG_MARKER";
	map[ParamType2::TYPE_MSFLOAT] = "ParamType2::TYPE_MSFLOAT";
	map[ParamType2::TYPE_UNSPECIFIED] = "ParamType2::TYPE_UNSPECIFIED";

	map[ParamType::TYPE_FLOAT] = "ParamType::TYPE_FLOAT";
	map[ParamType::TYPE_INT] = "ParamType::TYPE_INT";
	map[ParamType::TYPE_RGBA] = "ParamType::TYPE_RGBA";
	map[ParamType::TYPE_POINT3] = "ParamType::TYPE_POINT3";
	map[ParamType::TYPE_BOOL] = "ParamType::TYPE_BOOL";
	map[ParamType::TYPE_USER] = "ParamType::TYPE_USER";

	return map;
}

inline void generateInterfaceFuntionsID2(FPInterfaceDesc* fpInterfaceDesc)
{
	MSTR internal_name = fpInterfaceDesc->internal_name;
	internal_name.toUpper();
	Tab<FPFunctionDef*> functions = fpInterfaceDesc->functions;
	auto map = generateParamType();
	auto numFunction = functions.Count();
	//define number of functions
	LOG("// --------------------list functions of core interface {0}--------------------------\n", internal_name);
	LOG("// Number function of {0} core interface is {1}\n", internal_name, numFunction);
	LOG("#define I{0}_NUMFUCNTIONS {1}\n", internal_name, numFunction);

	log("// --------------------list functions of core interface {0}--------------------------\n", internal_name);
	log("// Number function of {0} core interface is {1}\n", internal_name, numFunction);
	log("#define I{0}_NUMFUCNTIONS {1}\n", internal_name, numFunction);
	//LOG("- Num functions is {0}\n", numFunction);
	for (int i = 0; i < numFunction; i++)
	{
		auto f = functions[i];
		auto func_internalname = f->internal_name;
		func_internalname.toUpper();  //hello
		auto func_id = f->ID;
		//define function ID
		LOG("// function ID of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, func_id);
		LOG("#define {0}_I{1} {2}\n", func_internalname, internal_name, func_id);

		log("// function ID of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, func_id);
		log("#define {0}_I{1} {2}\n", func_internalname, internal_name, func_id);
		auto params = f->params;
		auto numParams = params.Count();
		auto resultType = f->result_type;
		auto resultTypeEnumFormat = map[resultType];
		LOG("  // Result Type of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, resultTypeEnumFormat);
		LOG("  #define {0}_I{1}_RESULTTYPE {2}\n", func_internalname, internal_name, resultTypeEnumFormat);
		LOG("  // number parameter of fucntion {0} of core interface {1} is {2}\n", func_internalname, internal_name, numParams);
		LOG("  #define {0}_I{1}_NUMPARAMS {2}\n", func_internalname, internal_name, numParams);

		log("  // Result Type of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, resultTypeEnumFormat);
		log("  #define {0}_I{1}_RESULTTYPE {2}\n", func_internalname, internal_name, resultTypeEnumFormat);
		log("  // number parameter of fucntion {0} of core interface {1} is {2}\n", func_internalname, internal_name, numParams);
		log("  #define {0}_I{1}_NUMPARAMS {2}\n", func_internalname, internal_name, numParams);
		for (int i = 0; i < numParams; i++)
		{
			auto param = params[i];
			auto param_internal_name = param->internal_name;
			param_internal_name.toUpper();
			auto paramType = param->type;
			auto paramTypeEnumFormat = map[paramType];
			LOG("  // {0}, which is parameter number {1} of function\n// {2} of core interface {3} have param type is {4}\n", param_internal_name, i + 1, func_internalname, internal_name, paramTypeEnumFormat);
			LOG("  #define {0}_{1}_I{2}_PARAM{3}_TYPE {4}\n", param_internal_name, func_internalname, internal_name, i + 1, paramTypeEnumFormat);

			log("  #define {0}_{1}_I{2}_PARAM{3}_TYPE {4}\n", param_internal_name, func_internalname, internal_name, i + 1, paramTypeEnumFormat);
			log("  // {0}, which is parameter number {1} of function\n// {2} of core interface {3} have param type is {4}\n", param_internal_name, i + 1, func_internalname, internal_name, paramTypeEnumFormat);
			//LOG("  -- Param number {0} have internal name is {1}, paramType is {2}\n", i, finternal_name, paramTypeEnumFormat);
		}
		//log(" + function number {0} have internal name is {1}, id is {2}\n", i, func_internalname, func_id);
	}

	Tab<FPPropDef*> props = fpInterfaceDesc->props;
	auto numProps = props.Count();
	LOG("// ---------------------- List Properties of core interface {0} ----------------------\n", internal_name);
	log("// ---------------------- List Properties of core interface {0} ----------------------\n", internal_name);
	LOG("#define I{0}_NUMPROPS {1}\n", internal_name, numProps);
	log("#define I{0}_NUMPROPS {1}\n", internal_name, numProps);
	for (int i = 0; i < numProps; i++)
	{
		auto f = props[i];
		auto prop_internalname = f->internal_name;
		prop_internalname.toUpper();
		auto fucn_setterid = f->setter_ID;
		auto func_getterid = f->getter_ID;
		ParamType2 propTypes = f->prop_type;
		auto propTypes_enumFormat = map[(int)propTypes];
		//if (propTypes_enumFormat.empty()) propTypes_enumFormat ";
		LOG("// function_id of for get property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, func_getterid);
		log("// function_id of for get property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, func_getterid);
		LOG("#define {0}_I{1}_GETTER {2}\n", prop_internalname, internal_name, func_getterid);
		log("#define {0}_I{1}_GETTER {2}\n", prop_internalname, internal_name, func_getterid);
		LOG("// function_id of for set property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, fucn_setterid);
		log("// function_id of for set property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, fucn_setterid);
		LOG("#define {0}_I{1}_SETTER {2}\n", prop_internalname, internal_name, fucn_setterid);
		log("#define {0}_I{1}_SETTER {2}\n", prop_internalname, internal_name, fucn_setterid);
		//LOG("#define {0","ddd");
		if (!propTypes_enumFormat.empty()) {
			LOG("// parameter type of properties {0} of core interface {1} is {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			log("// parameter type of properties {0} of core interface {1} is {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			LOG("#define {0}_I{1}_TYPEPARAM {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			log("#define {0}_I{1}_TYPEPARAM {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
		}
		else {
			LOG("#define {0}_I{1}_TYPEPARAM_VOID {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			log("#define {0}_I{1}_TYPEPARAM_VOID {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
		}
		/*log(" + properties number {0} have internal name is {1},"
		" setterid is {2}, getterid is {3}\n", i, prop_internalname, fucn_setterid, func_getterid);*/
	}

	LOG("// ---------------------- List Enum of interfaces  ----------------------\n");
	Tab<FPEnum *> enumerations = fpInterfaceDesc->enumerations;
	auto numEnum = enumerations.Count();

	for (int i = 0; i < numEnum; i++)
	{
		auto e = enumerations[i];
		auto enumID = e->ID;
		auto enum_codes = e->enumeration;
		auto n = enum_codes.Count();

		std::string str = fmt::format("typedef enum class ENUM{0}_{1} {{", i + 1, internal_name);
		fmt::MemoryWriter w;
		w << str;
		LOG("/**");
		log("/**");
		for (int j = 0; j < n; j++)
		{
			auto enum_code = enum_codes[j];
			const wchar_t* name = enum_code.name;
			int code = enum_code.code;
			std::string s = ws2s(std::wstring(name));
			//std::string a = "ssss";
			//LOG("ffff\n");
			LOG("{0} - {1} Enum is {2} have code is {3}\n", i, j, s, code);
			log("{0} - {1} Enum is {2} have code is {3}\n", i, j, s, code);
			//w << 2;
			w << s << " = " << code << ",";
			//Printf("{0} - {1} Enum is ",i,j); LOG(s);LOG(" have code is {0}\n",code);
			//LOG("\n");
		}
		LOG("**/\n");
		log("**/\n");
		std::string str2 = fmt::format("}} {0}_ENUM{1};\n", internal_name, i + 1);
		w << str2;
		LOG(w.str());
		log(w.str());
	}
}

/************************************************************************/
/* generate core interface info with the interface_id
wn: this function only work if the provided interface_id is of "core" interface
*/
/************************************************************************/
inline void generateInterfaceFuntionsID2(Interface_ID id)
{
	FPInterface* fpInterface = GetCOREInterface(id);
	FPInterfaceDesc* fpInterfaceDesc = fpInterface->GetDesc();
	generateInterfaceFuntionsID2(fpInterfaceDesc);
}


inline void setInterFacePropertyTInt(int typeParam, FPInterface* fpInterface, int funcID, int value)
{
	FPParams p(1, typeParam, value);
	fpInterface->Invoke(funcID, &p);
}

inline void setInterFacePropertyTBool(int typeParam, FPInterface* fpInterface, int funcID, bool value)
{
	FPParams p(1, typeParam, value);
	fpInterface->Invoke(funcID, &p);
}

inline void setInterFacePropertyTString(int typeParam, FPInterface* fpInterface, int funcID, std::string value)
{
	MCHAR* desVal = const_cast<wchar_t*>((s2ws(value)).c_str());
	FPParams p(1, typeParam, desVal);
	fpInterface->Invoke(funcID, &p);
}

inline std::string uniquename(std::string name)
{	
	std::smatch m;

	//const char cstr[] = "subject";
	std::string s("subject");
	std::regex e ("(?!0)\\d+$");
	//std::regex e("123");

	std::regex_search(name, m, e);
	if (m.size() > 0)
	{
		std::string match = m.str();
		std::string prefix = m.prefix();
		int suffix = std::stoi(match) + 1;
		std::string r  = prefix + std::to_string(suffix);
		std::cout << "result is" << r << std::endl;
		return r;
	}
	/*for (int i = 0; i < m.size(); i++)
	{
		std::cout <<"string is "<<name<< "size is : " << m.size() << " and is " << m[i] << std::endl;
	}*/
	return name + "001";
}

inline std::wstring uniquename(std::wstring name)
{
	std::wsmatch m;
	std::wregex e(L"(?!0)\\d+$");
	//std::regex e("123");

	std::regex_search(name,m,e);
	if (m.size() > 0)
	{
		std::wstring match = m.str();
		std::wstring prefix = m.prefix();
		int suffix = std::stoi(match) + 1;
		std::wstring r = prefix + std::to_wstring(suffix);
		//std::wcout << "result is :" << r << std::endl;
		return r;
	}
	/*for (int i = 0; i < m.size(); i++)
	{
	std::cout <<"string is "<<name<< "size is : " << m.size() << " and is " << m[i] << std::endl;
	}*/
	return name + L"001";
}

// isolate=TRUE : IsolateSelection.EnterIsolateSelectionMode() 
// isolate=FALSE : IsolateSelection.ExitIsolateSelectionMode() 

inline void redrawViewPort()
{
	auto ip16 = GetCOREInterface16();
	GetCOREInterface16()->RedrawViews(ip16->GetTime());
}

inline bool setIsolate(BOOL isolate = TRUE) {
	FPInterface* fpInterface = GetCOREInterface(ISOLATESELECTION_INTERFACE_ID);
	FPValue result;
	if (isolate)
		fpInterface->Invoke(ENTERISOLATESELECTIONMODE_IISOLATESELECTION, result);
	else {
		fpInterface->Invoke(EXITISOLATESELECTIONMODE_IISOLATESELECTION, result);
		redrawViewPort();
	}
	return result.b;
}


inline std::wstring CreateLayer(INodeTab *nodes, std::wstring layer_name, BOOL isolate = TRUE) {
	auto iLayermanager = GetCOREInterface(LAYERMANAGER_INTERFACE_ID);
	FPInterface * ilayerProperties = nullptr;
	std::wstring name = layer_name.data();

	FPParams pLayerName(1, NAME_NEWLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE, name.data());
	FPValue result;
	iLayermanager->Invoke(NEWLAYERFROMNAME_ILAYERMANAGER, result, &pLayerName);
	ilayerProperties = result.fpi;

	while (ilayerProperties == nullptr) {
		name = uniquename(name);
		FPValue result;
		FPParams pLayerName(1, NAME_NEWLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE, name.data());
		iLayermanager->Invoke(NEWLAYERFROMNAME_ILAYERMANAGER, result, &pLayerName);
		ilayerProperties = result.fpi;
	};

	for (int i = 0; i < nodes->Count(); i++)
	{
		auto inode = (*nodes)[i];
		FPParams pAddNode(1, NODE_ADDNODE_ILAYERPROPERTIES_PARAM1_TYPE, inode);
		ilayerProperties->Invoke(ADDNODE_ILAYERPROPERTIES, &pAddNode);
	}

	if(isolate){
		//setIsolate(false);
		auto ip = GetCOREInterface16();
		ip->SelectNodeTab(*nodes, true);
		setIsolate(true);
	}

	return name;
}

inline void setIsolateLayer(const std::string layer_name) {
	//auto cmd = formatWS("yms.isolate_layer \"{0}\"", layer_name);
	//ExecuteMAXScriptScript(cmd.c_str());


	//auto cmd = formatWS("yms.delete_layer \"{0}\"", layer_name);
	//ExecuteMAXScriptScript(cmd.c_str());
	std::wstring a = s2ws(layer_name).data();

	auto ilayermanager = GetCOREInterface(LAYERMANAGER_INTERFACE_ID);
	//FPParams pDelLayerName(1, NAME_GETLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE, L"abcxy\0aa");
	//FPParams pDelLayerName(1, NAME_GETLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE, layer_name2.data());
	//FPParams pDelLayerName(1, NAME_GETLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE, s2ws(layer_name).data());
	FPParams pLayerName(1, NAME_GETLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE, a.data());
	FPValue result;
	ilayermanager->Invoke(GETLAYERFROMNAME_ILAYERMANAGER, result, &pLayerName);
	auto ilayperprop = result.fpi;
	if (ilayperprop != nullptr)
	{
		FPParams onOff(1, ONOFF_SELECT_ILAYERPROPERTIES_PARAM1_TYPE, true);
		ilayperprop->Invoke(SELECT_ILAYERPROPERTIES, &onOff);
		setIsolate(true);
	}

	//return result.b;
}

inline bool DeleteLayer(const std::string layer_name) {
	//auto cmd = formatWS("yms.delete_layer \"{0}\"", layer_name);
	//ExecuteMAXScriptScript(cmd.c_str());
	std::wstring a(s2ws(layer_name).data());
	std::wstring a2 = s2ws2(layer_name);
	
	auto ilayermanager = GetCOREInterface(LAYERMANAGER_INTERFACE_ID);
	//FPParams pDelLayerName(1, NAME_DELETELAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE, L"abcxy\0aa");
	//FPParams pDelLayerName(1, NAME_DELETELAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE, layer_name2.data());
	//FPParams pDelLayerName(1, NAME_DELETELAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE, s2ws(layer_name).data());
	FPParams pDelLayerName(1, NAME_DELETELAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE, a.data());
	//FPParams pDelLayerName(1, NAME_DELETELAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE, a2.data());
	//FPParams pDelLayerName(1, NAME_DELETELAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE,s2ws2(layer_name));
	FPValue result;	
	ilayermanager->Invoke(DELETELAYERBYNAME_ILAYERMANAGER, result, &pDelLayerName);
	return result.b;
}

inline void TestCreateLayer()
{
	INodeTab nodes;
	getSelNodeTab(nodes);
	CreateLayer(&nodes, L"abcxy");
}


#define createLight(x) CreateInstance(LIGHT_CLASS_ID,x)
#define createMaterial(x) CreateInstance(MATERIAL_CLASS_ID,x)
#define createTexMap(x) CreateInstance(TEXMAP_CLASS_ID,x)
#define createObjectSpaceModifier(x) CreateInstance(OSM_CLASS_ID,x)
#define createWorldSpaceModifier(x) CreateInstance(WSM_CLASS_ID,x)
#define createWSMObject(x) CreateInstance(WSM_OBJECT_CLASS_ID,x)
#define createCamera(x) CreateInstance(CAMERA_CLASS_ID,x)
#define createHelper(x) CreateInstance(HELPER_CLASS_ID,x)
#define createSystem(x) CreateInstance(SYSTEM_CLASS_ID,x)
#define createRefMaker(x) CreateInstance(REF_MAKER_CLASS_ID,x)
#define createRefTarget(x) CreateInstance(REF_TARGET_CLASS_ID,x)
#define createSceneImport(x) CreateInstance(SCENE_IMPORT_CLASS_ID,x)
#define createSceneExport(x) CreateInstance(SCENE_EXPORT_CLASS_ID,x)
#define createUtility(x) CreateInstance(UTILITY_CLASS_ID,x)
#define createUVGen(x) CreateInstance(UVGEN_CLASS_ID,x)
#define createGUP(x) CreateInstance(GUP_CLASS_ID,x)
#define createCusAttr(x) CreateInstance(CUST_ATTRIB_CLASS_ID,x)
#define createRadiosity(x) CreateInstance(RADIOSITY_CLASS_ID,x)

inline void* createBatchProOptimizer() {
	return CreateInstance(UTILITY_CLASS_ID, BATCH_PROOPTIMIZER_CLASS_ID);
}

class QuietMode
{
public:
	Interface* ip;
	bool a;
	QuietMode() {
		ip = GetCOREInterface16();
	}
	void set(bool quiet = true)
	{
		ip->SetQuietMode(quiet);
	}

	~QuietMode() {
		ip->SetQuietMode(false);
	}
};
