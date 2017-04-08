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

namespace Y3D.Tests
{
    public partial class TestControl : UserControl
    {
        public TestControl()
        {
            InitializeComponent();
        }

        private void btnTest1_Click(object sender, EventArgs e)
        {
            var x = Y3D.Projects.Utils.getMainWorker().ContinueWith(
                (task) =>
                {
                    if (!task.IsFaulted && !task.IsCanceled)
                        Y3D.Projects.Utils.TestClient.MTest1(new y3d.e.EmptyParam());
                }
            );
        }
        private void btnTest2_Click(object sender, EventArgs e)
        {
            var x = Y3D.Projects.Utils.getMainWorker().ContinueWith(
                (task) =>
                {
                    if (!task.IsFaulted && !task.IsCanceled)
                        Y3D.Projects.Utils.TestClient.MTest2(new y3d.e.EmptyParam());
                }
            );
            
        }

        private void btnTest3_Click(object sender, EventArgs e)
        {
            //y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
            //wp.Ip = "127.0.0.1:38001";
            ////var x = Y3D.Projects.Utils.MasterClient.IsReady(wp);
            ////MessageBox.Show(x.Status.ToString());
            //var x = Y3D.Projects.Utils.MasterClient.IsReadyAsync(wp);            
            //var rs = x.ResponseAsync.ContinueWith<bool>( (task) =>
            //{
            //    if (task.IsFaulted || task.IsCanceled)
            //    {
            //        MessageBox.Show("Fail to connect");
            //        return false;
            //    } else if (task.IsCompleted && task.Result != null && task.Result.Status == y3d.e.ServingStatus.Serving)
            //        MessageBox.Show(x.ResponseAsync.Result.Status.ToString());
            //    return true;    
            //});
            //rs.Wait();

            //rpc.YClient.test3();
        }

        private void btnTestBake3D_Click(object sender, EventArgs e)
        {
            //rpc.YClient.test_bake();
        }
    }
}
