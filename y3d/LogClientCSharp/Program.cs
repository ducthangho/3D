using Grpc.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


namespace LogClientCSharp
{
    class Program
    {
        void test1()
        {
            LogClient a = LogClient.Instance;

            for (int k = 0; k < 7; k++)
            {
                var i = k;
                Thread newThread = new Thread(new ThreadStart(() => {
                    Console.WriteLine("Thread {0} starting", i);

                    for (int j = 0; j < 150; j++)
                    {
                        a.LOG("This is message from thread {0}, j = {1}\n", i, j);
                    }
                }));
                newThread.Start();
            }
        }

        static void Main(string[] args)
        {
            //LogClient a = LogClient.Instance;            
            new Program().test1();          
        }
    }
}
