using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace Y3D.Forms
{
    public partial class YMainForm : Form
    {
        public YMainForm()
        {
            InitializeComponent();
        }

        private void testForm1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            testControl1.Show();
            objectsControl1.Hide();
        }

        private void YMainForm_Load(object sender, EventArgs e)
        {
            Utils.Tools.initSystem();
            objectsControl1.Show();
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            //this.objectsControl1.initGroup();
            ProjectForm pf = new ProjectForm();
            pf.ShowDialog();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
           var np = rpc.YClient.NewProject();
            if (np.Yal!=null) this.objectsControl1.updateYAL(np.Yal);
            //var ret = rpc.YClient.CClient.NewProject(new y3d.e.NewProjectParam());
            //MessageBox.Show(ret.Path);
        }
    }
}
