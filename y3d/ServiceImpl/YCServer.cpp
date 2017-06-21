#ifndef Y3D_EXPORT_MACRO
#define Y3D_EXPORT_MACRO
#endif
#include "common.h"
//#include "3dsmaxUtils.h"
#include "YLibs.h"
#include "YCServer.h"
#include "xnormal.pb.h"
#include "xNormalSettings.h"
#include "YProjectUtils.h"
#include "LogClient.h"
#include "impexp.h"

//std::promise<void> exit_requested;

Status YServiceImpl::Init4Test(ServerContext* context, const InitTestParam* request, InitTestResponse* reply)
{
	Invoke([request]() -> void {
		std::wstring cmd;

		auto* ip = GetCOREInterface();
		auto n = ip->GetINodeByName(s2ws(request->oname()).c_str());
		n->Hide(FALSE);
		auto it = request->init_test();

		//auto orignalStr = formatWS("{}_{}_o", request->oname().c_str(), request->id().c_str());
		auto lowStr = formatWS("{}_{}_low", request->oname().c_str(), request->id().c_str());
		auto hiStr = formatWS("{}_{}_high", request->oname().c_str(), request->id().c_str());
		auto cageStr = formatWS("{}_{}_cage", request->oname().c_str(), request->id().c_str());
		auto finalStr = formatWS("{}_{}_final", request->oname().c_str(), request->id().c_str());

		INodeTab nt1, nt2, nt3, nt4, nt5, nt6;
		nt1.AppendNode(n);
		ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, NODE_COPY, &nt2, &nt2);

		auto cid = new Class_ID(3830386867L, 0L); // convert to editable_mesh
		ip->ConvertNode(nt2[0], *cid);
		//Collapse(nt2[0]);

		ip->CloneNodes(nt2, n->GetObjOffsetPos(), true, NODE_COPY, &nt3, &nt3);
		ip->CloneNodes(nt2, n->GetObjOffsetPos(), true, NODE_COPY, &nt4, &nt4);
		ip->CloneNodes(nt2, n->GetObjOffsetPos(), true, NODE_COPY, &nt5, &nt5);

		//std::wstring tmp = n->GetName();
		//auto lowStr = tmp + L"_low";
		//auto hiStr = tmp + L"_high";
		//auto cageStr = tmp + L"_cage";

		nt2[0]->SetName(lowStr.c_str());
		nt3[0]->SetName(hiStr.c_str());
		nt4[0]->SetName(cageStr.c_str());
		nt5[0]->SetName(finalStr.c_str());

		nt6.AppendNode(nt2[0]);
		nt6.AppendNode(nt3[0]);
		nt6.AppendNode(nt4[0]);
		nt6.AppendNode(nt5[0]);
		ip->SelectNodeTab(nt6,true);
		//cmd = formatWS("yms.set_display_proxy \"{0}\" true", ws2s(hiStr).c_str());
		//LOG(cmd.c_str());
		//ExecuteMAXScriptScript(cmd.c_str());
		//cmd = formatWS("yms.set_display_proxy \"{0}\" true", ws2s(cageStr).c_str());
		//ExecuteMAXScriptScript(cmd.c_str());

		//ip->SelectNodeTab(nt1, TRUE, FALSE);
		//ip->FileSaveNodes(&nt1, formatWS("{0}\\{1}_o.max", request->test_folder(), ws2s(n->GetName()).c_str()).c_str());

		//ip->SelectNode(ip->GetINodeByName(lowStr.c_str()));
		//ip->SelectNode(ip->GetINodeByName(hiStr.c_str()),0);
		//ip->SelectNode(ip->GetINodeByName(cageStr.c_str()), 0);
		//ip->SelectNode(ip->GetINodeByName(finalStr.c_str()),0);
		cmd = formatWS("yms.create_layer \"{0}_{1}\" true true", request->oname().c_str(), request->id().c_str());
		ExecuteMAXScriptScript(cmd.c_str());
		//if (it.has_lowpoly()) {
		//	ip->SelectNode(nt2[0]);
		//	do_lowpoly(&it.lowpoly());
		//}
		//else {
		//	cmd = formatWS("yms.set_display_proxy \"{0}\" false", ws2s(lowStr).c_str());
		//	ExecuteMAXScriptScript(cmd.c_str());
		//}

		if (it.has_lowpoly()) {
			do_lowpoly(it.lowpoly(),true);
		}
		if (it.has_unwrap()) {
			do_unwrap(it.unwrap());
		}
		if (it.has_pack()) {
			do_pack(it.pack());
		}

		//ip->SelectNode(nt2[0]);
		//ip->SelectNode(nt3[0],0);
		//ip->SelectNode(nt4[0],0);
		//cmd = L"actionMan.executeAction 0 \"197\";";
		// select and move to new layer
		//theHold.Begin();


		//ip->SelectNode(ip->GetINodeByName(lowStr.c_str()));
		//ip->ExportToFile(formatWS("{0}\\{1}_low.obj", request->test_folder(), ws2s(n->GetName()).c_str()).c_str(), TRUE, SCENE_EXPORT_SELECTED, new Class_ID(1371343970L, 1730353346L));
		//ip->SelectNode(ip->GetINodeByName(hiStr.c_str()));
		//ip->ExportToFile(formatWS("{0}\\{1}_high.obj", request->test_folder(), ws2s(n->GetName()).c_str()).c_str(), TRUE, SCENE_EXPORT_SELECTED, new Class_ID(1371343970L, 1730353346L));
		//ip->SelectNode(ip->GetINodeByName(cageStr.c_str()));
		//ip->ExportToFile(formatWS("{0}\\{1}_cage.obj", request->test_folder(), ws2s(n->GetName()).c_str()).c_str(), TRUE, SCENE_EXPORT_SELECTED, new Class_ID(1371343970L, 1730353346L));


		ip->SelectNode(ip->GetINodeByName(finalStr.c_str()));
		ip->ExportToFile(formatWS("{0}\\{1}_final.obj", request->test_folder(), ws2s(n->GetName()).c_str()).c_str(), TRUE, SCENE_EXPORT_SELECTED, new Class_ID(1371343970L, 1730353346L));
		
		ip->SelectNodeTab(nt6, true);
		save_test(*request);
		//theHold.Accept();
	});
	//waitForReturn(ret);
	return Status::OK;
}

Status YServiceImpl::LoadTestData(ServerContext* context, const InitTestParam* request, ResultReply* reply) {
	Invoke([request]() -> void {
		load_test(*request);
		//theHold.Accept();
	});
	return Status::OK;
}

Status YServiceImpl::MakeNode4Edit(ServerContext* context, const Make4TestParam* request, ResultReply* reply)
{
	Invoke([request]() -> void {
		//INodeTab nRef;
		//GetSceneNodes(nRef, nullptr);
		auto* ip = GetCOREInterface();

		auto oname = s2ws(request->oname());
		auto n = ip->GetINodeByName(oname.c_str());

		INodeTab nt1;
		INodeTab nt2;
		INodeTab nt3;
		INodeTab nt4;
		getSelNodeTab(nt1);
		ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, NODE_COPY, &nt2, &nt2);
		ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, NODE_COPY, &nt3, &nt3);
		ip->CloneNodes(nt1, n->GetObjOffsetPos(), true, NODE_COPY, &nt4, &nt4);
		std::wstring tmp = n->GetName();
		auto lowStr = tmp + L"_low";
		auto hiStr = tmp + L"_high";
		auto cageStr = tmp + L"_cage";
		nt2[0]->SetName(lowStr.c_str());
		nt3[0]->SetName(hiStr.c_str());
		nt4[0]->SetName(cageStr.c_str());
		//auto* nameMaker = ip->NewNameMaker();
		//GetCOREInterface13()->SetNameSuffixLength(2);
		//std::map<MSTR, int> mnames;
		//ExecuteMAXScriptScript(L"yms.pre_optimize 'a' 'b'");
	});
	//waitForReturn(ret);
	return Status::OK;
}

Status YServiceImpl::RenameObject(ServerContext* context, const RenameParam* request, ResultReply* reply)
{
	auto ret = InvokeAsync([]() -> void {
		INodeTab nRef;
		GetSceneNodes(nRef, nullptr);
		auto* ip = GetCOREInterface();

		auto* nameMaker = ip->NewNameMaker();
		GetCOREInterface13()->SetNameSuffixLength(2);
		std::map<MSTR, int> mnames;

		for (int i = 0; i < nRef.Count(); ++i) {
			auto* node = nRef[i];
			//mprintf(L"ClassName: %s \n", node->ClassName());
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
		mprintf(L"Hello world\n");
		//ExecuteMAXScriptScript(L"yms.pre_optimize 'a' 'b'");
		//ip->max
		//y3d::setting::xnormal::Settings s;
		//std::string hiPoly = "D:\\teapot_hi.FBX";
		//std::string loPoly = "D:\\teapot_lo.obj";
		//createDefaultSettings(&s, hiPoly, loPoly, "D:\\teapot.png");
		//setSize(s, 1024, 1024);

		//bakeNormal(s,"D:\\example.xml");
	});
	reply->set_message("DDDD");
	//reply->message.set_message("ddd");
	waitForReturn(ret);
	return Status::OK;
	// ... (pre-existing code)
}

Status YServiceImpl::BakeNormal(ServerContext* context, const ENormal* enm, ResultReply* reply)
{
	//ENormal e = *enm;
	//Invoke([enm]() -> void {

	Invoke([enm]() -> void {
		if (enm->has_normal_xnormal()) {
			Settings& s = (Settings&)enm->normal_xnormal();
			createDefaultSettings(&s, enm->highpoly(), enm->lowpoly(), enm->out_tex());
			setSize(s, enm->tex_size(), enm->tex_size());
			bakeNormal(s);
		}
		else if (enm->has_normal_3dmax()) {
			auto s = enm->normal_3dmax();
			char buf[1000];
			std::sprintf(buf, "minh.bake_3dmax \"%s\" \"%s\" \"%s\" %d", enm->highpoly().c_str(), enm->lowpoly().c_str(), enm->out_tex().c_str(), enm->tex_size());
			auto cmd = s2ws(buf);
			//mprintf(L"zzz is me");
			//ExecuteMAXScriptScript(L"minh.test1 \"aaa \"");
			ExecuteMAXScriptScript(cmd.c_str());
			////ExecuteMAXScriptScript(cmd.c_str());
		}
	});
	//a.has_normal_xnormal
	return Status::OK;
}

Status YServiceImpl::BatchOptimize(ServerContext* context, const BatchOptimizeParam* bp, ResultReply* reply)
{
	Invoke([bp]() -> void {
		std::string tmp = "yms.pre_optimize";
		char buf[1000];
		std::sprintf(buf, "%s \"%s\" \"%s\"", tmp.c_str(), bp->folder().c_str(), bp->filename().c_str());
		auto cmd = s2ws(buf);
		//bp->folder.a
		//ExecuteMAXScriptScript(L"yms.pre_optimize 'a' 'b'");
		//mprintf(L"Command is %s\n",cmd.c_str());
		ExecuteMAXScriptScript(cmd.c_str());
	});
	return Status::OK;
}

Status YServiceImpl::GetObjectFromMax(ServerContext* context, const EmptyParam* ep, YAreaList* yal)
{
	Invoke([ep, yal]() -> void {
		ObjectFromMax(yal);
	});
	return Status::OK;
}
//
Status YServiceImpl::NewProject(ServerContext* context, const NewProjectParam* np, ResponseNProject* rnp)
{
	Invoke([np, rnp]() -> void {
		//bool noProject = true;
		//for (int i = 0; i < YSys.projects_size(); i++)
		//{
		//	auto pi = YSys.projects(i);
		//	if ((pi.pname() == np->fname()) && (pi.project_path() == np->folder())) {
		//		rnp->mutable_pinfo()->CopyFrom(pi);
		//		LoadNProject(rnp);
		//		//MessageBoxW(NULL, L"Load project cu....(chua lam)", L"Oh", MB_OK);
		//		noProject = false;
		//		break;
		//	}
		//}
		//if (noProject) {
		//	rnp->mutable_pinfo()->set_project_path(np->folder());
		//	rnp->mutable_pinfo()->set_pname(np->fname());
		//	NewYProject(np, rnp);
		//}
		rnp->mutable_pinfo()->set_project_path(np->project_path());
		auto opath = fmt::format("{0}\\{1}.max", np->folder(), np->fname());
		logserver::LOG(opath);
		rnp->mutable_pinfo()->set_original_path(opath);
		rnp->mutable_pinfo()->set_pname(np->fname());
		NewYProject(np, rnp);
	});
	return Status::OK;
}

Status YServiceImpl::LoadProject(ServerContext* context, const ProjectInfo* pi, ResponseNProject* rnp)
{
	Invoke([pi, rnp]() -> void {
		rnp->mutable_pinfo()->CopyFrom(*pi);
		//rnp->mutable_pinfo()->set_path(np->folder());
		//rnp->mutable_pinfo()->set_pname(np->fname());
		LoadNProject(rnp);
	});
	return Status::OK;
}

Status YServiceImpl::DoAction(ServerContext* context, grpc::ServerReaderWriter<YEvent, YEvent>* stream)
{

	YEvent ee;
	while (stream->Read(&ee)) {
		Invoke([ee]() -> void {
			DoYEvent(ee);
		});
		//stream->Write(received_e[0]);
		/*		for (const YEvent& e : received_e) {
		stream->Write(e);
		}*/
		//received_e.clear();
	}
	return Status::OK;
}

Status YServiceImpl::DoStreamClient(ServerContext* context, grpc::ServerReader<YEvent>* stream, ResponseEvent* re)
{
	YEvent ee;
	while (stream->Read(&ee)) {
		Invoke([ee]() -> void {
			DoYEvent(ee);
		});
	}
	return Status::OK;
}

Status YServiceImpl::DoEvent(ServerContext* context, const YEvent* ye, ResponseEvent* re)
{
	Invoke([ye]() -> void {
		YEvent ee;
		ee.CopyFrom(*ye);
		DoYEvent(ee);
	});
	return Status::OK;
}

Status YServiceImpl::DoManyEvent(ServerContext* context, const YEventList* yel, ResponseEvent* re)
{
	Invoke([yel]() -> void {
		YEventList el;
		el.CopyFrom(*yel);
		for (auto i = 0; i < el.events().size(); i++)
		{
			DoYEvent(el.events(i));
		}
	});
	return Status::OK;
}

Status YServiceImpl::LowPoly(ServerContext* context, const ::y3d::ELowpoly* request, ::y3d::ResultReply* response) {
	Invoke([request]() -> void {
		do_lowpoly(*request);
	});
	return Status::OK;
}

Status YServiceImpl::Unwrap(ServerContext* context, const ::y3d::EUnwrap* request, ::y3d::ResultReply* response) {
	Invoke([request]() -> void {
		do_unwrap(*request);
	});
	return Status::OK;
}

Status YServiceImpl::Packing(ServerContext* context, const ::y3d::EPacking* request, ::y3d::ResultReply* response) {
	Invoke([request]() -> void {
		do_pack(*request);
	});
	return Status::OK;
}
//Status YServiceImpl::LoadSystem(ServerContext* context, const EmptyParam* ep, YSystem* ys)
//{
//	Invoke([ys]() -> void {
//		ys->CopyFrom(YSys);
//	});
//	return Status::OK;
//}
//
//Status YServiceImpl::SaveSystem(ServerContext* context, const YSystem* ys, ResultReply* reply)
//{
//	Invoke([ys]() -> void {
//		YSys.CopyFrom(*ys);
//	});
//	return Status::OK;
//}

Status YServiceImpl::CloneObject(::grpc::ServerContext* context, const ::y3d::EmptyParam* request, ::y3d::ResultReply* response)
{
	Invoke([]() {
		auto* ip = GetCOREInterface();
		INodeTab inodes;
		getSelNodeTab(inodes);
		if (inodes.Count() > 0) {
			INodeTab src, dst;
			Point3 p(10, 0, 0);
			bool ok = ip->CloneNodes(inodes, p, true, NODE_COPY, &src, &dst);
			if (ok) {
				for (int i = 0; i < dst.Count(); ++i) {
					log(L"New node {0} : {1}\n", i, dst[i]->GetName());
				}
			}
		}
		mprintf(L"Hello world\n");
	});
	response->set_message("Xin chao ngay mai. Haha, Ve Nghe An thoi");

	return Status::OK;
}

YServiceImpl::~YServiceImpl()
{
	unregisterCB();
}

ServicePtr APIENTRY getServiceImpl()
{
	return new YServiceImpl();
}

void APIENTRY releaseObject(ServicePtr obj)
{
	if (obj) delete obj;
}

YSERVICE_API ServiceSharedPtr APIENTRY getServicePtr()
{
	return std::shared_ptr<AbstractService>(new YServiceImpl);
}

void YServiceImpl::Initialize(void* codegen, void* gli)
{
	grpc::g_core_codegen_interface = (grpc::CoreCodegenInterface*)codegen;
	grpc::g_glip = (grpc::GrpcLibraryInterface*)gli;
	registerCB();
}

Status YServiceImpl::Shutdown(::grpc::ServerContext* context, const ::y3d::EmptyParam* request, ::y3d::ResultReply* response)
{	
	//MessageBoxW(NULL, L"sac sac", L"Oh", MB_OK);	
	Invoke([]() {
		mprintf(L"SHUTDOWN.\n");
		ExecuteMAXScriptScript(L"quitMAX #noPrompt");		
	});
	return Status::OK;
}