using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Y3D.Models;
using Google.Protobuf;
using System.Windows.Forms;

namespace YTest
{
    class YOList
    {
        public static YGroup activeGroup = null;
        public static YArea activeArea = null;

        public static List<YObject> yoo = new List<YObject>();
        public static List<YArea> GetYAreas()
        {
            if (YOList.AllYArea.Count == 0)
            {
                var gg = new YGroup();
                gg.Name = "Group 1";
                var aa = new YArea();
                aa.Name = "Default Area";
                aa.Groups.Add(gg);

                var yy = new YObject();
                yy.Name = "oh";

                aa.Objs.Add(yy);

                YOList.AllYArea.Add(aa);
                YOList.activeArea = aa;
            }
            return YOList.AllYArea;
        }
        static private List<YArea> AllYArea = new List<YArea>();
    }
}
