﻿using System;
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

using Y3D.Users;

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
        public static ProjectInfo CurrentP = null;
        public static UserTestData TestData = new UserTestData();
        public static Forms.YMainForm mainform = null;

        //public static YSystem YSys = new YSystem();

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
                                    int count = 0;
                                    while (channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(5000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result && count++ < 50)
                                    {
                                        Console.WriteLine("5s. Retry"); // hien loading
                                    }
                                    if (count==50)
                                    {
                                        MessageBox.Show(String.Format("Can not connect to {0}:{1}", w.MachineIp, w.PortLoader));
                                        return Task.FromResult(false);
                                    }
                                    else
                                    {
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
                                                return true;
                                            }
                                            return false;
                                        });
                                    }

                                    //var channel = new Channel(String.Format("127.0.0.1:{0}", w.PortLoader), ChannelCredentials.Insecure);

                                    //if (channel.State != Grpc.Core.ChannelState.Ready)
                                    //{
                                    //    Task.Factory.StartNew(
                                    //        () =>
                                    //        {
                                    //            int count = 0;
                                    //            while (channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(5000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result && count++ < 50)
                                    //            {
                                    //                Console.WriteLine("5s. Retry"); // hien loading
                                    //            }//*/                   
                                    //        }
                                    //    ).ContinueWith(_ =>
                                    //    {
                                    //        if (_.IsFaulted || _.IsCanceled)
                                    //        {
                                    //            MessageBox.Show(String.Format("Can not connect to {0}:{1}", w.MachineIp, w.PortLoader));
                                    //            return Task.FromResult(false);
                                    //        }
                                    //        // start worker
                                    //        y3d.e.WorkerParam wp = new y3d.e.WorkerParam();
                                    //        wp.Wid = w.Wid;
                                    //        var rr = Y3D.Projects.Utils.MasterClient.StartWorkerAsync(wp).ResponseAsync;
                                    //        return rr.ContinueWith<bool>((wk) =>
                                    //        {
                                    //            if (wk.IsFaulted || wk.IsCanceled || wk.Result.Error)
                                    //            {
                                    //                MessageBox.Show("Can not start worker!");
                                    //                return false;
                                    //            }

                                    //            if (wk.IsCompleted)
                                    //            {
                                    //                worker = wk.Result.Worker;
                                    //                updateClient();
                                    //            }

                                    //            return true;
                                    //        });
                                    //    });
                                    //};
                                }
                            }
                            else 
                            {
                                if (w.NetState!=2)
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
                                } else 
                                {
                                    worker = w;
                                    updateClient();
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

        static public int is_project_exist(string path,string pname)
        {
            foreach (var item in Y3D.Users.Auth.usetting.Projects)
            {
                var pp = item.Value;
                if (pp.OriginalPath == path) return 1;
                if (pp.Pname == pname) return 2;
            }
            return 0;
        }


        static public bool newProjectFromMax()
        {
            if (worker != null)
            {
                var openFileProject = new System.Windows.Forms.OpenFileDialog();
                openFileProject.Filter = "max files (*.max)|*.max|All files (*.*)|*.*";
                DialogResult result = openFileProject.ShowDialog();

                if (result == DialogResult.OK)
                {
                    NewProjectParam np = new NewProjectParam();
                    np.Folder = System.IO.Path.GetDirectoryName(openFileProject.FileName);
                    np.Fname = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
                    np.ProjectPath = System.IO.Path.Combine(Auth.usetting.Workspace, np.Fname);

                    var x = is_project_exist(openFileProject.FileName,np.Fname);
                    if (x==1)
                    {
                        var pi = Auth.usetting.Projects[np.Fname];
                        if (LoadProject(pi))
                        {
                            return true;
                        }
                        // ton tai project -> load project
                    } else if (x==2)
                    {
                        // trung ten: script doi ten ...
                    } else
                    {
                        // new project
                        var rnp = MaxClient.NewProject(np);
                        if (rnp != null)
                        {
                            var o_file = System.IO.Path.Combine(np.ProjectPath, np.Fname + "_o.max");
                            File.Copy(openFileProject.FileName, o_file);
                            CurrentYAL = rnp.Yal;
                            CurrentP = rnp.PInfo;
                            Auth.usetting.Projects.Add(rnp.PInfo.Pname,rnp.PInfo);
                            Auth.updateUSetting();
                            return true;
                        }
                    }
                }
            } else
            {
                MessageBox.Show("No worker found!");
            }
            return false;
        }

        public static bool LoadProject(ProjectInfo pi)
        {
            if (Y3D.Projects.Utils.MaxClient == null) return false;
            var rnp = Y3D.Projects.Utils.MaxClient.LoadProject(pi);
            if (!rnp.Err)
            {
                if (rnp.Yal != null)
                {
                    CurrentYAL = rnp.Yal;
                    CurrentP = pi;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            return false;
        }

        public static bool DeleteProject(ProjectInfo pi)
        {
            if (pi == null) return false;
            if (pi.Pname == CurrentP.Pname)
            {
                mainform.resetOM();
                YEvent ye = new YEvent();
                ye.Close = new EClose();
                ye.Close.Bypass = true;
                Y3D.Projects.Utils.MaxClient.DoEvent(ye);
            }
            if (Auth.usetting.Projects.Remove(pi.Pname))
            {
                if (System.IO.Directory.Exists(@pi.ProjectPath))
                {
                    try
                    {
                        System.IO.Directory.Delete(@pi.ProjectPath, true);
                    }

                    catch (System.IO.IOException e)
                    {
                        Console.WriteLine(e.Message);
                    }
                }
                Auth.updateUSetting();
                return true;
            }
            return false;
        }

        public static bool CreateNewTest(string tname)
        {
            if (TestData == null) TestData = new UserTestData();
            if (!TestData.Utests.ContainsKey(tname))
            {
                TestData.Utests.Add(tname, new YListTest());
            }
            var ListTest = TestData.Utests[tname];
            VerTest vt = new VerTest();
            //vt.Id = Guid.NewGuid().ToString();
            vt.Id = Convert.ToBase64String(Guid.NewGuid().ToByteArray()).Substring(0, 8);
            var test_path = System.IO.Path.Combine(CurrentP.ProjectPath, "test",(tname+"_" + vt.Id));
            var count = 0;
            while (Directory.Exists(test_path))
            {
                count++;
                vt.Id = Convert.ToBase64String(Guid.NewGuid().ToByteArray()).Substring(0, 8);
                test_path = System.IO.Path.Combine(CurrentP.ProjectPath, "test", (tname + "_" + vt.Id));
                if (count > 10) return false;
            }
            System.IO.Directory.CreateDirectory(test_path);

            InitTestParam itp = new InitTestParam();
            itp.Tname = tname;
            itp.TestFolder = test_path;
            var x = Y3D.Projects.Utils.MaxClient.Init4Test(itp);
            TestData.Utests[tname].Otests.Add(vt);
            return true;
        }

    }
}