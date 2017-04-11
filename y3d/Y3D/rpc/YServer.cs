using System;
using System.Threading.Tasks;
using Grpc.Core;
using y3d.e;
using y3d.s;
using System.Windows.Forms;
using System.Windows.Threading;
using System.Diagnostics;

namespace Y3D.rpc
{

    class YServiceMainWorkerImpl : y3d.s.YServiceMainWorker.YServiceMainWorkerBase
    {
        public override Task<EmptyParam> UpdateWorkers(YWorkerResponse request, ServerCallContext context)
        {
            if (YServer.wform!=null)
            {
                YServer.wform.updateWorkerList(request);
            }
            return Task.FromResult(new EmptyParam());
        }

        public override Task<ResponseEvent> DoEvent(YEvent request, ServerCallContext context)
        {
            MessageBox.Show("da chon:");
            if (request.Select!=null)
            {
                MessageBox.Show("da chon:" + request.Select.Name);
            }
            return Task.FromResult(new ResponseEvent());
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
                                Services = { y3d.s.YServiceMainWorker.BindService(new YServiceMainWorkerImpl()) },
                                Ports = { new ServerPort("127.0.0.1", Y3D.Projects.Utils.worker.Wid+37000, ServerCredentials.Insecure) }
                            };
                            server.Start();
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
