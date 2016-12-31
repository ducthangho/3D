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

namespace YMax.Forms
{
    public partial class OManagerForm : Form
    {
        private readonly Actions.ObjectManagerItem yAction;
        public OManagerForm(Actions.ObjectManagerItem yAction)
        {
            InitializeComponent();
            this.yAction = yAction;
        }
        public OManagerForm()
        {
            InitializeComponent();
        }

        public void updateSizeMode(int sMode)
        {
            oListControl1.updateSizeMode(sMode);
            this.Width = oListControl1.Width + 10;
            //if (tabSidebar.Visible) this.Width += tabSidebar.Width;
        }

        public void updateTree()
        {
            this.agSidebar1.updateTree();
        }

        public void selectByNode(IINode n)
        {
            if (n == null) return;
            if (n.IsGroupHead)
            {
                agSidebar1.selectByGName(n.Name);
            } else
            {
                if (n.ParentNode!=null)
                {
                    if (n.ParentNode.IsGroupHead)
                    {
                        agSidebar1.selectByGName(n.ParentNode.Name);
                        oListControl1.selectByOName(n.Name);
                    }
                    else if (n.ParentNode.IsRootNode)
                    {
                        var ya = Utilities.YOList.findAreaByOName(n.Name);
                        if (ya != null)
                        {
                            agSidebar1.selectYArea(ya);
                        }
                        oListControl1.selectByOName(n.Name);
                    }
                }

            }
        }

        private void OManagerForm_Load(object sender, EventArgs e)
        {
        }

        private void oFromMaxBtn_Click(object sender, EventArgs e)
        {
            YMax.Utilities.YOList.ObjectFromMax();
            //this.oListControl1.loadFromArea(Utilities.YOList.activeArea);
            this.agSidebar1.updateTree();
            //this.oListControl1.loadFromArea(Utilities.YOList.yoo);
        }

        private void saveBtn_Click(object sender, EventArgs e)
        {
            //ManagedServices.MaxscriptSDK.
        }

        private void OManagerForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            yAction.Close();
        }

        private void sizeModeBtn_ButtonClick(object sender, EventArgs e)
        {
            if (tabSidebar.Visible)
            {
                tabSidebar.Hide();
                oListControl1.Left = 5;
                this.Width =  oListControl1.Width + 10;
            } else
            {
                tabSidebar.Show();
                oListControl1.Left = tabSidebar.Width + 5;
                this.Width = tabSidebar.Width + oListControl1.Width + 10;
            }
            
        }

        private void sModeBtn_Click(object sender, EventArgs e)
        {
            updateSizeMode(1);
        }

        private void mModeBtn_Click(object sender, EventArgs e)
        {
            updateSizeMode(2);
        }

        private void lModeBtn_Click(object sender, EventArgs e)
        {
            updateSizeMode(3);
        }

        private void OManagerForm_Activated(object sender, EventArgs e)
        {
            Loader.Global.DisableAccelerators();
        }

        private void OManagerForm_Deactivate(object sender, EventArgs e)
        {
            Loader.Global.EnableAccelerators();
        }
    }
}
