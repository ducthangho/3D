// Utils.h

#pragma once
#include <msclr\marshal_cppstd.h>
#using<System.Windows.Forms.dll> 
using namespace System::Windows::Forms;
using namespace System;
using namespace Autodesk::Max;


namespace Utils {
	
	public ref class MyEventHandler
	{
	private:
		//YourClass* ptr;

	public:
	
		MyEventHandler()
		{
			//ptr = new YourClass();
		}

		~MyEventHandler()
		{
			this->!MyEventHandler();
		}

		!MyEventHandler()
		{
			//delete ptr;
			//ptr = NULL;
		}

		

		void handle(IntPtr obj, IntPtr info)
		{
			System::String^ str = "Notification: ";
			//INotifyInfo inf = Global.NotifyInfo.Marshal(info);
			//str += inf.CallParam + "  ..  ";
			//str += (SystemNotificationCode)inf.Intcode;
			//std::string unmanaged_str = msclr::interop::marshal_as<std::string>(str);
			MessageBox::Show("Code received = "+info.ToInt64());			
			//ptr->SomeMethod();
		}
	};
}
