//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "Service.h"
#include "maxscript/maxscript.h"
#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>
#include "ymax.grpc.pb.h"
#include "yservice.grpc.pb.h"

#include "YProjectUtils.h"
#include "YCServer.h"

#define Service_CLASS_ID	Class_ID(0x768455e0, 0x74aca221)


class Service : public GUP
{
public:
	//Constructor/Destructor
	Service();
	virtual ~Service();

	// GUP Methods
	virtual DWORD     Start();
	virtual void      Stop();
	virtual DWORD_PTR Control(DWORD parameter);
	virtual void      DeleteThis();

	// Loading/Saving
	virtual IOResult Save(ISave* isave);
	virtual IOResult Load(ILoad* iload);
};



class ServiceClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/);
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return GUP_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return Service_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("Service"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetServiceDesc() { 
	static ServiceClassDesc ServiceDesc;
	return &ServiceDesc; 
}


Service::Service()
{

}

Service::~Service()
{

}

void Service::DeleteThis()
{
	delete this;
}

// Activate and Stay Resident
DWORD Service::Start()
{
	#pragma message(TODO("Do plugin initialization here"))
	#pragma message(TODO("Return if you want remain loaded or not"))
	mprintf(L"Starting YService for 3ds Max ...");	
	mprintf(L"Server listening on 0.0.0.0:50051");
	std::thread t([]() {

		initSystem();
		registerCB();
		//MessageBox(NULL, _T("Open the message box "), _T("message"), MB_OK | MB_SYSTEMMODAL);

		std::string server_address("0.0.0.0:50051");
		YServiceImpl service;
		grpc::ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		// register "service" as the instance through which we'll communicate with
		// clients. in this case it corresponds to an *synchronous* service.
		builder.RegisterService(&service);
		// finally assemble the server.
		std::unique_ptr<Server> server(builder.BuildAndStart());
		//std::cout << "server listening on " << server_address << std::endl;





		// wait for the server to shutdown. note that some other thread must be
		// responsible for shutting down the server for this call to ever return.
		//do {
		try {
			server->Wait();
		}
		catch (std::exception& e) {
			mprintf(L"Exception %s\n", e.what());
		};
		//} while (1);

	});
	t.detach();
	return GUPRESULT_KEEP;
}

void Service::Stop()
{
	#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR Service::Control( DWORD /*parameter*/ )
{
	return 0;
}

IOResult Service::Save(ISave* /*isave*/)
{
	return IO_OK;
}

IOResult Service::Load(ILoad* /*iload*/)
{
	return IO_OK;
}

inline void * ServiceClassDesc::Create(BOOL) { return new Service(); }
