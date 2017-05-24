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
            var bb = ((ELowpoly)o).LpBlender;
            setting = bb;
            ratio.Value = (decimal)bb.Ratio;
        }
    }
}
