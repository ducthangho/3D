#pragma once

#include "YLibs.h"
#include "YCClient.h"
#ifdef __cplusplus
extern "C"
{
#endif
class YServiceImpl final : public Tools::Service {

	Status MakeNode4Edit(ServerContext* context, const Make4TestParam* request,
		ResultReply* reply) override
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
			auto lowStr = tmp+L"_low";
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

	Status RenameObject(ServerContext* context, const RenameParam* request,	ResultReply* reply) override
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

	Status BakeNormal(ServerContext* context, const ENormal* enm, ResultReply* reply) override
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

	Status BatchOptimize(ServerContext* context, const BatchOptimizeParam* bp,	ResultReply* reply) override
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

	Status GetObjectFromMax(ServerContext* context, const EmptyParam* ep, YAreaList* yal) override
	{
		Invoke([ep,yal]() -> void {
			ObjectFromMax(yal);
		});
		return Status::OK;
	}
	Status NewProject(ServerContext* context, const NewProjectParam* np, ResponseNProject* rnp) override
	{

		Invoke([np, rnp]() -> void {
			bool noProject = true;
			for (int i = 0; i < YSys.projects_size(); i++)
			{
				auto pi = YSys.projects(i);
				if ((pi.pname() == np->fname()) && (pi.path() == np->folder())) {
					rnp->mutable_pinfo()->CopyFrom(pi);
					LoadNProject(rnp);
					//MessageBoxW(NULL, L"Load project cu....(chua lam)", L"Oh", MB_OK);
					noProject = false;
					break;
				}
			}
			if (noProject) {
				rnp->mutable_pinfo()->set_path(np->folder());
				rnp->mutable_pinfo()->set_pname(np->fname());
				NewYProject(np, rnp);
			}

		});
		return Status::OK;
	}

	Status LoadProject(ServerContext* context, const ProjectInfo* pi, ResponseNProject* rnp) 
	{

		Invoke([pi, rnp]() -> void {
			//rnp->mutable_pinfo = pi;
			rnp->mutable_pinfo()->CopyFrom(*pi);
			//rnp->mutable_pinfo()->set_path(np->folder());
			//rnp->mutable_pinfo()->set_pname(np->fname());
			LoadNProject(rnp);
		});
		return Status::OK;
	}

	Status DeleteProject(ServerContext* context, const ProjectInfo* pi, ResponseNProject* rnp) override
	{

		Invoke([pi, rnp]() -> void {
			rnp->mutable_pinfo()->CopyFrom(*pi);
			DeleteYProject(rnp);
		});
		return Status::OK;
	}


	Status DoAction(ServerContext* context, grpc::ServerReaderWriter<YEvent, YEvent>* stream) override {
		
		YEvent ee;
		while (stream->Read(&ee)) {
			Invoke([ee]() -> void {
				DoYEvent(ee);
			});
			stream->Write(received_e[0]);
			/*		for (const YEvent& e : received_e) {
			stream->Write(e);
			}*/
				//received_e.clear();
		}
		return Status::OK;
	}
	Status DoStreamClient(ServerContext* context, grpc::ServerReader<YEvent>* stream, ResponseEvent* re) override {
		YEvent ee;
		while (stream->Read(&ee)) {
			Invoke([ee]() -> void {
				DoYEvent(ee);
			});
		}
		return Status::OK;
	}

	Status DoEvent(ServerContext* context, const YEvent* ye, ResponseEvent* re) override {
		Invoke([ye]() -> void {
			YEvent ee;
			ee.CopyFrom(*ye);
			DoYEvent(ee);
		});
		return Status::OK;
	}

	Status LoadSystem(ServerContext* context, const EmptyParam* ep, YSystem* ys) override {
		Invoke([ys]() -> void {
			ys->CopyFrom(YSys);
		});
		return Status::OK;
	}
	Status SaveSystem(ServerContext* context, const YSystem* ys, ResultReply* reply) override {
		Invoke([ys]() -> void {
			YSys.CopyFrom(*ys);
		});
		return Status::OK;
	}

	Status CloneObject(ServerContext* context, const ::y3d::EmptyParam* request, ::y3d::ResultReply* response) override
	{
		
		Invoke([]() {
			auto* ip = GetCOREInterface();
			INodeTab inodes;
			getSelNodeTab(inodes);
			if (inodes.Count() > 0) {
				INodeTab src, dst;
				Point3 p(10, 0, 0);
				bool ok = ip->CloneNodes(inodes, p, true, NODE_COPY,&src,&dst);
				if (ok) {
					for (int i = 0; i < dst.Count(); ++i) {						
						log(L"New node {0} : {1}\n",i, dst[i]->GetName());
					}
				}
			}
			
		});
		//response->set_message("ABCD");
		return Status::OK;
	}

	
};

class YSWorkerImpl final : public YSWorkers::Service {
	Status AddWorker(ServerContext* context, const ::y3d::YWorkerRequest* req, ::y3d::YWorkerResponse* yw_res) override {
		Invoke([req, yw_res]() -> void {
			auto new_worker = YWList.add_workers();
			new_worker->set_wid(YWList.workers_size());
			string new_addr = fmt::format(FM_SERVER_ADDR, new_worker->wid() + PORT_MASTER);
			string wname = fmt::format("Worker {}", new_worker->wid());
			new_worker->set_wname(wname.c_str());
			new_worker->set_ip_address(new_addr.c_str());
			new_worker->set_status(YWorker_ServingStatus::YWorker_ServingStatus_UNKNOWN);
			if (req->call_in_app()) { //call from running worker app
				new_worker->set_status(YWorker_ServingStatus::YWorker_ServingStatus_SERVING);
				//MessageBoxW(NULL, s2ws(new_addr).c_str(), L"Oh", MB_OK);
			}
			else {
				if (!req->slient()) {
					auto r = system(req->app().path_run().c_str());
					//run app here
				}
				else {
					new_worker->set_status(YWorker_ServingStatus::YWorker_ServingStatus_NOT_SERVING);
				}
			}
			yw_res->mutable_wlist()->CopyFrom(YWList);
			yw_res->mutable_new_worker()->CopyFrom(*new_worker);
		});
		return Status::OK;
		//new_worker->id
	}

	Status AllWorkers(ServerContext* context, const ::y3d::EmptyParam* req, ::y3d::YWorkerList* yw_res) override {
		Invoke([req, yw_res]() -> void {
			if (!YWList.has_master()) {
				auto* m = YWList.mutable_master();
				m->set_wname("Master");
				m->set_ip_address(fmt::format(FM_SERVER_ADDR, PORT_MASTER));
				m->set_status(YWorker_ServingStatus::YWorker_ServingStatus_SERVING);
			}
			yw_res->CopyFrom(YWList);
		});
		return Status::OK;
		//new_worker->id
	}
};


//inline void addWorkerClient() {
//	string master_addr = fmt::format("localhost:{}", PORT_MASTER);
//	YSWorkersClient workerClient(grpc::CreateChannel(
//		master_addr , grpc::InsecureChannelCredentials()));
//}

inline void initServer() {
	string master_addr = fmt::format(FM_SERVER_ADDR, PORT_MASTER);
	YServiceImpl service_tools;
	YSWorkerImpl service_worker;
	grpc::ServerBuilder builder;
	builder.AddListeningPort(master_addr, grpc::InsecureServerCredentials());
	builder.RegisterService(&service_tools);
	builder.RegisterService(&service_worker);
	std::unique_ptr<Server> server(builder.BuildAndStart());

	if (server != NULL) { // master server
		try {
			initSystem();
			registerCB();
			//log("YServer MASTER listening on 0.0.0.0:{}", PORT_MASTER);
			server->Wait();
		}
		catch (std::exception& e) {
			mprintf(L"Exception %s\n", e.what());
		};
	}
	else {
		string master_addr = fmt::format("localhost:{}", PORT_MASTER);
		YSWorkersClient workerClient(grpc::CreateChannel(master_addr, grpc::InsecureChannelCredentials()));
		auto ww = workerClient.startNewWorker();
		grpc::ServerBuilder builder2;
		builder2.AddListeningPort(ww.new_worker().ip_address(), grpc::InsecureServerCredentials());
		builder2.RegisterService(&service_tools);
		builder2.RegisterService(&service_worker);
		std::unique_ptr<Server> server2(builder2.BuildAndStart());
		if (server2 != NULL) { // worker server
			try {
				//MessageBoxW(NULL, s2ws(ww.new_worker().ip_address()).c_str(), L"zzz", MB_OK);
				//YWList.mutable_workers()->Mutable(YWList.workers_size())->set_status(YWorker_ServingStatus::YWorker_ServingStatus_SERVING);
				server2->Wait();
				//log("YServer worker listening on {}",ww.new_worker().ip_address().c_str());
			}
			catch (std::exception& e) {
				//MessageBoxW(NULL, L"err", L"zzz", MB_OK);
				//YWList.mutable_workers()->Mutable(YWList.workers_size())->set_status(YWorker_ServingStatus::YWorker_ServingStatus_NOT_SERVING);
				mprintf(L"Exception %s\n", e.what());
			};
		}
		else {
			//YWList.mutable_workers()->Mutable(YWList.workers_size())->set_status(YWorker_ServingStatus::YWorker_ServingStatus_NOT_SERVING);
			//log("Can not start server on {}", ww.new_worker().ip_address().c_str());
		}
		// get port from master server

	}
}

#ifdef __cplusplus
}
#endif