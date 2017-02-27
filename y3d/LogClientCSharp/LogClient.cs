using Grpc.Core;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Y3D;

namespace LogClientCSharp
{
    public class LogClient
    {
        //
        private const string LOGSERVERADRESS = "127.0.0.1:39393";
        private static readonly Lazy<LogClient> logClient = new Lazy<LogClient>(() => new LogClient());
        private LogService.LogServiceClient client;
        private string _logServerTerminalAddress = "F:\\WorkSpace\\3D\\MaxNet\\Y3D\\x64\\Release\\LogServer.exe";
        private Object thisLock = new Object();
        private int a = 1;

        private LogClient()
        {
            this.client = new LogService.LogServiceClient(new Channel(LOGSERVERADRESS, ChannelCredentials.Insecure));
        }

        public static LogClient Instance
        {
            get
            {
                return logClient.Value;
            }
        }

        private bool IsProcessIsRunning(string processName)
        {
            Process[] pname = Process.GetProcessesByName(processName);
            if (pname.Length == 0)
                return false;
            else
                return true;
        }
        public string logServerTerminalAddress
        {
            get
            {
                lock (thisLock)
                {
                    return _logServerTerminalAddress;                     
                }
            }

            set
            {
                lock (thisLock)
                {
                    _logServerTerminalAddress = value;                    
                }
            }
        }

        private bool log(string message)
        {
            try
            {
                LogMessage m = new LogMessage();
                m.Message = message;
                //expected that the generated function "Log " of GRPC is thread safe
                client.Log(m);
                return true;
            }
            catch (RpcException e)
            {               
                lock (thisLock)
                {
                    this.client = new LogService.LogServiceClient(new Channel(LOGSERVERADRESS, ChannelCredentials.Insecure));
                    var filename = Path.GetFileNameWithoutExtension(_logServerTerminalAddress);
                    if (!IsProcessIsRunning(filename))
                        Process.Start(_logServerTerminalAddress);
                }                
                return false;
            }
        }

        //public void LOG(string message)
        //{
        //    if (!this.log(message)) this.log(message);
        //}

        public void LOG(string mes, params object[] args)
        {
            string message = string.Format(mes, args);
            if (!this.log(message)) this.log(message);
        }
    }
}
