using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Y3D.Projects
{
    public partial class LowBlenderControl : UserControl
    {
        public y3d.e.LPolyBlender setting = null;
        public LowBlenderControl()
        {
            InitializeComponent();
        }

        public void InitData(y3d.e.LPolyBlender bb)
        {
            setting = bb;
            ratio.Value = (decimal)bb.Ratio;
        }
    }
}
