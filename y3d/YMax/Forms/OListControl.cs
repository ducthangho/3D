using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Y3D.Entities;

namespace YMax.Forms
{
    public partial class OListControl : UserControl
    {
        private YObject curObject = null;

        public OListControl()
        {
            InitializeComponent();

            this.olvColumnFaces.AspectGetter = delegate (object x)
            {
                //YMesh t = (YMesh)x;
                YObject t = (YObject)x;
                if (t.Otype == ObjectType.Geometry)
                {
                    return t.Mesh.NumFaces;
                }
                return 0;
            };

            this.olvColumnType.AspectGetter = delegate (object x)
            {
                YObject t = (YObject)x;
                if (t.Otype == ObjectType.Geometry)
                {
                    return t.Mesh.Mtype;
                }
                return t.Otype;

                //return s[0].ToString();
            };

            this.olvColumnName.ImageGetter = delegate (object x)
            {
                YObject t = (YObject)x;
                return t.Otype.ToString();
            };

            Utilities.YOList.OnUpdateByArea += this.loadFromArea;
            Utilities.YOList.OnUpdateByGroup += this.loadFromGroup;

        }


        public void test1(YArea ya)
        {
            MessageBox.Show(ya.Name);
        }

        public void test2(YGroup yg)
        {
            MessageBox.Show("G:" + yg.Name);
        }

        public void loadFromArea(YArea ya)
        {
            this.yoFastListView.ClearObjects();
            this.yoFastListView.SetObjects(ya.Objs);
        }

        public void loadFromGroup(YGroup yg)
        {
            this.yoFastListView.ClearObjects();
            this.yoFastListView.SetObjects(yg.Objs);
            lbVerType.Text = "Version Type: " + yg.GvType.ToString();
        }


        public void selectByOName(string oname)
        {
            foreach (YObject o in yoFastListView.Objects)
            {
                if (o.Name == oname)
                {
                    this.yoFastListView.SelectedObject = o;
                    yoFastListView.EnsureModelVisible(o);
                    break;
                }
            }
        }
        //public void loadFromArea(List<YObject> objs)
        //{
        //    this.yoFastListView.ClearObjects();
        //    this.yoFastListView.SetObjects(objs);
        //}

        public void updateAGDetail(string aName, string gName)
        {

        }

        public void updateSizeMode(int sMode)
        {
            switch (sMode)
            {
                case 1: //small
                    this.Width = 450;
                    break;
                case 2:
                    this.Width = 550;
                    break;
                case 3:
                    this.Width = 750;
                    break;
                default:
                    break;
            }
            this.yoFastListView.Width = this.Width - 6;
        }

        private void listCheck_CheckedChanged(object sender, EventArgs e)
        {
            this.yoFastListView.CheckBoxes = ((CheckBox)sender).Checked;
            this.olvColumnName.HeaderCheckBox = ((CheckBox)sender).Checked;
        }

        private void yoFastListView_FormatCell(object sender, BrightIdeasSoftware.FormatCellEventArgs e)
        {
            if (e.ColumnIndex == this.olvColumnFaces.Index)
            {
                YObject y = (YObject)e.Model;
                var rid = -1;
                if (y.Otype == ObjectType.Geometry)
                    rid = Utilities.YProject.face_range.getColorId(y.Mesh.NumFaces);

                if (rid >= 0)
                {
                    var f = Utilities.YProject.face_range.fr[rid];
                    e.SubItem.ForeColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
                }
                else
                {
                    e.SubItem.ForeColor = Color.Gray;
                }
            }
        }

        private void yoFastListView_FormatRow(object sender, BrightIdeasSoftware.FormatRowEventArgs e)
        {
            //    Utilities.FRItem f = (Utilities.FRItem)e.Model;
            //    e.Item.BackColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
            //e.Item.ForeColor = Color.Green;
            //YObject y = (YObject)e.Model;
            //var rid = Utilities.YProject.face_range.getColorId(y.NumFaces);
            //if (rid>=0)
            //{
            //    var f = Utilities.YProject.face_range.fr[rid];
            //    e.Item.ForeColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
            //} else
            //{
            //    e.Item.ForeColor = Color.Gray;
            //}
        }

        private void yoFastListView_DoubleClick(object sender, EventArgs e)
        {

        }

        private void checkBoxInGroup_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Checked) {
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("max group open");
            } else
            {
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("max group close");
            }
        }

        private void btnXref_Click(object sender, EventArgs e)
        {
            ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.test_xref \"chao chao\"");
        }

        private void btnLow_Click(object sender, EventArgs e)
        {
            if (curObject!=null)
            {
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.do_lowpoly \""+ curObject.Name + "\"");
            }
        }

        private void btnExport_Click(object sender, EventArgs e)
        {
            Object x = this.yoFastListView.SelectedObject;
            Utilities.YOList.testExport(x);
        }

        private void checkBoxIsolate_CheckedChanged(object sender, EventArgs e)
        {
            if (!((CheckBox)sender).Checked)
            {
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("IsolateSelection.ExitIsolateSelectionMode()");
            }
        }

        private void yoFastListView_Click(object sender, EventArgs e)
        {
            Object x = this.yoFastListView.SelectedObject;
            if (x is YObject)
            {

                var y = (YObject)x;
                curObject = y;
                string cmd = "select $" + y.Name + ";";
                //if (this.checkBoxInGroup.Checked)
                //    cmd += "max tool zoomextents all;";

                if (this.checkBoxIsolate.Checked)
                    cmd += "actionMan.executeAction 0 \"197\";";
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand(cmd);

                if (this.checkBoxInGroup.Checked)
                {
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("max group open");
                }
                else
                {
                    ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("max group close");
                }
                //MessageBox.Show(y.Name);
            }
        }

        private void btnUnwrap_Click(object sender, EventArgs e)
        {
            Object x = this.yoFastListView.SelectedObject;
            if (x is YObject)
            {
                var y = (YObject)x;
                string cmd = "select $" + y.Name + ";";
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand(cmd);
                //ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.unwrap 128 1024 5");
                ManagedServices.MaxscriptSDK.ExecuteMaxscriptCommand("yms.unwrap3dmax 66 0.001");
            }
        }
    }
}
