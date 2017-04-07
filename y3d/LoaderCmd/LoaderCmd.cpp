// LoaderCmd.cpp : Defines the entry point for the console application.
//
#include "yloader.pb.h"
#include "yloader.grpc.pb.h"
#include "y3d.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"
#include "grpc++/grpc++.h"
#include "tclap/CmdLine.h"
#include "tbb/task_group.h"
#include "LogClient.h"
#include <common.h>
#include "YLibs.h"

std::string master_ip = "127.0.0.1:38000";


int show_all_workers(int32_t stat = 2) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::YWorkerResponse rep;
	y3d::AllWorkerParam request;
	grpc::ClientContext context;
	request.set_status(stat);
	auto status = client->AllWorkers(&context, request, &rep);
	for (int i = 0; i < rep.wlist().workers_size(); i++)
	{
		auto yw = rep.wlist().workers(i);
		auto status = &ServingStatus_Name(yw.status());
		auto wt = y3d::WorkerType_Name(yw.wtype());
		printf("\n%d. %s  (ip:%s:%d) (status:%s) (type:%s)", (i + 1), yw.wname().c_str(), yw.machine_ip().c_str(), yw.port_loader(), status->c_str(),wt.c_str());
	}
	return status.error_code();
}

int start_all_workers() {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	grpc::ClientContext context;
	y3d::EmptyParam request;
	y3d::ResultReply resp;
	auto status = client->StartAllWorkers(&context, request, &resp);
	return resp.error();
}

int start_worker(int32_t id) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::WorkerParam req;
	req.set_wid(id);
	y3d::YWorkerResponse rep;
	grpc::ClientContext context;
	auto status = client->StartWorker(&context, req, &rep);
	return 0;
}

int start_worker(std::string wnamed) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::WorkerParam req;
	req.set_wname(wnamed.c_str());
	y3d::YWorkerResponse rep;
	grpc::ClientContext context;
	auto status = client->StartWorker(&context, req, &rep);
	return 0;
}


int stop_worker(int32_t id) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::WorkerParam req;
	req.set_wid(id);
	y3d::YWorkerResponse rep;
	grpc::ClientContext context;
	auto status = client->StopWorker(&context, req, &rep);
	return 0;
}

int stop_worker(std::string wnamed) {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	y3d::WorkerParam req;
	req.set_wname(wnamed.c_str());
	y3d::YWorkerResponse rep;
	grpc::ClientContext context;
	auto status = client->StopWorker(&context, req, &rep);
	return 0;
}

int stop_all_worker() {
	auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
	grpc::ClientContext context;
	y3d::EmptyParam request;
	y3d::ResultReply resp;
	auto status = client->StopAllWorkers(&context, request, &resp);
	return resp.error();
}

int test1() {
	tbb::task_group tg;
	for (int i = 0; i < 20; ++i) {
		tg.run([i]() {
			printf("Client number %d is connecting\n", i);
			for (int j = 0; j < 50; ++j) {
				auto client = y3d::YServiceMaster::NewStub(grpc::CreateChannel(master_ip, grpc::InsecureChannelCredentials()));
				grpc::ClientContext context;
				y3d::YWorkerRequest req;
				req.set_call_in_app(false);
				req.set_slient(true);
				y3d::YWorkerResponse rep;
				auto status = client->AddWorker(&context, req, &rep);
			}
		});		
	}
	tg.wait();
	return 0;
}

int test2() {
	tbb::task_group tg;
	for (int i = 0; i < 7; ++i) {
		tg.run([i]() {
			printf("Client number %d is connecting\n", i);
			for (int j = 0; j < 150; ++j) {
				logserver::LOG("Hello world with fmt like format i={},j={}\n", i,j);
				logserver::Printf("Hello world with printf like format i=%d,j=%d, %s\n", i, j,"successful! haha");
			}
		});
	}
	tg.wait();
	//LogClient logClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
	//logserver::LOG("Hello world {}\n",123);
	return 1;
}


void test3() {
	IsProcessIsRunning(L"LogServer.exe");
}

void loadDll()
{

}

void test4() {	
	//auto client = y3d::Tools::NewStub(grpc::CreateChannel(service_ip, grpc::InsecureChannelCredentials()));
	std::string ip = "127.0.0.1:39001";
	auto client = y3d::YServiceTest::NewStub(grpc::CreateChannel(ip, grpc::InsecureChannelCredentials()));
	grpc::ClientContext context;
	y3d::EmptyParam req;
	y3d::EmptyParam res;
	grpc::Status status = client->MTest1(&context, req, &res);
}

//int shutdownService() {
//	auto client = y3d::Tools::NewStub(grpc::CreateChannel(service_ip, grpc::InsecureChannelCredentials()));
//	grpc::ClientContext context;
//	y3d::EmptyParam request;	
//	y3d::ResultReply resp;
//	auto status = client->Shutdown(&context, request, &resp);
//	printf("Shutdown service %d\n", status.error_code());
//	return status.error_code();
//}

//int shutdown(int32_t id = 0) {
//	shutdownService();
//	auto channel = grpc::CreateChannel(ip, grpc::InsecureChannelCredentials());
//	auto client = y3d::YServiceMaxLoader::NewStub(channel);
//	grpc::ClientContext context;
//	y3d::LibInfo request;
//	request.set_id(id);
//	y3d::ResultType resp;
//	auto status = client->Shutdown(&context, request, &resp);
//
//	return resp.error();
//}

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

		TCLAP::ValueArg<std::string> workerArg("w", "worker", "stop = stop worker service; start = start worker service; stopall = stop all worker services", false, "start", "string");
		cmd.add(workerArg);

		TCLAP::ValueArg<int> lsArg("l", "list", "0 = inactive; 1 = ready; 2 = all", false, 2, "integer");
		cmd.add(lsArg);


		TCLAP::ValueArg<int> testArg("t", "test", "1 = test1; 2 = test2; 3 = test3; 4 = test4", false, 2, "integer");
		cmd.add(testArg);

		// Parse the argv array.
		cmd.parse(argc, argv);

		if (lsArg.isSet()) {
			auto list = lsArg.getValue();
			show_all_workers(list);
		}
		else if (workerArg.isSet()) {
			auto worker = workerArg.getValue();
			if (worker == "stopall") {
				stop_all_worker();
			}else if (worker == "startall") {
				start_all_workers();
			}else if (widArg.isSet()) {
				auto wid = widArg.getValue();
				if (worker == "start") {
					start_worker(wid);
				}
				else if (worker == "stop") {
					stop_worker(wid);
				}
			}
			else if (wnameArg.isSet()) {
				auto wname = wnameArg.getValue();
				if (worker == "start") {
					start_worker(wname);
				} 
				else if (worker == "stop") {
					stop_worker(wname);
				}
			}
		}
		else if (testArg.isSet()) {
			auto t = testArg.getValue();
			if (t == 1) {
				test1();
			}
			else if (t == 2) 
			{ 
				test2(); 
			}
			else if (t==3)
			{
				test3();
			}
			else if (t == 4)
			{
				test4();
			}
		}
	}



	catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}	
    return 0;
}

