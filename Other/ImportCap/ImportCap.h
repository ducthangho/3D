#pragma once

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/


#include <kj/common.h>
#include <kj/exception.h>
#include <kj/io.h>
#include <kj/std/iostream.h>
#include <capnp/serialize.h>
#include <capnp/message.h>
#include <log.h>
#include <maxscript/maxscript.h>
#include "3d.capnp.h"
#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
//SIMPLE TYPE

#include <maxapi.h>
#include <ostream>
#include <MeshNormalSpec.h>
#include <mnmesh.h>
#include <bitarray.h>
#include <mesh.h>
#include <inode.h>
#include "polyobj.h"
#include "triobj.h"
#include "patchobj.h"
#include "IGame/IGame.h"
#include "ObjectWrapper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include "ICustAttribCollapseManager.h"
#include "ICustAttribContainer.h"
#include "modstack.h"
#include <map>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <string>
#include <impapi.h>
#include <impexp.h>
#include <direct.h>
#include <commdlg.h>
#include <dummy.h>
#include <IViewPanel.h>



//#include "tbb/blocked_range.h"
//#include "tbb/parallel_for.h"
//#include "tbb/task_scheduler_init.h"
//SIMPLE TYPE


#define USE_IMPNODES 1



extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

enum ExportOpts {
	mesh = 1,
	normals = 1 << 1,
	uv = 1 << 2,
	selection = 1 << 3,
	all = mesh | normals | uv | selection
};

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

#define NO_GROUP  0
#define GROUP_HEAD 1
#define GROUP_OPEN_HEAD 2
#define GROUP_MEMBER 3
#define GROUP_OPEN_MEMBER 4


#define CTL_CHARS  31
#define SINGLE_QUOTE 39

typedef Tab<INodeTab> NodeTabTab;
typedef std::vector<TSTR> GroupNames;

static Interface*		ip = GetCOREInterface();
static ImpInterface*	impip;
static Matrix3 ident(TRUE);
static BOOL autoConv = 0;
// Some handy macros
static float msc_wk;
static BOOL resetScene = FALSE;


template <class T>
struct MyHash;

template<>
struct MyHash<TSTR> {
	size_t operator()(TSTR const &s) const {
		return std::hash<std::wstring>()(s.data());
	}
};

inline UVVert* getUVVerts(MNMap& out) {
	return out.v;
}


inline UVVert* getUVVerts(MeshMap& out) {
	return out.tv;
}

inline MNMapFace* getUVFaces(MNMap& out) {
	return out.f;
}


inline TVFace* getUVFaces(MeshMap& out) {
	return out.tf;
}


inline BOOL TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0), m.GetRow(1)), m.GetRow(2))<0.0) ? 1 : 0;
}

inline void checkData(MNMesh& mesh) {
	if (!mesh.CheckAllData())
		mprintf(L"Something is wrong\n");
}

inline void checkData(Mesh& mesh) {};



inline Matrix3 getTM(INode* node) {

	if (!node) return ident;
	//Matrix3 mat = node->GetObjTMAfterWSM(ip->GetTime());
	Matrix3 mat = node->GetObjectTM(ip->GetTime());

	// RB: For some reason this fixes a problem where
	// the pivot gets screwed up if the object is at 
	// the origin. ???!!!			
	Point3 pos = mat.GetRow(3);
	if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f) {
		pos.x = 0.001f;
		pos.y = 0.001f;
		pos.z = 0.001f;
		mat.SetTrans(pos);
	}
	if (node->GetProperty(PROPID_HAS_WSM)) {
		mat = ident;
	}

	/*if (!node->GetParentNode()->IsRootNode()) {
	Matrix3 iptm = node->GetParentNode()->GetObjTMAfterWSM(ip->GetTime());
	mat = mat * Inverse(iptm);
	}	*/
	return mat;
}

// Replace some characters we don't care for.
inline TCHAR* FixupName(const TCHAR* name)
{
	static TCHAR buffer[256];
	TCHAR* cPtr;

	_tcscpy(buffer, name);
	cPtr = buffer;

	while (*cPtr) {
		if (*cPtr == _T('"'))
			*cPtr = SINGLE_QUOTE;
		else if (*cPtr <= CTL_CHARS)
			*cPtr = _T('_');
		cPtr++;
	}

	return buffer;
}


inline void time(Time& t, int type) {
	times[type] += t.duration();
}

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

inline void print(Point3& p) {
	mprintf(L" (%f, %f, %f) \n", p.x, p.y, p.x);
}

inline void print(Quat& p) {
	mprintf(L" (%f, %f, %f,  %f) \n", p.x, p.y, p.z, p.w);
}

inline void print(ScaleValue& sv) {
	auto& p = sv.s;
	auto& q = sv.q;
	mprintf(L" Scale component = (%f, %f, %f) ;  Axixs system = (%f, %f, %f,  %f) \n", p.x, p.y, p.z, q.x, q.y, q.z, q.w);
}


inline void printTM(Matrix3& tm) {
	mprintf(L"TM = [ \n");
	for (int i = 0; i < 4; ++i) {
		mprintf(L"P[%d] = ", i);
		print(tm.GetRow(i));
	}
	mprintf(L"]\n");
}


inline void printNodeTM(INode* node) {
	auto tm = getTM(node);
	printTM(tm);

	mprintf(L"Parent TM =\n");
	printTM(node->GetParentTM(ip->GetTime()));
	auto ofsp = node->GetObjOffsetPos();
	mprintf(L" GetObjOffsetPos() = ");
	print(ofsp);
	auto rot = node->GetObjOffsetRot();
	mprintf(L" GetObjOffsetRot () = ");
	print(rot);

	auto sv = node->GetObjOffsetScale();
	mprintf(L" GetObjOffsetScale () = ");
	print(sv);


	mprintf(L"----------------------------\n");
}

inline GenFace& faceRef(ObjectWrapper& in, int i) {
	return in.Faces()[i];
}

inline Object* getObject(INode* node) {
	if (!node) return nullptr;
	Time t;
	auto& os = node->EvalWorldState(GetCOREInterface()->GetTime());
	time(t, EVAL_OBJECT);
	return os.obj;
}


inline MNFace& faceRef(MNMesh& in, int i) {
	return in.f[i];
}

inline Face& faceRef(Mesh& in, int i) {
	return in.faces[i];
}

inline void VertexSelect(ObjectWrapper& o, BitArray& sels) {
	o.SetVertSel(sels);
}


inline void VertexSelect(MNMesh& mesh, BitArray& sels) {
	mesh.VertexSelect(sels);
}

inline void VertexSelect(Mesh& mesh, BitArray& sels) {
	mesh.vertSel = sels;
}

inline void FaceSel(ObjectWrapper& mesh, BitArray& sels) {
	mesh.SetFaceSel(sels);
}


inline void FaceSel(MNMesh& mesh, BitArray& sels) {
	mesh.FaceSelect(sels);
}

inline void FaceSel(Mesh& mesh, BitArray& sels) {
	mesh.faceSel = sels;
}

inline void EdgeVis(MNMesh& mesh, BitArray& sels) {
	int k = 0;
	for (int i = 0; i < mesh.numf; ++i) {
		auto& f = faceRef(mesh, i);
		//mprintf(L"Face %d:\t", i);
		for (int j = 0; j < f.deg; ++j) {
			if (sels[k++]) {
				f.visedg.Set(j);
			}
			else f.visedg.Clear(j);
		}
	}

}

inline void EdgeVis(Mesh& mesh, BitArray& sels) {
	int k = 0;
	for (int i = 0; i < mesh.numFaces; ++i) {
		auto& f = faceRef(mesh, i);
		//mprintf(L"Face %d:\t", i);
		for (int j = 0; j < 3; ++j) {
			if (sels[k++]) {
				f.setEdgeVis(j, EDGE_VIS);
				//mprintf(L"(%d, %d)\t",j,j+1);
			}
			else f.setEdgeVis(j, EDGE_INVIS);
		}
		//mprintf(L"\n");
	}

}

inline void EdgeVis(ObjectWrapper& in, BitArray& sels) {
	auto* mesh = in.GetPatchMesh();
	if (mesh) {
		return;
	}


	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {
		EdgeVis(*pmesh, sels);
		return;
	}
	auto* tmesh = in.GetTriMesh();
	if (tmesh) {
		EdgeVis(*tmesh, sels);
		return;
	}
}



inline void EdgeSel(MNMesh& mesh, BitArray& sels) {
	for (int i = 0; i < sels.GetSize(); ++i)
		if (sels[i]) mesh.SetEdgeSel(i, 1);//*/
}

inline void EdgeSel(Mesh& mesh, BitArray& sels) {
	mesh.edgeSel = sels;
}

inline void EdgeSel(ObjectWrapper& in, BitArray& sels) {
	auto* mesh = in.GetPatchMesh();
	if (mesh) {
		mesh->edgeSel = sels;
		return;
	}


	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {
		EdgeSel(*pmesh, sels);
		return;
	}
	auto* tmesh = in.GetTriMesh();
	if (tmesh) {
		EdgeSel(*tmesh, sels);
		return;
	}
}

inline int degOfFace(GenFace& f) {
	return f.numVerts;
}


inline int degOfFace(MNFace& f) {
	return f.deg;
}

inline int degOfFace(Face& f) {
	return 3;
}


template <class V_IN, class V_OUT>
inline void loadV(V_IN& vi, V_OUT& vo) {
	vo.orig = vi.getOrig();
	vo.p.x = vi.getX();
	vo.p.y = vi.getY();
	vo.p.z = vi.getZ();
	vo.ImportFlags(vi.getFlags());
}

template <class V_IN, class V_OUT>
inline void loadP(V_IN& vi, V_OUT& vo) {
	vo.x = vi.getX();
	vo.y = vi.getY();
	vo.z = vi.getZ();
}


template <class P1, class P2>
inline void printPts(P1& p0, P2& p) {
	mprintf(L" (%f, %f, %f)\t(%f, %f, %f)\n", p0.x, p0.y, p0.z, p.getX(), p.getY(), p.getZ());
}

template <class PT_IN, class PT_OUT>
inline void compareV(PT_IN& vi, PT_OUT& vo) {
	//assert(vi.orig == vo.getOrig() );
	assert(vi.x == vo.getX());
	assert(vi.y == vo.getY());
	assert(vi.z == vo.getZ());
}

template <class E_IN, class E_OUT>
inline void loadE(E_IN& vi, E_OUT& vo) {
	vo.v1 = vi.getV1();
	vo.v2 = vi.getV2();
	vo.f1 = vi.getF1();
	vo.f2 = vi.getF2();
	vo.track = vi.getTrack();
	vo.ImportFlags(vi.getFlags());
}

template <class ARR_IN, class ARR_OUT>
inline void copyArr(ARR_IN& ar1, ARR_OUT& ar2) {
	for (int i = 0; i < ar2.size(); ++i) {
		ar2.set(i, ar1[i]);
	}
}

template <class ARR_IN, class ARR_OUT>
inline void loadArr(ARR_IN& ar1, ARR_OUT& ar2) {
	for (int i = 0; i < ar1.size(); ++i) {
		ar2[i] = ar1[i];
	}
}



template <>
inline void loadArr(::capnp::List<bool>::Reader& ar1, BitArray& ar2) {
	for (int i = 0; i < ar1.size(); ++i) {
		ar2.Set(i, ar1[i]);
	}
}//*/


template <class F_IN>
inline void load3F(F_IN& vi, MNFace& vo) {
	vo.SetAlloc(3);
	vo.SetDeg(3);
	vo.vtx[0] = vi.getV0();
	vo.vtx[1] = vi.getV1();
	vo.vtx[2] = vi.getV2();
	vo.smGroup = vi.getSmGroup();
	vo.material = vi.getMaterial();
}

template <class F_IN>
inline void load3F(F_IN& vi, Face& vo) {
	vo.v[0] = vi.getV0();
	vo.v[1] = vi.getV1();
	vo.v[2] = vi.getV2();
	vo.smGroup = vi.getSmGroup();
	vo.setMatID(vi.getMaterial());
}

template <class Reader, class Face>
inline void load4F(Reader& vi, Face& vo) {

}

template <class F_IN, class F_OUT>
inline void loadF(F_IN& vi, F_OUT& vo) {
	int deg = vi.getDeg();
	vo.Init();
	vo.ImportFlags(vi.getFlags());
	//vo.Clear();

	if (deg > 0) {
		vo.SetAlloc(deg);
		vo.SetDeg(deg);

		auto& vtx = vo.vtx;
		auto ivtx = vi.getVtx();

		/*for (int i = 0; i < ivtx.size(); ++i) {
		vo.
		}*/

		//if (ivtx.size() != deg) mprintf(L"Chan : %d\t%d\n", deg, ivtx.size());
		loadArr(ivtx, vtx);

		/*auto& edg = vo.edg;
		auto iedg = vi.getEdg();
		loadArr(iedg, edg);*/

		//*/
	}




	/*auto idiag = vi.getDiag();
	vo.diag = new int[idiag.size()];
	auto& diag = vo.diag;
	loadArr(idiag, diag);*/

	vo.smGroup = vi.getSmGroup();
	vo.material = vi.getMaterial();
	vo.track = vi.getTrack();

	/*auto ivisedg = vi.getVisedg();
	vo.visedg.SetSize(ivisedg.size());
	loadArr(ivisedg, vo.visedg);

	auto iedgsel = vi.getEdgsel();
	vo.edgsel.SetSize(iedgsel.size());
	loadArr(iedgsel, vo.edgsel);

	auto ibndedg = vi.getBndedg();
	vo.bndedg.SetSize(ibndedg.size());
	loadArr(ibndedg, vo.bndedg);//*/
}


template <class F_IN, class F_OUT>
inline void loadEdgeSel(F_IN& vi, F_OUT& vo) {
	auto ivisedg = vi.getVisedg();
	vo.visedg.SetSize(ivisedg.size());
	loadArr(ivisedg, vo.visedg);

	auto iedgsel = vi.getEdgsel();
	vo.edgsel.SetSize(iedgsel.size());
	loadArr(iedgsel, vo.edgsel);

	auto ibndedg = vi.getBndedg();
	vo.bndedg.SetSize(ibndedg.size());
	loadArr(ibndedg, vo.bndedg);
}

template <class PerData_IN, class PerData_OUT>
inline void loadPerData(PerData_IN& item, PerData_OUT& vdo) {
	vdo.dnum = item.getDnum();
	vdo.type = item.getType();
	vdo.alloc = item.getAlloc();
	vdo.setAlloc(vdo.alloc);
	//mprintf(L" (%d, %d, %d)\n", vdi.dnum, vdi.type, vdi.alloc);
	auto data = item.getData();
	kj::byte* vdata = (kj::byte*)vdo.data;
	for (int j = 0; j < data.size(); ++j)
		vdata[j] = data[j];
}

template <class Message>
inline char* toBuffer(Message&  m) {
	return (char*)m.getSegmentsForOutput()[0].begin();
}

inline MNMesh& getMesh(PolyObject& o) {
	return o.GetMesh();
}


inline Mesh& getMesh(TriObject& o) {
	return o.GetMesh();
}

inline Mesh& getMesh(PatchObject& o) {
	return o.GetMesh(0);
}

inline Mesh& getMesh(Mesh& o) {
	return o;
}

inline MNMesh& getMesh(MNMesh& o) {
	return o;
}

inline PatchMesh& getMesh(PatchMesh& o) {
	return o;
}

inline Mesh& getMesh(ObjectWrapper& o) {
	return *o.GetTriMesh();
}

//inline MNMesh& getMesh(ObjectWrapper& o) {
//	return *o.GetPolyMesh();
//}



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
	//mprintf(L"Woahaaaaaaaaaaaaaaaaaa\n");
	if (o.IsSubClassOf(polyObjectClassID)) {
		//mprintf(L"polyObjectClassID\n");
		return POLYOBJ_T;
	}
	else if (o.IsSubClassOf(triObjectClassID)) {
		//mprintf(L"triObjectClassID\n");
		return TRIOBJ_T;
	}
	else if (o.IsSubClassOf(patchObjectClassID)) {
		//mprintf(L"patchObjectClassID\n");
		return PATCHOBJ_T;
	}
	//mprintf(L"Nothing\n");
	return TRIOBJ_T;
}

//template <class Obj>
inline int8_t getType(Object* o) {
	if (!o) return -1;
	//mprintf(L"Hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm\n");
	if (o->IsSubClassOf(polyObjectClassID)) {
		return POLYOBJ_T;
	}
	else if (o->IsSubClassOf(triObjectClassID)) {
		return TRIOBJ_T;
	}
	else if (o->IsSubClassOf(patchObjectClassID)) {
		return PATCHOBJ_T;
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
	//mprintf(L"Objectwrapper getYpe\n");
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

template <class Message>
inline std::wstring loadPolyObj(PolyObject& ob, Message& obj) {
	std::wstring name;
	auto& mesh = ob.GetMesh();
	std::string s(obj.getName().cStr());
	name = s2ws(s);
	auto meshCap = obj.getMmesh();
	loadMesh(mesh, meshCap);

	return name;
}

inline void setNormalFaceDegree(MNNormalFace & f, int deg) {
	f.SetDegree(deg);
}

inline void setNormalFaceDegree(MeshNormalFace & f, int deg) {
}

inline MNNormalSpec* GetSpecifiedNormals(MNMesh& in) {
	return in.GetSpecifiedNormals();
}

inline MeshNormalSpec* GetSpecifiedNormals(Mesh& in) {
	return (MeshNormalSpec *)in.GetInterface(MESH_NORMAL_SPEC_INTERFACE);
}

inline PatchMesh* GetSpecifiedNormals(ObjectWrapper& in) {
	return in.GetPatchMesh();
}


inline bool isNormalBuilt(MNNormalSpec* normSpec) {
	return (normSpec && normSpec->GetFlag(MNNORMAL_NORMALS_BUILT));
}

inline bool isNormalBuilt(MeshNormalSpec* normSpec) {
	return (normSpec && normSpec->GetFlag(MESH_NORMAL_NORMALS_BUILT));
}

inline void DebugPrint(MNNormalSpec* in, bool printAll = false) {
	in->MNDebugPrint(printAll);
}
inline void DebugPrint(MeshNormalSpec* in, bool printAll = false) {
	//in->MNDebugPrint(printAll);
}

inline int meshFaceType(MNMesh& in) {
	return 1;
}

inline int meshFaceType(MNMap& in) {
	return 1;
}

inline int meshFaceType(MeshMap& in) {
	return 3;
}


inline int meshFaceType(Mesh& in) {
	return 3;
}

inline void clearNormalFlags(MNNormalSpec* m) {
	m->ClearFlag(MNNORMAL_NORMALS_BUILT);
}

inline void clearNormalFlags(MeshNormalSpec* m) {
	m->ClearFlag(MESH_NORMAL_NORMALS_BUILT);
}

template <class M, class O>
inline void loadNormals(M& meshCap, O& mesh) {
	Time t;
	auto mpNormal = meshCap.getMpNormal();
	if (mpNormal.size() > 0) {
		mesh.SpecifyNormals();
		auto* normalSpecOut = GetSpecifiedNormals(mesh);
		if (normalSpecOut) {
			clearNormalFlags(normalSpecOut);
			// Fill in unspecified normals:
			normalSpecOut->SetParent(&mesh);
			normalSpecOut->SpecifyNormals();
			mesh.selLevel = MNM_SL_OBJECT;
			mesh.ClearDispFlag(0x8f);
			normalSpecOut->SetNumNormals(mpNormal.size());
			//mprintf(L"Number of normals is %d\n", mpNormal.size());
			for (int i = 0; i < mpNormal.size(); i++)
			{
				auto item = mpNormal[i];
				loadP(item, normalSpecOut->Normal(i));
				//normalSpecOut->SetNormalExplicit(i, true);
			}

			auto mpFace = meshCap.getMpFace();
			int type = meshFaceType(mesh);
			int numFaces = (int)(mpFace.size() / type);
			if (normalSpecOut->SetNumFaces(numFaces)) {
				//normalSpecOut->SetAllExplicit(true);
				for (int i = 0; i < numFaces; ++i) {
					int idx = i*type;
					auto last_item = mpFace[idx + type - 1];
					auto& f = normalSpecOut->Face(i);

					int deg = (last_item == LONG_MIN) ? 3 : type;
					setNormalFaceDegree(f, deg);

					//mprintf(L"Set degree %d\n", face.size());
					for (int j = 0; j < deg; ++j) {

						//normalSpecOut->SetNormalIndex(i, j, face[j]);
						int val = mpFace[idx + j];
						if (val < 0) {
							f.SetNormalID(j, val);
							f.SetSpecified(j, false);
						}
						else f.SpecifyNormalID(j, val);

						//f.MNDebugPrint();
						//mprintf(L"Face %d:  %d\n", i, val);
					}
				}
			}
			//normalSpecOut->CheckNormals();
			//normalSpecOut->BuildNormals();
			//mesh.buildNormals();
			normalSpecOut->SetParent(NULL);
		}
		else mprintf(L"No normalSpecOut\n");

	}
	else {
		auto* normalSpecOut = GetSpecifiedNormals(mesh);
		if (normalSpecOut) {
			normalSpecOut->SetParent(&mesh);
			normalSpecOut->BuildNormals();
			normalSpecOut->SetParent(NULL);
			//mprintf(L"Building ...... \n");
		}

	}
	time(t, LOAD_NORMALS);
}

inline int numVerts(ObjectWrapper& in) {
	return in.NumVerts();
}


inline int numVerts(MNMesh& in) {
	return in.numv;
}

inline int numVerts(Mesh& in) {
	return in.numVerts;
}

inline int numEdges(ObjectWrapper& in) {
	return in.NumEdges();
}


inline int numEdges(MNMesh& in) {
	return in.nume;
}

inline int numEdges(Mesh& in) {
	return in.numFaces * 3;
}

inline Point3& vert(ObjectWrapper& in, int i) {
	return *in.GetVert(i);
}



inline Point3& vert(MNMesh& in, int i) {
	return in.v[i].p;
}

inline Point3& vert(Mesh& in, int i) {
	return in.verts[i];
}

inline void setUVNumVerts(MNMap& out, int num) {
	out.VAlloc(num, FALSE);
	out.setNumVerts(num);
}

inline void setUVNumVerts(MeshMap& out, int num) {
	out.setNumVerts(num);
}

inline void setUVNumFaces(MNMap& out, int num) {
	out.FAlloc(num, FALSE);
	out.setNumFaces(num);
}

inline void setUVNumFaces(MeshMap& out, int num) {
	out.setNumFaces(num);
	//mprintf(L"UV Map nums = %d\n", out.fnum);
}

template <class ITEM>
inline void copyUVFace(ITEM& item, MNMapFace& fo, int srcIdx = 0, int deg = 3) {
	fo.deg = deg;
	fo.SetSize(deg);
	if (fo.tv)
		for (int j = 0; j < deg; ++j) {
			fo.tv[j] = item[j + srcIdx];
		}
}

template <class ITEM>
inline void copyUVFace(ITEM& item, TVFace& fo, int srcIdx = 0, int deg = 3) {
	for (int j = 0; j < 3; ++j) {
		fo.t[j] = item[j + srcIdx];
	}
}


template <class MNMAP_IN, class MNMAP_OUT>
inline void loadUVW(MNMAP_IN& in, MNMAP_OUT& out) {
	//out.ClearAndFree();
	//out.Init();
	//out.ImportFlags(in.getFlags());

	auto vi = in.getV();

	setUVNumVerts(out, vi.size());
	//mprintf(L"Set num = %d\n", vi.size());
	auto* v = getUVVerts(out);

	if (v) {
		for (int i = 0; i < vi.size(); ++i) {
			auto item = vi[i];
			loadP(item, v[i]);
		}
	}
	else mprintf(L"NULL v\n");

	auto fi = in.getF();
	int type = meshFaceType(out);
	int numFaces = (int)fi.size() / type;

	if (type == 3) {
		setUVNumFaces(out, numFaces);
		auto* f = getUVFaces(out);

		if (f) {
			int idx = 0;
			for (int i = 0; i < numFaces; ++i) {				
				int item = fi[idx + type - 1];				
				auto& fo = f[i];
				/*fo.deg = item.size();
				fo.SetSize(fo.deg);
				if (fo.tv)
				for (int j = 0; j < fo.deg; ++j) {
				fo.tv[j] = item[j];
				}//*/
				copyUVFace(fi, fo, idx, 3);
				idx += 3;
			}
		}
		else mprintf(L"Empty F\n");
	}
	else {		
		std::vector<int> degrees;
		degrees.reserve(numFaces / 3);
		int idx = 0;
		while (idx < numFaces) {			
			int deg = fi[idx++];			
			if (deg < 0) {
				degrees.emplace_back(-deg);
				idx -= deg;
			}
		}

		numFaces = degrees.size();
		setUVNumFaces(out, numFaces);
		auto* f = getUVFaces(out);

		if (f) {
			int idx = 0;
			for (int i = 0; i < numFaces; ++i) {
				idx++;
				int deg = degrees[i];
				int item = fi[idx];
				auto& fo = f[i];
				
				copyUVFace(fi, fo, idx, deg);
				idx += deg;
			}
		}
		else mprintf(L"Empty F\n");

	}
	//*/
}

template <class Obj, class Message>
inline void load(Obj& obj, Message& msg, int options = ExportOpts::all) {
	mprintf(L"Not implemented yet\n");
}



template <class Message>
inline void loadMesh(MNMesh& mesh, Message& meshCap, int options = ExportOpts::all) {
	if (options & ExportOpts::mesh) {
		mprintf(L"Finally here\n");
		auto v = meshCap.getV();

		mesh.setNumVerts(v.size());



		for (int i = 0; i < v.size(); ++i) {
			auto item = v[i];
			auto &p = mesh.v[i];
			loadV(item, p);
			p.ImportFlags(item.getFlags());
		};

		/*auto e = meshCap.getE();
		mesh.setNumEdges(e.size());

		for (int i = 0; i < e.size(); ++i) {
		auto edge = e[i];
		auto& oe = mesh.e[i];
		loadE(edge, oe);
		};*/

		auto f = meshCap.getF();
		mesh.setNumFaces(f.size());
		mprintf(L"Face num read %d     %d\n", f.size(), mesh.numf);

		for (int i = 0; i < f.size(); ++i) {
			auto face = f[i];
			auto& oface = mesh.f[i];
			loadF(face, oface);
		};

		//mesh.ClearFlag(MN_MESH_FILLED_IN);
		mesh.FillInMesh();//calculate edges

		auto edSupport = meshCap.getEdSupport();
		mesh.edSupport.SetSize(edSupport.size());
		loadArr(edSupport, mesh.edSupport);

		auto vdSupport = meshCap.getVdSupport();
		mesh.vdSupport.SetSize(vdSupport.size());
		loadArr(vdSupport, mesh.vdSupport);

		if (vdSupport.size()>0) {
			auto vd = meshCap.getVd();//Per vertex data		
			mesh.setNumVData(vd.size());

			for (int i = 0; i < vd.size(); ++i) {
				if (vdSupport[i]) {
					auto item = vd[i];
					auto & vdo = mesh.vd[i];
					loadPerData(item, vdo);
				}
			}
		}

		if (edSupport.size()>0) {
			auto ed = meshCap.getEd();//Per vertex data		
			mesh.setNumEData(ed.size());

			for (int i = 0; i < ed.size(); ++i) {
				if (edSupport[i]) {
					auto item = ed[i];
					auto & edo = mesh.ed[i];
					loadPerData(item, edo);
				}
			}
		}

	}

	mesh.selLevel = meshCap.getSelLevel();
	mesh.dispFlags = meshCap.getDispFlags();

	if (options & ExportOpts::selection) {
		//Then load Select flags
		/*for (int i = 0; i < f.size(); ++i) {
		auto face = f[i];
		auto& oface = mesh.f[i];
		loadEdgeSel(face, oface);
		};*/


		BitArray sels;
		auto vSel = meshCap.getVSel();
		if (vSel.size() > 0) {
			sels.SetSize(vSel.size());
			loadArr(vSel, sels);
			mesh.VertexSelect(sels);
		}

		auto fSel = meshCap.getFSel();
		if (fSel.size() > 0) {
			sels.SetSize(fSel.size());
			loadArr(fSel, sels);
			mesh.FaceSelect(sels);
		}

		auto eSel = meshCap.getESel();
		if (eSel.size() > 0) {
			sels.SetSize(eSel.size());
			loadArr(eSel, sels);
			//mesh.EdgeSelect(sels);
			for (int i = 0; i < sels.GetSize(); ++i)
				if (sels[i]) mesh.SetEdgeSel(i, 1);//*/
		}
	}


	if (options & ExportOpts::uv) {
		auto m = meshCap.getM();
		mesh.SetMapNum(m.size() + NUM_HIDDENMAPS);

		for (int i = 0; i < m.size(); ++i) {
			auto& item = m[i];
			int channel = item.getChannel();
			auto &mo = *mesh.M(channel);
			loadUVW(item, mo);
		}

		auto hmap = meshCap.getHmap();
		for (int i = 0; i < hmap.size(); ++i) {
			auto item = hmap[i];
			int channel = item.getChannel();
			auto &mo = *mesh.M(channel);
			loadUVW(item, mo);
		}//*/
	}

	if (options & ExportOpts::normals) {
		auto normalSpec = meshCap.getNormals();
		auto mpNormal = normalSpec.getMpNormal();
		if (mpNormal.size() > 0) {
			mesh.SpecifyNormals();
			auto normalSpecOut = mesh.GetSpecifiedNormals();
			if (normalSpecOut) {


				normalSpecOut->SetNumNormals(mpNormal.size());
				for (int i = 0; i < mpNormal.size(); i++)
				{
					auto item = mpNormal[i];
					loadP(item, normalSpecOut->Normal(i));
					//normalSpecOut->SetNormalExplicit(i, true);
				}

				auto mpFace = normalSpec.getMpFace();
				if (normalSpecOut->SetNumFaces(mpFace.size())) {
					//normalSpecOut->SetAllExplicit(true);
					for (int i = 0; i < mpFace.size(); ++i) {
						auto face = mpFace[i];

						for (int j = 0; j < face.size(); ++j) {
							normalSpecOut->SetNormalIndex(i, j, face[j]);
							//mprintf(L"Face %d:  %d\n", i, face[j]);
						}
					}
				}
			}
			else mprintf(L"No normalSpecOut\n");
		}
		//*/
		//printMNMesh(mesh, meshCap);
	}

	if (!mesh.CheckAllData())
		mprintf(L"Something is wrong\n");
	mprintf(L"Got root here\n");
}

template <class Message>
inline void readFaces(MNMesh& mesh, Message& meshCap) {
	auto faces = meshCap.getFaces();
	if (faces.isF3()) {
		//mprintf(L"Reading Mesh faces\n");
		auto f3 = faces.getF3();
		auto size = f3.size();
		//mprintf(L"Num Mesh faces = %d\n", size);
		mesh.setNumFaces(size);
		for (int i = 0; i < size; ++i) {
			auto item = f3[i];
			auto &f = faceRef(mesh, i);
			load3F(item, f);
		}
	}
	else {
		//mprintf(L"Reading MNMesh faces\n");
		auto mnfaces = faces.getMnfaces();
		auto info = mnfaces.getInfo();
		auto size = info.size();
		
		mesh.setNumFaces(size);
		//mprintf(L"Num MNMesh faces = %d\n", size);
		int idx = 0;
		auto data = mnfaces.getF();
		for (int i = 0; i < size; ++i) {
			auto item = info[i];
			auto &f = faceRef(mesh, i);

			f.smGroup = item.getSmGroup();
			f.material = item.getMaterial();
			int deg = item.getDeg();

			f.SetAlloc(deg);
			f.SetDeg(deg);

			for (int j = 0; j < deg; ++j) {
				f.vtx[j] = data[idx++];
			}
			
		}
		//mprintf(L"Finish reading MNMesh faces\n");
	}
}

template <class Message>
inline void readFaces(Mesh& mesh, Message& meshCap) {
	auto faces = meshCap.getFaces();
	if (faces.isF3()) {
		auto f3 = faces.getF3();
		//mprintf(L"Reading Mesh faces\n");
		auto size = f3.size();
		mesh.setNumFaces(size);
		//mprintf(L"Num Mesh faces = %d\n", size);
		for (int i = 0; i < size; ++i) {
			auto item = f3[i];
			auto &f = faceRef(mesh, i);
			load3F(item, f);
			/*if (i < 10) {
			mprintf(L"f %d\t%d\t%d\n", item.getV0(), item.getV1(), item.getV2());
			}*/
		}
	}
	else {
		//mprintf(L"Reading MNMesh faces\n");
		auto mnfaces = faces.getMnfaces();
		auto info = mnfaces.getInfo();
		auto size = info.size();
		//mprintf(L"Num MNMesh faces = %d\n", f4.size());
		mesh.setNumFaces(size);
		int idx = 0;
		auto data = mnfaces.getF();
		for (int i = 0; i < size; ++i) {
			auto item = info[i];
			auto &f = faceRef(mesh, i);

			f.smGroup = item.getSmGroup();
			f.setMatID(item.getMaterial());			

			for (int j = 0; j < 3; ++j) {
				f.v[j] = data[idx++];
			}

		}
	}


}

inline void buildEdges(MNMesh& mesh) {
	Time t;
	mesh.ClearFlag(MN_MESH_FILLED_IN);
	mesh.FillInMesh();
	time(t, BUILD_EDGE);
}

inline void buildEdges(Mesh& mesh) {
	Time t;
	mesh.BuildStripsAndEdges();
	/*for (int i = 0; i < mesh.numFaces; ++i) {
	auto& f = faceRef(mesh, i);
	for (int j = 0; j < 3; ++j)
	f.setEdgeVis(j, EDGE_VIS);
	}*/
	time(t, BUILD_EDGE);
}

inline void SetMapNum(MNMesh& mesh, int num) {
	mesh.SetMapNum(num);
}

inline void SetMapNum(Mesh& mesh, int num) {
	mesh.setNumMaps(num);
}

inline MNMap& getMapChannel(MNMesh& mesh, int channel) {
	return *mesh.M(channel);
}

inline MeshMap& getMapChannel(Mesh& mesh, int channel) {
	return mesh.Map(channel);
}

inline void setMapSupport(MNMesh& m, int channel) {
	m.InitMap(channel);
}

inline void setMapSupport(Mesh& m, int channel) {
	m.setMapSupport(channel, TRUE);
}

static BOOL DEBUG = FALSE;

template <class M, class Message>
inline void readMesh(M& mesh, Message& meshCap, Matrix3& tm, int options = ExportOpts::all) {

	BitArray sels;
	try {
		if (options & ExportOpts::mesh) {
			Time t;
			//mprintf(L"Finally ReadMesh here\n");
			auto nodeTM = Inverse(tm);
			if (DEBUG) {
				mprintf(L"Node TM = \n");
				printTM(tm);
				mprintf(L"Node Inverse TM = \n");
				printTM(nodeTM);
			}
			auto v = meshCap.getV();

			mesh.setNumVerts(v.size());
			//mprintf(L"%d verts have been loaded\n", v.size());

			for (int i = 0; i < v.size(); ++i) {
				auto item = v[i];
				auto &p = vert(mesh, i);
				loadP(item, p);
				if (msc_wk != 1.0)
				{
					p.x *= msc_wk;
					p.y *= msc_wk;
					p.z *= msc_wk;
				}

				if (DEBUG && i<10)
					mprintf(L"(%f, %f, %f) --> ",p.x,p.y,p.z);

				p = nodeTM * p;
				if (DEBUG && i<10)
					mprintf(L"(%f, %f, %f)\n", p.x, p.y, p.z);
				//p.ImportFlags(item.getFlags());
			};
			
			readFaces(mesh, meshCap);

			//mesh.ClearFlag(MN_MESH_FILLED_IN);
			//mesh.FillInMesh();//calculate edges
			buildEdges(mesh);

			auto evis = meshCap.getEVis();
			if (evis.size() > 0) {
				//mprintf(L"Num edge vis = %d\n", evis.size());
				sels.SetSize(evis.size());
				loadArr(evis, sels);
				EdgeVis(mesh, sels);
			}//*/
			time(t, LOAD_MESH);

			//mprintf(L"Num edges = %d\n", numEdges(mesh));
		}

		if (options & ExportOpts::selection) {
			Time t;
			auto vSel = meshCap.getVSel();
			if (vSel.size() > 0) {
				sels.SetSize(vSel.size());
				loadArr(vSel, sels);
				VertexSelect(mesh, sels);
			}

			auto fSel = meshCap.getFSel();
			if (fSel.size() > 0) {
				sels.SetSize(fSel.size());
				loadArr(fSel, sels);
				FaceSel(mesh, sels);
			}

			auto eSel = meshCap.getESel();
			if (eSel.size() > 0) {
				sels.SetSize(eSel.size());
				loadArr(eSel, sels);
				EdgeSel(mesh, sels);

			}
			time(t, LOAD_SELECTION);
		}


		//testEdge(mesh);

		if (options & ExportOpts::uv) {
			Time t;
			auto m = meshCap.getM();
			//auto hmap = meshCap.getHmap();

			//mprintf(L"Total uv map = %d\n", m.size() + hmap.size());
			int maxChannel = -1;
			int num_hidden = 0;
			for (int i = 0; i < m.size(); ++i) {
				auto item = m[i];
				int channel = item.getChannel();
				if (channel < 0) num_hidden++;
				if (maxChannel < channel) maxChannel = channel;
			}

			SetMapNum(mesh, maxChannel + 1 + num_hidden);
			//mprintf(L"Total M map = %d\n", maxChannel + 1 + num_hidden);
			for (int i = 0; i < m.size(); ++i) {
				auto item = m[i];
				int channel = item.getChannel();
				//mprintf(L"Channel %d\n", channel);
				setMapSupport(mesh, channel);
				auto &mo = getMapChannel(mesh, channel);

				loadUVW(item, mo);
			}

			/*mprintf(L"Loading hidden map %d\n", hmap.size());
			for (int i = 0; i < hmap.size(); ++i) {
			auto item = hmap[i];
			int channel = item.getChannel();
			setMapSupport(mesh, channel);
			mprintf(L"Channel %d\n", channel);
			auto &mo = getMapChannel(mesh,channel);
			loadUVW(item, mo);
			}//*/
			time(t, LOAD_UV);
		}

		if (options & ExportOpts::normals) {
			//mprintf(L"Now getting normals\n");
			loadNormals(meshCap, mesh);
			mesh.checkNormals(TRUE);	
		}
	}
	catch (const kj::Exception& e) {
		mprintf(L"--------------------\n");		
		mprintf(L"EXCEPTION %s (Line %d, file %s) \n", s2ws(e.getDescription().cStr()).c_str(), e.getLine(), fromStr(e.getFile(), strlen(e.getFile())));
		auto txt = stringifyStackTrace(e.getStackTrace());
		mprintf(L"%s\n", fromStr(txt.cStr(), txt.size()));

	}

	//*/
	//printMNMesh(mesh, meshCap);
	mesh.selLevel = meshCap.getSelLevel();
	mesh.dispFlags = meshCap.getDispFlags();
	//mprintf(L"Now getting selections\n");
	//checkData(mesh);
}

inline INode* CreateObjectInScene(SClass_ID sid, Class_ID cid) {
	Time t;
	Object* obj = static_cast<Object*>(CreateInstance(sid, cid));
	time(t, CREATE_NODE);
	auto* node = GetCOREInterface()->CreateObjectNode(obj);
	return node;
}

inline DummyObject* createDummyObject() {
	return (DummyObject*)CreateInstance(HELPER_CLASS_ID, Class_ID(DUMMY_CLASS_ID, 0));
}

inline PolyObject* createPolyObject() {
	//return CreateNewPolyObject();
	return static_cast<PolyObject*>(CreateInstance(GEOMOBJECT_CLASS_ID, polyObjectClassID));
}

inline TriObject* createTriObject() {
	return CreateNewTriObject();
	//return static_cast<TriObject*>(CreateInstance(GEOMOBJECT_CLASS_ID, triObjectClassID));
}

inline PatchObject* createPatchObject() {
	return static_cast<PatchObject*>(CreateInstance(GEOMOBJECT_CLASS_ID, patchObjectClassID));
}

inline const char* toStr(const wchar_t* name) {
	Interface14 *iface = GetCOREInterface14();
	UINT codepage = iface->DefaultTextSaveCodePage(true);
	TSTR pName = FixupName(name);
	return (const char*)(pName.ToCP(codepage));

}

inline const char* toStr(std::wstring& name) {
	return toStr(name.c_str());

}

inline TSTR fromStr(const char* str, size_t length) {
	Interface14 *iface = GetCOREInterface14();
	UINT codepage = iface->DefaultTextSaveCodePage(true);
	return TSTR::FromCP(codepage, str, length);
}


inline void getMasterScale(format3d::Anything::Reader& item) {
	int type;
	float scale;
	msc_wk = item.getMasterScale();
	GetMasterUnitInfo(&type, &scale);
	float msc_factor = (float)GetMasterScale(autoConv ? UNITS_INCHES : type);
	msc_wk = (float)((double)msc_wk / msc_factor);		/* Turn into a mult factor */
}

template <class OBJ>
inline void loadObject(OBJ& in, format3d::Object::Builder& msg) {

}

inline TriObject *GetTriObjectFromNode(INode *node, BOOL &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(GetCOREInterface()->GetTime()).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		//mprintf(L"Convert object to TriObject\n");
		TriObject *tri = (TriObject *)obj->ConvertToType(GetCOREInterface()->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));

		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) {
			//mprintf(L"Opppps,  --------------------------------------------> Delete me (GetTriObjectFromNode)\n");
			/*obj->DeleteMe();
			node->SetObjectRef(tri);*/
			deleteIt = TRUE;
		}
		return tri;
	}
	else
	{
		return NULL;
	}
}

inline PolyObject *GetPolyObjectFromNode(INode *node, BOOL &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(GetCOREInterface()->GetTime()).obj;
	if (obj->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0)))
	{
		//	mprintf(L"Convert object to PolyObject\n");
		PolyObject *poly = (PolyObject *)obj->ConvertToType(GetCOREInterface()->GetTime(),
			Class_ID(POLYOBJ_CLASS_ID, 0));

		// Note that the PolyObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != poly) {
			//	mprintf(L"Opppps,  --------------------------------------------> Delete me (GetPolyObjectFromNode)\n");			
			deleteIt = TRUE;
		}
		return poly;
	}
	else
	{
		return NULL;
	}
}

class ObjManipulatorRestore : public RestoreObj {
public:
	BOOL mOnlyForRedo;
	static BOOL mMode;
	ObjManipulatorRestore(BOOL onlyForRedo = FALSE) { mOnlyForRedo = onlyForRedo; }
	void Restore(int isUndo) {
		if (mOnlyForRedo) {
			mMode = GetCOREInterface7()->InManipMode();
			if (mMode) {
				GetCOREInterface7()->EndManipulateMode();
			}
		}
		else {
			if (mMode) {
				mMode = FALSE;
				GetCOREInterface7()->StartManipulateMode();
			}
		}
	}
	void Redo() {
		if (!mOnlyForRedo) {
			mMode = GetCOREInterface7()->InManipMode();
			if (mMode) {
				GetCOREInterface7()->EndManipulateMode();
			}
		}
		else {
			if (mMode) {
				mMode = FALSE;
				GetCOREInterface7()->StartManipulateMode();
			}
		}
	}

	TSTR Description() { return TSTR(_T("Restore Manipulate State")); }
};


class CollapseRestore : public RestoreObj {
public:
	BOOL mOnlyForRedo;
	static BOOL mMode;
	CollapseRestore(BOOL onlyForRedo = FALSE) { mOnlyForRedo = onlyForRedo; }
	void Restore(int isUndo) {
		if (mOnlyForRedo) {
			mMode = GetCOREInterface7()->InManipMode();
			if (mMode) {
				GetCOREInterface7()->EndManipulateMode();
			}
			GetCOREInterface7()->SuspendEditing();
		}
		else {
			if (mMode) {
				mMode = FALSE;
				GetCOREInterface7()->StartManipulateMode();
			}
			GetCOREInterface7()->ResumeEditing();
		}
	}
	void Redo() {
		if (!mOnlyForRedo) {
			mMode = GetCOREInterface7()->InManipMode();
			if (mMode) {
				GetCOREInterface7()->EndManipulateMode();
			}
			GetCOREInterface7()->SuspendEditing();
		}
		else {
			if (mMode) {
				mMode = FALSE;
				GetCOREInterface7()->StartManipulateMode();
			}
			GetCOREInterface7()->ResumeEditing();
		}
	}

	TSTR Description() { return TSTR(_T("Restore Manipulate State")); }
};


inline void Collapse(INode *node)
{
	ICustAttribCollapseManager * iCM = ICustAttribCollapseManager::GetICustAttribCollapseManager();

	Interface7 *ip = GetCOREInterface7();

	Object *oldObj = node->GetObjectRef();
	SClass_ID sc = oldObj->SuperClassID();

	theHold.Begin();
	if (theHold.Holding())
	{
		theHold.Put(new ObjManipulatorRestore());
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
		theHold.Accept(GetString(IDS_COLLAPSE));
	}
}

template <class ObjectType>
inline Class_ID getClassID(ObjectType* obj) {
	//mprintf(L"return triObjectClassID\n");
	return triObjectClassID;
}

template <>
inline Class_ID getClassID(PolyObject* obj) {
	return polyObjectClassID;
}

template <>
inline Class_ID getClassID(PatchObject* obj) {
	return patchObjectClassID;
}



template <class ObjectType>
inline void CollapseToType(INode* node) {
	ICustAttribCollapseManager * iCM = ICustAttribCollapseManager::GetICustAttribCollapseManager();

	Interface7 *ip = GetCOREInterface7();

	Object *oldObj = node->GetObjectRef();
	SClass_ID sc = oldObj->SuperClassID();

	theHold.Begin();
	if (theHold.Holding())
	{
	theHold.Put(new ObjManipulatorRestore());
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
		ObjectType* tobj = nullptr;
		auto classID = getClassID<ObjectType>(tobj);
		if (os.obj->CanConvertToType(classID)) {
			// Convert it to a TriObject and make that the new object
			tobj = (ObjectType*)obj->ConvertToType(ip->GetTime(), classID);
		}
		if (tobj != obj)
			ignoreBaseObjectCAs = false;


		tobj->SetSubSelState(0); // Reset the selection level to object level (in case it happens to have an edit mesh modifier
		oldObj->SetAFlag(A_LOCK_TARGET);
		node->SetObjectRef(tobj);
		//ip->InvalidateObCache(node);
		//node->NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);

		//LAM : added following to handle extension objects 8/2/00
		// Notify all mods and objs in the pipleine, that they have been collapsed
		// NH 14 April 04: Added support for the maintaining CAs on stack collapse
		if (iCM && iCM->GetCustAttribSurviveCollapseState())
		{
			NotifyCollapseMaintainCustAttribEnumProc2 PostNCEP(false, node, ignoreBaseObjectCAs, tobj);
			EnumGeomPipeline(&PostNCEP, oldObj);
		}
		else
		{
			NotifyCollapseEnumProc PostNCEP(false, node, tobj);
			EnumGeomPipeline(&PostNCEP, oldObj);
		}

		oldObj->ClearAFlag(A_LOCK_TARGET);
		oldObj->MaybeAutoDelete();
		if (obj != tobj) obj->MaybeAutoDelete();

		if (theHold.Holding())
		{
		theHold.Put(new CollapseRestore(TRUE));
		theHold.Put(new ObjManipulatorRestore(TRUE));
		}
		theHold.Accept(GetString(IDS_COLLAPSE));
	}
	else {		
		ObjectType* tobj = nullptr;
		auto classID = getClassID<ObjectType>(tobj);
		if (oldObj->CanConvertToType(classID)) {
			// Convert it to a TriObject and make that the new object
			tobj = (ObjectType*)oldObj->ConvertToType(ip->GetTime(), classID);
		}
		if (tobj != oldObj)
			ignoreBaseObjectCAs = false;

		tobj->SetSubSelState(0); // Reset the selection level to object level (in case it happens to have an edit mesh modifier
		oldObj->SetAFlag(A_LOCK_TARGET);
		node->SetObjectRef(tobj);
		//ip->InvalidateObCache(node);
		GetCOREInterface7()->InvalidateObCache(node);
		node->NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);

		if (iCM && iCM->GetCustAttribSurviveCollapseState())
		{
			NotifyCollapseMaintainCustAttribEnumProc2 PostNCEP(false, node, ignoreBaseObjectCAs, tobj);
			EnumGeomPipeline(&PostNCEP, oldObj);
		}
		else
		{
			NotifyCollapseEnumProc PostNCEP(false, node, tobj);
			EnumGeomPipeline(&PostNCEP, oldObj);
		}

		oldObj->ClearAFlag(A_LOCK_TARGET);
		oldObj->MaybeAutoDelete();
		

		if (theHold.Holding())
		{
		theHold.Put(new CollapseRestore(TRUE));
		theHold.Put(new ObjManipulatorRestore(TRUE));
		}
		theHold.Accept(GetString(IDS_COLLAPSE));//		
	}
}

inline INode* createOrConvert(INode* node, int8_t type) {
	Time t;
	auto currentType = getType(node);
	BOOL deleteIt = FALSE;
	//ICustAttribCollapseManager * iCM = ICustAttribCollapseManager::GetICustAttribCollapseManager();
	//mprintf(L"currentType vs type = %d vs %d\n", currentType, type);
	if (type == POLYOBJ_T) {
		if (!node) {
			mprintf(L"Created new PolyObject node\n");
			node = CreateObjectInScene(GEOMOBJECT_CLASS_ID, polyObjectClassID);
			//IGameScene * pIgame = GetIGameInterface();
			//pIgame->InitialiseIGame(node);

		}
		else {
			//GetCOREInterface()->CollapseNode(node, TRUE);			
			if (currentType != type) {
				CollapseToType<PolyObject>(node);
			}
			else Collapse(node);
			//			mprintf(L"Collapsed\n");
		};
		time(t, OBJECT_SCENE_CREATE);

	}
	else if (type == TRIOBJ_T) {
		if (!node) {
			mprintf(L"Created new TriObject node\n");

			node = CreateObjectInScene(GEOMOBJECT_CLASS_ID, triObjectClassID);
			//IGameScene * pIgame = GetIGameInterface();
			//pIgame->InitialiseIGame(node);

		}
		else {
			auto* nobj = getObject(node);
			if (!nobj)
				return nullptr;
			else if (currentType != type) {
				//mprintf(L"CollapseToType<TriObject>   %d vs %d\n",currentType,type);
				CollapseToType<TriObject>(node);
			}
			else {
				//mprintf(L"Collapse  TriObject\n");
				Collapse(node);
			}
			//		mprintf(L"Collapsed TriObject\n");
		}
		time(t, OBJECT_SCENE_CREATE);
	}
	else if (type == DUMMYOBJ_T){
		if (!node) {
			mprintf(L"Created new Dummy node\n");
			auto obj = createDummyObject();
			ImpNode* nd = impip->CreateNode();
			nd->Reference(obj);
			impip->AddNodeToScene(nd);
			node = nd->GetINode();
		}
	} else {
		//mprintf(L"Nan\n");
		return nullptr;
	}


	return node;
}

inline void getMatrix(format3d::Object::Reader& obj,float m[4][3]) {
	auto tm = obj.getTm();
	if (tm.size() != 12) return;	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			m[i][j] = tm[i * 3 + j];
		}
	}	
}


inline Matrix3 getMatrix(format3d::Object::Reader& obj) {
	auto tm = obj.getTm();
	if (tm.size() != 12) return Matrix3(TRUE);
	float m[4][3];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			m[i][j] = tm[i * 3 + j];
		}
	}
	Matrix3 mat(m);
	mat.SetNotIdent();
	if (msc_wk != 1.0)
	{
		mat.SetTrans(mat.GetTrans()*msc_wk);
	}
	return mat;
}

inline void loadParent(format3d::Object::Reader& obj, INode* node) {
	std::string pName = obj.getParent().cStr();
	if (pName.length() > 0) {
		auto* parentNode = ip->GetINodeByName(s2ws(pName.c_str()).c_str());
		auto* prNode = node->GetParentNode();
		if (parentNode && parentNode != node && parentNode != prNode && !parentNode->IsRootNode())
		{
			if (!prNode->IsRootNode()) node->Detach(ip->GetTime());
			mprintf(L"adjust parenting : %s(%s) -> %s\n",parentNode->GetName(), prNode->GetName(),  node->GetName());
			parentNode->AttachChild(node, 1);			
		}
	}
}


inline void importGroup(uint8_t type, int idx, INode* node, NodeTabTab* groupNodes = nullptr, GroupNames* grInfo = nullptr) {
	if (type == NO_GROUP || node == nullptr)	return;

	if (type == GROUP_HEAD || type == GROUP_OPEN_HEAD) {
		TSTR groupName(node->GetName());
		//mprintf(L"Group name = %s\n", groupName);
		/*if (groupNodes) {
			INodeTab tab;			
			groupNodes->Append(1, &tab, 10);
			mprintf(L"Appended group head\n");
		}*/
		if (type == GROUP_HEAD)
			node->SetGroupHead(true);
		else node->SetGroupHeadOpen(true);
	}
	else if (type == GROUP_MEMBER || type == GROUP_OPEN_MEMBER) {
		//node->SetGroupMember(true);	
		node->SetGroupMember(false);				
		if (!node->GetParentNode()->IsRootNode()) {
			node->Detach(ip->GetTime());
			INodeTab tab;
			tab.AppendNode(node, true);
			ip->DetachNodesFromGroup(tab);
		}

		if (idx >=0 && grInfo) {
			TSTR groupName = grInfo->at(idx);			
			if (groupNodes) {
				if (idx >= groupNodes->Count()) {					
					for (int i = groupNodes->Count(); i <= idx; ++i) {
						INodeTab tab;
						groupNodes->Append(1, &tab, 10);
					}
					//mprintf(L"Resize groupNodes\n");
				}
				//mprintf(L"Apppending member %s(%d) to group %s\n", node->GetName(), idx, groupName);
				auto& tab = (*groupNodes)[idx];				
				tab.Append(1, &node, 10);

				//mprintf(L"Size of group (%d) is now %d\n",idx, (*groupNodes)[idx].Count());
			}
		}		
	}
}

inline void importGroup(format3d::Object::Reader& obj, INode* node, NodeTabTab* groupNodes = nullptr, GroupNames* grInfo = nullptr) {
	uint8_t type = obj.getGroupType();
	if (type == NO_GROUP || !node)	return;

	if (type == GROUP_HEAD || type == GROUP_OPEN_HEAD) {
		TSTR groupName(node->GetName());		
		/*if (groupNodes) {
			INodeTab tab;
			tab.Append(1, &node, 10);
			groupNodes->Append(1,&tab,10);
		}*/

		/*if (groupNodes) {
			INodeTab tab;
			groupNodes->Append(1, &tab, 10);
			mprintf(L"Appended group head\n");
		}*/

		if (type==GROUP_HEAD)
			node->SetGroupHead(true);
		else node->SetGroupHeadOpen(true);
	}
	else if (type == GROUP_MEMBER || type == GROUP_OPEN_MEMBER){		
		//if (!node->IsGroupMember()) {
		//}
		//node->SetGroupMember(false);	
		auto* pr = node->GetParentNode();
		/*if (!pr->IsRootNode()) {
			node->Detach(ip->GetTime());
			INodeTab tab;
			tab.AppendNode(node, true);
			ip->DetachNodesFromGroup(tab);
		}
		/*if (!pr->IsRootNode())
			node->GetParentNode()*/

		auto gr = obj.getGroup();
		TSTR groupName;
		if (gr.isIndex() && grInfo) {
			auto idx = gr.getIndex();
			if (groupNodes) {
				groupName = grInfo->at(idx);				

				auto* eHead = ip->GetINodeByName(groupName);
				if (eHead && eHead != pr) {//Attach to existing group
					mprintf(L"UIda %d  %s  --> %s(%s)\n",idx, node->GetName(), groupName, pr->GetName());
					if (!pr->IsRootNode()) {		
						if (node->IsGroupMember()) {
							INodeTab tab;
							tab.AppendNode(node, true);
							ip->DetachNodesFromGroup(tab);
						} else node->Detach(ip->GetTime());
					}

					if (idx >= groupNodes->Count()) {
						for (int i = groupNodes->Count(); i <= idx; ++i) {
							INodeTab tab;
							groupNodes->Append(1, &tab, 10);
						}
						//mprintf(L"Resize groupNodes\n");
					}
					mprintf(L"Appending member %s(%d) to group %s(%s)\n", node->GetName(), idx, groupName, pr->GetName());
					auto& tab = (*groupNodes)[idx];
					tab.Append(1, &node, 10); //Only insert if there is no duplication
				}
			}
		}
		else if (gr.isGrName()) {
			mprintf(L"Unexpected here\n");
			auto name = gr.getGrName();
			groupName =  fromStr(name.cStr(),name.size());
			if (grInfo && groupNodes) {
				auto it = std::find(grInfo->begin(), grInfo->end(), groupName);
				if (it != grInfo->end()) {
					auto idx = it - grInfo->begin();
					if (idx >= groupNodes->Count()) {						
						for (int i = groupNodes->Count(); i <= idx; ++i) {
							INodeTab tab;
							groupNodes->Append(1, &tab, 10);
						}

					}
					auto& tab = (*groupNodes)[idx];				
					tab.Append(1, &node, 10); //Only insert if there is no duplication
				}
			}
			else {
				auto* eHead = ip->GetINodeByName(groupName);
				if (eHead) {//Attach to existing group
					INodeTab tab;
					tab.Append(1, &node, 1);
					ip->AttachNodesToGroup(tab, *eHead);
				}
			}

		}
	}
	
}

inline Point3
GetPivotOffset(INode* node)
{
	// get the pivot offset and remove the rotational/scale component
	Matrix3 mat(TRUE);
	Quat qRot = node->GetObjOffsetRot();
	qRot.MakeMatrix(mat);
	// max scales the object if the pivot is scaled so skip.
	// m.SetScale( ((SceneEntry *)data)->node->GetObjOffsetScale().s );
	Point3 pivot = -node->GetObjOffsetPos();
	mat = Inverse(mat);
	Point3 pOff = VectorTransform(mat, pivot);
	return pOff;
}

inline void
GetPivot(format3d::Object::Reader& obj,float pivot[10]) {
	auto p = obj.getPivot();
	for (int i = 0; i < 10; ++i)
		pivot[i] = p[i];	
}


inline void
SetPivot(INode* node, float pivot[10]) {
	Point3 p(pivot[0], pivot[1], pivot[2]);
	Quat rot(pivot[3], pivot[4], pivot[5], pivot[6]);
	ScaleValue s(Point3(pivot[7], pivot[8], pivot[9]));
	node->SetObjOffsetPos(p);
	node->SetObjOffsetRot(rot);
	node->SetObjOffsetScale(s);
}



inline void displayNode(INode* node) {
	if (!node) return;
	auto* pr = node->GetParentNode();
	mprintf(L"-------------------\n");
	mprintf(L"Node name =%s (%s)\n",node->GetName(),pr->GetName());
	mprintf(L"Node TM = [\n");
	auto tm = node->GetNodeTM(ip->GetTime());	
	printTM(tm);
	mprintf(L"]\n");
	auto p = GetPivotOffset(node);
	mprintf(L"Pivot = (%f, %f, %f)\n",p.x,p.y,p.z);
	auto* obj = getObject(node);
	ObjectWrapper ow;
	auto os = node->EvalWorldState(0);
	ow.Init(0, os );
	mprintf(L"Num vertices = %d\n", ow.NumVerts());
	auto* pts = ow.Verts();
	auto num = std::min(10, ow.NumVerts());
	for (int i = 0; i < num; ++i) {
		auto p = pts[i];
		mprintf(L"(%f, %f, %f)\t", p.x, p.y, p.z);
	}
	mprintf(L"\n");
	
	if (pr && !pr->IsRootNode()) {
		mprintf(L"Parent name = %s\n", pr->GetName());
		auto tm = pr->GetNodeTM(0);
		MRow* mm = tm.GetAddr();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 3; ++j) {
				mprintf(L"%f\t", mm[i][j]);
			}
			mprintf(L"\n");
		}
		mprintf(L"]\n");
	}

}

inline Object* loadObj(format3d::Object::Reader& obj, INode* node = nullptr, NodeTabTab* groupNodes = nullptr, GroupNames* grInfo = nullptr, int options = ExportOpts::all) {
	Time t;
	auto type = obj.getType();
	auto tm = getMatrix(obj);	
	auto* newNode = createOrConvert(node, type);
	if (newNode == nullptr) return nullptr;

	importGroup(obj,newNode, groupNodes,grInfo);
	TSTR groupName(newNode->GetParentNode()->GetName());	
	

	if (type == POLYOBJ_T) {
		PolyObject* nobj = (PolyObject*)getObject(newNode);
		//nobj->SetAFlag(A_LOCK_TARGET);
		auto name = obj.getName();
		TSTR nodeName(fromStr(name.cStr(), name.size()));		
		newNode->SetName(nodeName.data());
		loadParent(obj, newNode);
		auto meshCap = obj.getMesh();
		if (nodeName == L"obj_126") {
			mprintf(L"Before\n");
			//displayNode(newNode);
			DEBUG = true;
		}
		else DEBUG = false;

		readMesh(nobj->GetMesh(), meshCap, tm, options);
		
		loadParent(obj, newNode);
		newNode->SetNodeTM(0, tm);

		auto iFlags = obj.getIFlags();
		Control* tmCont = newNode->GetTMController();
		if (tmCont)
		{
			tmCont->SetInheritanceFlags(~iFlags, TRUE);
		}

		//if (nodeName == L"obj_126") {
		//	mprintf(L"After\n");
		//	//displayNode(newNode);
		//	DEBUG = true;
		//} else DEBUG = false;
		
		//nobj->ClearAFlag(A_LOCK_TARGET);		
		nobj->GetMesh().InvalidateTopoCache();
		nobj->GetMesh().InvalidateGeomCache();
		nobj->GetMesh().buildNormals();
		time(t, OBJECT_CREATION);
		return nobj;
	}
	else if (type == TRIOBJ_T) {
		TriObject* nobj = (TriObject*)getObject(newNode);
		//nobj->SetAFlag(A_LOCK_TARGET);
		auto name = obj.getName();
		TSTR nodeName(fromStr(name.cStr(),name.size()));
		//mprintf(L"Loading Tri Object %s\n", name.c_str());



		if (nodeName == L"obj_126") {
			mprintf(L"------------------------\n");
			mprintf(L"\nBefore\n");
			DEBUG = true;
			displayNode(newNode);
		} else DEBUG = false;

		newNode->SetName(nodeName.data());
		loadParent(obj, newNode);
		auto meshCap = obj.getMesh();
		readMesh(nobj->GetMesh(), meshCap, tm, options);
		
		newNode->SetNodeTM(0, tm);

		auto iFlags = obj.getIFlags();
		Control* tmCont = newNode->GetTMController();
		if (tmCont)
		{
			tmCont->SetInheritanceFlags(~iFlags, TRUE);
		}
		
		//nobj->ClearAFlag(A_LOCK_TARGET);
		//nobj->GetMesh().InvalidateTopoCache();

		if (nodeName == L"obj_126") {
			mprintf(L"After\n");
			displayNode(newNode);
		}
		nobj->GetMesh().InvalidateGeomCache();
		nobj->GetMesh().buildNormals();
		time(t, OBJECT_CREATION);
		return nobj;
	}
	else if (type == PATCHOBJ_T) {
	}//*/
	auto name = obj.getName();
	TSTR nodeName(fromStr(name.cStr(), name.size()));
	if (nodeName == L"obj_126") {
		mprintf(L"After\n");
		displayNode(newNode);
		DEBUG = true;
	}
	else DEBUG = false;
	loadParent(obj, newNode);
	newNode->SetNodeTM(GetCOREInterface()->GetTime(), tm);
	auto iFlags = obj.getIFlags();
	Control* tmCont = newNode->GetTMController();
	if (tmCont)
	{
		tmCont->SetInheritanceFlags(~iFlags, TRUE);
	}
	if (nodeName == L"obj_126") {
		mprintf(L"After\n");
		//displayNode(newNode);
	}
	//loadParent(obj, newNode);

	
	time(t, OBJECT_CREATION);
	return nullptr;
}


inline Object* load2(capnp::FlatArrayMessageReader& m) {
	mprintf(L"Test loading file\n");
	auto root = m.getRoot<format3d::Objects>();
	auto objs = root.getObjs();
	mprintf(L"Number of objects is %d\n", objs.size());
	std::vector<int> vv;
	std::vector<float> ff;
	for (int i = 0; i < objs.size(); ++i) {
		auto obj = objs[i];
		if (i == 499) {
			std::string s(obj.getName().cStr());
			mprintf(L"Object name is %s\n", s2ws(s).c_str());
		}
		auto mesh = obj.getMesh();
		auto f = mesh.getFaces();
		if (f.isF3()) {
			auto f3 = f.getF3();
			for (int j = 0; j < f3.size(); ++j) {
				auto face = f3[j];
				int x = face.getV0();
				int y = face.getV1();
				int z = face.getV2();
				int smGroup = face.getSmGroup();
				int mtl = face.getMaterial();
				vv.emplace_back(x);
				vv.emplace_back(y);
				vv.emplace_back(z);
				vv.emplace_back(smGroup);
				vv.emplace_back(mtl);
			}
		}

		auto m = mesh.getM();
		for (int i = 0; i < m.size(); ++i) {
			auto mn = m[i];
			auto tv = mn.getV();
			auto tf = mn.getF();
			for (int j = 0; j < tv.size(); ++j) {
				auto v = tv[j];
				auto x = v.getX();
				auto y = v.getY();
				auto z = v.getZ();
				ff.emplace_back(x);
				ff.emplace_back(y);
				ff.emplace_back(z);
			}

			for (int j = 0; j < tf.size(); ++j) {
				auto idx = tf[j];
				vv.emplace_back(idx);
			}
		}

		auto mpNormals = mesh.getMpNormal();
		for (int i = 0; i < mpNormals.size(); ++i) {
			auto p = mpNormals[i];
			auto x = p.getX();
			auto y = p.getY();
			auto z = p.getZ();
			ff.emplace_back(x);
			ff.emplace_back(y);
			ff.emplace_back(z);
		}
	}

	mprintf(L"Number of integers = %d\n", vv.size());
	mprintf(L"Number of floats = %d\n", ff.size());
	return nullptr;
}

template <class Message, class Container>
inline void* load(Message& msg, Container* node = nullptr, int options = ExportOpts::all) {
}

template <class List>
inline int search(INode& item, List& li) {
	std::wstring s(item.GetName());
	//mprintf(L"Searching for %s\n", item.GetName());
	//std::string s(ws2s(item.GetName()));
	for (int i = 0; i < li.size(); ++i) {
		auto item = li[i];
		//mprintf(L"Inspect item %s\n", s2ws(item.getName().cStr()).c_str());
		std::wstring tmp(s2ws(item.getName().cStr()));
		//mprintf(L"%d == %d ?  %s  vs  %s\n", s.size(), tmp.size(),s,tmp);
		if (s == tmp) {
			//mprintf(L"Found at index %d\n", i);
			return i;
		}
	}
	return -1;
}

template <class Message>
inline void* load(Message& msg, INode* node = nullptr, int options = ExportOpts::all) {
	auto message = msg.getRoot<format3d::Anything>();
	getMasterScale(message);
	GroupNames groupInfo;
	importGroupInfo(message, groupInfo);
	NodeTabTab groupNodes;
	auto items = message.getItems();
	if (items.isObj()) {
		auto obj = items.getObj();
		return loadObj(obj, node, &groupNodes, &groupInfo, options);
	}
	else if (items.isObjs()) {
		auto objs = items.getObjs();
		if (node == nullptr)
			return (objs.size()>0) ? loadObj(objs[0], node, &groupNodes,&groupInfo, options) : nullptr;
		if (objs.size() == 0) return nullptr;
		//mprintf(L"List size = %d\n", objs.size());
		int idx = search(*node, objs);
		return (idx != -1) ? loadObj(objs[idx], node, &groupNodes, &groupInfo, options) : loadObj(objs[0], node, &groupNodes, &groupInfo, options);
	}

	return		nullptr;

}

template <class List>
inline void getObjectNames(List& li, std::map<std::wstring, int>& out) {

	for (int i = 0; i < li.size(); ++i) {
		auto item = li[i];
		std::wstring tmp(s2ws(item.getName().cStr()));
		out[tmp] = i;
	}
}

template <class Message>
inline void* load(Message& msg, INodeTab* nodes = nullptr, int options = ExportOpts::all) {
	auto message = msg.getRoot<format3d::Anything>();
	getMasterScale(message);
	auto items = message.getItems();
	GroupNames groupInfo;
	importGroupInfo(message, groupInfo);
	NodeTabTab groupNodes;
	mprintf(L"loading .... \n");
	if (nodes == nullptr) {
		INodeTab lNodes;
		GetSceneNodes(lNodes, nullptr);
		if (lNodes.Count() == 0) return nullptr;
		return load(msg, &lNodes, options);
	}

	auto& nodesRef = *nodes;

	if (items.isObj()) {
		auto obj = items.getObj();
		if (nodes == nullptr || nodes->Count() == 0)
			return loadObj(obj, nullptr, &groupNodes,&groupInfo, options);
		return loadObj(obj, nodesRef[0], &groupNodes, &groupInfo, options);


	}
	else if (items.isObjs()) {
		auto objs = items.getObjs();
		mprintf(L"Loading multiple objects %d  %d\n", objs.size(), nodes->Count());
		if (objs.size() == 0 || nodes->Count() == 0) return nullptr;
		std::map<std::wstring, int> mapNames;
		getObjectNames(objs, mapNames);
		for (int i = 0; i < nodes->Count(); ++i) {
			auto* node = nodesRef[i];
			std::wstring name(node->GetName());
			mprintf(L"%s\t%d\n", name.c_str(), mapNames[name]);
			if (mapNames.find(name) != mapNames.end()) {
				mprintf(L"Importing %s\n", name.c_str());
				int idx = mapNames[name];
				auto obj = objs[idx];
				loadObj(obj, node, &groupNodes, &groupInfo, options);
			}
		}

	}

	return		nullptr;

}


template <class List, class Index>
inline void loadIndices2(List& objs, Index& idx, int options = ExportOpts::all) {
	auto* ip = GetCOREInterface();	
	TSTR name;
	TSTR prName;

	for (int i = 0; i < idx.size(); ++i) {
		auto ix = idx[i];
		auto obj = objs[ix];

		//mprintf(L"Index %d\n", ix);
		auto nm = obj.getName();
		auto parent = obj.getParent();
		name = fromStr(nm.cStr(), nm.size());
		prName = fromStr(parent.cStr(), parent.size());


		//MSTR s(s2ws(obj.getName().cStr()).c_str() );
		auto type = obj.getType();
		auto* node = GetCOREInterface()->GetINodeByName(name.data());

		if (node != nullptr) {
			//mprintf(L"here %s\n", name.data());
			loadObj(obj, node, options);
			//mapNames[name] = 0;
		}
		else {
			
				auto tm = getMatrix(obj);				
				auto iFlags = obj.getIFlags();				
				//mprintf(L"Adding %s as parent of %s   (%d  =  %d,  %d)\n", prName.data(), name.data(),strlen(nm.cStr()),name.length(),nm.size() );
				ImpNode* node = impip->CreateNode();
				
				if (type == DUMMYOBJ_T) {
					auto* o = createDummyObject();
					node->Reference(o);
				}
				else if (type == POLYOBJ_T) {
					auto meshCap = obj.getMesh();
					PolyObject* o = createPolyObject();
					readMesh(o->GetMesh(), meshCap, tm, options);
					node->Reference(o);
				}
				else {
					auto meshCap = obj.getMesh();
					TriObject* o = createTriObject();
					readMesh(o->GetMesh(), meshCap, tm, options);
					node->Reference(o);
				}

				impip->AddNodeToScene(node);
				INode* realINode = node->GetINode();
				//ip->DeSelectNode(realINode);				

				node->SetName(name,false);
				if (prName.length() > 0) {
					auto* parentNode = GetCOREInterface()->GetINodeByName(prName.data());
					if (parentNode && parentNode != realINode)
					{
						parentNode->AttachChild(realINode, 1);
					}
				}

				node->SetTransform(0, tm);
				Control* tmCont = realINode->GetTMController();
				if (tmCont)
				{
					tmCont->SetInheritanceFlags(~iFlags, TRUE);
				}
				
		}
	}	
}

//inline INode* GetINodeByName(MCHAR* name) {
//	auto* node = ip->GetINodeByName(name);
//	if (node && wcscmp(node->GetName(), name) == 0) return node;
//}

template <class List, class Index>
inline void loadIndices(List& objs, Index& idx, NodeTabTab* groupNodes = nullptr, GroupNames* grInfo = nullptr, int options = ExportOpts::all) {

	std::vector< std::pair<uint8_t, int> > groupData;
	struct MeshItem{
		TSTR name;
		TSTR parent;
		Matrix3 tm;
		float piv[10];
		DWORD iflag;
		Object* obj;
		ImpNode* node;
		INode* realNode;

		MeshItem(TSTR& _name, TSTR& _pr, float(*_tm)[3], float* p, DWORD _iflag, Object* _obj = nullptr, ImpNode* _node = nullptr, INode* _realNode = nullptr) :name(_name), parent(_pr), tm(_tm), iflag(_iflag), obj(_obj), node(_node), realNode(_realNode) {
			memcpy(piv, p, 10 * sizeof(float));
		};
		MeshItem(TSTR& _name, TSTR& _pr, Matrix3& _tm, float* p, DWORD _iflag, Object* _obj = nullptr, ImpNode* _node = nullptr, INode* _realNode = nullptr) :name(_name), parent(_pr), tm(_tm), iflag(_iflag), obj(_obj), node(_node), realNode(_realNode) {
			memcpy(piv, p, 10 * sizeof(float));
		};
	};

	auto* ip = GetCOREInterface();
	Interface14 *iface = GetCOREInterface14();
	UINT codepage = iface->DefaultTextSaveCodePage(true);		
	std::map<TSTR, int> mapNames;	
	std::vector<MeshItem> v;
	v.reserve(idx.size());
	groupData.reserve(idx.size());	

	TSTR name;
	TSTR prName;
	float m[4][3];

	Time t;
	for (int i = 0; i < idx.size(); ++i) {		
		auto ix = idx[i];
		auto obj = objs[ix];
		uint8_t groupTp = obj.getGroupType();
		

		//mprintf(L"Index %d\n", ix);
		auto nm = obj.getName();
		auto parent = obj.getParent();


		name = TSTR::FromCP(codepage,nm.cStr(),nm.size() );
			
		auto type = obj.getType();
		//auto handle = obj.getHandle();
		//auto* node = ip->GetINodeByHandle(handle);
		auto* node = ip->GetINodeByName(name.data());
		
		if (node != nullptr) {
			//mprintf(L"here %s\n",name.data() );							
			loadObj(obj, node, groupNodes, grInfo, options);
		}
		else {
			//BOOL found = mapNames.find(name) != mapNames.end();
			//if (!found)
			{				
				TSTR groupName(TSTR::FromCP(codepage, parent.cStr(), parent.size()));
				if (groupTp != NO_GROUP) {
					auto group = obj.getGroup();
					if (group.isIndex()) {
						int index = group.getIndex();
						groupData.emplace_back(groupTp, index);						
					}
					else if (group.isGrName() && grInfo) {
						auto grName = group.getGrName();
						TSTR grN(fromStr(grName.cStr(), grName.size()));
						auto it = std::find(grInfo->begin(), grInfo->end(), grN);
						if (it != grInfo->end()) {
							groupData.emplace_back(groupTp, it - grInfo->begin());							
						}
						else 
							groupData.emplace_back(groupTp, -1);													
					}
					else {
						groupData.emplace_back(groupTp, -1);						
					}
				} else groupData.emplace_back(NO_GROUP, -1);

				float pivot[10];
				GetPivot(obj,pivot);
				
				TSTR nodename(name.data());
				if (nodename == L"obj_126") {
					DEBUG = true;
					//displayNode(newNode);					
				}
				else DEBUG = false;

				//auto tm = getMatrix(obj);			
				//tms.emplace_back(tm);
				
				Matrix3 tm = getMatrix(obj);
				//prNames.push_back(prName);						
				auto iFlags = obj.getIFlags();			
				mapNames[name] = v.size();
				//mprintf(L"Adding %s as parent of %s   (%d  =  %d,  %d)\n", prName.data(), name.data(),strlen(nm.cStr()),name.length(),nm.size() );
				Object* object = nullptr;
				if (type == DUMMYOBJ_T) {
					auto* o = createDummyObject();					
					object = o;
				} else if (type == POLYOBJ_T) {
					auto meshCap = obj.getMesh();
					PolyObject* o = createPolyObject();
					readMesh(o->GetMesh(), meshCap, tm, options);				
					object = o;
				}
				else {
					auto meshCap = obj.getMesh();
					TriObject* o = createTriObject();
					readMesh(o->GetMesh(), meshCap, tm, options);										
					object = o;
				}
				v.emplace_back(name, TSTR::FromCP(codepage,parent.cStr(), parent.size()), tm, pivot, iFlags, object);				
			}			
		}
		
	}

	/*std::sort(mapNames.begin(), mapNames.end(), [](const NodePair& a, const NodePair& b	) {
		return a.first < b.first;
	});*/

	mprintf(L"First pass %d\n", t.duration());
	
	//size_t size = list.size();
	//tbb::parallel_for(tbb::blocked_range<size_t>(0, size), [&list,&ip](const tbb::blocked_range<size_t>& r) {
	//	for (size_t i = r.begin(); i < r.end(); ++i) {

	

#ifndef USE_IMPNODES

	for (int i = 0; i<v.size(); ++i) {
		auto& item = v[i];
		if (item.obj == nullptr) continue;
		INode* node = ip->CreateObjectNode(item.obj);		
		item.node = nullptr;
		item.realNode = node;
		node->SetName(item.name.data());
	}

	for (int i = 0; i<v.size(); ++i) {
		auto& item = v[i];
		INode* node = item.realNode;		
		auto& prName = item.parent;

		if (prName.length() > 0) {
			auto* prNode = ip->GetINodeByName(prName.data());
			if (prNode && prNode != node) {
				prNode->AttachChild(node, 1);
			}
			else {
				auto iter = mapNames.find(prName);
				if (iter != mapNames.end()) {
					auto idx = iter->second;
					auto& it = v[idx];
					auto* pNode = it.realNode;
					if (pNode && pNode != node) {
						pNode->AttachChild(node, 1);
					}
				}
			}
		}
	}//*/

	for (int i = 0; i < v.size(); ++i) {
		auto& item = v[i];
		auto* node = item.realNode;				
		node->SetNodeTM(0, item.tm);
		Control* tmCont = node->GetTMController();
		if (tmCont)
			tmCont->SetInheritanceFlags(~item.iflag, TRUE);
		
	}

#else	

	/*if (grInfo) {
		for (int i = 0; i < grInfo->size(); ++i) {
			mprintf(L"grInfo[%d] = %s\n", i,grInfo->at(i));
		}
	}
	mprintf(L"Size %d = %d\n", v.size(), groupData.size());	*/

	for (int i = 0; i<v.size(); ++i) {
		auto& item = v[i];		
		if (item.obj == nullptr) continue;
		ImpNode* impNode = impip->CreateNode();
		impNode->Reference(item.obj);
		INode* node = impNode->GetINode();													
		item.node = impNode;
		item.realNode = node;				
		
	}
	
	for (int i = 0; i<v.size(); ++i) {
		auto& item = v[i];
		INode* node = item.realNode;
		ImpNode* impNode = item.node;
		auto& prName = item.parent;		
		
		if (prName.length() > 0) {
			auto* prNode = GetCOREInterface()->GetINodeByName(prName.data());
			if (prNode && prNode != node) {
				prNode->AttachChild(node,1);
			}
			else {
				auto iter = mapNames.find(prName);
				if (iter != mapNames.end()) {
					auto idx = iter->second;
					auto& it = v[idx];
					auto* pNode = it.realNode;
					if (pNode && pNode != node) {					
						pNode->AttachChild(node, 1);
					}
				}
			}
		}					
	}

	for (int i = 0; i < v.size(); ++i) {
		auto& item = v[i];
		auto* node = item.realNode;
		auto* impNode = item.node;	
		impNode->SetName(item.name.data(), false);
		SetPivot(node, item.piv);
		//impNode->SetPivot(item.piv);
		impNode->SetTransform(0, item.tm);
		Control* tmCont = node->GetTMController();
		if (tmCont)		
			tmCont->SetInheritanceFlags(~item.iflag, TRUE);		
		auto& data = groupData[i];
		//mprintf(L"GroupData[%d] = %d, %d\n", i, data.first, data.second);		
		
		impip->AddNodeToScene(impNode);		
		
		TSTR nodename = node->GetName();
		/*if (nodename == L"obj_126") {
			displayNode(node);
		}//*/
		
		if (data.first != NO_GROUP && data.second != -1)
			importGroup(data.first, data.second, node, groupNodes, grInfo);
	}
	

	if (groupNodes && grInfo) {
		for (int i = 0; i < groupNodes->Count(); ++i) {
			auto& tab = (*groupNodes)[i];
			TSTR groupName = grInfo->at(i);
			
			/*mprintf(L"Group %d (%s) :\n", i, groupName);
			for (int j = 0; j < tab.Count(); ++j) {
				auto* pr = tab[j]->GetParentNode();
				mprintf(L"%s (%s) \t", tab[j]->GetName(), pr->IsRootNode() ? L"Root" : pr->GetName() );
			}
						
			mprintf(L"\n");*/
			if (tab.Count() > 0) {
				auto iter = mapNames.find(groupName);
				if (iter != mapNames.end()) {
					auto idx = iter->second;
					auto& it = v[idx];
					auto* head = it.realNode;
					if (head) {
						/*mprintf(L"Head name = %s(%d, %d)\n", head->GetName(),head->IsGroupHead(),head->IsGroupMember());
						mprintf(L"Found group %s  (num elems = %d)\n", groupName.data(), tab.Count());*/
						if (!ip->AttachNodesToGroup(tab, *head)) {
							mprintf(L"Failed to attach nodes\n");
							MSTR grn(groupName.data());							
							it.realNode = ip->GroupNodes(&tab, &grn, 0);
						}
						
					}
				}
				else {
					auto* head = ip->GetINodeByName(groupName.data());

					if (head) {
						//mprintf(L"Found existing group %s  (num elems = %d)\n", groupName,tab.Count());
						ip->AttachNodesToGroup(tab, *head);
							//mprintf(L"Failed to attach nodes 2\n");
					} else mprintf(L"NOT Found group %s\n", groupName);
				}				
			}
		}
	}

#endif
	mprintf(L"Create object & set names = %d\n", t.duration());

}

template <class Message>
inline void* load(Message& msg, std::vector<std::wstring>* names = nullptr, int options = ExportOpts::all) {
	auto message = msg.getRoot<format3d::Anything>();
	getMasterScale(message);
	GroupNames groupInfo;
	importGroupInfo(message, groupInfo);
	NodeTabTab groupNodes;
	auto items = message.getItems();
	mprintf(L"loading .... \n");

	if (items.isObj()) {
		auto obj = items.getObj();
		std::wstring name(s2ws(obj.getName().cStr()));
		auto* node = GetCOREInterface()->GetINodeByName(name.c_str());
		return loadObj(obj, node, &groupNodes, &groupInfo, options);
	}
	else if (items.isObjs()) {
		auto objs = items.getObjs();
		//mprintf(L"Loading multiple objects %d  %d\n", objs.size(), names->size());

		std::map<std::wstring, int> mapNames;
		getObjectNames(objs, mapNames);
		std::vector<int> index;
		index.reserve(mapNames.size());
		if (names && names->size() > 0) {
			for (int i = 0; i < names->size(); ++i) {
				auto& name = (*names)[i];
				if (mapNames.find(name) != mapNames.end()) {
					auto idx = mapNames[name];
					index.emplace_back(idx);
				}
			}
			std::sort(index.begin(), index.end());
		}
		else {
			for (int i = 0; i < objs.size(); ++i) {
				index.emplace_back(i);
			};
		};

		
		loadIndices(objs, index, &groupNodes, &groupInfo, options);

		/*for (int i = 0; i < index.size(); ++i) {
		auto idx = index[i];
		auto obj = objs[idx];
		std::wstring name(s2ws(obj.getName().cStr()));
		auto* node = GetCOREInterface()->GetINodeByName(name.c_str());
		loadObj(obj, node, options);
		}
		*/
	}

	return		nullptr;

}

template <class Message,class GroupList>
inline void importGroupInfo(Message & msg, GroupList& gr) {
	auto grInfo = msg.getGroupInfo();

	gr.reserve(grInfo.size());
	//mprintf(L"grInfo.size() = %d\n", grInfo.size());

	Interface14 *iface = GetCOREInterface14();
	UINT codepage = iface->DefaultTextSaveCodePage(true);	

	for (int i = 0; i < grInfo.size(); ++i) {
		auto item = grInfo[i];
		gr.emplace_back(TSTR::FromCP(codepage, item.cStr(), item.size()) );
		//mprintf(L"Load group name %s\n", gr[i]);
	}

}

template <class Message>
inline void* load(Message& msg, std::vector<int>* index = nullptr, int options = ExportOpts::all) {
	auto message = msg.getRoot<format3d::Anything>();
	getMasterScale(message);
	GroupNames groupInfo;
	importGroupInfo(message, groupInfo);
	NodeTabTab groupNodes;
	auto items = message.getItems();
	mprintf(L"loading .... \n");

	

	auto* ip = GetCOREInterface();

	if (items.isObj()) {
		auto obj = items.getObj();
		std::wstring name(s2ws(obj.getName().cStr()));
		auto* node = ip->GetINodeByName(name.c_str());
		return loadObj(obj, node, &groupNodes, &groupInfo, options);
	}
	else if (items.isObjs()) {
		auto objs = items.getObjs();
		std::vector<int> idx;

		if (index && index->size() > 0) {
			idx = *index;
			std::sort(idx.begin(), idx.end());
		}
		else {
			for (int i = 0; i < objs.size(); ++i) {
				idx.emplace_back(i);
			};
		};
		//if (idx.size() > 5000) {
		//	/*auto* manager = GetViewPanelManager();
		//	int currentLayout = ip->GetViewportLayout();*/
		//	
		//	/*auto* ip7 = GetCOREInterface7();
		//	mprintf(L"Current viewport Index  = %d\n", ip7->getActiveViewportIndex());*/
		//	//ip7->setActiveViewport(0);
		//	//for (int i = 0;i<)		

		//	ip->SetViewportLayout(VP_LAYOUT_1C);			
		//	
		//	//ip7->SetActiveViewportRenderLevel(LITE_WIREFRAME);
		//	//ip->SelectNode(GetCOREInterface()->GetRootNode(), 1);
		//}
		loadIndices(objs, idx, &groupNodes, &groupInfo, options);

	};

	return		nullptr;

}

template <class Container>
inline void* readMessage(char* filename, Container* container = nullptr, int options = ExportOpts::all) {
	static capnp::ReaderOptions opts;
	opts.traversalLimitInWords = ((uint64_t)2048) << 20;
	//std::ifstream myfile;
	//myfile.open(filename, std::ios::in | std::ios::binary);
	//if (myfile.is_open()) {
	//	/* ok, proceed with output */
	//	mprintf(L"File read\n");
	//	kj::std::StdInputStream instream(myfile);		
	//	
	//	
	//	capnp::InputStreamMessageReader reader(instream, opts);
	//	
	//	
	//	load(obj, reader);
	//	myfile.close();
	//	
	//}
	//else mprintf(L"File cannot be read\n");
	//mprintf(L"Everything done\n");

	HANDLE hMapFile;
	capnp::word* pBuf;

	//TCHAR szName[512];
	//_tcscpy(szName, A2T(stheParameterFileName));
	std::string s(filename);
#ifdef _UNICODE
	auto ws = s2ws(s);
#else
	auto ws = s;
#endif

	auto hFile = CreateFile(
		(TCHAR*)(ws.data()),
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
	auto error = GetLastError();
	if (hFile == INVALID_HANDLE_VALUE)
	{
		mprintf(L"Could not open file  (%d).\n",
			error);
		return nullptr;
	}

	hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	error = GetLastError();
	if (hMapFile == NULL)
	{
		mprintf(L"Could not open file mapping object (%d).\n",
			error);
		CloseHandle(hFile);
		return nullptr;
	}


	pBuf = (capnp::word*)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_READ,  // read/write permission
		0,
		0,
		0);
	error = GetLastError();
	if (pBuf == NULL)
	{
		mprintf(L"Could not map view of file (%d).\n",
			error);

		CloseHandle(hMapFile);
		CloseHandle(hFile);

		return nullptr;
	}

	auto size = GetFileSize(hFile, NULL);
	char* end = ((char*)pBuf) + size;
	mprintf(L"File size is %d\n", size);
	kj::ArrayPtr<capnp::word> arr = kj::arrayPtr(pBuf, (capnp::word*)end);

	capnp::FlatArrayMessageReader msg(arr, opts);

	void* obj = (void*)load(msg, container, options);
	//void* obj = (void*)load2(msg);
	//MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);
	CloseHandle(hFile);
	mprintf(L"Loading done\n");//*/
	return obj;
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

/***********************************************************EXPORT FUNCTION **************************************************************/
/*
Load Object from file
@filename : name of file
@node : pointer to a scene object whose content will be modified with data read from file. nullptr means a new scene obj will be created
@options: (optional, default to all)Load all, mesh, selection, uv, normals or a combination them
@exportFolder: (optional) the folder where the filename will be searched
@objName: (optional) if the file contains multiple objects, read the object whose name is specified by objName
*/
inline void loadObject(wchar_t* filename, INode* node = nullptr, int options = ExportOpts::all, wchar_t* exportFolder = L"", wchar_t* objName = nullptr) {
	std::wstring s;
	s = exportFolder;
	s += filename;
	s += L".cap";
	mprintf(L"Loading file %s (%s)\n", s.c_str(), filename);
	if (node && objName) node->SetName(objName);
	auto* obj = readMessage((char*)ws2s(s).c_str(), node, options);
	//node->G GetMesh().InvalidateTopoCache();
	//node->GetMesh().InvalidateGeomCache();
	//readMessage(obj, ws2s(s).c_str());
}

inline void replaceObject(wchar_t* name, INode* node = nullptr, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	std::wstring s;
	s = exportFolder;
	s += name;
	s += L".cap";
	if (node) GetCOREInterface()->DeleteNode(node, FALSE);
	INode* newNode = nullptr;
	auto* obj = readMessage((char*)ws2s(s).c_str(), newNode, ExportOpts::all);
	//readMessage(obj, ws2s(s).c_str());
}

/*
Load a selected Objects from a file
@filename : name of file
@nodes : pointer to a list of scene objects whose content will be modified with data read from file. nullptr means every node on scene can be loaded
@options: (optional, default to all)Load all, mesh, selection, uv, normals or a combination them
@exportFolder: (optional) the folder where the filename will be searched
*/
inline void loadObjects(wchar_t* name, INodeTab* nodes = nullptr, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	ip->DisableSceneRedraw();
	std::wstring s;
	s = exportFolder;
	s += name;
	s += L".cap";
	mprintf(L"Loading file %s (%s)\n", s.c_str(), name);
	auto* obj = readMessage((char*)ws2s(s).c_str(), nodes, options);
	ip->EnableSceneRedraw();
	//readMessage(obj, ws2s(s).c_str());
}


/*
Load a selected Objects from a file
@filename : name of file
@names : pointer to a list of object names whose content will be modified with data read from file. nullptr means all objects in the file will be loaded to scene
@options: (optional, default to all)Load all, mesh, selection, uv, normals or a combination them
@exportFolder: (optional) the folder where the filename will be searched
*/
inline void loadObjects(wchar_t* name, std::vector<std::wstring>* names = nullptr, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	ip->DisableSceneRedraw();
	std::wstring s;
	s = exportFolder;
	s += name;
	s += L".cap";
	mprintf(L"Loading file %s (%s)\n", s.c_str(), name);
	auto* obj = readMessage((char*)ws2s(s).c_str(), names, options);
	ip->EnableSceneRedraw();
	//readMessage(obj, ws2s(s).c_str());
}

/*
Load a selected Objects from a file
@filename : name of file
@idx : pointer to a list of index of objects in the file. nullptr means all objects in the file will be loaded to scene
@options: (optional, default to all)Load all, mesh, selection, uv, normals or a combination them
@exportFolder: (optional) the folder where the filename will be searched
*/
inline void loadObjects(wchar_t* name, std::vector<int>* idx = nullptr, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	ip->DisableSceneRedraw();	
	std::wstring s;
	s = exportFolder;
	s += name;
	//s += L".cap";
	mprintf(L"Loading file %s (%s)\n", s.c_str(), name);
	auto* obj = readMessage((char*)ws2s(s).c_str(), idx, options);
	
	

	mprintf(L"Total Object creation time = %d ms\n", times[OBJECT_CREATION]);
	mprintf(L"Total Build Normals time = %d ms\n", times[BUILD_NORMALS]);
	mprintf(L"Total Load Normals time = %d ms\n", times[LOAD_NORMALS]);
	mprintf(L"Total Build Edges time = %d ms\n", times[BUILD_EDGE]);
	mprintf(L"Total Scene object create time = %d ms\n", times[OBJECT_SCENE_CREATE]);
	mprintf(L"Total Load mesh time = %d ms\n", times[LOAD_MESH]);
	mprintf(L"Total Load selection time = %d ms\n", times[LOAD_SELECTION]);
	mprintf(L"Total Load UV time = %d ms\n", times[LOAD_UV]);
	mprintf(L"Total Eval Object time = %d ms\n", times[EVAL_OBJECT]);
	mprintf(L"Total Create scene Node time = %d ms\n", times[CREATE_NODE]);
	//readMessage(obj, ws2s(s).c_str());
	
	ip->EnableSceneRedraw();	
	
	mprintf(L"Number of selection now is %d\n", ip->GetSelNodeCount());
}

inline void testRead(char* name) {
	INode* newNode = nullptr;
	readMessage(name, newNode);
}