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
//				3DXI file exporter project template
//				For a more in-depth exemple of a 3DXI exporter,
//				please refer to maxsdk\samples\igame\export.
// AUTHOR:		Jean-Francois Yelle - created Mar.20.2007
//***************************************************************************/

#include "ExportCap.h"

#include "3dsmaxsdk_preinclude.h"
#include "IGame/IGame.h"
#include "polyobj.h"
#include "triobj.h"
#include <chrono>

#define ExportCap_CLASS_ID	Class_ID(0x5b685b50, 0x77b7a6de)

BOOL ObjManipulatorRestore::mMode = FALSE;
BOOL CollapseRestore::mMode = FALSE;
static wchar_t* exportFolder = L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\";

class ExportCap : public SceneExport 
{
	public:
		
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

		virtual BOOL SupportsOptions(int ext, DWORD options);
		virtual int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=TRUE, DWORD options=0);

		//Constructor/Destructor
		ExportCap();
		virtual ~ExportCap();

	protected:
		int GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode = NULL);
};



class ExportCapClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new ExportCap(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return ExportCap_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("ExportCap"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetExportCapDesc() { 
	static ExportCapClassDesc ExportCapDesc;
	return &ExportCapDesc; 
}





INT_PTR CALLBACK ExportCapOptionsDlgProc(HWND hWnd,UINT message,WPARAM /*wParam*/,LPARAM lParam) {
	static ExportCap *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (ExportCap *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- ExportCap -------------------------------------------------------
ExportCap::ExportCap()
{
	//tbb::task_scheduler_init init(8);
}

ExportCap::~ExportCap() 
{

}

int ExportCap::ExtCount()
{
	// Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *ExportCap::Ext(int /*n*/)
{		
	#pragma message	(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	return _T("cap");
}

const TCHAR *ExportCap::LongDesc()
{
	#pragma message	(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\""))
	return _T("Capnproto format");
}
	
const TCHAR *ExportCap::ShortDesc() 
{			
	#pragma message	(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("Capnproto");
}

const TCHAR *ExportCap::AuthorName()
{			
	#pragma message	(TODO("Return ASCII Author name"))
	return _T("Y3D");
}

const TCHAR *ExportCap::CopyrightMessage() 
{	
	#pragma message	(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *ExportCap::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *ExportCap::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int ExportCap::Version()
{				
	#pragma message	(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void ExportCap::ShowAbout(HWND /*window*/)
{			
	// Optional
}

class RedrawCB : RedrawViewsCallback {

	virtual void proc(Interface* ip) {
		mprintf(L"Scene redraw\n");
	};
};

BOOL ExportCap::SupportsOptions(int /*ext*/, DWORD /*options*/)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}

int ExportCap::GetSceneNodes(INodeTab& i_nodeTab, INode* i_currentNode /*=NULL*/)
{
	int i;
	if (i_currentNode == nullptr)
	{
		i_currentNode = GetCOREInterface()->GetRootNode();
	}
	else // IGame will crash 3ds Max if it is initialized with the root node.
	{
		i_nodeTab.AppendNode(i_currentNode);
	}
	for (i = 0; i < i_currentNode->NumberOfChildren(); i++)
	{
		GetSceneNodes(i_nodeTab, i_currentNode->GetChildNode(i));
	}
	return i_nodeTab.Count();
}


int	ExportCap::DoExport(const TCHAR* name, ExpInterface* ei,Interface* ip, BOOL suppressPrompts, DWORD options)
{
	/*mprintf(L"Testing write\n");
	auto start0 = std::chrono::steady_clock::now();
	test("E:\\test.cap",100,1);
	//testRead("E:\\test.cap");
	auto duration0 = std::chrono::duration_cast< std::chrono::milliseconds>
		(std::chrono::steady_clock::now() - start0);
	mprintf(L"Execution time is %d ms\n", duration0.count());
	return TRUE;//*/
	// This is where the file export operation occur.
	//static auto ip = GetCOREInterface();
	/*if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				ExportCapOptionsDlgProc, (LPARAM)this);//*/

	// Construct a tab with all this scene's nodes.
	// We could also only take the node currently selected, etc.
	INodeTab lNodes;
	if (ip->GetSelNodeCount() == 0)
		GetSceneNodes(lNodes);
	else {
		for (int i = 0; i < ip->GetSelNodeCount(); ++i) {
			auto* node = ip->GetSelNode(i);
			lNodes.AppendNode(node);
		}
	}

	// Initialize 3DXI (formerly IGame) object
	// For more information, please see 3ds Max SDK topic PG: Programming with 3DXI.
	//IGameScene * pIgame = GetIGameInterface();
	//pIgame->InitialiseIGame(lNodes);

	int opts = ExportOpts::all;
	int lopts = ExportOpts::all;
	//saveObjects(nullptr,L"group",opts, exportFolder);
	/*INode* item = ip->GetINodeByName(L"Teapot001");
	INodeTab list;
	if (item) {
		mprintf(L"Found Teapot\n");
		list.AppendNode(item);
	}
	item = ip->GetINodeByName(L"GeoSphere001");
	if (item) {
		mprintf(L"Found GeoSphere001\n");
		list.AppendNode(item);
	}

	loadObjects(L"group", &list,opts, exportFolder);*/

	//std::vector<std::wstring>* names = nullptr;
	//std::vector<int>* index = nullptr;
	/*names.emplace_back(L"Teapot001");
	names.emplace_back(L"GeoSphere001");
	for (int i = 0; i < 500; ++i) {
		loadObject(L"group", nullptr, opts, exportFolder, L"Teapot001");
		loadObject(L"group", nullptr, opts, exportFolder, L"GeoSphere001");
	}*/
	//RedrawCB cb;
	//ip->RegisterRedrawViewsCallback((RedrawViewsCallback*)&cb);
	auto start = std::chrono::steady_clock::now();
	
	//for (int i = 0; i < 5; ++i)
		//loadObjects(L"group3", index, opts, exportFolder);
	//testRead("D:\\test.cap");
	//saveObjects(nullptr, L"test", opts, exportFolder);
	//loadObjects(L"test", index, opts, exportFolder);
	//auto* node = lNodes[0];
	saveObjects(&lNodes, (TCHAR*)name, opts, L"");
		
	//saveObject(node, opts, L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");
	//loadObject(L"Mesh12", nullptr, lopts, L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");
	auto duration = std::chrono::duration_cast< std::chrono::milliseconds>
		(std::chrono::steady_clock::now() - start);
	mprintf(L"Execution time is %d ms\n", duration.count());
	
	/*start = std::chrono::steady_clock::now();
	saveObjects(nullptr, L"group3", opts, exportFolder);
	duration = std::chrono::duration_cast< std::chrono::milliseconds>
		(std::chrono::steady_clock::now() - start);
	mprintf(L"Saving time is %d ms\n", duration.count());*/

	/*for (int i = 0; i < lNodes.Count(); ++i) {
		auto * node = lNodes[i];
		auto igamenode = pIgame->GetIGameNode(node);
		std::wstring name = igamenode->GetName();
		mprintf(L"-----------------------\n");
		mprintf(L"Object name = %s   (%s)\n", name.c_str(), igamenode->GetName());
		auto obj = igamenode->GetIGameObject();
		mprintf(L"Child count = %d\n", igamenode->GetChildCount());
		mprintf(L"Max Type = %d\n", obj->GetMaxType() == IGameObject::MaxType::IGAME_MAX_GEOM );
		mprintf(L"IGame Type = %d\n", obj->GetIGameType() == IGameObject::ObjectTypes::IGAME_MESH);
		//obj->InitializeData();
		if (obj->GetIGameType() == IGameObject::ObjectTypes::IGAME_MESH) {
			IGameMesh* o = (IGameMesh*)obj;
			auto t = ip->GetTime();
			ObjectState os = node->EvalWorldState(t);
			auto* maxObj = os.obj;
			mprintf(L"Get number of modifiers = %d\n", o->GetNumModifiers());
			if (maxObj->IsSubClassOf(polyObjectClassID)) {
				PolyObject* poly = (PolyObject*)maxObj;
				mprintf(L"Is a poly object\n");
				mprintf(L"Number of faces %d\n", o->GetNumberOfFaces());
				mprintf(L"Number of polygons %d\n", o->GetNumberOfPolygons());
				BitArray sels;
				auto& mesh = poly->GetMesh();
				mesh.getFaceSel(sels);
				
				mprintf(L"Num faces = %d\n", sels.GetSize());

				mprintf(L"Num normals = %d\n", o->GetNumberOfPolygonNormals());
				
				mprintf(L"Object name = %s\n", name.c_str());
				saveObject(igamenode, opts,L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");
				loadObject(L"Teapot001", node, lopts,L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");

			} else if (maxObj->IsSubClassOf(triObjectClassID)) {				
				IGameMesh* o = (IGameMesh*)obj;
				auto* maxObj = o->GetMaxObject();
				if (maxObj->IsSubClassOf(triObjectClassID)) {
					TriObject* poly = (TriObject*)maxObj;
					mprintf(L"Is a tri object\n");
					mprintf(L"Number of faces %d\n", o->GetNumberOfFaces());
					mprintf(L"Number of polygons %d\n", o->GetNumberOfPolygons());
					BitArray sels;
					auto& mesh = poly->GetMesh();
					sels = mesh.FaceSel();
					
					mprintf(L"Num faces = %d\n", sels.GetSize());

					mprintf(L"Num normals = %d\n", o->GetNumberOfPolygonNormals());
					mprintf(L"Object name = %s\n", name);
					saveObject(igamenode, opts,L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");
					loadObject(L"Teapot001", node, lopts,L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");
				}
			}
			else {								
				ObjectWrapper ow;
				if (ow.Init(t, os, FALSE, ObjectWrapper::triEnable | ObjectWrapper::polyEnable, ObjectWrapper::triObject) && ow.GetTriMesh()) {
					saveObject(ow, (wchar_t*)name.c_str(), opts, L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");
					loadObject(L"Teapot001", node, lopts, L"C:\\Users\\ducthangho\\Documents\\Visual Studio 2015\\Projects\\ExportCap\\ExportCap\\");
				}
					
				
			}
		}
	}//*/
	#pragma message	(TODO("Initialize and open your file to write"))
	#pragma message	(TODO("Access IGameScene object to extract useful information."))

	#pragma message	(TODO("return TRUE if the file was exported properly."))
	
	return TRUE;
}


 
