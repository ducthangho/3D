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

namespace Y3D.rpc
{
    class YClient
    {


        public static Channel CChannel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
        public static Channel CSChannel = new Channel("127.0.0.1:1983", ChannelCredentials.Insecure);
        public static y3d.s.Tools.ToolsClient CClient = new y3d.s.Tools.ToolsClient(CChannel);
        public static y3d.s.Tools.ToolsClient CSClient = new y3d.s.Tools.ToolsClient(CSChannel);

        public static void StartCSever()
        {
            //CChannel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
            CSChannel = new Channel("127.0.0.1:1983", ChannelCredentials.Insecure);
            //if (CChannel.WaitForStateChangedAsync(ChannelState.))
            //CClient = new y3d.s.Tools.ToolsClient(CChannel);
            CSClient = new y3d.s.Tools.ToolsClient(CSChannel);
        }

        public static void StopCServer()
        {
            CChannel.ShutdownAsync().Wait();
            CSChannel.ShutdownAsync().Wait();
        }
        //[STAThread]
        public static void test1()
        {
            //RenameParam a = new RenameParam();
            //a.UseSelect = false;
            //var reply = CClient.RenameObject(a);
            //MessageBox.Show("Ket qua: " + reply.Message);

            //StringParam s = new StringParam();
            //s.Str = "oh no z";
            //var reply = CSClient.NewProject(s);

            var openFileProject = new System.Windows.Forms.OpenFileDialog();
            DialogResult result = openFileProject.ShowDialog();
            if (result == DialogResult.OK)
            {
                BatchOptimizeParam bp = new BatchOptimizeParam();
                bp.Folder = System.IO.Path.GetDirectoryName(openFileProject.FileName);
                bp.Filename = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
                var reply = CClient.BatchOptimize(bp);
            }

            //BatchOptimizeParam bp = new BatchOptimizeParam();
            //bp.Folder = "ac";
            //bp.Filename = "acac";
            //var reply = CSClient.BatchOptimize(bp);

            //(new Thread(() => {
            //    OptimizeParam op = new OptimizeParam();
            //    op.Ratio = 90;
            //    var reply = CSClient.BatchOptimize(op);
            //})).Start();

            //Make4TestParam m4 = new Make4TestParam();
            //m4.Oname = "Box001";
            //var ret = CSClient.MakeNode4Edit(m4);
        }

        public static void test2()
        {
            //TestParam tp = new TestParam();
            //tp.TestName = "t2";
            //var ret = CSClient.TestTestAsync(tp);

            RenameParam r = new RenameParam();
            var ret = CSClient.RenameObject(r);

            //Make4TestParam m4 = new Make4TestParam();
            //m4.Oname = "Box001";
            //var reply = CClient.MakeNode4Edit(m4);
        }

        public static void test3()
        {
            EmptyParam ep = new EmptyParam();
            var ret = CClient.GetObjectFromMax(ep);
            MessageBox.Show(ret.Areas[0].Name);
            //Make4TestParam m4 = new Make4TestParam();
             //m4.Oname = "Box001";
            //var reply = CClient.MakeNode4Edit(m4);
        }

        public static void test_bake()
        {
            ENormal en = new ENormal();
            Normal3DMax n3d = new Normal3DMax();
            en.Normal3Dmax = n3d;
            var reply = CClient.BakeNormal(en);
        }

        //public static async Task DoYEvent(YEvent ye)
        //{
        //    try
        //    {
        //        using (var call = CClient.DoAction())
        //        {
        //            var responseReaderTask = Task.Run(async () =>
        //            {
        //                while (await call.ResponseStream.MoveNext())
        //                {
        //                    var cye = call.ResponseStream.Current;
        //                    MessageBox.Show(cye.Select.Name);
        //                }
        //            });

        //            await call.RequestStream.WriteAsync(ye);
        //        }
        //    }
        //    catch (RpcException e)
        //    {
        //        throw;
        //    }
        //}


        public static async Task DoClientYEvent(YEvent ye)
        {
            try
            {
                using (var call = CClient.DoStreamClient())
                {
                    await call.RequestStream.WriteAsync(ye);
                    //await call.RequestStream.CompleteAsync();
                }
            }
            catch (RpcException e)
            {
                throw;
            }
        }

        public static ResponseNProject NewProject()
        {
            var openFileProject = new System.Windows.Forms.OpenFileDialog();
            DialogResult result = openFileProject.ShowDialog();
            if (result == DialogResult.OK)
            {
                BatchOptimizeParam bp = new BatchOptimizeParam();
                NewProjectParam np = new NewProjectParam();
                np.Folder = System.IO.Path.GetDirectoryName(openFileProject.FileName);
                np.Fname = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
                return CClient.NewProject(np);
            }
            return null;
        }

    }
}
