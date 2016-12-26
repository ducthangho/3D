using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using System.Windows.Forms;
using Y3D.YService;
using System.Threading;

namespace YTest.rpc
{
    class YClient
    {
        public static void test3()
        {
                MessageBox.Show("aa");
                Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
                var client = new Y3D.YService.Tools.ToolsClient(channel);
                RenameParam a = new RenameParam();
                a.UseSelect = false;
                //var reply = client.SayHello(new HelloRequest { Name = "zun" });
                var reply = client.RenameObject(a);

                MessageBox.Show("Ket qua: " + reply.Message);
                channel.ShutdownAsync().Wait();
        }
    }
}
