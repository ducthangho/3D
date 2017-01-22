using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using Google.Protobuf;
using System.Diagnostics;
using System.Windows.Forms;

namespace Y3D.Utils
{
    public class Tools
    {
        public static YSystem YSys = new YSystem();
        public static ProjectInfo CurrentProject = new ProjectInfo();
        public static YAreaList CurrentYAL = null;

        public static bool IsProcessOpen(string name)
        {
            foreach (Process clsProcess in Process.GetProcesses())
            {
                if (clsProcess.ProcessName.Contains(name))
                {
                    return true;
                }
            }
            return false;
        }

        public static void InitSystem()
        {
            if (!IsProcessOpen("3dsmax"))
            {
                Process maxProc = new Process();
                maxProc.StartInfo.FileName = "C:\\Program Files\\Autodesk\\3ds Max 2017\\3dsmax.exe";
                maxProc.Start();
            }
            if (rpc.YClient.CChannel.State != Grpc.Core.ChannelState.Ready)
            {
                while (!rpc.YClient.CChannel.ConnectAsync().IsCompleted)
                {
                    System.Threading.Thread.Sleep(10);
                }
                //rpc.YClient.CChannel.WaitForStateChangedAsync(Grpc.Core.ChannelState.Ready);
            }
            YSys = rpc.YClient.CClient.LoadSystem(new EmptyParam());
            //MessageBox.Show(YSys.DefaultSetting.MaxRecent.ToString());
        }

        public static bool LoadProject(ProjectInfo pi)
        {
            var rnp = rpc.YClient.CClient.LoadProject(pi);
            if (rnp.Yal != null)
            {
                CurrentYAL = rnp.Yal;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
