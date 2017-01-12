using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using Helloworld;
using System.Windows.Forms;
using y3d.s;
using y3d.e;
using System.Threading;

namespace YMax.rpc
{
    class YClient
    {
        public static Channel CChannel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
        public static y3d.s.Tools.ToolsClient CClient = new y3d.s.Tools.ToolsClient(CChannel);

        public static void StartCSever()
        {
            CChannel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
            CClient = new y3d.s.Tools.ToolsClient(CChannel);
        }

        public static void StopCServer()
        {
            CChannel.ShutdownAsync().Wait();
        }

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
                var client = new y3d.s.Tools.ToolsClient(channel);
                RenameParam a = new RenameParam();
                a.UseSelect = false;
                //var reply = client.SayHello(new HelloRequest { Name = "zun" });
                var reply = client.RenameObject(a);

                MessageBox.Show("Ket qua: " + reply.Message);
                channel.ShutdownAsync().Wait();
            })).Start();
        }

        public static void test4()
        {
            Channel channel = new Channel("127.0.0.1:1983", ChannelCredentials.Insecure);
            var client = new y3d.s.Tools.ToolsClient(channel);
            BatchOptimizeParam op = new BatchOptimizeParam();
            op.Ratio = 90;
            var reply = client.BatchOptimizeAsync(op);
            channel.ShutdownAsync().Wait();
        }
    }
}
