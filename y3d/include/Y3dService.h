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


#include <tchar.h>
#include <condition_variable>
#include <future>
#include <mutex>

//SIMPLE TYPE


#include <windows.h>

//#define GPR_INT64_MAX (gpr_int64)(~(gpr_uint64)0 >> 1)
extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;
#ifdef Y3DSERVICE_EXPORT
#define DLLAPI __declspec( dllexport )   
#else
#define DLLAPI __declspec( dllimport )   
#endif

typedef void(APIENTRY *StartServiceFunc)(std::string,std::string);
typedef void(APIENTRY *StopServiceFunc)();

#ifndef _USE_DYNAMIC_LOADED_DLL_
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	DLLAPI extern std::promise<void> exit_requested;
	DLLAPI extern std::mutex loading_requested;
	DLLAPI extern std::atomic<bool> isLoading;
	DLLAPI extern std::atomic<bool> isShuttingdown;
	DLLAPI extern std::condition_variable shutdown_cv;
	DLLAPI extern std::condition_variable ready_cv;
	DLLAPI void APIENTRY startService(const char* dllname = "ServiceImpl.dll", const char* ip_address="0.0.0.0.38001");
	DLLAPI void APIENTRY stopService();
#ifdef __cplusplus
}
#endif // __cplusplus
#endif