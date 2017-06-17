using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using y3d.e;

namespace Y3D.Projects
{
    public partial class LowBlenderControl : TestStepPattern
    {
        public y3d.e.LPolyBlender setting = null;

        public override string PatternName { get { return "Blender"; } }
        public override string SettingType { get { return "LpBlender"; } }
        public override string PatternStepType { get { return "lowpoly"; } }

        public LowBlenderControl()
        {
            InitializeComponent();
        }

        override public void InitData(object o)
        {
            var elow = (ELowpoly)o;
           
            if (elow==null)
            {
                elow = new ELowpoly();
                var bb = new LPolyBlender();
                bb.Ratio = 0.5f;
                elow.LpBlender = bb;
                //var bb = (elow).LpBlender;

            }
            setting = elow.LpBlender;
            ratio.Value = (decimal)setting.Ratio;
        }
    }
}
