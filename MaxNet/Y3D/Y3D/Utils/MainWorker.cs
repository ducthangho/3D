using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using y3d.s;
using Grpc.Core;
using System.Windows.Forms;

namespace Y3D.Utils
{
    class MainWorker
    {
        public static YWorker worker = null;
        public static Channel ChannelLoader = null;
        public static Channel ChannelMax = null;

        static public y3d.s.YServiceMaxTools.YServiceMaxToolsClient MaxClient;
        static public y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient LoaderClient;
        static public y3d.s.YServiceTest.YServiceTestClient TestClient;

        static public bool getMainWorker()
        {
            if (worker == null)
            {
                var rep = rpc.YClient.MasterClient.GiveMeAWorkerAsync(new y3d.e.EmptyParam());
                rep.ResponseAsync.Wait();
                if (rep.ResponseAsync.IsCompleted)
                {
                    y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
                    wp.Wid = rep.ResponseAsync.Result.Worker.Wid;
                    var rr = rpc.YClient.MasterClient.StartWorker(wp);
                    //r.ResponseAsync.Wait();
                    //var rr = r.ResponseAsync.Result;
                    if (rr.Error)
                    {
                        MessageBox.Show("Can not start worker!");
                    } else
                    {
                        worker = rep.ResponseAsync.Result.Worker;
                        updateClient();
                    }
                    return true;

                } else
                {
                    MessageBox.Show("Can not get a worker from server!");
                }
            } 
            //if (worker.Status != y3d.e.ServingStatus.Serving)
            //{
            //    LoaderClient.LoadDll(new LibInfo());
            //    worker.Status = ServingStatus.Serving;
            //}
            return (worker != null);
        }
        
        static public void updateClient()
        {
            ChannelLoader = new Channel(worker.IpLoader, ChannelCredentials.Insecure);
            ChannelMax = new Channel(worker.IpMax, ChannelCredentials.Insecure);
            LoaderClient = new YServiceMaxLoader.YServiceMaxLoaderClient(ChannelLoader);
            MaxClient = new YServiceMaxTools.YServiceMaxToolsClient(ChannelMax);
            TestClient = new YServiceTest.YServiceTestClient(ChannelMax);
        } 

    }
}
