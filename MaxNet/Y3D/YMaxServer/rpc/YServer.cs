using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using y3d.e;
using y3d.s;
using System.Windows.Forms;
using System.Threading;

namespace YMaxServer.rpc
{
    class YActionImpl : YAction.YActionBase
    {
        public override Task<ResponseEvent> DoUnwrap(EUnwrap request, ServerCallContext context)
        {
            return base.DoUnwrap(request, context);
        }
    }


    class YToolImpl : y3d.s.Tools.ToolsBase
    {
        //public static void openfile()
        //{
        ////    var openFileProject = new System.Windows.Forms.OpenFileDialog();
        ////    DialogResult result = openFileProject.ShowDialog();
        ////    if (result == DialogResult.OK)
        ////    {
        ////        var oFileName = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
        ////        var oFileDir = System.IO.Path.GetDirectoryName(openFileProject.FileName);
        ////        ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.pre_optimize \"" + oFileDir + "\" \"" + oFileName + "\"");
        ////    }
        ////}

        public override Task<ProjectInfo> NewProject(StringParam request, ServerCallContext context)
        {
            MessageBox.Show(request.Str);
            ProjectInfo a = new ProjectInfo();
            return Task.FromResult(a);
            //return base.NewProject(request, context);
        }

        public override Task<ResultReply> MakeNode4Edit(Make4TestParam request, ServerCallContext context)
        {
            Utils.YOList.make4test(request.Oname);
            ResultReply ret = new ResultReply();
            return Task.FromResult(ret);
        }

        public override Task<ResultReply> BatchOptimize(BatchOptimizeParam request, ServerCallContext context)
        {
            ResultReply ret = new ResultReply();
            Utils.YOList.testz();
            //ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.pre_optimize \"" + request.Folder + "\" \"" + request.Filename + "\"");
            return Task.FromResult(ret);
        }

        public override Task<ResultReply> TestTest(TestParam request, ServerCallContext context)
        {
            ResultReply ret = new ResultReply();
            MessageBox.Show(request.TestName);
            ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.exportObjectToFBX \"11.fbx\" \"D:\\\"");
            return Task.FromResult(ret);
        }


        //public override Task<ResultReply> BatchOptimize(OptimizeParam request, ServerCallContext context)
        //{

        //    //ResultReply ret = new ResultReply();
        //    //if (Loader.Core.CurFileName.Length > 0) // da load file max
        //    //{
        //    //    MessageBox.Show("o la la"); // lam sau4
        //    //}
        //    //else
        //    //{
        //    //    Thread t = new Thread(openfile);
        //    //    t.SetApartmentState(ApartmentState.STA);
        //    //    t.Start();
        //    //    Thread.Sleep(500);
        //    //    //(new System.Threading.Thread(() =>
        //    //    //{
        //    //    //    var openFileProject = new System.Windows.Forms.OpenFileDialog();
        //    //    //    //Form1 aa = new Form1();
        //    //    //    //aa.ShowDialog();
        //    //    //    DialogResult result = openFileProject.ShowDialog();
        //    //    //})).Start();


        //    //    //if (result == DialogResult.OK)
        //    //    //{
        //    //    //    var oFileName = System.IO.Path.GetFileNameWithoutExtension(openFileProject.FileName);
        //    //    //    var oFileDir = System.IO.Path.GetDirectoryName(openFileProject.FileName);
        //    //    //    //ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.pre_optimize \"" + oFileDir + "\" \"" + oFileName + "\"");
        //    //    //}
        //    }
        //    ResultReply ret = new ResultReply();
        //    return Task.FromResult(ret);
        //    //return base.BatchOptimize(request, context);
        //}

        public override Task<ProjectInfo> LoadProject(StringParam request, ServerCallContext context)
        {
            return base.LoadProject(request, context);
        }
    }

    class YServer
    {
        const int Port = 1983;
        public static Server server;
        public static void Start()
        {
            server = new Server
            {
                Services = { YAction.BindService(new YActionImpl()), y3d.s.Tools.BindService(new YToolImpl()) },
                Ports = { new ServerPort("localhost", Port, ServerCredentials.Insecure) }
            };
            server.Start();
        }

        public static void Stop()
        {
            MessageBox.Show("bye");
            server.ShutdownAsync().Wait();
        }
    }
}
