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

void toUpper(MSTR a) {

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
		logserver::LOG("#define I{0}_ID  Interface_ID(0x{1},0x{2})\n",internal_name, hexa, hexb);
	}
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
		LOG("#define IFN_{0}_I{1} {2}\n", func_internalname,internal_name,func_id);
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
		LOG("#define IPROPS_GET_{0}_I{1} {2}\n", prop_internalname, internal_name,func_getterid);
		LOG("#define IPROPS_SET_{0}_I{1} {2}\n", prop_internalname, internal_name, fucn_setterid);

		log(" + properties number {0} have internal name is {1},"
			" setterid is {2}, getterid is {3}\n", i, prop_internalname, fucn_setterid, func_getterid);
	}
}

void BatchOptimizer(y3d::IBatchProOptimizer ibatchProOptimizer)
{
	FPInterface* fpInterface = GetCOREInterface(IBATCHPROOPTIMIZER_ID);
	FPParams pSourceFileMode(1, TYPE_INT, 0);
	FPValue result;
	fpInterface->Invoke(IPROPS_SET_SOURCEFILEMODE_IBATCHPROOPTIMIZER, &pSourceFileMode);

	fpInterface->Invoke(IPROPS_GET_SOURCEFILEMODE_IBATCHPROOPTIMIZER, result);
	log("SourFileMode is {0}\n", result.i);
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
		//generateInterfaceFuntionsID(IBATCHPROOPTIMIZER_ID);
		y3d::IBatchProOptimizer y;
		BatchOptimizer(y);

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
void YServiceTestImpl::Initialize(void* codegen, void* gli)
{

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
