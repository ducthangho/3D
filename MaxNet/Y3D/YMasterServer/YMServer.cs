using System;
using System.Threading.Tasks;
using y3d.e;
using y3d.s;
using Google.Protobuf;
using Grpc.Core;
using System.Diagnostics;
using System.IO;
using System.Collections.Generic;
using System.Collections.Concurrent;

namespace YMasterServer
{
    class YServiceMasterImpl : y3d.s.YServiceMaster.YServiceMasterBase
    {
        public override Task<YWorkerResponse> AddWorker(YWorkerRequest req, ServerCallContext context)
        {
            var x = YMServer.GiveMeNewID(req.Machine.IpAddress);
            var t = x.ContinueWith<YWorkerResponse>((task) =>
            {
                YWorkerResponse ret = new YWorkerResponse();
                ret.Error = true;
                ret.Worker = YRedis.getWorkerById(task.Result);
                if (ret.Worker==null)
                {
                    ret.Worker = YMServer.createDefaultWorker(req.Machine.IpAddress, task.Result);
                }
                
                if (task.IsCompleted)
                {
                    ret.Wlist = new YWorkerList();
                    //if (req.CallInApp)
                    //{
                    //    ret.Worker.NetState = 1;
                    //}
                    if (ret.Worker.NetState == 4)
                    {
                        ret.Worker.NetState = 1;
                        YRedis.updateWorker(ret.Worker);
                        ret.Wlist.Workers.Add(YRedis.getWorkersByMachine(req.Machine));
                        ret.Error = false;
                        return ret;
                    } else
                    {
                        if (YRedis.addWorker(ret.Worker))
                        {
                            ret.Wlist.Workers.Add(YRedis.getWorkersByMachine(req.Machine));
                            //if (!req.CallInApp)
                            //{
                            //    if (!req.Slient)
                            //    {
                            //        Process proc = new Process();
                            //        proc.StartInfo.FileName = req.App.PathRun;
                            //        proc.Start();
                            //    }
                            //}
                            Console.WriteLine(String.Format("{0} Loader was created and listening on {1}:{2} ", ret.Worker.Wname, ret.Worker.MachineIp, ret.Worker.PortLoader));
                            YMServer.Update2GUI();
                            ret.Error = false;
                        }
                        //else
                        //{
                        //    Console.WriteLine(String.Format("Crash {0} Loader was restored and listening on {1}:{2} ", ret.Worker.Wname, ret.Worker.MachineIp, ret.Worker.PortLoader));
                        //    YMServer.Update2GUI();
                        //}
                    }

                }
                return ret;
            });
            return t;
        }

        public override Task<YWorkerResponse> AllWorkers(AllWorkerParam request, ServerCallContext context)
        {
            YWorkerResponse rep = new YWorkerResponse();
            rep.Wlist = new YWorkerList();
            rep.Error = false;
            if (request.Refresh)
            {
                var task = YMServer.refreshWorkers();
                return task.ContinueWith<YWorkerResponse>((t) =>
                {
                    if (request.Machine!=null)
                    {
                        rep.Wlist.Workers.Add(YRedis.getWorkersByMachine(request.Machine));
                    } else
                    {
                        rep.Wlist.Workers.Add(YRedis.getAllWorker());
                    }
                    return rep;
                });
            }
            if (request.Machine != null)
            {
                rep.Wlist.Workers.Add(YRedis.getWorkersByMachine(request.Machine));
            }
            else
            {
                rep.Wlist.Workers.Add(YRedis.getAllWorker());
            }
            return Task.FromResult(rep);
        }

        public override Task<YWorkerResponse> StartWorker(WorkerParam request, ServerCallContext context)
        {
            YWorkerResponse rep = new YWorkerResponse();
            rep.Error = true;

            var yw = YRedis.getWorkerById(request.Wid);
            if (yw != null)
            {
                var t = YMServer.StartWorker(yw);
                return t.ContinueWith<YWorkerResponse>((tt) =>
                {
                    if (tt.IsCompleted)
                    {
                        rep.Worker = yw;
                        rep.Wlist = new YWorkerList();
                        rep.Wlist.Workers.Add(YRedis.getWorkersByMachine(yw.MachineIp));
                        rep.Error = false;
                        return rep;
                    }
                    return rep;
                });
            }
            return Task.FromResult(rep);
        }

        public override Task<YWorkerResponse> StopWorker(WorkerParam request, ServerCallContext context)
        {
            YWorkerResponse rep = new YWorkerResponse();
            rep.Error = true;
            var yw = YRedis.getWorkerById(request.Wid);
            if (yw != null)
            {
                var t = YMServer.StopWorker(yw);
                return t.ContinueWith<YWorkerResponse>((tt) =>
                {
                    if (tt.IsCompleted)
                    {
                        rep.Worker = yw;
                        rep.Wlist = new YWorkerList();
                        rep.Wlist.Workers.Add(YRedis.getWorkersByMachine(yw.MachineIp));
                        rep.Error = false;
                        rep.Wid = request.Wid;
                        Console.WriteLine(String.Format("{0} is closed on {1}:{2}", yw.Wname, yw.MachineIp, yw.PortMax));
                    }
                    return rep;
                });
            }
            return Task.FromResult(rep);
            //var yw = YMServer.workers[request.Wid];
            //try
            //{
            //    Channel channel = new Channel(yw.IpLoader, ChannelCredentials.Insecure);
            //    y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient loaderClient = new y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient(channel);
            //    LibInfo req = new LibInfo();
            //    req.Id = yw.Wid;
            //    var retDll = loaderClient.Shutdown(req);
            //    yw.Status = y3d.e.YWorker.Types.ServingStatus.NotServing;

            //    YWorkerResponse rep = new YWorkerResponse();
            //    rep.Worker = yw;
            //    rep.Wlist = new YWorkerList();
            //    rep.Wlist.Workers.Add(YMServer.workers.Values);
            //    rep.Error = false;
            //    Console.WriteLine(String.Format("{0} is closed on {1}", yw.Wname, yw.IpMax));
            //    return Task.FromResult(rep);
            //}
            //catch (KeyNotFoundException ex)
            //{
            //    Console.WriteLine("Khong tim thay key");
            //}
            //return base.StartWorker(request, context);
        }

        public override Task<ResultReply> CloseWorkerApp(WorkerParam request, ServerCallContext context)
        {
            ResultReply rep = new ResultReply();
            rep.Error = true;
            YWorker yw;
            String s = "";
            yw = YMServer.GetWorker(request);
            s = String.Format("127.0.0.1:{0}", yw.Wid + 39000);
            var toolClient = new y3d.s.YServiceMaxTools.YServiceMaxToolsClient(new Channel(s, ChannelCredentials.Insecure));
            toolClient.ShutdownAsync(new EmptyParam());
            //YMServer.all_workers.Workers.Remove(yw);
            rep.Error = false;

            //new System.Threading.Thread(() =>
            //{
            //    var yw = YMServer.GetWorker(request);

            //    if (yw != null)
            //    {

            //        var p = System.Diagnostics.Process.GetProcessById(yw.ProcessId);
            //        //System.Windows.Forms.MessageBox.Show(p.Id.ToString());
            //        try
            //        {
            //            p.CloseMainWindow();
            //            p.Close();
            //            p.Kill();
            //        }
            //        catch (System.Exception ex)
            //        {
            //            //System.Windows.Forms.MessageBox.Show(ex.Message);
            //        }
            //        //if (p != null) p.Kill();
            //        //Channel channel = new Channel(yw.IpAddress, ChannelCredentials.Insecure);
            //        //y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient loaderClient = new y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient(channel);
            //        //loaderClient.CloseApp(new LibInfo());
            //        rep.Error = false;
            //        //return Task.FromResult(rep);
            //    }
            //    //Thread.CurrentThread.IsBackground = true;
            //    /* run your code here */
            //    //Console.WriteLine("Hello, world");
            //}).Start();

            //var yw = YMServer.GetWorker(request);

            //if (yw != null)
            //{

            //    var p = System.Diagnostics.Process.GetProcessById(yw.ProcessId);
            //    //System.Windows.Forms.MessageBox.Show(p.Id.ToString());
            //    try
            //    {
            //        p.CloseMainWindow();
            //        p.Close();
            //        p.Kill();
            //    }
            //    catch (System.Exception ex)
            //    {
            //        //System.Windows.Forms.MessageBox.Show(ex.Message);
            //    }
            //    //if (p != null) p.Kill();
            //    rep.Error = false;
            //    return Task.FromResult(rep);
            //}
            return Task.FromResult(rep);
        }

        public override Task<ResultReply> AppExitCallback(WorkerParam request, ServerCallContext context)
        {
            ResultReply rep = new ResultReply();
            rep.Error = true;
            YWorker yw = YRedis.getWorkerById(request.Wid);
            if (YRedis.removeWorkerById(request.Wid))
            {
                Console.WriteLine(String.Format("{0}({1}:{2}) has been removed automatically when someone exit application!", yw.Wname, yw.MachineIp, yw.PortLoader));
                YMServer.Update2GUI();
                rep.Error = false;
            }
            return Task.FromResult(rep);
        }

        public override Task<ResultReply> StopAllWorkers(EmptyParam request, ServerCallContext context)
        {
            return YMServer.StopAllWorker().ContinueWith<ResultReply>(_ => {
                Console.WriteLine("All worker are closed");
                return new ResultReply();
            });
            //return Task.FromResult(new ResultReply());
        }

        public override Task<YWorkerResponse> GiveMeAWorker(YWorkerRequest request, ServerCallContext context)
        {
            var yw = YMServer.GiveMeAFreeWorker(request);
            var t = yw.ContinueWith<YWorkerResponse>((task) =>
            {
                YWorkerResponse result = new YWorkerResponse();
                if (task.IsFaulted || task.IsCanceled)
                {
                    result.Error = true;
                    result.Worker = null;
                    result.Message = task.Exception.Message;
                } else if (task.IsCompleted)
                {
                    result.Error = false;
                    result.Worker = task.Result;
                }
                YMServer.Update2GUI();
                return result;
            });
            return t;
            //return Task.FromResult(rep);
        }

        public override Task<ResultReply> StartAllWorkers(EmptyParam request, ServerCallContext context)
        {
            YMServer.RestoreAllWorkers(true);
            Console.WriteLine("All worker are restored");
            return Task.FromResult(new ResultReply());
        }

        public override Task<ReadyRespone> IsReady(WorkerParam request, ServerCallContext context)
        {

            string ip = "";
            if (request.WtypeCase == WorkerParam.WtypeOneofCase.Ip)
            {
                ip = request.Ip;
            } else if (request.WtypeCase == WorkerParam.WtypeOneofCase.Worker)
            {
                ip = request.Worker.MachineIp+":"+request.Worker.PortMax;
            }

            var ready = YMServer.testConnection(request.Ip, request.Worker.PortMax);

            ReadyRespone rp = new ReadyRespone();
            rp.Status = ServingStatus.NotServing;
            if (ready) {
                rp.Status = ServingStatus.Serving;
            }
            return Task.FromResult(rp);
        }

        public override Task<UserResponse> UserRegister(UserParam request, ServerCallContext context)
        {
            return Task.FromResult(YRedis.SignUp(request.Uname, request.Password));
        }

        public override Task<UserResponse> UserLogin(UserParam request, ServerCallContext context)
        {
            return Task.FromResult(YRedis.SignIn(request.Uname, request.Password));
        }

        public override Task<ResultReply> UpdateUserSetting(UserParam request, ServerCallContext context)
        {
            ResultReply ret = new ResultReply();
            ret.Error = !YRedis.updateUserSetting(request.Uname, request.Usetting);
            if (ret.Error) ret.Message = "Can not update!";
            return Task.FromResult(ret);
        }
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
        public override Task<ResultReply> UpdateWorker(YWorker yw, ServerCallContext context)
        {
            ResultReply rep = new ResultReply();
            rep.Error = !YRedis.updateWorker(yw);
            return Task.FromResult(rep);
        }
    };

    class YMServer
    {
        public static YSystem YSys;
        public static YWorker CurrentWorker = null;
        const int StartPort = 38000;
        public const string MASTER_IP_DEFAULT = "127.0.0.1";
        public static string MASTER_IP_LAN= YMServer.GetLocalIPAddress();
        public static Server server;
        //public static ConcurrentDictionary<int, YWorker> workers = new ConcurrentDictionary<int, YWorker>();

        public static string GetLocalIPAddress()
        {
            var host = System.Net.Dns.GetHostEntry(System.Net.Dns.GetHostName());
            foreach (var ip in host.AddressList)
            {
                if (ip.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                {
                    return ip.ToString();
                }
            }
            return "";

        }

        public static Task<YWorker> check_worker(YWorker yw, bool check_loader_only=false)
        {
            Console.WriteLine("checking worker " + yw.Wid + " ...");
            if (yw.NetState == 4) return Task.FromResult(yw);
            yw.NetState = 0;
            var ip = (yw.MachineIp == YMServer.MASTER_IP_LAN) ? "127.0.0.1" : yw.MachineIp;
            if (testConnection(ip, yw.PortLoader))
            {
                yw.NetState = 1;
                if (check_loader_only) return Task.FromResult(yw);
                if (testConnection(ip, yw.PortMax))
                {
                    yw.NetState = 2;
                }
            }
            return Task.FromResult(yw);
        }

        public static Task<int> GiveMeNewID(string machine_ip)
        {
            var task = refreshWorkers();
            return task.ContinueWith<int>( t =>
            {
                var ww = t.Result;
                foreach (var w in ww)
                {
                    if (w.NetState == 4)
                    {
                        if (machine_ip == w.MachineIp) return w.Wid;
                    }
                }
                return YRedis.incrWorkerId();
            });
        }

        public static YWorker GetWorker(WorkerParam wp) {

            if ((wp.WtypeCase == WorkerParam.WtypeOneofCase.Wid))
            {
                var x = YRedis.rc.Db.HashGet("yworkers", wp.Wid.ToString());
                return YWorker.Parser.ParseFrom(x);
            }
            return null;
        }

        public static YWorker createDefaultWorker(String ip, int wid)
        {
            var tmp = new YWorker();
            tmp.Wid = wid;
            tmp.MachineIp = ip;
            tmp.PortLoader = 38000 + wid;
            tmp.PortMax = 39000 + wid;
            tmp.Wname = "Worker " + wid;
            tmp.Status = ServingStatus.NotServing;
            tmp.Wtype = WorkerType.Free;
            tmp.NetState = 0;
            return tmp;
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

        public static Task<YWorker> GiveMeAFreeWorker(YWorkerRequest req)
        {
            if (req.Wtype==WorkerType.MainWorker || req.Wtype==WorkerType.SubWorker) // get worker from same machine
            {
                var ww = YRedis.getWorkersByMachine(req.Machine);
                foreach (var w in ww)
                {
                    if (w.Wtype == WorkerType.Free)
                    {
                        w.Wtype = req.Wtype;
                        YRedis.updateWorker(w);
                        return Task.FromResult(w);
                    } else
                    {
                        if (req.Wtype == WorkerType.MainWorker && w.Wtype == WorkerType.MainWorker)
                        {
                            YRedis.updateWorker(w);
                            return Task.FromResult(w);
                        }
                    }
            
                }
            } else if (req.Wtype==WorkerType.RemoteWorker) // get worker from other machine
            {
                //var ww = YRedis.getAllWorker();
                var ww = YRedis.getWorkersByMachine(req.Machine,false); 
                foreach (var w in ww)
                {
                    if (w.Wtype == WorkerType.Free)
                    {
                        w.Wtype = req.Wtype;
                        YRedis.updateWorker(w);
                        return Task.FromResult(w);
                    }
                }
            }

            var new_id = YRedis.incrWorkerId();
            var nw = createDefaultWorker(req.Machine.IpAddress, new_id);
            nw.NetState = 4;
            YRedis.addWorker(nw);
            return Task.FromResult(nw);
            // not found any free worker


            //var index = YRedis.getMaxWorkerId() + 1;

            //if (testConnection(MASTER_IP_DEFAULT,index,38000))
            //{
            //    var tmp = createDefaultWorker(MASTER_IP_DEFAULT,index);
            //    if (testConnection(MASTER_IP_DEFAULT, index, 39000))
            //        tmp.Status = ServingStatus.Serving;
            //    else tmp.Status = ServingStatus.NotServing;
            //    tmp.Wtype = WorkerType.MainWorker;
            //    YRedis.updateWorker(tmp);
            //    return Task.FromResult(tmp);
            //}

            //if (StartApp("MAX3D"))
            //{
            //    var channel = new Channel(String.Format("127.0.0.1:{0}", index + 38000), ChannelCredentials.Insecure);

            //    if (channel.State != Grpc.Core.ChannelState.Ready)
            //    {
            //        return Task.Factory.StartNew(
            //            () =>
            //            {
            //                int count = 0;
            //                while (channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(5000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result && count++ < 20)
            //                {
            //                    Console.WriteLine("5s. Retry");
            //                }//*/                   
            //                Console.WriteLine("Connected!!!");
            //            }
            //        ).ContinueWith<YWorker>((tt) =>
            //        {
            //            YWorker tmp = YRedis.getWorkerById(index);
            //            if (tmp!=null)
            //            {
            //                tmp.Wtype = WorkerType.MainWorker;
            //                YRedis.updateWorker(tmp);
            //                return tmp;
            //            }
            //            return null;
            //        });
            //    };

            //    YWorker rt = YRedis.getWorkerById(index); 
            //    if (rt != null)
            //        return Task.FromResult<YWorker>(rt);
            //}

            return Task.FromResult<YWorker>(null);

            //for (int j = 1; j <= index; ++j)
            //{
            //    if (YMServer.workers[index] == null && testConnection(MASTER_IP_DEFAULT, index))
            //    {
            //        var tmp = createDefaultWorker(MASTER_IP_DEFAULT, index);
            //        if (YMServer.workers.TryAdd(index, tmp))
            //        {
            //            tmp.Wtype = YWorker.Types.WorkerType.MainWorker;
            //            tmp.NoApp = false;
            //            YMServer.workers[index] = tmp;
            //            return Task.FromResult(tmp);
            //        }
            //    }
            //}
        }


        public static void InitSystem()
        {
            YRedis.rc = new CacheClientBase(new ProtobufSerializer());
            if (!YRedis.rc.Db.KeyExists("ysystem"))
            {
                YSys = new YSystem();
                YSys.DefaultInfo = new ProjectInfo();
                YSys.DefaultSetting = new PSetting();
                YSys.DefaultSetting.MaxRecent = 5;

                YSys.MasterServer = new y3d.e.MasterServerInfo();
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

                //var x = RedisHelper.RedisProtobuf.Serialize(YSys);
                //var x = YSys.ToByteArray();
                //rc.db.StringSet("ysystem", x);
                //rdb.SetAdd("ysystem", x);
                //RedisHelper.sub.Add("ysystem", YSys);
                YRedis.rc.Sut.Add("ysystem", YSys);
            }
            else
            {
                var x = YRedis.rc.Db.StringGet("ysystem");
                YSys = YSystem.Parser.ParseFrom(x);
            }

            //var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d");
            //Console.WriteLine("Y3D System Path:" + path);
            //if (!Directory.Exists(path))
            //{
            //    Directory.CreateDirectory(path);
            //}
            //path = System.IO.Path.Combine(path, "ysetting.y3d");

            //if (!File.Exists(path))
            //{
            //    YSys = new YSystem();
            //    YSys.DefaultInfo = new ProjectInfo();
            //    YSys.DefaultSetting = new PSetting();
            //    YSys.DefaultSetting.MaxRecent = 5;

            //    YSys.MasterServer = new y3d.e.MasterServerInfo();
            //    YSys.MasterServer.Mname = "Y3D Master Server";
            //    YSys.MasterServer.Address = MASTER_IP_DEFAULT;
            //    YSys.MasterServer.Port = StartPort;
            //    // Set default working folder
            //    //YSys.WorkingFolder = System.IO.Path.Combine(System.IO.Directory.GetCurrentDirectory(), "ywork");
            //    YSys.WorkingFolder = "D:\\ywork";
            //    // Set default 3d application
            //    WorkerApp MAX3D_APP = new WorkerApp();
            //    MAX3D_APP.PathRun = "C:\\Program Files\\Autodesk\\3ds Max 2017\\3dsmax.exe";
            //    MAX3D_APP.Wname = "3dsmax";

            //    WorkerApp BLENDER_APP = new WorkerApp();
            //    BLENDER_APP.PathRun = "C:\\Program Files\\Blender Foundation\\Blender\\blender.exe";

            //    YSys.Apps.Add("MAX3D", MAX3D_APP);
            //    YSys.Apps.Add("BLENDER", BLENDER_APP);

            //    // Save to ysetting.y3d
            //    using (Stream output = File.OpenWrite(path))
            //    {
            //        YSys.WriteTo(output);
            //    }
            //    YSys.Projects.Clear();
            //}
            //else
            //{
            //    using (Stream file = File.OpenRead(path))
            //    {
            //        var a = Google.Protobuf.CodedInputStream.CreateWithLimits(file, 1024 << 20, 10);
            //        YSys = YSystem.Parser.ParseFrom(a);
            //    }
            //}

            //if (!Directory.Exists(YSys.WorkingFolder))
            //{
            //    Directory.CreateDirectory(YSys.WorkingFolder);
            //}
            //Start();
        }

        public static Task SaveSystem()
        {
            //var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d\\ysettings.y3d");
            //using (Stream output = File.OpenWrite(path))
            //{
            //    YSys.WriteTo(output);
            //}
            YRedis.rc.Sut.Add("ysystem", YSys);
            return Task.FromResult(0);
        }

        public static Task<List<YWorker>> refreshWorkers(bool loadTmp=false)
        {
            Console.WriteLine("Recheck status of all workers");
            var wlist = YRedis.getAllWorker();
            var tasks = new List<Task>();

            foreach (var w in wlist)
            {
                tasks.Add(Task.Run(
                    () => {
                        var old_state = w.NetState;
                        var y = check_worker(w, loadTmp);
                        return y.ContinueWith(_ => {
                            if (old_state != y.Result.NetState)
                                YRedis.updateWorker(y.Result);
                        });
                    }
                ));
            }
            return Task.WhenAll(tasks).ContinueWith<List<YWorker>>(_ => {
                var wl = YRedis.getAllWorker();
                // remove all workers without loader
                foreach (var w in wl)
                {
                    if (w.NetState == 0)
                    {
                        if (YRedis.removeWorkerById(w.Wid))
                        {
                            Console.WriteLine(String.Format("Remove Worker {0} from database", w.Wid));
                        }
                    }
                }
                var ret = YRedis.getAllWorker();
                if (ret.Count == 0)
                {
                    YRedis.rc.Db.KeyDelete("max_worker_id");
                    Console.WriteLine("reset max worker id!");
                }
                Console.WriteLine("check all workers done!");
                return ret;
            });
        }

        public static Task StopWorker(YWorker yw, bool stopServerOnly = true)
        {
            Console.WriteLine(String.Format("Stopping {0}..",yw.Wname));
            var ip = (yw.MachineIp == YMServer.MASTER_IP_LAN) ? "127.0.0.1" : yw.MachineIp;
            var client = new YServiceMaxLoader.YServiceMaxLoaderClient(new Channel(ip + ":" + yw.PortLoader, ChannelCredentials.Insecure));
            if (stopServerOnly)
            {
                int wid = yw.Wid;
                var rep = client.ShutdownAsync(new LibInfo());
                return rep.ResponseAsync.ContinueWith( (_,idx) => {
                    var wk = YRedis.getWorkerById((int)idx);
                    if (wk != null)
                    {              
                        wk.Status = ServingStatus.NotServing;
                        wk.NetState = 1;
                        YRedis.updateWorker(wk);
                    }
                },wid);                
            }
            else
            {
                return client.CloseAppAsync(new LibInfo()).ResponseAsync.ContinueWith( _ => { });
            }
        }
        public static Task StartWorker(YWorker yw, bool startServerOnly = true)
        {
            var ip = (yw.MachineIp == YMServer.MASTER_IP_LAN) ? "127.0.0.1" : yw.MachineIp;
            var client = new YServiceMaxLoader.YServiceMaxLoaderClient(new Channel(ip + ":" + yw.PortLoader, ChannelCredentials.Insecure));

            if (startServerOnly)
            {
                LibInfo req = new LibInfo();
                req.Id = yw.Wid;
                
                try
                {
                    int idx = yw.Wid;
                    var r = client.LoadDllAsync(req);
                    return r.ResponseAsync.ContinueWith((t,wid) => {//yw could have changed when get here
                        var o = YRedis.getWorkerById((int)wid);
                        if (t.IsFaulted || t.IsCanceled)
                        {
                            if (o!=null) Console.WriteLine(String.Format("{0} App Server can not start on {1}", o.Wname, o.MachineIp + ":" + o.PortMax));
                            return;
                        }

                        if (t.IsCompleted && o!=null)
                        {
                            Console.WriteLine(String.Format("{0} App Server is ready on {1}", o.Wname, o.MachineIp + ":" + o.PortMax));                          
                            o.Status = y3d.e.ServingStatus.Serving;
                            o.NetState = 3;
                            YRedis.updateWorker(o);
                            return;
                        }                        
                    },idx);

                    //yw.Status = y3d.e.ServingStatus.Serving;
                    //Console.WriteLine(String.Format("{0} App Server is ready on {1}", yw.Wname, yw.MachineIp + ":" + yw.PortMax));
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("Something wrong!");
                }
            }
            else
            {
            }
            return Task.FromResult(0);            

        } 
        public static Task StopAllWorker()
        {
            YRedis.saveWorkers2Temp();
            Console.WriteLine("Stopping all workers..");
            List<Task> tasks = new List<Task>();
            var ww = YRedis.getAllWorker();
            foreach (var w in ww)
            {
                if (w.Status == ServingStatus.Serving)
                    tasks.Add(StopWorker(w));
            }
            return Task.WhenAll(tasks);
        }

        public static Task RestoreAllWorkers(bool loadTmp=false)
        {
            if (loadTmp) YRedis.loadWorkerFromTemp();
            Console.WriteLine("Loading all workers...");
            Console.WriteLine("Restore status of all workers");
            return refreshWorkers(loadTmp).ContinueWith(_ =>
            {
                var ww = _.Result;
                if (ww.Count < 1)
                {
                    Console.WriteLine("No worker found!");
                    YRedis.rc.Db.KeyDelete("max_worker_id");
                    return Task.FromResult(0);
                } else
                {
                    List<Task> tList = new List<Task>();
                    foreach (var w in ww)
                    {
                        if (w.NetState == 1)
                        {
                            if (w.Status == ServingStatus.Serving)
                            {
                                tList.Add(StartWorker(w));
                            }
                        }
                    }
                    return Task.WhenAll(tList);
                }
            }).Unwrap();
        }

        public static void Update2GUI(string m_ip="127.0.0.1")
        {
            Console.WriteLine("Updating worker status to GUI...");
            var ww = YRedis.getWorkersByMachine(m_ip);
            foreach (var yw in ww)
            {
                Console.WriteLine(String.Format("{0} type is {1}", yw.Wname, yw.Wtype.ToString()));
                if (yw.Wtype != WorkerType.Free)
                {
                    var ip = (yw.MachineIp == YMServer.MASTER_IP_LAN) ? "127.0.0.1" : yw.MachineIp;
                    var channel = new Channel(ip + ":" + (yw.Wid + 37000), ChannelCredentials.Insecure);
                    var client = new YServiceMainWorker.YServiceMainWorkerClient(channel);
                    if (client != null)
                    {
                        YWorkerResponse wr = new YWorkerResponse();
                        wr.Worker = yw;
                        wr.Wlist = new YWorkerList();
                        wr.Wlist.Workers.Add(ww);
                        client.UpdateWorkersAsync(wr);
                    }
                }
            }
        }

        //public static Task saveWorkers()
        //{
        //    Console.WriteLine("Saving all workers...");
        //    var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d\\working_workers.y3d");
        //    var wl = new YWorkerList();
        //    wl.Workers.Add(YMServer.workers.Values);
        //    using (Stream output = File.OpenWrite(path))
        //    {
        //        wl.WriteTo(output);
        //    }
        //    return Task.FromResult(0);

        //}
        //public static void forceStopWorker(YWorker yw)
        //{
        //    var s = String.Format("127.0.0.1:{0}", yw.Wid + 39000);
        //    Channel channel = new Channel(s, ChannelCredentials.Insecure);
        //    var toolClient = new y3d.s.YServiceMaxTools.YServiceMaxToolsClient(channel);
        //    toolClient.ShutdownAsync(new EmptyParam());
        //}
        //public static Task<bool> ip_ready(string ip_address, int timeout=3000)
        //{

        //    var channel = new Channel(ip_address, ChannelCredentials.Insecure);
        //    var t = channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(timeout));

        //    return t.ContinueWith<bool>(
        //        (task) =>
        //        {
        //            bool rs = false;
        //            if (task.IsFaulted || task.IsCanceled)
        //                rs = true;
        //            return rs;
        //        }
        //    );

        //    //return (channel.State == ChannelState.Ready);
        //}

        //public static bool StartApp(string app_key)
        //{
        //    var app = YSys.Apps[app_key];
        //    Process prc = new Process();
        //    prc.StartInfo.FileName = app.PathRun;
        //    return prc.Start();
        //}

        //public static ProjectInfo FindProjectByPath(string folder, string fname)
        //{
        //    for (int i = 0; i < YSys.Projects.Count; i++)
        //    {
        //        if ((YSys.Projects[i].OriginalPath == folder) && (YSys.Projects[i].Pname == fname))
        //        {
        //            return YSys.Projects[i];
        //        }
        //    }
        //    return null;
        //}


        public static void Start()
        {
            //var t = RestoreAllWorkers();
            //t.ContinueWith(_ => {
            //    server = new Server
            //    {
            //        Services = { y3d.s.YServiceMaster.BindService(new YServiceMasterImpl()) },
            //        Ports = { new ServerPort(YSys.MasterServer.Address, YSys.MasterServer.Port, ServerCredentials.Insecure) }
            //    };
            //    server.Start();
            //});
            server = new Server
            {
                Services = { y3d.s.YServiceMaster.BindService(new YServiceMasterImpl()) },
                Ports = { new ServerPort(YSys.MasterServer.Address, YSys.MasterServer.Port, ServerCredentials.Insecure) }
            };
            server.Start();
            YRedis.rc.Db.KeyDelete("yworker_temp");
            RestoreAllWorkers();
        }

        public static Task Stop()
        {
            return server.ShutdownAsync();
        }
    }
}
