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
    public partial class Unwrap3DMaxControl : TestStepPattern
    {
        public override string PatternName { get { return "Unwrap 3DMAX"; } }
        public override string SettingType { get { return "Max3D"; } }
        public override string PatternStepType { get { return "unwrap"; } }

        public MaxUnwrap setting = null;

        public Unwrap3DMaxControl()
        {
            InitializeComponent();
        }

        override public void InitData(object o)
        {
            var un = (EUnwrap)o;
            if (un == null)
            {
                un = new EUnwrap();
                var u = new MaxUnwrap();
                u.Spacing = 0.001f;
                u.Angle = 66;
                un.Max3D = u;
            }
            setting = un.Max3D;
            nAngle.Value = (decimal)setting.Angle;
            nSpacing.Value = (decimal)setting.Spacing;
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            setting.Angle = (float)nAngle.Value;
            setting.Spacing = (float)nSpacing.Value;

            var CurrentTest = Utils.Store.GetState().ObjectManager.CurrentTest;

            YEvent unwrap_e = new YEvent();
            unwrap_e.Unwrap = new EUnwrap();
            unwrap_e.Unwrap.Oname =  CurrentTest.Oname + "_" + CurrentTest.Id + "_low";
            unwrap_e.Unwrap.Max3D = setting;

            Utils.doEvent(unwrap_e);
        }
    }
}
