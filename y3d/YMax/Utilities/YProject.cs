using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Autodesk.Max;
using System.Runtime.InteropServices;
using Y3D.Entities;
using Utils;


namespace YMax.Utilities
{
    class YProject
    {
        public static FaceRange face_range = new FaceRange();

        public static void meomeo(IntPtr obj, IntPtr info)
        {
            System.Windows.Forms.MessageBox.Show("chuan bi save");
            Loader.Core.PushPrompt("XIN CHAOOOOOOO");
        }

        public static void testHandle(IntPtr obj, IntPtr info)
        {
            // and use Interface.SelNodeCount + Interface.GetSelNode(i) here
            INotifyInfo inf = Loader.Global.NotifyInfo.Marshal(info);
            string str = DateTime.Now.ToShortTimeString() + "Notification: ";
            str += inf.CallParam + "  ..  ";
            str += (SystemNotificationCode)inf.Intcode;

            System.Windows.Forms.MessageBox.Show("Sending ... : " + info.ToInt64());

            MyEventHandler handler = new MyEventHandler();
            handler.handle(obj, info);

            //Loader.Core.PushPrompt("Hello");
        }
        public static void loadPSetting()
        {

        }
        /*public static void zz1(IntPtr obj, IntPtr info)
        {
            var contructor = typeof(YArea).GetConstructors()[0];

            INotifyInfo inf = Loader.Global.NotifyInfo.Marshal(info);
            //Y3D.Entities.YArea ya;
            //ya = (YArea)contructor.Invoke(new object[] { inf.GetNativeHandle(), false });
            //ya = (YArea)inf.CallParam;
            //ya = (Y3D.Entities.YArea)Marshal.PtrToStructure((IntPtr)inf.CallParam, typeof(Y3D.Entities.YArea));
            //Y3D.Entities.YArea ya;
            //try
            //{
            //    ya = (Y3D.Entities.YArea)Marshal.PtrToStructure(info, typeof(Y3D.Entities.YArea));
            //    System.Windows.Forms.MessageBox.Show(ya.Name);
            //}
            //finally
            //{
            //    Marshal.FreeHGlobal(info);
            //}
            //System.Windows.Forms.MessageBox.Show("oe oeo oe oe1");
            var b = inf.CallParam.GetType();
            System.Windows.Forms.MessageBox.Show(b.ToString());
        }//*/
        public static GlobalDelegates.Delegate5 fileSaveHandler = new GlobalDelegates.Delegate5(meomeo);
        public static GlobalDelegates.Delegate5 thang_test1 = new GlobalDelegates.Delegate5(testHandle);
        //public static GlobalDelegates.Delegate5 zun1 = new GlobalDelegates.Delegate5(zz1);

        public static Actions.YMaxCommand ycommand = new Actions.YMaxCommand();
    }
}
