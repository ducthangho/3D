#pragma once
#include "YLibs.h"

const string FM_ORGINAL_FILE = "{}\\{}.max"; 
const string FM_YDATA_DIR = "{}\\{}_y3d\\";
const string FM = "{}\\{}.max";
const string FM_YSYS_FOLDER = "{}\\y3d\\";
const string FM_YSETTING_FILE = "{}\\ysetting.y3d";
const string FM_YAREA_FILE = FM_YDATA_DIR + "yal.y3d";
const string FM_WORKING_FILE_PREFIX = FM_YDATA_DIR + "{}";
const int PORT_MASTER = 8000;
const string FM_SERVER_ADDR = "0.0.0.0:{}";
YSystem YSys;
YWorkerList YWList;
std::vector<YEvent> received_e;
//YEvent current_e;

inline void resetSystem() {
}

inline void initSystem() {

	auto* ip = GetCOREInterface();
	auto setting_path = ip->GetDir(9);

	//std::wstring yfolder = formatWS(FM_YSYS_FOLDER, setting_path);

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
	std::string path = fmt::format(FM_YSETTING_FILE, YSys.working_folder().c_str());
	log(path);
	log("projects size: {}", YSys.projects_size());
	fstream output(path, ios::out | ios::trunc | ios::binary);
	if (!YSys.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not override setting file!", L"Error", MB_OK);
	}
}

NameTab low_nametab;
NameTab high_nametab;
INodeTab low_nodes;
INodeTab high_nodes;

inline void getObjInfo(INode* node, YObject* yo) {
	auto* oo = getObject(node);
	auto t = getSuperType(oo);

	if (t == GEOMETRY_T) {
		yo->set_otype(y3d::ObjectType::GEOMETRY);
		//mprintf(L"Mesh: %s \n", node->GetName());
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

		if (mtype != y3d::YMesh::MeshType::YMesh_MeshType_Unknown) {
			if (ym.num_faces() > 1000) {
				high_nametab.AddName(node->GetName());
				high_nodes.AppendNode(node,true);
		}	
			else {
				low_nametab.AddName(node->GetName());
				low_nodes.AppendNode(node, true);
			}
		}
		
	}
	else if (t == CAMERA_T) {
		yo->set_otype(y3d::ObjectType::CAMERA);
		//mprintf(L"Cam: %s \n", node->GetName());
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

	//low_nametab->Delete(0, low_nametab->Count());
	//high_nametab->Delete(0, high_nametab->Count());
	//low_nodes->Delete(0, low_nodes->Count());
	//high_nodes->Delete(0, high_nodes->Count());

	buildGroup(root, ya);
	// switch to camera 1, Maximize Viewport Toggle, Views: Perspective User View, Viewport Visual Style Wireframe
	ExecuteMAXScriptScript(L"actionMan.executeAction 0 \"160\"");
	ExecuteMAXScriptScript(L"max vpt persp user;actionMan.executeAction 0 \"551\"");
	ip->SetViewportMax(true);
}

inline void DoXrefHigh(ProjectInfo* pi) {
	auto* ip = GetCOREInterface();
	std::wstring low_file = formatWS(FM_WORKING_FILE_PREFIX + "_low0.max", pi->path(), pi->pname(), pi->pname());
	ip->FileSaveNodes(&high_nodes, low_file.c_str());
	std::wstring original_file = formatWS(FM_ORGINAL_FILE, pi->path(), pi->pname());
	//log(original_file);
	//ip->SelectNodeTab(high_nodes, true, false);
	for (int i = 0; i < high_nodes.Count(); i++)
	{
		auto na = high_nodes[i]->NodeName();
		ip->DeleteNode(high_nodes[i], true);
	}

	ip->MergeFromFile(original_file.c_str(), true, true, false, 3, &high_nametab, 1, 1);
	std::wstring high_file = formatWS(FM_WORKING_FILE_PREFIX + "_high.max", pi->path(), pi->pname(), pi->pname());

	INodeTab xreftab;
	getSelNodeTab(xreftab);
	
	ip->FileSaveNodes(&xreftab, high_file.c_str());
	std::wstring cmd = formatWS("yms.xref_low \"{}\" \"{}\"", pi->path(), pi->pname());

	ExecuteMAXScriptScript(cmd.c_str());
	ip->MergeFromFile(original_file.c_str(), true, true, false, 3, &low_nametab, 1, 1);
	ip->FileSave();
}

inline void NewYProject(const NewProjectParam* pp, ResponseNProject* rnp) {
	std::wstring cmd = formatWS("yms.pre_optimize \"{}\" \"{}\"", pp->folder(), pp->fname());
	ExecuteMAXScriptScript(cmd.c_str());

	YAreaList yal;
	ObjectFromMax(&yal);

	std::string path = fmt::format(FM_YAREA_FILE, pp->folder(), pp->fname());

	auto pi = YSys.add_projects();
	pi->CopyFrom(rnp->pinfo());
	saveSystem();

	log(path);

	fstream output(path, ios::out | ios::trunc | ios::binary);
	if (!yal.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not create List Area Object file!", L"Error", MB_OK);
	}
	DoXrefHigh(pi);
	rnp->mutable_yal()->CopyFrom(yal);
	rnp->mutable_sys()->CopyFrom(YSys);
}

inline void LoadNProject(ResponseNProject* rnp) {
	auto* ip = GetCOREInterface();
	std::wstring work_90 = formatWS(FM_WORKING_FILE_PREFIX + "90.max", rnp->pinfo().path(), rnp->pinfo().pname(), rnp->pinfo().pname());
	std::string yal_path = fmt::format(FM_YAREA_FILE, rnp->pinfo().path(), rnp->pinfo().pname());

	//if (ip->LoadFromFile(s2ws(wpath).c_str()) == 0) {
	if (ip->LoadFromFile(work_90.c_str()) == 0) {
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
				rnp->mutable_sys()->CopyFrom(YSys);
			}
		}
	}
}

inline void DeleteYProject(ResponseNProject* rnp) {
	for (int i = 0; i < YSys.projects_size(); i++)
	{
		auto pi = YSys.projects(i);
		if ((pi.pname() == rnp->pinfo().pname()) && (pi.path() == rnp->pinfo().path())) {
			YSys.mutable_projects()->DeleteSubrange(i, 1);
			saveSystem();
			rnp->mutable_sys()->CopyFrom(YSys);
			return;
		}
	}
	MessageBoxW(NULL, L"Can not find project!", L"Error", MB_OK);
	rnp->set_err("No project found!");
}

inline void initXref() {
	auto* ip = GetCOREInterface();

	//low_nametab.AddName("")
	//ip->saveNo
}

inline void DoYEvent(YEvent ye) {
	auto* ip = GetCOREInterface();
	if (ye.has_select()) {
		auto name_select = ye.select().name();
		auto n = ip->GetINodeByName(s2ws(name_select).c_str());
		ip->SelectNode(n);
		//std::string cmd = "select $" + name_select + ";";
		//ExecuteMAXScriptScript(s2ws(cmd).c_str());
	}
	else if (ye.has_isolate()) {
		if (ye.isolate().endisolate()) {
			auto cmd = L"IsolateSelection.ExitIsolateSelectionMode()";
			ExecuteMAXScriptScript(cmd);
		}
		else {
			auto name_select = ye.isolate().name();
			auto n = ip->GetINodeByName(s2ws(name_select).c_str());
			ip->SelectNode(n);
			auto cmd = L"actionMan.executeAction 0 \"197\";";
			ExecuteMAXScriptScript(cmd);
		}
	}

}

inline void defaultFrange() {

}


//inline void getInfoHL() {
//
//}
