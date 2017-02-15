// LoaderCmd.cpp : Defines the entry point for the console application.
//


#include "yloader.pb.h"
#include "yloader.grpc.pb.h"
#include "y3d.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"
#include "grpc++/grpc++.h"
#include "tclap/CmdLine.h"

std::string master_ip = "127.0.0.1:38000";
std::string ip = "localhost:50050";
std::string service_ip = "localhost:50051";


int loadDll(int32_t id = 0) {
	auto client = y3d::YServiceMaxLoader::NewStub(grpc::CreateChannel(ip, grpc::InsecureChannelCredentials()));
	grpc::ClientContext context;
	y3d::LibInfo request;
	request.set_id(id);
	y3d::ResultType resp;
	auto status = client->LoadDll(&context, request, &resp);
	return resp.error();
	//printf("Execution result = %d\n", status.error_code());
}

int show_all_workers(int32_t stat = 2) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::YWorkerList ywl;
	y3d::AllWorkerParam request;
	grpc::ClientContext context;
	request.set_status(stat);
	auto status = client->AllWorkers(&context, request, &ywl);
	for (int i = 0; i < ywl.workers_size(); i++)
	{
		auto yw = ywl.workers(i);
		printf("\n%d. %s  (ip:%s)", (i + 1), yw.wname(), yw.ip_address());
	}
	return status.error_code();
}

int start_worker(int32_t id) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::WorkerParam req;
	req.set_wid(id);
	y3d::ResultReply resp;
	grpc::ClientContext context;
	auto status = client->StartWorker(&context, req, &resp);
	return resp.error();
}

int start_worker(std::string wnamed) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::WorkerParam req;
	req.set_wname(wnamed.c_str());
	y3d::ResultReply resp;
	grpc::ClientContext context;
	auto status = client->StartWorker(&context, req, &resp);
	return resp.error();
}

int shutdownService() {
	auto channel = grpc::CreateChannel(service_ip, grpc::InsecureChannelCredentials());
	auto client = y3d::Tools::NewStub(channel);
	grpc::ClientContext context;
	y3d::EmptyParam request;	
	y3d::ResultReply resp;
	auto status = client->Shutdown(&context, request, &resp);
	printf("Shutdown service %d\n", status.error_code());
	return status.error_code();
}

int shutdown(int32_t id = 0) {
	shutdownService();
	auto channel = grpc::CreateChannel(ip, grpc::InsecureChannelCredentials());
	auto client = y3d::YServiceMaxLoader::NewStub(channel);
	grpc::ClientContext context;
	y3d::LibInfo request;
	request.set_id(id);
	y3d::ResultType resp;
	auto status = client->Shutdown(&context, request, &resp);

	return resp.error();
}

//class LoaderClient

int main(int argc, char** argv)
{

	// Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems.
	try {
		// Define the command line object, and insert a message
		// that describes the program. The "Command description message" 
		// is printed last in the help text. The second argument is the 
		// delimiter (usually space) and the last one is the version number. 
		// The CmdLine object parses the argv array based on the Arg objects
		// that it contains. 
		TCLAP::CmdLine cmd("Command description message", ' ', "1.0");

		// Define a value argument and add it to the command line.
		// A value arg defines a flag and a type of value that it expects,
		// such as "-n Bishop".
		TCLAP::ValueArg<int> widArg("i", "wid", "Worker ID", false, 0, "integer");
		cmd.add(widArg);

		TCLAP::ValueArg<std::string> wnameArg("n", "wname", "Worker name", false, "", "string");
		cmd.add(wnameArg);

		TCLAP::ValueArg<std::string> workerArg("w", "worker", "stop = stop worker service; start = start worker service", false, "start", "string");
		cmd.add(workerArg);

		TCLAP::ValueArg<int> lsArg("l", "list", "0 = inactive; 1 = ready; 2 = all", false, 2, "integer");
		cmd.add(lsArg);

		// Parse the argv array.
		cmd.parse(argc, argv);

		if (lsArg.isSet()) {
			auto list = lsArg.getValue();
			show_all_workers(list);
		}
		else if (workerArg.isSet()) {
			auto worker = workerArg.getValue();
			if (widArg.isSet()) {
				auto wid = widArg.getValue();
				if (worker == "start") {
					start_worker(wid);
				}
			}
			else if (wnameArg.isSet()) {
				auto wname = wnameArg.getValue();
				if (worker == "start") {
					start_worker(wname);
				}
			}
		}
	}



	catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}	
    return 0;
}

