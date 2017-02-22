using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Grpc.Core;
using y3d.e;
using y3d.s;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace YMaxServer.rpc
{
    static class NativeMethods
    {
        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary")]
        public static extern int LoadLibrary(
            [MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        //[DllImport("kernel32.dll", EntryPoint = "GetProcAddress")]
        //public static extern IntPtr GetProcAddress(int hModule,
        //    [MarshalAs(UnmanagedType.LPStr)] string lpProcName);
        [DllImport("kernel32.dll", EntryPoint = "GetProcAddress", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
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

    class YServiceMaxLoaderImpl : YServiceMaxLoader.YServiceMaxLoaderBase
    {

        public YServiceMaxLoaderImpl()
        {
        }

        public const string SERVICE_DLL_PATH = @"C:\Program Files\Autodesk\3ds Max 2017\Service.dll";

        [UnmanagedFunctionPointer(CallingConvention.Winapi)]
        private delegate void StartService(string dllname = "ServiceImpl.dll", string ip_address="127.0.0.1:39001");

        [UnmanagedFunctionPointer(CallingConvention.Winapi)]
        private delegate void StopService();

        public string handleError(String msg, int error_code)
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
            return handleError(msg, error_code);
        }

        public void init()
        {
            if (pDll == 0)
                pDll = NativeMethods.LoadLibrary(SERVICE_DLL_PATH);

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

        public override Task<ResultType> Shutdown(LibInfo request, ServerCallContext context)
        {
                var rs = new ResultType();
                if (pDll == 0)
                    return Task.FromResult(rs);
                IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "stopService");
                //oh dear, error handling here
                if (pAddressOfFunctionToCall == IntPtr.Zero)
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

        public override Task<ResultType> CloseApp(LibInfo request, ServerCallContext context)
        {
            //Loader.Global.
            var p = System.Diagnostics.Process.GetCurrentProcess();
            p.Close();
            return base.CloseApp(request, context);
        }


        //void myworker(StartService startService)
        //{
        //    if (startService != null) startService();
        //}


        public override Task<ResultType> LoadDll(LibInfo request, ServerCallContext context)
        {
            var rs = new ResultType();
            rs.ProcessId = System.Diagnostics.Process.GetCurrentProcess().Id;
            rs.Error = false;
            
            if (pDll == 0)
                pDll = NativeMethods.LoadLibrary(SERVICE_DLL_PATH);
            //oh dear, error handling here
            //if (pDll == IntPtr.Zero)
            //
            if (pDll == 0)
            {
                rs.Message = handleError("Failed to load library " + SERVICE_DLL_PATH + ".");
                rs.Error = true;
                return Task.FromResult(rs);
            };


            IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "startService");
            
            //oh dear, error handling here
            if (pAddressOfFunctionToCall == IntPtr.Zero)
            {
                rs.Message = handleError("Failed to load function startService.");
                rs.Error = true;
                return Task.FromResult(rs);
            };


            StartService startService = (StartService)Marshal.GetDelegateForFunctionPointer(
                                                                                    pAddressOfFunctionToCall,
                                                                                    typeof(StartService));
            //MessageBox.Show(pAddressOfFunctionToCall.ToString());
            //var ip_addr = String.Format("127.0.0.1:{0}", 39000 + YLoaderServer.worker_id);
            //MessageBox.Show(YLoaderServer.worker.IpMax);
            //if (startService != null) startService("ServiceImpl.dll", (YLoaderServer.worker.MachineIp+":"+YLoaderServer.worker.PortMax));
            if (startService != null) startService("ServiceImpl.dll", "0.0.0.0:" + YLoaderServer.worker.PortMax);

            //Channel channel = new Channel("0.0.0.0:39001", ChannelCredentials.Insecure);
            //y3d.s.Tools.ToolsClient toolClient = new y3d.s.Tools.ToolsClient(channel);
            //var re = toolClient.CloneObject(new EmptyParam());

            return Task.FromResult(rs);

        }

        private int pDll = 0;
    }

    class YLoaderServer
    {
        public const string MASTER_IP = "127.0.0.1:38000";
        public static YWorker worker = null;
        public static Server server;
        public static void Start()
        {
            var MasterClient = new y3d.s.YServiceMaster.YServiceMasterClient(new Channel(MASTER_IP, ChannelCredentials.Insecure));
            YWorkerRequest req = new YWorkerRequest();
            req.CallInApp = true;

            req.Machine = new YMachine();
            req.Machine.IpAddress = "127.0.0.1";
            req.Machine.Mname = "";

            var rep = MasterClient.AddWorker(req);
            if (rep == null) return;
            //worker_id = rep.Worker.Wid;
            worker = rep.Worker.Clone();
            server = new Server
            {
                Services = { YServiceMaxLoader.BindService(new YServiceMaxLoaderImpl()) },
                Ports = { new ServerPort("localhost", rep.Worker.Wid + 38000, ServerCredentials.Insecure) }
            };
            server.Start();
        }

        public static void Stop()
        {
            server.ShutdownAsync().Wait();
        }
    }
}
