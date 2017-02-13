using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using y3d.s;
using Google.Protobuf;
using Grpc.Core;
using System.Diagnostics;

namespace YMasterServer
{

    class YWorkerImpl : y3d.s.YSWorkers.YSWorkersBase
    {
        public override Task<YWorkerResponse> AddWorker(YWorkerRequest req, ServerCallContext context)
        {
            YWorkerResponse ret = new YWorkerResponse();
            ret.NewWorker = new YWorker();
            ret.NewWorker.Wid = YMServer.YSys.MasterServer.MainWorkers.Count + 1;
            ret.NewWorker.IpAddress = String.Format("127.0.0.1:{0}", ret.NewWorker.Wid + 8000);
            ret.NewWorker.Wname = "Worker " + ret.NewWorker.Wid;
            ret.NewWorker.Status = YWorker.Types.ServingStatus.Unknown;
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
        const int StartPort = 8000;
        const string MASTER_IP_DEFAULT = "127.0.0.1";
        public static Server server;
    }
}
