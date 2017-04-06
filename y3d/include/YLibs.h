#pragma once
#include <condition_variable>
#include <future>

#include "tbb/concurrent_queue.h"
#include <vector>

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

//#include "xNormalSettings.h"

#include "ISceneEventManager.h"

//#include "ServiceImpl.h"

extern TCHAR *GetString(int id);

#if MAX_RELEASE == MAX_RELEASE_R19
#define BATCH_PROOPTIMIZER_CLASS_ID Class_ID(0x5cdb0866, 0x34ed5c0e)
#endif

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


inline void log(std::string& str) {
#ifdef YCDEBUG 
	std::wstring wstr = s2ws(str);
	mprintf(wstr.c_str());
	mflush();
#else
#endif
}

inline void log(char* str) {
#ifdef YCDEBUG 
	std::wstring wstr = s2ws(str);
	mprintf(wstr.c_str());
#else
#endif
}


inline void log(std::wstring& str) {
#ifdef YCDEBUG 	
	mprintf(str.c_str());
	mflush();
#else
#endif
}

inline void log(wchar_t* str) {
#ifdef YCDEBUG 	
	mprintf(str);
	mflush();
#else
#endif
}


template <typename... Args>
inline void log(std::string& format_str, const Args ... args) {
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
inline void log(char* format_str, const Args& ... args) {
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
inline void log(std::wstring& format_str, const Args&  ... args) {
#ifdef YCDEBUG 	
	fmt::MemoryWriter w;
	w.write(format_str, args...);
	mprintf(w.c_str());
	mflush();
#else
#endif
}

template <typename... Args>
inline void log(wchar_t* format_str, const Args & ... args) {
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


#define AUTODESK360_INTERFACE_ID  Interface_ID(0x52050f19,0x654018d5)
#define ISHADERMANAGERCREATOR_INTERFACE_ID  Interface_ID(0x56e81398,0x7f816f4f)
#define VISUALMS_INTERFACE_ID  Interface_ID(0x423d2cf2,0x526706b5)
#define MAXNETWORKERINTERFACE_INTERFACE_ID  Interface_ID(0x170b1314,0x540e0093)
#define AUTOTANGENTMAN_INTERFACE_ID  Interface_ID(0x13383fe1,0x326579c6)
#define INTERFACEIDREGISTRY_INTERFACE_ID  Interface_ID(0x50f8254e,0x269b1179)
#define MENUITEM_INTERFACE_ID  Interface_ID(0x2e926bd1,0x296e68f6)
#define OGSDIAGNOSTICS_INTERFACE_ID  Interface_ID(0x4fc324a4,0x22726963)
#define MANIP_INTERFACE_ID  Interface_ID(0x2c450aa2,0x7b9d0365)
#define NODEANDANIMS_INTERFACE_ID  Interface_ID(0x4ad13c9b,0x3ca97bec)
#define AUTODESK360INTERNAL_INTERFACE_ID  Interface_ID(0x755f643f,0x1c997148)
#define GIZMOSHAPE_INTERFACE_ID  Interface_ID(0x124e3169,0xf067ad4)
#define CUSTOMCONTROLSOPTIONS_INTERFACE_ID  Interface_ID(0xae7f4060,0xb4f5cdd)
#define SCHEMATICVIEW_INTERFACE_ID  Interface_ID(0x72f015c6,0x1dc62f2f)
#define EDITRENDERREGION_INTERFACE_ID  Interface_ID(0x4881007a,0x725d2e7b)
#define INODE_INTERFACE_ID  Interface_ID(0x67b113ca,0x34204b2b)
#define XMLANIMTREEENTRYLIST_INTERFACE_ID  Interface_ID(0x34d56bf5,0x1b2a7c80)
#define CHECKABORTCALLBACK_INTERFACE_ID  Interface_ID(0x666534,0x50101)
#define ASSETUSER_INTERFACE_ID  Interface_ID(0x32f7bccf,0xf1881566)
#define MENU_INTERFACE_ID  Interface_ID(0x4bd57e2e,0x6de57aeb)
#define IDISPLAYGAMMA_INTERFACE_ID  Interface_ID(0x2c65369,0x10be38b0)
#define XVIEWCHECKER_INTERFACE_ID  Interface_ID(0x3643717a,0x596f106f)
#define INODEGIPROPERTIES_INTERFACE_ID  Interface_ID(0x7fd53834,0x3b8525b5)
#define MXSDEBUGGER_INTERFACE_ID  Interface_ID(0x20a887fa,0xabcdef)
#define COLORMAN_INTERFACE_ID  Interface_ID(0x1bf46c90,0x18bf6199)
#define HELPSYSTEM_INTERFACE_ID  Interface_ID(0x3a4c02c3,0x4980c58)
#define GHOSTINGMANAGER_INTERFACE_ID  Interface_ID(0x173e6ee8,0x1f576633)
#define RETARGETDATA_INTERFACE_ID  Interface_ID(0x7cd15249,0xe980b37)
#define NULLINTERFACE_INTERFACE_ID  Interface_ID(0x0,0x0)
#define LIGHTINGUNITS_INTERFACE_ID  Interface_ID(0x7246dc,0x4384dc)
#define UTILITYPANEL_INTERFACE_ID  Interface_ID(0x7476dc,0x4364dc)
#define MRPBPARAMETERCLASSDESCCREATOR_INTERFACE_ID  Interface_ID(0x12454fe4,0x3da87ac6)
#define LOCKEDTRACKSMAN_INTERFACE_ID  Interface_ID(0x30384fe4,0x3d657ac6)
#define MISSINGUVCOORDINATES_INTERFACE_ID  Interface_ID(0x12616fdb,0x37ef729c)
#define REFHIERARCHY_INTERFACE_ID  Interface_ID(0x296e2793,0x247d12e4)
#define VIEWPANELMANAGER_INTERFACE_ID  Interface_ID(0xcbc11a6,0xab341400)
#define WORKSPACEMANAGER_INTERFACE_ID  Interface_ID(0xcdc11a6,0xab34dc00)
#define MOUSECONFIGMANAGER_INTERFACE_ID  Interface_ID(0x577c50a3,0x3ba8521b)
#define PARTICLEFLOW_INTERFACE_ID  Interface_ID(0xa41420b,0x1eb34500)
#define PLACEMENTTOOL_INTERFACE_ID  Interface_ID(0xaa6074ba,0x734851)
#define DAYLIGHTSIMULATIONUTILITIES_INTERFACE_ID  Interface_ID(0x307d152b,0x76f14d20)
#define CUIMOUSECONFIGMANAGERIMPLEMENT_INTERFACE_ID  Interface_ID(0x790c76c3,0x591c3988)
#define ROLLUP_INTERFACE_ID  Interface_ID(0x281a65e8,0x12db025d)
#define VIEWPORTBUTTONMGR_INTERFACE_ID  Interface_ID(0x47ab2195,0x22e8126e)
#define IINTERACTIONMODE_INTERFACE_ID  Interface_ID(0x993687ab,0x3cc77b11)
#define XMLANIMTREEENTRY_INTERFACE_ID  Interface_ID(0x5d25abd,0x36934bff)
#define VFBINTERFACE_INTERFACE_ID  Interface_ID(0x4da47cbb,0x70411c01)
#define ASSETMANAGER_INTERFACE_ID  Interface_ID(0x35331479,0x539570e8)
#define FILERESOLUTIONMANAGER_INTERFACE_ID  Interface_ID(0x675b2cab,0x19fe1fad)
#define CMDPANEL_INTERFACE_ID  Interface_ID(0x411753f6,0x69a93710)
#define TIPSYSTEM_INTERFACE_ID  Interface_ID(0x10c10195,0x224e0271)
#define MAXTHUMBNAILMGR_INTERFACE_ID  Interface_ID(0x74d43985,0x34886a28)
#define MANIPPLANE_INTERFACE_ID  Interface_ID(0x44460ea4,0xbf73be6)
#define SIMPLEMANIP_INTERFACE_ID  Interface_ID(0x617c41d4,0x6af06a5f)
#define MESHINSPECTOR_INTERFACE_ID  Interface_ID(0x438a2255,0xef961278)
#define UIACCESSOR_INTERFACE_ID  Interface_ID(0x79d41bc1,0x66594663)
#define SCENERADIOSITY_INTERFACE_ID  Interface_ID(0x6711e7a,0x5b504baa)
#define PARAMPUBLISHMGR_INTERFACE_ID  Interface_ID(0x416d0201,0xaa0fc23)
#define IRTSHADETREECOMPILER_INTERFACE_ID  Interface_ID(0x7eb345a1,0x2b67457e)
#define VALUECONVERTER_INTERFACE_ID  Interface_ID(0x19f37b9f,0x39ed5680)
#define _INTERFACE_ID  Interface_ID(0x490ddc99,0xbe8dc96)
#define PARAMWIRE_INTERFACE_ID  Interface_ID(0x490d0e99,0xbe87c96)
#define ASSIGNVERTEXCOLORS_INTERFACE_ID  Interface_ID(0x77870f30,0x4ed82b62)
#define MSZIP_INTERFACE_ID  Interface_ID(0x26492c82,0x10fc5210)
#define BITMAPLAYERMANAGER_INTERFACE_ID  Interface_ID(0x1563269c,0x7ec41d89)
#define RENDERMESSAGEMANAGER_INTERFACE_ID  Interface_ID(0x794e519b,0x3a74408a)
#define EXCHANGESTOREPACKAGEMANAGER_INTERFACE_ID  Interface_ID(0x8220250,0x2acc14fe)
#define MAXOPS_INTERFACE_ID  Interface_ID(0x64854123,0x7b9e551c)
#define VIEWPORTSSB_INTERFACE_ID  Interface_ID(0xa8d85d3d,0xcf44782)
#define CUSTATTRIBCOLLAPSEMANAGER_INTERFACE_ID  Interface_ID(0x7abc18a6,0x2a7e522b)
#define QUADMENU_INTERFACE_ID  Interface_ID(0x78b735e9,0x7c001f68)
#define MENTAL_RAY_PUBLIC_INTERFACE_INTERFACE_ID  Interface_ID(0x1ac964c8,0x18312c)
#define QUADMENUSETTINGS_INTERFACE_ID  Interface_ID(0x31561ddb,0x1a2f4619)
#define DRAGANDDROP_INTERFACE_ID  Interface_ID(0x51163ddb,0x2a4f1619)
#define MENUMAN_INTERFACE_ID  Interface_ID(0xadc20bd,0x7491741d)
#define ITABDIALOGMANAGER_INTERFACE_ID  Interface_ID(0x46465ead,0x6e3cbb)
#define _INTERFACE_ID  Interface_ID(0xaefb2400,0x1da74f68)
#define NITROUSGRAPHICSMANAGER_INTERFACE_ID  Interface_ID(0x73418f88,0x52c04a20)
#define DISPLAYMANAGER_INTERFACE_ID  Interface_ID(0x71218f68,0x52ab4a11)
#define ACTIONMAN_INTERFACE_ID  Interface_ID(0x4bb71a79,0x4e531e4f)
#define PEREZALLWEATHER_INTERFACE_ID  Interface_ID(0x57a47c50,0x1fb32e46)
#define SIMPLEFACEMANAGER_INTERFACE_ID  Interface_ID(0x5e62018,0xa73467e)
#define ASSEMBLYMGR_INTERFACE_ID  Interface_ID(0x576c528f,0x5cc23607)
#define GRIDPREFS_INTERFACE_ID  Interface_ID(0x7ccc77f5,0x344e7f37)
#define VIEWCUBEOPS_INTERFACE_ID  Interface_ID(0x1dd609f7,0x55eb2e35)
#define STEERINGWHEELSOPS_INTERFACE_ID  Interface_ID(0x1dd609f8,0x55eb2e34)
#define LOCKEDCOMPONENTSMAN_INTERFACE_ID  Interface_ID(0x63743a91,0x7cb759b)
#define FILELINKMGR_INTERFACE_ID  Interface_ID(0x7fdb459e,0x2e71308e)
#define DEFAULTACTIONS_INTERFACE_ID  Interface_ID(0x5be1b207,0x7813aa25)
#define HEIGHTMANAGER_INTERFACE_ID  Interface_ID(0x78632529,0xc179d875)
#define ACTIONITEMOVERRIDEMANAGER_INTERFACE_ID  Interface_ID(0x5e1b66eb,0x4bc034de)
#define IPARSERLOADER_INTERFACE_ID  Interface_ID(0x7fbe42d6,0x3e7f412b)
#define GLOBALDXDISPLAYMANAGER_INTERFACE_ID  Interface_ID(0x7ebe15d6,0x2b7b422b)
#define LOADSAVEANIMATION_INTERFACE_ID  Interface_ID(0x7e1b6067,0x6f470508)
#define MTLBROWSERFILTER_MANAGER_INTERFACE_ID  Interface_ID(0x371b4b10,0x6c715bbc)
#define INODEDISPLAYPROPERTIES_INTERFACE_ID  Interface_ID(0x44ef2511,0x6b071e99)
#define INODESHADINGPROPERTIES_INTERFACE_ID  Interface_ID(0x7acd13f1,0x2b2a128d)
#define SLATEDRAGDROPBRIDGE_INTERFACE_ID  Interface_ID(0x28150bb4,0x5c6d3030)
#define INODEBAKEPROPERTIES_INTERFACE_ID  Interface_ID(0x3e5d0f38,0x37e500ac)
#define RADIOSITYPREFERENCES_INTERFACE_ID  Interface_ID(0x54442e40,0x401621a4)
#define NVPXCONSTS_INTERFACE_ID  Interface_ID(0x548417a8,0x40f13528)
#define ISOLATESELECTION_INTERFACE_ID  Interface_ID(0x3ea56d31,0x3f034f9f)
#define HDIKSYS_INTERFACE_ID  Interface_ID(0x928ef7a2,0xd51ff97a)
#define IVIEWPORTSHADINGMGR_INTERFACE_ID  Interface_ID(0x7eba45d1,0x2b3d454b)
#define VIEWPORTBASICSETTINGS_INTERFACE_ID  Interface_ID(0x23fb7adf,0x1f33d1d3)
#define BONESYS_INTERFACE_ID  Interface_ID(0x438aff72,0xef9675ac)
#define VIEWPORTGISETTINGS_INTERFACE_ID  Interface_ID(0x23fde12f,0x2ad3d1ef)
#define VIEWPORTSHADOWSETTINGS_INTERFACE_ID  Interface_ID(0xe7d9afe2,0x4c7450a)
#define WORKINGPIVOT_INTERFACE_ID  Interface_ID(0x6ea64179,0x233915fd)
#define ILIGHTREF_INTERFACE_ID  Interface_ID(0x7ac123a1,0x2b56751a)
#define IXREFRECORD_INTERFACE_ID  Interface_ID(0x1a457bd1,0x7584ff0)
#define MAINTHREADTASKMANAGER_INTERFACE_ID  Interface_ID(0x79752833,0x743b438e)
#define MEDIT_INTERFACE_ID  Interface_ID(0x2c7b3f6e,0x16fb35d4)
#define SCENEMISSINGPLUGINS_INTERFACE_ID  Interface_ID(0xabba2102,0x10226207)
#define MENTAL_RAY_PREFERENCES_INTERFACE_ID  Interface_ID(0x594511cc,0x15505bac)
#define OPENEDGES_INTERFACE_ID  Interface_ID(0x116d7bc3,0x268227b5)
#define NAMEDSELECTIONSETMANAGER_INTERFACE_ID  Interface_ID(0x42a42b3,0x1cf1cc5)
#define NETWORKLICENSESTATUSMANAGER_INTERFACE_ID  Interface_ID(0x6b317bc6,0x535c5c0d)
#define INODEBAKEPROJPROPERTIES_INTERFACE_ID  Interface_ID(0x59d919a5,0x6fb90a85)
#define NVIDIARENDERERSHELPER_INTERFACE_ID  Interface_ID(0x52941f4d,0x67936471)
#define REGISTEREDDIALOGMGR_INTERFACE_ID  Interface_ID(0x3317507d,0x33fc71de)
#define BAKEELEMENTPROPERTIES_INTERFACE_ID  Interface_ID(0x94694761,0x17622221)
#define RENDERENHANCEMENTS_INTERFACE_ID  Interface_ID(0x28e11337,0x3862544b)
#define RENDERPRESETS_INTERFACE_ID  Interface_ID(0x40b33ff4,0x54463646)
#define QAT_INTERFACE_ID  Interface_ID(0x52941f9d,0x699364f1)
#define TMGIZMOS_INTERFACE_ID  Interface_ID(0xa0f66bc,0x7be465d2)
#define NODESELECTIONSET_INTERFACE_ID  Interface_ID(0x7eb12c12,0x353df2e1)
#define FRAMETAGMANAGER_INTERFACE_ID  Interface_ID(0x8cd73eea,0x688b1f11)
#define SCENEEXPOSURECONTROL_INTERFACE_ID  Interface_ID(0x1563269c,0x7ec41d84)
#define HAIR_INTERFACE_ID  Interface_ID(0x4aa37035,0xf3a686d)
#define TRACKBAR_INTERFACE_ID  Interface_ID(0x2aff3557,0x16aa714b)
#define TRACKVIEWS_INTERFACE_ID  Interface_ID(0x531c5f2c,0x6fdf29cf)
#define TRACKVIEW_INTERFACE_ID  Interface_ID(0x60fb7eef,0x1f6d6dd3)
#define BIPFIXER_INTERFACE_ID  Interface_ID(0xfcd6b18,0x6e714e23)
#define TRACKSELECTIONSETS_INTERFACE_ID  Interface_ID(0x18f36a84,0x1f572eb7)
#define OBJXREFS_INTERFACE_ID  Interface_ID(0x7ede1c65,0x353d271f)
#define AUTOSAVE_INTERFACE_ID  Interface_ID(0x4dab68f9,0x3e4032c6)
#define TIMESLIDER_INTERFACE_ID  Interface_ID(0x829e89e5,0x878ef6e5)
#define IMERGE_INTERFACE_ID  Interface_ID(0xec8ebb5f,0x6fa3f96b)
#define SUBSTMANAGER_INTERFACE_ID  Interface_ID(0x5d100fd,0x6ac76dab)
#define SCHEMATICVIEWS_INTERFACE_ID  Interface_ID(0x282360b2,0x1ffa57f6)
#define NODECLONEMGRTEST_INTERFACE_ID  Interface_ID(0x6d191c74,0x13059c2)
#define OBJXREFMGR_INTERFACE_ID  Interface_ID(0x38575e56,0x3b852a63)
#define LAYERMANAGER_INTERFACE_ID  Interface_ID(0x77ff1709,0x19186e96)
#define SCENECONVERTER_INTERFACE_ID  Interface_ID(0x5e3b0636,0x27af7de7)
#define BLOCKMGR_INTERFACE_ID  Interface_ID(0x327d3c71,0x542b7dac)
#define STYLEMGR_INTERFACE_ID  Interface_ID(0x3d8514f1,0x7c9252af)
#define BROWSERMGR_INTERFACE_ID  Interface_ID(0x65342378,0x4c540d3b)
#define BROWSER_INTERFACE_ID  Interface_ID(0x74a22be8,0x18e117c9)
#define BATCHRENDERMGR_INTERFACE_ID  Interface_ID(0x873b70be,0x4bef3bb8)
#define _INTERFACE_ID  Interface_ID(0x1d227363,0x5a723a41)
#define BITMAPPROXYMGR_INTERFACE_ID  Interface_ID(0x24e22528,0x601f3610)
#define CHANNELINFO_INTERFACE_ID  Interface_ID(0x438a1122,0xef966644)
#define RADIOSITYMESHOPS_INTERFACE_ID  Interface_ID(0x3a4e3329,0x290369ad)
#define SRR_EXPORTS_INTERFACE_ID  Interface_ID(0x6bd76cc2,0x4d6a556b)
#define REVITDBMANAGER_INTERFACE_ID  Interface_ID(0x6de97c24,0x40f32e75)
#define FLIGHTSTUDIO_INTERFACE_ID  Interface_ID(0x424c5545,0x524b0002)
#define INSTANCEMGR_INTERFACE_ID  Interface_ID(0x5ecd70b5,0x59092257)
#define BRUSHPRESETMGR_INTERFACE_ID  Interface_ID(0x586b1d59,0x454d0ae9)
#define BATCHPROOPTIMIZER_INTERFACE_ID  Interface_ID(0x24f54099,0x7462ee8)
#define SCENEEFFECTLOADER_INTERFACE_ID  Interface_ID(0x437e3629,0xef962968)
#define DAYLIGHTSYSTEMFACTORY_INTERFACE_ID  Interface_ID(0x5c665bc7,0x61b44a30)
#define SCENESTATEMGR_INTERFACE_ID  Interface_ID(0xeca7bd8e,0xd3934669)
#define SKINUTILS_INTERFACE_ID  Interface_ID(0x549a3355,0xef977711)
#define IVNORMALMGR_INTERFACE_ID  Interface_ID(0x28af453a,0x1eb87a50)
#define VISUALMSCA_INTERFACE_ID  Interface_ID(0x406a2f81,0x48460e09)
#define BOOLEANOBJECTMANAGER_INTERFACE_ID  Interface_ID(0x597001fa,0x3fac451a)
#define _INTERFACE_ID  Interface_ID(0x5aaa1218,0x35a74ae2)
#define CONTAINERS_INTERFACE_ID  Interface_ID(0xd951aee9,0x5769e48b)
#define CONTAINERPREFERENCES_INTERFACE_ID  Interface_ID(0xfa3b4e00,0xef1ba027)
#define FLOWRAYTRACEINTERFACE_INTERFACE_ID  Interface_ID(0x530943d4,0x280c5485)
#define EPOLYMANIPGRIP_INTERFACE_ID  Interface_ID(0x19ce513c,0x7b4a5132)
#define ONECLICKREVIT_INTERFACE_ID  Interface_ID(0x52554a2b,0x12757ea7)
#define PAINTDEFORMPRESETCONTEXT_INTERFACE_ID  Interface_ID(0x36764f94,0x56397b3e)
#define PAINTSOFTSELPRESETCONTEXT_INTERFACE_ID  Interface_ID(0x4b70352b,0x9454285)
#define LIGHTMETERMANAGER_INTERFACE_ID  Interface_ID(0x69905199,0x58840c61)
#define PSEUDOCOLORMANAGER_INTERFACE_ID  Interface_ID(0x7a942f30,0x44f4120e)
#define ICEFLOWFILEBIRTHSETUP_INTERFACE_ID  Interface_ID(0x5455499b,0x3be722d6)
#define ICEFLOWSHAPECONTROL_INTERFACE_ID  Interface_ID(0x32af1746,0x3cd775d4)
#define PARTICLEFLOWUTILITY_INTERFACE_ID  Interface_ID(0xef05a12,0x287626df)
#define IMETADATAMANAGER_INTERFACE_ID  Interface_ID(0xdcdd1738,0x15ee4791)
#define PFTPARTICLEVIEW_INTERFACE_ID  Interface_ID(0x39363dc9,0x1eb34500)
#define ICEFLOWSYSTEMFACTORY_INTERFACE_ID  Interface_ID(0xf285a9a,0x39587ad3)
#define RINGARRAY_INTERFACE_ID  Interface_ID(0x7a5a74ed,0x52304621)
#define PLUGINMANAGER_INTERFACE_ID  Interface_ID(0x5e3b07f6,0x273f7de7)
#define OFFSETMANAGER_INTERFACE_ID  Interface_ID(0x438aaa33,0xef9555aa)
#define CREASESETMANAGER_INTERFACE_ID  Interface_ID(0x2d321a81,0x171446c9)
#define PHYSXPANELINTERFACE_INTERFACE_ID  Interface_ID(0x20bc273d,0x6135868)
#define POP_INTERFACE_ID  Interface_ID(0x50b442e5,0x31c8552f)
#define ICCPHYSXINTEGRATOREXTFACTORY_INTERFACE_ID  Interface_ID(0x4c272d58,0x13e922bc)
#define NVPX_INTERFACE_ID  Interface_ID(0x4e0b7250,0x16f70e0a)
#define NETSERVER_INTERFACE_ID  Interface_ID(0x23c970f0,0x79263472)
#define BIPWORKBENCH_INTERFACE_ID  Interface_ID(0x78aa2c29,0x19a55d39)
#define BIPANALYZER_INTERFACE_ID  Interface_ID(0x14bf34cc,0x519c0633)
#define DXSHADERMANAGER_INTERFACE_ID  Interface_ID(0x6dce7429,0x200169ac)
#define BIPFILTER_INTERFACE_ID  Interface_ID(0x36ca302f,0x23d147a6)
#define ANIMLAYERMANAGER_INTERFACE_ID  Interface_ID(0xe343ef5,0x62944bc2)
#define IKSYS_INTERFACE_ID  Interface_ID(0x5b734601,0x7c7c7ece)
#define REACTIONMGR_INTERFACE_ID  Interface_ID(0x100940fa,0x43aa3a02)
#define RETIMERMAN_INTERFACE_ID  Interface_ID(0x753a5051,0x6c6740d6)
#define REVITIMPORTWORKFLOW_INTERFACE_ID  Interface_ID(0x620f3221,0x63d542aa)
#define FLIGHTSTUDIOIMPORT_INTERFACE_ID  Interface_ID(0x424c5545,0x524b0001)
#define AUTODESKMATERIALMANAGER_INTERFACE_ID  Interface_ID(0x88c0806,0x20481436)
#define IVNORMALMGR_INTERFACE_ID  Interface_ID(0x29a72d7f,0x6bff2956)
#define DWFEXPORTPREFERENCES_INTERFACE_ID  Interface_ID(0x230e5568,0xdd2a23)
#define FLIGHTSTUDIOEXPORT_INTERFACE_ID  Interface_ID(0x424c5545,0x524b0000)
#define LUMINOSITYUTIL_INTERFACE_ID  Interface_ID(0x452b54ac,0x57031d9e)
#define LIGHTINGANALYSISOVERLAYFACTORY_INTERFACE_ID  Interface_ID(0x5b5a6a27,0x26cf3458)
#define IBITMAPPAGER_INTERFACE_ID  Interface_ID(0x48e1041f,0x34c86c51)
#define IBITMAPPAGERINTERNAL_INTERFACE_ID  Interface_ID(0x4d5a42dd,0x688d0384)
#define ATSOPS_INTERFACE_ID  Interface_ID(0x20235822,0x2632197)
#define ATSCUSTOMDEPSOPS_INTERFACE_ID  Interface_ID(0x23b7a30,0x18b368ba)
#define IAUTOCAMMAX_INTERFACE_ID  Interface_ID(0x4bb65a82,0x27947a33)
#define GRIP_INTERFACE_ID  Interface_ID(0x59cb513c,0x7a0a5232)
#define DIALOGMONITOROPS_INTERFACE_ID  Interface_ID(0x5024223b,0x4c6026e6)
#define TVERTS_INTERFACE_ID  Interface_ID(0x28770efb,0x4b862591)
#define OVERLAPPEDUVWFACES_INTERFACE_ID  Interface_ID(0x39ba6e45,0x1fcf5023)
#define OVERLAPPINGVERTICES_INTERFACE_ID  Interface_ID(0x43d74abe,0x3c23abc)
#define ISOLATEDVERTICES_INTERFACE_ID  Interface_ID(0x4634351d,0x11654837)
#define FACESORIENTATION_INTERFACE_ID  Interface_ID(0x4a815af4,0x48c56402)
#define OVERLAPPINGFACES_INTERFACE_ID  Interface_ID(0x54dd1f44,0x4a2b0b56)
#define FLIPPEDUVWFACES_INTERFACE_ID  Interface_ID(0x6a1e3c73,0x45b72191)
#define MULTIPLEEDGES_INTERFACE_ID  Interface_ID(0x7a9f3524,0x49c804b4)
#define MATERIALPREVIEWSYSTEM_INTERFACE_ID  Interface_ID(0x21d47209,0x59eb64af)
#define PYTHON_INTERFACE_ID  Interface_ID(0x6ef3698f,0x570b4118)
#define PARAMCOLLECTOROPS_INTERFACE_ID  Interface_ID(0x2b152433,0x2e492929)
#define SME_INTERFACE_ID  Interface_ID(0x36a243ee,0x39b331d6)
#define WALKTHROUGHOPS_INTERFACE_ID  Interface_ID(0x399d085e,0x9825b4d)
#define PROJECTIONRENDERMGR_INTERFACE_ID  Interface_ID(0x45390e61,0x42de3b37)
#define PROJECTIONINTERSECTORMGR_INTERFACE_ID  Interface_ID(0x457d402d,0x36e81f43)
#define METADATAOBJECT_INTERFACE_ID  Interface_ID(0x2f422a24,0x137a4934)
#define MENTAL_RAY_STRING_OPTIONS_INTERFACE_ID  Interface_ID(0x50de7dd4,0xbe12e9e)
#define IRAY_STRING_OPTIONS_INTERFACE_ID  Interface_ID(0x6d0239,0x27a979cf)
#define PHYSSUNSKY_SHADERGENERATOR_INTERFACE_ID  Interface_ID(0x21797946,0x8f003b3)
#define BOOLEANEXPLORERMANAGER_INTERFACE_ID  Interface_ID(0x664f1e,0xca11e0)
#define MAXRIBBON_INTERFACE_ID  Interface_ID(0xaaf4f848,0xdcd26dc7)
#define CREASEEXPLORERMANAGER_INTERFACE_ID  Interface_ID(0x8742bf1e,0xca77c6f1)
#define DAYLIGHTSIMULATIONUIOPS_INTERFACE_ID  Interface_ID(0x5c2e03a0,0x81c429e)
#define NETWORKMANAGER_INTERFACE_ID  Interface_ID(0xa7352bd,0x61bb62a2)
#define MATERIALEXPLORERMANAGER_INTERFACE_ID  Interface_ID(0x87426721,0xcabb51f1)
#define SCENEEXPLORERMANAGER_INTERFACE_ID  Interface_ID(0x28406720,0x7a9b52f0)
#define DAYLIGHTSYSTEMFACTORY2_INTERFACE_ID  Interface_ID(0x688f66a6,0x423a37aa)
#define STATUSPANEL_INTERFACE_ID  Interface_ID(0x94357f0,0x623e71c2)
#define NETJOBSERVERINFO_INTERFACE_ID  Interface_ID(0x53d0125e,0x32275c22)
#define NETFRAMEINFO_INTERFACE_ID  Interface_ID(0x6ccf315e,0x62cb3e68)
#define NETRENDERELEMENT_INTERFACE_ID  Interface_ID(0x53934099,0x46694bff)
#define NETSTATUS_INTERFACE_ID  Interface_ID(0x1ce76e2f,0x44705312)
#define NETJOB_INTERFACE_ID  Interface_ID(0x31171e7e,0x7c766174)
#define MEMSTREAMMGR_INTERFACE_ID  Interface_ID(0x270517e7,0x29f449e3)
#define NETMANAGER_INTERFACE_ID  Interface_ID(0x252f7d20,0x33437aaa)
#define NETRENDER_INTERFACE_ID  Interface_ID(0x6da8475d,0x722c4b24)
#define NETCREATEHELPERS_INTERFACE_ID  Interface_ID(0x1d65311,0x7e6d8b)
#define MXSUNITRESULTS_INTERFACE_ID  Interface_ID(0x67894321,0x91827364)
#define INODEMENTALRAYPROPERTIES_INTERFACE_ID  Interface_ID(0x1c763921,0x4d542752)


#define IBATCHPROOPTIMIZER_NUMFUCNTIONS 1
#define BATCH_IBATCHPROOPTIMIZER 86
#define IBATCHPROOPTIMIZER_NUMPROPS 43
#define SOURCEFILEMODE_IBATCHPROOPTIMIZER_GETTER 0
#define SOURCEFILEMODE_IBATCHPROOPTIMIZER_SETTER 1
#define SOURCEFILEMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define SOURCEFILEFILES_IBATCHPROOPTIMIZER_GETTER 2
#define SOURCEFILEFILES_IBATCHPROOPTIMIZER_SETTER 3
#define SOURCEFILEFILES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_FILENAME_TAB_BV
#define SOURCEFILEDIR_IBATCHPROOPTIMIZER_GETTER 4
#define SOURCEFILEDIR_IBATCHPROOPTIMIZER_SETTER 5
#define SOURCEFILEDIR_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
#define SOURCEFILESUBDIR_IBATCHPROOPTIMIZER_GETTER 6
#define SOURCEFILESUBDIR_IBATCHPROOPTIMIZER_SETTER 7
#define SOURCEFILESUBDIR_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_GETTER 8
#define OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_SETTER 9
#define OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_GETTER 10
#define OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_SETTER 11
#define OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_GETTER 12
#define OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_SETTER 13
#define OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_GETTER 14
#define OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_SETTER 15
#define OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_GETTER 16
#define OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_SETTER 17
#define OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZATIONLEVELSTEPS_IBATCHPROOPTIMIZER_GETTER 18
#define OPTIMIZATIONLEVELSTEPS_IBATCHPROOPTIMIZER_SETTER 19
#define OPTIMIZATIONLEVELSTEPS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_GETTER 20
#define OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_SETTER 21
#define OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_GETTER 22
#define OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_SETTER 23
#define OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define OPTIMIZEHIDDEN_IBATCHPROOPTIMIZER_GETTER 24
#define OPTIMIZEHIDDEN_IBATCHPROOPTIMIZER_SETTER 25
#define OPTIMIZEHIDDEN_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define MERGEPOINTS_IBATCHPROOPTIMIZER_GETTER 26
#define MERGEPOINTS_IBATCHPROOPTIMIZER_SETTER 27
#define MERGEPOINTS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define MERGEPOINTSTHRESHOLD_IBATCHPROOPTIMIZER_GETTER 28
#define MERGEPOINTSTHRESHOLD_IBATCHPROOPTIMIZER_SETTER 29
#define MERGEPOINTSTHRESHOLD_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
#define MERGEFACES_IBATCHPROOPTIMIZER_GETTER 30
#define MERGEFACES_IBATCHPROOPTIMIZER_SETTER 31
#define MERGEFACES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define MERGEFACESTHRESHOLD_IBATCHPROOPTIMIZER_GETTER 32
#define MERGEFACESTHRESHOLD_IBATCHPROOPTIMIZER_SETTER 33
#define MERGEFACESTHRESHOLD_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
#define KEEPMATERIALBOUNDARIES_IBATCHPROOPTIMIZER_GETTER 34
#define KEEPMATERIALBOUNDARIES_IBATCHPROOPTIMIZER_SETTER 35
#define KEEPMATERIALBOUNDARIES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define COMPACTFACES_IBATCHPROOPTIMIZER_GETTER 36
#define COMPACTFACES_IBATCHPROOPTIMIZER_SETTER 37
#define COMPACTFACES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define PREVENTFLIPPEDNORMALS_IBATCHPROOPTIMIZER_GETTER 38
#define PREVENTFLIPPEDNORMALS_IBATCHPROOPTIMIZER_SETTER 39
#define PREVENTFLIPPEDNORMALS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define KEEPVERTEXCOLORS_IBATCHPROOPTIMIZER_GETTER 40
#define KEEPVERTEXCOLORS_IBATCHPROOPTIMIZER_SETTER 41
#define KEEPVERTEXCOLORS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define KEEPVERTEXCOLORSBOUNDARIES_IBATCHPROOPTIMIZER_GETTER 42
#define KEEPVERTEXCOLORSBOUNDARIES_IBATCHPROOPTIMIZER_SETTER 43
#define KEEPVERTEXCOLORSBOUNDARIES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define VERTEXCOLORSTOLERANCE_IBATCHPROOPTIMIZER_GETTER 44
#define VERTEXCOLORSTOLERANCE_IBATCHPROOPTIMIZER_SETTER 45
#define VERTEXCOLORSTOLERANCE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define KEEPTEXTURE_IBATCHPROOPTIMIZER_GETTER 46
#define KEEPTEXTURE_IBATCHPROOPTIMIZER_SETTER 47
#define KEEPTEXTURE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define KEEPUVBOUNDARIES_IBATCHPROOPTIMIZER_GETTER 48
#define KEEPUVBOUNDARIES_IBATCHPROOPTIMIZER_SETTER 49
#define KEEPUVBOUNDARIES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define UVTOLERANCE_IBATCHPROOPTIMIZER_GETTER 50
#define UVTOLERANCE_IBATCHPROOPTIMIZER_SETTER 51
#define UVTOLERANCE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
#define KEEPNORMALS_IBATCHPROOPTIMIZER_GETTER 52
#define KEEPNORMALS_IBATCHPROOPTIMIZER_SETTER 53
#define KEEPNORMALS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define NORMALMODE_IBATCHPROOPTIMIZER_GETTER 54
#define NORMALMODE_IBATCHPROOPTIMIZER_SETTER 55
#define NORMALMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define NORMALTHRESHOLD_IBATCHPROOPTIMIZER_GETTER 56
#define NORMALTHRESHOLD_IBATCHPROOPTIMIZER_SETTER 57
#define NORMALTHRESHOLD_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
#define SYMMETRYMODE_IBATCHPROOPTIMIZER_GETTER 58
#define SYMMETRYMODE_IBATCHPROOPTIMIZER_SETTER 59
#define SYMMETRYMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define SYMMETRYTOLERANCE_IBATCHPROOPTIMIZER_GETTER 60
#define SYMMETRYTOLERANCE_IBATCHPROOPTIMIZER_SETTER 61
#define SYMMETRYTOLERANCE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define DESTFILENAMEMODE_IBATCHPROOPTIMIZER_GETTER 62
#define DESTFILENAMEMODE_IBATCHPROOPTIMIZER_SETTER 63
#define DESTFILENAMEMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define AUTORENAME_IBATCHPROOPTIMIZER_GETTER 64
#define AUTORENAME_IBATCHPROOPTIMIZER_SETTER 65
#define AUTORENAME_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define DESTFILENAMEPREFIX_IBATCHPROOPTIMIZER_GETTER 66
#define DESTFILENAMEPREFIX_IBATCHPROOPTIMIZER_SETTER 67
#define DESTFILENAMEPREFIX_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
#define DESTFILENAMESUFFIX_IBATCHPROOPTIMIZER_GETTER 68
#define DESTFILENAMESUFFIX_IBATCHPROOPTIMIZER_SETTER 69
#define DESTFILENAMESUFFIX_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
#define DESTFOLDERMODE_IBATCHPROOPTIMIZER_GETTER 70
#define DESTFOLDERMODE_IBATCHPROOPTIMIZER_SETTER 71
#define DESTFOLDERMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define DESTFOLDERNAME_IBATCHPROOPTIMIZER_GETTER 72
#define DESTFOLDERNAME_IBATCHPROOPTIMIZER_SETTER 73
#define DESTFOLDERNAME_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
#define RECREATEFOLDERTREE_IBATCHPROOPTIMIZER_GETTER 74
#define RECREATEFOLDERTREE_IBATCHPROOPTIMIZER_SETTER 75
#define RECREATEFOLDERTREE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
#define DESTFORMATMODE_IBATCHPROOPTIMIZER_GETTER 76
#define DESTFORMATMODE_IBATCHPROOPTIMIZER_SETTER 77
#define DESTFORMATMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
#define DESTFORMATEXT_IBATCHPROOPTIMIZER_GETTER 78
#define DESTFORMATEXT_IBATCHPROOPTIMIZER_SETTER 79
#define DESTFORMATEXT_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
#define LOGFILEPATH_IBATCHPROOPTIMIZER_GETTER 80
#define LOGFILEPATH_IBATCHPROOPTIMIZER_SETTER 81
#define LOGFILEPATH_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
#define APPLYMODIFIER_IBATCHPROOPTIMIZER_GETTER 83
#define APPLYMODIFIER_IBATCHPROOPTIMIZER_SETTER 82
#define APPLYMODIFIER_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_BOOL
#define VALIDATEDIALOGS_IBATCHPROOPTIMIZER_GETTER 85
#define VALIDATEDIALOGS_IBATCHPROOPTIMIZER_SETTER 84
#define VALIDATEDIALOGS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_BOOL