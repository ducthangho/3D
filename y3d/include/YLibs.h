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



#define AUTODESK360_ID  Interface_ID(1376063257,1698699477)
#define ISHADERMANAGERCREATOR_ID  Interface_ID(1458049944,2139189071)
#define VISUALMS_ID  Interface_ID(1111305458,1382483637)
#define MAXNETWORKERINTERFACE_ID  Interface_ID(386601748,1410203795)
#define AUTOTANGENTMAN_ID  Interface_ID(322453473,845511110)
#define INTERFACEIDREGISTRY_ID  Interface_ID(1358439758,647696761)
#define MENUITEM_ID  Interface_ID(781347793,695101686)
#define OGSDIAGNOSTICS_ID  Interface_ID(1338188964,577923427)
#define MANIP_ID  Interface_ID(742722210,2073887589)
#define NODEANDANIMS_ID  Interface_ID(1255226523,1017740268)
#define AUTODESK360INTERNAL_ID  Interface_ID(1969185855,479818056)
#define GIZMOSHAPE_ID  Interface_ID(307114345,252082900)
#define CUSTOMCONTROLSOPTIONS_ID  Interface_ID(2927575136,189750493)
#define SCHEMATICVIEW_ID  Interface_ID(1928336838,499527471)
#define EDITRENDERREGION_ID  Interface_ID(1216413818,1918709371)
#define INODE_ID  Interface_ID(1739658186,874531627)
#define XMLANIMTREEENTRYLIST_ID  Interface_ID(886402037,455769216)
#define CHECKABORTCALLBACK_ID  Interface_ID(6710580,327937)
#define ASSETUSER_ID  Interface_ID(855096527,4052227430)
#define MENU_ID  Interface_ID(1272282670,1843755755)
#define IDISPLAYGAMMA_ID  Interface_ID(46551913,280901808)
#define XVIEWCHECKER_ID  Interface_ID(910389626,1500450927)
#define INODEGIPROPERTIES_ID  Interface_ID(2144679988,998581685)
#define MXSDEBUGGER_ID  Interface_ID(547915770,11259375)
#define COLORMAN_ID  Interface_ID(469003408,415195545)
#define HELPSYSTEM_ID  Interface_ID(978059971,77073496)
#define GHOSTINGMANAGER_ID  Interface_ID(389967592,525821491)
#define RETARGETDATA_ID  Interface_ID(2094092873,244845367)
#define NULLINTERFACE_ID  Interface_ID(0,0)
#define LIGHTINGUNITS_ID  Interface_ID(7489244,4424924)
#define UTILITYPANEL_ID  Interface_ID(7632604,4416732)
#define MRPBPARAMETERCLASSDESCCREATOR_ID  Interface_ID(306532324,1034451654)
#define LOCKEDTRACKSMAN_ID  Interface_ID(808996836,1030060742)
#define MISSINGUVCOORDINATES_ID  Interface_ID(308375515,938439324)
#define REFHIERARCHY_ID  Interface_ID(695084947,612176612)
#define VIEWPANELMANAGER_ID  Interface_ID(213651878,2872316928)
#define WORKSPACEMANAGER_ID  Interface_ID(215749030,2872368128)
#define MOUSECONFIGMANAGER_ID  Interface_ID(1467764899,1000886811)
#define PARTICLEFLOW_ID  Interface_ID(172048907,515065088)
#define PLACEMENTTOOL_ID  Interface_ID(2858448058,7555153)
#define DAYLIGHTSIMULATIONUTILITIES_ID  Interface_ID(813503787,1995525408)
#define CUIMOUSECONFIGMANAGERIMPLEMENT_ID  Interface_ID(2030859971,1495021960)
#define ROLLUP_ID  Interface_ID(672818664,316342877)
#define VIEWPORTBUTTONMGR_ID  Interface_ID(1202397589,585634414)
#define IINTERACTIONMODE_ID  Interface_ID(2570487723,1019706129)
#define XMLANIMTREEENTRY_ID  Interface_ID(97671869,915622911)
#define VFBINTERFACE_ID  Interface_ID(1302625467,1883315201)
#define ASSETMANAGER_ID  Interface_ID(892540025,1402302696)
#define FILERESOLUTIONMANAGER_ID  Interface_ID(1734028459,436084653)
#define CMDPANEL_ID  Interface_ID(1092047862,1772697360)
#define TIPSYSTEM_ID  Interface_ID(281084309,575537777)
#define MAXTHUMBNAILMGR_ID  Interface_ID(1960065413,881355304)
#define MANIPPLANE_ID  Interface_ID(1145441956,200752102)
#define SIMPLEMANIP_ID  Interface_ID(1635533268,1794140767)
#define MESHINSPECTOR_ID  Interface_ID(1133126229,4019589752)
#define UIACCESSOR_ID  Interface_ID(2043943873,1717126755)
#define SCENERADIOSITY_ID  Interface_ID(108076666,1531988906)
#define PARAMPUBLISHMGR_ID  Interface_ID(1097662977,178322467)
#define IRTSHADETREECOMPILER_ID  Interface_ID(2125677985,728188286)
#define VALUECONVERTER_ID  Interface_ID(435387295,971855488)
#define _ID  Interface_ID(1225645209,199810198)
#define PARAMWIRE_ID  Interface_ID(1225592473,199785622)
#define ASSIGNVERTEXCOLORS_ID  Interface_ID(2005339952,1322789730)
#define MSZIP_ID  Interface_ID(642329730,284971536)
#define BITMAPLAYERMANAGER_ID  Interface_ID(358819484,2126781833)
#define RENDERMESSAGEMANAGER_ID  Interface_ID(2035175835,980697226)
#define EXCHANGESTOREPACKAGEMANAGER_ID  Interface_ID(136446544,718017790)
#define MAXOPS_ID  Interface_ID(1686454563,2073974044)
#define VIEWPORTSSB_ID  Interface_ID(2832751933,217335682)
#define CUSTATTRIBCOLLAPSEMANAGER_ID  Interface_ID(2059147430,712921643)
#define QUADMENU_ID  Interface_ID(2025272809,2080382824)
#define MENTAL_RAY_PUBLIC_INTERFACE_ID  Interface_ID(449406152,1585452)
#define QUADMENUSETTINGS_ID  Interface_ID(827727323,439305753)
#define DRAGANDDROP_ID  Interface_ID(1360412123,709826073)
#define MENUMAN_ID  Interface_ID(182198461,1955689501)
#define ITABDIALOGMANAGER_ID  Interface_ID(1179016877,7224507)
#define _ID  Interface_ID(2935694336,497504104)
#define NITROUSGRAPHICSMANAGER_ID  Interface_ID(1933676424,1388333600)
#define DISPLAYMANAGER_ID  Interface_ID(1898024808,1386957329)
#define ACTIONMAN_ID  Interface_ID(1270291065,1314070095)
#define PEREZALLWEATHER_ID  Interface_ID(1470397520,531836486)
#define SIMPLEFACEMANAGER_ID  Interface_ID(98967576,175326846)
#define ASSEMBLYMGR_ID  Interface_ID(1466716815,1556231687)
#define GRIDPREFS_ID  Interface_ID(2093774837,877559607)
#define VIEWCUBEOPS_ID  Interface_ID(500566519,1441476149)
#define STEERINGWHEELSOPS_ID  Interface_ID(500566520,1441476148)
#define LOCKEDCOMPONENTSMAN_ID  Interface_ID(1668561553,130774427)
#define FILELINKMGR_ID  Interface_ID(2145076638,779169934)
#define DEFAULTACTIONS_ID  Interface_ID(1541517831,2014554661)
#define HEIGHTMANAGER_ID  Interface_ID(2019763497,3245987957)
#define ACTIONITEMOVERRIDEMANAGER_ID  Interface_ID(1578854123,1270887646)
#define IPARSERLOADER_ID  Interface_ID(2143175382,1048527147)
#define GLOBALDXDISPLAYMANAGER_ID  Interface_ID(2126386646,729498155)
#define LOADSAVEANIMATION_ID  Interface_ID(2115723367,1866925320)
#define MTLBROWSERFILTER_MANAGER_ID  Interface_ID(924535568,1819368380)
#define INODEDISPLAYPROPERTIES_ID  Interface_ID(1156523281,1795628697)
#define INODESHADINGPROPERTIES_ID  Interface_ID(2060260337,724177549)
#define SLATEDRAGDROPBRIDGE_ID  Interface_ID(672467892,1550659632)
#define INODEBAKEPROPERTIES_ID  Interface_ID(1046286136,937754796)
#define RADIOSITYPREFERENCES_ID  Interface_ID(1413754432,1075192228)
#define NVPXCONSTS_ID  Interface_ID(1417942952,1089549608)
#define ISOLATESELECTION_ID  Interface_ID(1051028785,1057181599)
#define HDIKSYS_ID  Interface_ID(2458843042,3575642490)
#define IVIEWPORTSHADINGMGR_ID  Interface_ID(2126136785,725435723)
#define VIEWPORTBASICSETTINGS_ID  Interface_ID(603683551,523489747)
#define BONESYS_ID  Interface_ID(1133182834,4019615148)
#define VIEWPORTGISETTINGS_ID  Interface_ID(603840815,718524911)
#define VIEWPORTSHADOWSETTINGS_ID  Interface_ID(3889803234,80168202)
#define WORKINGPIVOT_ID  Interface_ID(1856389497,590943741)
#define ILIGHTREF_ID  Interface_ID(2059477921,727086362)
#define IXREFRECORD_ID  Interface_ID(440761297,123228144)
#define MAINTHREADTASKMANAGER_ID  Interface_ID(2037721139,1950040974)
#define MEDIT_ID  Interface_ID(746274670,385562068)
#define SCENEMISSINGPLUGINS_ID  Interface_ID(2881102082,270688775)
#define MENTAL_RAY_PREFERENCES_ID  Interface_ID(1497698764,357587884)
#define OPENEDGES_ID  Interface_ID(292387779,646064053)
#define NAMEDSELECTIONSETMANAGER_ID  Interface_ID(69878451,30350533)
#define NETWORKLICENSESTATUSMANAGER_ID  Interface_ID(1798405062,1398561805)
#define INODEBAKEPROJPROPERTIES_ID  Interface_ID(1507400101,1874397829)
#define NVIDIARENDERERSHELPER_ID  Interface_ID(1385439053,1737712753)
#define REGISTEREDDIALOGMGR_ID  Interface_ID(857165949,872182238)
#define BAKEELEMENTPROPERTIES_ID  Interface_ID(2489927521,392307233)
#define RENDERENHANCEMENTS_ID  Interface_ID(685839159,945968203)
#define RENDERPRESETS_ID  Interface_ID(1085489140,1413887558)
#define QAT_ID  Interface_ID(1385439133,1771267313)
#define TMGIZMOS_ID  Interface_ID(168781500,2078565842)
#define NODESELECTIONSET_ID  Interface_ID(2125540370,893252321)
#define FRAMETAGMANAGER_ID  Interface_ID(2362916586,1753947921)
#define SCENEEXPOSURECONTROL_ID  Interface_ID(358819484,2126781828)
#define HAIR_ID  Interface_ID(1252225077,255486061)
#define TRACKBAR_ID  Interface_ID(721368407,380268875)
#define TRACKVIEWS_ID  Interface_ID(1394368300,1876896207)
#define TRACKVIEW_ID  Interface_ID(1627094767,527265235)
#define BIPFIXER_ID  Interface_ID(265120536,1852919331)
#define TRACKSELECTIONSETS_ID  Interface_ID(418605700,525807287)
#define OBJXREFS_ID  Interface_ID(2128485477,893200159)
#define AUTOSAVE_ID  Interface_ID(1303079161,1044394694)
#define TIMESLIDER_ID  Interface_ID(2191428069,2274293477)
#define IMERGE_ID  Interface_ID(3968777055,1873017195)
#define SUBSTMANAGER_ID  Interface_ID(97583357,1791454635)
#define SCHEMATICVIEWS_ID  Interface_ID(673407154,536500214)
#define NODECLONEMGRTEST_ID  Interface_ID(1830362228,19945922)
#define OBJXREFMGR_ID  Interface_ID(945249878,998582883)
#define LAYERMANAGER_ID  Interface_ID(2013206281,421031574)
#define SCENECONVERTER_ID  Interface_ID(1580926518,665812455)
#define BLOCKMGR_ID  Interface_ID(847068273,1412136364)
#define STYLEMGR_ID  Interface_ID(1032131825,2089964207)
#define BROWSERMGR_ID  Interface_ID(1697915768,1280576827)
#define BROWSER_ID  Interface_ID(1956785128,417404873)
#define BATCHRENDERMGR_ID  Interface_ID(2268819646,1273969592)
#define _ID  Interface_ID(488797027,1517435457)
#define BITMAPPROXYMGR_ID  Interface_ID(618800424,1612658192)
#define CHANNELINFO_ID  Interface_ID(1133121826,4019611204)
#define RADIOSITYMESHOPS_ID  Interface_ID(978203433,688089517)
#define SRR_EXPORTS_ID  Interface_ID(1809280194,1298814315)
#define REVITDBMANAGER_ID  Interface_ID(1844018212,1089678965)
#define FLIGHTSTUDIO_ID  Interface_ID(1112298821,1380646914)
#define INSTANCEMGR_ID  Interface_ID(1590522037,1493770839)
#define BRUSHPRESETMGR_ID  Interface_ID(1483414873,1162676969)
#define BATCHPROOPTIMIZER_ID  Interface_ID(620052633,122040040)
#define SCENEEFFECTLOADER_ID  Interface_ID(1132344873,4019595624)
#define DAYLIGHTSYSTEMFACTORY_ID  Interface_ID(1550212039,1639205424)
#define SCENESTATEMGR_ID  Interface_ID(3970416014,3549644393)
#define SKINUTILS_ID  Interface_ID(1419391829,4019681041)
#define IVNORMALMGR_ID  Interface_ID(682575162,515406416)
#define VISUALMSCA_ID  Interface_ID(1080700801,1212550665)
#define BOOLEANOBJECTMANAGER_ID  Interface_ID(1500512762,1068254490)
#define _ID  Interface_ID(1521095192,900156130)
#define CONTAINERS_ID  Interface_ID(3646009065,1466557579)
#define CONTAINERPREFERENCES_ID  Interface_ID(4198190592,4011565095)
#define FLOWRAYTRACEINTERFACE_ID  Interface_ID(1393116116,671896709)
#define EPOLYMANIPGRIP_ID  Interface_ID(432951612,2068468018)
#define ONECLICKREVIT_ID  Interface_ID(1381321259,309690023)
#define PAINTDEFORMPRESETCONTEXT_ID  Interface_ID(913723284,1446607678)
#define PAINTSOFTSELPRESETCONTEXT_ID  Interface_ID(1265644843,155533957)
#define LIGHTMETERMANAGER_ID  Interface_ID(1771065753,1485048929)
#define PSEUDOCOLORMANAGER_ID  Interface_ID(2056531760,1156846094)
#define ICEFLOWFILEBIRTHSETUP_ID  Interface_ID(1414875547,1005003478)
#define ICEFLOWSHAPECONTROL_ID  Interface_ID(850335558,1020753364)
#define PARTICLEFLOWUTILITY_ID  Interface_ID(250632722,678831839)
#define IMETADATAMANAGER_ID  Interface_ID(3705476920,367937425)
#define PFTPARTICLEVIEW_ID  Interface_ID(959856073,515065088)
#define ICEFLOWSYSTEMFACTORY_ID  Interface_ID(254302874,962099923)
#define RINGARRAY_ID  Interface_ID(2052748525,1378895393)
#define PLUGINMANAGER_ID  Interface_ID(1580926966,658472423)
#define OFFSETMANAGER_ID  Interface_ID(1133161011,4019541418)
#define CREASESETMANAGER_ID  Interface_ID(758258305,387204809)
#define PHYSXPANELINTERFACE_ID  Interface_ID(549201725,101931112)
#define POP_ID  Interface_ID(1353990885,835212591)
#define ICCPHYSXINTEGRATOREXTFACTORY_ID  Interface_ID(1277635928,334045884)
#define NVPX_ID  Interface_ID(1309373008,385289738)
#define NETSERVER_ID  Interface_ID(600404208,2032546930)
#define BIPWORKBENCH_ID  Interface_ID(2024418345,430267705)
#define BIPANALYZER_ID  Interface_ID(348075212,1369179699)
#define DXSHADERMANAGER_ID  Interface_ID(1842246697,536963500)
#define BIPFILTER_ID  Interface_ID(919220271,600917926)
#define ANIMLAYERMANAGER_ID  Interface_ID(238305013,1653885890)
#define IKSYS_ID  Interface_ID(1534281217,2088533710)
#define REACTIONMGR_ID  Interface_ID(269041914,1135229442)
#define RETIMERMAN_ID  Interface_ID(1966755921,1818706134)
#define REVITIMPORTWORKFLOW_ID  Interface_ID(1645163041,1674920618)
#define FLIGHTSTUDIOIMPORT_ID  Interface_ID(1112298821,1380646913)
#define AUTODESKMATERIALMANAGER_ID  Interface_ID(143394822,541594678)
#define IVNORMALMGR_ID  Interface_ID(698822015,1811884374)
#define DWFEXPORTPREFERENCES_ID  Interface_ID(588141928,14494243)
#define FLIGHTSTUDIOEXPORT_ID  Interface_ID(1112298821,1380646912)
#define LUMINOSITYUTIL_ID  Interface_ID(1160467628,1459821982)
#define LIGHTINGANALYSISOVERLAYFACTORY_ID  Interface_ID(1532652071,651113560)
#define IBITMAPPAGER_ID  Interface_ID(1222706207,885550161)
#define IBITMAPPAGERINTERNAL_ID  Interface_ID(1297760989,1754071940)
#define ATSOPS_ID  Interface_ID(539187234,40051095)
#define ATSCUSTOMDEPSOPS_ID  Interface_ID(37452336,414410938)
#define IAUTOCAMMAX_ID  Interface_ID(1270241922,664042035)
#define GRIP_ID  Interface_ID(1506496828,2047496754)
#define DIALOGMONITOROPS_ID  Interface_ID(1344545339,1281369830)
#define TVERTS_ID  Interface_ID(678891259,1267082641)
#define OVERLAPPEDUVWFACES_ID  Interface_ID(968519237,533680163)
#define OVERLAPPINGVERTICES_ID  Interface_ID(1138182846,63060668)
#define ISOLATEDVERTICES_ID  Interface_ID(1177826589,291850295)
#define FACESORIENTATION_ID  Interface_ID(1249991412,1220895746)
#define OVERLAPPINGFACES_ID  Interface_ID(1423777604,1244334934)
#define FLIPPEDUVWFACES_ID  Interface_ID(1780366451,1169629585)
#define MULTIPLEEDGES_ID  Interface_ID(2057254180,1237845172)
#define MATERIALPREVIEWSYSTEM_ID  Interface_ID(567570953,1508598959)
#define PYTHON_ID  Interface_ID(1861446031,1460355352)
#define PARAMCOLLECTOROPS_ID  Interface_ID(722805811,776546601)
#define SME_ID  Interface_ID(916603886,968045014)
#define WALKTHROUGHOPS_ID  Interface_ID(966592606,159537997)
#define PROJECTIONRENDERMGR_ID  Interface_ID(1161367137,1121860407)
#define PROJECTIONINTERSECTORMGR_ID  Interface_ID(1165836333,921182019)
#define METADATAOBJECT_ID  Interface_ID(792865316,326781236)
#define MENTAL_RAY_STRING_OPTIONS_ID  Interface_ID(1356758484,199306910)
#define IRAY_STRING_OPTIONS_ID  Interface_ID(7143993,665418191)
#define PHYSSUNSKY_SHADERGENERATOR_ID  Interface_ID(561609030,149947315)
#define BOOLEANEXPLORERMANAGER_ID  Interface_ID(6704926,13242848)
#define MAXRIBBON_ID  Interface_ID(2868181064,3704778183)
#define CREASEEXPLORERMANAGER_ID  Interface_ID(2269298462,3396847345)
#define DAYLIGHTSIMULATIONUIOPS_ID  Interface_ID(1546519456,136069790)
#define NETWORKMANAGER_ID  Interface_ID(175329981,1639670434)
#define MATERIALEXPLORERMANAGER_ID  Interface_ID(2269275937,3401273841)
#define SCENEEXPLORERMANAGER_ID  Interface_ID(675309344,2056999664)
#define DAYLIGHTSYSTEMFACTORY2_ID  Interface_ID(1754228390,1111111594)
#define STATUSPANEL_ID  Interface_ID(155408368,1648259522)
#define NETJOBSERVERINFO_ID  Interface_ID(1406145118,841440290)
#define NETFRAMEINFO_ID  Interface_ID(1825517918,1657486952)
#define NETRENDERELEMENT_ID  Interface_ID(1402159257,1181305855)
#define NETSTATUS_ID  Interface_ID(484929071,1148211986)
#define NETJOB_ID  Interface_ID(823598718,2088132980)
#define MEMSTREAMMGR_ID  Interface_ID(654645223,703875555)
#define NETMANAGER_ID  Interface_ID(623869216,860060330)
#define NETRENDER_ID  Interface_ID(1839744861,1915505444)
#define NETCREATEHELPERS_ID  Interface_ID(30823185,8285579)
#define MXSUNITRESULTS_ID  Interface_ID(1737048865,2441245540)
#define INODEMENTALRAYPROPERTIES_ID  Interface_ID(477509921,1297360722)