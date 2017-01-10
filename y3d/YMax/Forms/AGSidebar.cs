using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using y3d.e;
using Autodesk.Max;

namespace YMax.Forms
{
    public partial class AGSidebar : UserControl
    {
        public AGSidebar()
        {
            InitializeComponent();
        }

        void applyView(Object x)
        {
            if (x != null)
            {
                if (x is YArea)
                {
                    YArea ya = (YArea)x;
                    var s = "select #(";
                    foreach (var a in ya.Groups)
                    {
                        s += "$" + a.Name;
                    }
                    s += ")";
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand(s);
                }
                else
                {
                    YGroup yg = (YGroup)x;
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("select $" + yg.Name);
                }
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("max group close");

                if (cbIsolateGroup.Checked)
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("actionMan.executeAction 0 \"197\";");
                else
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("actionMan.executeAction 0 \"261\"");
            }
        }

        private void AGSidebar_Load(object sender, EventArgs e)
        {
            this.gtreeListView.CanExpandGetter = delegate (object x) {
                if (x is YArea) return true;
                if (x is YGroup)
                {
                    YGroup yg = (YGroup)x;
                    if (yg.Children.Count > 0) return true;
                }
                return false;
            };
            this.gtreeListView.ChildrenGetter = delegate (object x) {
                if (x is YArea)
                    return ((YArea)x).Groups;
                if (x is YGroup)
                {
                    YGroup yg = (YGroup)x;
                    if (yg.Children.Count > 0) return yg.Children;
                }
                throw new ArgumentException("Should be Area or Group");
            };

             //TreeListView.TreeRenderer treeColumnRenderer = this.treeListView.TreeColumnRenderer;
            this.olvColumnName.ImageGetter = delegate (object x) {
                if (x is YGroup)
                {
                    YGroup yg = (YGroup)x;
                    if (yg.GvType == GVerType.Standard) 
                        return "g_green";
                    if (yg.GvType == GVerType.Original)
                        return "g_blue";
                    if (yg.GvType == GVerType.InBake)
                        return "g_red";
                    return "g_gray";
                }
                else if (x is YArea)
                    return "area";
                else
                    return "g_gray";
            };
            Utilities.YOList.GetYAreas();
            //this.updateTree();
            //this.gtreeListView.SetObjects(Utilities.YOList.GetYAreas());
        }

        public void updateMultiCheck(bool ck)
        {
            runActBtn.Enabled = !ck;
            actCombo.Enabled = !ck;
            gtreeListView.CheckBoxes = ck;
            cbMultiGAction.Enabled = ck;
            cbMultiGAction.Visible = ck;
        }

        public void updateTree()
        {
            this.gtreeListView.SetObjects(Utilities.YOList.GetYAreas());
            this.gtreeListView.ExpandAll();
        }

        private void mulCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            //var ck = ((CheckBox)sender).Checked;
            updateMultiCheck(((CheckBox)sender).Checked);

        }

        private void gtreeListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            Object x = this.gtreeListView.SelectedObject;
            if (x != null)
            {
                if (x is YArea)
                {
                    Utilities.YOList.updateByArea((YArea)x);
                } else
                {
                    Utilities.YOList.updateByGroup((YGroup)x);
                    //ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("")
                }
            }
            
        }

        private void dispApplyBtn_Click(object sender, EventArgs e)
        {
            //ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("max tool maximize");
            //Loader.Core.ExecuteMAXCommand(50026);
            Loader.Core.ViewportLayout = 8;
        }

        private void gtreeListView_DoubleClick(object sender, EventArgs e)
        {
            applyView(this.gtreeListView.SelectedObject);
        }

        private void btnApplyView_Click(object sender, EventArgs e)
        {
            applyView(this.gtreeListView.SelectedObject);
        }

        private void runActBtn_Click(object sender, EventArgs e)
        {
            if (this.actCombo.SelectedIndex==0)
            {
                applyView(this.gtreeListView.SelectedObject);
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("macros.run \"y3d\" \"y3d_r2\"");
            }
        }

        public void selectByGName(string groupName) 
        {
            if  (Utilities.YOList.mapGroup.ContainsKey(groupName))
            {
                gtreeListView.SelectedObject = Utilities.YOList.mapGroup[groupName];
                gtreeListView.EnsureModelVisible(Utilities.YOList.mapGroup[groupName]);
                Utilities.YOList.updateByGroup(Utilities.YOList.mapGroup[groupName]);
            }
        }

        public void selectYArea(YArea ya)
        {
            gtreeListView.SelectedObject = ya;
            gtreeListView.EnsureModelVisible(ya);
            Utilities.YOList.updateByArea(ya);
        }
    }
}
