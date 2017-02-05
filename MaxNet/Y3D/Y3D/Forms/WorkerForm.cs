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

            Utils.Tools.YWList = rpc.YClient.CWClient.AllWorkers(new EmptyParam());
            Utils.Tools.YWList.Workers.Insert(0, Utils.Tools.YWList.Master);
            dlvWorker.SetObjects(Utils.Tools.YWList.Workers);
        }
    }
}
