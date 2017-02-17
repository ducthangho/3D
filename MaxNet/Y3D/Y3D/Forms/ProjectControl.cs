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
            //Console.WriteLine("ddd");
            //MessageBox.Show(rep.ResponseAsync.Result.Worker.Wname);
        }

        private void btnNewFromMax_Click(object sender, EventArgs e)
        {
            if (Utils.MainWorker.worker == null)
            {
                giveworker();
                //var rep = rpc.YClient.MasterClient.GiveMeAWorkerAsync(new y3d.e.EmptyParam());
                //await rep;
                //MessageBox.Show(rep.ResponseAsync.Result.Worker.Wname);

                //var ret = rpc.YClient.MasterClient.GiveMeAWorker(new y3d.e.EmptyParam());
                //rep.ResponseAsync.Wait();
                //while (!rep.ResponseAsync.IsCompleted)
                //{
                //    System.Threading.Thread.Sleep(2000);
                //}
                //var ret = rep.ResponseAsync.Result;
                //MessageBox.Show(ret.Worker.Wname);
                //if (ret != null)
                //{
                //    Utils.MainWorker.worker = ret.Worker;
                //    MessageBox.Show(ret.Worker.Wname);
                //    //Utils.MainWorker.updateClient();
                //    //Utils.MainWorker.LoaderClient.LoadDll(new y3d.s.LibInfo());
                //}

                //while (rep==null)
                //{
                //    System.Threading.Thread.Sleep(2000);
                //}
                //if (rep!=null)
                //{
                //    var ret = rep.ResponseAsync.Result;
                //    MessageBox.Show(ret.Worker.Wname);
                //}
                //rep.ResponseAsync.Wait();
                //if (rep.GetStatus().StatusCode == Grpc.Core.StatusCode.OK)
                //{
                //    var ret = rep.ResponseAsync.Result;
                //    MessageBox.Show(ret.Worker.Wname);
                //}

                //if (ret!=null)
                //{
                //    Utils.MainWorker.worker = ret.Worker;
                //    MessageBox.Show(ret.Worker.Wname); 
                //    Utils.MainWorker.updateClient();
                //    Utils.MainWorker.LoaderClient.LoadDll(new y3d.s.LibInfo());
                //}

            }
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
