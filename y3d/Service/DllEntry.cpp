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
// DESCRIPTION: Contains the Dll Entry stuff
// AUTHOR: 
//***************************************************************************/

#include "Y3dService.h"
#include "YLibs.h"


HINSTANCE hInstance;
int controlsInit = FALSE;

// This function is called by Windows when the DLL is loaded.  This 
// function may also be called many times during time critical operations
// like rendering.  Therefore developers need to be careful what they
// do inside this function.  In the code below, note how after the DLL is
// loaded the first time only a few statements are executed.

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID /*lpvReserved*/)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		MaxSDK::Util::UseLanguagePackLocale();
		// Hang on to this DLL's instance handle.
		hInstance = hinstDLL;

		//grpc::internal::GrpcLibraryInitializer m;

		wchar_t buf[256];
		auto nsize = GetModuleFileName(hInstance, buf, 256);
		std::wstring tmp(buf, nsize);
		LOG("DLL_PROCESS_ATTACH {0}\n", ws2s(tmp).c_str());
		isLoading = false;
		isShuttingdown = false;
		DisableThreadLibraryCalls(hInstance);
		// DO NOT do any initialization here. Use LibInitialize() instead.
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		wchar_t buf[256];
		auto nsize = GetModuleFileName(hInstance, buf, 256);
		std::wstring tmp(buf, nsize);
		LOG("DLL_PROCESS_DETACH {0}\n", ws2s(tmp).c_str());
	}
	return(TRUE);
}


TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, _countof(buf)) ? buf : NULL;
	return NULL;
}

