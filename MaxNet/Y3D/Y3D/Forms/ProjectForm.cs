using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

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
            this.dlvListProject.SetObjects(Utils.Tools.YSys.Projects);
        }

        private void btnLoadP_Click(object sender, EventArgs e)
        {

        }
    }
}
