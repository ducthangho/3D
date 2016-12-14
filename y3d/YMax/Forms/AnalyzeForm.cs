using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace YMax.Forms
{
    public partial class AnalyzeForm : Form
    {
        public AnalyzeForm()
        {
            InitializeComponent();
        }

        private void btnCustomColor_Click(object sender, EventArgs e)
        {
            Utilities.YProject.face_range.openFRForm();
        }

        private void btnAnalyze_Click(object sender, EventArgs e)
        {
            
            this.DialogResult = DialogResult.OK;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }
    }
}
