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

        [DllImport("kernel32.dll")]
        private static extern int FormatMessage(int dwFlags, IntPtr lpSource, int dwMessageId, int dwLanguageId, out string lpBuffer, int nSize, IntPtr pArguments);

        public static string GetErrorMessage(int errorCode)
        {
            const int FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x00000100;
            const int FORMAT_MESSAGE_IGNORE_INSERTS = 0x00000200;
            const int FORMAT_MESSAGE_FROM_SYSTEM = 0x00001000;

            string lpMsgBuf;
            int dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER
                | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

            int retVal = FormatMessage(dwFlags, IntPtr.Zero, errorCode, 0,
                                        out lpMsgBuf, 0, IntPtr.Zero);
            if (0 == retVal)
            {
                return null;
            }
            return lpMsgBuf;
        }
    }


    class YLoaderImpl: Y3D.Loader.LoaderBase
    {

        public YLoaderImpl()
        {            
        }

        public const string SERVICE_DLL_PATH = @"C:\Program Files\Autodesk\3ds Max 2017\Service.dll";

        [UnmanagedFunctionPointer(CallingConvention.Winapi)]
        private delegate void StartService(string dllname = "ServiceImpl.dll");

        [UnmanagedFunctionPointer(CallingConvention.Winapi)]
        private delegate void StopService();

        public string handleError(String msg,int error_code)
        {
            string s = "";
            if (error_code != 0)
            {
                s = msg + "\nError no: " + error_code + " - " + NativeMethods.GetErrorMessage(error_code);
                MessageBox.Show(s);
            }
            return s;
        }

        public string handleError(String msg)
        {            
            var error_code = Marshal.GetLastWin32Error();            
            return handleError(msg,error_code);
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
                rs.Message = handleError("Failed to get address of function stopService().");
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
                rs.Message = handleError("Failed to unload plugin.");               
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
                rs.Message = handleError("Failed to load library "+ SERVICE_DLL_PATH + ".");
                rs.Error = true;
                return Task.FromResult(rs);
            };
                

            IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "startService");
            //oh dear, error handling here
            if(pAddressOfFunctionToCall == IntPtr.Zero)
            {                
                rs.Message = handleError("Failed to load function startService.");
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
