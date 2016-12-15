using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Autodesk.Max;
using System.Runtime.InteropServices;
using Y3D.Entities;
using Utils;
using System.Windows.Forms;

namespace YMax.Utilities
{
    class YProject
    {
        public static FaceRange face_range = new FaceRange();
        private static Forms.OManagerForm form;
        public static YSystem ysystem = null;
        public static string setting_path = Loader.Core.GetDir(9);
        public static string oFileName = "";
        public static string oFileDir = "";

        public static void clearOldProject()
        {
            YOList.resetData();
        }

        public static void openAnaForm(Actions.ObjectManagerItem yAction)
        {
            Forms.AnalyzeForm anaForm = new Forms.AnalyzeForm();
            if (anaForm.ShowDialog() == DialogResult.OK)
            {
                if (form == null)
                    form = new Forms.OManagerForm(yAction);
                form.Show();
                form.TopMost = true;
                form.BringToFront();
                form.WindowState = FormWindowState.Normal;

                YMax.Utilities.YOList.ObjectFromMax();
                form.updateTree();
            }
        }

        public static void readPInfo(Actions.ObjectManagerItem yAction)
        {
            if (Loader.Core.CurFileName.Length>0) // da load file max
            {
                MessageBox.Show("o la la"); // lam sau4

            } else
            {
                var openFileProject = new System.Windows.Forms.OpenFileDialog();
                //MessageBox.Show("Chon project tu danh sach project"); // lam sau4
                DialogResult result = openFileProject.ShowDialog();
                if (result == DialogResult.OK)
                {
                    oFileName = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
                    oFileDir =  System.IO.Path.GetDirectoryName(openFileProject.FileName);
                    //MessageBox.Show(file);
                    //ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("new_project_from_file \"" + file + "\"");
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("pre_optimize \"" + oFileDir + "\" \"" + oFileName + "\"");
                    openAnaForm(yAction);
                }
            }


        }

        public static void closeOForm()
        {
            if (form == null)
            {
                return;
            }
            //form.Close();
            form.Dispose();
            form = null;
        }

        public static void meomeo(IntPtr obj, INotifyInfo info)
        {
            return;
            //System.Windows.Forms.MessageBox.Show("chuan bi save");
            ////Loader.Core.PushPrompt("XIN CHAOOOOOOO");
            //INotifyInfo inf = Loader.Global.NotifyInfo.Marshal(info);
        

            //FileIOType x;
            //x = (FileIOType)inf.CallParam;
            ////x = (FileIOType) Marshal.PtrToStructure(inf.CallParam, typeof(FileIOType));
            ////if (x==FileIOType.Max)
            //{
            //    System.Windows.Forms.MessageBox.Show(x.ToString());
            //}
            ////var s = System.IO.Path.Combine(Loader.Core.CurFilePath, Loader.Core.CurFileName);
            ////System.Windows.Forms.MessageBox.Show(s);
            ////Loader.Core.SaveToFile("D:\\111.max",true,true);
            ////System.Windows.Forms.MessageBox.Show(ManagedServices.PathSDK.GetDirectoryPath(ManagedServices.AppSDK.);
        }

        public static void testHandle(IntPtr obj, INotifyInfo info)
        {
            // and use Interface.SelNodeCount + Interface.GetSelNode(i) here
            //INotifyInfo inf = Loader.Global.NotifyInfo.Marshal(info);
            string str = DateTime.Now.ToShortTimeString() + "Notification: ";
            str += info.CallParam + "  ..  ";
            str += (SystemNotificationCode)info.Intcode;

            //var DLL = Assembly.LoadFile(@"C:\Program Files\Autodesk\3ds Max 2015\bin\assemblies\ycpp.gup");
            System.Windows.Forms.MessageBox.Show("Sending ... : " + info.ToString());
            MyEventHandler handler = new MyEventHandler();
            handler.handle(obj, info.NativePointer);
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
        public static GlobalDelegates.Delegate5 fileOpenHandle = new GlobalDelegates.Delegate5(meomeo);
        public static GlobalDelegates.Delegate5 thang_test1 = new GlobalDelegates.Delegate5(testHandle);
        //public static GlobalDelegates.Delegate5 zun1 = new GlobalDelegates.Delegate5(zz1);

        public static Actions.YMaxCommand ycommand = new Actions.YMaxCommand();
    }
}
