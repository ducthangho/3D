using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Y3D.Forms
{
    public partial class ProjectControl : UserControl
    {
        public ProjectControl()
        {
            InitializeComponent();
        }

        public async void giveworker()
        {
            var rep = rpc.YClient.MasterClient.GiveMeAWorkerAsync(new y3d.e.EmptyParam());
            await rep;
            //Console.WriteLine(String.Format("name: {0}", rep.ResponseAsync.Result.Worker.Wname));
            MessageBox.Show(rep.ResponseAsync.Result.Worker.Wname);
        }

        private void btnNewFromMax_Click(object sender, EventArgs e)
        {
            var x = Utils.MainWorker.getMainWorker();
            x.ContinueWith(
                (task) =>
                {
                    if (task.Result)
                        MessageBox.Show(Utils.MainWorker.worker.Wname);
                    else MessageBox.Show("Cannot start.");
                }
            );
            //if (Utils.MainWorker.worker!=null)
            //{
            //    MessageBox.Show(Utils.MainWorker.worker.Wname);
            //    //Utils.MainWorker.MaxClient.GetObjectFromMax(new y3d.e.EmptyParam());
            //}
            //if (rpc.YMasterServer.CurrentWorker == null)
            //{
            //    // start main worker 3dmax
            //    if (!rpc.YMasterServer.CheckCurrentWorker())
            //    {
            //        MessageBox.Show("Something wrong! Can not find a worker!");
            //        return;
            //    }
            //}
            //rpc.YClient.NewProject();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void btnNewBlank_Click(object sender, EventArgs e)
        {

        }

        private void btnDelP_Click(object sender, EventArgs e)
        {

        }

        private void btnEditP_Click(object sender, EventArgs e)
        {

        }

        private void btnLoadP_Click(object sender, EventArgs e)
        {

        }

        private void dlvListProject_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
