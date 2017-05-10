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
    public partial class TestDetailControl : UserControl
    {
        public y3d.e.VerTest vtest = null;
        public TestDetailControl()
        {
            InitializeComponent();
            
            lowControl1.OnUpdateButton += (has) =>
            {
                Utils.CurrentTest.HasLow = has;
                btnLow.BackColor = getButtonColor(has);
            };

        }

        private Color getButtonColor(bool has)
        {
            if (has) return Color.DarkSeaGreen;
            return Color.LightGray;
        }

        public void reloadTest(y3d.e.VerTest v)
        {
            Utils.CurrentTest = (VerTest)v;

            Utils.loadTest();
            btnLow.BackColor = getButtonColor(v.HasLow);
            btnUnwrap.BackColor = getButtonColor(v.HasUnwrap);
            btnBake.BackColor = getButtonColor(v.HasBake);
            btnPack.BackColor = getButtonColor(v.HasPack);

            if (v.InitTest != null)
            {
                lowControl1.InitData(v.InitTest.Lowpoly);
            }
            else
            {
                lowControl1.InitData(null);
            }
        }

        private void btnLow_Click(object sender, EventArgs e)
        {
            tabSettings.SelectedTab = tabPageLow;
        }

        private void btnUnwrap_Click(object sender, EventArgs e)
        {
            tabSettings.SelectedTab = tabPageUnwrap;
        }
    }
}
