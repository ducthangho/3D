using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace YMasterServer
{
    class Program
    {
        static ManualResetEvent _quitEvent = new ManualResetEvent(false);

        static void Main(string[] args)
        {
            YMServer.InitSystem();
            YMServer.Start();

            Console.CancelKeyPress += (sender, eArgs) =>
            {
                _quitEvent.Set();
                eArgs.Cancel = true;
            };

            // kick off asynchronous stuff 

            _quitEvent.WaitOne();

            //System.Windows.Forms.Application.Run();
            //Console.ReadLine();
        }
    }
}
