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
    public partial class YMainForm : Form
    {
        public YMainForm()
        {
            InitializeComponent();
        }

        private void testForm1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            testControl1.Show();
        }
    }
}
