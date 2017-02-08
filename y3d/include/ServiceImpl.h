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

#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>

#include <grpc++/impl/codegen/core_codegen_interface.h>
#include <grpc++/impl/codegen/grpc_library.h>

//SIMPLE TYPE
//typedef int(*InitializeProcPtr)(void* codegen, void* gli);

//Y3D_EXPORT_MACRO int Initialize(void* codegen, void* gli);//This function used initialize static variable needed to run grpc in dll mode. See Service.cpp for an example

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;