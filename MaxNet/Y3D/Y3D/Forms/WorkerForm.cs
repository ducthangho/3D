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
                    if (yw.Status == YWorker.Types.ServingStatus.NotServing)
                        return "c_gray";
                    if (yw.Status == YWorker.Types.ServingStatus.Serving)
                        return "c_green";
                }
                return "c_yellow";
            };

            AllWorkerParam req = new AllWorkerParam();
            req.Status = 2;
            var allWorkers = rpc.YClient.MasterClient.AllWorkers(req);
            if (allWorkers.Workers.Count > 0)
                dlvWorker.SetObjects(allWorkers.Workers);
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;
            StartWorkerParam req = new StartWorkerParam();
            req.Wid = yw.Wid;
            //req.Wname = yw.Wname;
            var rep = rpc.YClient.MasterClient.StartWorker(req);
            if (!rep.Error)
            {
                MessageBox.Show("Start thanh cong..");
                yw.Status = y3d.e.YWorker.Types.ServingStatus.Serving;
                //AllWorkerParam rr = new AllWorkerParam();
                //rr.Status = 2;
                //var allWorkers = rpc.YClient.MasterClient.AllWorkers(rr);
                //if (allWorkers.Workers.Count > 0)
                //    dlvWorker.SetObjects(allWorkers.Workers);
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {

        }
    }
}
