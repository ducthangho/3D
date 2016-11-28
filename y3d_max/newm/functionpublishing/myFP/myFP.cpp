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

#include "myFP.h"
#include "maxscript/maxscript.h"
//#include "IPathConfigMgr.h"
#include "maxapi.h"
#include "tab.h"

#define myFP_CLASS_ID	Class_ID(0x49df0319, 0x727841e7)

class myFP : public GUP
{
public:
	//Constructor/Destructor
	myFP();
	virtual ~myFP();

	// GUP Methods
	virtual DWORD     Start();
	virtual void      Stop();
	virtual DWORD_PTR Control(DWORD parameter);
	virtual void      DeleteThis();

	// Loading/Saving
	virtual IOResult Save(ISave* isave);
	virtual IOResult Load(ILoad* iload);
};



class myFPClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new myFP(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return GUP_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return myFP_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("myFP"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetmyFPDesc() { 
	static myFPClassDesc myFPDesc;
	return &myFPDesc; 
}




myFP::myFP()
{

}

myFP::~myFP()
{

}

void myFP::DeleteThis()
{
	delete this;
}

// Activate and Stay Resident
DWORD myFP::Start()
{
	#pragma message(TODO("Do plugin initialization here"))
	#pragma message(TODO("Return if you want remain loaded or not"))

	//mprintf(L"Hello world\n");
	return GUPRESULT_KEEP;
}

void myFP::Stop()
{
	#pragma message(TODO("Do plugin un-initialization here"))
}

DWORD_PTR myFP::Control( DWORD /*parameter*/ )
{
	return 0;
}

IOResult myFP::Save(ISave* /*isave*/)
{
	return IO_OK;
}

IOResult myFP::Load(ILoad* /*iload*/)
{
	return IO_OK;
}

class FactorialStaticInterface : public FPInterfaceDesc
{
public:

	static const Interface_ID id;

	enum FunctionIDs
	{
		_factorial,
		_print,
		_print_array
	};

	FactorialStaticInterface()
		: FPInterfaceDesc(id, _M("IFactorial"), 0, NULL, FP_CORE, p_end )
	{
		//mprintf(L"Hello world\n");
		AppendFunction(
			_factorial,      /* function ID */
			_M("fact"),     /* internal name */
			0,              /* function name string resource name */
			TYPE_INT,       /* Return type */
			0,              /* Flags */
			1,              /* Number of arguments */
			_M("x"),    /* argument internal name */
			0,          /* argument localizable name string resource id */
			TYPE_INT,   /* arg type */
			p_end
		);

		AppendFunction(
			_print,      /* function ID */
			_M("print"),     /* internal name */
			0,              /* function name string resource name */
			TYPE_INT,       /* Return type */
			0,              /* Flags */
			1,              /* Number of arguments */
			_M("x"),    /* argument internal name */
			0,          /* argument localizable name string resource id */
			TYPE_STRING,   /* arg type */
			p_end
		);

		AppendFunction(
			_print_array,      /* function ID */
			_M("print_array"),     /* internal name */
			0,              /* function name string resource name */
			TYPE_BOOL,       /* Return type */
			0,              /* Flags */
			1,              /* Number of arguments */
			_M("x"),    /* argument internal name */
			0,          /* argument localizable name string resource id */
			TYPE_FLOAT_TAB_BV,   /* arg type */
			p_end
		);
	}

	static int Factorial(int x) {
		return x <= 1 ? 1 : x * Factorial(x - 1);
	}

	static int Print(const TCHAR*  x) {
		mprintf(L"The string is %s\n", x);
		return 1;
	}

	static bool PrintArray(Tab<float>&  x) {
		for (int i = 0; i < x.Count(); ++i) {
			mprintf(L"x[%d] = %f\n", i, x[i]);
		};
		return true;
	}

	BEGIN_FUNCTION_MAP
		FN_1(_factorial, TYPE_INT, Factorial, TYPE_INT);
		FN_1(_print, TYPE_INT, Print, TYPE_STRING);
		FN_1(_print_array, TYPE_BOOL, PrintArray, TYPE_FLOAT_TAB_BV);
	END_FUNCTION_MAP
};

const Interface_ID FactorialStaticInterface::id = Interface_ID(0x497a06a4, 0x1c753305);
static FactorialStaticInterface factorial;