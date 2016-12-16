using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using Helloworld;
using System.Windows.Forms;
using Y3D.YService;
using System.Threading;

namespace YMax.rpc
{
    class YClient
    {
        public static void test()
        {
            Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
            var client = new Greeter.GreeterClient(channel);

            var reply = client.SayHello(new HelloRequest { Name = "zun" });

            MessageBox.Show("Greeting: " + reply.Message);
            channel.ShutdownAsync().Wait();
        }

        public static void test2()
        {
            Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
            var client = new yproto.YPrepare.YPrepareClient(channel);

            //var reply = client.SayHello(new HelloRequest { Name = "zun" });
            yproto.NumFaceRange a = new yproto.NumFaceRange();
            a.F.Add(1);
            a.F.Add(22);
            a.F.Add(4);
            var reply = client.MakeBox(a);

            MessageBox.Show("Greeting: " + reply.Objs.Count.ToString());
            channel.ShutdownAsync().Wait();
        }

        public static void test3()
        {
            (new Thread(() => {
                Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
                var client = new Y3D.YService.Tools.ToolsClient(channel);
                RenameParam a = new RenameParam();
                a.UseSelect = false;
                //var reply = client.SayHello(new HelloRequest { Name = "zun" });
                var reply = client.RenameObject(a);

                MessageBox.Show("Ket qua: " + reply.Message);
                channel.ShutdownAsync().Wait();
            })).Start();
        }
    }
}
