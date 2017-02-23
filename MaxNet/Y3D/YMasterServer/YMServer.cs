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
            var x = YMServer.GiveMeNewID();
            var t = x.ContinueWith<YWorkerResponse>((task) =>
            {
                YWorkerResponse ret = new YWorkerResponse();
                ret.Worker = new YWorker();
                ret.Error = true;
                if (task.IsCompleted)
                {
                    string ip = YMServer.MASTER_IP_DEFAULT;
                    if (req.Machine != null)
                    {
                        ip = req.Machine.IpAddress; // viet them ham lay ip ko co cong
                    }
                    //ret.Worker.Wid = System.Threading.Interlocked.Increment(ref YMServer.LastIndex);
                    ret.Worker.Wid = x.Result;
                    ret.Worker.MachineIp = req.Machine.IpAddress;
                    ret.Worker.PortLoader = 38000 + ret.Worker.Wid;
                    ret.Worker.PortMax = 39000 + ret.Worker.Wid;
                    ret.Worker.Wname = "Worker " + ret.Worker.Wid;
                    ret.Worker.Status = ServingStatus.NotServing;
                    ret.Worker.Wtype = YWorker.Types.WorkerType.Free;
                    ret.Worker.NoApp = true;

                    if (req.CallInApp)
                    {
                        ret.Worker.NoApp = false;
                    }

                    if (YMServer.workers.TryAdd(ret.Worker.Wid, ret.Worker))
                    {
                        ret.Wlist = new YWorkerList();
                        ret.Wlist.Workers.Add(YMServer.workers.Values);
                        if (!req.CallInApp)
                        {
                            if (!req.Slient)
                            {
                                Process proc = new Process();
                                proc.StartInfo.FileName = req.App.PathRun;
                                proc.Start();
                            }
                        } else
                        {

                        }
                        Console.WriteLine(String.Format("{0} Loader was created and listening on {1}:{2} ", ret.Worker.Wname, ret.Worker.MachineIp,ret.Worker.PortLoader));
                        YMServer.Update2GUI();
                        ret.Error = false;
                    } else
                    {
                        Console.WriteLine(String.Format("Crash {0} Loader was restored and listening on {1}:{2} ", ret.Worker.Wname, ret.Worker.MachineIp, ret.Worker.PortLoader));
                        YMServer.Update2GUI();
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
                    rep.Wlist.Workers.Add(YMServer.workers.Values);
                    return rep;
                });
            }
            rep.Wlist.Workers.Add(YMServer.workers.Values);
            return Task.FromResult(rep);
        }

        public override Task<YWorkerResponse> StartWorker(WorkerParam request, ServerCallContext context)
        {
            YWorkerResponse rep = new YWorkerResponse();
            rep.Error = true;
            try
            {
                var yw = YMServer.workers[request.Wid];                
                var t = YMServer.StartWorker(yw);
                return t.ContinueWith<YWorkerResponse>((tt) => {
                    if (tt.IsCompleted)
                    {
                        rep.Worker = yw;
                        rep.Wlist = new YWorkerList();
                        rep.Wlist.Workers.Add(YMServer.workers.Values);
                        rep.Error = false;
                        return rep;
                    }
                    return rep;
                });
  
                //Console.WriteLine(String.Format("{0} app server is ready on {1}", yw.Wname, yw.IpMax));
                //return Task.FromResult(rep);
            }
            catch (KeyNotFoundException ex)
            {
                Console.WriteLine("Khong tim thay key");
            }
            return Task.FromResult(rep);
        }

        public override Task<YWorkerResponse> StopWorker(WorkerParam request, ServerCallContext context)
        {
            YWorkerResponse rep = new YWorkerResponse();
            rep.Error = true;
            try
            {
                var yw = YMServer.workers[request.Wid];
                var t = YMServer.StopWorker(yw);

                return t.ContinueWith<YWorkerResponse>((tt) =>
                {
                    if (tt.IsCompleted)
                    {
                        rep.Worker = yw;
                        rep.Wlist = new YWorkerList();
                        rep.Wlist.Workers.Add(YMServer.workers.Values);
                        rep.Error = false;
                        rep.Wid = request.Wid;
                        Console.WriteLine(String.Format("{0} is closed on {1}:{2}", yw.Wname, yw.MachineIp, yw.PortMax));
                    }
                    return rep;
                });
            }
            catch (KeyNotFoundException ex)
            {
                Console.WriteLine("Khong tim thay key");
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
            YWorker yw;
            
            if (YMServer.workers.TryRemove(request.Wid, out yw))
            {
                Console.WriteLine(String.Format("{0}({1}:{2}) has been removed automatically when someone exit application!", yw.Wname, yw.MachineIp,yw.PortLoader));
                YMServer.Update2GUI();
                rep.Error = false;
            }
            
            return Task.FromResult(rep);
        }

        public override Task<ResultReply> StopAllWorkers(EmptyParam request, ServerCallContext context)
        {
            YMServer.StopAllWorker();
            Console.WriteLine("All worker are closed");
            return Task.FromResult(new ResultReply());
        }

        public override Task<YWorkerResponse> GiveMeAWorker(EmptyParam request, ServerCallContext context)
        {
            var yw = YMServer.GiveMeAFreeWorker();
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
                    Console.WriteLine("Done");
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
            YMServer.loadWorkers();
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

            var ready =  YMServer.ip_ready(ip);
            var t = ready.ContinueWith<ReadyRespone>((task) =>
            {
                ReadyRespone rp = new ReadyRespone();
                rp.Status = ServingStatus.NotServing;

                if (task.IsCompleted)
                {
                    if (ready.Result)
                    {
                        rp.Status = ServingStatus.Serving;
                    }
                }
                return rp;
            });
            return t;

            //ReadyRespone rp = new ReadyRespone();
            //rp.Status = ServingStatus.NotServing;

            //if (ready.IsCompleted)
            //{
            //    if (ready.Result)
            //    {
            //        rp.Status = ServingStatus.Serving;
            //    }
            //}
            //return Task.FromResult(rp);
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

    };

    class YMServer
    {
        public static YSystem YSys;
        public static YWorker CurrentWorker = null;
        const int StartPort = 38000;
        public static int LastIndex = 0;
        public const string MASTER_IP_DEFAULT = "127.0.0.1";
        public static Server server;
        public static ConcurrentDictionary<int, YWorker> workers = new ConcurrentDictionary<int, YWorker>();

        public static Task<bool> ip_ready(string ip_address, int timeout=3000)
        {
            var channel = new Channel(ip_address, ChannelCredentials.Insecure);
            var t = channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(timeout));

            return t.ContinueWith<bool>(
                (task) =>
                {
                    bool rs = false;
                    if (task.IsFaulted || task.IsCanceled)
                        rs = true;
                    return rs;
                }
            );

            //return (channel.State == ChannelState.Ready);
        }
       
        /// <summary>
        /// Check worker loader
        /// </summary>
        /// <param name="wid"></param>
        /// <param name="timeout"></param>
        /// <returns></returns>
        public static Task<bool> check_worker(int wid, int timeout = 5000)
        {
            var channel = new Channel(workers[wid].MachineIp+":"+workers[wid].PortLoader, ChannelCredentials.Insecure);
            var t = channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(timeout));
           
            return t.ContinueWith<bool>(
                (task) =>
                {
                    bool rs = false;
                    if (task.IsFaulted || task.IsCanceled)
                        rs = true;

                    Console.WriteLine("check_worker " + wid + " : " + task.Status.ToString());
                    var tmp = workers[wid];
                    tmp.NoApp = rs;
                    workers[wid] = tmp;
                    return rs;
                }
            );
        }

        public static Task<int> check_worker(String ip,int wid, int timeout = 5000)
        {
            int portLoader = 38000 + wid;
            var channel = new Channel(ip + ":" + portLoader, ChannelCredentials.Insecure);
            var t = channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(timeout));

            return t.ContinueWith<int>(
                (task) =>
                {
                    int rs = wid;
                    Console.WriteLine("check_worker " + wid + " : " + task.Status.ToString());
                    if (task.IsFaulted || task.IsCanceled)
                        return 0;

                   
                    var tmp = createDefaultWorker(ip, wid);
                    tmp.Wtype = YWorker.Types.WorkerType.Free;
                    tmp.NoApp = false;
                    workers.TryAdd(wid, tmp);                        
                    return rs;
                }
            );
        }

        /// <summary>
        /// Check worker App service
        /// </summary>
        /// <param name="wid"></param>
        /// <param name="timeout"></param>
        /// <returns></returns>
        public static Task<bool> check_worker_service(int wid, int timeout = 10000)
        {
            if (workers[wid].NoApp) return Task.FromResult(false);
            if (workers[wid].Status!=ServingStatus.Serving) return Task.FromResult(false);

            var channel = new Channel(workers[wid].MachineIp + ":" + workers[wid].PortMax, ChannelCredentials.Insecure);
            var t = channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(timeout));

            return t.ContinueWith<bool>(
                (task) =>
                {
                    bool rs = false;
                    if (task.IsFaulted || task.IsCanceled)
                        rs = true;
                    if (!rs)
                    {
                         StartWorker(workers[wid]);
                    }
                    return rs;
                }
            );
        }


        public static Task<int> GiveMeNewID()
        {
            var task = refreshWorkers();
            return task.ContinueWith<int>( (t) =>
            {
                if (t.IsFaulted || t.IsCanceled)
                {
                    System.Threading.Interlocked.Increment(ref YMServer.LastIndex);
                    return YMServer.LastIndex;
                }
                

                foreach (var w in workers.Values)
                {
                    if (w.NoApp) return w.Wid;
                }
                System.Threading.Interlocked.Increment(ref YMServer.LastIndex);
                return YMServer.LastIndex;
            });


            
        }


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
        public static YWorker GetWorker(WorkerParam wp) { 

            if ((wp.WtypeCase == WorkerParam.WtypeOneofCase.Wid))
            {
                try
                {
                    return workers[wp.Wid];
                }
                catch (KeyNotFoundException ex)
                {
                    return null;
                }
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
            tmp.Wtype = YWorker.Types.WorkerType.Free;
            tmp.NoApp = false;
            return tmp;
        }

        public static bool testConnection(String ip, int wid, int range = 38000)
        {
            var channel = new Channel(String.Format(ip + ":{0}", wid + range), ChannelCredentials.Insecure);
            return (!channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(3000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result);
        }

        public static Task<YWorker> GiveMeAFreeWorker()
        {
            foreach (var w in workers.Values)
            {
                if (w.Wtype == YWorker.Types.WorkerType.Free)
                {
                    w.Wtype = YWorker.Types.WorkerType.MainWorker;
                    workers[w.Wid] = w;
                    return Task.FromResult(w);
                }                
            }
            // not found any free worker
            var index = LastIndex + 1;
            Console.WriteLine(String.Format("id:{0}", index));
            int prevCount = workers.Count;
            return refreshMissingWorkers( (index<5) ? 5 : index ).ContinueWith<YWorker>( (t, count) => {
                if (t.IsCanceled || t.IsFaulted) return null;
                if (workers.Count != (int)count)//Check again
                {
                    foreach (var w in workers.Values)
                    {
                        if (w.Wtype == YWorker.Types.WorkerType.Free)
                        {
                            w.Wtype = YWorker.Types.WorkerType.MainWorker;
                            workers[w.Wid] = w;
                            return w;                            
                        }
                    }                    
                }
                return null;
            },prevCount).ContinueWith( tt => {
                if (tt.IsFaulted || tt.IsCanceled || tt.Result==null && StartApp("MAX3D"))
                {
                    var channel = new Channel(String.Format("127.0.0.1:{0}", index + 38000), ChannelCredentials.Insecure);

                    if (channel.State != Grpc.Core.ChannelState.Ready)
                    {
                        return Task.Factory.StartNew(
                            () =>
                            {
                                int count = 0;
                                while (channel.ConnectAsync(deadline: DateTime.UtcNow.AddMilliseconds(5000)).ContinueWith<bool>((t) => t.IsCanceled || t.IsFaulted).Result && count++ < 20)
                                {
                                    Console.WriteLine("5s. Retry");
                                }//*/                   
                            Console.WriteLine("Connected!!!");
                            }
                        ).ContinueWith<YWorker>(_ => {
                            YWorker tmp;
                            if (workers.TryGetValue(index, out tmp))
                            {
                                tmp.Wtype = YWorker.Types.WorkerType.MainWorker;
                                workers[index] = tmp;
                                return tmp;
                            }

                            tmp = createDefaultWorker(MASTER_IP_DEFAULT, index);
                            tmp.Wtype = YWorker.Types.WorkerType.MainWorker;
                            if (YMServer.workers.TryAdd(index, tmp))
                            {
                                return tmp;
                            }

                            return null;
                        });
                    };

                    YWorker rt;
                    if (workers.TryGetValue(index, out rt))
                        return Task.FromResult<YWorker>(rt);
                }
                return tt;
            }).Unwrap();

           

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
            //var path = System.IO.Path.Combine(System.IO.Directory.GetCurrentDirectory(), "ysettings.y3d");
            var wtemp_path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d\\working_workers.y3d");
            if (!File.Exists(wtemp_path))
            {
                System.IO.File.Delete(wtemp_path);
            }

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
        public static Task SaveSystem()
        {
            var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d\\ysettings.y3d");
            using (Stream output = File.OpenWrite(path))
            {
                YSys.WriteTo(output);
            }
            return Task.FromResult(0);
        }
        public static Task saveWorkers()
        {
            Console.WriteLine("Saving all workers...");
            var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d\\working_workers.y3d");
            var wl = new YWorkerList();
            wl.Workers.Add(YMServer.workers.Values);
            using (Stream output = File.OpenWrite(path))
            {
                wl.WriteTo(output);
            }
            return Task.FromResult(0);
        }

        public static Task loadWorkers()
        {
            Console.WriteLine("Loading all workers...");
            var path = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), "y3d\\working_workers.y3d");
            if (File.Exists(path))
            {
                using (Stream file = File.OpenRead(path))
                {
                    var a = Google.Protobuf.CodedInputStream.CreateWithLimits(file, 1024 << 20, 10);
                    YWorkerList wl = YWorkerList.Parser.ParseFrom(a);
                    workers.Clear();
                    foreach (var w in wl.Workers)
                    {
                        workers.TryAdd(w.Wid, w);
                    }
                }
            } else
            {
                Console.WriteLine(String.Format("Can not found : {0}", path));
            }
            Console.WriteLine("Restore status of all workers");
            //refreshWorkers();
            List<YWorker> wlist = new List<YWorker>(workers.Values);
            List<Task> tList = new List<Task>();
            foreach (var w in wlist)
            {
                if (w.Status == ServingStatus.Serving)
                {
                    tList.Add(StartWorker(w));
                }
            }

            return Task.WhenAll(tList);
            //    var wl = new YWorkerList();
            //wl.Workers.Add(YMServer.workers.Values);
            //using (Stream output = File.OpenWrite(path))
            //{
            //    wl.WriteTo(output);
            //}
        }

        public static Task FindLoaderWhenStart()
        {
            // tim cac loader server va add thanh worker khi moi khoi tao
            return Task.FromResult(0);
        }

        public static Task refreshWorkers()
        {
            Console.WriteLine("Recheck status of all workers");
            List<YWorker> wlist = new List<YWorker>(workers.Values);

            var tasks = new List<Task>();

            foreach (var w in wlist)
            {
                tasks.Add(Task.Run(() => check_worker(w.Wid)));
            }
            return Task.WhenAll(tasks);
        }

        public static Task refreshMissingWorkers(int lastIndex)
        {
            Console.WriteLine("Recheck status of all workers");            
            var tasks = new List<Task>();

            for (int wid = 1;wid<= lastIndex;++wid)
            {
                YWorker x;
                if (!workers.TryGetValue(wid,out x) )
                    tasks.Add(Task.Factory.StartNew( o => {
                        return check_worker(MASTER_IP_DEFAULT, (int)o);
                    },wid).Unwrap().ContinueWith( (t) => {
                        if (t.Result > 0)
                        {
                            var tmp = createDefaultWorker(MASTER_IP_DEFAULT, t.Result);
                            workers.TryAdd(t.Result, tmp);
                        }
                 } ) );
            }
            return Task.WhenAll(tasks);
        }


        public static Task StopWorker(YWorker yw, bool stopServerOnly = true)
        {
            Console.WriteLine(String.Format("Stopping {0}..",yw.Wname));
            var client = new YServiceMaxLoader.YServiceMaxLoaderClient(new Channel(yw.MachineIp + ":" + yw.PortLoader, ChannelCredentials.Insecure));
            if (stopServerOnly)
            {
                int wid = yw.Wid;
                var rep = client.ShutdownAsync(new LibInfo());
                return rep.ResponseAsync.ContinueWith( (_,idx) => {
                    var wk = workers[(int)idx];
                    if (wk != null)
                    {              
                        wk.Status = ServingStatus.NotServing;
                        workers[(int)idx] = wk;
                    }
                },wid);                
            }
            else
            {
                return client.CloseAppAsync(new LibInfo()).ResponseAsync.ContinueWith( _ => { });
            }
            //return Task.FromResult(0);
        }
        public static Task StartWorker(YWorker yw, bool startServerOnly = true)
        {           
            var client = new YServiceMaxLoader.YServiceMaxLoaderClient(new Channel(yw.MachineIp + ":" + yw.PortLoader, ChannelCredentials.Insecure));

            if (startServerOnly)
            {
                LibInfo req = new LibInfo();
                req.Id = yw.Wid;
                
                try
                {
                    //var retDll = client.LoadDll(req);
                    int idx = yw.Wid;
                    var r = client.LoadDllAsync(req);
                    return r.ResponseAsync.ContinueWith((t,wid) => {//yw could have changed when get here
                        var o = workers[(int)wid];
                        if (t.IsFaulted || t.IsCanceled)
                        {
                            if (o!=null) Console.WriteLine(String.Format("{0} App Server can not start on {1}", o.Wname, o.MachineIp + ":" + o.PortMax));
                            return;
                        }

                        if (t.IsCompleted && o!=null)
                        {
                            Console.WriteLine(String.Format("{0} App Server is ready on {1}", o.Wname, o.MachineIp + ":" + o.PortMax));                          
                            o.Status = y3d.e.ServingStatus.Serving;
                            workers[(int)wid] = o;
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
      
                //yw.ProcessId = retDll.ProcessId;

                //    YWorkerResponse rep = new YWorkerResponse();
                //    rep.Worker = yw;
                //    rep.Wlist = new YWorkerList();
                //    rep.Wlist.Workers.Add(YMServer.workers.Values);
                //    rep.Error = false;
                //    Console.WriteLine(String.Format("{0} is ready on 127.0.0.1:{1}", yw.Wname, yw.Wid + 39000));
                //    return Task.FromResult(rep);
            }
            else
            {
            }
            return Task.FromResult(0);            

        } 

        public static Task StopAllWorker()
        {
            var t = saveWorkers();
            return t.ContinueWith(_ => {
                Console.WriteLine("Stopping all workers..");
                List<Task> tasks = new List<Task>();
                foreach (var w in workers.Values)
                {
                    if (w.Status==ServingStatus.Serving)
                        tasks.Add(StopWorker(w));
                }
                return Task.WhenAll(tasks);
            }).Unwrap();
          
        }

        public static void Update2GUI()
        {
            Console.WriteLine("Updating worker status to GUI...");
            foreach (var yw in workers.Values)
            {
                Console.WriteLine(String.Format("{0} type is {1}", yw.Wname, yw.Wtype.ToString()));
                if (yw.Wtype!=YWorker.Types.WorkerType.Free)
                {
                    var channel = new Channel(yw.MachineIp + ":" + (yw.Wid + 37000), ChannelCredentials.Insecure);
                    var client = new YServiceMainWorker.YServiceMainWorkerClient(channel);
                    if (client != null)
                    {
                        YWorkerResponse wr = new YWorkerResponse();
                        wr.Worker = yw;
                        wr.Wlist = new YWorkerList();
                        wr.Wlist.Workers.Add(YMServer.workers.Values);
                        client.UpdateWorkersAsync(wr);
                    }
                }
            }
        }

        //public static void forceStopWorker(YWorker yw)
        //{
        //    var s = String.Format("127.0.0.1:{0}", yw.Wid + 39000);
        //    Channel channel = new Channel(s, ChannelCredentials.Insecure);
        //    var toolClient = new y3d.s.YServiceMaxTools.YServiceMaxToolsClient(channel);
        //    toolClient.ShutdownAsync(new EmptyParam());
        //}
        public static void Start()
        {
            server = new Server
            {
                Services = { y3d.s.YServiceMaster.BindService(new YServiceMasterImpl()) },
                Ports = { new ServerPort(YSys.MasterServer.Address, YSys.MasterServer.Port, ServerCredentials.Insecure) }
            };
            server.Start();            
        }

        public static Task Stop()
        {
            return server.ShutdownAsync();
        }
    }
}
