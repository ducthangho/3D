#include "common.h"
#include "YLibs.h"
#include "YProjectUtils.h"

#include "LogClient.h"
using namespace logserver;

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
	auto path = fmt::format("{0}\\yal.y3d",pp->project_path());
	//auto pi = YSys.add_projects();
	//pi->CopyFrom(rnp->pinfo());
	//saveSystem();
	logserver::LOG(path);
	std::fstream output(path, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!yal.SerializePartialToOstream(&output)) {
		MessageBoxW(NULL, L"Can not create List Area Object file!", L"Error", MB_OK);
	}
	rnp->mutable_yal()->CopyFrom(yal);
}

void LoadNProject(ResponseNProject* rnp) {
	auto* ip = GetCOREInterface();
	auto s90 = fmt::format("{0}\\{1}90.max", rnp->pinfo().project_path(), rnp->pinfo().pname());
	auto yal_path = fmt::format("{0}\\yal.y3d", rnp->pinfo().project_path());
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

void DoYEvent(YEvent ye) {
	if (ye.has_select()) {
		auto name_select = ye.select().name();
		std::string cmd = "select $" + name_select + ";";
		//MessageBoxW(NULL, s2ws(cmd).c_str(), L"TEST", MB_OK);
		ExecuteMAXScriptScript(s2ws(cmd).c_str());
		return;
	}
	if (ye.has_close()) {
		if (ye.close().bypass())
			ExecuteMAXScriptScript(L"actionMan.executeAction 0 \"16\"");
			//"resetMaxFile #noPrompt"
		else {

		}
	}
}
