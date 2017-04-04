using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using y3d.s;
using Grpc.Core;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace Y3D.Users
{
    class Auth
    {
        static public YUser yuser = null;
        static public YMachine ymachine = getMachineInfo();
        static public UserSetting usetting = null;

        public static string GetLocalIPAddress()
        {
            var host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (var ip in host.AddressList)
            {
                if (ip.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                {
                    return ip.ToString();
                }
            }
            return "";
        }

        public static bool StartApp(string app_key)
        {
            if (!usetting.Apps.ContainsKey(app_key)) return false;
            var app = usetting.Apps[app_key];
            Process prc = new Process();
            prc.StartInfo.FileName = app.PathRun;
            return prc.Start();
        }

        static public YMachine getMachineInfo()
        {
            YMachine ym = new YMachine();
            ym.Mname = Environment.MachineName;
            ym.IpAddress = GetLocalIPAddress();
            ym.WorkingFolder = "D:\\ywork";
            var path = Path.Combine(ym.WorkingFolder, "u");
            Directory.CreateDirectory(path);
            path = Path.Combine(ym.WorkingFolder, "p");
            //if (!Directory.Exists(path))
            Directory.CreateDirectory(path);
            return ym;
        }

        static public UserResponse signUp(string uname, string pass)
        {
            UserParam up = new UserParam();
            up.Uname = uname;
            up.Password = pass;

            var rep = Y3D.Projects.Utils.MasterClient.UserRegister(up);
            if (!rep.Rep.Error)
            {
                yuser = rep.User;
                usetting = rep.Usetting;
            }
            return rep;
        }

        static public UserResponse signIn(string uname, string pass)
        {
            UserParam up = new UserParam();
            up.Uname = uname;
            up.Password = pass;

            var rep = Y3D.Projects.Utils.MasterClient.UserLogin(up);
            if (!rep.Rep.Error)
            {
                yuser = rep.User;
                usetting = rep.Usetting;
            }
            return rep;
        }

        static public bool updateUSetting()
        {
            if (yuser == null) return false;
            UserParam up = new UserParam();
            up.Usetting = usetting;
            up.Uname = yuser.Username;
            var x = Y3D.Projects.Utils.MasterClient.UpdateUserSetting(up);
            if (!x.Error) return true;
            return false;
        }

    }
}
