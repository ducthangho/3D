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

        private List<Button> tab_buttons = new List<Button>();

        private IDisposable _subUpdateStepButton = null;
        public TestDetailControl()
        {
            InitializeComponent();

            tab_buttons.Add(btnLow);
            tab_buttons.Add(btnUnwrap);
            tab_buttons.Add(btnPack);
            tab_buttons.Add(btnBake);

            _subUpdateStepButton = YEventUtils.UpdateStepButton.Subscribe(
                b => updateTabButton(b)
            );
        }

        public void updateTabButton(ButtonUpdateParam b)
        {
            bool state = false;
            if (b.action=="ADD" || b.action == "CHANGE")
            {
                state = true;
            } 
            switch (b.id)
            {
                case 0:
                    updateTabButtonState(0, state);
                    updateTabButtonState(1, false);
                    updateTabButtonState(2, false);
                    updateTabButtonState(3, false);
                    break;
                case 1:
                    updateTabButtonState(1, state);
                    updateTabButtonState(2, false);
                    updateTabButtonState(3, false);
                    break;
                case 2:
                    updateTabButtonState(2, state);
                    break;
                case 3:
                    updateTabButtonState(3, state);
                    break;
                default:
                    break;
            }
            YEventUtils.ResetStepButton.OnNext(true);
        }

        public void updateTabButtonState(int i,bool b)
        {
            var bb = tab_buttons[i];
            bb.BackColor = getButtonColor(b);
            switch (i)
            {
                case 0:
                    Utils.CurrentTest.HasLow = b;
                    break;
                case 1:
                    Utils.CurrentTest.HasUnwrap = b;
                    break;
                case 2:
                    Utils.CurrentTest.HasPack = b;
                    break;
                case 3:
                    Utils.CurrentTest.HasBake = b;
                    break;
                default:
                    break;
            }
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
            lbTestName.Text = "Name : " + v.Oname + "_" + v.Id;
            btnLow.BackColor = getButtonColor(v.HasLow);
            btnUnwrap.BackColor = getButtonColor(v.HasUnwrap);
            btnPack.BackColor = getButtonColor(v.HasPack);
            btnBake.BackColor = getButtonColor(v.HasBake);

            YEventUtils.ResetStepButton.OnNext(true);


            if (v.InitTest != null)
            {
                if (!v.HasLow) return;

                if (v.InitTest.Lowpoly != null)
                {
                    lowTab1.Init(v.InitTest.Lowpoly, v.InitTest.Lowpoly.LowtypeCase.ToString());
                }
                //else
                //{
                //    lowTab1.Init(null);
                //}

                if (!v.HasUnwrap) return;

                if (v.InitTest.Unwrap!=null)
                {
                    unwrapTab1.Init(v.InitTest.Unwrap, v.InitTest.Unwrap.SettingCase.ToString());
                }

                if (!v.HasBake) return;
            }
            //if (v.InitTest != null)
            //{
            //    lowControl1.InitData(v.InitTest.Lowpoly);
            //}
            //else
            //{
            //    lowControl1.InitData(null);
            //}
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
