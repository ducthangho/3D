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

#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"
//SIMPLE TYPE


#include <impexp.h>


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

inline void VertexSelect(ObjectWrapper& o, BitArray& sels) {
	o.SetVertSel(sels);
}


inline void VertexSelect(MNMesh& mesh, BitArray& sels) {
	mesh.VertexSelect(sels);
}

inline void VertexSelect(Mesh& mesh, BitArray& sels) {
	mesh.vertSel = sels;
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


inline BitArray& getEdgeVis(MNMesh& in, BitArray& sels) {
	sels.SetSize(in.nume);
	int k = 0;
	for (int i = 0; i < in.numf; ++i) {
		auto& f = in.f[i];
		for (int j = 0; j < f.deg; ++j)
			if (f.visedg[j])
				sels.Set(k++);
			else sels.Clear(k++);
	}
	return sels;
}

inline BitArray& getEdgeVis(Mesh& in, BitArray& sels) {	
	//in.BuildVisEdgeList();
	sels.SetSize(in.numFaces*3);
	int k = 0;
	for (int i = 0; i < in.numFaces; ++i) {
		auto& f = faceRef(in,i);
		for (int j = 0; j < 3; ++j) {
			if (f.getEdgeVis(j)) {
				sels.Set(k++);
				//if (i < 10) mprintf(L"(%d, %d, %d)\n", i, j, j + 1);
			} 
			else sels.Clear(k++);
		}
	}
	return sels;
}

inline BitArray& getEdgeVis(ObjectWrapper& in, BitArray& sels) {
	auto* mesh = in.GetPatchMesh();
	if (mesh) {
		sels.SetSize(in.NumEdges());		
		sels.SetAll();
		return sels;
	}


	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {		
		return getEdgeVis(*pmesh,sels);
	}
	auto* tmesh = in.GetTriMesh();
	if (tmesh) return getEdgeVis(*tmesh, sels);
	return sels;
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

inline void EdgeVis(MNMesh& mesh, BitArray& sels) {
	int k = 0;
	for (int i = 0; i < mesh.numf; ++i) {
		auto& f = faceRef(mesh, i);
		//mprintf(L"Face %d:\t", i);
		for (int j = 0; j < f.deg; ++j) {
			if (sels[k++]) {
				f.visedg.Set(j);
			} else f.visedg.Clear(j);
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
				f.setEdgeVis(j,EDGE_VIS);
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
inline void loadV(V_IN& vi, V_OUT& vo) {
	vo.orig = vi.getOrig();
	vo.p.x = vi.getX();
	vo.p.y = vi.getY();
	vo.p.z = vi.getZ();
	vo.ImportFlags(vi.getFlags());
}


template <class V_IN, class V_OUT>
inline void saveP(V_IN& vi, V_OUT& vo) {
	vo.setX(vi.x);
	vo.setY(vi.y);
	vo.setZ(vi.z);
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
inline void saveE(E_IN& vi, E_OUT& vo) {
	vo.setV1(vi.v1);
	vo.setV2(vi.v2);
	vo.setF1(vi.f1);
	vo.setF2(vi.f2);
	vo.setTrack(vi.track);
	vo.setFlags(vi.ExportFlags());
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

template <class F_OUT>
inline void save3F(IGameMesh& vi, int faceIndex, F_OUT& vo) {
	vo.setV0(vi.GetFaceVertex(faceIndex,0));
	vo.setV1(vi.GetFaceVertex(faceIndex, 1));
	vo.setV2(vi.GetFaceVertex(faceIndex, 2));

	vo.setSmGroup(vi.GetFaceSmoothingGroup(faceIndex) );
	vo.setMaterial(vi.GetFaceMaterialID(faceIndex));
}

inline void save3F(MNFace& vi,  format3d::QuadFace::Builder& vo) {
	vo.setV0(vi.vtx[0]);
	vo.setV1(vi.vtx[1]);
	vo.setV2(vi.vtx[2]);
	vo.setV3(-1);

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.material);
}

inline void save3F(MNFace& vi, format3d::TriFace::Builder& vo) {
	vo.setV0(vi.vtx[0]);
	vo.setV1(vi.vtx[1]);
	vo.setV2(vi.vtx[2]);	

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.material);
}

inline void save3F(GenFace& vi, format3d::TriFace::Builder& vo) {
	vo.setV0(vi.verts[0]);
	vo.setV1(vi.verts[1]);
	vo.setV2(vi.verts[2]);	
}

inline void save3F(GenFace& vi, format3d::QuadFace::Builder& vo) {
	vo.setV0(vi.verts[0]);
	vo.setV1(vi.verts[1]);
	vo.setV2(vi.verts[2]);
	vo.setV3(-1);
}

inline void save4F(GenFace& vi, format3d::TriFace::Builder& vo) {
	vo.setV0(vi.verts[0]);
	vo.setV1(vi.verts[1]);
	vo.setV2(vi.verts[2]);
}

inline void save4F(GenFace& vi, format3d::QuadFace::Builder& vo) {
	vo.setV0(vi.verts[0]);
	vo.setV1(vi.verts[1]);
	vo.setV2(vi.verts[2]);
	vo.setV3(vi.verts[3]);
}


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


inline void save3F(Face& vi, format3d::QuadFace::Builder& vo) {
	vo.setV0(vi.v[0]);
	vo.setV1(vi.v[1]);
	vo.setV2(vi.v[2]);
	vo.setV3(-1);

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.getMatID());
}

inline void save3F(Face& vi, format3d::TriFace::Builder& vo) {
	vo.setV0(vi.v[0]);
	vo.setV1(vi.v[1]);
	vo.setV2(vi.v[2]);	

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.getMatID());
}


template <class F_IN>
inline void load3F(F_IN& vi, Face& vo) {
	vo.v[0] = vi.getV0();
	vo.v[1] = vi.getV1();
	vo.v[2] = vi.getV2();
	vo.smGroup = vi.getSmGroup();
	vo.setMatID(vi.getMaterial());
}



template <class F_OUT>
inline void save4F(IGameMesh& vi, F_OUT& vo,int faceIndex) {
	vo.setV0(vi.GetFaceVertex(faceIndex, 0));
	vo.setV1(vi.GetFaceVertex(faceIndex, 1));
	vo.setV2(vi.GetFaceVertex(faceIndex, 2));
	vo.setV3(vi.GetFaceVertex(faceIndex, 3));

	vo.setSmGroup(vi.GetFaceSmoothingGroup(faceIndex));
	vo.setMaterial(vi.GetFaceMaterialID(faceIndex));
}

template <class F_OUT>
inline void save4F(MNFace& vi, F_OUT& vo) {
	vo.setV0(vi.vtx[0]);
	vo.setV1(vi.vtx[1]);
	vo.setV2(vi.vtx[2]);
	vo.setV3(vi.vtx[3]);

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.material);
}

template <class Reader, class Face>
inline void load4F(Reader& vi, Face& vo) {

}

template <>
inline void load4F(format3d::QuadFace::Reader& vi, MNFace& vo) {
	vo.SetAlloc(4);
	vo.SetDeg(4);
	vo.vtx[0] = vi.getV0();
	vo.vtx[1] = vi.getV1();
	vo.vtx[2] = vi.getV2();
	vo.vtx[3] = vi.getV3();
	vo.smGroup = vi.getSmGroup();
	vo.material = vi.getMaterial();
}

template <>
inline void load4F(format3d::TriFace::Reader& vi, MNFace& vo) {
	vo.SetAlloc(3);
	vo.SetDeg(3);
	vo.vtx[0] = vi.getV0();
	vo.vtx[1] = vi.getV1();
	vo.vtx[2] = vi.getV2();	
	vo.smGroup = vi.getSmGroup();
	vo.material = vi.getMaterial();
}


template <class F_IN, class F_OUT>
inline void saveF(F_IN& vi, F_OUT& vo) {
	vo.setFlags(vi.ExportFlags());
	int deg = vi.deg;
	vo.setDeg(deg);

	if (deg > 0) {
		auto vtx = vo.initVtx(deg);
		for (int i = 0; i < deg; ++i) {
			vtx.set(i, vi.vtx[i]);
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

template <class F_IN, class F_OUT>
inline void saveStrippedF(F_IN& vi, F_OUT& vo) {
	int deg = vi.deg;
	vo.setDeg(deg);

	if (deg > 0) {
		auto vtx = vo.initVtx(deg);
		for (int i = 0; i < deg; ++i) {
			vtx.set(i, vi.vtx[i]);
		}


	}

	vo.setSmGroup(vi.smGroup);
	vo.setMaterial(vi.material);
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
inline void loadStrippedF(F_IN& vi, F_OUT& vo) {
	int deg = vi.getDeg();
	vo.Init();

	if (deg > 0) {
		vo.SetAlloc(deg);
		vo.SetDeg(deg);

		auto& vtx = vo.vtx;
		auto ivtx = vi.getVtx();
		loadArr(ivtx, vtx);
	}


	vo.smGroup = vi.getSmGroup();
	vo.material = vi.getMaterial();
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
	//mprintf(L"Nothing\n");
	return -1;
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
inline void savePolyObjStripped(PolyObject& o, const char* name, Message& obj) {
	auto& mesh = o.GetMesh();
	obj.setType(POLYOBJ_T);
	obj.setName(name);
	auto meshCap = obj.initMmesh();
	saveMNMeshStripped(mesh, meshCap);
	saveMNMeshUVW(o, mesh, meshCap);
	//mprintf(L"Saving poly obj\n");
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

template <class Obj, class Message>
inline void saveObj(Obj& o, const char* name, Message& message, bool stripped = true) {
	if (o.IsSubClassOf(polyObjectClassID)) {
		auto& mesh = o.GetMesh();
		if (stripped) {
			auto obj = message.initRoot<format3d::PolyObjStripped>();
			savePolyObjStripped((PolyObject&)o, name, obj);
		}
		else {
			auto obj = message.initRoot<format3d::PolyObj>();
			savePolyObj((PolyObject&)o, name, obj);
		}

	}
}//*/

template <class Message>
inline std::wstring loadPolyObjStripped(PolyObject& ob, Message& obj) {
	std::wstring name;
	auto& mesh = ob.GetMesh();
	std::string s(obj.getName().cStr());
	name = s2ws(s);
	auto meshCap = obj.getMmesh();
	loadMeshStripped(mesh, meshCap);

	return name;
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


template <class Obj, class Message>
inline std::wstring loadObj(Obj& o, Message& message, bool stripped = true,int options = ExportOpts::all) {

	if (o.IsSubClassOf(polyObjectClassID)) {
		PolyObject& ob = (PolyObject&)o;
		auto& mesh = ob.GetMesh();
		if (stripped) {
			auto obj = message.getRoot<format3d::PolyObjStripped>();
			return loadPolyObjStripped(ob, obj);
		}
		else {
			auto obj = message.getRoot<format3d::PolyObj>();
			return loadPolyObj(ob, obj);
		}
	}
	return L"";
}//*/



template <class MNMESH_IN, class MNMESH_OUT>
inline void saveMNMesh(MNMESH_IN& in, MNMESH_OUT& out) {
	auto v = out.initV(in.numv);
	for (int i = 0; i < in.numv; ++i) {
		saveV(in.v[i], v[i]);
	}

	/*auto e = out.initE(in.nume);
	for (int i = 0; i < in.nume; ++i) {
	saveE(in.e[i], e[i]);
	}//*/

	auto f = out.initF(in.numf);
	for (int i = 0; i < in.numf; ++i) {
		saveF(in.f[i], f[i]);
	}


	out.setSelLevel(in.selLevel);
	out.setDispFlags(in.dispFlags);

	BitArray sels;
	in.getVertexSel(sels);
	auto vSel = out.initVSel(sels.GetSize());
	copyArr(sels, vSel);

	in.getEdgeSel(sels);
	auto eSel = out.initESel(sels.GetSize());
	copyArr(sels, eSel);

	in.getFaceSel(sels);
	auto fSel = out.initFSel(sels.GetSize());
	copyArr(sels, fSel);

	Box3 bb;
	in.BBox(bb);
	auto box = out.initBdgBox();
	auto pmin = box.initPmin();
	auto pmax = box.initPmax();
	pmin.setX(bb.pmin.x);
	pmin.setY(bb.pmin.y);
	pmin.setZ(bb.pmin.z);

	pmax.setX(bb.pmax.x);
	pmax.setY(bb.pmax.y);
	pmax.setZ(bb.pmax.z);

	auto edSupport = out.initEdSupport(in.edSupport.GetSize());
	copyArr(in.edSupport, edSupport);

	auto vdSupport = out.initVdSupport(in.vdSupport.GetSize());
	copyArr(in.vdSupport, vdSupport);

	if (in.vd) {
		auto vd = out.initVd(vdSupport.size());//Per vertex data				

		for (int i = 0; i < vd.size(); ++i) {
			if (in.vDataSupport(i)) {
				auto item = vd[i];
				auto & vdi = in.vd[i];
				savePerData(vdi, item);
			}
		}




	}

	if (in.ed) {
		auto ed = out.initEd(edSupport.size());//Per Edge data				

		for (int i = 0; i < ed.size(); ++i) {
			if (in.eDataSupport(i)) {
				auto item = ed[i];
				auto & edi = in.ed[i];
				savePerData(edi, item);
			}
		}
	}

	in.SpecifyNormals();

	auto* normalSpec = in.GetSpecifiedNormals();
	if (normalSpec) {
		normalSpec->BuildNormals();
		auto normalSpecOut = out.initNormals();
		auto mpNormal = normalSpecOut.initMpNormal(normalSpec->GetNumNormals());
		for (int i = 0; i < normalSpec->GetNumNormals(); i++)
		{
			auto item = mpNormal[i];
			auto nrm = normalSpec->Normal(i).Normalize();
			saveP(nrm, item);
		}

		auto mpFace = normalSpecOut.initMpFace(normalSpec->GetNumFaces());
		for (int i = 0; i < mpFace.size(); ++i) {
			auto& face = normalSpec->Face(i);
			int num = getNormalFaceDegree(face);
			if (num == 3 || num == 4) {
				auto item = mpFace.init(i, num);
				for (int j = 0; j < item.size(); ++j) {
					item.set(j, face.GetNormalID(j));
					//mprintf(L"Face %d:  %d\n",i,item[j]);
				}
			} else mpFace.init(i, 0);
		}//*/
	}
	else mprintf(L"None *********\n");
}

template <class MNMESH_IN, class MNMESH_OUT>
inline void saveMNMeshStripped(MNMESH_IN& in, MNMESH_OUT& out) {
	auto v = out.initV(in.numv);
	for (int i = 0; i < in.numv; ++i) {
		saveP(in.v[i].p, v[i]);
	}

	/*auto e = out.initE(in.nume);
	for (int i = 0; i < in.nume; ++i) {
	saveE(in.e[i], e[i]);
	}//*/

	auto f = out.initF(in.numf);
	for (int i = 0; i < in.numf; ++i) {
		saveStrippedF(in.f[i], f[i]);
	}


	out.setSelLevel(in.selLevel);
	out.setDispFlags(in.dispFlags);

	BitArray sels;
	in.getVertexSel(sels);
	auto vSel = out.initVSel(sels.GetSize());
	copyArr(sels, vSel);

	in.getEdgeSel(sels);
	auto eSel = out.initESel(sels.GetSize());
	copyArr(sels, eSel);

	in.getFaceSel(sels);
	auto fSel = out.initFSel(sels.GetSize());
	copyArr(sels, fSel);

	Box3 bb;
	in.BBox(bb);
	auto box = out.initBdgBox();
	auto pmin = box.initPmin();
	auto pmax = box.initPmax();
	pmin.setX(bb.pmin.x);
	pmin.setY(bb.pmin.y);
	pmin.setZ(bb.pmin.z);

	pmax.setX(bb.pmax.x);
	pmax.setY(bb.pmax.y);
	pmax.setZ(bb.pmax.z);

	in.SpecifyNormals();

	auto* normalSpec = in.GetSpecifiedNormals();
	if (normalSpec) {
		normalSpec->BuildNormals();
		auto mpNormal = out.initMpNormal(normalSpec->GetNumNormals());
		for (int i = 0; i < normalSpec->GetNumNormals(); i++)
		{
			auto item = mpNormal[i];
			auto nrm = normalSpec->Normal(i).Normalize();
			saveP(nrm, item);
		}

		auto mpFace = out.initMpFace(normalSpec->GetNumFaces());
		for (int i = 0; i < mpFace.size(); ++i) {
			auto& face = normalSpec->Face(i);
			auto item = mpFace.init(i, in.f[i].deg);
			for (int j = 0; j < item.size(); ++j) {
				item.set(j, face.GetNormalID(j));
				//mprintf(L"Face %d:  %d\n",i,item[j]);
			}
		}//*/
	}
	else mprintf(L"None *********\n");
}

template <class MNMESH_OUT>
inline void storeFaces(MNMesh& in, MNMESH_OUT& out) {

	auto faces = out.getFaces();
	auto f4 = faces.initF4(in.numf);
	for (int i = 0; i < in.numf; ++i) {
		auto& f = in.f[i];
		auto item = f4[i];
		if (f.deg == 3) {
			save3F(f, item);			
		}
		else if (f.deg == 4){
			save4F(f, item);			
		}
	}
}

template <class MNMESH_OUT>
inline void storeFaces(ObjectWrapper& in, MNMESH_OUT& out) {

	auto faces = out.getFaces();
	auto f4 = faces.initF4(in.NumFaces());
	for (int i = 0; i < in.NumFaces(); ++i) {
		auto f = in.GetFace(i);
		auto item = f4[i];
		if (f.numVerts == 3) {
			save3F(f, item);
			item.setSmGroup(in.GetSmoothingGroup(i));
			item.setMaterial(in.GetMtlID(i));
		}
		else if (f.numVerts == 4) {
			save4F(f, item);
			item.setSmGroup(in.GetSmoothingGroup(i));
			item.setMaterial(in.GetMtlID(i));
		}
	}


}


template <class MNMESH_OUT>
inline void storeFaces(Mesh& in, MNMESH_OUT& out) {	
	auto faces = out.getFaces();
	auto f3 = faces.initF3(in.numFaces);
	//mprintf(L"Saving %d faces\n", in.numFaces);
	for (int i = 0; i < in.numFaces; ++i) {
		auto& f = faceRef(in,i);
		auto item = f3[i];		
		save3F(f, item);		
		/*if (i < 10) {
			mprintf(L"f %d\t%d\t%d\n", item.getV0(), item.getV1(), item.getV2());
		}*/
	}
}

inline void setNormalFaceDegree(MNNormalFace & f, int deg) {
	f.SetDegree(deg);
}

inline int getNormalFaceDegree(MNNormalFace & f) {
	return f.GetDegree();
}

inline void setNormalFaceDegree(MeshNormalFace & f, int deg) {
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
inline void buildNormals(M& in, O& out) {
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
						mpFace.set(i*type + j, face.GetNormalID(j));
					else {
						//item.set(j, -1);//*/
						mpFace.set(i*type + j, -face.GetNormalID(j));
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
inline void buildNormals(ObjectWrapper& in, O& out) {	
	auto* mesh = in.GetTriMesh();
	if (mesh) {
		buildNormals(*mesh, out);
		return;
	}
	auto* pmesh = in.GetPolyMesh();
	if (pmesh) {
		buildNormals(*pmesh, out);
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


template <class M,class O>
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
					auto last_item = mpFace[idx+type-1];
					auto& f = normalSpecOut->Face(i);

					int deg = (last_item == LONG_MIN) ? 3 : type;
					setNormalFaceDegree(f, deg);
					
					//mprintf(L"Set degree %d\n", face.size());
					for (int j = 0; j < deg; ++j) {
						
						//normalSpecOut->SetNormalIndex(i, j, face[j]);
						int val = mpFace[idx+j];
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
inline void storeMesh(MNMESH_IN& in, MNMESH_OUT& out, int options = ExportOpts::all,  Box3* bbox = nullptr) {
	BitArray sels;
	
	if (options & ExportOpts::mesh) {
		int numv = numVerts(in);
		auto v = out.initV(numv);
		//mprintf(L"Store %d num verts\n", numv);
		for (int i = 0; i < numv; ++i) {
			saveP(vert(in, i), v[i]);
			/*if (i < 10) {
				printPts(vert(in, i), v[i]);
			}*/
		}
		//mprintf(L"---------------\n");
		storeFaces(in, out);
		//mprintf(L"---------------\n");


		//testEdge(in);
		
		getEdgeVis(in, sels);
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
	
	if (options & ExportOpts::normals) buildNormals(in, out);
	
}



template <class MNMAP_OUT>
inline void saveUVW(MNMap& in, MNMAP_OUT& out) {
	//out.setFlags(in.ExportFlags());

	auto v = out.initV(in.numv);
	for (int i = 0; i < v.size(); ++i) {
		auto item = v[i];
		saveP(in.v[i], item);
	}

	auto f = out.initF(in.numf * 4);
	for (int i = 0; i < in.numf; ++i) {
		//auto fi = f.init(i, in.f[i].deg);
		int idx = i << 2;//i*4
		int deg = in.f[i].deg;
		auto& inf = in.f[i];

		for (int j = 0; j < deg; ++j) {
			f.set(idx++, inf.tv[j]);
		}

		if (deg == 3) f.set(idx, -1);
	}

}

template <class MNMAP_OUT>
inline void saveUVW(MeshMap& in, MNMAP_OUT& out) {
	//out.setFlags(in.ExportFlags());

	auto v = out.initV(in.vnum);
	//mprintf(L"Saving %d UV verts\n", in.vnum);
	for (int i = 0; i < v.size(); ++i) {
		auto item = v[i];
		saveP(in.tv[i], item);
	}

	auto f = out.initF(in.fnum * 3);
	//mprintf(L"Saving %d UV faces\n------------------\n", in.fnum);
	for (int i = 0; i < in.fnum; ++i) {
		int idx = i * 3;//i*3
		auto& inf = in.tf[i];
		//copyArr(in.tf[i].t, f,i*3);
		for (int j = 0; j < 3; ++j) {
			f.set(idx++, inf.t[j]);
		}
	}

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

template <class ITEM>
inline void copyUVFace(ITEM& item,MNMapFace& fo, int srcIdx = 0, int deg =3) {
	fo.deg = deg;
	fo.SetSize(deg);
	if (fo.tv)
		for (int j = 0; j < deg; ++j) {
			fo.tv[j] = item[j+ srcIdx];
		}
}

template <class ITEM>
inline void copyUVFace(ITEM& item, TVFace& fo, int srcIdx = 0, int deg = 3) {
	for (int j = 0; j < 3; ++j) {
		fo.t[j] = item[j+ srcIdx];
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
	
	setUVNumFaces(out, numFaces);
	//mprintf(L"Set MeshMap face num = %d\n", fi.size());
	auto* f = getUVFaces(out);

	if (f) {
		for (int i = 0; i < numFaces; ++i) {
			int idx = i*type;
			int item = fi[idx+type-1];
			int deg = (item == -1) ? 3 : type;
			auto& fo = f[i];
			/*fo.deg = item.size();
			fo.SetSize(fo.deg);
			if (fo.tv)
				for (int j = 0; j < fo.deg; ++j) {
					fo.tv[j] = item[j];
				}//*/
			copyUVFace(fi, fo,idx,deg);
		}
	}
	else mprintf(L"Empty F\n");
	//*/
}



template <class MNMESH_OUT>
inline void saveMNMeshUVW(PolyObject& o, MNMesh& in, MNMESH_OUT& out) {
	if (!o.HasUVW()) return;
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
			saveUVW(*ptr, map);
		}
	}

	/*mprintf(L"Aha\n");
	channels.clear();
	for (int i = -NUM_HIDDENMAPS; i < 0; ++i) {
		//if (o.HasUVW(i)) {
			auto* ptr = in.M(i);
			if (ptr && !ptr->GetFlag(MN_DEAD)) {
				channels.emplace_back(i);
			}
		//}
	}

	mprintf(L"Hidden Channel %d\n", channels.size());

	auto hmap = out.initHmap(channels.size());
	for (int i = 0; i < channels.size(); ++i) {
		auto channel = channels[i];
		auto* ptr = in.M(channel);
		if (ptr) {
			auto map = hmap[i];
			map.setChannel(channel);
			saveUVW(*ptr, map);
		}
	}//*/

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
inline void saveMNMeshUVW(Obj& o, Mesh& in, MNMESH_OUT& out) {
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
		saveUVW(ptr, map);		
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
inline void saveMNMeshUVW(ObjectWrapper& o, Mesh& in, MNMESH_OUT& out) {
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
		saveUVW(ptr, map);
	}
}


template <class Obj>
inline void saveMesh(Obj& o, ::capnp::MallocMessageBuilder& message, int options = ExportOpts::all) {
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
	saveMesh(o, meshCap, options);
}

/*template <class Obj>
inline void saveMesh(Obj& o, format3d::MNMeshStripped::Builder& meshCap, int options = ExportOpts::all) {
	if (o.IsSubClassOf(polyObjectClassID)) {
		auto& mesh = getMesh(o);
		saveMNMeshStripped(mesh, meshCap);
		saveMNMeshUVW(o, mesh, meshCap);
	}
}//*/

/*template <class Obj>
inline void saveMesh(Obj& o, format3d::MNMesh::Builder& meshCap) {
	mprintf(L"Saving general object\n");
	if (o.IsSubClassOf(polyObjectClassID)) {
		auto& mesh = getMesh(o);
		saveMNMesh(mesh, meshCap);
		saveMNMeshUVW(o, mesh, meshCap);
	}
}//*/


inline void saveMesh(PolyObject& o, ::format3d::Mesh::Builder& meshCap, int options = ExportOpts::all) {
	MNMesh& mesh = getMesh(o);
	//mprintf(L"Saving poly object\n");
	storeMesh(mesh, meshCap, options);
	if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
}


inline void saveMesh(TriObject& o, ::format3d::Mesh::Builder& meshCap, int options = ExportOpts::all) {
	Mesh& mesh = getMesh(o);
	//mprintf(L"Saving tri object\n");
	storeMesh(mesh, meshCap, options);
	if (options & ExportOpts::uv)saveMNMeshUVW(o, mesh, meshCap);
}


inline void saveMesh(ObjectWrapper& o, ::format3d::Mesh::Builder& meshCap, int options = ExportOpts::all) {
	//mprintf(L"Saving wrapper object\n");
	auto& mesh = getMesh(o);	
	storeMesh(o, meshCap, options);
	if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
}


inline void saveMesh(PatchObject& o, ::format3d::Mesh::Builder& meshCap, int options = ExportOpts::all) {
	Mesh& mesh = getMesh(o);
	//mprintf(L"Saving patch object\n");
	storeMesh(mesh, meshCap, options);
	if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
}


inline void saveMesh(Mesh& mesh, ::format3d::Mesh::Builder& meshCap, int options = ExportOpts::all) {
	//mprintf(L"Saving Mesh\n");
	storeMesh(mesh, meshCap, options);
	//if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
}

inline void saveMesh(MNMesh& mesh, ::format3d::Mesh::Builder& meshCap, int options = ExportOpts::all) {
	//mprintf(L"Saving MNMesh\n");
	storeMesh(mesh, meshCap, options);
	//if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
}

inline void saveMesh(Object& o, ::format3d::Mesh::Builder& meshCap, int options = ExportOpts::all) {

	if (o.IsSubClassOf(polyObjectClassID)) {
		PolyObject& obj = (PolyObject&)(o);
		saveMesh(obj, meshCap, options);
	}
	else if (o.IsSubClassOf(triObjectClassID)) {
		TriObject& obj = (TriObject&)(o);
		saveMesh(obj, meshCap, options);
	}
	else if (o.IsSubClassOf(patchObjectClassID)) {
		PatchObject& obj = (PatchObject&)(o);
		saveMesh(obj, meshCap, options);
	}
	else {
		Mesh& mesh = getMesh(o);
		//mprintf(L"Saving Object\n");
		storeMesh(mesh, meshCap, options);
		if (options & ExportOpts::uv) saveMNMeshUVW(o, mesh, meshCap);
	}
	
}


template <class Obj, class Message>
inline void load(Obj& obj, Message& msg, int options = ExportOpts::all) {
	mprintf(L"Not implemented yet\n");
}

template <class Obj, class Message>
inline void loadStripped(Obj& obj, Message& msg) {
	mprintf(L"Not implemented yet\n");
}

template <>
inline void load<MNMesh, capnp::InputStreamMessageReader>(MNMesh& mesh, capnp::InputStreamMessageReader& msg, int options) {
	//mprintf(L"Finally here\n");
	auto meshCap = msg.getRoot<format3d::MNMesh>();
	//printMNMesh(mesh, meshCap);
	//mprintf(L"Got root here\n");
}

template <>
inline void load<MNMap, format3d::MNMap::Reader>(MNMap& mesh, format3d::MNMap::Reader& msg, int options) {

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
			load3F(item,f);
		}
	}
	else {
		//mprintf(L"Reading MNMesh faces\n");
		auto f4 = faces.getF4();
		auto size = f4.size();
		//mprintf(L"Num MNMesh faces = %d\n", f4.size());
		mesh.setNumFaces(size);
		int j = 0;
		for (int i = 0; i < size; ++i) {
			auto item = f4[i];
			auto &f = faceRef(mesh, i);
			if (item.getV3() == -1) {
				load3F(item, f);
				j++;
			}
			else load4F(item, f);
		}
		//mprintf(L"Trifaces = %d\tquad faces = %d\n", j, f4.size() - j);
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
		auto f4 = faces.getF4();
		auto size = f4.size();
		//mprintf(L"Num MNMesh faces = %d\n", f4.size());
		mesh.setNumFaces(size);
		int j = 0;
		for (int i = 0; i < size; ++i) {
			auto item = f4[i];
			auto &f = faceRef(mesh, i);			
			load3F(item, f);
			/*if (i < 10) {
				mprintf(L"f %d\t%d\t%d\t%d\n", item.getV0(), item.getV1(), item.getV2(),item.getV3());
			}*/
		}
		//mprintf(L"Trifaces = %d\tquad faces = %d\n", j, f4.size() - j);
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
}

inline void setMapSupport(Mesh& m, int channel) {
	m.setMapSupport(channel, TRUE);
}

inline void checkData(MNMesh& mesh) {
	if (!mesh.CheckAllData())
		mprintf(L"Something is wrong\n");
	mprintf(L"Got root here\n");//*/
}

inline void checkData(Mesh& mesh) {};

template <class M,class Message>
inline void readMesh(M& mesh, Message& meshCap,int options = ExportOpts::all) {

	BitArray sels;
	if (options & ExportOpts::mesh) {
		Time t;
		//mprintf(L"Finally ReadMesh here\n");
		auto v = meshCap.getV();

		mesh.setNumVerts(v.size());
		//mprintf(L"%d verts have been loaded\n", v.size());

		for (int i = 0; i < v.size(); ++i) {
			auto item = v[i];
			auto &p = vert(mesh, i);
			loadP(item, p);
			//p.ImportFlags(item.getFlags());
		};

		/*auto f3 = meshCap.getF3();
		auto f4 = meshCap.getF4();
		size_t size = f3.size() + f4.size();
		mesh.setNumFaces(size);
		mprintf(L"Face num read %d    %d   %d \n", size,f3.size(),f4.size());

		for (int i = 0; i < f3.size(); ++i) {
			auto face = f3[i];
			auto& oface = faceRef(mesh,i);
			load3F(face, oface);
		};

		int j = f3.size();
		for (int i = 0; i < f4.size(); ++i) {
			auto face = f4[i];
			auto& oface = faceRef(mesh, j++);
			load4F(face, oface);
		};//*/

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
		//mesh.checkNormals(TRUE);	
	}
	
	//*/
	//printMNMesh(mesh, meshCap);
	mesh.selLevel = meshCap.getSelLevel();
	mesh.dispFlags = meshCap.getDispFlags();
	//mprintf(L"Now getting selections\n");
	//checkData(mesh);
}


template <>
inline void load<MNMesh, capnp::FlatArrayMessageReader>(MNMesh& mesh, capnp::FlatArrayMessageReader& msg, int options) {
	mprintf(L"Finally here\n");
	auto meshCap = msg.getRoot<format3d::MNMesh>();
	loadMesh(mesh, meshCap,options);
}


template <class Message>
inline void loadMeshStripped(MNMesh& mesh, Message& meshCap) {
	auto v = meshCap.getV();

	mesh.setNumVerts(v.size());
	for (int i = 0; i < v.size(); ++i) {
		auto item = v[i];
		loadP(item, mesh.v[i].p);
	};


	auto f = meshCap.getF();
	mesh.setNumFaces(f.size());
	//mprintf(L"Face num read %d     %d\n", f.size(), mesh.numf);

	for (int i = 0; i < f.size(); ++i) {
		auto face = f[i];
		auto& oface = mesh.f[i];
		loadStrippedF(face, oface);
	};

	//mesh.ClearFlag(MN_MESH_FILLED_IN);
	mesh.FillInMesh();//calculate edges

	mesh.selLevel = meshCap.getSelLevel();
	mesh.dispFlags = meshCap.getDispFlags();

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


	auto mpNormal = meshCap.getMpNormal();
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

			auto mpFace = meshCap.getMpFace();
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

	if (!mesh.CheckAllData())
		mprintf(L"Something is wrong\n");
	mprintf(L"Got root here\n");
}




template <>
inline void loadStripped<MNMesh, capnp::FlatArrayMessageReader>(MNMesh& mesh, capnp::FlatArrayMessageReader& msg) {
	mprintf(L"Finally here\n");
	auto meshCap = msg.getRoot<format3d::MNMeshStripped>();
	loadMeshStripped(mesh, meshCap);
}


template <>
inline void loadStripped<PolyObject, capnp::FlatArrayMessageReader>(PolyObject& obj, capnp::FlatArrayMessageReader& msg) {
	auto name = loadObj(obj, msg, true);
	//mprintf(L"Loaded object %s\n", name.c_str());
}

template <>
inline void load(PolyObject& obj, capnp::FlatArrayMessageReader& msg, int options) {
	auto name = loadObj(obj, msg, false,options);
	//mprintf(L"Loaded object %s\n", name.c_str());
}



inline INode* CreateObjectInScene(SClass_ID sid, Class_ID cid) {
	Time t;
	Object* obj = static_cast<Object*>(CreateInstance(sid, cid));	
	time(t, CREATE_NODE);
	auto* node = GetCOREInterface()->CreateObjectNode(obj);
	return node;
}

inline PolyObject* createPolyObject() {
	return static_cast<PolyObject*>(CreateInstance(GEOMOBJECT_CLASS_ID, polyObjectClassID));
}

inline TriObject* createTriObject() {
	return static_cast<TriObject*>(CreateInstance(GEOMOBJECT_CLASS_ID, triObjectClassID));
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


template <class OBJ>
inline void saveObject(OBJ& o, format3d::Object::Builder& obj, char* name, int options = ExportOpts::all) {
	//o.LockObject();
	//mprintf(L"Is object locked %d\n", o.IsObjectLocked());
	auto& mesh = getMesh(o);
	//mprintf(L"Save !!!!!!!! %s\t%d\n",s2ws(name).c_str(),1);
	obj.setType(getType(o));
	obj.setName(name);
	//mprintf(L"Init mesh\n");
	::format3d::Mesh::Builder meshCap = obj.initMesh();
	saveMesh(mesh, meshCap, options);
	//o.UnlockObject();
}

inline void saveObject(INode* node, format3d::Object::Builder& obj, int options = ExportOpts::all) {
	//auto* obj = node->GetIGameObject();
	if (!node) return;
	auto* o = getObject(node);
	std::wstring s(node->GetName());	
	//mprintf(L"Saving object %s\n", s.c_str());
	o->SetAFlag(A_LOCK_TARGET);
	if (o->IsSubClassOf(polyObjectClassID)) {
		PolyObject* ob = (PolyObject*)o;
		saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
	}
	else if (o->IsSubClassOf(triObjectClassID)) {
		TriObject* ob = (TriObject*)o;
		saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
	}
	else if (o->IsSubClassOf(patchObjectClassID)) {
		PatchObject* ob = (PatchObject*)o;
		saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
	}
	else if (o->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) {

		TriObject* ob = (TriObject*)o->ConvertToType(GetCOREInterface()->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));
		//->SetAFlag(A_LOCK_TARGET);
		if (ob) 
			saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
		mprintf(L"MaybeAutoDelete\n");
		if (ob && ob != o) ob->MaybeAutoDelete();
		//ob->UnlockObject();
	}
	o->ClearAFlag(A_LOCK_TARGET);
}




inline void saveObject(IGameNode* node, format3d::Object::Builder& obj, int options = ExportOpts::all) {
	//auto* obj = node->GetIGameObject();
	if (!node) return;
	auto* o = (node->GetIGameObject()->GetMaxObject());
	std::wstring s(node->GetName());
	o->LockObject();
	if (o->IsSubClassOf(polyObjectClassID)) {
		PolyObject* ob = (PolyObject*)o;
		saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
	}
	else if (o->IsSubClassOf(triObjectClassID)) {
		TriObject* ob = (TriObject*)o;
		saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
	}
	else if (o->IsSubClassOf(patchObjectClassID)) {
		PatchObject* ob = (PatchObject*)o;		
		saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
	}
	else if (o->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) {

		TriObject* ob = (TriObject*)o->ConvertToType(GetCOREInterface()->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));
		if (ob) {
			saveObject(*ob, obj, (char*)ws2s(s).c_str(), options);
			if (ob != o) ob->MaybeAutoDelete();
		}
	}
	o->UnlockObject();
}


template <class Obj>
inline void saveObject(Obj& o, ::capnp::MallocMessageBuilder& m, char* name, int options = ExportOpts::all) {
	//auto& mesh = getMesh(o);
	auto root = m.initRoot<format3d::Anything>();
	auto items = root.getItems();
	auto obj = items.initObj();
	obj.setType(getType(o));
	obj.setName(name);
	::format3d::Mesh::Builder meshCap = obj.initMesh();
	saveMesh(o, meshCap, options);
}


inline void saveObject(IGameNode* node, ::capnp::MallocMessageBuilder& m, int options = ExportOpts::all) {
	auto root = m.initRoot<format3d::Anything>();
	auto items = root.getItems();
	auto obj = items.initObj();
	saveObject(node, obj, options);
}

//inline std::wstring getName(IGameNode* node) {
//	std::wstring s(node->GetName());
//	return s;
//}




template <class OBJ>
inline void saveObject(OBJ& o, format3d::Anything::Builder& msg, char* name, int options = ExportOpts::all) {
	//o.LockObject();
	auto& mesh = getMesh(o);
	//o.UnlockObject();
	mprintf(L"Save !!!!!!!!\n");
	auto items = msg.getItems();
	auto obj = items.initObj();
	saveObject(o, obj, name, options);
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

	//theHold.Begin();
	//if (theHold.Holding())
	//{
	//theHold.Put(new ObjManipulatorRestore);
	//theHold.Put(new CollapseRestore());
	////theHold.Put(new UpdateUIRestore());
	//}
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

		/*if (theHold.Holding())
		{
		theHold.Put(new CollapseRestore(TRUE));
		theHold.Put(new ObjManipulatorRestore(TRUE));
		}
		theHold.Accept(GetString(IDS_COLLAPSE));*/
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

		/*if (theHold.Holding())
		{
			theHold.Put(new CollapseRestore(TRUE));
			theHold.Put(new ObjManipulatorRestore(TRUE));
		}
		theHold.Accept(GetString(IDS_COLLAPSE));//*/
	}
}


inline INode* createOrConvert(INode* node, int8_t type) {
	Time t;
	auto currentType = getType(node);
	BOOL deleteIt = FALSE;
	//ICustAttribCollapseManager * iCM = ICustAttribCollapseManager::GetICustAttribCollapseManager();
	if (type == POLYOBJ_T) {
		if (!node) {
			//mprintf(L"Created new PolyObject node\n");			
			node = CreateObjectInScene(GEOMOBJECT_CLASS_ID, polyObjectClassID);			
			//IGameScene * pIgame = GetIGameInterface();
			//pIgame->InitialiseIGame(node);
			
		}
		else {			
			//GetCOREInterface()->CollapseNode(node, TRUE);			
			if (currentType != type) {
				CollapseToType<PolyObject>(node);
			} else Collapse(node);
//			mprintf(L"Collapsed\n");
		};
		time(t, OBJECT_SCENE_CREATE);
		
	} else if (type == TRIOBJ_T) {
		if (!node) {
			//mprintf(L"Created new TriObject node\n");
			
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
	else return nullptr;

	
	return node;
}



inline Object* loadObj(format3d::Object::Reader& obj, INode* node = nullptr, int options = ExportOpts::all) {	
	Time t;
	auto type = obj.getType();	
	auto* newNode = createOrConvert(node, type);
	if (newNode == nullptr) return nullptr;
	
	if (type == POLYOBJ_T) {
		PolyObject* nobj = (PolyObject*)getObject(newNode);
		//nobj->SetAFlag(A_LOCK_TARGET);
		std::wstring name = s2ws(obj.getName().cStr());		
		//mprintf(L"Loading Poly Object %s\n", name.c_str());
		newNode->SetName(name.c_str());
		auto meshCap = obj.getMesh();
		readMesh(nobj->GetMesh(), meshCap, options);
		//nobj->ClearAFlag(A_LOCK_TARGET);
		//nobj->GetMesh().InvalidateTopoCache();
		//nobj->GetMesh().InvalidateGeomCache();
		time(t, OBJECT_CREATION);		
		return nobj;
	} else if (type == TRIOBJ_T) {
		TriObject* nobj = (TriObject*)getObject(newNode);
		//nobj->SetAFlag(A_LOCK_TARGET);
		std::wstring name = s2ws(obj.getName().cStr());
		//mprintf(L"Loading Tri Object %s\n", name.c_str());
		newNode->SetName(name.c_str());
		auto meshCap = obj.getMesh();
		readMesh(nobj->GetMesh(), meshCap, options);
		//nobj->ClearAFlag(A_LOCK_TARGET);
		//nobj->GetMesh().InvalidateTopoCache();
		//nobj->GetMesh().InvalidateGeomCache();
		time(t, OBJECT_CREATION);
		return nobj;
	}
	else if (type == PATCHOBJ_T) {		
	}//*/
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
			mprintf(L"Object name is %s\n",s2ws(s).c_str());
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
		if (s==tmp) {
			//mprintf(L"Found at index %d\n", i);
			return i;
		}
	}
	return -1;
}

template <class Message>
inline void* load(Message& msg,INode* node = nullptr, int options = ExportOpts::all) {	
	auto message = msg.getRoot<format3d::Anything>();
	auto items = message.getItems();	
	if (items.isObj()) {		
		auto obj = items.getObj();
		return loadObj(obj, node, options);
	}
	else if (items.isObjs()) {
		auto objs = items.getObjs();
		if (node==nullptr)
			return (objs.size()>0) ? loadObj(objs[0], node, options) : nullptr;
		if (objs.size() == 0) return nullptr;
		//mprintf(L"List size = %d\n", objs.size());
		int idx = search(*node, objs);
		return (idx!=-1) ? loadObj(objs[idx],node,options) : loadObj(objs[0], node, options);
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
	auto items = message.getItems();
	mprintf(L"loading .... \n");
	if (nodes == nullptr) {
		INodeTab lNodes;
		GetSceneNodes(lNodes,nullptr);
		if (lNodes.Count() == 0) return nullptr;
		return load(msg, &lNodes, options);
	}

	auto& nodesRef = *nodes;

	if (items.isObj()) {
		auto obj = items.getObj();
		if (nodes==nullptr || nodes->Count()==0)
			return loadObj(obj, nullptr, options);	
		return loadObj(obj, nodesRef[0], options);


	}
	else if (items.isObjs()) {
		auto objs = items.getObjs();
		mprintf(L"Loading multiple objects %d  %d\n", objs.size(), nodes->Count());
		if (objs.size() == 0 || nodes->Count()==0) return nullptr;
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
				loadObj(obj, node, options);
			} 
		}

	}

	return		nullptr;

}


template <class Message>
inline void* load(Message& msg, std::vector<std::wstring>* names = nullptr, int options = ExportOpts::all) {
	auto message = msg.getRoot<format3d::Anything>();
	auto items = message.getItems();
	mprintf(L"loading .... \n");
		
	if (items.isObj()) {
		auto obj = items.getObj();
		std::wstring name(s2ws(obj.getName().cStr()));
		auto* node = GetCOREInterface()->GetINodeByName(name.c_str());		
		return loadObj(obj, node, options);
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

		
		for (int i = 0; i < index.size(); ++i) {
			auto idx = index[i];			
			auto obj = objs[idx];
			std::wstring name(s2ws(obj.getName().cStr()));
			auto* node = GetCOREInterface()->GetINodeByName(name.c_str());
			loadObj(obj, node, options);
		}

	}

	return		nullptr;

}

template <class Message>
inline void* load(Message& msg, std::vector<int>* index = nullptr, int options = ExportOpts::all) {
	auto message = msg.getRoot<format3d::Anything>();
	auto items = message.getItems();
	mprintf(L"loading .... \n");

	auto* ip = GetCOREInterface();

	if (items.isObj()) {
		auto obj = items.getObj();
		std::wstring name(s2ws(obj.getName().cStr()));
		auto* node = GetCOREInterface()->GetINodeByName(name.c_str());
		return loadObj(obj, node, options);
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

		std::vector<std::pair<std::wstring, Object*>> list;
		std::map<std::wstring, int> mapNames;

		for (int i = 0; i < idx.size(); ++i) {
			auto ix = idx[i];
			auto obj = objs[ix];
		//	mprintf(L"Index %d\n", ix);
			std::wstring name(s2ws(obj.getName().cStr()));
			//MSTR s(s2ws(obj.getName().cStr()).c_str() );
			auto type = obj.getType();
			auto* node = GetCOREInterface()->GetINodeByName(name.c_str());
			if (node != nullptr && mapNames.find(name)==mapNames.end() ) {
				//mprintf(L"here %s\t%s\n",name.c_str(),node->GetName() );				
				loadObj(obj, node, options);
				mapNames[name] = 0;
			} 
			else 
			{
				//Object* o = (type == POLYOBJ_T) ? (Object*)createPolyObject() : (Object*)createTriObject();
				
				auto meshCap = obj.getMesh();
				if (type == POLYOBJ_T) {
					PolyObject* o = createPolyObject();
					//o->InitNodeName(s);
					readMesh(o->GetMesh(), meshCap, options);					
					list.emplace_back(name, o);
				}
				else {
					TriObject* o = createTriObject();
					//o->InitNodeName(s);
					//mprintf(L"Default node name %s\n", s);
					readMesh(o->GetMesh(), meshCap, options);
					list.emplace_back(name, o);
				}
			}
		}


		std::vector<INode*> inodes(list.size());
		//size_t size = list.size();
		//tbb::parallel_for(tbb::blocked_range<size_t>(0, size), [&list,&ip](const tbb::blocked_range<size_t>& r) {
		//	for (size_t i = r.begin(); i < r.end(); ++i) {
			
		Time t;
		
			for (int i=0;i<list.size();++i){
				auto& item = list[i];
				auto* node = ip->CreateObjectNode(item.second);
				inodes[i] = node;				
				/*std::string s= "Object";
				s += std::to_string(i);				
				node->SetName(s2ws(s).c_str());*/
				//mprintf(L"%s\n", item.first.c_str());
				//mprintf(L"%s\n", L"abcxyz");
			}

			tbb::parallel_for(tbb::blocked_range<size_t>(0, inodes.size()), [&mapNames,&inodes,&list, &ip](const tbb::blocked_range<size_t>& r) {
				for (size_t i = r.begin(); i < r.end(); ++i) {
					//for (int i = 0; i < inodes.size(); ++i) {
					auto& item = list[i];
					auto* node = inodes[i];
					auto name = item.first;
					if (mapNames.find(name) == mapNames.end()) {
						mapNames[name] = 0;
					}
					else {
						auto count = mapNames[name]++;
						name += L"_";
						name += std::to_wstring(count);
					}
					node->SetName(name.c_str());
				}
			});
		//});
			mprintf(L"Create object & set names = %d\n", t.duration());
	}
	
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
	
	void* obj =  (void*)load(msg, container,options);
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

template <class Obj>
inline void saveObject(Obj& o, TCHAR* name, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	//auto& mesh = getMesh(o);
	::capnp::MallocMessageBuilder m;
	auto root = m.initRoot<format3d::Anything>();
	auto items = root.getItems();
	auto obj = items.initObj();
	obj.setType(getType(o));
	std::wstring s(name);
	obj.setName(ws2s(s).c_str());
	s = exportFolder;
	s += name;
	s += L".cap";
	mprintf(L"Saving file %s \n", s.c_str());
	::format3d::Mesh::Builder meshCap = obj.initMesh();
	saveMesh(o, meshCap, options);
	writeMessage(m, (char*)ws2s(s).c_str());
}

inline void saveObject(IGameNode* node, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	if (node == nullptr) return;
	auto& o = *(node->GetIGameObject()->GetMaxObject());
	std::wstring s(node->GetName());
	saveObject(o, (TCHAR*)s.c_str(), options,exportFolder);
}

inline void saveObject(INode* node, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	if (node == nullptr) return;
	auto& o = *getObject(node);
	std::wstring s(node->GetName());
	saveObject(o, (TCHAR*)s.c_str(), options, exportFolder);
}

inline void saveObject(INode* node, TCHAR* name, int options = ExportOpts::all, wchar_t* exportFolder = L"") {
	if (node == nullptr) return;
	auto& o = *getObject(node);
	std::wstring s(name);
	saveObject(o, (TCHAR*)s.c_str(), options, exportFolder);
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
	auto items = root.getItems();
	auto objs = items.initObjs(nodes->Count());
	
	std::wstring s(name);
	
	s = exportFolder;
	s += name;
	s += L".cap";
	mprintf(L"Saving %d items\n", nodes->Count());
	
	auto& nRef = *nodes;
	for (int i=0;i<nRef.Count();++i){		
		auto obj = objs[i];		
		auto* node = nRef[i];			
			//mprintf(L"Saving %d\n", i);
		saveObject(node, obj, options);		
	}
	writeMessage(m, (char*)ws2s(s).c_str());
}

/*
Load Object from file
@filename : name of file
@node : pointer to a scene object whose content will be modified with data read from file. nullptr means a new scene obj will be created
@options: (optional, default to all)Load all, mesh, selection, uv, normals or a combination them
@exportFolder: (optional) the folder where the filename will be searched
@objName: (optional) if the file contains multiple objects, read the object whose name is specified by objName
*/
inline void loadObject(wchar_t* filename, INode* node = nullptr, int options = ExportOpts::all, wchar_t* exportFolder = L"",wchar_t* objName=nullptr) {
	std::wstring s;
	s = exportFolder;
	s += filename;
	s += L".cap";
	mprintf(L"Loading file %s (%s)\n", s.c_str(), filename);
	if (node && objName) node->SetName(objName);
	auto* obj = readMessage((char*)ws2s(s).c_str(), node, options);
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
	GetCOREInterface()->DisableSceneRedraw();
	std::wstring s;
	s = exportFolder;
	s += name;
	s += L".cap";
	mprintf(L"Loading file %s (%s)\n", s.c_str(), name);
	auto* obj = readMessage((char*)ws2s(s).c_str(), nodes, options);
	GetCOREInterface()->EnableSceneRedraw();
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
	std::wstring s;
	s = exportFolder;
	s += name;
	s += L".cap";
	mprintf(L"Loading file %s (%s)\n", s.c_str(), name);
	auto* obj = readMessage((char*)ws2s(s).c_str(), names, options);
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
	std::wstring s;
	s = exportFolder;
	s += name;
	s += L".cap";
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
}


inline void save(INode& node, int options = ExportOpts::all, wchar_t* name = nullptr,  wchar_t* exportFolder = L"") {
	if (!name) name = (wchar_t*)node.GetName();

	std::wstring s;
	s = exportFolder;
	s += name;
	s += L".cap";	
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

inline void testRead(char* name) {
	INode* newNode = nullptr;
	readMessage(name, newNode);
}