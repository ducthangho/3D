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
    public partial class ProjectControl : UserControl
    {
        public ProjectControl()
        {
            InitializeComponent();
            this.olvColumnPath.AspectGetter = delegate (object x)
            {
                ProjectInfo t = (ProjectInfo)x;
                return t.OriginalPath + "\\" + t.Pname + ".max";
            };
            if (Users.Auth.usetting == null) return;
            if (Users.Auth.usetting.Projects.Count>0)
                this.dlvListProject.SetObjects(Users.Auth.usetting.Projects.Values);
        }

        private void btnNewFromMax_Click(object sender, EventArgs e)
        {
            if (Y3D.Projects.Utils.newProjectFromMax())
            {
                this.dlvListProject.SetObjects(Users.Auth.usetting.Projects.Values);
                Utils.mainform.switch2OManager();
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void btnNewBlank_Click(object sender, EventArgs e)
        {

        }

        private void btnDelP_Click(object sender, EventArgs e)
        {
            DialogResult dialogResult = MessageBox.Show("Are you sure you want to delete?", "Oh", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                ProjectInfo pi = (ProjectInfo)this.dlvListProject.SelectedObject;
                if (Utils.DeleteProject(pi))
                {
                    this.dlvListProject.SetObjects(Users.Auth.usetting.Projects.Values);
                }
            }
        }

        private void btnEditP_Click(object sender, EventArgs e)
        {

        }

        private void LoadP()
        {
            ProjectInfo pi = (ProjectInfo)this.dlvListProject.SelectedObject;
            if (pi != null)
            {
                if (Y3D.Projects.Utils.CurrentP != null)
                {
                    if (pi.Pname == Utils.CurrentP.Pname) return;
                }
                if (Utils.LoadProject(pi))
                {
                    Utils.mainform.switch2OManager();
                } else
                {
                    MessageBox.Show("Can not load this project!");
                }
            }
        }

        private void btnLoadP_Click(object sender, EventArgs e)
        {
            LoadP();
        }

        private void dlvListProject_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void dlvListProject_DoubleClick(object sender, EventArgs e)
        {
            LoadP();
        }
    }
}
