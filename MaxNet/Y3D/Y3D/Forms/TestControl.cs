using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Grpc.Core;

namespace Y3D.Forms
{
    public partial class TestControl : UserControl
    {
        public TestControl()
        {
            InitializeComponent();
        }

        private void btnTest1_Click(object sender, EventArgs e)
        {
            if (Utils.MainWorker.getMainWorker())
            {
                Utils.MainWorker.TestClient.MTest1(new y3d.e.EmptyParam());
            }

        }

        private void btnTest2_Click(object sender, EventArgs e)
        {
            if (Utils.MainWorker.getMainWorker())
            {
                Utils.MainWorker.TestClient.MTest2(new y3d.e.EmptyParam());
            }
        }

        private void btnTest3_Click(object sender, EventArgs e)
        {
            y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
            wp.Ip = "127.0.0.1:38001";
            //var x = rpc.YClient.MasterClient.IsReady(wp);
            //MessageBox.Show(x.Status.ToString());
            var x = rpc.YClient.MasterClient.IsReadyAsync(wp);
            x.ResponseAsync.Wait();
            if (x.ResponseAsync.IsCompleted)
            {
                MessageBox.Show(x.ResponseAsync.Result.Status.ToString());
            }

            //rpc.YClient.test3();
        }

        private void btnTestBake3D_Click(object sender, EventArgs e)
        {
            //rpc.YClient.test_bake();
        }
    }
}
