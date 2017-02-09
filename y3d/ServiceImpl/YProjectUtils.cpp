#include "common.h"
#include "YLibs.h"
#include "YProjectUtils.h"


YSystem YSys;
void initSystem()
{

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
	std::fstream input(path, std::ios::in | std::ios::binary);
	if (!input) {
		ProjectInfo pi;
		YSys.mutable_default_info()->CopyFrom(pi);
		PSetting ps;
		ps.set_max_recent(5);
		YSys.mutable_default_setting()->CopyFrom(ps);
		YSys.clear_projects();
		std::fstream output(path, std::ios::out | std::ios::trunc | std::ios::binary);
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

void saveSystem()
{
	std::string kk;
	kk.append(YSys.working_folder().c_str());
	kk.append("ysetting.y3d");
	//MessageBoxW(NULL, s2ws(kk).c_str(), L"sAVE", MB_OK);
	//mprintf(L"aa:%s \n", s2ws(kk).c_str());
	std::fstream output(kk, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!YSys.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not override setting file!", L"Error", MB_OK);
	}
}

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
	buildGroup(root, ya);
	// switch to camera 1, Maximize Viewport Toggle, Views: Perspective User View, Viewport Visual Style Wireframe
	ExecuteMAXScriptScript(L"actionMan.executeAction 0 \"160\"");
	ExecuteMAXScriptScript(L"max vpt persp user;actionMan.executeAction 0 \"551\"");
	ip->SetViewportMax(true);
}

void NewYProject(const NewProjectParam* pp, ResponseNProject* rnp) {
	std::string tmp = "yms.pre_optimize";
	char buf[1000];
	std::sprintf(buf, "%s \"%s\" \"%s\"", tmp.c_str(), pp->folder().c_str(), pp->fname().c_str());
	auto cmd = s2ws(buf);
	ExecuteMAXScriptScript(cmd.c_str());

	YAreaList yal;
	ObjectFromMax(&yal);

	std::string path;
	path = pp->folder();
	path += "\\" + pp->fname() + "_y3d\\yal.y3d";

	auto pi = YSys.add_projects();
	pi->CopyFrom(rnp->pinfo());
	saveSystem();

	log(path);

	std::fstream output(path, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!yal.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not create List Area Object file!", L"Error", MB_OK);
	}
	rnp->mutable_yal()->CopyFrom(yal);
	rnp->mutable_sys()->CopyFrom(YSys);
}

void LoadNProject(ResponseNProject* rnp) {
	auto* ip = GetCOREInterface();
	std::string pfolder;
	pfolder.append(rnp->pinfo().path().c_str());
	pfolder += "\\" + rnp->pinfo().pname() + "_y3d\\";
	//pfolder.append("\\");
	std::string wpath;
	wpath.append(pfolder);
	wpath.append(rnp->pinfo().pname().c_str());
	wpath.append("90.max");
	std::string yal_path;
	yal_path.append(pfolder);
	yal_path.append("yal.y3d");
	if (ip->LoadFromFile(s2ws(wpath).c_str()) == 0) {
		MessageBoxW(NULL, L"Can not load working file!", L"Error", MB_OK);
	}
	else {
		std::fstream input(yal_path, std::ios::in | std::ios::binary);
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

void DeleteYProject(ResponseNProject* rnp) {
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

void initXref() {
	auto* ip = GetCOREInterface();
	NameTab low_nametab;
	INodeTab low_nodes;
	//low_nametab.AddName("")
	//ip->saveNo
}

void DoYEvent(YEvent ye) {
	if (ye.has_select()) {
		auto name_select = ye.select().name();
		std::string cmd = "select $" + name_select + ";";
		//MessageBoxW(NULL, s2ws(cmd).c_str(), L"TEST", MB_OK);
		ExecuteMAXScriptScript(s2ws(cmd).c_str());
	}
}

void defaultFrange() {

}
