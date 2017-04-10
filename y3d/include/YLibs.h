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


// --------------------list functions of core interface BATCHPROOPTIMIZER--------------------------
// Number function of BATCHPROOPTIMIZER core interface is 1
#define IBATCHPROOPTIMIZER_NUMFUCNTIONS 1
// function ID of function BATCH of core interface BATCHPROOPTIMIZER is 86
#define BATCH_IBATCHPROOPTIMIZER 86
  // Result Type of function BATCH of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_VOID
  #define BATCH_IBATCHPROOPTIMIZER_RESULTTYPE ParamType2::TYPE_VOID
  // number parameter of fucntion BATCH of core interface BATCHPROOPTIMIZER is 0
  #define BATCH_IBATCHPROOPTIMIZER_NUMPARAMS 0
// ---------------------- List Properties of core interface BATCHPROOPTIMIZER ----------------------
#define IBATCHPROOPTIMIZER_NUMPROPS 43
// function_id of for get property SOURCEFILEMODE of core interface BATCHPROOPTIMIZER is 0
#define SOURCEFILEMODE_IBATCHPROOPTIMIZER_GETTER 0
// function_id of for set property SOURCEFILEMODE of core interface BATCHPROOPTIMIZER is 1
#define SOURCEFILEMODE_IBATCHPROOPTIMIZER_SETTER 1
// parameter type of properties SOURCEFILEMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define SOURCEFILEMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property SOURCEFILEFILES of core interface BATCHPROOPTIMIZER is 2
#define SOURCEFILEFILES_IBATCHPROOPTIMIZER_GETTER 2
// function_id of for set property SOURCEFILEFILES of core interface BATCHPROOPTIMIZER is 3
#define SOURCEFILEFILES_IBATCHPROOPTIMIZER_SETTER 3
// parameter type of properties SOURCEFILEFILES of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_FILENAME_TAB_BV
#define SOURCEFILEFILES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_FILENAME_TAB_BV
// function_id of for get property SOURCEFILEDIR of core interface BATCHPROOPTIMIZER is 4
#define SOURCEFILEDIR_IBATCHPROOPTIMIZER_GETTER 4
// function_id of for set property SOURCEFILEDIR of core interface BATCHPROOPTIMIZER is 5
#define SOURCEFILEDIR_IBATCHPROOPTIMIZER_SETTER 5
// parameter type of properties SOURCEFILEDIR of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_STRING
#define SOURCEFILEDIR_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
// function_id of for get property SOURCEFILESUBDIR of core interface BATCHPROOPTIMIZER is 6
#define SOURCEFILESUBDIR_IBATCHPROOPTIMIZER_GETTER 6
// function_id of for set property SOURCEFILESUBDIR of core interface BATCHPROOPTIMIZER is 7
#define SOURCEFILESUBDIR_IBATCHPROOPTIMIZER_SETTER 7
// parameter type of properties SOURCEFILESUBDIR of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define SOURCEFILESUBDIR_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property OPTIMIZATIONLEVEL1 of core interface BATCHPROOPTIMIZER is 8
#define OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_GETTER 8
// function_id of for set property OPTIMIZATIONLEVEL1 of core interface BATCHPROOPTIMIZER is 9
#define OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_SETTER 9
// parameter type of properties OPTIMIZATIONLEVEL1 of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZATIONLEVEL2 of core interface BATCHPROOPTIMIZER is 10
#define OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_GETTER 10
// function_id of for set property OPTIMIZATIONLEVEL2 of core interface BATCHPROOPTIMIZER is 11
#define OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_SETTER 11
// parameter type of properties OPTIMIZATIONLEVEL2 of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZATIONLEVEL3 of core interface BATCHPROOPTIMIZER is 12
#define OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_GETTER 12
// function_id of for set property OPTIMIZATIONLEVEL3 of core interface BATCHPROOPTIMIZER is 13
#define OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_SETTER 13
// parameter type of properties OPTIMIZATIONLEVEL3 of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZATIONLEVEL4 of core interface BATCHPROOPTIMIZER is 14
#define OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_GETTER 14
// function_id of for set property OPTIMIZATIONLEVEL4 of core interface BATCHPROOPTIMIZER is 15
#define OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_SETTER 15
// parameter type of properties OPTIMIZATIONLEVEL4 of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZATIONLEVEL5 of core interface BATCHPROOPTIMIZER is 16
#define OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_GETTER 16
// function_id of for set property OPTIMIZATIONLEVEL5 of core interface BATCHPROOPTIMIZER is 17
#define OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_SETTER 17
// parameter type of properties OPTIMIZATIONLEVEL5 of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZATIONLEVELSTEPS of core interface BATCHPROOPTIMIZER is 18
#define OPTIMIZATIONLEVELSTEPS_IBATCHPROOPTIMIZER_GETTER 18
// function_id of for set property OPTIMIZATIONLEVELSTEPS of core interface BATCHPROOPTIMIZER is 19
#define OPTIMIZATIONLEVELSTEPS_IBATCHPROOPTIMIZER_SETTER 19
// parameter type of properties OPTIMIZATIONLEVELSTEPS of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZATIONLEVELSTEPS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZATIONLEVELMODE of core interface BATCHPROOPTIMIZER is 20
#define OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_GETTER 20
// function_id of for set property OPTIMIZATIONLEVELMODE of core interface BATCHPROOPTIMIZER is 21
#define OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_SETTER 21
// parameter type of properties OPTIMIZATIONLEVELMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZEBORDERSMODE of core interface BATCHPROOPTIMIZER is 22
#define OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_GETTER 22
// function_id of for set property OPTIMIZEBORDERSMODE of core interface BATCHPROOPTIMIZER is 23
#define OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_SETTER 23
// parameter type of properties OPTIMIZEBORDERSMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property OPTIMIZEHIDDEN of core interface BATCHPROOPTIMIZER is 24
#define OPTIMIZEHIDDEN_IBATCHPROOPTIMIZER_GETTER 24
// function_id of for set property OPTIMIZEHIDDEN of core interface BATCHPROOPTIMIZER is 25
#define OPTIMIZEHIDDEN_IBATCHPROOPTIMIZER_SETTER 25
// parameter type of properties OPTIMIZEHIDDEN of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define OPTIMIZEHIDDEN_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property MERGEPOINTS of core interface BATCHPROOPTIMIZER is 26
#define MERGEPOINTS_IBATCHPROOPTIMIZER_GETTER 26
// function_id of for set property MERGEPOINTS of core interface BATCHPROOPTIMIZER is 27
#define MERGEPOINTS_IBATCHPROOPTIMIZER_SETTER 27
// parameter type of properties MERGEPOINTS of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define MERGEPOINTS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property MERGEPOINTSTHRESHOLD of core interface BATCHPROOPTIMIZER is 28
#define MERGEPOINTSTHRESHOLD_IBATCHPROOPTIMIZER_GETTER 28
// function_id of for set property MERGEPOINTSTHRESHOLD of core interface BATCHPROOPTIMIZER is 29
#define MERGEPOINTSTHRESHOLD_IBATCHPROOPTIMIZER_SETTER 29
// parameter type of properties MERGEPOINTSTHRESHOLD of core interface BATCHPROOPTIMIZER is ParamType::TYPE_FLOAT
#define MERGEPOINTSTHRESHOLD_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
// function_id of for get property MERGEFACES of core interface BATCHPROOPTIMIZER is 30
#define MERGEFACES_IBATCHPROOPTIMIZER_GETTER 30
// function_id of for set property MERGEFACES of core interface BATCHPROOPTIMIZER is 31
#define MERGEFACES_IBATCHPROOPTIMIZER_SETTER 31
// parameter type of properties MERGEFACES of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define MERGEFACES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property MERGEFACESTHRESHOLD of core interface BATCHPROOPTIMIZER is 32
#define MERGEFACESTHRESHOLD_IBATCHPROOPTIMIZER_GETTER 32
// function_id of for set property MERGEFACESTHRESHOLD of core interface BATCHPROOPTIMIZER is 33
#define MERGEFACESTHRESHOLD_IBATCHPROOPTIMIZER_SETTER 33
// parameter type of properties MERGEFACESTHRESHOLD of core interface BATCHPROOPTIMIZER is ParamType::TYPE_FLOAT
#define MERGEFACESTHRESHOLD_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
// function_id of for get property KEEPMATERIALBOUNDARIES of core interface BATCHPROOPTIMIZER is 34
#define KEEPMATERIALBOUNDARIES_IBATCHPROOPTIMIZER_GETTER 34
// function_id of for set property KEEPMATERIALBOUNDARIES of core interface BATCHPROOPTIMIZER is 35
#define KEEPMATERIALBOUNDARIES_IBATCHPROOPTIMIZER_SETTER 35
#define KEEPMATERIALBOUNDARIES_IBATC// parameter type of properties KEEPMATERIALBOUNDARIES of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define KEEPMATERIALBOUNDARIES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property COMPACTFACES of core interface BATCHPROOPTIMIZER is 36
#define COMPACTFACES_IBATCHPROOPTIMIZER_GETTER 36
// function_id of for set property COMPACTFACES of core interface BATCHPROOPTIMIZER is 37
#define COMPACTFACES_IBATCHPROOPTIMIZER_SETTER 37
// parameter type of properties COMPACTFACES of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define COMPACTFACES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property PREVENTFLIPPEDNORMALS of core interface BATCHPROOPTIMIZER is 38
#define PREVENTFLIPPEDNORMALS_IBATCHPROOPTIMIZER_GETTER 38
// function_id of for set property PREVENTFLIPPEDNORMALS of core interface BATCHPROOPTIMIZER is 39
#define PREVENTFLIPPEDNORMALS_IBATCHPROOPTIMIZER_SETTER 39
// parameter type of properties PREVENTFLIPPEDNORMALS of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define PREVENTFLIPPEDNORMALS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property KEEPVERTEXCOLORS of core interface BATCHPROOPTIMIZER is 40
#define KEEPVERTEXCOLORS_IBATCHPROOPTIMIZER_GETTER 40
// function_id of for set property KEEPVERTEXCOLORS of core interface BATCHPROOPTIMIZER is 41
#define KEEPVERTEXCOLORS_IBATCHPROOPTIMIZER_SETTER 41
// parameter type of properties KEEPVERTEXCOLORS of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define KEEPVERTEXCOLORS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property KEEPVERTEXCOLORSBOUNDARIES of core interface BATCHPROOPTIMIZER is 42
#define KEEPVERTEXCOLORSBOUNDARIES_IBATCHPROOPTIMIZER_GETTER 42
// function_id of for set property KEEPVERTEXCOLORSBOUNDARIES of core interface BATCHPROOPTIMIZER is 43
#define KEEPVERTEXCOLORSBOUNDARIES_IBATCHPROOPTIMIZER_SETTER 43
// parameter type of properties KEEPVERTEXCOLORSBOUNDARIES of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define KEEPVERTEXCOLORSBOUNDARIES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property VERTEXCOLORSTOLERANCE of core interface BATCHPROOPTIMIZER is 44
#define VERTEXCOLORSTOLERANCE_IBATCHPROOPTIMIZER_GETTER 44
// function_id of for set property VERTEXCOLORSTOLERANCE of core interface BATCHPROOPTIMIZER is 45
#define VERTEXCOLORSTOLERANCE_IBATCHPROOPTIMIZER_SETTER 45
// parameter type of properties VERTEXCOLORSTOLERANCE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define VERTEXCOLORSTOLERANCE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property KEEPTEXTURE of core interface BATCHPROOPTIMIZER is 46
#define KEEPTEXTURE_IBATCHPROOPTIMIZER_GETTER 46
// function_id of for set property KEEPTEXTURE of core interface BATCHPROOPTIMIZER is 47
#define KEEPTEXTURE_IBATCHPROOPTIMIZER_SETTER 47
// parameter type of properties KEEPTEXTURE of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define KEEPTEXTURE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property KEEPUVBOUNDARIES of core interface BATCHPROOPTIMIZER is 48
#define KEEPUVBOUNDARIES_IBATCHPROOPTIMIZER_GETTER 48
// function_id of for set property KEEPUVBOUNDARIES of core interface BATCHPROOPTIMIZER is 49
#define KEEPUVBOUNDARIES_IBATCHPROOPTIMIZER_SETTER 49
// parameter type of properties KEEPUVBOUNDARIES of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define KEEPUVBOUNDARIES_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property UVTOLERANCE of core interface BATCHPROOPTIMIZER is 50
#define UVTOLERANCE_IBATCHPROOPTIMIZER_GETTER 50
// function_id of for set property UVTOLERANCE of core interface BATCHPROOPTIMIZER is 51
#define UVTOLERANCE_IBATCHPROOPTIMIZER_SETTER 51
// parameter type of properties UVTOLERANCE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_FLOAT
#define UVTOLERANCE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
// function_id of for get property KEEPNORMALS of core interface BATCHPROOPTIMIZER is 52
#define KEEPNORMALS_IBATCHPROOPTIMIZER_GETTER 52
// function_id of for set property KEEPNORMALS of core interface BATCHPROOPTIMIZER is 53
#define KEEPNORMALS_IBATCHPROOPTIMIZER_SETTER 53
// parameter type of properties KEEPNORMALS of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define KEEPNORMALS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property NORMALMODE of core interface BATCHPROOPTIMIZER is 54
#define NORMALMODE_IBATCHPROOPTIMIZER_GETTER 54
// function_id of for set property NORMALMODE of core interface BATCHPROOPTIMIZER is 55
#define NORMALMODE_IBATCHPROOPTIMIZER_SETTER 55
// parameter type of properties NORMALMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define NORMALMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property NORMALTHRESHOLD of core interface BATCHPROOPTIMIZER is 56
#define NORMALTHRESHOLD_IBATCHPROOPTIMIZER_GETTER 56
// function_id of for set property NORMALTHRESHOLD of core interface BATCHPROOPTIMIZER is 57
#define NORMALTHRESHOLD_IBATCHPROOPTIMIZER_SETTER 57
// parameter type of properties NORMALTHRESHOLD of core interface BATCHPROOPTIMIZER is ParamType::TYPE_FLOAT
#define NORMALTHRESHOLD_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_FLOAT
// function_id of for get property SYMMETRYMODE of core interface BATCHPROOPTIMIZER is 58
#define SYMMETRYMODE_IBATCHPROOPTIMIZER_GETTER 58
// function_id of for set property SYMMETRYMODE of core interface BATCHPROOPTIMIZER is 59
#define SYMMETRYMODE_IBATCHPROOPTIMIZER_SETTER 59
// parameter type of properties SYMMETRYMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define SYMMETRYMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property SYMMETRYTOLERANCE of core interface BATCHPROOPTIMIZER is 60
#define SYMMETRYTOLERANCE_IBATCHPROOPTIMIZER_GETTER 60
// function_id of for set property SYMMETRYTOLERANCE of core interface BATCHPROOPTIMIZER is 61
#define SYMMETRYTOLERANCE_IBATCHPROOPTIMIZER_SETTER 61
// parameter type of properties SYMMETRYTOLERANCE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define SYMMETRYTOLERANCE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property DESTFILENAMEMODE of core interface BATCHPROOPTIMIZER is 62
#define DESTFILENAMEMODE_IBATCHPROOPTIMIZER_GETTER 62
// function_id of for set property DESTFILENAMEMODE of core interface BATCHPROOPTIMIZER is 63
#define DESTFILENAMEMODE_IBATCHPROOPTIMIZER_SETTER 63
// parameter type of properties DESTFILENAMEMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define DESTFILENAMEMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property AUTORENAME of core interface BATCHPROOPTIMIZER is 64
#define AUTORENAME_IBATCHPROOPTIMIZER_GETTER 64
// function_id of for set property AUTORENAME of core interface BATCHPROOPTIMIZER is 65
#define AUTORENAME_IBATCHPROOPTIMIZER_SETTER 65
// parameter type of properties AUTORENAME of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define AUTORENAME_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property DESTFILENAMEPREFIX of core interface BATCHPROOPTIMIZER is 66
#define DESTFILENAMEPREFIX_IBATCHPROOPTIMIZER_GETTER 66
// function_id of for set property DESTFILENAMEPREFIX of core interface BATCHPROOPTIMIZER is 67
#define DESTFILENAMEPREFIX_IBATCHPROOPTIMIZER_SETTER 67
// parameter type of properties DESTFILENAMEPREFIX of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_STRING
#define DESTFILENAMEPREFIX_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
// function_id of for get property DESTFILENAMESUFFIX of core interface BATCHPROOPTIMIZER is 68
#define DESTFILENAMESUFFIX_IBATCHPROOPTIMIZER_GETTER 68
// function_id of for set property DESTFILENAMESUFFIX of core interface BATCHPROOPTIMIZER is 69
#define DESTFILENAMESUFFIX_IBATCHPROOPTIMIZER_SETTER 69
// parameter type of properties DESTFILENAMESUFFIX of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_STRING
#define DESTFILENAMESUFFIX_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
// function_id of for get property DESTFOLDERMODE of core interface BATCHPROOPTIMIZER is 70
#define DESTFOLDERMODE_IBATCHPROOPTIMIZER_GETTER 70
// function_id of for set property DESTFOLDERMODE of core interface BATCHPROOPTIMIZER is 71
#define DESTFOLDERMODE_IBATCHPROOPTIMIZER_SETTER 71
// parameter type of properties DESTFOLDERMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define DESTFOLDERMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property DESTFOLDERNAME of core interface BATCHPROOPTIMIZER is 72
#define DESTFOLDERNAME_IBATCHPROOPTIMIZER_GETTER 72
// function_id of for set property DESTFOLDERNAME of core interface BATCHPROOPTIMIZER is 73
#define DESTFOLDERNAME_IBATCHPROOPTIMIZER_SETTER 73
// parameter type of properties DESTFOLDERNAME of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_STRING
#define DESTFOLDERNAME_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
// function_id of for get property RECREATEFOLDERTREE of core interface BATCHPROOPTIMIZER is 74
#define RECREATEFOLDERTREE_IBATCHPROOPTIMIZER_GETTER 74
// function_id of for set property RECREATEFOLDERTREE of core interface BATCHPROOPTIMIZER is 75
#define RECREATEFOLDERTREE_IBATCHPROOPTIMIZER_SETTER 75
// parameter type of properties RECREATEFOLDERTREE of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_bool
#define RECREATEFOLDERTREE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property DESTFORMATMODE of core interface BATCHPROOPTIMIZER is 76
#define DESTFORMATMODE_IBATCHPROOPTIMIZER_GETTER 76
// function_id of for set property DESTFORMATMODE of core interface BATCHPROOPTIMIZER is 77
#define DESTFORMATMODE_IBATCHPROOPTIMIZER_SETTER 77
// parameter type of properties DESTFORMATMODE of core interface BATCHPROOPTIMIZER is ParamType::TYPE_INT
#define DESTFORMATMODE_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property DESTFORMATEXT of core interface BATCHPROOPTIMIZER is 78
#define DESTFORMATEXT_IBATCHPROOPTIMIZER_GETTER 78
// function_id of for set property DESTFORMATEXT of core interface BATCHPROOPTIMIZER is 79
#define DESTFORMATEXT_IBATCHPROOPTIMIZER_SETTER 79
// parameter type of properties DESTFORMATEXT of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_STRING
#define DESTFORMATEXT_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
// function_id of for get property LOGFILEPATH of core interface BATCHPROOPTIMIZER is 80
#define LOGFILEPATH_IBATCHPROOPTIMIZER_GETTER 80
// function_id of for set property LOGFILEPATH of core interface BATCHPROOPTIMIZER is 81
#define LOGFILEPATH_IBATCHPROOPTIMIZER_SETTER 81
// parameter type of properties LOGFILEPATH of core interface BATCHPROOPTIMIZER is ParamType2::TYPE_STRING
#define LOGFILEPATH_IBATCHPROOPTIMIZER_TYPEPARAM ParamType2::TYPE_STRING
// function_id of for get property APPLYMODIFIER of core interface BATCHPROOPTIMIZER is 83
#define APPLYMODIFIER_IBATCHPROOPTIMIZER_GETTER 83
// function_id of for set property APPLYMODIFIER of core interface BATCHPROOPTIMIZER is 82
#define APPLYMODIFIER_IBATCHPROOPTIMIZER_SETTER 82
// parameter type of properties APPLYMODIFIER of core interface BATCHPROOPTIMIZER is ParamType::TYPE_BOOL
#define APPLYMODIFIER_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_BOOL
// function_id of for get property VALIDATEDIALOGS of core interface BATCHPROOPTIMIZER is 85
#define VALIDATEDIALOGS_IBATCHPROOPTIMIZER_GETTER 85
// function_id of for set property VALIDATEDIALOGS of core interface BATCHPROOPTIMIZER is 84
#define VALIDATEDIALOGS_IBATCHPROOPTIMIZER_SETTER 84
// parameter type of properties VALIDATEDIALOGS of core interface BATCHPROOPTIMIZER is ParamType::TYPE_BOOL
#define VALIDATEDIALOGS_IBATCHPROOPTIMIZER_TYPEPARAM ParamType::TYPE_BOOL


// --------------------list functions of core interface LAYERMANAGER--------------------------
// Number function of LAYERMANAGER core interface is 14
#define ILAYERMANAGER_NUMFUCNTIONS 14
// function ID of function GETLAYER of core interface LAYERMANAGER is 0
#define GETLAYER_ILAYERMANAGER 0
  // Result Type of function GETLAYER of core interface LAYERMANAGER is ParamType2::TYPE_INTERFACE
  #define GETLAYER_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_INTERFACE
  // number parameter of fucntion GETLAYER of core interface LAYERMANAGER is 1
  #define GETLAYER_ILAYERMANAGER_NUMPARAMS 1
  // WHICH, which is parameter number 1 of function
// GETLAYER of core interface LAYERMANAGER have param type is ParamType::TYPE_INT
  #define WHICH_GETLAYER_ILAYERMANAGER_PARAM1_TYPE ParamType::TYPE_INT
// function ID of function NEWLAYER of core interface LAYERMANAGER is 1
#define NEWLAYER_ILAYERMANAGER 1
  // Result Type of function NEWLAYER of core interface LAYERMANAGER is ParamType2::TYPE_INTERFACE
  #define NEWLAYER_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_INTERFACE
  // number parameter of fucntion NEWLAYER of core interface LAYERMANAGER is 0
  #define NEWLAYER_ILAYERMANAGER_NUMPARAMS 0
// function ID of function GETLAYERFROMNAME of core interface LAYERMANAGER is 2
#define GETLAYERFROMNAME_ILAYERMANAGER 2
  // Result Type of function GETLAYERFROMNAME of core interface LAYERMANAGER is ParamType2::TYPE_INTERFACE
  #define GETLAYERFROMNAME_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_INTERFACE
  // number parameter of fucntion GETLAYERFROMNAME of core interface LAYERMANAGER is 1
  #define GETLAYERFROMNAME_ILAYERMANAGER_NUMPARAMS 1
  // NAME, which is parameter number 1 of function
// GETLAYERFROMNAME of core interface LAYERMANAGER have param type is ParamType2::TYPE_STRING
  #define NAME_GETLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_STRING
// function ID of function NEWLAYERFROMNAME of core interface LAYERMANAGER is 3
#define NEWLAYERFROMNAME_ILAYERMANAGER 3
  // Result Type of function NEWLAYERFROMNAME of core interface LAYERMANAGER is ParamType2::TYPE_INTERFACE
  #define NEWLAYERFROMNAME_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_INTERFACE
  // number parameter of fucntion NEWLAYERFROMNAME of core interface LAYERMANAGER is 1
  #define NEWLAYERFROMNAME_ILAYERMANAGER_NUMPARAMS 1
  // NAME, which is parameter number 1 of function
// NEWLAYERFROMNAME of core interface LAYERMANAGER have param type is ParamType2::TYPE_STRING
  #define NAME_NEWLAYERFROMNAME_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_STRING
// function ID of function EDITLAYERBYNAME of core interface LAYERMANAGER is 8
#define EDITLAYERBYNAME_ILAYERMANAGER 8
  // Result Type of function EDITLAYERBYNAME of core interface LAYERMANAGER is ParamType2::TYPE_VOID
  #define EDITLAYERBYNAME_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_VOID
  // number parameter of fucntion EDITLAYERBYNAME of core interface LAYERMANAGER is 1
  #define EDITLAYERBYNAME_ILAYERMANAGER_NUMPARAMS 1
  // NAME, which is parameter number 1 of function
// EDITLAYERBYNAME of core interface LAYERMANAGER have param type is ParamType2::TYPE_STRING
  #define NAME_EDITLAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_STRING
// function ID of function DELETELAYERBYNAME of core interface LAYERMANAGER is 9
#define DELETELAYERBYNAME_ILAYERMANAGER 9
  // Result Type of function DELETELAYERBYNAME of core interface LAYERMANAGER is ParamType2::TYPE_bool
  #define DELETELAYERBYNAME_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_bool
  // number parameter of fucntion DELETELAYERBYNAME of core interface LAYERMANAGER is 1
  #define DELETELAYERBYNAME_ILAYERMANAGER_NUMPARAMS 1
  // NAME, which is parameter number 1 of function
// DELETELAYERBYNAME of core interface LAYERMANAGER have param type is ParamType2::TYPE_STRING
  #define NAME_DELETELAYERBYNAME_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_STRING
// function ID of function CLOSEDIALOG of core interface LAYERMANAGER is 10
#define CLOSEDIALOG_ILAYERMANAGER 10
  // Result Type of function CLOSEDIALOG of core interface LAYERMANAGER is ParamType2::TYPE_VOID
  #define CLOSEDIALOG_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_VOID
  // number parameter of fucntion CLOSEDIALOG of core interface LAYERMANAGER is 0
  #define CLOSEDIALOG_ILAYERMANAGER_NUMPARAMS 0
// function ID of function ISDIALOGOPEN of core interface LAYERMANAGER is 11
#define ISDIALOGOPEN_ILAYERMANAGER 11
  // Result Type of function ISDIALOGOPEN of core interface LAYERMANAGER is ParamType2::TYPE_bool
  #define ISDIALOGOPEN_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_bool
  // number parameter of fucntion ISDIALOGOPEN of core interface LAYERMANAGER is 0
  #define ISDIALOGOPEN_ILAYERMANAGER_NUMPARAMS 0
// function ID of function LAYERPROPDIALOG of core interface LAYERMANAGER is 12
#define LAYERPROPDIALOG_ILAYERMANAGER 12
  // Result Type of function LAYERPROPDIALOG of core interface LAYERMANAGER is ParamType2::TYPE_VOID
  #define LAYERPROPDIALOG_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_VOID
  // number parameter of fucntion LAYERPROPDIALOG of core interface LAYERMANAGER is 1
  #define LAYERPROPDIALOG_ILAYERMANAGER_NUMPARAMS 1
  // LAYERLIST, which is parameter number 1 of function
// LAYERPROPDIALOG of core interface LAYERMANAGER have param type is ParamType2::TYPE_REFTARG_TAB_BR
  #define LAYERLIST_LAYERPROPDIALOG_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_REFTARG_TAB_BR
// function ID of function GETNEXTDEFAULTLAYERNAME of core interface LAYERMANAGER is 13
#define GETNEXTDEFAULTLAYERNAME_ILAYERMANAGER 13
  // Result Type of function GETNEXTDEFAULTLAYERNAME of core interface LAYERMANAGER is ParamType2::TYPE_TSTR_BV
  #define GETNEXTDEFAULTLAYERNAME_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_TSTR_BV
  // number parameter of fucntion GETNEXTDEFAULTLAYERNAME of core interface LAYERMANAGER is 0
  #define GETNEXTDEFAULTLAYERNAME_ILAYERMANAGER_NUMPARAMS 0
// function ID of function DOESLAYERHIERARCHYCONTAINNODES of core interface LAYERMANAGER is 18
#define DOESLAYERHIERARCHYCONTAINNODES_ILAYERMANAGER 18
  // Result Type of function DOESLAYERHIERARCHYCONTAINNODES of core interface LAYERMANAGER is ParamType2::TYPE_bool
  #define DOESLAYERHIERARCHYCONTAINNODES_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_bool
  // number parameter of fucntion DOESLAYERHIERARCHYCONTAINNODES of core interface LAYERMANAGER is 1
  #define DOESLAYERHIERARCHYCONTAINNODES_ILAYERMANAGER_NUMPARAMS 1
  // NAME, which is parameter number 1 of function
// DOESLAYERHIERARCHYCONTAINNODES of core interface LAYERMANAGER have param type is ParamType2::TYPE_STRING
  #define NAME_DOESLAYERHIERARCHYCONTAINNODES_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_STRING
// function ID of function DELETELAYERHIERARCHY of core interface LAYERMANAGER is 19
#define DELETELAYERHIERARCHY_ILAYERMANAGER 19
  // Result Type of function DELETELAYERHIERARCHY of core interface LAYERMANAGER is ParamType::TYPE_INT
  #define DELETELAYERHIERARCHY_ILAYERMANAGER_RESULTTYPE ParamType::TYPE_INT
  // number parameter of fucntion DELETELAYERHIERARCHY of core interface LAYERMANAGER is 2
  #define DELETELAYERHIERARCHY_ILAYERMANAGER_NUMPARAMS 2
  // NAME, which is parameter number 1 of function
// DELETELAYERHIERARCHY of core interface LAYERMANAGER have param type is ParamType2::TYPE_STRING
  #define NAME_DELETELAYERHIERARCHY_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_STRING
  // FORCEDELETE, which is parameter number 2 of function
// DELETELAYERHIERARCHY of core interface LAYERMANAGER have param type is ParamType2::TYPE_bool
  #define FORCEDELETE_DELETELAYERHIERARCHY_ILAYERMANAGER_PARAM2_TYPE ParamType2::TYPE_bool
// function ID of function GETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER is 6
#define GETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER 6
  // Result Type of function GETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER is ParamType2::TYPE_bool
  #define GETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_bool
  // number parameter of fucntion GETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER is 0
  #define GETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER_NUMPARAMS 0
// function ID of function SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER is 7
#define SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER 7
  // Result Type of function SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER is ParamType2::TYPE_VOID
  #define SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER_RESULTTYPE ParamType2::TYPE_VOID
  // number parameter of fucntion SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER is 2
  #define SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER_NUMPARAMS 2
  // DUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE, which is parameter number 1 of function
// SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER have param type is ParamType2::TYPE_bool
  #define DUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER_PARAM1_TYPE ParamType2::TYPE_bool
  // PERSIST, which is parameter number 2 of function
// SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE of core interface LAYERMANAGER have param type is ParamType2::TYPE_bool
  #define PERSIST_SETDUPLICATELAYERWITHOUTSAMEHIERARCHYONMERGE_ILAYERMANAGER_PARAM2_TYPE ParamType2::TYPE_bool
// ---------------------- List Properties of core interface LAYERMANAGER ----------------------
#define ILAYERMANAGER_NUMPROPS 2
// function_id of for get property COUNT of core interface LAYERMANAGER is 4
#define COUNT_ILAYERMANAGER_GETTER 4
// function_id of for set property COUNT of core interface LAYERMANAGER is -1
#define COUNT_ILAYERMANAGER_SETTER -1
// parameter type of properties COUNT of core interface LAYERMANAGER is ParamType::TYPE_INT
#define COUNT_ILAYERMANAGER_TYPEPARAM ParamType::TYPE_INT
// function_id of for get property CURRENT of core interface LAYERMANAGER is 5
#define CURRENT_ILAYERMANAGER_GETTER 5
// function_id of for set property CURRENT of core interface LAYERMANAGER is -1
#define CURRENT_ILAYERMANAGER_SETTER -1
// parameter type of properties CURRENT of core interface LAYERMANAGER is ParamType2::TYPE_INTERFACE
#define CURRENT_ILAYERMANAGER_TYPEPARAM ParamType2::TYPE_INTERFACE

// --------------------list functions of core interface LAYERPROPERTIES--------------------------
// Number function of LAYERPROPERTIES core interface is 11
#define ILAYERPROPERTIES_NUMFUCNTIONS 11
// function ID of function ADDNODE of core interface LAYERPROPERTIES is 0
#define ADDNODE_ILAYERPROPERTIES 0
  // Result Type of function ADDNODE of core interface LAYERPROPERTIES is ParamType2::TYPE_VOID
  #define ADDNODE_ILAYERPROPERTIES_RESULTTYPE ParamType2::TYPE_VOID
  // number parameter of fucntion ADDNODE of core interface LAYERPROPERTIES is 1
  #define ADDNODE_ILAYERPROPERTIES_NUMPARAMS 1
  // NODE, which is parameter number 1 of function
// ADDNODE of core interface LAYERPROPERTIES have param type is ParamType2::TYPE_INODE
  #define NODE_ADDNODE_ILAYERPROPERTIES_PARAM1_TYPE ParamType2::TYPE_INODE
// function ID of function SELECT of core interface LAYERPROPERTIES is 1
#define SELECT_ILAYERPROPERTIES 1
  // Result Type of function SELECT of core interface LAYERPROPERTIES is ParamType2::TYPE_VOID
  #define SELECT_ILAYERPROPERTIES_RESULTTYPE ParamType2::TYPE_VOID
  // number parameter of fucntion SELECT of core interface LAYERPROPERTIES is 1
  #define SELECT_ILAYERPROPERTIES_NUMPARAMS 1
  // ONOFF, which is parameter number 1 of function
// SELECT of core interface LAYERPROPERTIES have param type is ParamType::TYPE_BOOL
  #define ONOFF_SELECT_ILAYERPROPERTIES_PARAM1_TYPE ParamType::TYPE_BOOL
// function ID of function SETNAME of core interface LAYERPROPERTIES is 13
#define SETNAME_ILAYERPROPERTIES 13
  // Result Type of function SETNAME of core interface LAYERPROPERTIES is ParamType::TYPE_BOOL
  #define SETNAME_ILAYERPROPERTIES_RESULTTYPE ParamType::TYPE_BOOL
  // number parameter of fucntion SETNAME of core interface LAYERPROPERTIES is 1
  #define SETNAME_ILAYERPROPERTIES_NUMPARAMS 1
  // NAME, which is parameter number 1 of function
// SETNAME of core interface LAYERPROPERTIES have param type is ParamType2::TYPE_STRING
  #define NAME_SETNAME_ILAYERPROPERTIES_PARAM1_TYPE ParamType2::TYPE_STRING
// function ID of function NODES of core interface LAYERPROPERTIES is 65
#define NODES_ILAYERPROPERTIES 65
  // Result Type of function NODES of core interface LAYERPROPERTIES is ParamType::TYPE_BOOL
  #define NODES_ILAYERPROPERTIES_RESULTTYPE ParamType::TYPE_BOOL
  // number parameter of fucntion NODES of core interface LAYERPROPERTIES is 1
  #define NODES_ILAYERPROPERTIES_NUMPARAMS 1
  // LAYERNODES, which is parameter number 1 of function
// NODES of core interface LAYERPROPERTIES have param type is ParamType2::TYPE_INODE_TAB_BR
  #define LAYERNODES_NODES_ILAYERPROPERTIES_PARAM1_TYPE ParamType2::TYPE_INODE_TAB_BR
// function ID of function GETPARENT of core interface LAYERPROPERTIES is 67
#define GETPARENT_ILAYERPROPERTIES 67
  // Result Type of function GETPARENT of core interface LAYERPROPERTIES is ParamType2::TYPE_INTERFACE
  #define GETPARENT_ILAYERPROPERTIES_RESULTTYPE ParamType2::TYPE_INTERFACE
  // number parameter of fucntion GETPARENT of core interface LAYERPROPERTIES is 0
  #define GETPARENT_ILAYERPROPERTIES_NUMPARAMS 0
// function ID of function SETPARENT of core interface LAYERPROPERTIES is 68
#define SETPARENT_ILAYERPROPERTIES 68
  // Result Type of function SETPARENT of core interface LAYERPROPERTIES is ParamType::TYPE_BOOL
  #define SETPARENT_ILAYERPROPERTIES_RESULTTYPE ParamType::TYPE_BOOL
  // number parameter of fucntion SETPARENT of core interface LAYERPROPERTIES is 1
  #define SETPARENT_ILAYERPROPERTIES_NUMPARAMS 1
  // PARENT, which is parameter number 1 of function
// SETPARENT of core interface LAYERPROPERTIES have param type is ParamType2::TYPE_INTERFACE
  #define PARENT_SETPARENT_ILAYERPROPERTIES_PARAM1_TYPE ParamType2::TYPE_INTERFACE
// function ID of function GETCHILD of core interface LAYERPROPERTIES is 69
#define GETCHILD_ILAYERPROPERTIES 69
  // Result Type of function GETCHILD of core interface LAYERPROPERTIES is ParamType2::TYPE_INTERFACE
  #define GETCHILD_ILAYERPROPERTIES_RESULTTYPE ParamType2::TYPE_INTERFACE
  // number parameter of fucntion GETCHILD of core interface LAYERPROPERTIES is 1
  #define GETCHILD_ILAYERPROPERTIES_NUMPARAMS 1
  // INDEX, which is parameter number 1 of function
// GETCHILD of core interface LAYERPROPERTIES have param type is ParamType2::TYPE_INDEX
  #define INDEX_GETCHILD_ILAYERPROPERTIES_PARAM1_TYPE ParamType2::TYPE_INDEX
// function ID of function GETNUMCHILDREN of core interface LAYERPROPERTIES is 70
#define GETNUMCHILDREN_ILAYERPROPERTIES 70
  // Result Type of function GETNUMCHILDREN of core interface LAYERPROPERTIES is ParamType::TYPE_INT
  #define GETNUMCHILDREN_ILAYERPROPERTIES_RESULTTYPE ParamType::TYPE_INT
  // number parameter of fucntion GETNUMCHILDREN of core interface LAYERPROPERTIES is 0
  #define GETNUMCHILDREN_ILAYERPROPERTIES_NUMPARAMS 0
// function ID of function CANDELETE of core interface LAYERPROPERTIES is 71
#define CANDELETE_ILAYERPROPERTIES 71
  // Result Type of function CANDELETE of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
  #define CANDELETE_ILAYERPROPERTIES_RESULTTYPE ParamType2::TYPE_bool
  // number parameter of fucntion CANDELETE of core interface LAYERPROPERTIES is 0
  #define CANDELETE_ILAYERPROPERTIES_NUMPARAMS 0
// function ID of function GETNUMNODES of core interface LAYERPROPERTIES is 72
#define GETNUMNODES_ILAYERPROPERTIES 72
  // Result Type of function GETNUMNODES of core interface LAYERPROPERTIES is ParamType::TYPE_INT
  #define GETNUMNODES_ILAYERPROPERTIES_RESULTTYPE ParamType::TYPE_INT
  // number parameter of fucntion GETNUMNODES of core interface LAYERPROPERTIES is 0
  #define GETNUMNODES_ILAYERPROPERTIES_NUMPARAMS 0
// function ID of function HASSCENEXREFNODESINHIERARCHY of core interface LAYERPROPERTIES is 73
#define HASSCENEXREFNODESINHIERARCHY_ILAYERPROPERTIES 73
  // Result Type of function HASSCENEXREFNODESINHIERARCHY of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
  #define HASSCENEXREFNODESINHIERARCHY_ILAYERPROPERTIES_RESULTTYPE ParamType2::TYPE_bool
  // number parameter of fucntion HASSCENEXREFNODESINHIERARCHY of core interface LAYERPROPERTIES is 0
  #define HASSCENEXREFNODESINHIERARCHY_ILAYERPROPERTIES_NUMPARAMS 0
// ---------------------- List Properties of core interface LAYERPROPERTIES ----------------------
#define ILAYERPROPERTIES_NUMPROPS 33
// function_id of for get property ON of core interface LAYERPROPERTIES is 2
#define ON_ILAYERPROPERTIES_GETTER 2
// function_id of for set property ON of core interface LAYERPROPERTIES is 3
#define ON_ILAYERPROPERTIES_SETTER 3
// parameter type of properties ON of core interface LAYERPROPERTIES is ParamType::TYPE_BOOL
#define ON_ILAYERPROPERTIES_TYPEPARAM ParamType::TYPE_BOOL
// function_id of for get property LOCK of core interface LAYERPROPERTIES is 4
#define LOCK_ILAYERPROPERTIES_GETTER 4
// function_id of for set property LOCK of core interface LAYERPROPERTIES is 5
#define LOCK_ILAYERPROPERTIES_SETTER 5
// parameter type of properties LOCK of core interface LAYERPROPERTIES is ParamType::TYPE_BOOL
#define LOCK_ILAYERPROPERTIES_TYPEPARAM ParamType::TYPE_BOOL
// function_id of for get property CURRENT of core interface LAYERPROPERTIES is 6
#define CURRENT_ILAYERPROPERTIES_GETTER 6
// function_id of for set property CURRENT of core interface LAYERPROPERTIES is 7
#define CURRENT_ILAYERPROPERTIES_SETTER 7
// parameter type of properties CURRENT of core interface LAYERPROPERTIES is ParamType::TYPE_BOOL
#define CURRENT_ILAYERPROPERTIES_TYPEPARAM ParamType::TYPE_BOOL
// function_id of for get property WIRECOLOR of core interface LAYERPROPERTIES is 8
#define WIRECOLOR_ILAYERPROPERTIES_GETTER 8
// function_id of for set property WIRECOLOR of core interface LAYERPROPERTIES is 9
#define WIRECOLOR_ILAYERPROPERTIES_SETTER 9
// parameter type of properties WIRECOLOR of core interface LAYERPROPERTIES is ParamType2::TYPE_COLOR_BV
#define WIRECOLOR_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_COLOR_BV
// function_id of for get property ISGIEXCLUDED of core interface LAYERPROPERTIES is 10
#define ISGIEXCLUDED_ILAYERPROPERTIES_GETTER 10
// function_id of for set property ISGIEXCLUDED of core interface LAYERPROPERTIES is 11
#define ISGIEXCLUDED_ILAYERPROPERTIES_SETTER 11
// parameter type of properties ISGIEXCLUDED of core interface LAYERPROPERTIES is ParamType::TYPE_BOOL
#define ISGIEXCLUDED_ILAYERPROPERTIES_TYPEPARAM ParamType::TYPE_BOOL
// function_id of for get property NAME of core interface LAYERPROPERTIES is 12
#define NAME_ILAYERPROPERTIES_GETTER 12
// function_id of for set property NAME of core interface LAYERPROPERTIES is -1
#define NAME_ILAYERPROPERTIES_SETTER -1
// parameter type of properties NAME of core interface LAYERPROPERTIES is ParamType2::TYPE_STRING
#define NAME_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_STRING
// function_id of for get property RENDERABLE of core interface LAYERPROPERTIES is 14
#define RENDERABLE_ILAYERPROPERTIES_GETTER 14
// function_id of for set property RENDERABLE of core interface LAYERPROPERTIES is 15
#define RENDERABLE_ILAYERPROPERTIES_SETTER 15
// parameter type of properties RENDERABLE of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define RENDERABLE_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property INHERITVISIBILITY of core interface LAYERPROPERTIES is 16
#define INHERITVISIBILITY_ILAYERPROPERTIES_GETTER 16
// function_id of for set property INHERITVISIBILITY of core interface LAYERPROPERTIES is 17
#define INHERITVISIBILITY_ILAYERPROPERTIES_SETTER 17
// parameter type of properties INHERITVISIBILITY of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define INHERITVISIBILITY_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property PRIMARYVISIBILITY of core interface LAYERPROPERTIES is 18
#define PRIMARYVISIBILITY_ILAYERPROPERTIES_GETTER 18
// function_id of for set property PRIMARYVISIBILITY of core interface LAYERPROPERTIES is 19
#define PRIMARYVISIBILITY_ILAYERPROPERTIES_SETTER 19
// parameter type of properties PRIMARYVISIBILITY of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define PRIMARYVISIBILITY_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property SECONDARYVISIBILITY of core interface LAYERPROPERTIES is 20
#define SECONDARYVISIBILITY_ILAYERPROPERTIES_GETTER 20
// function_id of for set property SECONDARYVISIBILITY of core interface LAYERPROPERTIES is 21
#define SECONDARYVISIBILITY_ILAYERPROPERTIES_SETTER 21
// parameter type of properties SECONDARYVISIBILITY of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define SECONDARYVISIBILITY_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property RECEIVESHADOWS of core interface LAYERPROPERTIES is 22
#define RECEIVESHADOWS_ILAYERPROPERTIES_GETTER 22
// function_id of for set property RECEIVESHADOWS of core interface LAYERPROPERTIES is 23
#define RECEIVESHADOWS_ILAYERPROPERTIES_SETTER 23
// parameter type of properties RECEIVESHADOWS of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define RECEIVESHADOWS_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property CASTSHADOWS of core interface LAYERPROPERTIES is 24
#define CASTSHADOWS_ILAYERPROPERTIES_GETTER 24
// function_id of for set property CASTSHADOWS of core interface LAYERPROPERTIES is 25
#define CASTSHADOWS_ILAYERPROPERTIES_SETTER 25
// parameter type of properties CASTSHADOWS of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define CASTSHADOWS_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property APPLYATMOSPHERICS of core interface LAYERPROPERTIES is 26
#define APPLYATMOSPHERICS_ILAYERPROPERTIES_GETTER 26
// function_id of for set property APPLYATMOSPHERICS of core interface LAYERPROPERTIES is 27
#define APPLYATMOSPHERICS_ILAYERPROPERTIES_SETTER 27
// parameter type of properties APPLYATMOSPHERICS of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define APPLYATMOSPHERICS_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property RENDEROCCLUDED of core interface LAYERPROPERTIES is 28
#define RENDEROCCLUDED_ILAYERPROPERTIES_GETTER 28
// function_id of for set property RENDEROCCLUDED of core interface LAYERPROPERTIES is 29
#define RENDEROCCLUDED_ILAYERPROPERTIES_SETTER 29
// parameter type of properties RENDEROCCLUDED of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define RENDEROCCLUDED_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property ISHIDDEN of core interface LAYERPROPERTIES is 30
#define ISHIDDEN_ILAYERPROPERTIES_GETTER 30
// function_id of for set property ISHIDDEN of core interface LAYERPROPERTIES is 31
#define ISHIDDEN_ILAYERPROPERTIES_SETTER 31
// parameter type of properties ISHIDDEN of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define ISHIDDEN_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property ISFROZEN of core interface LAYERPROPERTIES is 32
#define ISFROZEN_ILAYERPROPERTIES_GETTER 32
// function_id of for set property ISFROZEN of core interface LAYERPROPERTIES is 33
#define ISFROZEN_ILAYERPROPERTIES_SETTER 33
// parameter type of properties ISFROZEN of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define ISFROZEN_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property BOXMODE of core interface LAYERPROPERTIES is 34
#define BOXMODE_ILAYERPROPERTIES_GETTER 34
// function_id of for set property BOXMODE of core interface LAYERPROPERTIES is 35
#define BOXMODE_ILAYERPROPERTIES_SETTER 35
// parameter type of properties BOXMODE of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define BOXMODE_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property BACKFACECULL of core interface LAYERPROPERTIES is 36
#define BACKFACECULL_ILAYERPROPERTIES_GETTER 36
// function_id of for set property BACKFACECULL of core interface LAYERPROPERTIES is 37
#define BACKFACECULL_ILAYERPROPERTIES_SETTER 37
// parameter type of properties BACKFACECULL of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define BACKFACECULL_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property ALLEDGES of core interface LAYERPROPERTIES is 38
#define ALLEDGES_ILAYERPROPERTIES_GETTER 38
// function_id of for set property ALLEDGES of core interface LAYERPROPERTIES is 39
#define ALLEDGES_ILAYERPROPERTIES_SETTER 39
// parameter type of properties ALLEDGES of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define ALLEDGES_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property VERTEXTICKS of core interface LAYERPROPERTIES is 40
#define VERTEXTICKS_ILAYERPROPERTIES_GETTER 40
// function_id of for set property VERTEXTICKS of core interface LAYERPROPERTIES is 41
#define VERTEXTICKS_ILAYERPROPERTIES_SETTER 41
// parameter type of properties VERTEXTICKS of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define VERTEXTICKS_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property SHOWTRAJECTORY of core interface LAYERPROPERTIES is 42
#define SHOWTRAJECTORY_ILAYERPROPERTIES_GETTER 42
// function_id of for set property SHOWTRAJECTORY of core interface LAYERPROPERTIES is 43
#define SHOWTRAJECTORY_ILAYERPROPERTIES_SETTER 43
// parameter type of properties SHOWTRAJECTORY of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define SHOWTRAJECTORY_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property XRAY of core interface LAYERPROPERTIES is 44
#define XRAY_ILAYERPROPERTIES_GETTER 44
// function_id of for set property XRAY of core interface LAYERPROPERTIES is 45
#define XRAY_ILAYERPROPERTIES_SETTER 45
// parameter type of properties XRAY of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define XRAY_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property IGNOREEXTENTS of core interface LAYERPROPERTIES is 46
#define IGNOREEXTENTS_ILAYERPROPERTIES_GETTER 46
// function_id of for set property IGNOREEXTENTS of core interface LAYERPROPERTIES is 47
#define IGNOREEXTENTS_ILAYERPROPERTIES_SETTER 47
// parameter type of properties IGNOREEXTENTS of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define IGNOREEXTENTS_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property SHOWFROZENINGRAY of core interface LAYERPROPERTIES is 48
#define SHOWFROZENINGRAY_ILAYERPROPERTIES_GETTER 48
// function_id of for set property SHOWFROZENINGRAY of core interface LAYERPROPERTIES is 49
#define SHOWFROZENINGRAY_ILAYERPROPERTIES_SETTER 49
// parameter type of properties SHOWFROZENINGRAY of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define SHOWFROZENINGRAY_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property SHOWVERTEXCOLORS of core interface LAYERPROPERTIES is 50
#define SHOWVERTEXCOLORS_ILAYERPROPERTIES_GETTER 50
// function_id of for set property SHOWVERTEXCOLORS of core interface LAYERPROPERTIES is 51
#define SHOWVERTEXCOLORS_ILAYERPROPERTIES_SETTER 51
// parameter type of properties SHOWVERTEXCOLORS of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define SHOWVERTEXCOLORS_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property VERTEXCOLORSSHADED of core interface LAYERPROPERTIES is 52
#define VERTEXCOLORSSHADED_ILAYERPROPERTIES_GETTER 52
// function_id of for set property VERTEXCOLORSSHADED of core interface LAYERPROPERTIES is 53
#define VERTEXCOLORSSHADED_ILAYERPROPERTIES_SETTER 53
// parameter type of properties VERTEXCOLORSSHADED of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define VERTEXCOLORSSHADED_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property VISIBILITY of core interface LAYERPROPERTIES is 54
#define VISIBILITY_ILAYERPROPERTIES_GETTER 54
// function_id of for set property VISIBILITY of core interface LAYERPROPERTIES is 55
#define VISIBILITY_ILAYERPROPERTIES_SETTER 55
// parameter type of properties VISIBILITY of core interface LAYERPROPERTIES is ParamType::TYPE_FLOAT
#define VISIBILITY_ILAYERPROPERTIES_TYPEPARAM ParamType::TYPE_FLOAT
// function_id of for get property IMAGEMOTIONBLURMULTIPLIER of core interface LAYERPROPERTIES is 56
#define IMAGEMOTIONBLURMULTIPLIER_ILAYERPROPERTIES_GETTER 56
// function_id of for set property IMAGEMOTIONBLURMULTIPLIER of core interface LAYERPROPERTIES is 57
#define IMAGEMOTIONBLURMULTIPLIER_ILAYERPROPERTIES_SETTER 57
// parameter type of properties IMAGEMOTIONBLURMULTIPLIER of core interface LAYERPROPERTIES is ParamType::TYPE_FLOAT
#define IMAGEMOTIONBLURMULTIPLIER_ILAYERPROPERTIES_TYPEPARAM ParamType::TYPE_FLOAT
// function_id of for get property MOTIONBLURON of core interface LAYERPROPERTIES is 58
#define MOTIONBLURON_ILAYERPROPERTIES_GETTER 58
// function_id of for set property MOTIONBLURON of core interface LAYERPROPERTIES is 59
#define MOTIONBLURON_ILAYERPROPERTIES_SETTER 59
// parameter type of properties MOTIONBLURON of core interface LAYERPROPERTIES is ParamType2::TYPE_bool
#define MOTIONBLURON_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_bool
// function_id of for get property MOTIONBLUR of core interface LAYERPROPERTIES is 60
#define MOTIONBLUR_ILAYERPROPERTIES_GETTER 60
// function_id of for set property MOTIONBLUR of core interface LAYERPROPERTIES is 61
#define MOTIONBLUR_ILAYERPROPERTIES_SETTER 61
// parameter type of properties MOTIONBLUR of core interface LAYERPROPERTIES is ParamType2::TYPE_ENUM
#define MOTIONBLUR_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_ENUM
// function_id of for get property DISPLAY of core interface LAYERPROPERTIES is 62
#define DISPLAY_ILAYERPROPERTIES_GETTER 62
// function_id of for set property DISPLAY of core interface LAYERPROPERTIES is 63
#define DISPLAY_ILAYERPROPERTIES_SETTER 63
// parameter type of properties DISPLAY of core interface LAYERPROPERTIES is ParamType2::TYPE_ENUM
#define DISPLAY_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_ENUM
// function_id of for get property INODEGIPROPERTIES of core interface LAYERPROPERTIES is 64
#define INODEGIPROPERTIES_ILAYERPROPERTIES_GETTER 64
// function_id of for set property INODEGIPROPERTIES of core interface LAYERPROPERTIES is -1
#define INODEGIPROPERTIES_ILAYERPROPERTIES_SETTER -1
// parameter type of properties INODEGIPROPERTIES of core interface LAYERPROPERTIES is ParamType2::TYPE_INTERFACE
#define INODEGIPROPERTIES_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_INTERFACE
// function_id of for get property LAYERASREFTARG of core interface LAYERPROPERTIES is 66
#define LAYERASREFTARG_ILAYERPROPERTIES_GETTER 66
// function_id of for set property LAYERASREFTARG of core interface LAYERPROPERTIES is -1
#define LAYERASREFTARG_ILAYERPROPERTIES_SETTER -1
// parameter type of properties LAYERASREFTARG of core interface LAYERPROPERTIES is ParamType2::TYPE_REFTARG
#define LAYERASREFTARG_ILAYERPROPERTIES_TYPEPARAM ParamType2::TYPE_REFTARG
