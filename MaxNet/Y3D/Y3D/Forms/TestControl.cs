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
            var x = Utils.MainWorker.getMainWorker().ContinueWith(
                (task) =>
                {
                    if (task.IsCompleted)
                        Utils.MainWorker.TestClient.MTest1(new y3d.e.EmptyParam());
                }
            );            
        }

        private void btnTest2_Click(object sender, EventArgs e)
        {
            var x = Utils.MainWorker.getMainWorker().ContinueWith(
                (task) =>
                {
                    if (task.IsCompleted)
                        Utils.MainWorker.TestClient.MTest2(new y3d.e.EmptyParam());
                }
            );
            
        }

        private void btnTest3_Click(object sender, EventArgs e)
        {
            y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
            wp.Ip = "127.0.0.1:38001";
            //var x = rpc.YClient.MasterClient.IsReady(wp);
            //MessageBox.Show(x.Status.ToString());
            var x = Utils.MainWorker.getMainWorker().ContinueWith<  Task<bool>  >(
                (task) =>
                {
                    if (!task.Result)
                    {
                        MessageBox.Show("Fail to connect");
                        return Task.FromResult(false);
                    }

                    var xx = rpc.YClient.MasterClient.IsReadyAsync(wp);
                    var rs = xx.ResponseAsync.ContinueWith<bool>((t) =>
                    {
                        if (t.IsCompleted)
                        {
                            MessageBox.Show(xx.ResponseAsync.Result.Status.ToString());
                            return true;
                        }
                        else MessageBox.Show("Fail to connect");
                        return false;
                    });

                    return rs;
                }
            ); 
            

            //rpc.YClient.test3();
        }

        private void btnTestBake3D_Click(object sender, EventArgs e)
        {
            //rpc.YClient.test_bake();
        }
    }
}
