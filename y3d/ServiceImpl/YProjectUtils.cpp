#include "common.h"
#include "YLibs.h"
#include "YProjectUtils.h"



const std::string FM_ORGINAL_FILE = "{0}\\{1}_o.max";
const std::string FM_YAREA_FILE = "{0}\\yal.y3d";
MyNodeEventCB mcb;
//const std::string FM_WORKING_FILE_PREFIX = "{0}{1}";
NameTab low_nametab;
NameTab high_nametab;
INodeTab low_nodes;
INodeTab high_nodes;
int current_worker_id = 0;


void getObjInfo(INode* node, YObject* yo) {
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
				high_nodes.AppendNode(node, true);
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

void buildGroup(INode* node, YGroup* yg) {
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

void buildGroup(INode* node, YArea* ya) {
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


void ObjectFromMax(YAreaList* yal) {
	auto* ip = GetCOREInterface();
	auto ya = yal->add_areas();
	ya->set_name("Area 1");
	auto root = GetCOREInterface()->GetRootNode();

	high_nametab.SetCount(0, TRUE);
	low_nametab.SetCount(0, TRUE);
	low_nodes.SetCount(0, TRUE);
	high_nodes.SetCount(0, TRUE);

	buildGroup(root, ya);
	// switch to camera 1, Maximize Viewport Toggle, Views: Perspective User View, Viewport Visual Style Wireframe
	ExecuteMAXScriptScript(L"actionMan.executeAction 0 \"160\"");
	ExecuteMAXScriptScript(L"max vpt persp user;actionMan.executeAction 0 \"551\"");
	ip->SetViewportMax(true);
}

void NewYProject(const NewProjectParam* pp, ResponseNProject* rnp) {
	logserver::LOG("\nNew project:{}\n",rnp->pinfo().pname());
	std::string tmp = "yms.pre_optimize";
	char buf[1000];
	std::sprintf(buf, "%s \"%s\" \"%s\" \"%s\"", tmp.c_str(), pp->folder().c_str(), pp->fname().c_str(),pp->project_path().c_str());
	auto cmd = s2ws(buf);
	ExecuteMAXScriptScript(cmd.c_str());

	YAreaList yal;
	ObjectFromMax(&yal);

	//std::string path;
	//path = pp->project_path();
	//path += "\\yal.y3d";
	auto path = fmt::format(FM_YAREA_FILE,pp->project_path());
	//auto pi = YSys.add_projects();
	//pi->CopyFrom(rnp->pinfo());
	//saveSystem();
	logserver::LOG(path);
	std::fstream output(path, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!yal.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not create List Area Object file!", L"Error", MB_OK);
	}
	ProjectInfo* pi = new ProjectInfo();
	pi->CopyFrom(rnp->pinfo());
	DoXrefHigh(pi);
	rnp->mutable_yal()->CopyFrom(yal);
}

void LoadNProject(ResponseNProject* rnp) {
	auto* ip = GetCOREInterface();
	auto s90 = fmt::format("{0}\\{1}90.max", rnp->pinfo().project_path(), rnp->pinfo().pname());
	auto yal_path = fmt::format(FM_YAREA_FILE, rnp->pinfo().project_path());
	rnp->set_err(false);

	if (ip->LoadFromFile(s2ws(s90).c_str()) == 0) {
		rnp->set_err(true);
		MessageBoxW(NULL, L"Can not load working file!", L"Error", MB_OK);
	}
	else {
		std::fstream input(yal_path, std::ios::in | std::ios::binary);
		if (!input) {
			rnp->set_err(true);
			MessageBoxW(NULL, L"Can not load List Area Object file!", L"Error", MB_OK);
		}
		else {
			YAreaList yal;
			if (!yal.ParseFromIstream(&input)) {
				rnp->set_err(true);
				MessageBoxW(NULL, L"Can not read yal file!", L"Error", MB_OK);
			}
			else {
				rnp->mutable_yal()->CopyFrom(yal);
			}
		}
	}
}
//

inline void DoXrefHigh(ProjectInfo* pi) {
	auto* ip = GetCOREInterface();
	std::wstring low_file = formatWS("{0}\\{1}_low0.max", pi->project_path(), pi->pname());
	ip->FileSaveNodes(&high_nodes, low_file.c_str());
	//std::wstring original_file = formatWS(FM_ORGINAL_FILE, pi->project_path(), pi->pname());
	std::wstring original_file = formatWS("{}", pi->original_path());

	logserver::LOG(original_file);
	//log(original_file);
	//ip->SelectNodeTab(high_nodes, true, false);
	for (int i = 0; i < high_nodes.Count(); i++)
	{
		auto na = high_nodes[i]->NodeName();
		ip->DeleteNode(high_nodes[i], true);
	}

	ip->MergeFromFile(original_file.c_str(), true, true, false, 3, &high_nametab, 1, 1);
	std::wstring high_file = formatWS("{0}\\{1}_high.max", pi->project_path(), pi->pname());

	INodeTab xreftab;
	getSelNodeTab(xreftab);
	ip->FileSaveNodes(&xreftab, high_file.c_str());
	std::wstring cmd = formatWS("yms.xref_low \"{0}\" \"{1}\"", pi->project_path(), pi->pname());

	ExecuteMAXScriptScript(cmd.c_str());
	ip->MergeFromFile(original_file.c_str(), true, true, false, 3, &low_nametab, 1, 1);
	ip->FileSave();
}

inline void xref_low(std::string project_path, std::string pname) {

}

inline void pre_optimize(std::string oFileDir, std::string oFileName, std::string projectPath) {

}

void DoYEvent(YEvent ye) {
	auto* ip = GetCOREInterface();
	if (ye.has_select()) {
		auto name_select = ye.select().name();
		auto n = ip->GetINodeByName(s2ws(name_select).c_str());
		ip->SelectNode(n);
		if (!ye.select().isolate()) {
			auto cmd = L"IsolateSelection.EnterIsolateSelectionMode()";
			ExecuteMAXScriptScript(cmd);
		}
		else {
			auto cmd = L"IsolateSelection.ExitIsolateSelectionMode()";
			ExecuteMAXScriptScript(cmd);
		}
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
	else if (ye.has_close()) {
		if (ye.close().bypass())
			ExecuteMAXScriptScript(L"actionMan.executeAction 0 \"16\"");
			//"resetMaxFile #noPrompt"
		else {
		}
	}
}
