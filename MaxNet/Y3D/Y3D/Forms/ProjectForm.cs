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
                return t.Path + "\\" + t.Pname+".max";
            };

            this.dlvListProject.SetObjects(Utils.Tools.YSys.Projects);
            this.TopMost = true;
        }

        private void LoadP()
        {
            ProjectInfo pi = (ProjectInfo)this.dlvListProject.SelectedObject;
            if (Utils.Tools.LoadProject(pi))
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
            //var rnp = rpc.YClient.CClient.LoadProject(pi);
            //if (rnp.Yal != null) this.objectsControl1.updateYAL(np.Yal);
        }

        private void dlvListProject_DoubleClick(object sender, EventArgs e)
        {
            LoadP();
        }

        private void btnDelP_Click(object sender, EventArgs e)
        {

            DialogResult dialogResult = MessageBox.Show("Are you sure you want to delete?", "Oh", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                ProjectInfo pi = (ProjectInfo)this.dlvListProject.SelectedObject;
                var ret = rpc.YClient.CClient.DeleteProject(pi);
                if (ret.Err.Length==0)
                {
                    Utils.Tools.YSys = ret.Sys;
                    dlvListProject.SetObjects(Utils.Tools.YSys.Projects);
                }
            }

        }
    }
}
