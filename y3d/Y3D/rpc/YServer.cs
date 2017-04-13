using System;
using System.Threading.Tasks;
using Grpc.Core;
using y3d.e;
using System.Windows.Forms;
using System.Windows.Threading;
using System.Diagnostics;
using LogClientCSharp;
using Google.Protobuf;

namespace Y3D.rpc
{

    class YServiceMainWorkerImpl : YServiceMainWorker.YServiceMainWorkerBase
    {
        public override Task<ENone2> UpdateWorkers(Y3D.YWorkerResp request, ServerCallContext context)
        {
            if (YServer.wform!=null)
            {                
                YWorkerResponse req = YWorkerResponse.Parser.ParseFrom(request.ToByteArray());                
                YServer.wform.updateWorkerList(req);
            }
            return Task.FromResult(new ENone2() );
        }


        public override Task<ResponseEvent2> DoEvent(YEvent2 request, ServerCallContext context)
        {
            //MessageBox.Show("da chon:");
            if (request !=null && request.Select!=null)
            {
                LogClient.Instance.LOG("Selected {0}\n",request.Select.Name);
            }
            return Task.FromResult(new ResponseEvent2());
        }
        


    };


    class YServer
    {

        public static Server server=null;
        public static Forms.WorkerForm wform = null;

        static public void myLoading()
        {
            SplashScreen.SplashForm loadingForm = new SplashScreen.SplashForm();
            loadingForm.AppName = "Initializing data";
            loadingForm.Icon = Properties.Resources.wave;
            loadingForm.ShowIcon = true;
            loadingForm.TopMost = true;
            loadingForm.BringToFront();
            //    loadingForm.ShowInTaskbar = true;
            Application.Run(loadingForm);
        }


        public static void Start()
        {
            System.Threading.Thread t = new System.Threading.Thread(new System.Threading.ThreadStart(Projects.Utils.myLoading));
            t.Start();
            Y3D.Projects.Utils.getMainWorker().ContinueWith(
                (task) =>
                {
                        if (Y3D.Projects.Utils.worker!=null)
                        {
                            server = new Server
                            {
                                Services = { YServiceMainWorker.BindService(new YServiceMainWorkerImpl()) },
                                Ports = { new ServerPort("127.0.0.1", Y3D.Projects.Utils.worker.Wid+37000, ServerCredentials.Insecure) }
                            };
                            server.Start();
                            LogClient.Instance.LOG("Y3D Server is now listening on port {0}\n", Y3D.Projects.Utils.worker.Wid + 37000);
                    }
                    //t.Abort();
                }
            ).Wait();
            Y3D.Projects.Utils.mainform = new Forms.YMainForm();
            Y3D.Projects.Utils.mainform.Show();
            while (t.ThreadState == System.Threading.ThreadState.Unstarted)
            {
                System.Threading.Thread.Sleep(1000);
            }
            t.Abort();
        }

        public static Task Stop()
        {
            //MessageBox.Show("bye");
            return server.ShutdownAsync();
        }
    }
}
