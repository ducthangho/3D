using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Y3D.Models;

namespace YTest
{
    public partial class Sidebar1 : UserControl
    {
        public Sidebar1()
        {
            InitializeComponent();
        }

        private void Sidebar1_Load(object sender, EventArgs e)
        {
            this.gtreeListView.CanExpandGetter = delegate (object x) {
                return (x is YArea);
            };
            this.gtreeListView.ChildrenGetter = delegate (object x) {
                if (x is YArea)
                    return ((YArea)x).Groups;
                throw new ArgumentException("Should be Area or Group");
            };

            this.olvColumnName.ImageGetter = delegate (Object x) {
                var y = x.ToString();
                if (x is YGroup)
                    return "fg";
                else if (x is YArea)
                {
                    return "fa";
                }
                else
                    return "fa";
            };

            //this.gtreeListView.SetObjects(YOList.GetYAreas());
            this.gtreeListView.SetObjects(YMax.Utilities.YOList.GetYAreas());
        }
    }
}
