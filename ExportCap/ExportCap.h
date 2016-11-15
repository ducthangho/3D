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
#include <kj/debug.h>
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
#include <algorithm>
#include <chrono>
#include <string>
#include <impapi.h>
#include <dummy.h>



//#include "tbb/blocked_range.h"
//#include "tbb/parallel_for.h"
//#include "tbb/task_scheduler_init.h"
//SIMPLE TYPE


#include <impexp.h>

#define USE_IMPNODES


extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;


enum ExportOpts {	
	mesh = 1,
	normals = 1 << 1,
	uv = 1 << 2,
	selection = 1 <<3,
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

#define CTL_CHARS  31
#define SINGLE_QUOTE 39

#define NO_GROUP  0
#define GROUP_HEAD 1
#define GROUP_OPEN_HEAD 2
#define GROUP_MEMBER 3
#define GROUP_OPEN_MEMBER 4



static Interface*		ip = GetCOREInterface();
static ImpInterface*	impip;
static Matrix3 ident(TRUE);
static BOOL autoConv = 0;

inline BOOL TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0), m.GetRow(1)), m.GetRow(2))<0.0) ? 1 : 0;
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
	Point3 pivot = node->GetObjOffsetPos();
	mat = Inverse(mat);
	Point3 pOff = VectorTransform(mat, pivot);
	return pOff;
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



inline Matrix3 getTM(INode* node) {

	if (!node) return ident;
	//Matrix3 mat = node->GetObjTMAfterWSM(ip->GetTime());
	Matrix3 mat = node->GetNodeTM(ip->GetTime());

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

	/*auto* pr = node->GetParentNode();
	if (pr && !pr->IsRootNode()) {
		Matrix3 iptm = pr->GetObjTMAfterWSM(ip->GetTime());
		mat = mat * Inverse(iptm);
	}*/
	return mat;
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
	mprintf(L" (%f, %f, %f,  %f) \n", p.x,p.y,p.z,p.w);
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

inline void displayNode(INode* node) {
	if (!node) return;
	auto* pr = node->GetParentNode();
	mprintf(L"-------------------\n");
	mprintf(L"Node name =%s (%s)\n", node->GetName(), pr->GetName());
	mprintf(L"Node TM = [\n");
	auto tm = node->GetNodeTM(ip->GetTime());
	printTM(tm);
	auto p = GetPivotOffset(node);
	mprintf(L"Pivot = (%f, %f, %f)\n", p.x, p.y, p.z);
	auto* obj = getObject(node);
	ObjectWrapper ow;
	auto os = node->EvalWorldState(0);
	ow.Init(0, os);
	mprintf(L"Num vertices = %d\n", ow.NumVerts());
	auto* pts = ow.Verts();
	auto num = min(10, ow.NumVerts());
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



inline MNFace& faceRef(MNMesh& in, int i) {
	return in.f[i];
}

inline Face& faceRef(Mesh& in, int i) {
	return in.faces[i];
}



inline BitArray& getVertexSel(MNMesh& in, BitArray& sels) {
	in.getVertexSel(sels);
	return sels;
}

inline BitArray& getVertexSel(ObjectWrapper& in, BitArray& sels) {
	return in.GetVertSel();
}


inline BitArray& getVertexSel(Mesh& in, BitArray& sels) {
	return in.VertSel();
}



inline BitArray& getFaceSel(ObjectWrapper& in, BitArray& sels) {
	return in.GetFaceSel();	
}



inline BitArray& getFaceSel(MNMesh& in, BitArray& sels) {
	in.getFaceSel(sels);
	return sels;
}

inline BitArray& getFaceSel(Mesh& in, BitArray& sels) {
	return in.FaceSel();
}


inline BitArray& getEdgeVis(MNMesh& in, BitArray& sels, BOOL negScale = FALSE) {
	sels.SetSize(in.nume);
	int k = 0;
	for (int i = 0; i < in.numf; ++i) {
		auto& f = in.f[i];
		for (int j = 0; j < f.deg; ++j) {
			if (!negScale) {
				if (f.visedg[j])
					sels.Set(k++);
				else sels.Clear(k++);
			}
			else {
				if (f.visedg[f.deg-1-j])
					sels.Set(k++);
				else sels.Clear(k++);
			}
		}
	}
	return sels;
}

inline BitArray& getEdgeVis(Mesh& in, BitArray& sels,BOOL negScale = FALSE) {	
	//in.BuildVisEdgeList();
	sels.SetSize(in.numFaces*3);
	int k = 0;
	for (int i = 0; i < in.numFaces; ++i) {
		auto& f = faceRef(in,i);
		for (int j = 0; j < 3; ++j) {
			if (!negScale) {
				if (f.getEdgeVis(j)) {
					sels.Set(k++);
					//if (i < 10) mprintf(L"(%d, %d, %d)\n", i, j, j + 1);
				}
				else sels.Clear(k++);
			}
			else {
				if (f.getEdgeVis(2-j)) {
					sels.Set(k++);
					//if (i < 10) mprintf(L"(%d, %d, %d)\n", i, j, j + 1);
				}
				else sels.Clear(k++);
			};
		}
	}
	return sels;
}

inline BitArray& getEdgeVis(ObjectWrapper& in, BitArray& sels, BOOL negScale = FALSE) {
	auto* mesh = in.GetPatchMesh();
	if (mesh) {
		sels.SetSize(in.NumEdges());		
		sels.SetAll();
		return sels;
	}


	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {		
		return getEdgeVis(*pmesh,sels, negScale);
	}
	auto* tmesh = in.GetTriMesh();
	if (tmesh) return getEdgeVis(*tmesh, sels, negScale);
	return sels;
}


inline BitArray& getEdgeSel(ObjectWrapper& in, BitArray& sels) {
	auto* mesh = in.GetPatchMesh();
	if (mesh) return mesh->EdgeSel();
	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {
		pmesh->getEdgeSel(sels);
		return sels;
	}
	auto* tmesh = in.GetTriMesh();
	if (tmesh) return tmesh->edgeSel;
	return sels;
	
}


inline BitArray& getEdgeSel(MNMesh& in, BitArray& sels) {
	in.getEdgeSel(sels);
	return sels;
}

inline BitArray& getEdgeSel(Mesh& in, BitArray& sels) {
	return in.edgeSel;
}



inline void getBB(MNMesh& in, Box3& bb) {
	in.BBox(bb);
}

inline void getBB(Mesh& in, Box3& bb) {
	in.buildBoundingBox();
	bb = in.getBoundingBox();
}

inline void getBB(ObjectWrapper& in, Box3& bb) {
	auto* mesh = in.GetPatchMesh();
	if (mesh) {
		mesh->buildBoundingBox();
		bb = mesh->getBoundingBox();
		return;
	}


	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {
		getBB(*pmesh, bb);
		return;
	}
	auto* tmesh = in.GetTriMesh();
	if (tmesh) {
		getBB(*tmesh, bb);
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
inline void saveV(V_IN& vi, V_OUT& vo) {
	vo.setOrig(vi.orig);
	vo.setX(vi.p.x);
	vo.setY(vi.p.y);
	vo.setZ(vi.p.z);
	vo.setFlags(vi.ExportFlags());
}


template <class V_IN, class V_OUT>
inline void saveP(V_IN& vi, V_OUT& vo) {
	vo.setX(vi.x);
	vo.setY(vi.y);
	vo.setZ(vi.z);
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
inline void saveE(E_IN& vi, E_OUT& vo) {
	vo.setV1(vi.v1);
	vo.setV2(vi.v2);
	vo.setF1(vi.f1);
	vo.setF2(vi.f2);
	vo.setTrack(vi.track);
	vo.setFlags(vi.ExportFlags());
}

template <class PT_IN, class PT_OUT>
inline void compareE(PT_IN& vi, PT_OUT& vo) {
	assert(vi.v1 == vo.getV1());
	assert(vi.v2 == vo.getV2());
	assert(vi.f1 == vo.getF1());
	assert(vi.f2 == vo.getF2());
	assert(vi.track == vo.getTrack());
}

template <class I, class O>
void compareArrV(I& in, O& out) {
	for (int i = 0; i < out.size(); ++i) {
		compareV(in[i], out[i]);
		//printPts(in[i], out[i]);				
	}
}

template <class I, class O>
inline void compareArrE(I& in, O& out) {
	for (int i = 0; i < out.size(); ++i) {
		compareE(in[i], out[i]);
	}
}

template <class I, class O>
inline void compareArrP(I& in, O& out) {
	for (int i = 0; i < out.size(); ++i) {
		compareP(in[i], out[i]);
	}
}

template <class ARR_IN, class ARR_OUT>
inline void copyArr(ARR_IN& ar1, ARR_OUT& ar2) {
	for (int i = 0; i < ar2.size(); ++i) {
		ar2.set(i, ar1[i]);
	}
}


template <class F_OUT>
inline void save3F(IGameMesh& vi, int faceIndex, F_OUT& vo,BOOL negScale = FALSE) {
	if (!negScale) {
		vo.setV0(vi.GetFaceVertex(faceIndex, 0));
		vo.setV1(vi.GetFaceVertex(faceIndex, 1));
		vo.setV2(vi.GetFaceVertex(faceIndex, 2));
	}
	else {
		vo.setV0(vi.GetFaceVertex(faceIndex, 2));
		vo.setV1(vi.GetFaceVertex(faceIndex, 1));
		vo.setV2(vi.GetFaceVertex(faceIndex, 0));
	}
	

	vo.setSmGroup(vi.GetFaceSmoothingGroup(faceIndex) );
	vo.setMaterial(vi.GetFaceMaterialID(faceIndex));
}


inline void save3F(MNFace& vi, format3d::TriFace::Builder& vo, BOOL negScale = FALSE) {
	if (!negScale) {
		vo.setV0(vi.vtx[0]);
		vo.setV1(vi.vtx[1]);
		vo.setV2(vi.vtx[2]);
	}
	else {
		vo.setV0(vi.vtx[2]);
		vo.setV1(vi.vtx[1]);
		vo.setV2(vi.vtx[0]);
	}
	

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.material);
}

inline void save3F(GenFace& vi, format3d::TriFace::Builder& vo, BOOL negScale = FALSE) {
	if (!negScale) {
		vo.setV0(vi.verts[0]);
		vo.setV1(vi.verts[1]);
		vo.setV2(vi.verts[2]);
	}
	else {
		vo.setV0(vi.verts[2]);
		vo.setV1(vi.verts[1]);
		vo.setV2(vi.verts[0]);
	}
}


inline void save4F(GenFace& vi, format3d::TriFace::Builder& vo, BOOL negScale = FALSE) {
	if (!negScale) {
		vo.setV0(vi.verts[0]);
		vo.setV1(vi.verts[1]);
		vo.setV2(vi.verts[2]);
	}
	else {
		vo.setV0(vi.verts[2]);
		vo.setV1(vi.verts[1]);
		vo.setV2(vi.verts[0]);
	}
}


inline void save3F(Face& vi, format3d::TriFace::Builder& vo, BOOL negScale = FALSE) {
	if (!negScale) {
		vo.setV0(vi.v[0]);
		vo.setV1(vi.v[1]);
		vo.setV2(vi.v[2]);
	}
	else {
		vo.setV0(vi.v[2]);
		vo.setV1(vi.v[2]);
		vo.setV2(vi.v[0]);
	};

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.getMatID());
}


template <class F_OUT>
inline void save4F(IGameMesh& vi, F_OUT& vo,int faceIndex, BOOL negScale = FALSE) {
	if (!negScale) {
		vo.setV0(vi.GetFaceVertex(faceIndex, 0));
		vo.setV1(vi.GetFaceVertex(faceIndex, 1));
		vo.setV2(vi.GetFaceVertex(faceIndex, 2));
		vo.setV3(vi.GetFaceVertex(faceIndex, 3));
	}
	else {
		vo.setV0(vi.GetFaceVertex(faceIndex, 3));
		vo.setV1(vi.GetFaceVertex(faceIndex, 2));
		vo.setV2(vi.GetFaceVertex(faceIndex, 1));
		vo.setV3(vi.GetFaceVertex(faceIndex, 0));
	}

	vo.setSmGroup(vi.GetFaceSmoothingGroup(faceIndex));
	vo.setMaterial(vi.GetFaceMaterialID(faceIndex));
}

template <class F_OUT>
inline void save4F(MNFace& vi, F_OUT& vo, BOOL negScale = FALSE) {
	if (!negScale) {
		vo.setV0(vi.vtx[0]);
		vo.setV1(vi.vtx[1]);
		vo.setV2(vi.vtx[2]);
		vo.setV3(vi.vtx[3]);
	}
	else {
		vo.setV0(vi.vtx[3]);
		vo.setV1(vi.vtx[2]);
		vo.setV2(vi.vtx[1]);
		vo.setV3(vi.vtx[0]);
	}

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.material);
}



template <class F_IN, class F_OUT>
inline void saveF(F_IN& vi, F_OUT& vo, BOOL negScale = FALSE) {
	vo.setFlags(vi.ExportFlags());
	int deg = vi.deg;
	vo.setDeg(deg);

	if (deg > 0) {
		auto vtx = vo.initVtx(deg);
		if (!negScale) {
			for (int i = 0; i < deg; ++i) {
				vtx.set(i, vi.vtx[i]);
			}
		}
		else {
			for (int i = 0; i < deg; ++i) {
				vtx.set(i, vi.vtx[deg-1-i]);
			}
		}

		/*auto edg = vo.initEdg(deg);
		for (int i = 0; i < deg; ++i) {
		edg.set(i, vi.edg[i]);
		}*/

		//*/
	}

	/*Tab<int> diags;
	diags.Resize(vi.TriNum() * 3);
	vi.GetTriangles(diags);
	auto diag = vo.initDiag(diags.Count());

	for (int i = 0; i < diag.size(); ++i) {
	diag.set(i, diags[i]);
	}//*/

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.material);
	vo.setTrack(vi.track);

	auto visedg = vo.initVisedg(vi.visedg.GetSize());
	copyArr(vi.visedg, visedg);

	auto edgsel = vo.initEdgsel(vi.edgsel.GetSize());
	copyArr(vi.edgsel, edgsel);

	auto bndedg = vo.initBndedg(vi.bndedg.GetSize());
	copyArr(vi.bndedg, bndedg);

}


template <class PerData_IN, class PerData_OUT>
inline void savePerData(PerData_IN& vdi, PerData_OUT& item) {
	item.setDnum(vdi.dnum);
	item.setType(vdi.type);
	item.setAlloc(vdi.alloc);
	//mprintf(L" (%d, %d, %d)\n", vdi.dnum, vdi.type, vdi.alloc);
	auto data = item.initData(vdi.alloc);
	kj::byte* vdata = (kj::byte*)vdi.data;
	for (int j = 0; j < data.size(); ++j)
		data[j] = vdata[j];
}


template <class I, class  O>
inline void compare(I& in, O& out) {
	for (int i = 0; i < out.size(); ++i) {
		/*if (out[i] != in[i]) {
		mprintf(L"False   %d   %d\n",in[i],out[i]);
		return;
		}*/

		assert(in[i] == out[i]);
	}
	//mprintf(L"OK\n");
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
	else if (o.IsSubClassOf(patchObjectClassID)){
		//mprintf(L"patchObjectClassID\n");
		return PATCHOBJ_T;
	}
	else if (o.SuperClassID() == HELPER_CLASS_ID) {
		//mprintf(L"patchObjectClassID\n");
		if (o.ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
			return DUMMYOBJ_T;
		return HELPEROBJ_T;
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
	if (o==nullptr) return -1;	
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
inline void savePolyObj(PolyObject& o, const char* name, Message& obj) {
	auto& mesh = o.GetMesh();
	obj.setType(POLYOBJ_T);
	obj.setName(name);
	auto meshCap = obj.initMmesh();
	saveMNMesh(mesh, meshCap);
	saveMNMeshUVW(o, mesh, meshCap);
}



template <class MNMESH_OUT>
inline void storeFaces(MNMesh& in, MNMESH_OUT& out, BOOL negScale = FALSE) {		
	auto faces = out.getFaces();		
	auto mnfaces = faces.initMnfaces();
	auto info = mnfaces.initInfo( in.numf);
	int num = 0;
	for (int i = 0; i < info.size(); ++i) {
		auto item = info[i];
		auto& inf = in.f[i];
		
		int deg = inf.deg;
		num += deg;
		item.setDeg(deg);
		item.setSmGroup(inf.smGroup);
		item.setMaterial(inf.material);
	}

	auto data = mnfaces.initF(num);
	int idx = 0;
	if (!negScale) {
		for (int i = 0; i < info.size(); ++i) {
			auto &inf = in.f[i];
			int deg = inf.deg;
			for (int j = 0; j < deg; ++j) {
				data.set(idx++, inf.vtx[j]);
			}
		}
	}
	else {
		for (int i = 0; i < info.size(); ++i) {
			auto &inf = in.f[i];
			int deg = inf.deg;
			for (int j = 0; j < deg; ++j) {
				data.set(idx++, inf.vtx[deg-1-j]);
			}
		}
	}
}

template <class MNMESH_OUT>
inline void storeFaces(ObjectWrapper& in, MNMESH_OUT& out, BOOL negScale = FALSE) {

	auto faces = out.getFaces();
	auto mnfaces = faces.initMnfaces();
	auto info = mnfaces.initInfo(in.NumFaces());

	int num = 0;
	for (int i = 0; i < info.size(); ++i) {
		auto item = info[i];
		auto inf = in.GetFace(i);

		int deg = inf.numVerts;
		num += deg;
		item.setDeg(deg);
		item.setSmGroup(in.GetSmoothingGroup(i));
		item.setMaterial(in.GetMtlID(i));
	}

	auto data = mnfaces.initF(num);
	int idx = 0;
	if (!negScale) {
		for (int i = 0; i < info.size(); ++i) {
			GenFace inf = in.GetFace(i);
			int deg = inf.numVerts;

			for (int j = 0; j < deg; ++j) {
				data.set(idx++, inf.verts[j]);
			}
		}
	}
	else {
		for (int i = 0; i < info.size(); ++i) {
			GenFace inf = in.GetFace(i);
			int deg = inf.numVerts;

			for (int j = 0; j < deg; ++j) {
				data.set(idx++, inf.verts[deg-1-j]);
			}
		}
	}
	


}


template <class MNMESH_OUT>
inline void storeFaces(Mesh& in, MNMESH_OUT& out, BOOL negScale=FALSE) {
	auto faces = out.getFaces();
	auto f3 = faces.initF3(in.numFaces);
	//mprintf(L"Saving %d faces\n", in.numFaces);
	for (int i = 0; i < in.numFaces; ++i) {
		auto& f = faceRef(in,i);
		auto item = f3[i];		
		save3F(f, item, negScale);
		/*if (i < 10) {
			mprintf(L"f %d\t%d\t%d\n", item.getV0(), item.getV1(), item.getV2());
		}*/
	}
}


inline int getNormalFaceDegree(MNNormalFace & f) {
	return f.GetDegree();
}


inline int getNormalFaceDegree(MeshNormalFace & f) {
	return 3;
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
	return 4;
}

inline int meshFaceType(MNMap& in) {
	return 4;
}

inline int meshFaceType(MeshMap& in) {
	return 3;
}


inline int meshFaceType(Mesh& in) {
	return 3;
}


template <class M,class O>
inline void buildNormals(M& in, O& out,BOOL negScale = FALSE) {
	Time t;
	//in.buildNormals();
	//in.SpecifyNormals();

	auto* normalSpec = GetSpecifiedNormals(in);
	if (normalSpec) {
		clearNormalFlags(normalSpec);
		normalSpec->SetParent(&in);
		normalSpec->BuildNormals();
	}
	if (normalSpec && isNormalBuilt(normalSpec)) {
		
		normalSpec->SpecifyNormals();
		auto mpNormal = out.initMpNormal(normalSpec->GetNumNormals());
		for (int i = 0; i < normalSpec->GetNumNormals(); i++)
		{
			auto item = mpNormal[i];
			auto nrm = normalSpec->Normal(i).Normalize();
			saveP(nrm, item);
		}

		int type = meshFaceType(in);		

		auto mpFace = out.initMpFace(normalSpec->GetNumFaces() * type);
		for (int i = 0; i < normalSpec->GetNumFaces(); ++i) {
			auto& face = normalSpec->Face(i);			
			int num = getNormalFaceDegree(face);			
			if (num == 3 || num == 4) {
				for (int j = 0; j < num; ++j) {

					if (face.GetSpecified(j))
						mpFace.set(i*type + j, (!negScale) ? face.GetNormalID(j) : face.GetNormalID(num-1-j));
					else {
						//item.set(j, -1);//*/
						mpFace.set(i*type + j, (!negScale) ? -face.GetNormalID(j) : -face.GetNormalID(num - 1 - j));
						//mprintf(L"Face %d:  %d(%d)\n", i, face.GetNormalID(j), face.GetSpecified(j) == true);
					}
				}
			}
			if (num<type-1) mpFace.set((i+1)*type - 1, LONG_MIN);
		}//*/
		//DebugPrint(normalSpec);
	}
	//else mprintf(L"None *********\n");
	time(t, BUILD_NORMALS);
}

template <class O>
inline void buildNormals(ObjectWrapper& in, O& out,BOOL negScale = FALSE) {	
	auto* mesh = in.GetTriMesh();
	if (mesh) {
		buildNormals(*mesh, out, negScale);
		return;
	}
	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {
		buildNormals(*pmesh, out, negScale);
		return;
	}
	auto* patchmesh = in.GetPatchMesh();
	if (patchmesh) {
		/*TODO: */
	}	
}


inline void clearNormalFlags(MNNormalSpec* m) {
	m->ClearFlag(MNNORMAL_NORMALS_BUILT);
}

inline void clearNormalFlags(MeshNormalSpec* m) {
	m->ClearFlag(MESH_NORMAL_NORMALS_BUILT);
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
	return in.numFaces*3;
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


inline void testEdge(MNMesh& in) {


}

inline void testEdge(Mesh& in) {
	/*for (int i = 0; i < 10; ++i) {
		auto& f = faceRef(in, i);
		mprintf(L"Face %d:\t", i);
		for (int j = 0; j < 3; ++j) {
			if (f.getEdgeVis(j))
				mprintf(L"\tEdge (%d, %d)\n", j, j + 1);
		}
	}//*/
}

inline int GetSelLevel(ObjectWrapper& in) {
	return in.GetSelLevel();
}

inline int GetSelLevel(MNMesh& in) {
	return in.selLevel;
}

inline int GetSelLevel(Mesh& in) {
	return in.selLevel;
}

inline int GetDispFlags(ObjectWrapper& in) {
	auto* mesh = in.GetPatchMesh();
	if (mesh) return mesh->dispFlags;
	auto* pmesh = in.GetPolyMesh();
	if (pmesh) return pmesh->dispFlags;
	auto* tmesh = in.GetTriMesh();
	if (tmesh) return tmesh->dispFlags;
	return 0;
}

inline int GetDispFlags(MNMesh& in) {
	return in.dispFlags;
}

inline int GetDispFlags(Mesh& in) {
	return in.dispFlags;
}


template <class MNMESH_IN, class MNMESH_OUT>
inline void storeMesh(MNMESH_IN& in, MNMESH_OUT& out, Matrix3& tm, int options = ExportOpts::all,  Box3* bbox = nullptr) {
	BitArray sels;
	BOOL negScale = FALSE;
	if (options & ExportOpts::mesh) {
		negScale = TMNegParity(tm);		

		int numv = numVerts(in);
		auto v = out.initV(numv);
		if (DEBUG) {
			printTM(tm);
			auto inv = Inverse(tm);
			mprintf(L"Inv tm = \n");
			printTM(inv);
		}

		//mprintf(L"Store %d num verts\n", numv);
		for (int i = 0; i < numv; ++i) {
			if (DEBUG && i < 10) {
				auto p = vert(in, i);
				auto np = tm * p;
				mprintf(L"(%f, %f, %f) --> (%f, %f, %f)\n", p.x, p.y, p.z, np.x, np.y, np.z);
			}
			auto vv = tm * vert(in, i);
			//vv *= 0.01;
			saveP(vv, v[i]);
			/*if (i < 10) {
				printPts(vert(in, i), v[i]);
			}*/
		}
		//mprintf(L"---------------\n");		
		storeFaces(in, out, negScale);
		//mprintf(L"---------------\n");


		//testEdge(in);
		
		getEdgeVis(in, sels, negScale);
		//mprintf(L"Saving %d edge vis\n", sels.GetSize());
		auto evis = out.initEVis(sels.GetSize());		
		copyArr(sels, evis);//*/
		
	}

	out.setDispFlags(GetDispFlags(in));	
	out.setSelLevel(GetSelLevel(in));

	if (options & ExportOpts::selection) {
		//mprintf(L"Saving selection now\n");
		auto& in_vsel = getVertexSel(in, sels);
		auto vSel = out.initVSel(in_vsel.GetSize());
		copyArr(in_vsel, vSel);

		auto& in_esel = getEdgeSel(in, sels);
		//mprintf(L"Store %d edges\n", in_esel.GetSize());
		auto eSel = out.initESel(in_esel.GetSize());
		copyArr(in_esel, eSel);

		auto& in_fsel = getFaceSel(in, sels);
		auto fSel = out.initFSel(in_fsel.GetSize());
		copyArr(in_fsel, fSel);				
	};

	Box3 bb;
	if (!bbox) {
		getBB(in, bb);
	}
	else bb = *bbox;
	
	auto box = out.initBdgBox();
	auto pmin = box.initPmin();
	auto pmax = box.initPmax();
	pmin.setX(bb.pmin.x);
	pmin.setY(bb.pmin.y);
	pmin.setZ(bb.pmin.z);

	pmax.setX(bb.pmax.x);
	pmax.setY(bb.pmax.y);
	pmax.setZ(bb.pmax.z);
	
	//in.buildNormals();
	if (options & ExportOpts::normals) {
		negScale = TMNegParity(tm);
		buildNormals(in, out,negScale);
	}
	
}



template <class MNMAP_OUT>
inline void saveUVW(MNMap& in, MNMAP_OUT& out, BOOL negScale = FALSE) {
	//out.setFlags(in.ExportFlags());

	auto v = out.initV(in.numv);
	for (int i = 0; i < v.size(); ++i) {
		auto item = v[i];
		saveP(in.v[i], item);
	}

	int num = 0;
	for (int i = 0; i < in.numf; ++i) {
	
		auto& inf = in.f[i];
		int deg = inf.deg;
		num += deg + 1;
		
	}

	auto f = out.initF(num);
	int idx = 0;
	if (!negScale) {				
		for (int i = 0; i < in.numf; ++i) {			
			auto& inf = in.f[i];
			int deg = inf.deg;

			f.set(idx++, -deg);

			for (int j = 0; j < deg; ++j) {				
				f.set(idx++,inf.tv[j]);
			}
			
		}		
	} else {					
		for (int i = 0; i < in.numf; ++i) {
			auto& inf = in.f[i];
			int deg = inf.deg;

			f.set(idx++, -deg);

			for (int j = 0; j < deg; ++j) {
				f.set(idx++, inf.tv[deg-1-j]);
			}

		}
	}

}

template <class MNMAP_OUT>
inline void saveUVW(MeshMap& in, MNMAP_OUT& out, BOOL negScale = FALSE) {
	//out.setFlags(in.ExportFlags());

	auto v = out.initV(in.vnum);
	//mprintf(L"Saving %d UV verts\n", in.vnum);
	for (int i = 0; i < v.size(); ++i) {
		auto item = v[i];
		saveP(in.tv[i], item);
	}

	auto f = out.initF(in.fnum * 3);
	//mprintf(L"Saving %d UV faces\n------------------\n", in.fnum);
	if (!negScale) {
		int idx = 0;
		for (int i = 0; i < in.fnum; ++i) {
			//int idx = i * 3;//i*3
			auto& inf = in.tf[i];
			//copyArr(in.tf[i].t, f,i*3);

			for (int j = 0; j < 3; ++j) {
				f.set(idx++, inf.t[j]);
			}
		}
	} else {
		int idx = 0;
		for (int i = 0; i < in.fnum; ++i) {
			//int idx = i * 3;//i*3
			auto& inf = in.tf[i];
			//copyArr(in.tf[i].t, f,i*3);

			for (int j = 0; j < 3; ++j) {
				f.set(idx++, inf.t[2-j]);
			}
		}
	}

}


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



template <class MNMESH_OUT>
inline void saveMNMeshUVW(PolyObject& o, MNMesh& in, MNMESH_OUT& out, BOOL negScale= FALSE) {
	//if (!o.HasUVW()) return;
	int maxChannel = o.NumMapsUsed();	
	std::vector<int> channels;
	channels.reserve(maxChannel+NUM_HIDDENMAPS);
	//mprintf(L"Max channels %d    \n", maxChannel);
	for (int i = -NUM_HIDDENMAPS; i < maxChannel; ++i)
		if (o.HasUVW(i)) {	
			auto* ptr = in.M(i);
			if (ptr && !ptr->GetFlag(MN_DEAD)) {
				channels.emplace_back(i);
			}
		}


	auto m = out.initM(channels.size());
	int j = 0;

	for (int i = 0; i < channels.size(); ++i) {
		int channel = channels[i];
		auto* ptr = in.M(channel);
		if (ptr && !ptr->GetFlag(MN_DEAD)) {
		//	mprintf(L"Used channels %d  %d   \n", channels.size(), channel);
			auto map = m[i];
			map.setChannel(channel);
			saveUVW(*ptr, map, negScale);
		}
	}
}

template <class OBJ>
inline bool hasUVW(OBJ& o) {
	return o.HasUVW();
}

template <>
inline bool hasUVW(ObjectWrapper& o) {
	return true;
}


template <class Obj,class MNMESH_OUT>
inline void saveMNMeshUVW(Obj& o, Mesh& in, MNMESH_OUT& out,BOOL negScale = FALSE) {
	if (!hasUVW(o)) return;
	int maxChannel = o.NumMapsUsed();	
	std::vector<int> channels;
	channels.reserve(maxChannel+ NUM_HIDDENMAPS);
	//mprintf(L"Max channels %d    \n", maxChannel);
	for (int i = -NUM_HIDDENMAPS; i < maxChannel; ++i){
		if (o.HasUVW(i)) 			
			channels.emplace_back(i);			
	}


	auto m = out.initM(channels.size());
	int j = 0;

	for (int i = 0; i < channels.size(); ++i) {
		int channel = channels[i];
		auto& ptr = in.Map(channel);		
		//mprintf(L"Used channels %d  %d   \n", channels.size(), channel);
		auto map = m[i];
		map.setChannel(channel);
		saveUVW(ptr, map, negScale);
	}

	/*mprintf(L"Aha\n");
	channels.clear();
	for (int i = -NUM_HIDDENMAPS; i < 0; ++i) {
		auto idx = i + NUM_HIDDENMAPS;
		if (o.HasUVW(i))
			channels.emplace_back(i);	
	}

	mprintf(L"Hidden Channel %d\n", channels.size());

	auto hmap = out.initHmap(channels.size());
	for (int i = 0; i < channels.size(); ++i) {
		auto channel = channels[i];
		auto& ptr = in.Map(channel);		
		auto map = hmap[i];
		map.setChannel(channel);
		saveUVW(ptr, map);		
	}//*/

}

template <class MNMESH_OUT>
inline void saveMNMeshUVW(ObjectWrapper& o, Mesh& in, MNMESH_OUT& out, BOOL negScale = FALSE) {
	if (!hasUVW(o)) return;
	int maxChannel = in.getNumMaps();
	std::vector<int> channels;
	channels.reserve(maxChannel + NUM_HIDDENMAPS);
	//mprintf(L"Max channels %d    \n", maxChannel);
	for (int i = -NUM_HIDDENMAPS; i < maxChannel; ++i) {
		if (in.mapSupport(i))
			channels.emplace_back(i);
	}


	auto m = out.initM(channels.size());
	int j = 0;

	for (int i = 0; i < channels.size(); ++i) {
		int channel = channels[i];
		auto& ptr = in.Map(channel);
		//mprintf(L"Used channels %d  %d   \n", channels.size(), channel);
		auto map = m[i];
		map.setChannel(channel);
		saveUVW(ptr, map, negScale);
	}
}


template <class Obj>
inline void saveMesh(Obj& o, ::capnp::MallocMessageBuilder& message, Matrix3& tm, int options = ExportOpts::all) {
	/*if (o.IsSubClassOf(polyObjectClassID)) {
	auto& mesh = o.GetMesh();
	if (stripped) {
	auto meshCap = message.initRoot<format3d::MNMeshStripped>();
	saveMNMeshStripped(mesh, meshCap);
	saveMNMeshUVW(o, mesh, meshCap);
	}
	else {
	auto meshCap = message.initRoot<format3d::MNMesh>();
	saveMNMesh(mesh, meshCap);
	saveMNMeshUVW(o, mesh, meshCap);
	}

	}//*/
	auto meshCap = message.initRoot<format3d::Mesh>();
	saveMesh(o, meshCap,tm, options);
}


inline void saveMesh(PolyObject& o, ::format3d::Mesh::Builder& meshCap, Matrix3& tm, int options = ExportOpts::all) {
	MNMesh& mesh = getMesh(o);
	//mprintf(L"Saving poly object\n");	
	storeMesh(mesh, meshCap, tm, options);
	if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap, TMNegParity(tm));	
}


inline void saveMesh(TriObject& o, ::format3d::Mesh::Builder& meshCap, Matrix3& tm, int options = ExportOpts::all) {
	Mesh& mesh = getMesh(o);
	//mprintf(L"Saving tri object\n");
	storeMesh(mesh, meshCap, tm, options);
	if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap, TMNegParity(tm));
}


inline void saveMesh(ObjectWrapper& o, ::format3d::Mesh::Builder& meshCap, Matrix3& tm, int options = ExportOpts::all) {
	//mprintf(L"Saving wrapper object\n");
	auto& mesh = getMesh(o);	
	storeMesh(o, meshCap, tm, options);
	if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap, TMNegParity(tm));
}


inline void saveMesh(PatchObject& o, ::format3d::Mesh::Builder& meshCap, Matrix3& tm, int options = ExportOpts::all) {
	Mesh& mesh = getMesh(o);
	//mprintf(L"Saving patch object\n");
	storeMesh(mesh, meshCap, tm, options);
	if (options & ExportOpts::uv)saveMNMeshUVW(o, mesh, meshCap, TMNegParity(tm));
}


inline void saveMesh(Mesh& mesh, ::format3d::Mesh::Builder& meshCap, Matrix3& tm, int options = ExportOpts::all) {
	//mprintf(L"Saving Mesh\n");
	storeMesh(mesh, meshCap, tm, options);
	//if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
}

inline void saveMesh(MNMesh& mesh, ::format3d::Mesh::Builder& meshCap, Matrix3& tm, int options = ExportOpts::all) {
	//mprintf(L"Saving MNMesh\n");
	storeMesh(mesh, meshCap, tm, options);
	//if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
}

inline void saveMesh(Object& o, ::format3d::Mesh::Builder& meshCap,Matrix3& tm, int options = ExportOpts::all) {

	if (o.IsSubClassOf(polyObjectClassID)) {
		PolyObject& obj = (PolyObject&)(o);
		saveMesh(obj, meshCap, tm, options);
	}
	else if (o.IsSubClassOf(triObjectClassID)) {
		TriObject& obj = (TriObject&)(o);
		saveMesh(obj, meshCap, tm, options);
	}
	else if (o.IsSubClassOf(patchObjectClassID)) {
		PatchObject& obj = (PatchObject&)(o);
		saveMesh(obj, meshCap,tm, options);
	}
	else {
		Mesh& mesh = getMesh(o);
		//mprintf(L"Saving Object\n");
		storeMesh(mesh, meshCap, tm, options);
		if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
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


inline MNMap& getMapChannel(MNMesh& mesh, int channel) {
	return *mesh.M(channel);
}

inline MeshMap& getMapChannel(Mesh& mesh, int channel) {
	return mesh.Map(channel);
}



inline void checkData(MNMesh& mesh) {
	if (!mesh.CheckAllData())
		mprintf(L"Something is wrong\n");
}

inline void checkData(Mesh& mesh) {};


inline INode* CreateObjectInScene(SClass_ID sid, Class_ID cid) {
	Time t;
	Object* obj = static_cast<Object*>(CreateInstance(sid, cid));	
	time(t, CREATE_NODE);
	auto* node = GetCOREInterface()->CreateObjectNode(obj);
	return node;
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



template <class Message>
inline void writeMessage(Message& message, char* filename) {
	std::ofstream myfile;
	myfile.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
	if (myfile.is_open()) {
		/* ok, proceed with output */
		kj::std::StdOutputStream ostream(myfile);
		capnp::writeMessage(ostream, message);
		myfile.close();
		mprintf(L"File saved\n");
	}
	else mprintf(L"File cannot be saved\n");
}

//inline void saveObject(PolyObject& o, format3d::Object::Builder& obj, char* name, int options = ExportOpts::all) {
//	MNMesh& mesh = getMesh(o);
//	obj.setType(getType(o));
//	obj.setName(name);
//	::format3d::Mesh::Builder meshCap = obj.initMesh();
//	saveMesh(mesh, meshCap, options);
//}


inline void ExportNodeTM(INode* node,format3d::Object::Builder& obj) {
	auto tmout = obj.initTm(12);
	Matrix3 tm = node->GetNodeTM(ip->GetTime());
	//Matrix3 tm = node->GetObjTMAfterWSM(0);
	MRow* mm = tm.GetAddr();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			tmout.set(i * 3 + j, mm[i][j]);
		}
	}

	auto pivot = obj.initPivot(10);
	auto piv = node->GetObjOffsetPos();
	pivot.set(0, piv.x);
	pivot.set(1, piv.y);
	pivot.set(2, piv.z);
	auto quat = node->GetObjOffsetRot();
	pivot.set(3, quat.x);
	pivot.set(4, quat.y);
	pivot.set(5, quat.z);
	pivot.set(6, quat.w);
	auto scale = node->GetObjOffsetScale().s;
	pivot.set(7, scale.x);
	pivot.set(8, scale.y);
	pivot.set(9, scale.z);
}

inline void ExportNodeHeader(INode* node, format3d::Object::Builder& obj) {
	Interface14 *iface = GetCOREInterface14();
	UINT codepage = iface->DefaultTextSaveCodePage(true);

	auto* nodeName = node->GetName();
	TSTR name(nodeName);
	//TSTR name = FixupName(nodeName);
	//if (name == L"obj_06") {
	//	
	//	auto* nameMaker = ip->NewNameMaker();			
	//	if (nameMaker->NameExists(name)) {
	//		//mprintf(L"Renaming %s ", nodeName);
	//		nameMaker->MakeUniqueName(name);
	//		node->SetName(name.data());
	//		//mprintf(L"to %s \n", nodeName);
	//	}
	//	mprintf(L"Here\n");
	//}
	
	obj.setName((const char*)name.ToCP(codepage));
	obj.setHandle(node->GetHandle());

	INode* parent = node->GetParentNode();
	if (parent && !parent->IsRootNode()) {
		name = (parent->GetName());
		const char* parentName_locale = name.ToCP(codepage);
		if (parentName_locale) {
			//mprintf(L"Parent name of %s is %s   %d\n", node->GetName(), s2ws(parentName_locale).c_str(),strlen(parentName_locale));
			obj.setParent(parentName_locale);
		}
	}// else obj.setParent("");
}

inline void ExportGroup(INode* node, format3d::Object::Builder& obj,std::vector<INode*>* groups = nullptr) {	
	uint8_t type = NO_GROUP;
	if (node == nullptr) return;
		
	if (node->IsGroupMember())
		type = GROUP_MEMBER;
	else if (node->IsGroupHead())
		type = GROUP_HEAD;
	else if (node->IsOpenGroupMember())
		type = GROUP_OPEN_MEMBER;
	else if (node->IsOpenGroupHead())
		type = GROUP_OPEN_HEAD;

	//mprintf(L"Set group type =%d\n",type);
	obj.setGroupType(type);
	if (type == GROUP_HEAD || type == GROUP_OPEN_HEAD) {
		auto gr = obj.initGroup();
		//gr.setGrName( toStr(node->GetName()) );
		mprintf(L"Exporting group %s\n", node->GetName());
		if (groups) {
			groups->emplace_back(node);
			gr.setIndex(groups->size()-1);
		}
	}
	else if (type == GROUP_MEMBER || type == GROUP_OPEN_MEMBER) {
		auto gr = obj.initGroup();
		if (groups && !groups->empty()) {
			int idx = groups->size() - 1;
			TSTR tmp(node->GetName());
			
			
			auto* head = groups->back();			
			if (head == node->GetParentNode()) {
				gr.setIndex(idx);				
			}
			else {
				auto* pr = node->GetParentNode();
				if (pr && !pr->IsRootNode()) {
					auto it = std::find(groups->begin(), groups->end(), pr);
					if (it != groups->end()) {
						gr.setIndex(it - groups->begin());
					}
					else {
						groups->emplace_back(pr);
						gr.setIndex(groups->size() - 1);
					}
				}
			}
			//auto* pr = (*groups)[idx];
			/*auto* pr = node->GetParentNode();
			mprintf(L"Exporting member %s of group %s\n", node->GetName(), pr->GetName());*/
		}
		else {
			auto* pr = node->GetParentNode();
			if (pr && !pr->IsRootNode()) 
				gr.setGrName( toStr(pr->GetName()));
			//mprintf(L"Exporting member %s of group %s\n", node->GetName(), pr->GetName());
		}
	}
}


template <class OBJ>
inline void saveObject(OBJ& o, format3d::Object::Builder& obj, Matrix3& tm,int options = ExportOpts::all) {		
	auto type = getType(o);
	obj.setType(type);
	auto& mesh = getMesh(o);
	//mprintf(L"Init mesh\n");
	if (type != DUMMYOBJ_T) {
		::format3d::Mesh::Builder meshCap = obj.initMesh();
		saveMesh(o, meshCap, tm, options);
		//saveMesh(mesh, meshCap, tm, options);

	}
	//o.UnlockObject();
}

static BOOL DEBUG = false;

inline void saveObject(INode* node, format3d::Object::Builder& obj, int options = ExportOpts::all) {
	//auto* obj = node->GetIGameObject();
	//static int count = 0;
	if (!node) return;		
	auto* o = getObject(node);
	auto tm = getTM(node);	
	//o->SetAFlag(A_LOCK_TARGET);
	ExportNodeTM(node, obj);
	ExportNodeHeader(node, obj);
	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();
	obj.setIFlags(iFlags);
	//mprintf(L"Write iFlags = %d\n", iFlags);
	
	

	if (o->IsSubClassOf(polyObjectClassID)) {
		PolyObject* ob = (PolyObject*)o;
		saveObject(*ob, obj, tm, options);
	}
	else if (o->IsSubClassOf(triObjectClassID)) {
		TriObject* ob = (TriObject*)o;
		saveObject(*ob, obj, tm, options);
	} else if (o->SuperClassID() == HELPER_CLASS_ID) {		
		if (o->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) {			
			obj.setType(DUMMYOBJ_T);			
		}
		else {
			MSTR s;
			o->GetClassName(s);
			mprintf(L"Missing help object. Check out %s\n",s);
		}
	}
	else {		
		if (o->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) {			
			TriObject* ob = (TriObject*)o->ConvertToType(GetCOREInterface()->GetTime(),
				Class_ID(TRIOBJ_CLASS_ID, 0));
			//->SetAFlag(A_LOCK_TARGET);
			//mprintf(L"Saving object2 %s     %d\n", s2ws(str).c_str(), s);
			if (ob)
				saveObject(*ob, obj, tm, options);
			//mprintf(L"MaybeAutoDelete\n");
			if (ob && ob != o) ob->MaybeAutoDelete();
			//ob->UnlockObject();
		}
		else {
			mprintf(L"********************************* Cannot convert *******************************\n");
			obj.setType(DUMMYOBJ_T);	
			MSTR s;
			o->GetClassName(s);
			mprintf(L"Check out %s\n", s);
		}
	}
	

	/*if ( strcmp(s,"Mesh12") ==0  )
		printNodeTM(node);*/
	//o->ClearAFlag(A_LOCK_TARGET);
}




inline void saveObject(IGameNode* node, format3d::Object::Builder& obj, int options = ExportOpts::all) {
	//auto* obj = node->GetIGameObject();
	if (!node) return;
	auto* o = (node->GetIGameObject()->GetMaxObject());

	//o->LockObject();
	auto tm = node->GetLocalTM(GetCOREInterface()->GetTime()).ExtractMatrix3();
	ExportNodeTM(node->GetMaxNode(), obj);
	ExportNodeHeader(node->GetMaxNode(), obj);
	if (o->IsSubClassOf(polyObjectClassID)) {
		PolyObject* ob = (PolyObject*)o;
		saveObject(*ob, obj,tm, options);
	}
	else if (o->IsSubClassOf(triObjectClassID)) {
		TriObject* ob = (TriObject*)o;
		saveObject(*ob, obj,tm, options);
	}
	else if (o->SuperClassID() == HELPER_CLASS_ID) {
		if (o->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) {
			obj.setType(DUMMYOBJ_T);			
		}


	} else if (o->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) {

		TriObject* ob = (TriObject*)o->ConvertToType(GetCOREInterface()->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));
		if (ob) {
			saveObject(*ob, obj, tm, options);
			if (ob != o) ob->MaybeAutoDelete();
		}
	}
	//o->UnlockObject();
}

inline void saveMasterScale(format3d::Anything::Builder& item) {
	int type;
	float scale;
	GetMasterUnitInfo(&type, &scale);
	float msc_factor = (float)GetMasterScale(type);
	item.setMasterScale(msc_factor);
}


template <class Obj>
inline void saveObject(Obj& o, ::capnp::MallocMessageBuilder& m, char* name, Matrix3& tm,int options = ExportOpts::all) {
	//auto& mesh = getMesh(o);
	auto root = m.initRoot<format3d::Anything>();
	saveMasterScale(root);
	auto items = root.getItems();
	auto obj = items.initObj();
	obj.setType(getType(o));
	obj.setName(name);
	::format3d::Mesh::Builder meshCap = obj.initMesh();
	saveMesh(o, meshCap, options);
	auto tmout = obj.initTm(12);
	MRow* mm = tm.GetAddr();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			tmout.set(i * 3 + j, mm[i][j]);
		}
	}
}


inline void saveObject(IGameNode* node, ::capnp::MallocMessageBuilder& m, int options = ExportOpts::all) {
	auto root = m.initRoot<format3d::Anything>();
	saveMasterScale(root);
	auto items = root.getItems();
	auto obj = items.initObj();	
	saveObject(node, obj, options);
}

//inline std::wstring getName(IGameNode* node) {
//	std::wstring s(node->GetName());
//	return s;
//}




template <class OBJ>
inline void saveObject(OBJ& o, format3d::Anything::Builder& msg, char* name, Matrix3& tm, int options = ExportOpts::all) {
	//o.LockObject();
	auto& mesh = getMesh(o);
	saveMasterScale(msg);
	//o.UnlockObject();
	mprintf(L"Save !!!!!!!!\n");
	auto items = msg.getItems();
	auto obj = items.initObj();
	saveObject(o, obj, name, tm,options);
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
		theHold.Accept(GetString(IDS_COLLAPSE));
	}
}

template <class ObjectType>
inline Class_ID getClassID(ObjectType* obj){
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
		node->NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);

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
		//node->NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);

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

template <class Obj>
inline void saveObject(INode* node,Obj& o, TCHAR* name, Matrix3& tm, DWORD iFlags, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	//auto& mesh = getMesh(o);
	::capnp::MallocMessageBuilder m;
	auto root = m.initRoot<format3d::Anything>();
	saveMasterScale(root);
	auto items = root.getItems();
	auto obj = items.initObj();	
	//ExportGroup(node, obj);
	obj.setType(getType(o));
	std::wstring s(name);
	obj.setName(ws2s(s).c_str());
	s = exportFolder;
	s += name;
	s += L".cap";
	mprintf(L"Saving file %s \n", s.c_str());
	::format3d::Mesh::Builder meshCap = obj.initMesh();
	saveMesh(o, meshCap, tm, options);
	auto tmout = obj.initTm(12);
	MRow* mm = tm.GetAddr();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			tmout.set(i * 3 + j,mm[i][j]);			
		}
	}	
	obj.setIFlags(iFlags);
	writeMessage(m, (char*)ws2s(s).c_str());
}

inline void saveObject(IGameNode* node, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	if (node == nullptr) return;
	auto& o = *(node->GetIGameObject()->GetMaxObject());
	auto* inode = node->GetMaxNode();
	std::wstring s(node->GetName());
	auto tm = node->GetLocalTM(GetCOREInterface()->GetTime()).ExtractMatrix3();	
	DWORD iFlags = node->GetMaxNode()->GetTMController()->GetInheritanceFlags();
	saveObject(inode,o, (TCHAR*)s.c_str(),tm, iFlags, options,exportFolder);
}


inline void saveObject(INode* node, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	if (node == nullptr) return;
	auto& o = *getObject(node);
	std::wstring s(node->GetName());
	//auto tm = node->GetObjTMAfterWSM(GetCOREInterface()->GetTime());
	auto tm = getTM(node);
		
	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();	
	saveObject(node,o, (TCHAR*)s.c_str(),tm, iFlags, options, exportFolder);
}

inline void saveObject(INode* node, TCHAR* name, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	if (node == nullptr) return;
	auto& o = *getObject(node);
	std::wstring s(name);
	//auto tm = node->GetObjTMAfterWSM(GetCOREInterface()->GetTime());
	auto tm = getTM(node);
	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();	
	saveObject(node,o, (TCHAR*)s.c_str(),tm,iFlags, options, exportFolder);
}

/*
Save a list of scene objects to file
@nodes : pointer to the list of  scene object whose content will be saved file. nullptr means every scene object will be saved
@name : name of file
@options: (optional, default to all)Load all, mesh, selection, uv, normals or a combination them
@exportFolder: (optional) the folder where the filename will be searched
*/
inline void saveObjects(INodeTab* nodes, TCHAR* name, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	if (nodes == nullptr) {
		//Get all scene nodes
		INodeTab tmp;
		GetSceneNodes(tmp, nullptr);
		return saveObjects(&tmp, name, options, exportFolder);
	}

	::capnp::MallocMessageBuilder m;
	auto root = m.initRoot<format3d::Anything>();
	saveMasterScale(root);
	auto items = root.getItems();
	auto objs = items.initObjs(nodes->Count());
	
	std::wstring s(name);
	
	s = exportFolder;
	s += name;
	//s += L".cap";
	//mprintf(L"Saving %d items to %s\n", nodes->Count(),s);
	
	auto& nRef = *nodes;

	std::vector<INode*> groups;
	auto* nameMaker = ip->NewNameMaker();
	GetCOREInterface13()->SetNameSuffixLength(2);
	std::map<MSTR, int> mnames;

	for (int i = 0; i < nRef.Count(); ++i) {
		auto obj = objs[i];
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
	

	for (int i = 0; i < nRef.Count(); ++i) {
		auto obj = objs[i];
		auto* node = nRef[i];
		//if (i==51747)
		MSTR nodeName(node->GetName());
		MSTR tmp(nodeName);
		tmp.toLower();
		/*if (mnames.find(tmp) == mnames.end()){
			mnames[tmp]++;
		}
		else if (mnames[tmp]++ > 0){
			mprintf(L"Renaming %s ", nodeName);
			nameMaker->MakeUniqueName(nodeName);
			node->SetName(nodeName.data());
			tmp = nodeName;
			tmp.toLower();
			mnames[tmp]++;
			mprintf(L"to %s \n", nodeName);
		};
		*/
		
		try {
			//mprintf(L"Exporting node %s, type %d", nodeName,getType(node) );
			ExportGroup(node, obj, &groups);
		
		
			saveObject(node, obj, options);		
		}
		catch (const kj::Exception& e) {
			mprintf(L"--------------------\n");
			mprintf(L"Saving %d\n", i);
			mprintf(L"EXCEPTION %s (Line %d, file %s) \n", s2ws(e.getDescription().cStr()).c_str(), e.getLine(), fromStr(e.getFile(), strlen(e.getFile())));
			auto txt = stringifyStackTrace(e.getStackTrace());
			mprintf(L"%s\n", fromStr(txt.cStr(), txt.size()));

		}
	}

	mprintf(L"Now saving groupInfo\n");

	auto groupInfo = root.initGroupInfo(groups.size());
	for (int i=0;i<groups.size();++i){
		auto* node = groups[i];
		groupInfo.set(i, toStr(node->GetName()));
		//mprintf(L"%d : %s\n", i, node->GetName());

	}
	writeMessage(m, (char*)ws2s(s).c_str());
}



inline void test(char* name,int fcount=1000,int objectCount=100000){
	mprintf(L"fcount= %d\tobjectcount = %d\n", fcount, objectCount);
	Time t;
	::capnp::MallocMessageBuilder message;
	auto o = message.initRoot<format3d::Objects>();
	auto objs = o.initObjs(objectCount);
	char tmp[100];

	for (int i = 0; i < objectCount; ++i) {
		auto obj = objs[i];
		std::string s = "Noname ";
		sprintf_s(tmp, "%d", i);
		s += tmp;
		obj.setName(s.c_str());
		auto mesh = obj.initMesh();
		auto faces = mesh.getFaces();
		auto trifaces = faces.initF3(fcount);
		auto m = mesh.initM(1);
		auto mn = m[0];
		auto mnf = mn.initF(fcount);
		auto mnv = mn.initV(fcount);
		auto mv = mesh.initV(fcount);
		auto mpNormal = mesh.initMpNormal(fcount);
		for (int j = 0; j < fcount; ++j) {
			auto f = trifaces[j];
			f.setV0(1);
			f.setV1(1);
			f.setV2(1);
			f.setSmGroup(1);
			f.setMaterial(1);

			mnf.set(j, j);
			auto p = mnv[j];
			p.setX(1);
			p.setY(1);
			p.setZ(1);

			auto pp = mv[j];
			pp.setX(1);
			pp.setY(1);
			pp.setZ(1);

			auto p3 = mpNormal[j];
			p3.setX(1);
			p3.setY(1);
			p3.setZ(1);
		}
		if (i == 0) {
			//size_t size = capnp::computeSerializedSizeInWords(obj);
			mprintf(L"Size of one object is %d      %d\n", obj.totalSize(),s.size());
			//auto words = capnp::messageToFlatArray(obj);
			//mprintf(L"Actual size =%d\n",words.)

		}
	}
	uint64_t size = capnp::computeSerializedSizeInWords(message) << 3;
	mprintf(L"Builder time = %d ms\t Size = %d bytes\n", t.duration(), size);
	writeMessage(message, name);
}

