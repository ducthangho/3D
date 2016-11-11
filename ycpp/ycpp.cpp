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

#include "ycpp.h"
#include "maxscript/maxscript.h"
#include <cstdio> // For printing and file access.
#include <string>
#include <fstream>
#include <chrono>
#include <locale>
#include <iostream>
#include <sstream>
#include <cstring>
#include "notify.h"


#define ycpp_CLASS_ID	Class_ID(0x68356c8d, 0x877e862a)


class ycpp : public GUP
{

	static void TimeUnitsChanged(void *param, NotifyInfo *info)
	{
		mprintf(L"Day la time unit change: duoc goi tu ben Dotnet");
	}

	static void MyCustom1(void *param, NotifyInfo *info) {
		mprintf(L"Day la CPP: duoc goi tu ben Dotnet");
	}
public:
	//Constructor/Destructor
	ycpp();
	virtual ~ycpp();

	// GUP Methods
	virtual DWORD     Start();
	virtual void      Stop();
	virtual DWORD_PTR Control(DWORD parameter);
	virtual void      DeleteThis();

	// Loading/Saving
	virtual IOResult Save(ISave* isave);
	virtual IOResult Load(ILoad* iload);

};



class ycppClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new ycpp(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return GUP_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return ycpp_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("ycpp"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetycppDesc() { 
	static ycppClassDesc ycppDesc;
	return &ycppDesc; 
}




ycpp::ycpp()
{
	RegisterNotification(TimeUnitsChanged, this, NOTIFY_TIMEUNITS_CHANGE);
	RegisterNotification(MyCustom1, this, NOTIFY_FILE_PRE_SAVE);
}

ycpp::~ycpp()
{
	UnRegisterNotification(TimeUnitsChanged, this, NOTIFY_TIMEUNITS_CHANGE);

}

void ycpp::DeleteThis()
{
	delete this;
}


// Activate and Stay Resident
DWORD ycpp::Start()
{
	mprintf(L"Xin chao dong bao day la YCPP /n");
	
	#pragma message(TODO("Do plugin initialization here"))
	#pragma message(TODO("Return if you want remain loaded or not"))
	return GUPRESULT_KEEP;
}

void ycpp::Stop()
{
	#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR ycpp::Control( DWORD /*parameter*/ )
{
	return 0;
}

IOResult ycpp::Save(ISave* /*isave*/)
{
	return IO_OK;
}

IOResult ycpp::Load(ILoad* /*iload*/)
{
	return IO_OK;
}

inline std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
