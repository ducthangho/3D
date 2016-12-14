using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Autodesk.Max;
using System.Runtime.InteropServices;
using Utils;
using YMax.Utilities;
using System.Threading;

namespace YMax.Forms
{
    public partial class YTestForm : Form
    {
        private readonly Actions.Y3DTestActionItem yAction;
        public YTestForm(Actions.Y3DTestActionItem yAction)
        {
            InitializeComponent();
            this.yAction = yAction;
        }

        public YTestForm()
        {
            InitializeComponent();
        }

        private void btnCustom1_Click(object sender, EventArgs e)
        {
            Loader.Global.BroadcastNotification(SystemNotificationCode.Custom1);
        }

        private void btnCustom2_Click(object sender, EventArgs e)
        {
            Loader.Global.BroadcastNotification(SystemNotificationCode.FilePreSave);
        }

        private void YTestForm_Activated(object sender, EventArgs e)
        {
            Loader.Global.DisableAccelerators();
        }

        private void YTestForm_Deactivate(object sender, EventArgs e)
        {
            Loader.Global.EnableAccelerators();
        }

        private void btnThangTest_Click(object sender, EventArgs e)
        {
            //Utilities.YOList.activeArea.
            //IntPtr unmanagedPointer = Marshal.AllocHGlobal(Utilities.YOList.activeArea.CalculateSize());
            //Marshal.Copy(Utilities.YOList.activeArea., 0, unmanagedPointer, Utilities.YOList.activeArea.CalculateSize());.
            //int number = 1024;

            //unsafe
            //{
            //    //char* c;
            //    byte* p = (byte*)&number;

            //}
            Loader.Global.BroadcastNotification(SystemNotificationCode.Custom7);
        }

        private void btnTestD1_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(Loader.Core.CurFilePath);
            MessageBox.Show(Loader.Core.CurFilePath);
            MessageBox.Show(Loader.Core.CurFileName);
        }

        private void YTestForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.yAction.Close();
        }

        private void btnGrpcStop_Click(object sender, EventArgs e)
        {
            rpc.YServer.Stop();
        }

        private void btnGrpcStart_Click(object sender, EventArgs e)
        {
            rpc.YServer.Start();
        }

        private void btnGrpcClient_Click(object sender, EventArgs e)
        {
            rpc.YClient.test2();
        }

        private void btnTestMerge_Click(object sender, EventArgs e)
        {
            //var ff = "D:\\3D\\3dmax\\scenes\\1\\box3.max";
            var ff = "D:\\Down\\FShare\\Cafe sapa final - share by Vu Phuc Hung\\Cafe sapa final Share by Vu Phuc Hung\\cafe sapa final.max";

            var low_nametab = Loader.Global.NameTab.Create();
            //low_nametab.AddName("Box001");
            //low_nametab.Init();
            //low_nametab.SetSize(2);
            //low_nametab.AddName("Cone001");
            //low_nametab.AddName("Plane001");
            //low_nametab.AddName("Box001");
            //low_nametab.AddName("Torus001");
            //low_nametab.AddName("Box002");
            //low_nametab.AddName("Box007");
            //low_nametab.AddName("Box008");
            //low_nametab.AddName("Cylinder001");
            //low_nametab.AddName("Box009");
            //low_nametab.AddName("Tube001");
            Loader.Core.LoadFromFile(ff, false);
            //Loader.Core.MergeFromFile(ff, true, false, false, 4, low_nametab, 1, 1);

            //var s = "";
            //for (int i = 0; i < low_nametab.Count; i++)
            //{
            //    s += "," + low_nametab[i];
            //}
            //MessageBox.Show(s);
            //MessageBox.Show(low_nametab.Count.ToString());
            //low_nametab.SetSize(low_nametab.Count/3);
            //Loader.Core.MergeFromFile(ff, true, true, false, 3, low_nametab, 1, 1);


            //Loader.Core.MergeFromFile(System.IO.Path.Combine(YProject.oFileDir, YProject.oFileName + ".max"), true, true, false, 3, low_nametab, 1, 1);
            //MessageBox.Show(low_nametab.Count.ToString());
            //MessageBox.Show(low_nametab[1]);
        }
    }
}
