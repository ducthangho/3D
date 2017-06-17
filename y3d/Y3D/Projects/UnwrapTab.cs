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
    public partial class UnwrapTab : SettingStepControl
    {
        public override int buttonIndex
        {
            get
            {
                return 1;
            }
        }

        public override string PatternType
        {
            get
            {
                return "unwrap";
            }
        }
        public UnwrapTab()
        {
            InitializeComponent();
            SubReset = YEventUtils.ResetStepButton.Subscribe(
                b =>
                {
                    if (b)
                    {
                        if (!Utils.CurrentTest.HasUnwrap)
                        {
                            Init(null);
                        }
                    }
                }
            );
        }
    }
}
