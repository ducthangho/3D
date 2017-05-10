#include "common.h"
#include "YLibs.h"
#include "YProjectUtils.h"
#include "grpc_client.h"
//#include "move_lambda.h"
#include <ppltasks.h>
#include <array>

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
#include <experimental/resumable>
#endif

#include <future>
//#include <pplawait.h>
//#include <pplawait.h>

using namespace concurrency;
using namespace std::experimental;


const std::string FM_ORGINAL_FILE = "{0}\\{1}_o.max";
const std::string FM_YAREA_FILE = "{0}\\yal.y3d";
MyNodeEventCB mcb;
//const std::string FM_WORKING_FILE_PREFIX = "{0}{1}";
NameTab low_nametab;
NameTab high_nametab;
INodeTab low_nodes;
INodeTab high_nodes;
int current_worker_id = 0;
std::map<int, std::string>  map;

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

inline void LayerInterfaceExample()
{
	FPInterface* fpInterface = GetCOREInterface(LAYERMANAGER_INTERFACE_ID);

	FPValue result;
	fpInterface->Invoke(COUNT_ILAYERMANAGER_GETTER, result);
	auto numLayer = result.i;
	LOG("number layer is {0}\n", numLayer);

	for (int i = 0; i < numLayer; i++)
	{
		FPParams pWHICH(1, WHICH_GETLAYER_ILAYERMANAGER_PARAM1_TYPE, i);
		fpInterface->Invoke(GETLAYER_ILAYERMANAGER, result, &pWHICH);

		auto fp = result.fpi;

		FPValue r;
		fp->Invoke(NAME_ILAYERPROPERTIES_GETTER, r);
		auto layerName = r.s;
		LOG("Layer number {} have layer name is ", i + 1); LOG(layerName); LOG("\n");

		//auto interfaceID = fp->GetID();
		//int fn_id = fp->FindFn(L"select");
		//LOG("fn_id is {}\n", fn_id);
		//LOG("Interface_ID is {}",InterFaceID2S(interfaceID));
		//LOG("Interface_ID is of LAYERMANAGER INTERFACE is {}", InterFaceID2S(LAYERMANAGER_INTERFACE_ID));

		//FPMixinInterface* ppmixinInterface = dynamic_cast<FPMixinInterface*>(fp);
		//if (ppmixinInterface != NULL)
		//	LOG("fp is instance of class FPMixinInterface\n");

		//FPMixinInterface* ppmixinInterface2 = dynamic_cast<FPMixinInterface*>(fpInterface);
		//if (ppmixinInterface2 == NULL)
		//	LOG("fpInterface is not instance of class FPMixinInterface\n");

		//auto fpInterfaceDesc = fp->GetDesc();
		//auto id = fpInterfaceDesc->GetID();
		//LOG("Interface_ID is {}\n", InterFaceID2S(id));

		////auto id = fpInterfaceDesc->ID;
		//auto internal_name = fpInterfaceDesc->internal_name;
		//LOG("interface internal_name is{0}\n", internal_name);
		//Tab<FPFunctionDef*> functions = fpInterfaceDesc->functions;
		//auto numFunction = functions.Count();
		//LOG("function numbers of interface {0} is {1}\n", internal_name, numFunction);

		//generateInterfaceFuntionsID2(fpInterfaceDesc);
	}
}

inline void pre_optimize(std::string oFileDir, std::string oFileName, std::string projectPath)
{
	FPInterface* fpInterface = GetCOREInterface(BATCHPROOPTIMIZER_INTERFACE_ID);

	FPParams pSourceFileMode(1, TYPE_INT, 0);
	fpInterface->Invoke(SOURCEFILEMODE_IBATCHPROOPTIMIZER_SETTER, &pSourceFileMode);

	std::string file1 = oFileDir + "\\" + oFileName + ".max";
	Tab<MCHAR*> sourceFile_Files;
	auto file = const_cast<wchar_t*>(s2ws(file1).c_str());
	sourceFile_Files.Append(1, &file);
	FPParams pSourceFileFiles(1, SOURCEFILEFILES_IBATCHPROOPTIMIZER_TYPEPARAM, sourceFile_Files);
	fpInterface->Invoke(SOURCEFILEFILES_IBATCHPROOPTIMIZER_SETTER, &pSourceFileFiles);

	FPParams OptimizationLevel1(1, OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_TYPEPARAM, 90);
	fpInterface->Invoke(OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel1);

	FPParams OptimizationLevel2(1, OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel2);

	FPParams OptimizationLevel3(1, OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel3);

	FPParams OptimizationLevel4(1, OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel4);

	FPParams OptimizationLevel5(1, OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel5);

	FPParams OptimizationLevelMode(1, OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevelMode);

	setInterFacePropertyTInt(OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_SETTER, 1);
	setInterFacePropertyTBool(AUTORENAME_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, AUTORENAME_IBATCHPROOPTIMIZER_SETTER, false);
	setInterFacePropertyTInt(DESTFILENAMEMODE_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, DESTFILENAMEMODE_IBATCHPROOPTIMIZER_SETTER, 1);
	setInterFacePropertyTInt(DESTFOLDERMODE_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, DESTFOLDERMODE_IBATCHPROOPTIMIZER_SETTER, 2);

	//std::string desDir = ".\\" + oFileName + "_y3d";
	setInterFacePropertyTString(DESTFOLDERNAME_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, DESTFOLDERNAME_IBATCHPROOPTIMIZER_SETTER, projectPath);

	fpInterface->Invoke(BATCH_IBATCHPROOPTIMIZER);

	std::string maxFile = projectPath + "\\" + oFileName + "90.max";
	GetCOREInterface16()->LoadFromFile(s2ws(maxFile).data(), Interface8::LoadFromFileFlags::kSuppressPrompts&Interface8::LoadFromFileFlags::kUseFileUnits);
	FPValue result;
	fpInterface->Invoke(DESTFOLDERNAME_IBATCHPROOPTIMIZER_GETTER, result);
	const wchar_t* a = result.s;
	logserver::LOG(a);
	//logserver::LOG(b);
}

void DoYEvent(YEvent ye) {
	
	auto* ip = GetCOREInterface();
	if (ye.has_select()) {
		auto name_select = ye.select().name();
		auto n = ip->GetINodeByName(s2ws(name_select).c_str());
		n->Hide(FALSE);
		ip->SelectNode(n);
		setIsolate(ye.select().isolate());
		//if (!ye.select().isolate()) {
		//	auto cmd = L"IsolateSelection.EnterIsolateSelectionMode()";
		//	ExecuteMAXScriptScript(cmd);
		//}
		//else {
		//	auto cmd = L"IsolateSelection.ExitIsolateSelectionMode()";
		//	ExecuteMAXScriptScript(cmd);
		//}
	}
	else if (ye.has_isolate()) {
		auto ii = ye.isolate();
		if (ii.endisolate()) {
			setIsolate(false);
			//auto cmd = L"IsolateSelection.ExitIsolateSelectionMode()";
			//ExecuteMAXScriptScript(cmd);
		}
		else if (!ii.name().empty()) {
			auto name_select = ii.name();
			auto n = ip->GetINodeByName(s2ws(name_select).c_str());
			ip->SelectNode(n);
			setIsolate(true);
			//auto cmd = L"actionMan.executeAction 0 \"197\";";
			//ExecuteMAXScriptScript(cmd);
		}
		else if (!ii.layer().empty()) {
			setIsolateLayer(ii.layer());
		}

	}
	else if (ye.has_close()) {
		if (ye.close().bypass())
			ExecuteMAXScriptScript(L"actionMan.executeAction 0 \"16\"");
			//"resetMaxFile #noPrompt"
		else {
		}
	}
	else if (ye.has_del()) {
		auto dd = ye.del();
		if (dd.names().size()>0) {
			for (int i = 0; i < dd.names().size(); i++)
			{
				ip->DeleteNode(ip->GetINodeByName(s2ws(dd.names(i)).c_str()),1);
			}
		}
		else if (dd.layers().size()>0) {
			for (int i = 0; i < dd.layers().size(); i++)
			{
				//DeleteLayer(s2ws(dd.layers(i)).c_str());
				DeleteLayer(dd.layers(i));
			}
		}
		else if (dd.groups().size()>0) {

		}
	}
	else if (ye.has_mod()) {
		LOG("\nDo event MOD!");
		std::wstring cmd;
		if (ye.mod().type() == "string") {
			cmd = formatWS("${0}.modifiers[#{1}].{2} = \"{3}\"", ye.mod().oname(), ye.mod().mod_name(), ye.mod().key(), ye.mod().value());
		}
		else {
			cmd = formatWS("${0}.modifiers[#{1}].{2} = {3} as {4}", ye.mod().oname(), ye.mod().mod_name(), ye.mod().key(), ye.mod().value(), ye.mod().type());
		}
		LOG(cmd.c_str());
		ExecuteMAXScriptScript(cmd.c_str());
	}
	else if (ye.has_yclone()) {
		auto n = ip->GetINodeByName(s2ws(ye.yclone().oname()).c_str());
		auto n2 = ip->GetINodeByName(s2ws(ye.yclone().cname()).c_str());
		if (n2 != NULL||n==NULL) return;
		INodeTab nt1, nt2;
		nt1.AppendNode(n);

		CloneType ct = NODE_NONE;
		switch (ye.yclone().clone_type())
		{
			case EClone::CloneType::EClone_CloneType_NODE_INSTANCE: {
				ct = NODE_INSTANCE;
				break;
			}
			case EClone::CloneType::EClone_CloneType_NODE_COPY: {
				ct = NODE_COPY;
				break;
			}
			case EClone::CloneType::EClone_CloneType_NODE_REFERENCE:{
				ct = NODE_REFERENCE;
				break;
			}
		default:
			break;
		}

		if (ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, ct, &nt2, &nt2)) {
			nt2[0]->SetName(s2ws(ye.yclone().cname()).c_str());
			switch (ye.yclone().convert_type())
			{
				case Editable_Mesh: {
					auto cid = new Class_ID(3830386867L, 0L); // convert to editable_mesh
					ip->ConvertNode(nt2[0], *cid);
					break;
				}
				case Editable_Poly:
					break;
				default:
					break;
			}


		}
	}
	else if (ye.has_lowpoly()) {
		do_lowpoly(ye.lowpoly());
	}
	else if (ye.has_unwrap()) {
		do_unwrap(ye.unwrap());
	}
	else if (ye.has_rename()) {
		auto n = ip->GetINodeByName(s2ws(ye.rename().oname()).c_str());
		n->SetName(s2ws(ye.rename().nname()).c_str());
	}

}

//template <class REQ,class RESP>
task<void> send_request() {	
	YEvent2 ye;
	//ESelect es;
	//es.set_name(ws2s(n->GetName()));
	//es.set_isolate(false);
	ye.mutable_select()->set_name(ws2s(L"Hello world"));
	ye.mutable_select()->set_isolate(false);


	//Status* status;
	// thay = async				
	//client->AsyncDoEvent(&context, ye, &cq_);
	//LOG("Select object:{0}\n", ye.select().name());
	auto* client = getClientInstance();
	if (client) {
		struct Result {
			//grpc::CompletionQueue cq;
			grpc::Status status;
			y3d::ResponseEvent2 rep;
			std::string name;
		};
		Result* rs = new Result();
		rs->name = ws2s(L"Hello world");

		//auto lambda = create_move_lambda(std::move(cq), [](std::unique_ptr<grpc::CompletionQueue>& cq) {

		//});
		auto task = create_task([rs, client, ye]() {
			grpc::CompletionQueue cq;
			grpc::ClientContext context;
			auto rpc = client->AsyncDoEvent(&context, ye, &cq);
			LOG("Async called\n");
			rpc->Finish(&(rs->rep), &(rs->status), (void*)1);
			void* got_tag;
			bool ok = false;
			LOG("Waiting...\n");
			cq.Next(&got_tag, &ok);
			if (ok && got_tag == (void*)1) {
				LOG("Got tag\n");
				return rs->status;
			}
			LOG("Failed\n");
			return grpc::Status::CANCELLED;
		}).then([rs](grpc::Status t) {
			logserver::LOG("Done. Selected {}. Clean up. \n", rs->name);
			delete rs;
		});
		return task;
		//tasks.push_back(task);
	}
	else {
		logserver::LOG("Cannot connect to MainWorker server\n");
	};
	return task<void>();
}

void MyNodeEventCB::SelectionChanged(NodeKeyTab & nodes)
{	
	if (nodes.Count() > 0) {
		/*auto client = y3d::YServiceMainWorker::NewStub(grpc::CreateChannel("127.0.0.1:37001", grpc::InsecureChannelCredentials()));*/
		std::vector< task<grpc::Status> > tasks;
		for (int i = 0; i < nodes.Count(); i++)
		{
			auto n = NodeEventNamespace::GetNodeByKey(nodes[i]);
			if (n == NULL) continue;
			if (n->Selected()) {

				//co_await send_request();
				//GRPC_CALL(DoEvent, &context, ye, &rep);
				//auto status = client->DoEvent(&context, ye, &rep);

				/*		YEvent ye;
				ESelect es;
				es.set_name(ws2s(n->GetName()));
				es.set_isolate(false);
				ye.mutable_select()->CopyFrom(es);
				received_e.clear();
				received_e.push_back(ye);*/
				//mprintf(L"Test select: %s \n", n->GetName());
				break;
			}
		}
		auto joinTask = when_all(begin(tasks), end(tasks));
		joinTask.wait();
		//MessageBoxW(NULL, n->GetName(), L"TEST", MB_OK);
	}
}

BOOL do_clone(const EClone ec) {
	return TRUE;
}

BOOL do_unwrap(const EUnwrap eu) {
	if (eu.has_max3d()) {
		std::wstring cmd = formatWS("yms.unwrap3dmax \"{0}\" \"{1}\" \"{2}\"", eu.oname(), eu.max3d().angle(), eu.max3d().spacing());
		return ExecuteMAXScriptScript(cmd.c_str());
	}
	else if (eu.has_blender()) {

	}
	return TRUE;
}

BOOL do_lowpoly(const ELowpoly el, bool make_temp) {
	if (el.has_lp_xref()) {
		/*	auto* ip = GetCOREInterface();
		auto n = ip->GetINodeByName(s2ws(eu->oname()).c_str());*/
		//auto cmd = formatWS("yms.set_display_proxy \"{0}\" false", ws2s(lowStr).c_str());
		//ExecuteMAXScriptScript(cmd.c_str());
	}
	if (el.has_lp_3dmax()) {
		auto mm = el.lp_3dmax();
		auto name4clone = (el.nname().size() > 0) ? el.nname() : el.oname();
		std::wstring cmd = formatWS("yms.lowpoly_3dmax \"{0}\" \"{1}\" {2}", el.oname(), el.nname(), mm.vertex_percent());
		if (ExecuteMAXScriptScript(cmd.c_str())) {
			if (make_temp) {
				YEvent ee;
				EClone ec;
				ec.set_oname(name4clone);
				ec.set_cname(name4clone + "_tmp");
				ee.mutable_yclone()->CopyFrom(ec);
				DoYEvent(ee);
			}

			if (el.convert_type() == ConvertType::None) return TRUE;
			auto* ip = GetCOREInterface();
			auto n = ip->GetINodeByName(s2ws(name4clone).c_str());

			if (el.convert_type() == ConvertType::Editable_Mesh) {
				LOG("Convert : {0}",name4clone);
				auto cid = new Class_ID(3830386867L, 0L); // convert to editable_mesh
			    ip->ConvertNode(n, *cid);
			}
			return TRUE;
		}
		return FALSE;
	}
	else if (el.has_lp_blender()) {
		auto bl = el.lp_blender();
		std::wstring cmd = formatWS("yms.lowpoly_blender \"{0}\" \"{1}\"", el.oname(), bl.ratio());
		return ExecuteMAXScriptScript(cmd.c_str());
	}
	return TRUE;
}

BOOL do_pack(const EPacking ep) {
	if (ep.has_packrect()) {
		auto x = ep.packrect();
	}
	return TRUE;
}

BOOL save_test(InitTestParam x) {
	auto* ip = GetCOREInterface();
	//ip->ClearNodeSelection();
	setIsolateLayer(fmt::format("{}_{}",x.oname(),x.id()));
	INodeTab inodes;
	getSelNodeTab(inodes);
	//ip->SaveToFile()
	ip->FileSaveNodes(&inodes, formatWS("{0}\\{1}_data.max", x.test_folder(), x.oname()).c_str());
	return TRUE;
}

BOOL load_test(InitTestParam x) {
	LOG("\nLoad test:{}",x.oname());
	auto* ip = GetCOREInterface();
	auto data_path = formatWS("{0}\\{1}_data.max", x.test_folder(), x.oname()).c_str();
	ip->MergeFromFile(data_path, TRUE);
	return TRUE;
}