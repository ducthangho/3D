using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using Helloworld;
using y3d.e;
using y3d.s;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace YMax.rpc
{

    class GreeterImpl : Greeter.GreeterBase
    {
        public override Task<HelloReply> SayHello(HelloRequest request, ServerCallContext context)
        {
            return Task.FromResult(new HelloReply { Message = "Hello " + request.Name });
        }
    }

    class YToolImpl: y3d.s.Tools.ToolsBase
    {
        //public override Task<ProjectInfo> NewProject(NewProjectParam request, ServerCallContext context)
        //{
        //    return base.NewProject(request, context);
        //}

        //public override Task<ProjectInfo> LoadProject(StringParam request, ServerCallContext context)
        //{

        //    return base.LoadProject(request, context);
        //}
    }

    static class NativeMethods
    {
        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary")]
        public static extern int LoadLibrary(
            [MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        [DllImport("kernel32.dll", EntryPoint = "GetProcAddress")]
        public static extern IntPtr GetProcAddress(int hModule,
            [MarshalAs(UnmanagedType.LPStr)] string lpProcName);

        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary")]
        public static extern bool FreeLibrary(int hModule);
    }


    class YLoaderImpl: Y3D.Loader.LoaderBase
    {

        public YLoaderImpl()
        {            
        }

        public const string SERVICE_DLL_PATH = @"C:\Program Files\Autodesk\3ds Max 2017\Plugins\Service.dll";

        [UnmanagedFunctionPointer(CallingConvention.Winapi)]
        private delegate void StartService(string dllname = "ServiceImpl.dll");

        [UnmanagedFunctionPointer(CallingConvention.Winapi)]
        private delegate void StopService();

        public void init()
        {
            if (pDll == 0)
                pDll = NativeMethods.LoadLibrary(SERVICE_DLL_PATH);
            //oh dear, error handling here
            //if (pDll == IntPtr.Zero)

            if (pDll == 0)
            {
                MessageBox.Show("Failed to load plugin " + SERVICE_DLL_PATH);              
                return;
            };


            IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "startService");
            //oh dear, error handling here
            if (pAddressOfFunctionToCall == IntPtr.Zero)
            {
                MessageBox.Show("Failed to load function startService");               
                return;
            };


            StartService startService = (StartService)Marshal.GetDelegateForFunctionPointer(
                                                                                    pAddressOfFunctionToCall,
                                                                                    typeof(StartService));
            if (startService != null) startService();
        }

        public override global::System.Threading.Tasks.Task<global::Y3D.ResultType> Shutdown(global::Y3D.LibInfo request, ServerCallContext context)
        {
            var rs = new Y3D.ResultType();
            if (pDll == 0)
                return Task.FromResult(rs);
            IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "stopService");
            //oh dear, error handling here
            if(pAddressOfFunctionToCall == IntPtr.Zero)
            {
                MessageBox.Show("Failed to load function stopService");
                NativeMethods.FreeLibrary(pDll);
                rs.Error = true;
                return Task.FromResult(rs);
            };

            StopService stopService = (StopService)Marshal.GetDelegateForFunctionPointer(
                                                                                    pAddressOfFunctionToCall,
                                                                                    typeof(StopService));
            if (stopService != null) stopService();
            bool result = NativeMethods.FreeLibrary(pDll);
            pDll = 0;
            if (!result)
            {
                MessageBox.Show("Failed to unload plugin");               
                rs.Error = true;
                return Task.FromResult(rs);
            };

            return Task.FromResult(rs);
        }
        

        public override Task<Y3D.ResultType> LoadDll(Y3D.LibInfo request, ServerCallContext context)
        {
            var rs = new Y3D.ResultType();
            rs.Error = false;
            if (pDll==0)
                pDll = NativeMethods.LoadLibrary(SERVICE_DLL_PATH);
            //oh dear, error handling here
            //if (pDll == IntPtr.Zero)

            if (pDll == 0) {
                MessageBox.Show("Failed to load plugin " + SERVICE_DLL_PATH);
                rs.Error = true;
                return Task.FromResult(rs);
            };
                

            IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "startService");
            //oh dear, error handling here
            if(pAddressOfFunctionToCall == IntPtr.Zero)
            {
                MessageBox.Show("Failed to load function startService");
                rs.Error = true;
                return Task.FromResult(rs);
            };


            StartService startService = (StartService)Marshal.GetDelegateForFunctionPointer(
                                                                                    pAddressOfFunctionToCall,
                                                                                    typeof(StartService));
            if (startService != null) startService();
            
            return Task.FromResult(rs);
        }

        private int pDll = 0;
    }

    class YLoaderServer
    {
        const int Port = 50050;
        public static Server server;
        public static void Start()
        {           
            server = new Server
            {
                //Services = { Greeter.BindService(new GreeterImpl()), YAction.BindService(new YActionImpl()) },            

                Services = { Y3D.Loader.BindService(new YLoaderImpl()) },
                Ports = { new ServerPort("localhost", Port, ServerCredentials.Insecure) }
            };            
            server.Start();
        }

        public static void Stop()
        {
            server.ShutdownAsync().Wait();
        }
    }

    class YServer
    {
        const int Port = 50051;
        public static Server server;
        public static void Start()
        {
            server = new Server
            {
                //Services = { Greeter.BindService(new GreeterImpl()), YAction.BindService(new YActionImpl()) },
                Services = { y3d.s.Tools.BindService(new YToolImpl()) },
                Ports = { new ServerPort("localhost", Port, ServerCredentials.Insecure) }
            };
            server.Start();
        }

        public static void Stop()
        {
            server.ShutdownAsync().Wait();
        }
    }
}
