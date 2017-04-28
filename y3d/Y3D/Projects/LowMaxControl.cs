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
    public partial class LowMaxControl : UserControl
    {
        public y3d.e.LPoly3DMax settings = null;
        public event Action<y3d.e.LPoly3DMax> OnApply;
        public bool editMode = false;
        public LowMaxControl()
        {
            InitializeComponent();
        }
        public void InitData(y3d.e.LPoly3DMax s)
        {
            if (s == null)
            {
                s = new y3d.e.LPoly3DMax();
                s.VertexCount = 0;
                s.VertexPercent = 50;
                editMode = false;
            }
            settings = s;
            barPercent.Value = s.VertexPercent;
            vertexCount.Value = s.VertexCount;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            settings.VertexCount = (int)vertexCount.Value;
            settings.VertexPercent = barPercent.Value;
            OnApply(settings);
            editMode = true;
        }

        private void barPercent_ValueChanged(object sender, decimal value)
        {
            if (editMode)
            {
                y3d.e.YEvent e = new y3d.e.YEvent();
                e.Mod = new y3d.e.EModifierUpdate();
                e.Mod.Key = "ProOptimizer";
                e.Mod.Value = value.ToString();
                e.Mod.Type = "int";
                Utils.doEvent(e);
            }
        }
    }
}
