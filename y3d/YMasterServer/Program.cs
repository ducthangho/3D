using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace YMasterServer
{
    class Program
    {

        [DllImport("Kernel32.dll")]
        private static extern IntPtr GetStdHandle(int nStdHandle);

        [DllImport("Kernel32.dll")]
        private static extern bool SetConsoleMode(IntPtr hConsoleHandle, uint dwMode);

        [DllImport("kernel32.dll")]
        static extern bool GetConsoleMode(IntPtr hConsoleHandle, out int mode);

        const int STD_INPUT_HANDLE = -10;
        const int ENABLE_QUICK_EDIT_MODE = 0x0040;
        const int ENABLE_INSERT_MODE = 0x0020;
        const int ENABLE_MOUSE_INPUT = 0x0010;
        const int ENABLE_WINDOW_INPUT = 0x0008;

        static ManualResetEvent _quitEvent = new ManualResetEvent(false);

        static void Main(string[] args)
        {
            int mode;
            IntPtr handle = GetStdHandle(STD_INPUT_HANDLE);
            GetConsoleMode(handle, out mode);
            mode &= ~(ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE | ENABLE_WINDOW_INPUT);
            //mode &= ~ENABLE_QUICK_EDIT_MODE;
            //mode &= ~ENABLE_INSERT_MODE;
            //mode &= ~ENABLE_WINDOW_INPUT;

            SetConsoleMode(handle, (uint)mode);

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
