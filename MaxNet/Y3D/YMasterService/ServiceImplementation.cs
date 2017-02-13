using YMasterService.Framework;
using System.ServiceProcess;
using System.Runtime.InteropServices;

namespace YMasterService
{
    /// <summary>
    /// The actual implementation of the windows service goes here...
    /// </summary>
    [WindowsService("YMasterService",
        DisplayName = "YMasterService",
        Description = "Y3D Master Service.",
        EventLogSource = "YMasterService",
        StartMode = ServiceStartMode.Automatic)]
    

    public class ServiceImplementation : IWindowsService
    {
        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        /// <filterpriority>2</filterpriority>
        public void Dispose()
        {
        }
        private System.Threading.Thread _thread;

        public void quit()
        {
            YMServer.Stop();
        }
        /// <summary>
        /// This method is called when the service gets a request to start.
        /// </summary>
        /// <param name="args">Any command line arguments</param>
        public void OnStart(string[] args)
        {
            //_thread = new System.Threading.Thread(YMServer.InitSystem);
            //_thread.Start();
            //(new System.Threading.Thread(() =>
            //{
            //    YMServer.InitSystem();
            //})).Start();
            YMServer.InitSystem();
            YMServer.Start();
        }
        /// <summary>
        /// This method is called when the service gets a request to stop.
        /// </summary>
        public void OnStop()
        {
            YMServer.Stop();
        }

        /// <summary>
        /// This method is called when a service gets a request to pause,
        /// but not stop completely.
        /// </summary>
        public void OnPause()
        {
        }

        /// <summary>
        /// This method is called when a service gets a request to resume 
        /// after a pause is issued.
        /// </summary>
        public void OnContinue()
        {
        }

        /// <summary>
        /// This method is called when the machine the service is running on
        /// is being shutdown.
        /// </summary>
        public void OnShutdown()
        {
            YMServer.Stop();
        }

        /// <summary>
        /// This method is called when a custom command is issued to the service.
        /// </summary>
        /// <param name="command">The command identifier to execute.</param >
        public void OnCustomCommand(int command)
        {
        }
    }
}
