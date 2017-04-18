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

void generateInterFacesID()
{
	/************************************************************************/
	/* show all the core interface id of 3ds max
	*/
	/************************************************************************/
	int num = NumCOREInterfaces();
	logserver::LOG("Num interface is {0}", num);

	int index = 127;
	logserver::LOG("cls");
	for (int index = 0; index < num; index++)
	{
		FPInterface* fpInterface = GetCOREInterfaceAt(index);
		FPInterfaceDesc* fpInterfaceDesc = fpInterface->GetDesc();
		MSTR internal_name = fpInterfaceDesc->internal_name;
		//logserver::LOG("internal name of interface at index {0} is {1}\n", index, internal_name);
		internal_name.toUpper();
		Interface_ID interfaceID = fpInterface->GetID();
		//logserver::LOG("internal name of interface at index {0} is {1}, interface_id is ({3},{4})\n", index, internal_name,a,b);
		ULONG a = interfaceID.PartA();
		ULONG b = interfaceID.PartB();
		std::stringstream stream;
		stream << std::hex << a;
		std::string hexa(stream.str());
		stream.str("");
		stream << std::hex << b;
		std::string hexb(stream.str());
		logserver::LOG("#define {0}_INTERFACE_ID  Interface_ID(0x{1},0x{2})\n",internal_name, hexa, hexb);
	}
}

inline std::string InterFaceID2S(Interface_ID interfaceID)
{
	std::stringstream buffer;	
	ULONG a = interfaceID.PartA();
	ULONG b = interfaceID.PartB();
	std::stringstream stream;
	stream << std::hex << a;
	std::string hexa(stream.str());
	stream.str("");
	stream << std::hex << b;
	std::string hexb(stream.str());
	buffer << "Interface_ID(0x"<<hexa<<",0x" <<hexb<<")"<<std::endl;
	return buffer.str();
	//logserver::LOG("#define {0}_INTERFACE_ID  Interface_ID(0x{1},0x{2})\n", internal_name, hexa, hexb);
}

void generateInterfaceFuntionsID(Interface_ID id)
{
	FPInterface* fpInterface = GetCOREInterface(id);
	FPInterfaceDesc* fpInterfaceDesc = fpInterface->GetDesc();
	MSTR internal_name = fpInterfaceDesc->internal_name;
	internal_name.toUpper();	
	Tab<FPFunctionDef*> functions = fpInterfaceDesc->functions;
	auto numFunction = functions.Count();
	//define number of functions
	LOG("#define I{0}_NUMFUCNTIONS {1}\n", internal_name, numFunction);

	//LOG("- Num functions is {0}\n", numFunction);
	for (int i = 0; i < numFunction; i++)
	{
		auto f = functions[i];
		auto func_internalname = f->internal_name;
		func_internalname.toUpper();
		auto func_id = f->ID;
		//define function ID
		LOG("#define {0}_I{1} {2}\n", func_internalname,internal_name,func_id);
		//log(" + function number {0} have internal name is {1}, id is {2}\n", i, func_internalname, func_id);
	}

	Tab<FPPropDef*> props = fpInterfaceDesc->props;
	auto numProps = props.Count();
	LOG("#define I{0}_NUMPROPS {1}\n", internal_name, numProps);
	//LOG("- Num Properties is {0}\n", numProps);
	for (int i = 0; i < numProps; i++)
	{
		auto f = props[i];
		auto prop_internalname = f->internal_name;
		prop_internalname.toUpper();
		auto fucn_setterid = f->setter_ID;
		auto func_getterid = f->getter_ID;
		LOG("#define {0}_I{1}_GETTER {2}\n", prop_internalname, internal_name,func_getterid);
		LOG("#define {0}_I{1}_SETTER {2}\n", prop_internalname, internal_name, fucn_setterid);

		/*log(" + properties number {0} have internal name is {1},"
			" setterid is {2}, getterid is {3}\n", i, prop_internalname, fucn_setterid, func_getterid);*/
	}
}

std::map<int, std::string>  map;
std::map<int, std::string> generateParamType()
{
	//static std::map<int, std::string> map;
	if (!map.empty())   //wn: not thread safe
		return map;
	map[ParamType2::TYPE_PCNT_FRAC] = "ParamType2::TYPE_PCNT_FRAC";
	map[ParamType2::TYPE_WORLD] = "ParamType2::TYPE_WORLD";
	map[ParamType2::TYPE_STRING] = "ParamType2::TYPE_STRING";
	map[ParamType2::TYPE_FILENAME] = "ParamType2::TYPE_FILENAME";
	map[ParamType2::TYPE_HSV] = "ParamType2::TYPE_HSV";
	map[ParamType2::TYPE_COLOR_CHANNEL] = "ParamType2::TYPE_COLOR_CHANNEL";
	map[ParamType2::TYPE_TIMEVALUE] = "ParamType2::TYPE_TIMEVALUE";
	map[ParamType2::TYPE_RADIOBTN_INDEX] = "ParamType2::TYPE_RADIOBTN_INDEX";
	map[ParamType2::TYPE_MTL] = "ParamType2::TYPE_MTL";
	map[ParamType2::TYPE_TEXMAP] = "ParamType2::TYPE_TEXMAP";
	map[ParamType2::TYPE_BITMAP] = "ParamType2::TYPE_BITMAP";
	map[ParamType2::TYPE_INODE] = "ParamType2::TYPE_INODE";
	map[ParamType2::TYPE_REFTARG] = "ParamType2::TYPE_REFTARG";
	map[ParamType2::TYPE_INDEX] = "ParamType2::TYPE_INDEX";
	map[ParamType2::TYPE_MATRIX3] = "ParamType2::TYPE_MATRIX3";
	map[ParamType2::TYPE_PBLOCK2] = "ParamType2::TYPE_PBLOCK2";
	map[ParamType2::TYPE_POINT4] = "ParamType2::TYPE_POINT4";
	map[ParamType2::TYPE_FRGBA] = "ParamType2::TYPE_FRGBA";
	map[ParamType2::TYPE_ENUM] = "ParamType2::TYPE_ENUM";
	map[ParamType2::TYPE_VOID] = "ParamType2::TYPE_VOID";
	map[ParamType2::TYPE_INTERVAL] = "ParamType2::TYPE_INTERVAL";
	map[ParamType2::TYPE_ANGAXIS] = "ParamType2::TYPE_ANGAXIS";
	map[ParamType2::TYPE_QUAT] = "ParamType2::TYPE_QUAT";
	map[ParamType2::TYPE_RAY] = "ParamType2::TYPE_RAY";
	map[ParamType2::TYPE_POINT2] = "ParamType2::TYPE_POINT2";
	map[ParamType2::TYPE_BITARRAY] = "ParamType2::TYPE_BITARRAY";
	map[ParamType2::TYPE_CLASS] = "ParamType2::TYPE_CLASS";
	map[ParamType2::TYPE_MESH] = "ParamType2::TYPE_MESH";
	map[ParamType2::TYPE_OBJECT] = "ParamType2::TYPE_OBJECT";
	map[ParamType2::TYPE_CONTROL] = "ParamType2::TYPE_CONTROL";
	map[ParamType2::TYPE_POINT] = "ParamType2::TYPE_POINT";
	map[ParamType2::TYPE_TSTR] = "ParamType2::TYPE_TSTR";
	map[ParamType2::TYPE_IOBJECT] = "ParamType2::TYPE_IOBJECT";
	map[ParamType2::TYPE_INTERFACE] = "ParamType2::TYPE_INTERFACE";
	map[ParamType2::TYPE_HWND] = "ParamType2::TYPE_HWND";
	map[ParamType2::TYPE_NAME] = "ParamType2::TYPE_NAME";
	map[ParamType2::TYPE_COLOR] = "ParamType2::TYPE_COLOR";
	map[ParamType2::TYPE_FPVALUE] = "ParamType2::TYPE_FPVALUE";
	map[ParamType2::TYPE_VALUE] = "ParamType2::TYPE_VALUE";
	map[ParamType2::TYPE_DWORD] = "ParamType2::TYPE_DWORD";
	map[ParamType2::TYPE_bool] = "ParamType2::TYPE_bool";
	map[ParamType2::TYPE_INTPTR] = "ParamType2::TYPE_INTPTR";
	map[ParamType2::TYPE_INT64] = "ParamType2::TYPE_INT64";
	map[ParamType2::TYPE_DOUBLE] = "ParamType2::TYPE_DOUBLE";
	map[ParamType2::TYPE_BOX3] = "ParamType2::TYPE_BOX3";
	map[ParamType2::TYPE_BEZIERSHAPE] = "ParamType2::TYPE_BEZIERSHAPE";
	map[ParamType2::TYPE_FLOAT_TAB] = "ParamType2::TYPE_FLOAT_TAB";
	map[ParamType2::TYPE_INT_TAB] = "ParamType2::TYPE_INT_TAB";
	map[ParamType2::TYPE_RGBA_TAB] = "ParamType2::TYPE_RGBA_TAB";
	map[ParamType2::TYPE_POINT3_TAB] = "ParamType2::TYPE_POINT3_TAB";
	map[ParamType2::TYPE_BOOL_TAB] = "ParamType2::TYPE_BOOL_TAB";
	map[ParamType2::TYPE_ANGLE_TAB] = "ParamType2::TYPE_ANGLE_TAB";
	map[ParamType2::TYPE_PCNT_FRAC_TAB] = "ParamType2::TYPE_PCNT_FRAC_TAB";
	map[ParamType2::TYPE_WORLD_TAB] = "ParamType2::TYPE_WORLD_TAB";
	map[ParamType2::TYPE_STRING_TAB] = "ParamType2::TYPE_STRING_TAB";
	map[ParamType2::TYPE_FILENAME_TAB] = "ParamType2::TYPE_FILENAME_TAB";
	map[ParamType2::TYPE_HSV_TAB] = "ParamType2::TYPE_HSV_TAB";
	map[ParamType2::TYPE_COLOR_CHANNEL_TAB] = "ParamType2::TYPE_COLOR_CHANNEL_TAB";
	map[ParamType2::TYPE_TIMEVALUE_TAB] = "ParamType2::TYPE_TIMEVALUE_TAB";
	map[ParamType2::TYPE_RADIOBTN_INDEX_TAB] = "ParamType2::TYPE_RADIOBTN_INDEX_TAB";
	map[ParamType2::TYPE_MTL_TAB] = "ParamType2::TYPE_MTL_TAB";
	map[ParamType2::TYPE_TEXMAP_TAB] = "ParamType2::TYPE_TEXMAP_TAB";
	map[ParamType2::TYPE_BITMAP_TAB] = "ParamType2::TYPE_BITMAP_TAB";
	map[ParamType2::TYPE_INODE_TAB] = "ParamType2::TYPE_INODE_TAB";
	map[ParamType2::TYPE_REFTARG_TAB] = "ParamType2::TYPE_REFTARG_TAB";
	map[ParamType2::TYPE_INDEX_TAB] = "ParamType2::TYPE_INDEX_TAB";
	map[ParamType2::TYPE_MATRIX3_TAB] = "ParamType2::TYPE_MATRIX3_TAB";
	map[ParamType2::TYPE_PBLOCK2_TAB] = "ParamType2::TYPE_PBLOCK2_TAB";
	map[ParamType2::TYPE_POINT4_TAB] = "ParamType2::TYPE_POINT4_TAB";
	map[ParamType2::TYPE_FRGBA_TAB] = "ParamType2::TYPE_FRGBA_TAB";
	map[ParamType2::TYPE_VOID_TAB] = "ParamType2::TYPE_VOID_TAB";
	map[ParamType2::TYPE_INTERVAL_TAB] = "ParamType2::TYPE_INTERVAL_TAB";
	map[ParamType2::TYPE_ANGAXIS_TAB] = "ParamType2::TYPE_ANGAXIS_TAB";
	map[ParamType2::TYPE_QUAT_TAB] = "ParamType2::TYPE_QUAT_TAB";
	map[ParamType2::TYPE_RAY_TAB] = "ParamType2::TYPE_RAY_TAB";
	map[ParamType2::TYPE_POINT2_TAB] = "ParamType2::TYPE_POINT2_TAB";
	map[ParamType2::TYPE_BITARRAY_TAB] = "ParamType2::TYPE_BITARRAY_TAB";
	map[ParamType2::TYPE_CLASS_TAB] = "ParamType2::TYPE_CLASS_TAB";
	map[ParamType2::TYPE_MESH_TAB] = "ParamType2::TYPE_MESH_TAB";
	map[ParamType2::TYPE_OBJECT_TAB] = "ParamType2::TYPE_OBJECT_TAB";
	map[ParamType2::TYPE_CONTROL_TAB] = "ParamType2::TYPE_CONTROL_TAB";
	map[ParamType2::TYPE_POINT_TAB] = "ParamType2::TYPE_POINT_TAB";
	map[ParamType2::TYPE_TSTR_TAB] = "ParamType2::TYPE_TSTR_TAB";
	map[ParamType2::TYPE_IOBJECT_TAB] = "ParamType2::TYPE_IOBJECT_TAB";
	map[ParamType2::TYPE_INTERFACE_TAB] = "ParamType2::TYPE_INTERFACE_TAB";
	map[ParamType2::TYPE_HWND_TAB] = "ParamType2::TYPE_HWND_TAB";
	map[ParamType2::TYPE_NAME_TAB] = "ParamType2::TYPE_NAME_TAB";
	map[ParamType2::TYPE_COLOR_TAB] = "ParamType2::TYPE_COLOR_TAB";
	map[ParamType2::TYPE_FPVALUE_TAB] = "ParamType2::TYPE_FPVALUE_TAB";
	map[ParamType2::TYPE_VALUE_TAB] = "ParamType2::TYPE_VALUE_TAB";
	map[ParamType2::TYPE_DWORD_TAB] = "ParamType2::TYPE_DWORD_TAB";
	map[ParamType2::TYPE_bool_TAB] = "ParamType2::TYPE_bool_TAB";
	map[ParamType2::TYPE_INTPTR_TAB] = "ParamType2::TYPE_INTPTR_TAB";
	map[ParamType2::TYPE_INT64_TAB] = "ParamType2::TYPE_INT64_TAB";
	map[ParamType2::TYPE_DOUBLE_TAB] = "ParamType2::TYPE_DOUBLE_TAB";
	map[ParamType2::TYPE_BOX3_TAB] = "ParamType2::TYPE_BOX3_TAB";
	map[ParamType2::TYPE_BEZIERSHAPE_TAB] = "ParamType2::TYPE_BEZIERSHAPE_TAB";
	map[ParamType2::TYPE_INT_BR] = "ParamType2::TYPE_INT_BR";
	map[ParamType2::TYPE_BOOL_BR] = "ParamType2::TYPE_BOOL_BR";
	map[ParamType2::TYPE_ANGLE_BR] = "ParamType2::TYPE_ANGLE_BR";
	map[ParamType2::TYPE_PCNT_FRAC_BR] = "ParamType2::TYPE_PCNT_FRAC_BR";
	map[ParamType2::TYPE_WORLD_BR] = "ParamType2::TYPE_WORLD_BR";
	map[ParamType2::TYPE_COLOR_CHANNEL_BR] = "ParamType2::TYPE_COLOR_CHANNEL_BR";
	map[ParamType2::TYPE_TIMEVALUE_BR] = "ParamType2::TYPE_TIMEVALUE_BR";
	map[ParamType2::TYPE_RADIOBTN_INDEX_BR] = "ParamType2::TYPE_RADIOBTN_INDEX_BR";
	map[ParamType2::TYPE_INDEX_BR] = "ParamType2::TYPE_INDEX_BR";
	map[ParamType2::TYPE_RGBA_BR] = "ParamType2::TYPE_RGBA_BR";
	map[ParamType2::TYPE_BITMAP_BR] = "ParamType2::TYPE_BITMAP_BR";
	map[ParamType2::TYPE_POINT3_BR] = "ParamType2::TYPE_POINT3_BR";
	map[ParamType2::TYPE_HSV_BR] = "ParamType2::TYPE_HSV_BR";
	map[ParamType2::TYPE_REFTARG_BR] = "ParamType2::TYPE_REFTARG_BR";
	map[ParamType2::TYPE_MATRIX3_BR] = "ParamType2::TYPE_MATRIX3_BR";
	map[ParamType2::TYPE_POINT4_BR] = "ParamType2::TYPE_POINT4_BR";
	map[ParamType2::TYPE_FRGBA_BR] = "ParamType2::TYPE_FRGBA_BR";
	map[ParamType2::TYPE_ENUM_BR] = "ParamType2::TYPE_ENUM_BR";
	map[ParamType2::TYPE_INTERVAL_BR] = "ParamType2::TYPE_INTERVAL_BR";
	map[ParamType2::TYPE_ANGAXIS_BR] = "ParamType2::TYPE_ANGAXIS_BR";
	map[ParamType2::TYPE_QUAT_BR] = "ParamType2::TYPE_QUAT_BR";
	map[ParamType2::TYPE_RAY_BR] = "ParamType2::TYPE_RAY_BR";
	map[ParamType2::TYPE_POINT2_BR] = "ParamType2::TYPE_POINT2_BR";
	map[ParamType2::TYPE_BITARRAY_BR] = "ParamType2::TYPE_BITARRAY_BR";
	map[ParamType2::TYPE_MESH_BR] = "ParamType2::TYPE_MESH_BR";
	map[ParamType2::TYPE_POINT_BR] = "ParamType2::TYPE_POINT_BR";
	map[ParamType2::TYPE_TSTR_BR] = "ParamType2::TYPE_TSTR_BR";
	map[ParamType2::TYPE_COLOR_BR] = "ParamType2::TYPE_COLOR_BR";
	map[ParamType2::TYPE_FPVALUE_BR] = "ParamType2::TYPE_FPVALUE_BR";
	map[ParamType2::TYPE_DWORD_BR] = "ParamType2::TYPE_DWORD_BR";
	map[ParamType2::TYPE_bool_BR] = "ParamType2::TYPE_bool_BR";
	map[ParamType2::TYPE_INTPTR_BR] = "ParamType2::TYPE_INTPTR_BR";
	map[ParamType2::TYPE_INT64_BR] = "ParamType2::TYPE_INT64_BR";
	map[ParamType2::TYPE_DOUBLE_BR] = "ParamType2::TYPE_DOUBLE_BR";
	map[ParamType2::TYPE_BOX3_BR] = "ParamType2::TYPE_BOX3_BR";
	map[ParamType2::TYPE_BEZIERSHAPE_BR] = "ParamType2::TYPE_BEZIERSHAPE_BR";
	map[ParamType2::TYPE_FLOAT_TAB_BR] = "ParamType2::TYPE_FLOAT_TAB_BR";
	map[ParamType2::TYPE_INT_TAB_BR] = "ParamType2::TYPE_INT_TAB_BR";
	map[ParamType2::TYPE_RGBA_TAB_BR] = "ParamType2::TYPE_RGBA_TAB_BR";
	map[ParamType2::TYPE_POINT3_TAB_BR] = "ParamType2::TYPE_POINT3_TAB_BR";
	map[ParamType2::TYPE_BOOL_TAB_BR] = "ParamType2::TYPE_BOOL_TAB_BR";
	map[ParamType2::TYPE_ANGLE_TAB_BR] = "ParamType2::TYPE_ANGLE_TAB_BR";
	map[ParamType2::TYPE_PCNT_FRAC_TAB_BR] = "ParamType2::TYPE_PCNT_FRAC_TAB_BR";
	map[ParamType2::TYPE_WORLD_TAB_BR] = "ParamType2::TYPE_WORLD_TAB_BR";
	map[ParamType2::TYPE_STRING_TAB_BR] = "ParamType2::TYPE_STRING_TAB_BR";
	map[ParamType2::TYPE_FILENAME_TAB_BR] = "ParamType2::TYPE_FILENAME_TAB_BR";
	map[ParamType2::TYPE_HSV_TAB_BR] = "ParamType2::TYPE_HSV_TAB_BR";
	map[ParamType2::TYPE_COLOR_CHANNEL_TAB_BR] = "ParamType2::TYPE_COLOR_CHANNEL_TAB_BR";
	map[ParamType2::TYPE_TIMEVALUE_TAB_BR] = "ParamType2::TYPE_TIMEVALUE_TAB_BR";
	map[ParamType2::TYPE_RADIOBTN_INDEX_TAB_BR] = "ParamType2::TYPE_RADIOBTN_INDEX_TAB_BR";
	map[ParamType2::TYPE_MTL_TAB_BR] = "ParamType2::TYPE_MTL_TAB_BR";
	map[ParamType2::TYPE_TEXMAP_TAB_BR] = "ParamType2::TYPE_TEXMAP_TAB_BR";
	map[ParamType2::TYPE_BITMAP_TAB_BR] = "ParamType2::TYPE_BITMAP_TAB_BR";
	map[ParamType2::TYPE_INODE_TAB_BR] = "ParamType2::TYPE_INODE_TAB_BR";
	map[ParamType2::TYPE_REFTARG_TAB_BR] = "ParamType2::TYPE_REFTARG_TAB_BR";
	map[ParamType2::TYPE_INDEX_TAB_BR] = "ParamType2::TYPE_INDEX_TAB_BR";
	map[ParamType2::TYPE_MATRIX3_TAB_BR] = "ParamType2::TYPE_MATRIX3_TAB_BR";
	map[ParamType2::TYPE_POINT4_TAB_BR] = "ParamType2::TYPE_POINT4_TAB_BR";
	map[ParamType2::TYPE_FRGBA_TAB_BR] = "ParamType2::TYPE_FRGBA_TAB_BR";
	map[ParamType2::TYPE_TSTR_TAB_BR] = "ParamType2::TYPE_TSTR_TAB_BR";
	map[ParamType2::TYPE_ENUM_TAB_BR] = "ParamType2::TYPE_ENUM_TAB_BR";
	map[ParamType2::TYPE_INTERVAL_TAB_BR] = "ParamType2::TYPE_INTERVAL_TAB_BR";
	map[ParamType2::TYPE_ANGAXIS_TAB_BR] = "ParamType2::TYPE_ANGAXIS_TAB_BR";
	map[ParamType2::TYPE_QUAT_TAB_BR] = "ParamType2::TYPE_QUAT_TAB_BR";
	map[ParamType2::TYPE_RAY_TAB_BR] = "ParamType2::TYPE_RAY_TAB_BR";
	map[ParamType2::TYPE_POINT2_TAB_BR] = "ParamType2::TYPE_POINT2_TAB_BR";
	map[ParamType2::TYPE_BITARRAY_TAB_BR] = "ParamType2::TYPE_BITARRAY_TAB_BR";
	map[ParamType2::TYPE_CLASS_TAB_BR] = "ParamType2::TYPE_CLASS_TAB_BR";
	map[ParamType2::TYPE_MESH_TAB_BR] = "ParamType2::TYPE_MESH_TAB_BR";
	map[ParamType2::TYPE_OBJECT_TAB_BR] = "ParamType2::TYPE_OBJECT_TAB_BR";
	map[ParamType2::TYPE_CONTROL_TAB_BR] = "ParamType2::TYPE_CONTROL_TAB_BR";
	map[ParamType2::TYPE_POINT_TAB_BR] = "ParamType2::TYPE_POINT_TAB_BR";
	map[ParamType2::TYPE_IOBJECT_TAB_BR] = "ParamType2::TYPE_IOBJECT_TAB_BR";
	map[ParamType2::TYPE_INTERFACE_TAB_BR] = "ParamType2::TYPE_INTERFACE_TAB_BR";
	map[ParamType2::TYPE_HWND_TAB_BR] = "ParamType2::TYPE_HWND_TAB_BR";
	map[ParamType2::TYPE_NAME_TAB_BR] = "ParamType2::TYPE_NAME_TAB_BR";
	map[ParamType2::TYPE_COLOR_TAB_BR] = "ParamType2::TYPE_COLOR_TAB_BR";
	map[ParamType2::TYPE_FPVALUE_TAB_BR] = "ParamType2::TYPE_FPVALUE_TAB_BR";
	map[ParamType2::TYPE_VALUE_TAB_BR] = "ParamType2::TYPE_VALUE_TAB_BR";
	map[ParamType2::TYPE_DWORD_TAB_BR] = "ParamType2::TYPE_DWORD_TAB_BR";
	map[ParamType2::TYPE_bool_TAB_BR] = "ParamType2::TYPE_bool_TAB_BR";
	map[ParamType2::TYPE_INTPTR_TAB_BR] = "ParamType2::TYPE_INTPTR_TAB_BR";
	map[ParamType2::TYPE_INT64_TAB_BR] = "ParamType2::TYPE_INT64_TAB_BR";
	map[ParamType2::TYPE_DOUBLE_TAB_BR] = "ParamType2::TYPE_DOUBLE_TAB_BR";
	map[ParamType2::TYPE_BOX3_TAB_BR] = "ParamType2::TYPE_BOX3_TAB_BR";
	map[ParamType2::TYPE_BEZIERSHAPE_TAB_BR] = "ParamType2::TYPE_BEZIERSHAPE_TAB_BR";
	map[ParamType2::TYPE_RGBA_BV] = "ParamType2::TYPE_RGBA_BV";
	map[ParamType2::TYPE_POINT3_BV] = "ParamType2::TYPE_POINT3_BV";
	map[ParamType2::TYPE_HSV_BV] = "ParamType2::TYPE_HSV_BV";
	map[ParamType2::TYPE_INTERVAL_BV] = "ParamType2::TYPE_INTERVAL_BV";
	map[ParamType2::TYPE_BITMAP_BV] = "ParamType2::TYPE_BITMAP_BV";
	map[ParamType2::TYPE_MATRIX3_BV] = "ParamType2::TYPE_MATRIX3_BV";
	map[ParamType2::TYPE_POINT4_BV] = "ParamType2::TYPE_POINT4_BV";
	map[ParamType2::TYPE_FRGBA_BV] = "ParamType2::TYPE_FRGBA_BV";
	map[ParamType2::TYPE_ANGAXIS_BV] = "ParamType2::TYPE_ANGAXIS_BV";
	map[ParamType2::TYPE_QUAT_BV] = "ParamType2::TYPE_QUAT_BV";
	map[ParamType2::TYPE_RAY_BV] = "ParamType2::TYPE_RAY_BV";
	map[ParamType2::TYPE_POINT2_BV] = "ParamType2::TYPE_POINT2_BV";
	map[ParamType2::TYPE_BITARRAY_BV] = "ParamType2::TYPE_BITARRAY_BV";
	map[ParamType2::TYPE_MESH_BV] = "ParamType2::TYPE_MESH_BV";
	map[ParamType2::TYPE_POINT_BV] = "ParamType2::TYPE_POINT_BV";
	map[ParamType2::TYPE_TSTR_BV] = "ParamType2::TYPE_TSTR_BV";
	map[ParamType2::TYPE_COLOR_BV] = "ParamType2::TYPE_COLOR_BV";
	map[ParamType2::TYPE_FPVALUE_BV] = "ParamType2::TYPE_FPVALUE_BV";
	map[ParamType2::TYPE_CLASS_BV] = "ParamType2::TYPE_CLASS_BV";
	map[ParamType2::TYPE_BOX3_BV] = "ParamType2::TYPE_BOX3_BV";
	map[ParamType2::TYPE_BEZIERSHAPE_BV] = "ParamType2::TYPE_BEZIERSHAPE_BV";
	map[ParamType2::TYPE_FLOAT_TAB_BV] = "ParamType2::TYPE_FLOAT_TAB_BV";
	map[ParamType2::TYPE_INT_TAB_BV] = "ParamType2::TYPE_INT_TAB_BV";
	map[ParamType2::TYPE_RGBA_TAB_BV] = "ParamType2::TYPE_RGBA_TAB_BV";
	map[ParamType2::TYPE_POINT3_TAB_BV] = "ParamType2::TYPE_POINT3_TAB_BV";
	map[ParamType2::TYPE_BOOL_TAB_BV] = "ParamType2::TYPE_BOOL_TAB_BV";
	map[ParamType2::TYPE_ANGLE_TAB_BV] = "ParamType2::TYPE_ANGLE_TAB_BV";
	map[ParamType2::TYPE_PCNT_FRAC_TAB_BV] = "ParamType2::TYPE_PCNT_FRAC_TAB_BV";
	map[ParamType2::TYPE_WORLD_TAB_BV] = "ParamType2::TYPE_WORLD_TAB_BV";
	map[ParamType2::TYPE_STRING_TAB_BV] = "ParamType2::TYPE_STRING_TAB_BV";
	map[ParamType2::TYPE_FILENAME_TAB_BV] = "ParamType2::TYPE_FILENAME_TAB_BV";
	map[ParamType2::TYPE_HSV_TAB_BV] = "ParamType2::TYPE_HSV_TAB_BV";
	map[ParamType2::TYPE_COLOR_CHANNEL_TAB_BV] = "ParamType2::TYPE_COLOR_CHANNEL_TAB_BV";
	map[ParamType2::TYPE_TIMEVALUE_TAB_BV] = "ParamType2::TYPE_TIMEVALUE_TAB_BV";
	map[ParamType2::TYPE_RADIOBTN_INDEX_TAB_BV] = "ParamType2::TYPE_RADIOBTN_INDEX_TAB_BV";
	map[ParamType2::TYPE_MTL_TAB_BV] = "ParamType2::TYPE_MTL_TAB_BV";
	map[ParamType2::TYPE_TEXMAP_TAB_BV] = "ParamType2::TYPE_TEXMAP_TAB_BV";
	map[ParamType2::TYPE_BITMAP_TAB_BV] = "ParamType2::TYPE_BITMAP_TAB_BV";
	map[ParamType2::TYPE_INODE_TAB_BV] = "ParamType2::TYPE_INODE_TAB_BV";
	map[ParamType2::TYPE_REFTARG_TAB_BV] = "ParamType2::TYPE_REFTARG_TAB_BV";
	map[ParamType2::TYPE_INDEX_TAB_BV] = "ParamType2::TYPE_INDEX_TAB_BV";
	map[ParamType2::TYPE_MATRIX3_TAB_BV] = "ParamType2::TYPE_MATRIX3_TAB_BV";
	map[ParamType2::TYPE_POINT4_TAB_BV] = "ParamType2::TYPE_POINT4_TAB_BV";
	map[ParamType2::TYPE_FRGBA_TAB_BV] = "ParamType2::TYPE_FRGBA_TAB_BV";
	map[ParamType2::TYPE_PBLOCK2_TAB_BV] = "ParamType2::TYPE_PBLOCK2_TAB_BV";
	map[ParamType2::TYPE_VOID_TAB_BV] = "ParamType2::TYPE_VOID_TAB_BV";
	map[ParamType2::TYPE_TSTR_TAB_BV] = "ParamType2::TYPE_TSTR_TAB_BV";
	map[ParamType2::TYPE_ENUM_TAB_BV] = "ParamType2::TYPE_ENUM_TAB_BV";
	map[ParamType2::TYPE_INTERVAL_TAB_BV] = "ParamType2::TYPE_INTERVAL_TAB_BV";
	map[ParamType2::TYPE_ANGAXIS_TAB_BV] = "ParamType2::TYPE_ANGAXIS_TAB_BV";
	map[ParamType2::TYPE_QUAT_TAB_BV] = "ParamType2::TYPE_QUAT_TAB_BV";
	map[ParamType2::TYPE_RAY_TAB_BV] = "ParamType2::TYPE_RAY_TAB_BV";
	map[ParamType2::TYPE_POINT2_TAB_BV] = "ParamType2::TYPE_POINT2_TAB_BV";
	map[ParamType2::TYPE_BITARRAY_TAB_BV] = "ParamType2::TYPE_BITARRAY_TAB_BV";
	map[ParamType2::TYPE_CLASS_TAB_BV] = "ParamType2::TYPE_CLASS_TAB_BV";
	map[ParamType2::TYPE_MESH_TAB_BV] = "ParamType2::TYPE_MESH_TAB_BV";
	map[ParamType2::TYPE_OBJECT_TAB_BV] = "ParamType2::TYPE_OBJECT_TAB_BV";
	map[ParamType2::TYPE_CONTROL_TAB_BV] = "ParamType2::TYPE_CONTROL_TAB_BV";
	map[ParamType2::TYPE_POINT_TAB_BV] = "ParamType2::TYPE_POINT_TAB_BV";
	map[ParamType2::TYPE_IOBJECT_TAB_BV] = "ParamType2::TYPE_IOBJECT_TAB_BV";
	map[ParamType2::TYPE_INTERFACE_TAB_BV] = "ParamType2::TYPE_INTERFACE_TAB_BV";
	map[ParamType2::TYPE_HWND_TAB_BV] = "ParamType2::TYPE_HWND_TAB_BV";
	map[ParamType2::TYPE_NAME_TAB_BV] = "ParamType2::TYPE_NAME_TAB_BV";
	map[ParamType2::TYPE_COLOR_TAB_BV] = "ParamType2::TYPE_COLOR_TAB_BV";
	map[ParamType2::TYPE_FPVALUE_TAB_BV] = "ParamType2::TYPE_FPVALUE_TAB_BV";
	map[ParamType2::TYPE_VALUE_TAB_BV] = "ParamType2::TYPE_VALUE_TAB_BV";
	map[ParamType2::TYPE_DWORD_TAB_BV] = "ParamType2::TYPE_DWORD_TAB_BV";
	map[ParamType2::TYPE_bool_TAB_BV] = "ParamType2::TYPE_bool_TAB_BV";
	map[ParamType2::TYPE_INTPTR_TAB_BV] = "ParamType2::TYPE_INTPTR_TAB_BV";
	map[ParamType2::TYPE_INT64_TAB_BV] = "ParamType2::TYPE_INT64_TAB_BV";
	map[ParamType2::TYPE_DOUBLE_TAB_BV] = "ParamType2::TYPE_DOUBLE_TAB_BV";
	map[ParamType2::TYPE_BOX3_TAB_BV] = "ParamType2::TYPE_BOX3_TAB_BV";
	map[ParamType2::TYPE_BEZIERSHAPE_TAB_BV] = "ParamType2::TYPE_BEZIERSHAPE_TAB_BV";
	map[ParamType2::TYPE_FLOAT_BP] = "ParamType2::TYPE_FLOAT_BP";
	map[ParamType2::TYPE_INT_BP] = "ParamType2::TYPE_INT_BP";
	map[ParamType2::TYPE_BOOL_BP] = "ParamType2::TYPE_BOOL_BP";
	map[ParamType2::TYPE_ANGLE_BP] = "ParamType2::TYPE_ANGLE_BP";
	map[ParamType2::TYPE_PCNT_FRAC_BP] = "ParamType2::TYPE_PCNT_FRAC_BP";
	map[ParamType2::TYPE_WORLD_BP] = "ParamType2::TYPE_WORLD_BP";
	map[ParamType2::TYPE_COLOR_CHANNEL_BP] = "ParamType2::TYPE_COLOR_CHANNEL_BP";
	map[ParamType2::TYPE_TIMEVALUE_BP] = "ParamType2::TYPE_TIMEVALUE_BP";
	map[ParamType2::TYPE_RADIOBTN_INDEX_BP] = "ParamType2::TYPE_RADIOBTN_INDEX_BP";
	map[ParamType2::TYPE_INDEX_BP] = "ParamType2::TYPE_INDEX_BP";
	map[ParamType2::TYPE_ENUM_BP] = "ParamType2::TYPE_ENUM_BP";
	map[ParamType2::TYPE_DWORD_BP] = "ParamType2::TYPE_DWORD_BP";
	map[ParamType2::TYPE_bool_BP] = "ParamType2::TYPE_bool_BP";
	map[ParamType2::TYPE_INTPTR_BP] = "ParamType2::TYPE_INTPTR_BP";
	map[ParamType2::TYPE_INT64_BP] = "ParamType2::TYPE_INT64_BP";
	map[ParamType2::TYPE_DOUBLE_BP] = "ParamType2::TYPE_DOUBLE_BP";
	map[ParamType2::TYPE_KEYARG_MARKER] = "ParamType2::TYPE_KEYARG_MARKER";
	map[ParamType2::TYPE_MSFLOAT] = "ParamType2::TYPE_MSFLOAT";
	map[ParamType2::TYPE_UNSPECIFIED] = "ParamType2::TYPE_UNSPECIFIED";

	map[ParamType::TYPE_FLOAT] = "ParamType::TYPE_FLOAT";
	map[ParamType::TYPE_INT] = "ParamType::TYPE_INT";
	map[ParamType::TYPE_RGBA] = "ParamType::TYPE_RGBA";
	map[ParamType::TYPE_POINT3] = "ParamType::TYPE_POINT3";
	map[ParamType::TYPE_BOOL] = "ParamType::TYPE_BOOL";
	map[ParamType::TYPE_USER] = "ParamType::TYPE_USER";

	return map;
}

void generateInterfaceFuntionsID2(FPInterfaceDesc* fpInterfaceDesc)
{
	MSTR internal_name = fpInterfaceDesc->internal_name;
	internal_name.toUpper();
	Tab<FPFunctionDef*> functions = fpInterfaceDesc->functions;
	auto map = generateParamType();
	auto numFunction = functions.Count();
	//define number of functions
	LOG("// --------------------list functions of core interface {0}--------------------------\n", internal_name);
	LOG("// Number function of {0} core interface is {1}\n", internal_name, numFunction);
	LOG("#define I{0}_NUMFUCNTIONS {1}\n", internal_name, numFunction);
	
	log("// --------------------list functions of core interface {0}--------------------------\n", internal_name);
	log("// Number function of {0} core interface is {1}\n", internal_name, numFunction);
	log("#define I{0}_NUMFUCNTIONS {1}\n", internal_name, numFunction);
	//LOG("- Num functions is {0}\n", numFunction);
	for (int i = 0; i < numFunction; i++)
	{
		auto f = functions[i];
		auto func_internalname = f->internal_name;
		func_internalname.toUpper();  //hello
		auto func_id = f->ID;
		//define function ID
		LOG("// function ID of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, func_id);
		LOG("#define {0}_I{1} {2}\n", func_internalname, internal_name, func_id);

		log("// function ID of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, func_id);
		log("#define {0}_I{1} {2}\n", func_internalname, internal_name, func_id);
		auto params = f->params;
		auto numParams = params.Count();
		auto resultType = f->result_type;
		auto resultTypeEnumFormat = map[resultType];
		LOG("  // Result Type of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, resultTypeEnumFormat);
		LOG("  #define {0}_I{1}_RESULTTYPE {2}\n", func_internalname, internal_name, resultTypeEnumFormat);
		LOG("  // number parameter of fucntion {0} of core interface {1} is {2}\n", func_internalname, internal_name, numParams);
		LOG("  #define {0}_I{1}_NUMPARAMS {2}\n", func_internalname, internal_name, numParams);

		log("  // Result Type of function {0} of core interface {1} is {2}\n", func_internalname, internal_name, resultTypeEnumFormat);
		log("  #define {0}_I{1}_RESULTTYPE {2}\n", func_internalname, internal_name, resultTypeEnumFormat);		
		log("  // number parameter of fucntion {0} of core interface {1} is {2}\n", func_internalname, internal_name, numParams);		
		log("  #define {0}_I{1}_NUMPARAMS {2}\n", func_internalname, internal_name, numParams);
		for (int i = 0; i < numParams; i++)
		{
			auto param = params[i];
			auto param_internal_name = param->internal_name;
			param_internal_name.toUpper();
			auto paramType = param->type;
			auto paramTypeEnumFormat = map[paramType];
			LOG("  // {0}, which is parameter number {1} of function\n// {2} of core interface {3} have param type is {4}\n", param_internal_name, i + 1, func_internalname, internal_name, paramTypeEnumFormat);
			LOG("  #define {0}_{1}_I{2}_PARAM{3}_TYPE {4}\n", param_internal_name, func_internalname, internal_name, i + 1, paramTypeEnumFormat);

			log("  #define {0}_{1}_I{2}_PARAM{3}_TYPE {4}\n", param_internal_name, func_internalname, internal_name, i + 1, paramTypeEnumFormat);
			log("  // {0}, which is parameter number {1} of function\n// {2} of core interface {3} have param type is {4}\n", param_internal_name, i + 1, func_internalname, internal_name, paramTypeEnumFormat);
			//LOG("  -- Param number {0} have internal name is {1}, paramType is {2}\n", i, finternal_name, paramTypeEnumFormat);
		}
		//log(" + function number {0} have internal name is {1}, id is {2}\n", i, func_internalname, func_id);
	}

	Tab<FPPropDef*> props = fpInterfaceDesc->props;
	auto numProps = props.Count();
	LOG("// ---------------------- List Properties of core interface {0} ----------------------\n", internal_name);
	log("// ---------------------- List Properties of core interface {0} ----------------------\n", internal_name);
	LOG("#define I{0}_NUMPROPS {1}\n", internal_name, numProps);
	log("#define I{0}_NUMPROPS {1}\n", internal_name, numProps);
	for (int i = 0; i < numProps; i++)
	{
		auto f = props[i];
		auto prop_internalname = f->internal_name;
		prop_internalname.toUpper();
		auto fucn_setterid = f->setter_ID;
		auto func_getterid = f->getter_ID;
		ParamType2 propTypes = f->prop_type;
		auto propTypes_enumFormat = map[(int)propTypes];
		//if (propTypes_enumFormat.empty()) propTypes_enumFormat ";
		LOG("// function_id of for get property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, func_getterid);
		log("// function_id of for get property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, func_getterid);
		LOG("#define {0}_I{1}_GETTER {2}\n", prop_internalname, internal_name, func_getterid);
		log("#define {0}_I{1}_GETTER {2}\n", prop_internalname, internal_name, func_getterid);
		LOG("// function_id of for set property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, fucn_setterid);
		log("// function_id of for set property {0} of core interface {1} is {2}\n", prop_internalname, internal_name, fucn_setterid);
		LOG("#define {0}_I{1}_SETTER {2}\n", prop_internalname, internal_name, fucn_setterid);
		log("#define {0}_I{1}_SETTER {2}\n", prop_internalname, internal_name, fucn_setterid);
		//LOG("#define {0","ddd");
		if (!propTypes_enumFormat.empty()) {
			LOG("// parameter type of properties {0} of core interface {1} is {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			log("// parameter type of properties {0} of core interface {1} is {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			LOG("#define {0}_I{1}_TYPEPARAM {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			log("#define {0}_I{1}_TYPEPARAM {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
		}
		else {
			LOG("#define {0}_I{1}_TYPEPARAM_VOID {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
			log("#define {0}_I{1}_TYPEPARAM_VOID {2}\n", prop_internalname, internal_name, propTypes_enumFormat);
		}
		/*log(" + properties number {0} have internal name is {1},"
		" setterid is {2}, getterid is {3}\n", i, prop_internalname, fucn_setterid, func_getterid);*/
	}

	LOG("// ---------------------- List Enum of interfaces  ----------------------\n");
	Tab<FPEnum *> enumerations = fpInterfaceDesc->enumerations;
	auto numEnum = enumerations.Count();
	
	for (int i = 0; i < numEnum; i++)
	{
		auto e = enumerations[i];
		auto enumID = e->ID;
		auto enum_codes = e->enumeration;		
		auto n = enum_codes.Count();		
		
		std::string str = fmt::format("typedef enum class ENUM{0}_{1} {{", i + 1,internal_name);
		fmt::MemoryWriter w;
		w << str;
		LOG("/**");
		log("/**");
		for (int j = 0; j < n; j++)
		{
			auto enum_code = enum_codes[j];
			const wchar_t* name = enum_code.name;
			int code = enum_code.code;
			std::string s = ws2s(std::wstring(name));
			//std::string a = "ssss";
			//LOG("ffff\n");
			LOG("{0} - {1} Enum is {2} have code is {3}\n", i, j, s, code);
			log("{0} - {1} Enum is {2} have code is {3}\n", i, j, s, code);
			//w << 2;
			w << s << " = " << code <<",";			
			//Printf("{0} - {1} Enum is ",i,j); LOG(s);LOG(" have code is {0}\n",code);
			//LOG("\n");
		}	
		LOG("**/\n");
		log("**/\n");
		std::string str2 = fmt::format("}} {0}_ENUM{1};\n", internal_name, i + 1);
		w << str2;
		LOG(w.str());
		log(w.str());
	}
}

/************************************************************************/
/* generate core interface info with the interface_id
wn: this function only work if the provided interface_id is of "core" interface
*/
/************************************************************************/
void generateInterfaceFuntionsID2(Interface_ID id)
{
	FPInterface* fpInterface = GetCOREInterface(id);
	FPInterfaceDesc* fpInterfaceDesc = fpInterface->GetDesc();
	generateInterfaceFuntionsID2(fpInterfaceDesc);
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

inline void setInterFacePropertyTInt(int typeParam, FPInterface* fpInterface, int funcID, int value)
{
	FPParams p(1, typeParam, value);
	fpInterface->Invoke(funcID, &p);
}

inline void setInterFacePropertyTBool(int typeParam, FPInterface* fpInterface,int funcID, bool value)
{
	FPParams p(1, typeParam, value);
	fpInterface->Invoke(funcID,&p);
}

inline void setInterFacePropertyTString(int typeParam, FPInterface* fpInterface, int funcID, std::string value)
{
	MCHAR* desVal = const_cast<wchar_t*>((s2ws(value)).c_str());
	FPParams p(1, typeParam, desVal);
	fpInterface->Invoke(funcID, &p);
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
	GetCOREInterface16()->LoadFromFile(s2ws(maxFile).data(),Interface8::LoadFromFileFlags::kSuppressPrompts&Interface8::LoadFromFileFlags::kUseFileUnits);
	FPValue result;
	fpInterface->Invoke(DESTFOLDERNAME_IBATCHPROOPTIMIZER_GETTER, result);
	const wchar_t* a = result.s;	
	logserver::LOG(a);
	//logserver::LOG(b);
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

inline void xref_low_error(std::wstring project_path, std::wstring pname) {

	Tab<const MCHAR*> sourceFile_Files;
	std::wstring filename = (project_path + L"\\" + pname + L"_low0.max");
	const wchar_t* file = filename.c_str();
	sourceFile_Files.Append(1, &file);
	//sourceFile_Files.Shrink();

	Tab<MSTR*> tstr_tab;
	MSTR str(filename.c_str());
	MSTR* a = &str;
	tstr_tab.Append(1, &a);
	//tstr_tab.Shrink();

	Tab<MSTR*> tstr_tab_dynamic_allocate_items;
	MSTR* str_dynamic_allocate_items = new MSTR(filename.c_str());
	tstr_tab_dynamic_allocate_items.Append(1, &str_dynamic_allocate_items);
	//tstr_tab_dynamic_allocate_items.Shrink();


	//FPValue param1;
	//int i = 10;
	////param1.LoadPtr(FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE, filename.data());	
	//param1.type = FILENAME_ADDXREFITEMSFROMFILE_IOBJXREFMGR_PARAM1_TYPE;
	//param1.s = filename.data();
	//param1.LoadPtr(ParamType2::TYPE_INT_BP, &i);	
	//LOG("param1.s here= "); LOG(param1.s); LOG("\n");

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
		//param3.InitTab(ParamType2::TYPE_STRING_TAB, sourceFile_Files.Count());
		//param3.LoadPtr(ParamType2::TYPE_STRING_TAB_BV, &sourceFile_Files);
		//param3.Load(ParamType2::TYPE_STRING_TAB_BV,&sourceFile_Files);
		for (int i = 0; i < sourceFile_Files.Count(); ++i) {
			std::string s = ws2s( sourceFile_Files[i] );
			LOG("Aha {0} {1}\n", i, s.c_str());
		}
		param3.LoadPtr(ParamType2::TYPE_STRING_TAB, &sourceFile_Files);

		for (int i = 0; i < sourceFile_Files.Count(); ++i) {
			std::string s = ws2s(sourceFile_Files[i]);
			LOG("BBB {0} {1}\n", i, s.c_str());
		}
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
	
//  	FPValue param4;
//  	param4.type = ParamType2::TYPE_FPVALUE_TAB_BV;
// 	Tab<FPValue*> param4_fpvalues;
// 	FPValue param4_fpvalue;
// 	FPValue* pointer_param4_fpvalue;
// 	param4_fpvalue.type = (ParamType2)1;
// 	param4_fpvalue.i = 10;
// 	param4_fpvalues.Append(1, &pointer_param4_fpvalue);
// 	param4.fpv_tab = &param4_fpvalues;
	
	FPValue param4;
	//param4.type = ParamType2::TYPE_INT_TAB;
	//param4.type = ParamType2::TYPE_INT_TAB_BV;
	//param4.type = ParamType2::TYPE_INT_TAB_BR;
  	Tab<int> xrefoptions;
	xrefoptions.Resize(1);
 	int h = 1;
 	xrefoptions.Append(1, &h);
	
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

inline void xref_low(std::string project_path, std::string pname) {
	FPInterface* fpInterface = GetCOREInterface(OBJXREFMGR_INTERFACE_ID);
	FPParams p(1, DUPOBJNAMEACTION_IOBJXREFMGR_TYPEPARAM, OBJXREFMGR_ENUM3::deleteOld);
	fpInterface->Invoke(DUPOBJNAMEACTION_IOBJXREFMGR_SETTER, &p);

	auto* ip = GetCOREInterface16();
	INodeTab selectedINodes;
	ip->GetSelNodeTab(selectedINodes);
	Tab<MSTR*> objNames;
	Tab<const MCHAR*> objNames_mchartype;
	auto numNodeSelection = selectedINodes.Count();
	objNames.Resize(numNodeSelection);
	objNames_mchartype.Resize(numNodeSelection);
	LOG("count is {}\n", numNodeSelection);
	for (int i = 0; i < numNodeSelection; i++)
	{
		auto inode = selectedINodes[i];
		//auto name = const_cast<wchar_t*>(inode->GetName());
		auto name = inode->NodeName();
		auto pname = &name;
		objNames.Append(1, &pname);
		//LOG(obj_names[i]->data());
		LOG(objNames[i]->data()); LOG("\n");
		auto name_wcharType = inode->GetName();
		objNames_mchartype.Append(1, &name_wcharType);
		LOG(objNames_mchartype[i]); LOG("\n");
	}
	
	//std::string filename = (project_path + "\\" + pname + "_low0.max");
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
		
		//LayerInterfaceExample();
		//y3d::IBatchProOptimizer y;
		//BatchProOptimizer(y);
		/*GetCOREInterface16()->load*/
		//std::string oFileDir = "F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene";
		std::wstring projectPath = L"F:\\WorkSpace\\3Ds Max\\Building Phong Tam Project\\scenes\\TestProOptimizerScene";
		std::wstring oFileName = L"001";
		xref_low_error(projectPath, oFileName);
		//pre_optimize(oFileDir, oFileName, projectPath);

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

Status YServiceTestImpl::MTest2(ServerContext* context, const EmptyParam* request, EmptyParam* reply)
{
	Invoke([]() {
		log("This is MTest 2\n");
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
