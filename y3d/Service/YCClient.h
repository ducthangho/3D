#pragma once

#include "YLibs.h"
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using y3d::YSWorkers;

class YSWorkersClient {
public:
	YSWorkersClient(std::shared_ptr<Channel> channel)
		: stub_(YSWorkers::NewStub(channel)) {}

	YWorkerResponse startNewWorker() {
		YWorkerRequest req;
		req.set_call_in_app(true);
		req.set_slient(true);
		ClientContext context;
		YWorkerResponse reply;
		Status status = stub_->AddWorker(&context, req, &reply);
		if (status.ok()) {
			return reply;
			//// start new server with new address
			//YServiceImpl stools;
			//YSWorkerImpl sworker;
			//grpc::ServerBuilder builder;
			//builder.AddListeningPort(reply.new_worker().ip_address(), grpc::InsecureServerCredentials());
			//builder.RegisterService(&stools);
			//builder.RegisterService(&sworker);
			//std::unique_ptr<Server> server(builder.BuildAndStart());
			//if (server != NULL) { // master server
			//	try {
			//		server->Wait();
			//		log("YServer worker listening on "+reply.new_worker().ip_address());
			//	}
			//	catch (std::exception& e) {
			//		mprintf(L"Exception %s\n", e.what());
			//	};
			//}
			//else {
			//	log("Can not start server on " + reply.new_worker().ip_address());
			//}
		}
		else {
			reply.set_error("error");
			return reply;
		}
	}
 private:
	 std::unique_ptr<YSWorkers::Stub> stub_;
};

//class ToolsClient {
//public:
//	ToolsClient(std::shared_ptr<Channel> channel, const std::string& db)
//		: stub_(y3d::Tools::NewStub(channel)) {});
//}
//};

