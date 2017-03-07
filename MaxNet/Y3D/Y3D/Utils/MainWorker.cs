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

        static public Task<bool> getMainWorker()
        {
            if (worker == null)
            {
                var rep = rpc.YClient.MasterClient.GiveMeAWorkerAsync(new y3d.e.EmptyParam());
                var x = rep.ResponseAsync.ContinueWith<Task<bool>>(
                    (task) =>
                    {
                        if (task.IsFaulted || task.IsCanceled)
                        {
                            MessageBox.Show("Can not get a worker from server!");
                            return Task.FromResult(false);
                        } else

                        if (task.IsCompleted)
                        {
                            y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
                            wp.Wid = task.Result.Worker.Wid;
                            var rr = rpc.YClient.MasterClient.StartWorkerAsync(wp).ResponseAsync;
                            //r.ResponseAsync.Wait();
                            //var rr = r.ResponseAsync.Result;
                            return rr.ContinueWith<bool>((wk) => {
                                if (wk.IsFaulted || wk.IsCanceled || wk.Result.Error)
                                {
                                    MessageBox.Show("Can not start worker!");
                                    return false;
                                }

                                if (wk.IsCompleted)
                                {
                                    worker = wk.Result.Worker;
                                    updateClient();
                                }

                                return true;
                            });
                        }

                        return Task.FromResult(false);
                    });
                //rep.ResponseAsync.Wait();
                return x.Unwrap();

            } else
            {
                y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
                wp.Wid = worker.Wid;
                wp.Ip = worker.MachineIp+":"+ worker.PortLoader;
                var x = rpc.YClient.MasterClient.IsReadyAsync(wp);
                var rs = x.ResponseAsync.ContinueWith<bool>((task) =>
                {
                    if (task.IsFaulted || task.IsCanceled)
                    {
                        worker = null;
                        rpc.YClient.MasterClient.StopWorker(wp); //remove worker from list, could be wrong. Xem Ky cho nayf!!!!!!!!!!!!!!!!!!!!!!!
                        return false;
                    }                    
                    return true;
                });
            }
            //if (worker.Status != y3d.e.ServingStatus.Serving)
            //{
            //    LoaderClient.LoadDll(new LibInfo());
            //    worker.Status = ServingStatus.Serving;
            //}
            return Task.FromResult(false);
        }
        
        static public void detach_mainworker()
        {
            if (worker!=null)
            {
                worker.Wtype = YWorker.Types.WorkerType.Free;
                rpc.YClient.MasterClient.UpdateWorkerAsync(worker);
            }
        }

        static public void updateClient()
        {
            ChannelLoader = new Channel(worker.MachineIp+":"+worker.PortLoader, ChannelCredentials.Insecure);
            ChannelMax = new Channel(worker.MachineIp + ":" + worker.PortMax, ChannelCredentials.Insecure);
            LoaderClient = new YServiceMaxLoader.YServiceMaxLoaderClient(ChannelLoader);
            MaxClient = new YServiceMaxTools.YServiceMaxToolsClient(ChannelMax);
            TestClient = new YServiceTest.YServiceTestClient(ChannelMax);
        } 



    }
}
