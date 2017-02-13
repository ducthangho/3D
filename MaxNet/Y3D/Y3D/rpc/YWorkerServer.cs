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
    class YWorkerServer
    {
        const int Port = 38000;
        public static Server server;
        public static void Start()
        {
            //server = new Server
            //{
            //    Services = { y3d.s.YSWorkers.BindService(new YWorkerImpl()) },
            //    Ports = { new ServerPort("127.0.0.1", Port, ServerCredentials.Insecure) }
            //};
            //server.Start();
        }

        public static void Stop()
        {
            //MessageBox.Show("bye");
            server.ShutdownAsync().Wait();
        }
    }
}
