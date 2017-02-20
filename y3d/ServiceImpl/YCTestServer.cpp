#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotocd.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")
#endif

#include "YCTestServer.h"
#include "common.h"
#include "YLibs.h"


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

Status YServiceTestImpl::MTest1(ServerContext* context, const EmptyParam* request, EmptyParam* reply)
{
	Invoke([]() {
		//MakeBentCylinder();
		//addbend();
		addUnwrap();
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
	});
	return Status::OK;
}


Status YServiceTestImpl::MTest2(ServerContext* context, const EmptyParam* request, EmptyParam* reply)
{
	Invoke([]() {
		log("This is MTest 2 zzz\n");
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
