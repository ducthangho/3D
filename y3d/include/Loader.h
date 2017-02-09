#pragma once

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
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

#include <grpc++/impl/codegen/core_codegen_interface.h>
#include <grpc++/impl/codegen/grpc_library.h>

#include "Y3dService.h"
#include "yloader.pb.h"
#include "yloader.grpc.pb.h"

#ifdef YSERVICE_EXPORT
#define YSERVICE_API __declspec( dllexport )   
#else
#define YSERVICE_API __declspec( dllimport )   
#endif

class LoaderAbstractService : public y3d::Loader::Service {
public:
	virtual void Initialize(void* codegen, void* gli) = 0;//This function used initialize static variable needed to run grpc in dll mode. See Service.cpp for an example
	virtual ::grpc::Status Shutdown(::grpc::ServerContext* context, const ::y3d::LibInfo* request, ::y3d::ResultType* response) = 0;
	virtual ::grpc::Status LoadDll(::grpc::ServerContext* context, const ::y3d::LibInfo* request, ::y3d::ResultType* response) = 0;
};

class YLoaderImpl final : public LoaderAbstractService {
public:

	virtual void Initialize(void* codegen, void* gli) override;//This function used initialize static variable needed to run grpc in dll mode. See Service.cpp for an example
	virtual ::grpc::Status Shutdown(::grpc::ServerContext* context, const ::y3d::LibInfo* request, ::y3d::ResultType* response) override;
	virtual ::grpc::Status LoadDll(::grpc::ServerContext* context, const ::y3d::LibInfo* request, ::y3d::ResultType* response) override;

};

typedef LoaderAbstractService* LoaderPtr;
typedef std::shared_ptr<LoaderAbstractService> LoaderSharedPtr;

YSERVICE_API LoaderPtr APIENTRY getLoaderImpl();
YSERVICE_API void APIENTRY releaseObject(LoaderPtr obj);
YSERVICE_API LoaderSharedPtr APIENTRY getLoaderServicePtr();

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;
