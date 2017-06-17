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
    public partial class TestStepPattern : UserControl
    {
        public object settings = null;
        public TestStepPattern()
        {
            InitializeComponent();
        }
        public virtual string PatternName
        {
            get { return ""; }
        }
        public virtual string SettingType
        {
            get { return ""; }
        }
        public virtual string PatternStepType
        {
            
            get { return GetType().Name; }
        }
        public virtual void Unplug() { }
        public virtual void InitData(object o) { }


        protected virtual void Subscribe() {}
        private void Unsubscribe() { }

        protected override void Dispose(bool disposing)
        {
            Unplug();
            if (disposing && (components != null))
            {
                Unsubscribe();
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        public void doApply()
        {

        }

        private void TestStepPattern_Load(object sender, EventArgs e)
        {
            Subscribe();
        }
    }
}
