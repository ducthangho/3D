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
#if !defined(LOG_EXPORT_MACRO) || LOG_EXPORT_MACRO >= 0
#undef LOG_EXPORT_MACRO
#define LOG_EXPORT_MACRO 
#define LOG_EXPORT_GLOBAL_MACRO 
//printf("hello defined world")
#endif // !Y3D_EXPORT_MACRO
