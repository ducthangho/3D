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
        private List<Button> htab_buttons = new List<Button>();

        public YMainForm()
        {
            InitializeComponent();
            htab_buttons.Add(btnTabProject);
            htab_buttons.Add(btnTabObject);
            htab_buttons.Add(btnTabTest);
        }

        private void active_htab(Button n)
        {
            foreach (var bt in htab_buttons)
            {
                if (bt.Name != n.Name)
                {
                    bt.Image = (Image)Y3D.Properties.Resources.ResourceManager.GetObject(bt.Tag.ToString() + "_off");
                } else
                {
                    bt.Image = (Image)Y3D.Properties.Resources.ResourceManager.GetObject(n.Tag.ToString() + "_on");
                }
            }
            if (n.Tag != objectsControl1.Tag) objectsControl1.Hide();
            if (n.Tag != testControl1.Tag) testControl1.Hide();
            if (n.Tag != projectControl1.Tag) projectControl1.Hide();
        }

        private void testForm1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            testControl1.Show();
            objectsControl1.Hide();
        }

        private void YMainForm_Load(object sender, EventArgs e)
        {
            //MessageBox.Show(System.IO.Directory.GetCurrentDirectory());
            Utils.Tools.InitSystem();
            projectControl1.Show();
            active_htab(btnTabProject);
            rpc.YServer.Start();
        }

        private void objectManagerMItem_Click(object sender, EventArgs e)
        {
            objectsControl1.Show();
            testControl1.Hide();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //var np = rpc.YClient.NewProject();
            //if (np != null)
            //{
            //    this.objectsControl1.updateYAL(np.Yal);
            //}
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ProjectForm pf = new ProjectForm();
            DialogResult result = pf.ShowDialog();
            if (result == DialogResult.OK)
            {
                this.objectsControl1.updateYAL(Utils.Tools.CurrentYAL);
            }
        }

        private void workersManagerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            WorkerForm wf = new WorkerForm();
            DialogResult result = wf.ShowDialog();
            if (result == DialogResult.OK)
            {
                
            }
        }

        private void btnTabTest_Click(object sender, EventArgs e)
        {
            testControl1.Show();
            active_htab(btnTabTest);
        }

        private void btnTabObject_Click(object sender, EventArgs e)
        {
            objectsControl1.Show();
            active_htab(btnTabObject);
        }

        private void btnTabProject_Click(object sender, EventArgs e)
        {
            projectControl1.Show();
            active_htab(btnTabProject);
        }
    }
}
