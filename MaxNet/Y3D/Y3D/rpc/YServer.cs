using System;
using System.Threading.Tasks;
using Grpc.Core;
using y3d.e;
using y3d.s;
using System.Windows.Forms;
using System.Windows.Threading;
using System.Diagnostics;

namespace Y3D.rpc
{

    class YServiceMainWorkerImpl : y3d.s.YServiceMainWorker.YServiceMainWorkerBase
    {
        public override Task<EmptyParam> UpdateWorkers(YWorkerResponse request, ServerCallContext context)
        {
            if (YServer.wform!=null)
            {
                YServer.wform.updateWorkerList(request);
            }
            return Task.FromResult(new EmptyParam());
        }
    };


    class YServer
    {
        public static Server server=null;
        public static Forms.WorkerForm wform = null;
        public static void Start()
        {
            var x = Utils.MainWorker.getMainWorker().ContinueWith(
                (task) =>
                {
                    if (task.IsCompleted)
                    {
                        if (Utils.MainWorker.worker!=null)
                        {
                            server = new Server
                            {
                                Services = { y3d.s.YServiceMainWorker.BindService(new YServiceMainWorkerImpl()) },
                                Ports = { new ServerPort("0.0.0.0", Utils.MainWorker.worker.Wid+37000, ServerCredentials.Insecure) }
                            };
                            server.Start();
                        }
                    }
                }
            );

        }

        public static void Stop()
        {
            //MessageBox.Show("bye");
            server.ShutdownAsync().Wait();
        }
    }
}
