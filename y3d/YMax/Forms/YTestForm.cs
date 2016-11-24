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
    }
}
