using Grpc.Core;
using NDesk.Options;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using y3d;
using y3d.s;

namespace LogClientCSharp
{
    class Program
    {

        void loaddll()
        {
            Console.WriteLine("Helo here");
            var client = new YServiceMaxLoader.YServiceMaxLoaderClient(new Channel("127.0.0.1:38001", ChannelCredentials.Insecure));
            LibInfo req = new LibInfo();
            req.Id = 38001+9;
            // int idx = yw.Wid;
            var r = client.LoadDllAsync(req);
        }

        void test1()
        {
            LogClient a = LogClient.Instance;

            for (int k = 0; k < 7; k++)
            {
                var i = k;
                Thread newThread = new Thread(new ThreadStart(() =>
                {
                    Console.WriteLine("Thread {0} starting", i);

                    for (int j = 0; j < 150; j++)
                    {
                        a.LOG("C#: This is message from thread {0}, j = {1}\n", i, j);
                    }
                }));
                newThread.Start();
            }
        }

        void PickTest(int testNumber)
        {
            if (testNumber == 1)
                test1();
            else if (testNumber == 2)
                loaddll();
        }

        static int verbosity;
        static void Main(string[] args)
        {

            bool show_help = false;
            List<string> names = new List<string>();
            int repeat = 1;            

            var p = new OptionSet() {
                { "n|name=", "the {NAME} of someone to greet.",v => names.Add (v) },
                { "r|repeat=","the number of {TIMES} to repeat the greeting.\n" +"this must be an integer.",(int v) => repeat = v },
                { "v", "increase debug message verbosity",v => { if (v != null) ++verbosity; } },
                { "t|numtest=","Pick the Test scenario number",(int v) => { new Program().PickTest(v); } },
                { "h|help",  "show this message and exit",v => show_help = v != null },
            };

            List<string> extra;
            try
            {
                extra = p.Parse(args);
            }
            catch (OptionException e)
            {
                Console.Write("greet: ");
                Console.WriteLine(e.Message);
                Console.WriteLine("Try `LogClientCSharp --help' for more information.");
                return;
            }

            if (show_help)
            {
                ShowHelp(p);
                return;
            }

            string message;
            if (extra.Count > 0)
            {
                message = string.Join(" ", extra.ToArray());
                Debug("Using new message: {0}", message);
            }
            else
            {
                message = "Hello {0}!";
                Debug("Using default message: {0}", message);
            }

            foreach (string name in names)
            {
                for (int i = 0; i < repeat; ++i)
                    Console.WriteLine(message, name);
            }
        }

        static void ShowHelp(OptionSet p)
        {
            Console.WriteLine("Usage: greet [OPTIONS]+ message");
            Console.WriteLine("Greet a list of individuals with an optional message.");
            Console.WriteLine("If no message is specified, a generic greeting is used.");
            Console.WriteLine();
            Console.WriteLine("Options:");
            p.WriteOptionDescriptions(Console.Out);
        }

        static void Debug(string format, params object[] args)
        {
            if (verbosity > 0)
            {
                Console.Write("# ");
                Console.WriteLine(format, args);
            }
        }
    }
}
