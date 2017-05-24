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

        private TestStepPattern LowSettingControl
        {
            get { return (TestStepPattern)lowSettingContainer.GetControlFromPosition(0, 0); }
            set
            {
                var oldControl = LowSettingControl;
                if (oldControl != null)
                {
                    lowSettingContainer.Controls.Remove(oldControl);
                    oldControl.Dispose();

                    //oldControl.RawMessage -= AddMessageToRaw;
                    //oldControl.NotificationMessage -= AddMessageToNotification;
                }
                if (value!=null)
                {
                    value.Anchor = AnchorStyles.None;
                    lowSettingContainer.Controls.Add(value, 0, 0);
                }
                //value.RawMessage += AddMessageToRaw;
                //value.NotificationMessage += AddMessageToNotification;
            }
        }

        public void InitData(y3d.e.ELowpoly ll)
        {
            elow = ll;
            if (elow!=null)
            {
                btnAdd.Enabled = false;
                cmbLowType.Enabled = false;
                btnDelete.Enabled = true;
                //lowSettingContainer.GetControlFromPosition(0, 0);
                var z = ((List<TestStepPatternFactory>)cmbLowType.DataSource).Find(x => x.SettingType == elow.LowtypeCase.ToString());
                if (z != null)
                {
                    labelNoSetting.Hide();
                    LowSettingControl = z.CreateInstance();
                    LowSettingControl.InitData(elow);
                } else
                {
                    labelNoSetting.Show();
                }
                //if (elow.LowtypeCase == y3d.e.ELowpoly.LowtypeOneofCase.Lp3Dmax)
                //{
                //    if (lowmax==null)
                //    {
                //        lowmax = new LowMaxControl();
                //        lowmax.Dock = DockStyle.Fill;
                //        //this.Controls.Add(lowmax);
                //        lowSettingContainer.Controls.Add(lowmax);
                //        lowmax.OnApply += (setting) =>
                //        {
                //            elow.Lp3Dmax = setting;
                //            Utils.UpdateLowPoly(elow);
                //            OnUpdateButton(true);
                //        };
                //    }
                //    lowmax.Show();
                //    lowmax.BringToFront();
                //    lowmax.InitData(elow);
                //}
                //else if (elow.LowtypeCase == y3d.e.ELowpoly.LowtypeOneofCase.LpBlender)
                //{
                //    if (lowblender == null)
                //    {
                //        lowblender = new LowBlenderControl();
                //        lowblender.Dock = DockStyle.Fill;
                //        this.Controls.Add(lowblender);
                //    }
                //    lowblender.Show();
                //    lowblender.BringToFront();
                //    lowblender.InitData(elow);
                //}
            } else
            {

                btnAdd.Enabled = true;
                cmbLowType.Enabled = true;
                btnDelete.Enabled = false;

                LowSettingControl = null;
                labelNoSetting.Show();
                //if (lowmax != null) lowmax.Hide();
            }
        }


        private void btnAdd_Click(object sender, EventArgs e)
        {
            elow = new y3d.e.ELowpoly();
            elow.Oname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_high";
            elow.Nname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_low";

            var x = (TestStepPatternFactory)cmbLowType.SelectedItem;

            switch (x.SettingType)
            {
                case "Lp3Dmax":
                    elow.Lp3Dmax = new y3d.e.LPoly3DMax();
                    elow.Lp3Dmax.VertexCount = 0;
                    elow.Lp3Dmax.VertexPercent = 50;
                    InitData(elow);
                    OnUpdateButton(true);
                    break;
                case "LpBlender":
                    elow = new y3d.e.ELowpoly();
                    elow.LpBlender = new y3d.e.LPolyBlender();
                    InitData(elow);
                    OnUpdateButton(true);
                    break;
                default:
                    break;
            }
            //if (cmbLowType == "3DS MAX")
            //{
            //    elow.Lp3Dmax = new y3d.e.LPoly3DMax();
            //    elow.Lp3Dmax.VertexCount = 0;
            //    elow.Lp3Dmax.VertexPercent = 50;
            //} else if (cmbLowType.Text == "BLENDER")
            //{
            //    elow = new y3d.e.ELowpoly();
            //    elow.LpBlender = new y3d.e.LPolyBlender();
            //}

            //if (cmbLowType.Text != "(none)")
            //    InitData(elow);
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            //if (lowmax!=null)
            //    lowmax.Hide();
            //if (lowblender != null)
            //    lowblender.Hide();
            //LowSettingControl.Unplug();
            LowSettingControl = null;

            btnAdd.Enabled = true;
            cmbLowType.Enabled = true;
            btnDelete.Enabled = false;

            OnUpdateButton(false);
        }

        private void LowControl_Load(object sender, EventArgs e)
        {
            var allLowSetting = TestStepPatternFactory.AllLowSettings()
            .OrderBy(pattern => pattern.PatternName)
            .ToList();

            cmbLowType.DataSource = allLowSetting;
            cmbLowType.DisplayMember = "PatternName";
        }
    }
}
