using System;
using System.Threading.Tasks;
using y3d.e;
using y3d.s;
using Google.Protobuf;
using Grpc.Core;
using System.Diagnostics;
using System.IO;
using System.Collections.Generic;

namespace YMasterService
{

    class YServiceMasterImpl : y3d.s.YServiceMaster.YServiceMasterBase
    {
        public override Task<YWorkerResponse> AddWorker(YWorkerRequest req, ServerCallContext context)
        {
            YWorkerResponse ret = new YWorkerResponse();
            ret.NewWorker = new YWorker();
            //ret.NewWorker.Wid = YMServer.YSys.MasterServer.MainWorkers.Count + 1;
            YMServer.LastIndex++;
            ret.NewWorker.Wid = YMServer.LastIndex;
            ret.NewWorker.IpAddress = String.Format("127.0.0.1:{0}", ret.NewWorker.Wid + 38000);
            ret.NewWorker.Wname = "Worker " + ret.NewWorker.Wid;
            ret.NewWorker.Status = YWorker.Types.ServingStatus.Unknown;
            YMServer.all_workers.Workers.Add(ret.NewWorker);
            //YMServer.Workers.Add(ret.NewWorker);
            if (!req.CallInApp)
            {
                if (!req.Slient)
                {
                    Process proc = new Process();
                    proc.StartInfo.FileName = req.App.PathRun;
                    proc.Start();
                }
            }
            //Utils.Tools.YWList.Workers.Add(ret.NewWorker);
            //ret.Wlist = Utils.Tools.YWList;
            return Task.FromResult(ret);
        }

        public override Task<YWorkerList> AllWorkers(EmptyParam request, ServerCallContext context)
        {
            return Task.FromResult(YMServer.all_workers);
        }

        //public override Task<YWorkerList> AllWorkers(EmptyParam request, ServerCallContext context)
        //{
        //    return Task.FromResult(new YWorkerList(Utils.Tools.YWList));
        //}

        //public override Task<ResultReply> CheckHealth(EmptyParam request, ServerCallContext context)
        //{
        //    //context.Status = new Grpc.Core.Status(StatusCode.OK,"OK");
        //    return Task.FromResult(new ResultReply());
        //}

        //public override Task<ResultReply> UpdateWorkerStatus(YWorker yw, ServerCallContext context)
        //{
        //    for (int i = 0; i < Utils.Tools.YWList.Workers.Count; i++)
        //    {
        //        if (Utils.Tools.YWList.Workers[i].Wid == yw.Wid)
        //        {
        //            Utils.Tools.YWList.Workers[i] = yw;
        //            return Task.FromResult(new ResultReply());
        //        }
        //    }
        //    return Task.FromResult(new ResultReply());
        //}

    };


    class YMServer
    {
        public static YSystem YSys;
        public static YWorker CurrentWorker = null;
        const int StartPort = 38000;
        public static int LastIndex = 0;
        const string MASTER_IP_DEFAULT = "127.0.0.1";
        public static Server server;
        //public static List<YWorker> Workers = new List<YWorker>();
        public static YWorkerList all_workers = new YWorkerList();

        public static bool StartApp(string app_key)
        {
            var app = YSys.Apps[app_key];
            Process prc = new Process();
            prc.StartInfo.FileName = app.PathRun;
            return prc.Start();
        }

        public static ProjectInfo FindProjectByPath(string folder, string fname)
        {
            for (int i = 0; i < YSys.Projects.Count; i++)
            {
                if ((YSys.Projects[i].OriginalPath == folder) && (YSys.Projects[i].Pname == fname))
                {
                    return YSys.Projects[i];
                }
            }
            return null;
        }

        public static YWorker GiveMeAFreeWorker()
        {
            if (YSys.MasterServer.MainWorkers.Count < 1)
            {
                if (StartApp("MAX3D"))
                {
                    //if (rpc.YClient.CChannel.State != Grpc.Core.ChannelState.Ready)
                    //{
                    //    while (!rpc.YClient.CChannel.ConnectAsync().IsCompleted)
                    //    {
                    //        System.Threading.Thread.Sleep(5000);
                    //    }
                    //    //rpc.YClient.CChannel.WaitForStateChangedAsync(Grpc.Core.ChannelState.Ready);
                    //}
                }
            }
            return null;
        }
        public static bool CheckCurrentWorker()
        {
            if (CurrentWorker == null)
            {
                CurrentWorker = GiveMeAFreeWorker();
                if (CurrentWorker == null) return false;
            }
            return true;

        }

        public static void InitSystem()
        {
            //var path = System.IO.Path.Combine(System.IO.Directory.GetCurrentDirectory(), "ysettings.y3d");
            var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d");
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
            path = System.IO.Path.Combine(path, "ysetting.y3d");

            if (!File.Exists(path))
            {
                YSys = new YSystem();
                YSys.DefaultInfo = new ProjectInfo();
                YSys.DefaultSetting = new PSetting();
                YSys.DefaultSetting.MaxRecent = 5;

                YSys.MasterServer = new y3d.e.YMasterServer();
                YSys.MasterServer.Mname = "Y3D Master Server";
                YSys.MasterServer.Address = MASTER_IP_DEFAULT;
                YSys.MasterServer.Port = StartPort;
                // Set default working folder
                //YSys.WorkingFolder = System.IO.Path.Combine(System.IO.Directory.GetCurrentDirectory(), "ywork");
                YSys.WorkingFolder = "D:\\ywork";
                // Set default 3d application
                WorkerApp MAX3D_APP = new WorkerApp();
                MAX3D_APP.PathRun = "C:\\Program Files\\Autodesk\\3ds Max 2017\\3dsmax.exe";
                MAX3D_APP.Wname = "3dsmax";

                WorkerApp BLENDER_APP = new WorkerApp();
                BLENDER_APP.PathRun = "C:\\Program Files\\Blender Foundation\\Blender\\blender.exe";

                YSys.Apps.Add("MAX3D", MAX3D_APP);
                YSys.Apps.Add("BLENDER", BLENDER_APP);

                // Save to ysetting.y3d
                using (Stream output = File.OpenWrite(path))
                {
                    YSys.WriteTo(output);
                }
                YSys.Projects.Clear();
            }
            else
            {
                using (Stream file = File.OpenRead(path))
                {
                    var a = Google.Protobuf.CodedInputStream.CreateWithLimits(file, 1024 << 20, 10);
                    YSys = YSystem.Parser.ParseFrom(a);
                }
            }

            if (!Directory.Exists(YSys.WorkingFolder))
            {
                Directory.CreateDirectory(YSys.WorkingFolder);
            }
            //Start();
        }

        public static void SaveSystem()
        {
            var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d\\ysettings.y3d");
            using (Stream output = File.OpenWrite(path))
            {
                YSys.WriteTo(output);
            }
        }
        
        public static void StopWorker(YWorker yw, bool stopServerOnly=true)
        {
            Channel channel = new Channel(yw.IpAddress, ChannelCredentials.Insecure);
            y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient client = new YServiceMaxLoader.YServiceMaxLoaderClient(channel);
            if (stopServerOnly)
            {
                client.Shutdown(new LibInfo());
            } else
            {
                client.CloseApp(new LibInfo());
            }
        }

        public static void StopAllWorker()
        {
            foreach (YWorker w in all_workers.Workers)
            {
                StopWorker(w);
            }
        }

        public static void Start()
        {
            server = new Server
            {
                Services = { y3d.s.YServiceMaster.BindService(new YServiceMasterImpl()) },
                Ports = { new ServerPort(YSys.MasterServer.Address, YSys.MasterServer.Port, ServerCredentials.Insecure) }
            };
            server.Start();
        }

        public static void Stop()
        {
            server.ShutdownAsync().Wait();
        }
    }
}
