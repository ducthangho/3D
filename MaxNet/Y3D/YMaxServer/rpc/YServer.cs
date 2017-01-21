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
using System.Windows.Threading;
using System.Diagnostics;

namespace YMaxServer.rpc
{
    public class ResultFunc<R>
    {
        public R result;        
        public void Exec(Func<R> func)
        {
            result = func();
        }
    };

    public class DialogState

    {

        public DialogResult result;

        public FileDialog dialog;

        public void ThreadProcShowDialog()

        {

            result = dialog.ShowDialog();

        }

    }

    class YToolImpl : y3d.s.Tools.ToolsBase
    {
        static Dispatcher dispatcher;
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

        public void Invoke(Action a)
        {
            dispatcher = Dispatcher.CurrentDispatcher;
            if (dispatcher != null) dispatcher.Invoke(a);
        }

        //This function is used to execute thread which require STA
        //e.g. System.Windows.Forms.OpenFileDialog();
        //See TestTest() for more example
        public R STAFunc<R>(Func<R> func)
        {
            ResultFunc<R> state = new ResultFunc<R>();
            System.Threading.Thread t = new System.Threading.Thread(() => state.Exec(func));
            t.SetApartmentState(System.Threading.ApartmentState.STA);
            t.Start();
            t.Join();
            return state.result;
        }


        public override Task<ProjectInfo> NewProject(StringParam request, ServerCallContext context)
        {
            MessageBox.Show(request.Str);
            ProjectInfo a = new ProjectInfo();
            return Task.FromResult(a);
            //return base.NewProject(request, context);
        }

        public override Task<ResultReply> MakeNode4Edit(Make4TestParam request, ServerCallContext context)
        {
           
            Invoke(() =>
            {
                try
                {
                    Utils.YOList.make4test(request.Oname);
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
            });
            
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

        //public DialogResult STAShowDialog(FileDialog dialog)
        //{
        //    DialogState state = new DialogState();
        //    state.dialog = dialog;
        //    System.Threading.Thread t = new System.Threading.Thread(state.ThreadProcShowDialog);
        //    t.SetApartmentState(System.Threading.ApartmentState.STA);
        //    t.Start();
        //    t.Join();
        //    return state.result;
        //}

        //[STAThread]
        public override Task<ResultReply> TestTest(TestParam request, ServerCallContext context)
        {
            ResultReply ret = new ResultReply();
            dispatcher = Dispatcher.CurrentDispatcher;

            Invoke(() =>
            {
                try
                {
                    //MessageBox.Show(request.TestName);
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("print \"I love maxscript\"");

                    //System.Threading.Thread.CurrentThread.SetApartmentState(ApartmentState.STA);

                    DialogResult result = STAFunc<DialogResult>( () => //These code will be executed in a STAthread before returning the result
                    {
                        var diag = new System.Windows.Forms.OpenFileDialog(); 
                        return diag.ShowDialog();
                    });

                   

                    if (result == DialogResult.OK)
                    {
                        ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("print \"OK\"");
                    }
                }
                catch (Exception e)
                {
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("print \"" + e.Message + "\"");
                    Debug.WriteLine(e.Message);
                }
            });            
            
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
                Services = { y3d.s.Tools.BindService(new YToolImpl()) },
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
