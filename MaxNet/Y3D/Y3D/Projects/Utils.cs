using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using y3d.s;
using Grpc.Core;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace Y3D.Projects
{
    class Utils
    {
        public static YWorker worker = null;
        static Channel ChannelLoader = null;
        static Channel ChannelMax = null;

        public static YWorker mworker = null;

        static public y3d.s.YServiceMaxTools.YServiceMaxToolsClient MaxClient;
        static public y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient LoaderClient;
        static public y3d.s.YServiceTest.YServiceTestClient TestClient;

        public static string master_ip = "127.0.0.1";
        public static y3d.s.YServiceMaster.YServiceMasterClient MasterClient;

        public static YAreaList CurrentYAL = null;
        public static YSystem YSys = new YSystem();

        public static void initSystem()
        {
            MasterClient = new y3d.s.YServiceMaster.YServiceMasterClient(new Channel(master_ip+ ":38000", ChannelCredentials.Insecure));
        }

        public static bool testConnection(String ip, int port)
        {
            var channel = new Channel(String.Format(ip + ":{0}", port), ChannelCredentials.Insecure);
            return (!channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(3000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result);
        }

        public static bool testConnection(String ip, int wid, int range = 38000)
        {
            var channel = new Channel(String.Format(ip + ":{0}", wid + range), ChannelCredentials.Insecure);
            return (!channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(3000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result);
        }

        static public void updateClient()
        {
            //var ip = (yw.MachineIp == YMServer.MASTER_IP_LAN) ? "127.0.0.1" : yw.MachineIp;
            var ip = "127.0.0.1";
            ChannelLoader = new Channel(ip + ":" + worker.PortLoader, ChannelCredentials.Insecure);
            ChannelMax = new Channel(ip + ":" + worker.PortMax, ChannelCredentials.Insecure);
            LoaderClient = new YServiceMaxLoader.YServiceMaxLoaderClient(ChannelLoader);
            MaxClient = new YServiceMaxTools.YServiceMaxToolsClient(ChannelMax);
            TestClient = new YServiceTest.YServiceTestClient(ChannelMax);
        }

        public static bool IsProcessOpen(string name)
        {
            foreach (Process clsProcess in Process.GetProcesses())
            {
                if (clsProcess.ProcessName.Contains(name))
                {
                    return true;
                }
            }
            return false;
        }

        static public Task<bool> getMainWorker()
        {
            if (worker == null)
            {
                y3d.e.YWorkerRequest req = new YWorkerRequest();
                req.Machine = Y3D.Users.Auth.ymachine;
                req.Wtype = WorkerType.MainWorker;

                var rep = MasterClient.GiveMeAWorkerAsync(req);
                var x = rep.ResponseAsync.ContinueWith<Task<bool>>(
                    (task) =>
                    {
                        if (task.IsFaulted || task.IsCanceled)
                        {
                            MessageBox.Show("Can not get a worker from server!");
                            return Task.FromResult(false);
                        }
                        else

                        if (task.IsCompleted)
                        {
                            var w = task.Result.Worker;
                            if (w.NetState == 4)
                            {
                                if (Y3D.Users.Auth.StartApp("MAX3D"))
                                {
                                    var channel = new Channel(String.Format("127.0.0.1:{0}", w.PortLoader), ChannelCredentials.Insecure);

                                    if (channel.State != Grpc.Core.ChannelState.Ready)
                                    {
                                        Task.Factory.StartNew(
                                            () =>
                                            {
                                                int count = 0;
                                                while (channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(5000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result && count++ < 20)
                                                {
                                                    Console.WriteLine("5s. Retry"); // hien loading
                                                    if (count == 30) break;
                                                }//*/                   
                                                if (count < 30)
                                                {
                                                    Console.WriteLine("Connected!!!");
                                                }
                                            }
                                        ).ContinueWith(_ =>
                                        {
                                            if (_.IsFaulted || _.IsCanceled)
                                            {
                                                MessageBox.Show(String.Format("Can not connect to {0}:{1}", w.MachineIp, w.PortLoader));
                                                return Task.FromResult(false);
                                            }
                                            // start worker
                                            y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
                                            wp.Wid = w.Wid;
                                            var rr = Y3D.Projects.Utils.MasterClient.StartWorkerAsync(wp).ResponseAsync;
                                            return rr.ContinueWith<bool>((wk) =>
                                            {
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
                                        });
                                    };
                                }
                            }
                            else 
                            {
                                if (w.NetState == 1)
                                {
                                    y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
                                    wp.Wid = w.Wid;
                                    var rr = Y3D.Projects.Utils.MasterClient.StartWorkerAsync(wp).ResponseAsync;
                                    return rr.ContinueWith<bool>((wk) =>
                                    {
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
                                } else if (w.NetState==2)
                                {
                                    worker = w;
                                    return Task.FromResult(true);
                                }
                            }
                        }

                        return Task.FromResult(false);
                    });
                return x.Unwrap();
            }
            else
            {
                //y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
                //wp.Wid = worker.Wid;
                //wp.Ip = worker.MachineIp + ":" + worker.PortLoader;
                //var x = Y3D.Projects.Utils.MasterClient.IsReadyAsync(wp);
                //var rs = x.ResponseAsync.ContinueWith<bool>((task) =>
                //{
                //    if (task.IsFaulted || task.IsCanceled)
                //    {
                //        worker = null;
                //        Y3D.Projects.Utils.MasterClient.StopWorker(wp); //remove worker from list, could be wrong. Xem Ky cho nayf!!!!!!!!!!!!!!!!!!!!!!!
                //        return false;
                //    }
                //    return true;
                //});
                return Task.FromResult(true); 
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
            if (worker != null)
            {
                worker.Wtype = WorkerType.Free;
                Y3D.Projects.Utils.MasterClient.UpdateWorkerAsync(worker);
            }
        }

        static public void newProjectFromMax()
        {
            //var mw = Y3D.Users.Auth.getMainWorker();

            //var x = Y3D.Projects.Utils.getMainWorker();
            //x.ContinueWith(
            //    (task) =>
            //    {

            //        if (task.IsFaulted || task.IsCanceled)
            //        {
            //            MessageBox.Show("Cannot start.");
            //            return;
            //        }
            //        if (task.IsCompleted)
            //        {
            //            if (task.Result)
            //            {
            //                var openFileProject = new System.Windows.Forms.OpenFileDialog();
            //                openFileProject.Filter = "max files (*.max)|*.max|All files (*.*)|*.*";
            //                DialogResult result = openFileProject.ShowDialog();

            //                if (result == DialogResult.OK)
            //                {
            //                    NewProjectParam np = new NewProjectParam();
            //                    np.Folder = System.IO.Path.GetDirectoryName(openFileProject.FileName);
            //                    np.Fname = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
            //                }
            //            }
            //        }

            //    }
            //);
        }

        public static bool LoadProject(ProjectInfo pi)
        {
            var rnp = rpc.YClient.CClient.LoadProject(pi);
            if (rnp.Yal != null)
            {
                CurrentYAL = rnp.Yal;
                return true;
            }
            else
            {
                return false;
            }
        }

    }
}
