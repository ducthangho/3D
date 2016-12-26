// Utils.h

#pragma once

#include <msclr\marshal_cppstd.h>
#using<System.Windows.Forms.dll> 
#include "maxscript/maxscript.h"
using namespace System::Windows::Forms;
using namespace System;
//using namespace Autodesk::Max;


namespace Utils {

	class A {
	private:
		int x;
	public:
		A(int b) : x(b) {};
		void hello() {
			mprintf(L"Hello fuck %d\n", x);
			auto ip = GetCOREInterface();
			auto str = ip->GetCurFileName();
			mprintf(L"File name = %s\n", str);
		}
	};

	public ref class MyEventHandler
	{
	private:
		A* a;
		//YourClass* ptr;

	public:

		MyEventHandler()
		{
			a = new A(123);
			//ptr = new YourClass();
		}

		~MyEventHandler()
		{
			this->!MyEventHandler();
		}

		!MyEventHandler()
		{
			delete a;
			a = NULL;
		}



		void handle(IntPtr obj, IntPtr info)
		{
			System::String^ str = "Notification: ";
			//INotifyInfo inf = Global.NotifyInfo.Marshal(info);
			//str += inf.CallParam + "  ..  ";
			//str += (SystemNotificationCode)inf.Intcode;
			//std::string unmanaged_str = msclr::interop::marshal_as<std::string>(str);
			//MessageBox::Show("Code received = "+info.ToInt64());
			//auto ip = Autodesk::Max::GlobalInterface::Instance->COREInterface;
			//MessageBox::Show("File name is " + ip->CurFileName);

			a->hello();
			//ptr->SomeMethod();
		}
	};
}
