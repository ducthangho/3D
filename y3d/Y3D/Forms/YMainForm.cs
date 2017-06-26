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
    public partial class YMainForm : MetroFramework.Forms.MetroForm
    {
        private List<Button> htab_buttons = new List<Button>();
        public int size_mode = 1;
        public int[] pannel_width = null;
        public YMainForm()
        {
            InitializeComponent();
            htab_buttons.Add(btnTabProject);
            htab_buttons.Add(btnTabObject);
            htab_buttons.Add(btnTabTest);
        }

        public void toggle_size()
        {
            size_mode = 1 - size_mode;

            var s = mainPannel.Size;
            s.Width = pannel_width[size_mode];
            mainPannel.Size = s;

            if (size_mode == 0)
            {

            }
        }

        private void init_htab()
        {
            htab_buttons.Add(btnTabProject);
            htab_buttons.Add(btnTabObject);
            htab_buttons.Add(btnTabTest);

            pannel_width[0] = 300;
            pannel_width[1] = 550;


            //if (btnExpand.Text == ">")
            //{
            //    var s = this.Size;
            //    s.Width = objectsControl1.PreferredSize.Width + 50;
            //    this.Size = s;

            //    var l = objectsControl1.Location;
            //    l.X = 0;
            //    objectsControl1.Location = l;
            //    btnExpand.Text = "<";
            //}
            //else
            //{
            //    var s = this.Size;
            //    s.Width = 350;
            //    this.Size = s;

            //    var l = objectsControl1.Location;
            //    l.X -= 200;
            //    objectsControl1.Location = l;
            //    btnExpand.Text = ">";
            //}

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

            if (n.Tag != "omana")
            {
                btnExpand.Enabled = false;
                btnExpand.Hide();
                var s = this.Size;
                s.Width = 350;
                this.Size = s;
            } else
            {
                btnExpand.Enabled = true;
                btnExpand.Show();
                var s = this.Size;
                if (btnExpand.Text == "<")
                {
                    s.Width = objectsControl1.PreferredSize.Width + 50;
                } else
                {
                    s.Width = 350;
                }
                this.Size = s;
            }
        }

        private void testForm1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            testControl1.Show();
            objectsControl1.Hide();
        }

        private void YMainForm_Load(object sender, EventArgs e)
        {
            //rpc.YServer.Start();
            projectControl1.Show();
            active_htab(btnTabProject);
        }

        private void objectManagerMItem_Click(object sender, EventArgs e)
        {
            //objectsControl1.Show();
            //testControl1.Hide();
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
            //ProjectForm pf = new ProjectForm();
            //DialogResult result = pf.ShowDialog();
            //if (result == DialogResult.OK)
            //{
            //    this.objectsControl1.updateYAL(Y3D.Projects.Utils.CurrentYAL);
            //}
        }

        //private void workersManagerToolStripMenuItem_Click(object sender, EventArgs e)
        //{
        //    rpc.YServer.wform = new WorkerForm();
        //    DialogResult result = rpc.YServer.wform.ShowDialog();
        //    if (result == DialogResult.OK)
        //    {

        //    }
        //}


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

        private void projectControl1_Load(object sender, EventArgs e)
        {

        }

        private void YMainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Y3D.Projects.Utils.detach_mainworker();
            //rpc.YServer.Stop();
            Application.Exit();
        }

        public void switch2OManager()
        {
            objectsControl1.Show();
            //objectsControl1.updateYAL(Y3D.Projects.Utils.CurrentYAL);
            var yal = Projects.Utils.Store.GetState().ObjectManager.CurrentYAL;
            objectsControl1.updateYAL(yal);
            active_htab(btnTabObject);
        }

        public void resetOM()
        {
            //Y3D.Projects.Utils.CurrentYAL.Areas.Clear();
            //objectsControl1.updateYAL(Y3D.Projects.Utils.CurrentYAL);
            Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.ResetYAreaAction { });
            var yal = Projects.Utils.Store.GetState().ObjectManager.CurrentYAL;
            objectsControl1.updateYAL(yal);
            //Y3D.Projects.Utils.CurrentYAL = null;
        }

        private void btnMainMenu_Click(object sender, EventArgs e)
        {
            metroContextMenu1.Show(btnMainMenu, new Point(0, btnMainMenu.Height));
        }

        private void toggleExpand()
        {

        }

        private void btnExpand_Click(object sender, EventArgs e)
        {
            if (btnExpand.Text==">")
            {
                var s = this.Size;
                s.Width = objectsControl1.PreferredSize.Width + 50;
                this.Size = s;

                var l = objectsControl1.Location;
                l.X = 0;
                objectsControl1.Location = l;
                btnExpand.Text = "<";
                btnExpand.Image = (Image)Y3D.Properties.Resources.ResourceManager.GetObject("sizemode_1");
            } else
            {
                var s = this.Size;
                s.Width = 350;
                this.Size = s;

                var l = objectsControl1.Location;
                l.X -= 200;
                objectsControl1.Location = l;
                btnExpand.Text = ">";
                btnExpand.Image = (Image)Y3D.Properties.Resources.ResourceManager.GetObject("sizemode_0");
            }
        }

        private void workerManagerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rpc.YServer.wform = new WorkerForm();
            DialogResult result = rpc.YServer.wform.ShowDialog();
            if (result == DialogResult.OK)
            {

            }
        }
    }
}
