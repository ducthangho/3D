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
using System.Reactive.Linq;
using YFlow.TestDetailComponent;

namespace Y3D.Projects
{
    public partial class LowMaxControl : TestStepPattern
    {
        public y3d.e.LPoly3DMax settings = null;
        public event Action<y3d.e.LPoly3DMax> OnApply;
        public bool editMode = false;

        public override string PatternName { get { return "3DMax"; } }
        public override string SettingType { get { return "Lp3Dmax"; } }
        public override string PatternStepType { get { return "lowpoly"; } }

        private IDisposable _subUnplug;
        private IDisposable _subEndEdit;

        static public string surfix_clone_from = "_high";
        static public string surfix_save_to = "_low";
        static public string surfix_tmp = "_low_tmp";

        public LowMaxControl()
        {
            InitializeComponent();
        }

        public void updateByEditMode(bool mode)
        {
            if (mode)
            {
                foreach (Control c in this.Controls)
                {
                    c.Enabled = true;
                }
                btnEditMode.Enabled = true;
                btnEditMode.Text = "Exit Edit Mode";
            } else
            {
                foreach (Control c in this.Controls)
                {
                    c.Enabled = false;
                }
                btnEditMode.Enabled = true;
                btnEditMode.Text = "Begin Edit Mode";
            }
        }

        override public void InitData(object o)
        {
            var elow = (ELowpoly)o;
            if (elow == null)
            {
                elow = new ELowpoly();
                var s = new y3d.e.LPoly3DMax();
                var CurrentTest = Utils.Store.GetState().ObjectManager.CurrentTest;
                elow.Oname = CurrentTest.Oname + "_" + CurrentTest.Id + "_high";
                elow.Nname = CurrentTest.Oname + "_" + CurrentTest.Id + "_low";
                s.VertexCount = 0;
                s.VertexPercent = 50;
                editMode = false;

                elow.Lp3Dmax = s;
            }
            settings = elow.Lp3Dmax;
            barPercent.Value = settings.VertexPercent;
            vertexCount.Value = settings.VertexCount;
            //YEventUtils.UnPlug.Subscribe();
            updateByEditMode(editMode);
        }

        //private void btnOk_Click(object sender, EventArgs e)
        //{
        //    settings.VertexCount = (int)vertexCount.Value;
        //    settings.VertexPercent = barPercent.Value;
        //    //OnApply(settings);
        //    editMode = true;
        //}

        private async void barPercent_ValueChanged(object sender, decimal value)
        {
            if (YEventUtils.EditMode)
            {
                var CurrentTest = Utils.Store.GetState().ObjectManager.CurrentTest;
                if (CurrentTest == null) return;
                //LogClientCSharp.LogClient.Instance.LOG("Value:{0}\n", value);
                y3d.e.YEvent e = new y3d.e.YEvent();
                e.Mod = new y3d.e.EModifierUpdate();
                e.Mod.Oname = CurrentTest.Oname + "_" + CurrentTest.Id + "_low_tmp";
                e.Mod.ModName = "ProOptimizer";
                e.Mod.Key = "VertexPercent";
                e.Mod.Value = value.ToString();
                e.Mod.Type = "Float";
                //Utils.doEvent(e);
                //await YEventUtils.addEvent(e);
                YEventUtils.addObservableEvent(e);
            }
        }

        private void btnEditMode_Click(object sender, EventArgs e)
        {
            var emode = Utils.Store.GetState().TestState.TestStepState;
            if (emode == YFlow.TestDetailComponent.EditStateEnum.Ready)
                //Projects.Utils.Store.Dispatch(new YFlow.TestDetailComponent.BeginEdit{});
                Projects.Utils.Store.Dispatch(new YFlow.TestDetailComponent.SwitchStateAction { NewState = EditStateEnum.Editing });
            else if (emode == YFlow.TestDetailComponent.EditStateEnum.Editing)
            {
                Projects.Utils.Store.Dispatch(new YFlow.TestDetailComponent.SwitchStateAction { NewState = EditStateEnum.EndEdit });
            }
        }

        public override void Unplug()
        {
            YEventList ye = new YEventList();
            YEventUtils.EndEdit.OnNext(ye);
        }

        protected override void Subscribe()
        {
            Utils.Store.DistinctUntilChanged(state => new { state.TestState.TestStepState }).Subscribe(
                    state =>
                    {
                        if (state.TestState.CurrentStep == PatternStepType)
                        {
                            if (state.TestState.TestStepState == EditStateEnum.Editing)
                            {
                                updateByEditMode(true);
                                YEventUtils.surfix_tmp = "_low_tmp";
                             //var high_name = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_high";
                             var CurrentTest = Utils.Store.GetState().ObjectManager.CurrentTest;
                                var tmp_name = CurrentTest.Oname + "_" + CurrentTest.Id + YEventUtils.surfix_tmp;
                             //clone_e.Yclone.ConvertType = ConvertType.EditableMesh;

                             YEvent low_e = new YEvent();
                                low_e.Lowpoly = new ELowpoly();
                                low_e.Lowpoly.Lp3Dmax = settings;
                                low_e.Lowpoly.Oname = tmp_name;

                                YEvent isolate_e = new YEvent();
                                isolate_e.Isolate = new EIsolate();
                                isolate_e.Isolate.Name = tmp_name;

                                YEventList el = new YEventList();
                                el.Events.Add(low_e);
                                el.Events.Add(isolate_e);

                                YEventUtils.reload(true, el);
                            }
                            else if (state.TestState.TestStepState == EditStateEnum.EndEdit)
                            {
                                DialogResult dialogResult = MessageBox.Show("Do you want to save the changes?", "Apply changes", MessageBoxButtons.YesNoCancel);
                                if (dialogResult == DialogResult.Yes)
                                {
                                    updateByEditMode(false);
                                    YEvent remove_e = new YEvent();
                                    remove_e.Del = new EDelete();
                                    var tname = state.ObjectManager.CurrentTest.Oname + "_" + state.ObjectManager.CurrentTest.Id;

                                    remove_e.Del.Names.Add(tname + surfix_save_to);

                                    YEvent rename_e = new YEvent();
                                    rename_e.Rename = new ERename();
                                    rename_e.Rename.Oname = tname + surfix_tmp;
                                    rename_e.Rename.Nname = tname + surfix_save_to;

                                    YEvent clone_e = new YEvent();
                                    clone_e.Yclone = new EClone();
                                    clone_e.Yclone.Oname = tname + surfix_tmp;
                                    clone_e.Yclone.Cname = tname + surfix_save_to;
                                    clone_e.Yclone.CloneType = EClone.Types.CloneType.NodeInstance;
                                    clone_e.Yclone.ConvertType = ConvertType.EditableMesh;

                                    YEventList yel = new YEventList();
                                    yel.Events.Add(remove_e);
                                    yel.Events.Add(clone_e);

                                    Utils.doManyEvent(yel);

                                    state.TestState.TestStepState = EditStateEnum.Ready;
                                    YEventUtils.UpdateStepButton.OnNext(new ButtonUpdateParam(0, "CHANGE"));
                                }
                                else if (dialogResult == DialogResult.No)
                                {
                                    updateByEditMode(false);
                                    YEventUtils.reload(false, null);
                                    state.TestState.TestStepState = EditStateEnum.Ready;
                                }
                                else if (dialogResult == DialogResult.Cancel)
                                {
                                    state.TestState.TestStepState = EditStateEnum.Editing;
                                }
                            }
                        }
                    }
                   );
            //_subEndEdit = YEventUtils.EndEdit.Subscribe(
            //    el =>
            //    {
            //        YEventUtils.endEditMode(el);
            //    }    
            //);
        }

        private void Unsubscribe()
        {
            Utils.Store.GetState().TestState.TestStepState = EditStateEnum.Delete;
            YEventUtils.EditMode = false;
            if (_subEndEdit != null)
                _subEndEdit.Dispose();
            if (_subUnplug != null)
                _subUnplug.Dispose();
        }
    }
}
