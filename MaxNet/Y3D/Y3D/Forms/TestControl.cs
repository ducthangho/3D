using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Y3D.Forms
{
    public partial class TestControl : UserControl
    {
        public TestControl()
        {
            InitializeComponent();
        }

        private void btnTest1_Click(object sender, EventArgs e)
        {
            //rpc.YClient.test1();
        }

        private void btnTest2_Click(object sender, EventArgs e)
        {
            //rpc.YClient.test2();
        }

        private void btnTest3_Click(object sender, EventArgs e)
        {
            //rpc.YClient.test3();
        }

        private void btnTestBake3D_Click(object sender, EventArgs e)
        {
            //rpc.YClient.test_bake();
        }
    }
}
