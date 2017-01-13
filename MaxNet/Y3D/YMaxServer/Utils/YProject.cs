using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Autodesk.Max;
using System.Runtime.InteropServices;
using y3d.e;
using System.Windows.Forms;
using System.IO;
using Google.Protobuf;

namespace YMaxServer.Utils
{
    class YProject
    {
        //public static FaceRange face_range = new FaceRange();
        //public static Forms.OManagerForm form;
        //public static YSystem ysystem = null;
        public static string setting_path = Loader.Core.GetDir(9);
        public static string oFileName = "";
        public static string oFileDir = "";
        public static YSystem sysSetting = null;

        public static void resetSystem()
        {
            var path = System.IO.Path.Combine(setting_path, "y3d");
            if (sysSetting is YSystem)
            {
                sysSetting.Projects.Clear();
                using (Stream output = File.OpenWrite(path))
                {
                    sysSetting.WriteTo(output);
                }
            }
        }
        public static void initSystem()
        {
            var path = System.IO.Path.Combine(setting_path, "y3d");
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
            path = Path.Combine(path, "ysettings.y3d");
            if (!File.Exists(path))
            {
                sysSetting = new YSystem();
                sysSetting.DefaultInfo = new ProjectInfo();
                //sysSetting.DefaultInfo.Alist.Areas.Add(Utilities.YOList.GetYAreas());
                sysSetting.DefaultSetting = new PSetting();
                sysSetting.DefaultSetting.MaxRecent = 5;
                using (Stream output = File.OpenWrite(path))
                {
                    sysSetting.WriteTo(output);
                }
                sysSetting.Projects.Clear();
            }
            else
            {
                using (Stream file = File.OpenRead(path))
                {
                    var a = Google.Protobuf.CodedInputStream.CreateWithLimits(file, 1024 << 20, 10);
                    sysSetting = YSystem.Parser.ParseFrom(a);
                }
            }
        }

        public static void newProject()
        {
            MessageBox.Show("XIn chaozzz");
        }

        public static void saveProject(string pname)
        {
            var path = Loader.Core.CurFilePath;
            Google.Protobuf.WellKnownTypes.Timestamp t = new Google.Protobuf.WellKnownTypes.Timestamp();
            var ticks = DateTime.Now.Ticks;
            //pp.Ts.Seconds = (long)((ticks / 10000000) - 11644473600LL);
            t.Seconds = DateTime.Now.Second;
            t.Nanos = (int)((ticks % 10000000) * 100);

            foreach (var pp in sysSetting.Projects)
            {
                if (pp.Path == path)
                {
                    // override
                    pp.Ts.Seconds = t.Seconds;
                    pp.Ts.Nanos = t.Nanos;
                    return;
                }
            }
            ProjectInfo p = new ProjectInfo();
            p.Pname = pname;
            p.Path = path;
            p.Ts = t;
            sysSetting.Projects.Add(p);
            using (Stream output = File.OpenWrite(System.IO.Path.Combine(setting_path, "y3d")))
            {
                sysSetting.WriteTo(output);
            }


        }

        //public static void openAnaForm(Actions.ObjectManagerItem yAction)
        //{
        //    Forms.AnalyzeForm anaForm = new Forms.AnalyzeForm();
        //    if (anaForm.ShowDialog() == DialogResult.OK)
        //    {
        //        if (form == null)
        //            form = new Forms.OManagerForm(yAction);
        //        form.Show();
        //        form.TopMost = true;
        //        form.BringToFront();
        //        form.WindowState = FormWindowState.Normal;

        //        YMax.Utilities.YOList.ObjectFromMax();
        //        form.updateTree();
        //    }
        //}

        //public static void readPInfo(Actions.ObjectManagerItem yAction)
        //{
        //    if (Loader.Core.CurFileName.Length>0) // da load file max
        //    {
        //        MessageBox.Show("o la la"); // lam sau4

        //    } else
        //    {
        //        var openFileProject = new System.Windows.Forms.OpenFileDialog();
        //        //MessageBox.Show("Chon project tu danh sach project"); // lam sau4
        //        DialogResult result = openFileProject.ShowDialog();
        //        if (result == DialogResult.OK)
        //        {
        //            oFileName = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
        //            oFileDir =  System.IO.Path.GetDirectoryName(openFileProject.FileName);
        //            //MessageBox.Show(file);
        //            //ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("new_project_from_file \"" + file + "\"");
        //            ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.pre_optimize \"" + oFileDir + "\" \"" + oFileName + "\"");
        //            openAnaForm(yAction);
        //        }
        //    }
        //}

        //public static void closeOForm()
        //{
        //    if (form == null)
        //    {
        //        return;
        //    }
        //    //form.Close();
        //    form.Dispose();
        //    form = null;
        //}

        //public static void closeAllForm()
        //{
        //    if (form == null)
        //    {
        //        return;
        //    }
        //    form.Close();
        //    //form.Dispose();
        //    form = null;
        //}

        public static void meomeo(IntPtr obj, INotifyInfo info)
        {
            return;
            //System.Windows.Forms.MessageBox.Show("chuan bi save");
        }
  
        public static GlobalDelegates.Delegate5 fileOpenHandle = new GlobalDelegates.Delegate5(meomeo);
        public static Actions.YMaxCommand ycommand = new Actions.YMaxCommand();
    }
}
