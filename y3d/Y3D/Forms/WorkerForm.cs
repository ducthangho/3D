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
using LogClientCSharp;

namespace Y3D.Forms
{
    public partial class WorkerForm : Form
    {
        public static LogClient log = LogClient.Instance;
        public WorkerForm()
        {
            InitializeComponent();
        }

        public void updateWorkerList(YWorkerResponse x)
        {
            dlvWorker.SetObjects(x.Wlist.Workers);
            btnStart.Enabled = false;
            btnStop.Enabled = false;
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


            this.olvColumnAddr.AspectGetter = delegate (object x)
            {
                if (x is YWorker)
                {
                    YWorker yw = (YWorker)x;
                    return yw.MachineIp + ":" + yw.PortLoader;
                }
                return "...";
            };
            AllWorkerParam req = new AllWorkerParam();
            req.Status = 2;
            req.Refresh = true;
            req.Machine = Y3D.Users.Auth.ymachine;

            //var rep = Y3D.Projects.Utils.MasterClient.AllWorkers(req);
            //if (!rep.Error)
            //{
            //    dlvWorker.SetObjects(rep.Wlist.Workers);
            //}


            var xx = Y3D.Projects.Utils.MasterClient.AllWorkersAsync(req);
            var rs = xx.ResponseAsync.ContinueWith((t) =>
            {
                if (!t.IsFaulted && !t.IsCanceled) 
                {
                    dlvWorker.SetObjects(t.Result.Wlist.Workers);
                }
            });
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (!Projects.Utils.checkMaster()) return;
            log.LOG("Button start clicked\n.");
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;
            if (yw == null)//No object selected
            {
                if (dlvWorker.GetItemCount() == 0) return;
                yw = (YWorker)dlvWorker.GetModelObject(0);
                dlvWorker.SelectObject(yw, true);
            }
            this.btnStart.Enabled = false;
            if (yw.Status == ServingStatus.Serving) return;

            WorkerParam req = new WorkerParam();
            req.Wid = yw.Wid;
            //req.Wname = yw.Wname;
            var rep = Y3D.Projects.Utils.MasterClient.StartWorkerAsync(req);
            rep.ResponseAsync.ContinueWith(
                (t) =>
                {
                    if (t.IsCanceled || t.IsFaulted) { }
                        return;

                    var rs = t.Result;
                    if (!rs.Error && rs.Wlist!=null && rs.Wlist.Workers!=null)
                    {
                        dlvWorker.SetObjects(rs.Wlist.Workers);
                        //MessageBox.Show("Start thanh cong..");
                        return;
                    }
                    btnStart.Enabled = true;
                }
            );            
        }

        private void selectWorkerID(Int32 id)
        {
            for (int i=0;i<dlvWorker.GetItemCount();++i)
            {
                var yw = (YWorker)dlvWorker.GetModelObject(i);
                if (yw.Wid == id)
                {
                    dlvWorker.Focus();
                    dlvWorker.EnsureModelVisible(yw);
                    dlvWorker.SelectObject(yw);
                    dlvWorker.HideSelection = false;
                    return;
                }
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (!Projects.Utils.checkMaster()) return;
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;
            if (yw == null)//No object selected
            {
                if (dlvWorker.GetItemCount() == 0) return;                
                yw = (YWorker)dlvWorker.GetModelObject(dlvWorker.GetItemCount() - 1);
                dlvWorker.SelectObject(yw, true);
            }
            btnStop.Enabled = false;
            if (yw.Status == ServingStatus.NotServing) return;

            WorkerParam wp = new WorkerParam();
            wp.Wid = yw.Wid;
            var rep = Y3D.Projects.Utils.MasterClient.StopWorkerAsync(wp);

            rep.ResponseAsync.ContinueWith(
                (t) =>
                {
                    if (t.IsCanceled || t.IsFaulted)
                        return;

                    var rs = t.Result;
                    if (!rs.Error)
                    {                        
                        dlvWorker.SetObjects(rs.Wlist.Workers);
                        selectWorkerID(rs.Wid);
                        return;
                        //MessageBox.Show("Stop thanh cong..");
                    }
                    btnStop.Enabled = true;
                }
            );          
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if (!Projects.Utils.checkMaster()) return;
            //Channel channel = new Channel("127.0.0.1:39001", ChannelCredentials.Insecure);
            //var toolClient = new y3d.s.Tools.ToolsClient(channel);

            //var re = toolClient.CloneObject(new EmptyParam());
            //MessageBox.Show(re.Message);

            //MessageBox.Show(re.Message);
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;
            WorkerParam wp = new WorkerParam();
            wp.Wid = yw.Wid;
            //MessageBox.Show(yw.ProcessId.ToString());
            var rep = Y3D.Projects.Utils.MasterClient.CloseWorkerApp(wp);
        }

        private void dlvWorker_Click(object sender, EventArgs e)
        {
            YWorker yw = (YWorker)this.dlvWorker.SelectedObject;
            if (yw == null)
            {
                btnStart.Enabled = false;
                btnStop.Enabled = false;
                return;
            }
            if (yw.Status==ServingStatus.Serving)
            {
                btnStart.Enabled = false;
                btnStop.Enabled = true;
            } else if (yw.Status == ServingStatus.NotServing)
            {
                btnStart.Enabled = true;
                btnStop.Enabled = false;
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
