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

using Google.Protobuf;

using Y3D.Users;
using SplashScreen;

using YFlow;
using Redux;
using System.Reactive.Linq;
using System.Threading;
//using System.Reactive;
//using System.Reactive.Linq;
//using System.Reactive.Disposables;


namespace Y3D.Projects
{
    class Utils
    {
        public static IStore<ApplicationStates> Store { get; private set; }

        public static YWorker worker = null;
        static Channel ChannelLoader = null;
        static Channel ChannelMax = null;

        public static YWorker mworker = null;

        static public y3d.s.YServiceMaxTools.YServiceMaxToolsClient MaxClient = null;
        static public y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient LoaderClient = null;
        static public y3d.s.YServiceTest.YServiceTestClient TestClient = null;

        public static string master_ip = "127.0.0.1";
        public static y3d.s.YServiceMaster.YServiceMasterClient MasterClient = null;

        //public static YAreaList CurrentYAL = null;
        public static ProjectInfo CurrentP = null;

        //public static UserTestData TestData = new UserTestData();
        //public static Dictionary<string, bool> TestInScence = new Dictionary<string, bool>();
        //public static VerTest CurrentTest = null;

        public static Forms.YMainForm mainform = null;

        //public static string current_layer = "0";

        public static void initState()
        {
            var initialState = new ApplicationStates
            {
                isBusy = false,
                ObjectManager = new YFlow.ObjectManagerComponent.States()
            };
            initialState.ObjectManager.CurrentLayer = "0";
            initialState.ObjectManager.CurrentTest = null;
            initialState.ObjectManager.CurrentObject = null;
            initialState.ObjectManager.TestInScence = new Dictionary<string, bool>();
            initialState.ObjectManager.isSaved = true;
            initialState.ObjectManager.CurrentYAL = null;
            initialState.ObjectManager.TestData = new UserTestData();
            initialState.ObjectManager.autoSave = true;

            Store = new Store<ApplicationStates>(YFlow.ApplicationReducers.ReduceApplication, initialState);

            Store.DistinctUntilChanged(state => new { state.ObjectManager.CurrentTest }).Subscribe(
                state =>
                {
                    //if (!state.ObjectManager.isSaved)
                    //{
                    //    MessageBox.Show("Unsave");
                    //    Store.Dispatch(new YFlow.ObjectManagerComponent.SaveTestAction());
                    //}
                    //else
                    //{
                    //    MessageBox.Show("Saved");
                    //}

                    if (state.ObjectManager.CurrentTest != null)
                    {
                        if (state.ObjectManager.TestInScence[state.ObjectManager.CurrentLayer] == false)
                        {
                            if (loadTest(state.ObjectManager.CurrentTest))
                            {
                                state.ObjectManager.TestInScence[state.ObjectManager.CurrentLayer] = true;
                            }
                        }
                    }
                }
            );

            Store.DistinctUntilChanged(state => new { state.ObjectManager.CurrentLayer }).Subscribe(
                state =>
                {
                    if (state.ObjectManager.CurrentLayer != null)
                    {
                        displayLayer(state.ObjectManager.CurrentLayer);
                    }
                }
            );

            Store.DistinctUntilChanged(state => new { state.ObjectManager.isSaved }).Subscribe(
                state =>
                {
                    //MessageBox.Show("save:"+state.ObjectManager.isSaved.ToString());
                    if (state.ObjectManager.autoSave && state.ObjectManager.isSaved == false)
                    {
                        Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.SaveTestAction { });
                        if (saveTestData(state.ObjectManager.TestData))
                            Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.SaveTestAction { });
                        //state.ObjectManager.isSaved = true;
                    }
                }
            );

            //Utils.Store.DistinctUntilChanged(state => new { state.ObjectManager.CurrentObject }).Subscribe(
            //    state =>
            //    {
            //        //MessageBox.Show("Test");
            //        if (state.ObjectManager.CurrentObject!=null)
            //            MessageBox.Show(state.ObjectManager.CurrentObject.Name);
            //    }
            //);
        }

        public static void displayLayer(string lname)
        {
            //if (current_layer == lname) return;
            if (!checkMaxTools(worker)) return;
            //current_layer = lname;
            YEvent ye = new YEvent();
            ye.Isolate = new EIsolate();
            ye.Isolate.Layer = lname;
            ye.Isolate.EndIsolate = false;
            Y3D.Projects.Utils.MaxClient.DoEventAsync(ye);
        }

        //static public Task<bool> myLoading(Task t)
        //{
        //    return Task.Run(
        //        async () =>
        //        {
        //            SplashForm loadingForm = new SplashForm();
        //            loadingForm.AppName = "Initializing data";
        //            loadingForm.Icon = Properties.Resources.wave;
        //            loadingForm.ShowIcon = true;
        //            loadingForm.TopMost = true;
        //            loadingForm.BringToFront();
        //            if (!t.IsCompleted)
        //            {

        //            }
        //            //    loadingForm.ShowInTaskbar = true;
        //            try
        //            {
        //                Application.Run(loadingForm);
        //            }
        //            catch (System.Threading.ThreadAbortException e)
        //            {
        //                return false;
        //                //MessageBox.Show(e.Message);
        //            }
        //            return true;
        //        }    
        //    );


        //}

        static public void myLoading(CancellationToken ct)
        {

            if (ct.IsCancellationRequested)
            {
                Application.ExitThread();
            }

            SplashForm loadingForm = new SplashForm();
            loadingForm.AppName = "Initializing data";
            loadingForm.Icon = Properties.Resources.wave;
            loadingForm.ShowIcon = true;
            loadingForm.TopMost = true;
            loadingForm.BringToFront();


            try
            {
                if (ct.IsCancellationRequested)
                {
                    Application.ExitThread();
                } else
                {
                    Application.Run(loadingForm);
                }
            }
            catch (System.Threading.ThreadAbortException e)
            {
                //MessageBox.Show(e.Message);
            }

            if (ct.IsCancellationRequested)
            {
                Application.ExitThread();
            }
            //Store.DistinctUntilChanged(state => new { state.isBusy }).Subscribe(
            //    state =>
            //    {
            //        LogClientCSharp.LogClient.Instance.LOG("State subcr: " + state.isBusy.ToString() + "   \n");
            //        if (state.isBusy == false)
            //        {
            //            Application.ExitThread();
            //            //MessageBox.Show("out nhe");
            //        }

            //    }
            //);
        }

        public static bool saveTestData(UserTestData TestData)
        { 
            if (CurrentP == null) return true;
            var test_path = System.IO.Path.Combine(CurrentP.ProjectPath, "test","ytest.y3d");
        
            using (Stream output = File.OpenWrite(test_path))
            {
                try
                {
                    TestData.WriteTo(output);
                    return true;
                }
                catch (Exception)
                {

                    return false;
                }
                //Store.GetState().ObjectManager.TestData.WriteTo(output);
            }
            return false;
        }
        public static void loadTestData(YAreaList yal)
        {
            if (CurrentP == null) return;
            var test_path = System.IO.Path.Combine(CurrentP.ProjectPath, "test", "ytest.y3d");
            if (File.Exists(test_path))
            {
                //var initialState = new ApplicationStates
                //{
                //    isBusy = false,
                //    ObjectManager = new YFlow.ObjectManagerComponent.States()
                //};
                //using (Stream file = File.OpenRead(test_path))
                //{
                //    var a = Google.Protobuf.CodedInputStream.CreateWithLimits(file, 1024 << 20, 10);
                //    TestData = UserTestData.Parser.ParseFrom(a);
                //}
                using (Stream stream = File.OpenRead(test_path))
                {
                    try
                    {
                        Store.GetState().ObjectManager.TestData = UserTestData.Parser.ParseFrom(stream);
                    }
                    catch (Exception)
                    {
                        if (Store.GetState().ObjectManager.TestData!=null)
                            Store.GetState().ObjectManager.TestData.Utests.Clear();
                    }
                }
 
                //initialState.ObjectManager.CurrentLayer = "0";
                //initialState.ObjectManager.CurrentTest = null;
                //initialState.ObjectManager.TestInScence = new Dictionary<string, bool>();
                //initialState.ObjectManager.isSaved = false;
                //initialState.ObjectManager.CurrentYAL = yal;

                //Store = null;
                //Store = new Store<ApplicationStates>(YFlow.ApplicationReducers.ReduceApplication, initialState);


            }
            Store.GetState().ObjectManager.CurrentYAL = yal;
            //Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.SetYAreaAction
            //{
            //    yal = yal
            //});
        }

        public static bool checkMaster(bool force=true)
        {
            if (MasterClient != null && force == false) return true;
            var channel = new Channel(master_ip + ":38000", ChannelCredentials.Insecure);
            if (!channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(5000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result)
            {
                MasterClient = new y3d.s.YServiceMaster.YServiceMasterClient(channel);
                return true;
            }
            else
            {
                MasterClient = null;
                var x = MessageBox.Show("Can not connect to master server!! Do you want to retry!","Error",MessageBoxButtons.RetryCancel,MessageBoxIcon.Error);
                if (x == DialogResult.Retry) {
                    return checkMaster();
                } 
            }
            return false;
        }

        public static bool checkLoader(YWorker w, bool force=true)
        {
            if (!checkMaster()) return false;
            if (w == null && force==false) return false;
            if (testConnection("127.0.0.1", w.PortLoader)) return true;
            return false;
        }

        public static bool checkMaxTools(YWorker w, bool force = true)
        {
            if (!checkLoader(w,force)) return false;
            if (w == null && force == false) return false;
            if (testConnection("127.0.0.1", w.PortMax)) return true;
            return false;
        }

        public static void initSystem()
        {
            checkMaster();
        }

        //public static void initProject()
        //{
        //    if (CurrentP == null) return;
        //    var test_path = System.IO.Path.Combine(CurrentP.ProjectPath, "test", "ytest.y3d");
        //    if (File.Exists(test_path))
        //    {
        //        //using (Stream file = File.OpenRead(test_path))
        //        //{
        //        //    var a = Google.Protobuf.CodedInputStream.CreateWithLimits(file, 1024 << 20, 10);
        //        //    TestData = UserTestData.Parser.ParseFrom(a);
        //        //}
        //        var initialState = new ApplicationStates
        //        {
        //            isBusy = false,
        //            ObjectManager = new YFlow.ObjectManagerComponent.States()
        //        };

        //        using (Stream stream = File.OpenRead(test_path))
        //        {
        //            try
        //            {
        //                initialState.ObjectManager.TestData = UserTestData.Parser.ParseFrom(stream);
        //            }
        //            catch (Exception)
        //            {
        //                initialState.ObjectManager.TestData.Utests.Clear();
        //            }
        //        }
        //        initialState.ObjectManager.TestInScence.Clear();

        //        Store = new Store<ApplicationStates>(YFlow.ApplicationReducers.ReduceApplication, initialState);

        //        //Store.DistinctUntilChanged(state => new { state.ObjectManager.TestData }).Subscribe(
        //        //    state => {

        //        //    }
        //        //);
        //        //IObservable<ApplicationStates> oo = Observable.DistinctUntilChanged<ApplicationStates>(x => new ApplicationStates());
        //        //Store.Subscribe(oo,(ApplicationStates s)
        //    }
        //}

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
            if (!checkMaster()) return Task.FromResult(false);

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
            if (!Y3D.Projects.Utils.checkMaster()) return;
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
                            //CurrentYAL = rnp.Yal;
                            CurrentP = rnp.PInfo;
                            Projects.Utils.loadTestData(rnp.Yal);
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
            if (!checkMaxTools(worker)) return false;
            if (Store!=null)
                Store.GetState().ObjectManager.TestInScence.Clear();
            var rnp = Y3D.Projects.Utils.MaxClient.LoadProject(pi);
            if (!rnp.Err)
            {
                if (rnp.Yal != null)
                {
                    CurrentP = pi;
                    Projects.Utils.loadTestData(rnp.Yal);
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
            if (!checkMaxTools(worker)) return false;
            if (pi == null) return false;
            if (CurrentP!=null)
            {
                if (pi.Pname == CurrentP.Pname)
                {
                    mainform.resetOM();
                    YEvent ye = new YEvent();
                    ye.Close = new EClose();
                    ye.Close.Bypass = true;
                    Y3D.Projects.Utils.MaxClient.DoEvent(ye);
                }
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

        public static int unique_id(Google.Protobuf.Collections.RepeatedField<VerTest> arr, int increase=1)
        {
            if (arr.Count == 0) return increase;
            var x = arr.Last();
            if (x!=null)
            {
                return x.Tid + increase;
            }
            return -1;
        }

        public static bool CreateNewTest(string oname, string note,InitTestPreset preset)
        {
            if (!checkMaxTools(worker)) return false;
            VerTest vt = new VerTest();
            vt.Vnote = note;
            vt.Oname = oname;
            vt.HasNormal = false;
            vt.HasPack = false;
            vt.HasBake = false;
            var TestData = Store.GetState().ObjectManager.TestData;
            //vt.Id = Guid.NewGuid().ToString();
            //vt.Id = Convert.ToBase64String(Guid.NewGuid().ToByteArray()).Substring(0, 8);

            if (TestData == null) TestData = new UserTestData();
            if (!TestData.Utests.ContainsKey(oname))
            {
                TestData.Utests.Add(oname, new YListTest());
            }

            vt.Tid = unique_id(TestData.Utests[oname].Otests);
            vt.Id = "ver" + vt.Tid;
            var test_path = System.IO.Path.Combine(CurrentP.ProjectPath, "test",(oname+"_" + vt.Id));
            var count = 1;
            while (Directory.Exists(test_path))
            {
                count++;
                vt.Tid = unique_id(TestData.Utests[oname].Otests, count);
                vt.Id = "ver" + vt.Tid;
                test_path = System.IO.Path.Combine(CurrentP.ProjectPath, "test", (oname + "_" + vt.Id));
                if (count > 10) return false;
            }
            System.IO.Directory.CreateDirectory(test_path);

            InitTestParam itp = new InitTestParam();
            itp.Oname = oname;
            itp.TestFolder = test_path;
            itp.Note = note;
            itp.InitTest = preset;
            itp.Id = vt.Id;

            vt.InitTest = itp.InitTest;
            vt.HasLow = (vt.InitTest.Lowpoly != null);
            vt.HasUnwrap = (vt.InitTest.Unwrap != null);
            vt.HasPack = (vt.InitTest.Pack != null);

            if (vt.HasLow)
            {
                vt.InitTest.Lowpoly.Oname = vt.Oname + "_" + vt.Id + "_high";
                vt.InitTest.Lowpoly.Nname = vt.Oname + "_" + vt.Id + "_low";
                //vt.InitTest.Lowpoly.Surfix = "_low";
            }
            if (vt.HasUnwrap)
            {
                vt.InitTest.Unwrap.Oname = vt.Oname + "_" + vt.Id+ "_low";
            }

            itp.InitTest = vt.InitTest;
            //var x = Y3D.Projects.Utils.MaxClient.Init4TestAsync(itp);
            var x = Y3D.Projects.Utils.MaxClient.Init4Test(itp);

            Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.AddTestAction
            {
                vtest = vt
            });
            //Projects.Utils.saveTestData();
            //current_layer = oname + "_" + vt.Id;
            return true;
        }

        public static bool DeleteTest(VerTest v)
        {
            if (!checkMaxTools(worker)) return false;
            YListTest ListTest = null;
            var TestData = Store.GetState().ObjectManager.TestData;

            if (TestData.Utests.ContainsKey(v.Oname))
                ListTest = TestData.Utests[v.Oname];
            if (ListTest!=null)
            {
                YEvent ye = new YEvent();
                ye.Del = new EDelete();
                ye.Del.Layers.Add(v.Oname + "_" + v.Id);
                Y3D.Projects.Utils.MaxClient.DoEventAsync(ye);

                var test_path = Path.Combine(CurrentP.ProjectPath, "test", (v.Oname + "_" + v.Id));
                if (Directory.Exists(test_path))
                {
                    try
                    {
                        System.IO.Directory.Delete(test_path, true);
                    }
                    catch (System.IO.IOException ee)
                    {
                        Console.WriteLine(ee.Message);
                    }
                }
            }
            return true;
        }

        public static bool UpdateLowPoly(ELowpoly el)
        {
            if (!checkMaxTools(worker)) return false;
            Y3D.Projects.Utils.MaxClient.LowPolyAsync(el);
            return true;
        }

        public static async Task<bool> doEvent(YEvent e)
        {
            if (!checkMaxTools(worker)) return false;
            await Y3D.Projects.Utils.MaxClient.DoEventAsync(e);
            return true;
        }

        public static bool doManyEvent(YEventList e)
        {
            if (!checkMaxTools(worker)) return false;
            Y3D.Projects.Utils.MaxClient.DoManyEventAsync(e);
            return true;
        }

        public static bool loadTest(VerTest v)
        {
            if (v == null) return false;
            InitTestParam lt = new InitTestParam();
            lt.Id = v.Id;
            lt.Oname = v.Oname;
            var layerName = v.Oname + "_" + v.Id;
            lt.TestFolder = System.IO.Path.Combine(CurrentP.ProjectPath, "test", layerName);
            if (!checkMaxTools(worker)) return false;
            Y3D.Projects.Utils.MaxClient.LoadTestData(lt);
            
            return true;

        }
    }
}
