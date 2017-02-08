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

#include "Loader.h"
#include "Y3dService.h"
#include "maxscript/maxscript.h"
#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>

#include "YCServer.h"
#include <iparamb2.h>
#include <utilapi.h>
#include <grpc++/impl/grpc_library.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <plugin.h>

#define Service_CLASS_ID	Class_ID(0x768455e0, 0x74aca221)
const std::string LOADER_IP = "0.0.0.0:50050";
const std::string SERVER_IP = "0.0.0.0:50051";
static const char *dll_fname = "SBMExporter.dle";

typedef int(*PluginInitFunc)();
typedef int(*PluginShutdownFunc)();
typedef ClassDesc *(*PluginClassDescFunc)(int);

HMODULE dll = nullptr;
std::promise<void> exit_requested;
std::mutex loading_requested;
std::condition_variable ready_cv;
std::atomic<bool> isLoading = false;
std::atomic<bool> isShuttingdown = false;
std::condition_variable shutdown_cv;


//__declspec(dllimport) int Initialize(void* codegen, void* gli);

class Y3dService : public GUP
{
public:
	//Constructor/Destructor
	Y3dService();
	virtual ~Y3dService();

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

	virtual const TCHAR* InternalName() 			{ return _T("Y3dService"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetServiceDesc() { 
	static ServiceClassDesc ServiceDesc;
	return &ServiceDesc; 
}

DLLAPI void APIENTRY startService(const char* dllname /*= "ServiceImpl.dll"*/)
{
	std::unique_lock<std::mutex> lk(loading_requested);
	if (isLoading) ready_cv.wait(lk, []() {return !isLoading; });
//	LOG.clear();
	if (isShuttingdown) return;//If there is a shuttingdown request, return

	bool notDone = false;

	if (!dll) {		
		//lk.unlock(); //Now release lock
		if (!isLoading.compare_exchange_strong(notDone, true) || isShuttingdown) return;
		std::thread t([dllname]() {
			//std::unique_lock<std::mutex> lock(loading_requested);
			if (dll) {
				isLoading = false;
				return;
			}
			auto* tmp = find_dll_dir();
			std::string dll_path;
			if (tmp) {
				std::string plugdir = tmp;
				dll_path = plugdir + "\\" + dllname;
			}
			else {
				dll_path = dllname;
			}

					LOG("DLL path is {0}\n", dll_path);
					// load plugin and grab all the functions we need		
			
			HMODULE dllHandle = 0;
			try {				
					std::unique_lock<std::mutex> lk(loading_requested);
					dll = LoadLibraryA(dll_path.c_str());
					if (!dll) {
						LOG("Failed to load exporter: {0}\n", dll_path);
						isLoading = false;
						return;
					}
				
					dllHandle = dll;
					GetServiceImplFunc getServiceImpl = (GetServiceImplFunc)GetProcAddress(dll, "getServiceImpl");
					ReleaseObjectFunc releaseObject = (ReleaseObjectFunc)GetProcAddress(dll, "releaseObject");
					if (!getServiceImpl || !releaseObject) {
						dll = nullptr;
						if (dllHandle) FreeLibraryAndExitThread(dllHandle,0);
					
						return;
					}

				/*initSystem();
				registerCB();*/
				//MessageBox(NULL, _T("Open the message box "), _T("message"), MB_OK | MB_SYSTEMMODAL);		
				/*if (grpc::g_core_codegen_interface == nullptr) {
				grpc::g_core_codegen_interface = &grpc::internal::g_core_codegen;
				}*/
			
				std::string server_address(SERVER_IP);
				//ServicePtr service = getServiceImpl();			
				std::shared_ptr<AbstractService> servicePtr(getServiceImpl(), releaseObject);
				servicePtr->Initialize(grpc::g_core_codegen_interface, grpc::g_glip);//This is a must have function, used to initialize static global function of grpc
				grpc::ServerBuilder builder;
				builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
				// register "service" as the instance through which we'll communicate with
				// clients. in this case it corresponds to an *synchronous* service.
				builder.RegisterService(servicePtr.get() );
				// finally assemble the server.
				std::unique_ptr<Server> server(builder.BuildAndStart());

				//std::cout << "server listening on " << server_address << std::endl;		
				// wait for the server to shutdown. note that some other thread must be
				// responsible for shutting down the server for this call to ever return.
				//do {

			
				//service->Helloworld();
				//server->Wait();

				auto serveFn = [&]() {
					server->Wait();
				};

				std::thread serving_thread(serveFn);
				isLoading = false;
				lk.unlock();
				ready_cv.notify_all();
				//lock.release();
				auto f = exit_requested.get_future();
				f.wait();
				isShuttingdown = true;
				server->Shutdown();
				serving_thread.join();
				exit_requested.swap(std::promise<void>());//Reset exit_requested promise								
			}
			catch (std::exception& e) {
				std::wstring s = s2ws(fmt::format("Exception {}\n", e.what()));
				MessageBox(NULL, s.c_str(), L"Exception", MB_OK);
			};

			dll = nullptr;
			if (dllHandle) {
				auto fFreeResult = FreeLibrary(dllHandle);
				if (!fFreeResult) {
					if (!UnmapViewOfFile(dllHandle))
						LOG("Cannot unload dll\n");
				}
				isShuttingdown = false;
				shutdown_cv.notify_all();
			}
			//releaseObject(service);
			//} while (1);
			//*/
		});
		t.detach();
		if (isLoading) {
			//Wait until the Server is up before continuing;
			//lk.lock();
			ready_cv.wait(lk, []() {return !isLoading; });
		}
	}
}

DLLAPI void APIENTRY stopService()
{
	exit_requested.set_value();
	std::unique_lock<std::mutex> lk(loading_requested);
	if (dll) {
		bool notDone = false;
		isLoading = false;
		isShuttingdown.compare_exchange_strong(notDone, true);
		shutdown_cv.wait(lk);
		isShuttingdown = false;
		dll = nullptr;
	}
	

}

Y3dService::Y3dService()
{

}

Y3dService::~Y3dService()
{
}

void Y3dService::DeleteThis()
{
	delete this;
}

// Activate and Stay Resident
DWORD Y3dService::Start()
{
	#pragma message(TODO("Do plugin initialization here"))
	#pragma message(TODO("Return if you want remain loaded or not"))
	/*grpc::internal::GrpcLibraryInitializer g_gli_initializer;
	g_gli_initializer.summon();*/	
	mprintf(L"Starting YService for 3ds Max ...\n");	
	mprintf(L"Server listening on 0.0.0.0:50051\n");
		
	std::thread t1([]() {

		/*initSystem();
		registerCB();*/
		//MessageBox(NULL, _T("Open the message box "), _T("message"), MB_OK | MB_SYSTEMMODAL);		
		/*if (grpc::g_core_codegen_interface == nullptr) {
		grpc::g_core_codegen_interface = &grpc::internal::g_core_codegen;
		}*/
		DbgAssert(grpc::g_glip != nullptr);
		DbgAssert(grpc::g_core_codegen_interface != nullptr);
		std::string server_address(LOADER_IP);
		//ServicePtr service = getServiceImpl();			
		auto servicePtr = getLoaderServicePtr();
		servicePtr->Initialize(grpc::g_core_codegen_interface, grpc::g_glip);//This is a must have function, used to initialize static global function of grpc
		grpc::ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		// register "service" as the instance through which we'll communicate with
		// clients. in this case it corresponds to an *synchronous* service.
		builder.RegisterService(servicePtr.get());
		// finally assemble the server.
		std::unique_ptr<Server> server(builder.BuildAndStart());

		//std::cout << "server listening on " << server_address << std::endl;		
		// wait for the server to shutdown. note that some other thread must be
		// responsible for shutting down the server for this call to ever return.
		//do {
		try {
			//service->Helloworld();
			server->Wait();
		}
		catch (std::exception& e) {
			std::wstring s = s2ws(fmt::format("Exception {}\n", e.what()));
			MessageBox(NULL, s.c_str(), L"Exception", MB_OK);
		};
		//releaseObject(service);
		//} while (1);
		//*/
	});
	t1.detach();
	return GUPRESULT_KEEP;
}

void Y3dService::Stop()
{
	#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR Y3dService::Control( DWORD /*parameter*/ )
{
	return 0;
}

IOResult Y3dService::Save(ISave* /*isave*/)
{
	return IO_OK;
}

IOResult Y3dService::Load(ILoad* /*iload*/)
{
	return IO_OK;
}

inline void * ServiceClassDesc::Create(BOOL) { return new Y3dService(); }
