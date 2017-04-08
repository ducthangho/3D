using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using y3d.e;

namespace Y3D.Forms
{
    public partial class ProjectForm : Form
    {
        public ProjectForm()
        {
            InitializeComponent();
        }

        private void ProjectForm_Load(object sender, EventArgs e)
        {
            //MessageBox.Show(Utils.Tools.YSys.Projects.Count.ToString());
            this.olvColumnPath.AspectGetter = delegate (object x)
            {
                //YMesh t = (YMesh)x;
                ProjectInfo t = (ProjectInfo)x;
                return t.OriginalPath + "\\" + t.Pname+".max";
            };

            this.dlvListProject.SetObjects(Users.Auth.usetting.Projects.AsEnumerable());
            this.TopMost = true;
        }

        private void LoadP()
        {
            ProjectInfo pi = (ProjectInfo)this.dlvListProject.SelectedObject;
            if (Y3D.Projects.Utils.LoadProject(pi))
            {
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("Something wrong!! Can not load project");
            }
        }

        private void btnLoadP_Click(object sender, EventArgs e)
        {
            LoadP();
            //var rnp = Y3D.Projects.Utils.MaxClient.LoadProject(pi);
            //if (rnp.Yal != null) this.objectsControl1.updateYAL(np.Yal);
        }

        private void dlvListProject_DoubleClick(object sender, EventArgs e)
        {
            LoadP();
        }

        private void btnDelP_Click(object sender, EventArgs e)
        {

            //DialogResult dialogResult = MessageBox.Show("Are you sure you want to delete?", "Oh", MessageBoxButtons.YesNo);
            //if (dialogResult == DialogResult.Yes)
            //{
            //    ProjectInfo pi = (ProjectInfo)this.dlvListProject.SelectedObject;
            //    var ret = Y3D.Projects.Utils.MaxClient.DeleteProject(pi);
            //    if (ret.Err.Length==0)
            //    {
            //        Utils.Tools.YSys = ret.Sys;
            //        dlvListProject.SetObjects(Utils.Tools.YSys.Projects);
            //    }
            //}

        }
    }
}
