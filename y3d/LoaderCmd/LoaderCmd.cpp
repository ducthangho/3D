// LoaderCmd.cpp : Defines the entry point for the console application.
//


#include "yloader.pb.h"
#include "yloader.grpc.pb.h"
#include "y3d.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"
#include "grpc++/grpc++.h"
#include "tclap/CmdLine.h"

std::string ip = "localhost:50050";
std::string service_ip = "localhost:50051";

int loadDll(int32_t id = 0) {
	auto channel = grpc::CreateChannel(ip, grpc::InsecureChannelCredentials());
	auto client = y3d::Loader::NewStub(channel);
	grpc::ClientContext context;
	y3d::LibInfo request;
	request.set_id(id);
	y3d::ResultType resp;
	auto status = client->LoadDll(&context, request, &resp);

	return resp.error();
	//printf("Execution result = %d\n", status.error_code());
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
	auto client = y3d::Loader::NewStub(channel);
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
	TCLAP::ValueArg<std::string> ipArg("i", "ip", "IP address of the server", false, "localhost:50050", "string");
	cmd.add(ipArg);
	TCLAP::ValueArg<int> loadArg("l", "status", "0 = shutdown; 1 = load", true, 0, "integer");
	cmd.add(loadArg);

	// Parse the argv array.
	cmd.parse(argc, argv);
	ip = ipArg.getValue();
	auto status = loadArg.getValue();
	if (status == 0)
		shutdown();
	else if (status == 1)
		loadDll();
	}
	catch (TCLAP::ArgException &e)  // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}	
    return 0;
}

