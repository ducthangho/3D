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

namespace Y3D.Projects
{
    public partial class ObjectsControl : UserControl
    {
        public ObjectsControl()
        {
            InitializeComponent();
        }

        private void ObjectsControl_Load(object sender, EventArgs e)
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

            this.olvColFace.AspectGetter = delegate (object x)
            {
                //YMesh t = (YMesh)x;
                YObject t = (YObject)x;
                if (t.Otype == ObjectType.Geometry)
                {
                    return t.Mesh.NumFaces;
                }
                return 0;
            };

            this.olvColTypeO.AspectGetter = delegate (object x)
            {
                YObject t = (YObject)x;
                if (t.Otype == ObjectType.Geometry)
                {
                    return t.Mesh.Mtype;
                }
                return t.Otype;

                //return s[0].ToString();
            };

            this.olvColNameO.ImageGetter = delegate (object x)
            {
                YObject t = (YObject)x;
                return t.Otype.ToString();
            };

        }

        public void initGroup()
        {
            EmptyParam ep = new EmptyParam();
            var yal = Y3D.Projects.Utils.MaxClient.GetObjectFromMax(ep);
            this.gtreeListView.SetObjects(yal.Areas);
        }

        public void updateYAL(YAreaList yal)
        {
            this.gtreeListView.SetObjects(yal.Areas);
            if (yal.Areas.Count>0)
            {
                gtreeListView.SelectedObject = yal.Areas[0];
            }
        }

        private void gtreeListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            Object x = this.gtreeListView.SelectedObject;
            if (x != null)
            {
                if (x is YArea)
                {
                    YArea ya = (YArea)x;
                    this.objectListCtrl.SetObjects(ya.Objs);
                }
                else
                {
                    YGroup yg = (YGroup)x;
                    this.objectListCtrl.SetObjects(yg.Objs);
                }
            }
        }

        private void objectListCtrl_Click(object sender, EventArgs e)
        {
            Object x = objectListCtrl.SelectedObject;
            if (x is YObject)
            {

                var y = (YObject)x;
                if (checkBoxIsolate.Checked)
                {
                    YEvent ye = new YEvent();
                    ye.Isolate = new EIsolate();
                    ye.Isolate.EndIsolate = false;
                    ye.Isolate.Name = y.Name;
                    Y3D.Projects.Utils.MaxClient.DoEvent(ye);
                } else
                {
                    YEvent ye = new YEvent();
                    ye.Select = new ESelect();
                    ye.Select.Name = y.Name;
                    Y3D.Projects.Utils.MaxClient.DoEvent(ye);
                }

                
                //if (this.checkBoxInGroup.Checked)
                //    cmd += "max tool zoomextents all;";


                //MessageBox.Show(y.Name);
            }
        }

        private void checkBoxIsolate_CheckedChanged(object sender, EventArgs e)
        {
            Object x = objectListCtrl.SelectedObject;
            if (x is YObject)
            {
                var y = (YObject)x;
                YEvent ye = new YEvent();
                ye.Isolate = new EIsolate();
                ye.Isolate.EndIsolate = !((CheckBox)sender).Checked;
                ye.Isolate.Name = y.Name;
                Y3D.Projects.Utils.MaxClient.DoEvent(ye);
            }
        }

        private void listCheck_CheckedChanged(object sender, EventArgs e)
        {
            objectListCtrl.CheckBoxes = ((CheckBox)sender).Checked;
            this.olvColNameO.HeaderCheckBox = ((CheckBox)sender).Checked;
        }

        private void btnAddTest_Click(object sender, EventArgs e)
        {

        }
    }
}
