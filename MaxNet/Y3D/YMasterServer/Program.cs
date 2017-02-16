using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace YMasterServer
{
    class Program
    {
        static void Main(string[] args)
        {
            YMServer.InitSystem();
            YMServer.Start();
            Console.ReadLine();
        }
    }
}
