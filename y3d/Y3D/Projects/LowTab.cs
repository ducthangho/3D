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
    public partial class LowTab : SettingStepControl
    {
        public override int buttonIndex
        {
            get
            {
                return 0;
            }
        }

        public override string PatternType
        {
            get
            {
                return "lowpoly";
            }
        }

        public LowTab()
        {
            InitializeComponent();
            SubReset = YEventUtils.ResetStepButton.Subscribe(
                b =>
                {
                    if (b)
                    {
                        var CurrentTest = Utils.Store.GetState().ObjectManager.CurrentTest;
                        if (!CurrentTest.HasLow)
                        {
                            Init(null);
                        }
                    }
                }    
            );
        }
    }
}
