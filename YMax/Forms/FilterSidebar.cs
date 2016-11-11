using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace YMax.Forms
{
    public partial class FilterSidebar : UserControl
    {

        public FilterSidebar()
        {
            InitializeComponent();
        }

        private void btnEditFRange_Click(object sender, EventArgs e)
        {
            Utilities.YProject.face_range.openFRForm();
        }
    }
}
