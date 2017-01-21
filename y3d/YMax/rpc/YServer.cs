using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using Helloworld;
using y3d.e;
using y3d.s;
using System.Windows.Forms;

namespace YMax.rpc
{
    class GreeterImpl : Greeter.GreeterBase
    {
        public override Task<HelloReply> SayHello(HelloRequest request, ServerCallContext context)
        {
            return Task.FromResult(new HelloReply { Message = "Hello " + request.Name });
        }
    }

    class YToolImpl: y3d.s.Tools.ToolsBase
    {
        //public override Task<ProjectInfo> NewProject(NewProjectParam request, ServerCallContext context)
        //{
        //    return base.NewProject(request, context);
        //}

        public override Task<ProjectInfo> LoadProject(StringParam request, ServerCallContext context)
        {

            return base.LoadProject(request, context);
        }
    }
    
    class YServer
    {
        const int Port = 50051;
        public static Server server;
        public static void Start()
        {
            server = new Server
            {
                //Services = { Greeter.BindService(new GreeterImpl()), YAction.BindService(new YActionImpl()) },
                Services = { y3d.s.Tools.BindService(new YToolImpl()) },
                Ports = { new ServerPort("localhost", Port, ServerCredentials.Insecure) }
            };
            server.Start();
        }

        public static void Stop()
        {
            server.ShutdownAsync().Wait();
        }
    }
}
