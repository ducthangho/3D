﻿using System;
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

        public override Task<ResponseEvent> DoEvent(YEvent request, ServerCallContext context)
        {
            if (request.Select!=null)
            {
                MessageBox.Show("da chon:" + request.Select.Name);
            }
            return Task.FromResult(new ResponseEvent());
        }
    };


    class YServer
    {
        public static Server server=null;
        public static Forms.WorkerForm wform = null;
        public static Task Start()
        {
            return Y3D.Projects.Utils.getMainWorker().ContinueWith(
                (task) =>
                {
                        if (Y3D.Projects.Utils.worker!=null)
                        {
                            server = new Server
                            {
                                Services = { y3d.s.YServiceMainWorker.BindService(new YServiceMainWorkerImpl()) },
                                Ports = { new ServerPort("127.0.0.1", Y3D.Projects.Utils.worker.Wid+37000, ServerCredentials.Insecure) }
                            };
                            server.Start();
                        }
                }
            );
        }

        public static Task Stop()
        {
            //MessageBox.Show("bye");
            return server.ShutdownAsync();
        }
    }
}