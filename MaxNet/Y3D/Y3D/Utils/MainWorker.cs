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
        public static YWorker worker = null;
        public static Channel ChannelLoader = null;
        public static Channel ChannelMax = null;

        static public y3d.s.YServiceMaxTools.YServiceMaxToolsClient MaxClient;
        static public y3d.s.YServiceMaxLoader.YServiceMaxLoaderClient LoaderClient;

        static public void updateClient()
        {
            ChannelLoader = new Channel(worker.IpAddress, ChannelCredentials.Insecure);
            ChannelMax = new Channel(String.Format("127.0.0.1:{0}", worker.Wid + 39000), ChannelCredentials.Insecure);
            LoaderClient = new YServiceMaxLoader.YServiceMaxLoaderClient(ChannelLoader);
            MaxClient = new YServiceMaxTools.YServiceMaxToolsClient(ChannelMax);
        } 

    }
}
