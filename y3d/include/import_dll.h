#pragma once
#if !defined(Y3D_EXPORT_MACRO) || Y3D_EXPORT_MACRO >= 0
#undef Y3D_EXPORT_MACRO
#define Y3D_EXPORT_MACRO __declspec( dllimport )
#define Y3D_EXPORT_GLOBAL_MACRO __declspec( dllimport )
//printf("hello defined world")
#endif // !Y3D_EXPORT_MACRO
