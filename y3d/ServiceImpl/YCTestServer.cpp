#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotocd.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")
#endif

#include "YCTestServer.h"
#include "common.h"
#include "LogClient.h"
#include "YLibs.h"
#include "tab.h"
#include "ymaxcoreinterface.pb.h"
#include "convert.h"
#include "XRef/iXrefObjMgr.h"
#include "XRef/iXrefProxy.h"
#include "XRef/iXrefObj.h"
#define LOG_ENABLE

using namespace logserver;

void MakeBentCylinder()
{
	float radius = 10, height = 10, angle = 10;
	int segments = 20;

	// Create a new object using CreateInstance()
	Object *obj = (Object*)CreateInstance(GEOMOBJECT_CLASS_ID, Class_ID(CYLINDER_CLASS_ID, 0));

	// Get ahold of the parameter block
	IParamArray *iCylParams = obj->GetParamBlock();

	// Get the current animation time
	TimeValue time = GetCOREInterface()->GetTime();

	// Set the value of radius, height and segments.
	iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_RADIUS), time, radius);
	iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_HEIGHT), time, height);
	iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_SEGMENTS), time, segments);
	
	auto ip = GetCOREInterface();
	// Create the object
	INode *node = ip->CreateObjectNode(obj);

	// Create a bend modifier
	//Modifier *bend = (Modifier*)ip->CreateInstance(OSM_CLASS_ID, Class_ID(BENDOSM_CLASS_ID, 0));
	//
	//// Set the bend angle
	//IParamBlock2* ipBendBlock = ((Animatable*)bend)->GetParamBlock(0);  //only one pblock2
	//ipBendBlock->SetValue(BEND_ANGLE, time, angle);

	//// Create a node in the scene for the object
	//// Note that COREInterface12 derives from Interface7
	//GetCOREInterface12()->AddModifier(*node, *bend);
}

void addbend()
{
	try
	{
		float radius = 10, height = 10, angle = 10;
		int segments = 20;

		// Create a new object using CreateInstance()
		Object *obj = (Object*)CreateInstance(GEOMOBJECT_CLASS_ID, Class_ID(CYLINDER_CLASS_ID, 0));

		// Get ahold of the parameter block
		IParamArray *iCylParams = obj->GetParamBlock();

		// Get the current animation time
		TimeValue time = GetCOREInterface()->GetTime();

		// Set the value of radius, height and segments.
		iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_RADIUS), time, radius);
		iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_HEIGHT), time, height);
		iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_SEGMENTS), time, segments);
		
		auto* ip = GetCOREInterface();
		INode *node = ip->CreateObjectNode(obj);
		
		//Create a new object using CreateInstance()
		auto pINode = ip->GetSelNode(0);
		log(L"New node name is {0} : \n", pINode->GetName());

		// Get the current animation time

		/*iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_RADIUS), time, 10);
		iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_HEIGHT), time, 20);
		iCylParams->SetValue(obj->GetParamBlockIndex(CYLINDER_SEGMENTS), time, 10);*/

		// Create a bend modifier
		Modifier *bend = (Modifier*)ip->CreateInstance(OSM_CLASS_ID, Class_ID(BENDOSM_CLASS_ID, 0));

		log(L"{}",bend->GetName().data());

		// Set the bend angle
		IParamBlock2* ipBendBlock = ((Animatable*)bend)->GetParamBlock(0);  //only one pblock2
		//ipBendBlock->SetValue(BEND_ANGLE, time, 10);
		ipBendBlock->SetValue(BEND_ANGLE, time, angle);

		GetCOREInterface12()->AddModifier(*pINode, *bend);
		GetCOREInterface12()->AddModifier(*node, *bend);

	}
	catch (const std::exception& e)
	{
		/*e.what();
		log("Exception: " + std::string(e.what()));*/
	}
}

void addUnwrap() {
	auto* ip = GetCOREInterface();
	auto pINode = ip->GetSelNode(0);
	log(L"New node name is {0} : \n", pINode->GetName());

	TimeValue time = GetCOREInterface()->GetTime();
	Modifier *unwrap_uvw = (Modifier*)ip->CreateInstance(OSM_CLASS_ID, Class_ID(0x2df2e3a, 0x72ba4e1f));	
	log(L"{}", unwrap_uvw->GetName().data());

	IParamBlock2* ipUVUnrapBlock = ((Animatable*)unwrap_uvw)->GetParamBlock(0);  //only one pblock2
	//ipBendBlock->SetValue(0, time, );
	//ipUVUnrapBlock()->SetValue()
	int width = 0;
	int height = 0;
	int spline_display = 1;
	int splinemap_manualseams = 1;
	Interval iv = FOREVER;
	ParamID pid = 8;
	ipUVUnrapBlock->GetValue(pid, time, width, iv);
	log("width value is {}\n", width);

	ipUVUnrapBlock->GetValue(9, time, height, iv);
	log("height value is {}\n", height);

	ipUVUnrapBlock->GetValue(29, time, spline_display, iv);
	log("spline_display value is {}\n", spline_display);

	ipUVUnrapBlock->GetValue(27, time, splinemap_manualseams, iv);
	log("splinemap_manualseams value is {}\n", splinemap_manualseams);

	//ipUVUnrapBlock->SetValue(27, time, 1);
	//ipUVUnrapBlock->SetValue(19, time, 0.23f);
	height = 0;
	ipUVUnrapBlock->GetValue(9, time, height, iv);
	log("new height value is {}\n", height);

	log(L"New node name is {0} : \n", pINode->GetName());
	GetCOREInterface12()->AddModifier(*pINode, *unwrap_uvw);

	ipUVUnrapBlock->SetValue(9, time, 22);
	ipUVUnrapBlock->GetValueByName(L"renderuv_height", time, height, iv);
	log("new height value in getvaluebyname is {}\n", height);
}


void BatchProOptimizer(y3d::IBatchProOptimizer ibatchProOptimizer)
{
	FPInterface* fpInterface = GetCOREInterface(BATCHPROOPTIMIZER_INTERFACE_ID);

	FPParams pSourceFileMode(1, TYPE_INT, 0);
	fpInterface->Invoke(SOURCEFILEMODE_IBATCHPROOPTIMIZER_SETTER, &pSourceFileMode);
	FPValue result;
	fpInterface->Invoke(SOURCEFILEMODE_IBATCHPROOPTIMIZER_GETTER, result);
	log("SourFileMode is {0}\n", result.i);

	MCHAR* file1 = L"F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene\\001.max";
	Tab<MCHAR*> sourceFile_Files;
	sourceFile_Files.Append(1,&file1);
	FPParams pSourceFileFiles(1, SOURCEFILEFILES_IBATCHPROOPTIMIZER_TYPEPARAM, sourceFile_Files);
	fpInterface->Invoke(SOURCEFILEFILES_IBATCHPROOPTIMIZER_SETTER, &pSourceFileFiles);

	fpInterface->Invoke(BATCH_IBATCHPROOPTIMIZER);
	//fpInterface->Invoke(SOURCEFILEFILES_IBATCHPROOPTIMIZER_GETTER, result);
}


void pre_optimize(std::string oFileDir,std::string oFileName, std::string projectPath)
{
	FPInterface* fpInterface = GetCOREInterface(BATCHPROOPTIMIZER_INTERFACE_ID);

	FPParams pSourceFileMode(1, TYPE_INT, 0);
	fpInterface->Invoke(SOURCEFILEMODE_IBATCHPROOPTIMIZER_SETTER, &pSourceFileMode);

	std::string file1 = oFileDir + "\\" + oFileName + ".max";
	Tab<MCHAR*> sourceFile_Files;
	auto file = const_cast<wchar_t*>(s2ws(file1).c_str());
	sourceFile_Files.Append(1, &file);
	FPParams pSourceFileFiles(1, SOURCEFILEFILES_IBATCHPROOPTIMIZER_TYPEPARAM, sourceFile_Files);
	fpInterface->Invoke(SOURCEFILEFILES_IBATCHPROOPTIMIZER_SETTER, &pSourceFileFiles);

	FPParams OptimizationLevel1(1, OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_TYPEPARAM, 90);
	fpInterface->Invoke(OPTIMIZATIONLEVEL1_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel1);

	FPParams OptimizationLevel2(1, OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL2_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel2);

	FPParams OptimizationLevel3(1, OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL3_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel3);

	FPParams OptimizationLevel4(1, OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL4_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel4);

	FPParams OptimizationLevel5(1, OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVEL5_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevel5);

	FPParams OptimizationLevelMode(1, OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_TYPEPARAM, 0);
	fpInterface->Invoke(OPTIMIZATIONLEVELMODE_IBATCHPROOPTIMIZER_SETTER, &OptimizationLevelMode);

	setInterFacePropertyTInt(OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, OPTIMIZEBORDERSMODE_IBATCHPROOPTIMIZER_SETTER,1);
	setInterFacePropertyTBool(AUTORENAME_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, AUTORENAME_IBATCHPROOPTIMIZER_SETTER, false);
	setInterFacePropertyTInt(DESTFILENAMEMODE_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, DESTFILENAMEMODE_IBATCHPROOPTIMIZER_SETTER, 1);
	setInterFacePropertyTInt(DESTFOLDERMODE_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, DESTFOLDERMODE_IBATCHPROOPTIMIZER_SETTER, 2);

	//std::string desDir = ".\\" + oFileName + "_y3d";
	setInterFacePropertyTString(DESTFOLDERNAME_IBATCHPROOPTIMIZER_TYPEPARAM, fpInterface, DESTFOLDERNAME_IBATCHPROOPTIMIZER_SETTER, projectPath);
	
	fpInterface->Invoke(BATCH_IBATCHPROOPTIMIZER);

	std::string maxFile = projectPath + "\\" + oFileName + "90.max";

	auto ip16 = GetCOREInterface16();
	QuietMode quietmode;
	quietmode.set();
	bool loadsucess = ip16->LoadFromFile(s2ws(maxFile).data(),
		Interface8::LoadFromFileFlags::kSuppressPrompts&Interface8::LoadFromFileFlags::kUseFileUnits);
	ip16->SaveToFile(s2ws(maxFile).data());
}

inline void xref_low_old(std::wstring project_path, std::wstring pname) {
	FPInterface* fpInterface = GetCOREInterface(OBJXREFMGR_INTERFACE_ID);
	FPParams p(1, DUPOBJNAMEACTION_IOBJXREFMGR_TYPEPARAM, OBJXREFMGR_ENUM3::deleteOld);
	fpInterface->Invoke(DUPOBJNAMEACTION_IOBJXREFMGR_SETTER, &p);

	auto* ip = GetCOREInterface16();
	INodeTab inodetab;
	ip->GetSelNodeTab(inodetab);
	Tab<MSTR*> obj_names;
	Tab<const MCHAR*> obj_names_mchartype;
	auto numNodeSelection = inodetab.Count();
	obj_names_mchartype.Resize(numNodeSelection);
	obj_names.Resize(numNodeSelection);
	LOG("count is {}\n", numNodeSelection);
	for (int i = 0; i < numNodeSelection; i++)
	{
		auto inode = inodetab[i];
		//auto name = const_cast<wchar_t*>(inode->GetName());
		auto name = inode->NodeName();
		auto pname = &name;
		obj_names.Append(1, &pname);
		//LOG(obj_names[i]->data());
		LOG(obj_names[i]->data()); LOG("\n");
		auto name_wcharType = inode->GetName();
		obj_names_mchartype.Append(1, &name_wcharType);
		LOG(obj_names_mchartype[i]); LOG("\n");
	}
	//obj_names.SetCount(inodetab.Count(), true);
	//obj_names.Shrink();

	// 	FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
	FPParams fnParams;
	//FPValue result, param1, param3;

	std::wstring filename = (project_path + L"\\" + pname + L"_low0.max");
	MSTR a = filename.data();


	Tab<MCHAR*> sourceFile_Files;
	auto file = const_cast<wchar_t*>(filename.c_str());
	sourceFile_Files.Append(1, &file);
	//FPParams pSourceFileFiles(1, TYPE_STRING_TAB_BV, sourceFile_Files);
	//pSourceFileFiles.GetFPValue(0)->type;

	//param1.tstr = &a;	
	//FPValue param1(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, s2ws(filename).data());
	FPValue param1;
	////	param1.Load(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, s2ws(filename).data());
	//auto isPtrType = param1.IsPointerBasedType(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE);
	//LOG("is Pointer base type ?? {0}\n", isPtrType);
	param1.LoadPtr(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, filename.data());
	////param1.type = FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
	////param1.s = s2ws(filename).data();	
	LOG("param1.s here= "); LOG(param1.s); LOG("\n");
	//LOG("param1.s here= "); LOG(param1.tstr->data()); LOG("\n");
	////LOG("param1.tstr = {0}.\n",(param1.tstr==NULL));
	////FPValue param3(ParamType2::TYPE_STRING_TAB, 20);
	//isPtrType = param1.IsPointerBasedType(TYPE_STRING_TAB_BV);
	//LOG("is Pointer base type ?? {0}\n", isPtrType);
	FPValue param3;
	//param3.InitTab(ParamType2::TYPE_STRING_TAB,numNodeSelection);
	//param3.s_tab = &obj_names_mchartype;
	//const wchar_t* p1 = L"dsdsdsa";	
	OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE;
	//param3.InitTab(OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE,0);
	//param3.s_tab = &obj_names_mchartype;

	//param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);
	//param3.Load(ParamType2::TYPE_STRING_TAB_BV,&sourceFile_Files);
	param3.LoadPtr(ParamType2::TYPE_STRING_TAB, &sourceFile_Files);
	//param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);
	//SYSTEM_CALL(param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);)

	//param3.Load(ParamType2::TYPE_STRING_TAB, &sourceFile_Files);
	auto s_tab = param3.s_tab;
	auto count = s_tab->Count();
	LOG("count here is {}\n", count);
	for (int i = 0; i < count; i++)
	{
		auto sp = (*s_tab)[i];
		LOG("Name {} from param3.s_tab ", i); LOG(sp); LOG("\n");
	}
	if (s_tab == nullptr)
		LOG("ffffffffffffffffffff\n");
	else
		LOG("uha\n");

	//param3.Free();

	//auto s_tab = param3.tstr_tab;
	//LOG("count here is {}\n", s_tab->Count());
	//auto mstr = (*s_tab)[0];
	//if (mstr == nullptr)
	//{
	//	LOG("oh fucking shit time 1\n");
	//	MSTR a1(p1);
	//	mstr = &a1;
	//}
	//	if (mstr != nullptr){
	//		auto str = param3.s_tab;
	//		auto str0 = (*str)[0];
	//		if (str0 == nullptr)
	//			LOG("oh Ffffffffffffffff ");
	//	}





	/*auto s = (*s_tab)[0];
	if (s == nullptr)
	{
	LOG("fucking shit");
	}*/

	//LOG("count here is {}\n", ws2s(std::wstring((*s_tab)[0])));
	//param3.LoadPtr(ParamType2::TYPE_STRING_TAB,obj_names);
	//param3.type = ParamType2::TYPE_STRING_TAB;
	//param3.s_tab = &obj_names_mchartype;
	// 	param3.InitTab(ParamType2::TYPE_STRING_TAB, obj_names.Count());
	//  	Tab<MSTR*>* t = &obj_names;
	// 	param3.tstr_tab = t;
	//FPValue v(ParamType2::TYPE_STRING_TAB, obj_names);
	//LOG("type is {}\n",param3.type);
	//auto s_tab = param3.s_tab;

}

void xref_low_error(std::wstring project_path, std::wstring pname) {

	Tab<const MCHAR*> sourceFile_Files;
	Tab<const MCHAR*>* pointer_sourceFile_Files = new Tab<const MCHAR*>();
	std::wstring filename = (project_path + L"\\" + pname + L"_low0.max");
	const wchar_t* file = filename.c_str();
	sourceFile_Files.Append(1, &file);
	//sourceFile_Files.Shrink();
	pointer_sourceFile_Files->Append(1, &file);

	Tab<MSTR*> tstr_tab;
	MSTR str(filename.c_str());
	MSTR* a = &str;
	tstr_tab.Append(1, &a);
	//tstr_tab.Shrink();

	Tab<MSTR*> tstr_tab_dynamic_allocate_items;
	MSTR* str_dynamic_allocate_items = new MSTR(filename.c_str());
	tstr_tab_dynamic_allocate_items.Append(1, &str_dynamic_allocate_items);
	//tstr_tab_dynamic_allocate_items.Shrink();


	FPValue param1;
	//int i = 10;
	////param1.LoadPtr(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, filename.data());	
	param1.type = FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
	param1.s = filename.data();
	//param1.LoadPtr(ParamType2::TYPE_INT_BP, &i);	
	//LOG("param1.s here= "); LOG(param1.s); LOG("\n");
	
	FPValue param2;
	param2.type = PROMPTOBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM2_TYPE;
	param2.b = false;

//  	FPValue param1_FPValue;
//  	TCHAR* param1_TCHAR = _T("Test Track View");
// 	param1_FPValue.type = TYPE_STRING,
// 	param1_FPValue.s = param1_TCHAR;
//  	param1.type = (ParamType2)TYPE_FPVALUE;
//  	param1.fpv = &param1_FPValue;
	try
	{
		FPValue param3;
		OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE;
		param3.InitTab(ParamType2::TYPE_STRING_TAB, sourceFile_Files.Count());

		auto ptr = param3.s_tab;
		for (int i = 0; i < ptr->Count(); ++i) {
			//std::string s = ws2s(sourceFile_Files[i]);
			//LOG("BBB {0} {1}\n", i, s.c_str());
			(*ptr)[i] = sourceFile_Files[i];									
		}

		param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);
		//param3.Load(ParamType2::TYPE_STRING_TAB_BV,&sourceFile_Files);
 		for (int i = 0; i < sourceFile_Files.Count(); ++i) {
 			std::string s = ws2s( sourceFile_Files[i] );
 			LOG("Aha  {0} {1}\n", i, s.c_str());
 		}

		//param3.LoadPtr(ParamType2::TYPE_STRING_TAB, &sourceFile_Files);

// 		for (int i = 0; i < sourceFile_Files.Count(); ++i) {
// 			std::string s = ws2s(sourceFile_Files[i]);
// 			LOG("BBB {0} {1}\n", i, s.c_str());
// 		}
		//param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, tstr_tab);
		//param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &tstr_tab_dynamic_allocate_items);
		//SYSTEM_CALL(param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);)
		//param3.Load(ParamType2::TYPE_STRING_TAB, &sourceFile_Files);
	}
	catch (const std::exception&)
	{
		LOG("Why this is error, tell me pls");
	};
	

	LOG("???????----------- {}\n",ws2s(file).c_str());
	
  	FPValue param4;
	param4.InitTab(XREFOPTIONS_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM4_TYPE, 0);
  	//param4.type = ParamType2::TYPE_FPVALUE_TAB_BV;
// 	Tab<FPValue*> param4_fpvalues;
// 	FPValue param4_fpvalue;
// 	FPValue* pointer_param4_fpvalue;
// 	param4_fpvalue.type = (ParamType2)1;
// 	param4_fpvalue.i = 10;
// 	param4_fpvalues.Append(1, &pointer_param4_fpvalue);
// 	param4.fpv_tab = &param4_fpvalues;
	
	//FPValue param4;
	//param4.type = ParamType2::TYPE_INT_TAB;
	//param4.type = ParamType2::TYPE_INT_TAB_BV;
	//param4.type = ParamType2::TYPE_INT_TAB_BR;
 // 	Tab<int> xrefoptions;
	//xrefoptions.Resize(1);
 //	int h = 1;
 //	xrefoptions.Append(1, &h);
	
  	//param4.i_tab = &xrefoptions;

	//LOG("type is {0}", param4.type);

	//FPParams fnParams;
	//FPValue result, param1;
	//FPValue param1_FPValue;
	//TCHAR* param1_TCHAR = _T("Test Track View");
	//param1_FPValue.type = TYPE_STRING,
	//	param1_FPValue.s = param1_TCHAR;

	//param1.type = (ParamType2)TYPE_FPVALUE,
	//	param1.fpv = &param1_FPValue;
	//fnParams.params.append(&param1,1);

	//LOG("type is {0}", param1.type);

// 	auto s_tab = param3.s_tab;
// 	auto count = s_tab->Count();
// 	LOG("count here is {}\n", count);
// 	for (int i = 0; i < count; i++)
// 	{
// 		auto sp = (*s_tab)[i];
// 		LOG("Name {} from param3.s_tab ", i); LOG(sp); LOG("\n");
// 	}
// 	if (s_tab == nullptr)
// 		LOG("this not good\n");
// 	else
// 		LOG("uha\n");
}

//inline void xref_low_old(std::string project_path, std::string pname) {
//	FPInterface* fpInterface = GetCOREInterface(OBJXREFMGR_INTERFACE_ID);
//	FPParams p(1, DUPOBJNAMEACTION_IOBJXREFMGR_TYPEPARAM,OBJXREFMGR_ENUM3::deleteOld);
//	fpInterface->Invoke(DUPOBJNAMEACTION_IOBJXREFMGR_SETTER, &p);
//
//	auto* ip = GetCOREInterface16();
//	INodeTab inodetab;
//	ip->GetSelNodeTab(inodetab);
//	Tab<MSTR*> obj_names;
//	Tab<const MCHAR*> obj_names_mchartype;
//	auto numNodeSelection = inodetab.Count();
//	obj_names_mchartype.Resize(numNodeSelection);
//	obj_names.Resize(numNodeSelection);
//	LOG("count is {}\n", numNodeSelection);
//	for (int i = 0; i < numNodeSelection; i++)
//	{
//		auto inode = inodetab[i];
//		//auto name = const_cast<wchar_t*>(inode->GetName());
//		auto name = inode->NodeName();
//		auto pname = &name;
//		obj_names.Append(1, &pname);
//		//LOG(obj_names[i]->data());
//		LOG(obj_names[i]->data()); LOG("\n");
//		auto name_wcharType = inode->GetName();
//		obj_names_mchartype.Append(1, &name_wcharType);
//		LOG(obj_names_mchartype[i]); LOG("\n");
//	}
//	//obj_names.SetCount(inodetab.Count(), true);
//	//obj_names.Shrink();
//	
//// 	FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
// 	FPParams fnParams;
// 	//FPValue result, param1, param3;
//
//	std::string filename = (project_path + "\\" + pname + "_low0.max");	
//	MSTR a = s2ws(filename).data();
//
//
//	Tab<MCHAR*> sourceFile_Files;
//	auto file = const_cast<wchar_t*>(s2ws(filename).c_str());
//	sourceFile_Files.Append(1, &file);
//	//FPParams pSourceFileFiles(1, TYPE_STRING_TAB_BV, sourceFile_Files);
//	//pSourceFileFiles.GetFPValue(0)->type;
//
//	//param1.tstr = &a;	
//	//FPValue param1(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, s2ws(filename).data());
// 	FPValue param1;
// ////	param1.Load(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, s2ws(filename).data());
//	//auto isPtrType = param1.IsPointerBasedType(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE);
//	//LOG("is Pointer base type ?? {0}\n", isPtrType);
//	param1.LoadPtr(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, s2ws(filename).data());
//	////param1.type = FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
//	////param1.s = s2ws(filename).data();	
//	LOG("param1.s here= "); LOG(param1.s); LOG("\n");
//	//LOG("param1.s here= "); LOG(param1.tstr->data()); LOG("\n");
//	////LOG("param1.tstr = {0}.\n",(param1.tstr==NULL));
//	////FPValue param3(ParamType2::TYPE_STRING_TAB, 20);
//	//isPtrType = param1.IsPointerBasedType(TYPE_STRING_TAB_BV);
//	//LOG("is Pointer base type ?? {0}\n", isPtrType);
//	FPValue param3;
//	//param3.InitTab(ParamType2::TYPE_STRING_TAB,numNodeSelection);
//	//param3.s_tab = &obj_names_mchartype;
//	//const wchar_t* p1 = L"dsdsdsa";	
//	OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE;
//	//param3.InitTab(OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE,0);
//	//param3.s_tab = &obj_names_mchartype;
// 	
//	//param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);
//	//param3.Load(ParamType2::TYPE_STRING_TAB_BV,&sourceFile_Files);
//	param3.LoadPtr(ParamType2::TYPE_STRING_TAB, &sourceFile_Files);
//	//param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);
//	//SYSTEM_CALL(param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);)
//	
//	//param3.Load(ParamType2::TYPE_STRING_TAB, &sourceFile_Files);
//  	auto s_tab = param3.s_tab;
// 	auto count = s_tab->Count();
// 	LOG("count here is {}\n", count);
//	for (int i = 0; i < count; i++)
//	{
//		auto sp = (*s_tab)[i];
//		LOG("Name {} from param3.s_tab ", i); LOG(sp); LOG("\n");
//	}
// 	if (s_tab == nullptr)
// 		LOG("oh ffffffffffffffff\n");
// 	else
// 		LOG("uha\n");
//
//	//param3.Free();
//
//	//auto s_tab = param3.tstr_tab;
//	//LOG("count here is {}\n", s_tab->Count());
//	//auto mstr = (*s_tab)[0];
//	//if (mstr == nullptr)
//	//{
//	//	LOG("oh fucking shit time 1\n");
//	//	MSTR a1(p1);
//	//	mstr = &a1;
//	//}
// //	if (mstr != nullptr){
// //		auto str = param3.s_tab;
// //		auto str0 = (*str)[0];
// //		if (str0 == nullptr)
// //			LOG("oh fucking shit ");
// //	}
//
//
//
// 	
//
//	/*auto s = (*s_tab)[0];
//	if (s == nullptr)
//	{
//		LOG("fucking shit");
//	}*/
//
//	//LOG("count here is {}\n", ws2s(std::wstring((*s_tab)[0])));
//	//param3.LoadPtr(ParamType2::TYPE_STRING_TAB,obj_names);
// 	//param3.type = ParamType2::TYPE_STRING_TAB;
// 	//param3.s_tab = &obj_names_mchartype;
//// 	param3.InitTab(ParamType2::TYPE_STRING_TAB, obj_names.Count());
////  	Tab<MSTR*>* t = &obj_names;
//// 	param3.tstr_tab = t;
//	//FPValue v(ParamType2::TYPE_STRING_TAB, obj_names);
//	//LOG("type is {}\n",param3.type);
//	//auto s_tab = param3.s_tab;
//	
//}

template <class DstType, class SrcType>
bool IsType(const SrcType* src)
{
	return dynamic_cast<const DstType*>(src) != 0;
}

inline void xref_low_2(std::wstring project_path, std::wstring pname) {
	FPInterface* iObjXrefMGR = GetCOREInterface(OBJXREFMGR_INTERFACE_ID);
	FPParams p(1, DUPOBJNAMEACTION_IOBJXREFMGR_TYPEPARAM, OBJXREFMGR_ENUM3::deleteOld);
	iObjXrefMGR->Invoke(DUPOBJNAMEACTION_IOBJXREFMGR_SETTER, &p);

	auto* ip = GetCOREInterface16();
	INodeTab selectedINodes;
	ip->GetSelNodeTab(selectedINodes);

	Tab<const MCHAR*> objNames;
	auto numNodeSelection = selectedINodes.Count();
	objNames.Resize(numNodeSelection);
	
	FPParams pAddXrefItemFromFile;
	FPValue pFileName;
	std::wstring filename = (project_path + L"\\" + pname + L"_high.max");
	pFileName.type = FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
	pFileName.s = filename.data();
	pAddXrefItemFromFile.params.append(pFileName);

	FPValue pPromptObjNames;
	pPromptObjNames.type = PROMPTOBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM2_TYPE;
	pPromptObjNames.b = false;
	pAddXrefItemFromFile.params.append(pPromptObjNames);

	FPValue pObjName;
	pObjName.InitTab(OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE, numNodeSelection);
	auto ptr = pObjName.s_tab;
	for (int i = 0; i < ptr->Count(); ++i) {
		auto selectedNode = selectedINodes[i];		
		(*ptr)[i] = selectedNode->GetName();
	}
	pAddXrefItemFromFile.params.append(pObjName);

	FPValue pxrefOptions;
	pxrefOptions.InitTab(XREFOPTIONS_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM4_TYPE, 0);
	pAddXrefItemFromFile.params.append(pxrefOptions);
	
	FPValue result;
	iObjXrefMGR->Invoke(ADDXREFITEMSFROMFILE_IOBJXREFMGR, result, &pAddXrefItemFromFile);

	FPInterface* IXrefRecord = result.fpi;
	
	if (IXrefRecord != nullptr)
	{
		FPParams pGetItems;	
		FPValue pTypes;

		//FPInterfaceDesc* interfaceDesc = IXrefRecord->GetDesc();
		//generateInterfaceFuntionsID2(interfaceDesc);
		

// 		pTypes.InitTab(TYPES_GETITEMS_IIXREFRECORD_PARAM1_TYPE, 1);		
// 		(*pTypes.i_tab)[0] = (int)ENUM1_IXREFRECORD::XRefObjectType;
// 		pGetItems.params.append(&pTypes,1);		

		pGetItems.params.append(&pTypes, 1);	
		auto& param1 = pGetItems.params[0];
		param1.InitTab(TYPES_GETITEMS_IIXREFRECORD_PARAM1_TYPE, 1);
		(*param1.i_tab)[0] = (int)ENUM1_IXREFRECORD::XRefObjectType;
	
//  		FPValue pXrefItems;		
//  		pXrefItems.InitTab(XREFITEMS_GETITEMS_IIXREFRECORD_PARAM2_TYPE, 1);
//  		pGetItems.params.append(&pXrefItems,1);
	
 	//	FPValue pXrefItems;
		//Tab<ReferenceTarget*>* tabRefTarg = new Tab<ReferenceTarget*>();
		//tabRefTarg->SetCount(1,true);
		////tabRefTarg->Init();
		////tabRefTarg->ZeroCount();
		////tabRefTarg->Delete(0, 1);
 	//	LOG("Count tabRefTarg element is {}\n", tabRefTarg->Count());
 	//	pXrefItems.type = XREFITEMS_GETITEMS_IIXREFRECORD_PARAM2_TYPE;
 	//	pXrefItems.r_tab = tabRefTarg;
 	//	pGetItems.params.append(&pXrefItems, 1);
		//pGetItems.params[1].r_tab->Delete(0, 1);
		//FPValue result;
		//IXrefRecord->Invoke(GETITEMS_IIXREFRECORD, result, &pGetItems);
		
		FPValue pXrefItems;				
		pGetItems.params.append(&pXrefItems,1);
		auto& param2 = pGetItems.params[1];
		param2.InitTab(XREFITEMS_GETITEMS_IIXREFRECORD_PARAM2_TYPE, 0);

		IXrefRecord->Invoke(GETITEMS_IIXREFRECORD, result, &pGetItems);
		//LOG("first item in original is {}\n", (*pXrefItems.r_tab)[0] != nullptr);
		//LOG("number of item in original is {}\n", pXrefItems.r_tab->Count());
		if (param2.r_tab != nullptr)
		{			
			for (int i = 0; i < param2.r_tab->Count(); i++)
			{
				auto item = (*param2.r_tab)[i];				
		
				FPParams pSetProxyItemSrcFile;
				FPValue pitem;
				pSetProxyItemSrcFile.params.append(&pitem, 1);
				auto& p1 = pSetProxyItemSrcFile.params[0];
				p1.type = PROXYITEM_SETPROXYITEMSRCFILE_IOBJXREFMGR_PARAM1_TYPE;
				p1.r = item;

				FPValue pFileName;
				pSetProxyItemSrcFile.params.append(&pFileName,1);
				auto& p2 = pSetProxyItemSrcFile.params[1];
				p2.type = FILENAME_SETPROXYITEMSRCFILE_IOBJXREFMGR_PARAM2_TYPE;
				std::wstring proxyFile = (project_path + L"\\" + pname + L"_low0.max");
				p2.s = proxyFile.c_str();

				FPValue result;
				iObjXrefMGR->Invoke(SETPROXYITEMSRCFILE_IOBJXREFMGR, result, &pSetProxyItemSrcFile);
				
				
				IXRefObject *ix = (IXRefObject8 *)item;

				auto objName = ix->GetCurObjName();				
				
				auto iXrefProxy = IXRefProxy::GetInterface(*item);

				FPParams proxyItemName(1, PROXYITEMNAME_IIXREFPROXY_TYPEPARAM, objName.data());
				iXrefProxy->Invoke(PROXYITEMNAME_IIXREFPROXY_SETTER,&proxyItemName);
			}
		}
		else LOG("r_tab is nullptr\n"); 		
	}
		

	//const wchar_t* vfilename = s2ws(filename).data();
	//Tab<int> a;
	//LOG(vfilename);
	//FPParams AddXRefItemsFromFile(4, FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, vfilename,
	//	PROMPTOBJNAMES_ADDXREFITEMSTORECORD_IOBJXREFMGR_PARAM2_TYPE, false,
	//	OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE, objNames_mchartype,
	//	XREFOPTIONS_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM4_TYPE,a
	//);
		
// 	FPValue result;
// 	fpInterface->Invoke(ADDXREFITEMSFROMFILE_IOBJXREFMGR, result, &AddXRefItemsFromFile);
}

inline void xref_low(std::wstring project_path, std::wstring pname) {
	FPInterface* iObjXrefMGR = GetCOREInterface(OBJXREFMGR_INTERFACE_ID);
	FPParams p(1, DUPOBJNAMEACTION_IOBJXREFMGR_TYPEPARAM, OBJXREFMGR_ENUM3::deleteOld);
	iObjXrefMGR->Invoke(DUPOBJNAMEACTION_IOBJXREFMGR_SETTER, &p);

	auto* ip = GetCOREInterface16();
	INodeTab selectedINodes;
	ip->GetSelNodeTab(selectedINodes);

	Tab<const MCHAR*> objNames;
	auto numNodeSelection = selectedINodes.Count();
	objNames.Resize(numNodeSelection);

	FPParams pAddXrefItemFromFile;
	FPValue pFileName;
	std::wstring filename = (project_path + L"\\" + pname + L"_high.max");
	pFileName.type = FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
	pFileName.s = filename.data();
	pAddXrefItemFromFile.params.append(pFileName);

	FPValue pPromptObjNames;
	pPromptObjNames.type = PROMPTOBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM2_TYPE;
	pPromptObjNames.b = false;
	pAddXrefItemFromFile.params.append(pPromptObjNames);

	FPValue pObjName;
	pObjName.InitTab(OBJNAMES_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM3_TYPE, numNodeSelection);
	auto ptr = pObjName.s_tab;
	for (int i = 0; i < ptr->Count(); ++i) {
		auto selectedNode = selectedINodes[i];
		(*ptr)[i] = selectedNode->GetName();
	}
	pAddXrefItemFromFile.params.append(pObjName);

	FPValue pxrefOptions;
	pxrefOptions.InitTab(XREFOPTIONS_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM4_TYPE, 0);
	pAddXrefItemFromFile.params.append(pxrefOptions);

	FPValue result;
	iObjXrefMGR->Invoke(ADDXREFITEMSFROMFILE_IOBJXREFMGR, result, &pAddXrefItemFromFile);

	FPInterface* IXrefRecord = result.fpi;

	if (IXrefRecord != nullptr)
	{
		FPParams pGetItems;
		FPValue pTypes;

		pGetItems.params.append(&pTypes, 1);
		auto& param1 = pGetItems.params[0];
		param1.InitTab(TYPES_GETITEMS_IIXREFRECORD_PARAM1_TYPE, 1);
		(*param1.i_tab)[0] = (int)ENUM1_IXREFRECORD::XRefObjectType;

		FPValue pXrefItems;
		pGetItems.params.append(&pXrefItems, 1);
		auto& param2 = pGetItems.params[1];
		param2.InitTab(XREFITEMS_GETITEMS_IIXREFRECORD_PARAM2_TYPE, 0);

		IXrefRecord->Invoke(GETITEMS_IIXREFRECORD, result, &pGetItems);
		if (param2.r_tab != nullptr)
		{
			for (int i = 0; i < param2.r_tab->Count(); i++)
			{
				auto item = (*param2.r_tab)[i];

				FPParams pSetProxyItemSrcFile;
				FPValue pitem;
				pSetProxyItemSrcFile.params.append(&pitem, 1);
				auto& p1 = pSetProxyItemSrcFile.params[0];
				p1.type = PROXYITEM_SETPROXYITEMSRCFILE_IOBJXREFMGR_PARAM1_TYPE;
				p1.r = item;

				FPValue pFileName;
				pSetProxyItemSrcFile.params.append(&pFileName, 1);
				auto& p2 = pSetProxyItemSrcFile.params[1];
				p2.type = FILENAME_SETPROXYITEMSRCFILE_IOBJXREFMGR_PARAM2_TYPE;
				std::wstring proxyFile = (project_path + L"\\" + pname + L"_low0.max");
				p2.s = proxyFile.c_str();

				FPValue result;
				iObjXrefMGR->Invoke(SETPROXYITEMSRCFILE_IOBJXREFMGR, result, &pSetProxyItemSrcFile);


				IXRefObject *ix = (IXRefObject8 *)item;

				auto objName = ix->GetCurObjName();

				auto iXrefProxy = IXRefProxy::GetInterface(*item);

				FPParams proxyItemName(1, PROXYITEMNAME_IIXREFPROXY_TYPEPARAM, objName.data());
				iXrefProxy->Invoke(PROXYITEMNAME_IIXREFPROXY_SETTER, &proxyItemName);
			}
		}
		else LOG("r_tab is nullptr\n");
	}
}

void LayerInterfaceExample()
{
	FPInterface* fpInterface = GetCOREInterface(LAYERMANAGER_INTERFACE_ID);

	FPValue result;
	fpInterface->Invoke(COUNT_ILAYERMANAGER_GETTER, result);
	auto numLayer = result.i;
	LOG("number layer is {0}\n", numLayer);

	for (int i = 0; i < numLayer; i++)
	{
		FPParams pWHICH(1, WHICH_GETLAYER_ILAYERMANAGER_PARAM1_TYPE, i);
		fpInterface->Invoke(GETLAYER_ILAYERMANAGER, result, &pWHICH);

		auto fp = result.fpi;

		FPValue r;
		fp->Invoke(NAME_ILAYERPROPERTIES_GETTER, r);
		auto layerName = r.s;
		LOG("Layer number {} have layer name is ", i + 1); LOG(layerName); LOG("\n");

		//auto interfaceID = fp->GetID();
		//int fn_id = fp->FindFn(L"select");
		//LOG("fn_id is {}\n", fn_id);
		//LOG("Interface_ID is {}",InterFaceID2S(interfaceID));
		//LOG("Interface_ID is of LAYERMANAGER INTERFACE is {}", InterFaceID2S(LAYERMANAGER_INTERFACE_ID));

		//FPMixinInterface* ppmixinInterface = dynamic_cast<FPMixinInterface*>(fp);
		//if (ppmixinInterface != NULL)
		//	LOG("fp is instance of class FPMixinInterface\n");

		//FPMixinInterface* ppmixinInterface2 = dynamic_cast<FPMixinInterface*>(fpInterface);
		//if (ppmixinInterface2 == NULL)
		//	LOG("fpInterface is not instance of class FPMixinInterface\n");

		//auto fpInterfaceDesc = fp->GetDesc();
		//auto id = fpInterfaceDesc->GetID();
		//LOG("Interface_ID is {}\n", InterFaceID2S(id));

		////auto id = fpInterfaceDesc->ID;
		//auto internal_name = fpInterfaceDesc->internal_name;
		//LOG("interface internal_name is{0}\n", internal_name);
		//Tab<FPFunctionDef*> functions = fpInterfaceDesc->functions;
		//auto numFunction = functions.Count();
		//LOG("function numbers of interface {0} is {1}\n", internal_name, numFunction);

		//generateInterfaceFuntionsID2(fpInterfaceDesc);
	}
}

void  TestCallMyFP()
{
	Interface_ID FP_BASIC_INTERFACE(0x7d0c759f, 0x7714b4b);
	//auto iFP = GetCOREInterface(FP_BASIC_INTERFACE);
	Class_ID fp_basics_CLASS_ID(0x3f869fdf, 0x63e46b8e);
	auto iFP = GetInterface(0x0010e0, fp_basics_CLASS_ID, FP_BASIC_INTERFACE);
	//LOG("is iFP == null? {}", (iFP == nullptr));
	//generateInterfaceFuntionsID2(iFP->GetDesc());
	
	//FPParams pMyTestFunction3;
	//FPValue px;
	//px.type = (ParamType2)X_MYTESTFUNCTION3_IFPBASICS_PARAM1_TYPE;
	//int a = 10;
	//px.i = a;
	//pMyTestFunction3.params.append(px);
	//iFP->Invoke(MYTESTFUNCTION3_IFPBASICS,&pMyTestFunction3);

// 	FPParams pMyTestFunction3;
// 	FPValue px2;
// 	px2.type = (ParamType2)X_MYTESTFUNCTION2_IFPBASICS_PARAM1_TYPE;
// 	int a = 10;
// 	px2.iptr = &a;
// 	pMyTestFunction3.params.append(px2);
// 	iFP->Invoke(MYTESTFUNCTION2_IFPBASICS, &pMyTestFunction3);
// 	LOG("a now is {}",a);

	//FPParams pMyFunction4;
	//FPValue pListRect4;
	//Tab<float>* a = new Tab<float>();
	//float f = 20;
	//a->Append(1, &f);
	//pListRect4.type = LISTRECT_MYFUNCTION4_IFPBASICS_PARAM1_TYPE;
	//pListRect4.f_tab = a;
	//pMyFunction4.params.append(pListRect4);
	//iFP->Invoke(MYFUNCTION4_IFPBASICS, &pMyFunction4);

	//FPValue *pListRect3 = new FPValue();
	//Tab<float>* a = new Tab<float>();
	//float f = 20;
	//a->Append(1, &f);
	//pListRect3->type = LISTRECT_MYFUNCTION3_IFPBASICS_PARAM1_TYPE;
	//pListRect3->f_tab = a;
	////pListRect3->LoadPtr(LISTRECT_MYFUNCTION3_IFPBASICS_PARAM1_TYPE, a);
	//FPParams pMyFunction3(1, LISTRECT_MYFUNCTION3_IFPBASICS_PARAM1_TYPE, a);
	//iFP->Invoke(MYFUNCTION3_IFPBASICS, &pMyFunction3);
	//LOG("f now is {}, f_tab !=null ?? {}\n", f, pListRect3->f_tab != nullptr);
	//LOG("value now is {}\n", (*pListRect3->f_tab)[0]);

// 	FPValue *pListRect4 = new FPValue();
// 	Tab<float>* tabfloat = new Tab<float>();
// 	float f = 20;
// 	tabfloat->Append(1, &f);
// 	pListRect4->InitTab(LISTRECT_MYFUNCTION4_IFPBASICS_PARAM1_TYPE, 1);
// 	(*pListRect4->f_tab)[0] = 20;
// 
// 	FPParams pMyFunction4;
// 	pMyFunction4.params.append(pListRect4, 1);
// 	iFP->Invoke(MYFUNCTION4_IFPBASICS, &pMyFunction4);
// 	LOG("value now is {}\n", (*pMyFunction4.params[0].f_tab)[0]);
// 	LOG("value from origin FPValue is {}\n", (*(pListRect4->f_tab))[0]);


	FPValue pListRect3;
 	pListRect3.InitTab(LISTRECT_MYFUNCTION3_IFPBASICS_PARAM1_TYPE, 1);
 	(*pListRect3.f_tab)[0] = 20;
 
 	FPParams pMyFunction3;
 	pMyFunction3.params.append(&pListRect3, 1);
 	iFP->Invoke(MYFUNCTION3_IFPBASICS, &pMyFunction3);

	LOG("number item now is {}\n", (*pMyFunction3.params[0].f_tab).Count());
	LOG("value now is {}\n", (*pMyFunction3.params[0].f_tab)[0]);
	LOG("value from origin FPValue is {}\n", (*(pListRect3.f_tab))[0]);

	
}

Status YServiceTestImpl::MTest1(ServerContext* context, const EmptyParam* request, EmptyParam* reply)
{
	Invoke([]() {
		//MakeBentCylinder();
		//addbend();
		//addUnwrap();
		//log("New node name is \n");
		/*auto* ip = GetCOREInterface();

		Create a new object using CreateInstance()
		auto pINode = ip->GetSelNode(0);
		log(L"New node name is {0} : \n", pINode->GetName());

		auto obj = pINode->GetObjectRef();

		Get ahold of parameter block
		IParamArray *iCylParams = obj->GetParamBlock();

		 Get the current animation time
		TimeValue time = ip->GetTime();*/
		
		//generateInterFacesID();
		//Interface_ID a(539887512, 898175643);
		//generateInterfaceFuntionsID2(a);
		//generateInterfaceFuntionsID2(OBJXREFMGR_INTERFACE_ID);
		//generateInterfaceFuntionsID2(ISOLATESELECTION_INTERFACE_ID);
	
		//setIsolate(true);

		//LayerInterfaceExample();
		//y3d::IBatchProOptimizer y;
		//BatchProOptimizer(y);
		/*GetCOREInterface16()->load*/
		//std::wstring oFileDir = L"F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene";
		//std::string oFileDir = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene";
 		//std::wstring projectPath = L"F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene";
 		std::wstring projectPath = L"F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene";
  		std::wstring pname = L"001";
  		xref_low(projectPath, pname);
		//TestCallMyFP();
		//ProjectInfo pi;
		//pi.set_project_path(projectPath);
		//pi.set_pname(oFileName);
		//pi.set_original_path(projectPath + oFileName + ".max");



		//std::wstring low_file = formatWS("{0}\\{1}_low0.max", projectPath, oFileName);
		//INodeTab hight_nodes;
		//auto ip16 = GetCOREInterface16();
		//GetCOREInterface16()->GetSelNodeTab(hight_nodes);
		//std::wstring high_file = formatWS("{0}\\{1}_high.max", projectPath, oFileName);
		//ip16->FileSaveNodes(&hight_nodes, high_file.c_str());

		//pre_optimize(oFileDir, oFileName, projectPath);

 		//std::string maxFile = projectPath + "\\" + oFileName + "90.max";
// 		GetCOREInterface16()->LoadFromFile(s2ws(maxFile).data(), Interface8::LoadFromFileFlags::kSuppressPrompts&Interface8::LoadFromFileFlags::kUseFileUnits);
		//auto ip16 = GetCOREInterface16();
		//TestCreateLayer();
// 		bool isquiet = ip16->GetQuietMode();
// 		LOG("is quiet {}", isquiet);
// 		QuietMode a;
// 		a.set();
// 		bool loadsucess = ip16->LoadFromFile(s2ws(maxFile).data(),Interface8::LoadFromFileFlags::kSuppressPrompts&Interface8::LoadFromFileFlags::kUseFileUnits);
// 
// 		ip16->SaveToFile(s2ws(maxFile).data());
		//ip16->ImportFromFile(s2ws(maxFile).data(), false);		
		//LOG("is loadsuccess {0}",loadsucess);
		//MSTR internal_name = fpInterfaceDesc->internal_name;
		//logserver::LOG("Internal name is {0}\n", internal_name);
		//ULONG flags = fpInterfaceDesc->flags;
		//std::stringstream stream;
		//stream << std::hex << flags;
		//std::string result(stream.str());
		//logserver::LOG("Internal name is {0}, flags is {1}\n", internal_name, flags);
	});
	//logserver::LOG("Hello world from ServiceImpl\n");

	return Status::OK;
}

Status YServiceTestImpl::MTest2(ServerContext* context, const StringParam* request, EmptyParam* reply)
{
	Invoke([&request]() {
		//DeleteLayer(request->str());
		setIsolateLayer(request->str());
	});
	return Status::OK;
}

Status YServiceTestImpl::GenerateInterfaceInfo(ServerContext * context, const EmptyParam * request, EmptyParam * reply)
{
	Invoke([]() {
		generateInterfaceFuntionsID2(OBJXREFMGR_INTERFACE_ID);
	});
	return Status::OK;
}
void YServiceTestImpl::Initialize(void* codegen, void* gli)
{
	grpc::g_core_codegen_interface = (grpc::CoreCodegenInterface*)codegen;
	grpc::g_glip = (grpc::GrpcLibraryInterface*)gli;
}

YServiceTestImpl::~YServiceTestImpl()
{

}

YSERVICE_API ServiceTestSharedPtr APIENTRY getServiceTestPtr()
{
	return std::shared_ptr<AbstractTestService>(new YServiceTestImpl);
}

ServiceTestPtr APIENTRY getServiceTestImpl()
{
	return new YServiceTestImpl();
}

YSERVICE_API void APIENTRY releaseObjectTest(ServiceTestPtr obj)
{
	if (obj) delete obj;
}
