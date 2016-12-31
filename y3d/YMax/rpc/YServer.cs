using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using Helloworld;
using Y3D.Entities;
using Y3D.YService;
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

    class YActionImpl : YAction.YActionBase
    {
        public override async Task<ResponseEvent> DoAction(IAsyncStreamReader<Event> requestStream, ServerCallContext context)
        {
            while (await requestStream.MoveNext())
            {
                var ev =  requestStream.Current;
                if (ev.Select!=null)
                {
                    MessageBox.Show(ev.Select.Name);
                } else if (ev.Move!=null)
                {
                    MessageBox.Show("sac");
                }
            }
            return new ResponseEvent();
            //return base.DoAction(requestStream, context);
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
                Services = { Greeter.BindService(new GreeterImpl()), YAction.BindService(new YActionImpl()) },
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
