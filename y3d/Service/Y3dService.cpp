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

#include "common.h"
#include "Y3dService.h"
#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include <grpc++/impl/grpc_library.h>
#include <grpc++/impl/codegen/proto_utils.h>

#include "YCServer.h"

#define Service_CLASS_ID	Class_ID(0x768455e0, 0x74aca221)
const std::string MASTER_IP = "0.0.0.0:38000";

HMODULE dll = nullptr;
std::promise<void> exit_requested;
std::mutex loading_requested;
std::condition_variable ready_cv;
std::atomic<bool> isLoading = false;
std::atomic<bool> isShuttingdown = false;
std::condition_variable shutdown_cv;



DLLAPI void APIENTRY startService(const char* dllname, const char* ip_address)
{
	
	std::unique_lock<std::mutex> lk(loading_requested);
	if (isLoading) ready_cv.wait(lk, []() {return !isLoading; });
//	LOG.clear();
	if (isShuttingdown) return;//If there is a shuttingdown request, return

	bool notDone = false;

	if (!dll) {		
		//lk.unlock(); //Now release lock
		if (!isLoading.compare_exchange_strong(notDone, true) || isShuttingdown) return;
		std::thread t([dllname, ip_address]() {
			
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
			//LOG("DLL path is {0}\n", dll_path);
			//LOG("Xin chao");
					// load plugin and grab all the functions we need		
			HMODULE dllHandle = 0;
			
			try {				
					std::unique_lock<std::mutex> lk(loading_requested);
					SYSTEM_CALL(dll = LoadLibraryA(dll_path.c_str()));
					if (!dll) {						
						isLoading = false;
						return;
					}
				
					dllHandle = dll;
					SYSTEM_CALL(GetServiceImplFunc getServiceImpl = (GetServiceImplFunc)GetProcAddress(dll, "getServiceImpl"));
					SYSTEM_CALL(ReleaseObjectFunc releaseObject = (ReleaseObjectFunc)GetProcAddress(dll, "releaseObject"));
					if (!getServiceImpl || !releaseObject) {
						dll = nullptr;
						if (dllHandle) SYSTEM_CALL(FreeLibraryAndExitThread(dllHandle,0));
					
						return;
					}
					
				/*initSystem();
				registerCB();*/
				//MessageBox(NULL, _T("Open the message box "), _T("message"), MB_OK | MB_SYSTEMMODAL);		
				/*if (grpc::g_core_codegen_interface == nullptr) {
				grpc::g_core_codegen_interface = &grpc::internal::g_core_codegen;
				}*/
				std::string server_address(ip_address);
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
