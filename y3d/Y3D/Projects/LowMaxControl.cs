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
                elow.Oname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_high";
                elow.Nname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_low";
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
            if (editMode)
            {
                if (Utils.CurrentTest == null) return;
                //LogClientCSharp.LogClient.Instance.LOG("Value:{0}\n", value);
                y3d.e.YEvent e = new y3d.e.YEvent();
                e.Mod = new y3d.e.EModifierUpdate();
                e.Mod.Oname = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_low_tmp";
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
            editMode = !editMode;
            updateByEditMode(editMode);
            if (editMode)
            {
                YEventUtils.surfix_tmp = "_low_tmp";
                //var high_name = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + "_high";
                var tmp_name = Utils.CurrentTest.Oname + "_" + Utils.CurrentTest.Id + YEventUtils.surfix_tmp;
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

                
                YEventUtils.reload(editMode, el);
                //Utils.doManyEvent(el);
            } else
            {
                YEventUtils.reload(editMode, null);
            }

        }

        public override void Unplug()
        {
            YEventList ye = new YEventList();
            YEventUtils.EndEdit.OnNext(ye);
        }

        protected override void Subscribe()
        {
            _subEndEdit = YEventUtils.EndEdit.Subscribe(
                el =>
                {
                    YEventUtils.endEditMode(el);
                }    
            );
        }

        private void Unsubscribe()
        {
            if (_subEndEdit != null)
                _subEndEdit.Dispose();
            if (_subUnplug != null)
                _subUnplug.Dispose();
        }

        private void LowMaxControl_Load(object sender, EventArgs e)
        {

        }
    }
}
