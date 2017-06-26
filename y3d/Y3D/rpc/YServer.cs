using System;
using System.Threading.Tasks;
using Grpc.Core;
using y3d.e;
using System.Windows.Forms;
using System.Windows.Threading;
using System.Diagnostics;
using LogClientCSharp;
using Google.Protobuf;
using System.Reactive.Linq;
using System.Reactive.Disposables;

using System.Reactive.Threading.Tasks;
using System.Reactive.Concurrency;
using System.Threading;

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
            var rs = new ResponseEvent2();
            if (request != null)
            {
                if (request.Select != null)
                {
                    //MessageBox.Show("Selected "+request.Select.Name);
                    LogClient.Instance.LOG("Selected {0}\n", request.Select.Name);

                    rs.Msg = request.Select.Name + "....";
                }
                else if (request.SelectMany != null)
                {
                    var items = request.SelectMany;
                    String s = "";
                    for (int i = 0; i < items.Name.Count; ++i)
                    {
                        var it = items.Name[i];
                        s += it + " ; ";
                    }
                    rs.Msg = s;
                }
            }
            

            return Task.FromResult(rs);
        }
        


    };


    class YServer
    {

        public static Server server=null;
        public static Forms.WorkerForm wform = null;
        

        public static void Start()
        {
            //Projects.Utils.Store.Dispatch(new YFlow.SetBusyAction { isBusy = true });

            //System.Threading.Thread t = new System.Threading.Thread(new System.Threading.ThreadStart(Projects.Utils.myLoading(Projects.Utils.Store)));
            var tokenSource = new CancellationTokenSource();
            var token = tokenSource.Token;

            System.Threading.Thread t = new System.Threading.Thread(new System.Threading.ThreadStart(() =>
            {
                Projects.Utils.myLoading(token);
            }
            ));
            t.Start();

            Y3D.Projects.Utils.getMainWorker().ContinueWith(
                (task) =>
                {
                    if (Y3D.Projects.Utils.worker != null)
                    {
                        server = new Server
                        {
                            Services = { YServiceMainWorker.BindService(new YServiceMainWorkerImpl()) },
                            Ports = { new ServerPort("127.0.0.1", Y3D.Projects.Utils.worker.Wid + 37000, ServerCredentials.Insecure) }
                        };
                        server.Start();
                        LogClient.Instance.LOG("Y3D Server is now listening on port {0}\n", Y3D.Projects.Utils.worker.Wid + 37000);
                    }
                    //t.Abort();
                }).Wait();

            //Projects.Utils.Store.Dispatch(new YFlow.SetBusyAction { isBusy = false });

            Y3D.Projects.Utils.mainform = new Forms.YMainForm();
            Y3D.Projects.Utils.mainform.Show();
            tokenSource.Cancel();
            //try
            //{
            //    t.Abort();
            //}
            //catch (System.Threading.ThreadAbortException)
            //{

            //}



            //Task done = Task.Factory.StartNew (() => {
            //    return Y3D.Projects.Utils.getMainWorker().ContinueWith(
            //    (task) =>
            //    {
            //        if (Y3D.Projects.Utils.worker != null)
            //        {
            //            server = new Server
            //            {
            //                Services = { YServiceMainWorker.BindService(new YServiceMainWorkerImpl()) },
            //                Ports = { new ServerPort("127.0.0.1", Y3D.Projects.Utils.worker.Wid + 37000, ServerCredentials.Insecure) }
            //            };
            //            server.Start();
            //            LogClient.Instance.LOG("Y3D Server is now listening on port {0}\n", Y3D.aProjects.Utils.worker.Wid + 37000);
            //        }
            //        //t.Abort();
            //    });


            //} );
            //while (done.Status == TaskStatus.WaitingForActivation) { };
            //LogClient.Instance.LOG("State = "+done.IsCompleted.ToString()+"     "+done.Status.ToString()+"\n");

            //System.Threading.Thread t = new System.Threading.Thread(new System.Threading.ThreadStart(() =>
            //{
            //    Projects.Utils.myLoading(done);
            //}
            //));


            //t.Start();
            //done.Wait();

            //IQbservable<int> ob = Observable.Create<int>(o =>
            //{
            //    var cancel = new CancellationDisposable(); // internally creates a new CancellationTokenSource
            //    //Projects.Utils.myLoading();

            //    //while (!cancel.Token.IsCancellationRequested)
            //    //{

            //    //}
            //    Application.ExitThread();
            //    return cancel;
            //});

            //IObservable<bool> loadding = Projects.Utils.myLoading().ToObservable();


            //if (t.IsAlive || t.ThreadState==System.Threading.ThreadState.Running) t.Abort();

            //while (t.ThreadState == System.Threading.ThreadState.Unstarted)
            //{
            //    System.Threading.Thread.Sleep(1000);
            //}
            //System.Threading.Thread.Sleep(2000);
            //try
            //{
            //    while (t.ThreadState == System.Threading.ThreadState.Unstarted || !t.IsAlive)
            //    {
            //        System.Threading.Thread.Sleep(1000);
            //    }
            //    if (t.IsAlive && t.ThreadState==System.Threading.ThreadState.Running) t.Abort();
            //}
            //catch (System.Threading.ThreadAbortException)
            //{
            //    MessageBox.Show("ac");
            //}

        }

        public static Task Stop()
        {
            //MessageBox.Show("bye");
            return server.ShutdownAsync();
        }
    }
}
