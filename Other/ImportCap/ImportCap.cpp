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

#include "ImportCap.h"

#define ImportCap_CLASS_ID	Class_ID(0x6edec269, 0xd3c6a249)

BOOL ObjManipulatorRestore::mMode = FALSE;
BOOL CollapseRestore::mMode = FALSE;
static wchar_t* exportFolder = L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\";

class ImportCap : public SceneImport
{
public:
	//Constructor/Destructor
	ImportCap();
	virtual ~ImportCap();

	virtual int				ExtCount();					// Number of extensions supported
	virtual const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	virtual const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR *	AuthorName();				// ASCII Author name
	virtual const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	virtual const TCHAR *	OtherMessage1();			// Other message #1
	virtual const TCHAR *	OtherMessage2();			// Other message #2
	virtual unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	virtual void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	virtual int				DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	// Import file
};



class ImportCapClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new ImportCap(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_IMPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return ImportCap_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("ImportCap"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetImportCapDesc() { 
	static ImportCapClassDesc ImportCapDesc;
	return &ImportCapDesc; 
}




INT_PTR CALLBACK ImportCapOptionsDlgProc(HWND hWnd,UINT message,WPARAM ,LPARAM lParam) {
	static ImportCap* imp = nullptr;

	switch(message) {
		case WM_INITDIALOG:
			imp = (ImportCap *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- ImportCap -------------------------------------------------------
ImportCap::ImportCap()
{

}

ImportCap::~ImportCap() 
{

}

int ImportCap::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const TCHAR *ImportCap::Ext(int /*n*/)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("cap");
}

const TCHAR *ImportCap::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("Capnp file");
}
	
const TCHAR *ImportCap::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("Capnp");
}

const TCHAR *ImportCap::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("");
}

const TCHAR *ImportCap::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *ImportCap::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *ImportCap::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int ImportCap::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void ImportCap::ShowAbout(HWND /*hWnd*/)
{			
	// Optional
}

int ImportCap::DoImport(const TCHAR* filename, ImpInterface* ii, Interface* i, BOOL suppressPrompts)
{
	#pragma message(TODO("Implement the actual file import here and"))	

	/*if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				ImportCapOptionsDlgProc, (LPARAM)this);*/

	#pragma message(TODO("return TRUE If the file is imported properly"))

	ip = i;
	impip = ii;

	if (resetScene) {
		impip->NewScene();
	}

	int opts = ExportOpts::all;
	int lopts = ExportOpts::all;
	std::vector<int>* index = nullptr;
	mprintf(L"Ip = %d\n", i);
	mprintf(L"ImpIp = %d\n", ii);
	Time t;
	loadObjects((TCHAR*)filename, index, opts, L"");
	mprintf(L"Total Execution time = %d ms\n", t.duration());
	return TRUE;
}
	
