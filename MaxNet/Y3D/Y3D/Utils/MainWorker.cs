using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using y3d.s;
using Grpc.Core;

namespace Y3D.Utils
{
    class MainWorker
    {
        YWorker worker;
        public Channel CChannel;
        public Channel CSChannel;
        public Channel MasterChannel;

        public y3d.s.Tools.ToolsClient CClient;
        public y3d.s.YServiceMaster.YServiceMasterClient CWClient;
        //public y3d.s.YMainWorker CSMClient;

        public void updateClient()
        {

            CChannel = new Channel(worker.IpAddress, ChannelCredentials.Insecure);

        } 

    }
}
