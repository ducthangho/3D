using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using y3d.e;

namespace Y3D.Projects
{
    public partial class TestInitForm : MetroFramework.Forms.MetroForm
    {
        public y3d.e.InitTestPreset testPreset = null;
        public string note { get; set; }

        public InitTestPreset defaultInit()
        {
            InitTestPreset tp = new InitTestPreset();
            tp.Unwrap = new EUnwrap();
            tp.Unwrap.Max3D = new MaxUnwrap();
            tp.Unwrap.Max3D.Angle = 66;

            //tp.Lowpoly = new ELowpoly();
            //tp.Lowpoly.Lp3Dmax = new LPoly3DMax();
            //tp.Lowpoly.Lp3Dmax.VertexPercent = 0.1f;
            //tp.Lowpoly.Lp3Dmax.OptimizationMode = LPoly3DMax.Types.OpMode.ProtectBorders;
            //tp.Lowpoly.Lp3Dmax.Normals = LPoly3DMax.Types.NormalMode.ProtectNormals;

            return tp;
        }

        public TestInitForm()
        {
            InitializeComponent();
            this.AcceptButton = btnMakeTest;
            testPreset = defaultInit();
        }

        private void btnMakeTest_Click(object sender, EventArgs e)
        {
            note = txtNote.Text;
            this.DialogResult = DialogResult.OK;
        }
    }
}
