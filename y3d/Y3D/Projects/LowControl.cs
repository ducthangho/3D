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
    public partial class LowControl : UserControl
    {
        public y3d.e.ELowpoly elow = null;
        public LowMaxControl lowmax = null;
        public LowBlenderControl lowblender = null;

        public event Action<bool> OnUpdateButton;

        public LowControl()
        {
            InitializeComponent();
        }

        public void InitData(y3d.e.ELowpoly ll)
        {
            elow = ll;
            if (elow!=null)
            {
                btnAdd.Enabled = false;
                btnDelete.Enabled = true;
                if (elow.LowtypeCase == y3d.e.ELowpoly.LowtypeOneofCase.Lp3Dmax)
                {
                    if (lowmax==null)
                    {
                        lowmax = new LowMaxControl();
                        lowmax.Dock = DockStyle.Fill;
                        this.Controls.Add(lowmax);
                        lowmax.OnApply += (setting) =>
                        {
                            elow.Lp3Dmax = setting;
                            Utils.UpdateLowPoly(elow);
                            OnUpdateButton(true);
                        };
                    }
                    lowmax.Show();
                    lowmax.BringToFront();
                    lowmax.InitData(ll.Lp3Dmax);
                }
                else if (elow.LowtypeCase == y3d.e.ELowpoly.LowtypeOneofCase.LpBlender)
                {
                    if (lowblender == null)
                    {
                        lowblender = new LowBlenderControl();
                        lowblender.Dock = DockStyle.Fill;
                        this.Controls.Add(lowblender);
                    }
                    lowblender.Show();
                    lowblender.BringToFront();
                    lowblender.InitData(elow.LpBlender);
                }
            } else
            {
                btnAdd.Enabled = true;
                btnDelete.Enabled = false;
                if (lowmax != null) lowmax.Hide();
            }
        }


        private void btnAdd_Click(object sender, EventArgs e)
        {
            elow = new y3d.e.ELowpoly();
            elow.Oname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_high";
            elow.Nname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_low";
            if (cmbLowType.Text == "3DS MAX")
            {
                elow.Lp3Dmax = new y3d.e.LPoly3DMax();
                elow.Lp3Dmax.VertexCount = 0;
                elow.Lp3Dmax.VertexPercent = 50;
            } else if (cmbLowType.Text == "BLENDER")
            {
                elow = new y3d.e.ELowpoly();
                elow.LpBlender = new y3d.e.LPolyBlender();
            }

            if (cmbLowType.Text != "(none)")
                InitData(elow);
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (lowmax!=null)
                lowmax.Hide();
            if (lowblender != null)
                lowblender.Hide();
            btnAdd.Enabled = true;
            btnDelete.Enabled = false;
            OnUpdateButton(false);
        }
    }
}
