using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Autodesk.Max;
using System.Runtime.InteropServices;

namespace YMax.Forms
{
    public partial class YMainForm : Form
    {
        private readonly Actions.Y3DMainActionItem yAction;
        public YMainForm(Actions.Y3DMainActionItem yAction)
        {
            InitializeComponent();
            this.yAction = yAction;
        }
        public YMainForm()
        {
            InitializeComponent();
        }

        private void YMainForm_Load(object sender, EventArgs e)
        {
   
        }

        private void YMainForm_Activated(object sender, EventArgs e)
        {
            Loader.Global.DisableAccelerators();
        }

        private void YMainForm_Deactivate(object sender, EventArgs e)
        {
            Loader.Global.EnableAccelerators();
        }

        private void btnNewProject_Click(object sender, EventArgs e)
        {
            //Utilities.YProject.readPInfo(this.yAction);
            //DialogResult result = openFileProject.ShowDialog();
            //if (result == DialogResult.OK)
            //{
            //    string file = openFileProject.FileName;
            //    //MessageBox.Show(file);
            //    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.new_project_from_file \""+file+"\"");
            //}
        }
    }
}
