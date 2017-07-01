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
using System.IO;
using System.Reactive.Linq;

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
            };

            this.olvColNameO.ImageGetter = delegate (object x)
            {
                YObject t = (YObject)x;
                return t.Otype.ToString();
            };

            this.olvInScene.ImageGetter = delegate (object x) {
                if (x is VerTest)
                {
                    var vv = (VerTest)x;
                    var sk = vv.Oname + "_" + vv.Id;
                    if (!Utils.Store.GetState().ObjectManager.TestInScence.ContainsKey(sk)) return "g_red";
                    if (Utils.Store.GetState().ObjectManager.TestInScence[sk])
                    {
                        return "g_green";
                    } else
                    {
                        return "g_red";
                    }
                }
                return "g_gray";
            };

            
            Utils.Store.DistinctUntilChanged(state => new { state.ObjectManager.ListTest }).Subscribe(
                state =>
                {
                    if (state.ObjectManager.ListTest!=null)
                    {
                        olvLocalTest.SetObjects(state.ObjectManager.ListTest);
                    } 
                    else
                    {
                        olvLocalTest.SetObjects(new List<VerTest>());
                    }
                }
            );


            Utils.Store.DistinctUntilChanged(state => new { state.ObjectManager.CurrentTest }).Subscribe(
                state =>
                {
                    if (state.ObjectManager.CurrentTest != null)
                    {
                        panelEditTest.BringToFront();
                    } else
                    {
                        panelObjList.BringToFront();
                    }
                }
            );

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
                Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.ExitTestAction { });
                Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.SelectObjectAction {
                    oname = y.Name
                });

                if (checkBoxIsolate.Checked)
                {
                    YEvent ye = new YEvent();
                    ye.Isolate = new EIsolate();
                    ye.Isolate.EndIsolate = false;
                    ye.Isolate.Name = y.Name;
                    Y3D.Projects.Utils.MaxClient.DoEventAsync(ye);
                } else
                {
                    YEvent ye = new YEvent();
                    ye.Select = new ESelect();
                    ye.Select.Name = y.Name;
                    ye.Select.Isolate = false;
                    Y3D.Projects.Utils.MaxClient.DoEventAsync(ye);
                }
                //if (this.checkBoxInGroup.Checked)
                //    cmd += "max tool zoomextents all;";
            }
        }

        private void objectListCtrl_ItemsChanged(object sender, BrightIdeasSoftware.ItemsChangedEventArgs e)
        {

        }


        private void checkBoxIsolate_CheckedChanged(object sender, EventArgs e)
        {
            Object x = objectListCtrl.SelectedObject;
            if (x is YObject)
            {
                var y = (YObject)x;
                YEvent ye = new YEvent();
                ye.Isolate = new EIsolate();
                ye.Isolate.EndIsolate = (((CheckBox)sender).Checked==false);
                ye.Isolate.Name = y.Name;
                Y3D.Projects.Utils.MaxClient.DoEvent(ye);
            }
        }

        private void listCheck_CheckedChanged(object sender, EventArgs e)
        {
            objectListCtrl.CheckBoxes = ((CheckBox)sender).Checked;
            this.olvColNameO.HeaderCheckBox = ((CheckBox)sender).Checked;
        }

        private void checkBoxInGroup_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void toolStripExpand_Click(object sender, EventArgs e)
        {
            var s = this.Location;
            s.X -= 200;
            this.Location = s;
        }

        private void toolStripInfo_Click(object sender, EventArgs e)
        {
        }

        private void btnPreview_Click(object sender, EventArgs e)
        {
            Object o = objectListCtrl.SelectedObject;
            if (o is YObject)
            {
                var oo = (YObject)o;
                Object v = olvLocalTest.SelectedObject;
                if (v is VerTest)
                {
                    var vv = (VerTest)v;
                    var test_path = Path.Combine(Utils.CurrentP.ProjectPath, "test", (oo.Name + "_" + vv.Id), oo.Name+"_final.obj");
                    try
                    {
                        System.Diagnostics.Process.Start(test_path);
                    }
                    catch (Exception)
                    {
                        MessageBox.Show("Can not preview this object!!");
                    }
                    
                }
            }
        }

        private void btnAddTest_Click(object sender, EventArgs e)
        {
            Object x = objectListCtrl.SelectedObject;
            if (x is YObject)
            {
                Projects.TestInitForm tf = new TestInitForm();
                var r = tf.ShowDialog();
                if (r == DialogResult.Cancel) return;
                var y = (YObject)x;
                if (Projects.Utils.CreateNewTest(y.Name, tf.note, tf.testPreset))
                {
                    var TestData = Utils.Store.GetState().ObjectManager.TestData;
                    olvLocalTest.SetObjects(TestData.Utests[y.Name].Otests);
                }
            }
        }

        private void btnTestDel_Click(object sender, EventArgs e)
        {
            Object o = objectListCtrl.SelectedObject;
            if (o is YObject)
            {
                var oo = (YObject)o;
                Object v = olvLocalTest.SelectedObject;
                if (v is VerTest)
                {
                    var vv = (VerTest)v;
                    if (Utils.DeleteTest(vv))
                    {
                        Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.DeleteTestAction {
                            vtest = vv
                        });
                        var TestData = Utils.Store.GetState().ObjectManager.TestData;
                        olvLocalTest.SetObjects(TestData.Utests[oo.Name].Otests);
                    }
                }
            }
        }

        private void btnTestEdit_Click(object sender, EventArgs e)
        {
            Object v = olvLocalTest.SelectedObject;
            if (v is VerTest)
            {
                Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.EnterTestAction
                {
                    vtest = (VerTest)v
                });
                panelEditTest.BringToFront();
            }
        }

        private void btnBack_Click(object sender, EventArgs e)
        {
            Projects.Utils.Store.Dispatch(new YFlow.TestDetailComponent.SwitchStateAction {
                NewState = YFlow.TestDetailComponent.EditStateEnum.EndEdit
            });

            Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.ExitTestAction{});
        }

        private void olvLocalTest_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            Object v = olvLocalTest.SelectedObject;
            if (v is VerTest)
            {
                Projects.Utils.Store.Dispatch(new YFlow.ObjectManagerComponent.EnterTestAction {
                   vtest = (VerTest)v
                });
            }
        }

        private void btnSave2File_Click(object sender, EventArgs e)
        {
            // save layer to file
        }
    }
}
