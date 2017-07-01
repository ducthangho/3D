using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reactive.Linq;

namespace Y3D.Projects
{
    public partial class SettingStepControl : UserControl
    {
        public IDisposable SubReset = null;
        public SettingStepControl()
        {
            InitializeComponent();
            var allSettings = TestStepPatternFactory.All(PatternType)
                            .OrderBy(pattern => pattern.PatternName)
                            .ToList();
            cmbType.DataSource = allSettings;
            cmbType.DisplayMember = "PatternName";
        }

        public virtual string PatternType
        {
            get { return ""; }
        }

        public virtual int buttonIndex
        {
            get { return 0; }
        }

        private TestStepPattern SettingControl
        {
            get { return (TestStepPattern)settingContainer.GetControlFromPosition(0, 0); }
            set
            {
                var oldControl = SettingControl;
                if (oldControl != null)
                {
                    settingContainer.Controls.Remove(oldControl);
                    oldControl.Dispose();
                }
                if (value != null)
                {
                    btnAdd.Enabled = false;
                    cmbType.Enabled = false;
                    btnDelete.Enabled = true;
                    labelNoSetting.Hide();

                    value.Anchor = AnchorStyles.None;
                    settingContainer.Controls.Add(value, 0, 0);
                } else
                {
                    btnAdd.Enabled = true;
                    cmbType.Enabled = true;
                    btnDelete.Enabled = false;
                    labelNoSetting.Show();
                }
            }
        }

        public void Init(object o,string settingType)
        {
            if (o!=null)
            {
                var z = ((List<TestStepPatternFactory>)cmbType.DataSource).Find(x => x.SettingType == settingType);
                if (z != null)
                {
                    cmbType.SelectedItem = z;
                    SettingControl = z.CreateInstance();
                    SettingControl.InitData(o);
                }
                else
                {
                    SettingControl = null;
                }
            }
            else
            {
                SettingControl = null;
            }
        }

        public void Init(TestStepPatternFactory z)
        {
            if (z != null)
            {
                SettingControl = z.CreateInstance();
                SettingControl.InitData(null);
            }
            else
            {
                if (SettingControl!=null)
                    SettingControl = null;
            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            var x = (TestStepPatternFactory)cmbType.SelectedItem;
            Init(x);
            YEventUtils.UpdateStepButton.OnNext(new ButtonUpdateParam(buttonIndex, "ADD"));
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            Init(null);
            YEventUtils.UpdateStepButton.OnNext(new ButtonUpdateParam(buttonIndex, "DELETE"));
        }

        private void SettingStepControl_Load(object sender, EventArgs e)
        {
            //var allSettings = TestStepPatternFactory.All(PatternType)
            //.OrderBy(pattern => pattern.PatternName)
            //.ToList();
            //cmbType.DataSource = allSettings;
            //cmbType.DisplayMember = "PatternName";
        }
    }
}
