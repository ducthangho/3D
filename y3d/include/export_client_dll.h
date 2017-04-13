#pragma once

#ifndef __LIBPROTOBUF
#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotocd.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")
#endif
#define __LIBPROTOBUF
#endif

#if !defined(GRPC_CLIENT_EXPORT_MACRO) || GRPC_CLIENT_EXPORT_MACRO >= 0
#undef GRPC_CLIENT_EXPORT_MACRO
#define GRPC_CLIENT_EXPORT_MACRO __declspec( dllexport )
#define GRPC_CLIENT_EXPORT_GLOBAL_MACRO __declspec( dllexport ) extern
//printf("hello defined world")
#endif // !Y3D_EXPORT_MACRO
