using Grpc.Core;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using y3d.e;
namespace Y3D.Forms
{
    public partial class WorkerForm : Form
    {
        public WorkerForm()
        {
            InitializeComponent();
        }

        private void btnNew_Click(object sender, EventArgs e)
        {

        }

        private void WorkerForm_Load(object sender, EventArgs e)
        {
            this.olvColumnStatus.ImageGetter = delegate (object x) {
                if (x is YWorker)
                {
                    YWorker yw = (YWorker)x;
                    if (yw.Status == ServingStatus.NotServing)
                        return "c_red";
                    if (yw.Status == ServingStatus.Serving)
                        return "c_green";
                }
                return "c_yellow";
            };

            AllWorkerParam req = new AllWorkerParam();
            req.Status = 2;
            var rep = rpc.YClient.MasterClient.AllWorkers(req);
            if (!rep.Error)
                dlvWorker.SetObjects(rep.Wlist.Workers);
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;            
            WorkerParam req = new WorkerParam();
            req.Wid = yw.Wid;
            //req.Wname = yw.Wname;
            var rep = rpc.YClient.MasterClient.StartWorker(req);
            if (!rep.Error)
            {
                dlvWorker.SetObjects(rep.Wlist.Workers);
                MessageBox.Show("Start thanh cong..");
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;
            WorkerParam wp = new WorkerParam();
            wp.Wid = yw.Wid;
            var rep = rpc.YClient.MasterClient.StopWorker(wp);
            if (!rep.Error)
            {
                dlvWorker.SetObjects(rep.Wlist.Workers);
                MessageBox.Show("Stop thanh cong..");
            }
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            //Channel channel = new Channel("127.0.0.1:39001", ChannelCredentials.Insecure);
            //var toolClient = new y3d.s.Tools.ToolsClient(channel);

            //var re = toolClient.CloneObject(new EmptyParam());
            //MessageBox.Show(re.Message);

            //MessageBox.Show(re.Message);
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;
            WorkerParam wp = new WorkerParam();
            wp.Wid = yw.Wid;
            //MessageBox.Show(yw.ProcessId.ToString());
            var rep = rpc.YClient.MasterClient.CloseWorkerApp(wp);
        }
    }
}
