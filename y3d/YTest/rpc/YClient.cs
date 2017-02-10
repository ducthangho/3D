using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using System.Windows.Forms;
using y3d.s;
using y3d.e;
using System.Threading;

namespace YTest.rpc
{
    class YClient
    {
        public static Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
        public static Tools.ToolsClient YTools = new Tools.ToolsClient(channel);

        public static void test3()
        {
                RenameParam a = new RenameParam();
                a.UseSelect = false;

                
                var reply = YTools.RenameObject(a);
                MessageBox.Show("Ket qua: " + reply.Message);
                //channel.ShutdownAsync().Wait();
        }

        public static void testNewP()
        {
            StringParam s = new StringParam();
            s.Str = "My Project 1";
            var reply = YTools.NewProject(new NewProjectParam());
            MessageBox.Show(reply.PInfo.ProjectPath);
        }
    }
}
