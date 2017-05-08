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
    public partial class LowMaxControl : UserControl
    {
        public y3d.e.LPoly3DMax settings = null;
        public event Action<y3d.e.LPoly3DMax> OnApply;
        public bool editMode = false;
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

        public void InitData(y3d.e.LPoly3DMax s)
        {
            if (s == null)
            {
                s = new y3d.e.LPoly3DMax();
                s.VertexCount = 0;
                s.VertexPercent = 50;
                editMode = false;
            }
            settings = s;
            barPercent.Value = s.VertexPercent;
            vertexCount.Value = s.VertexCount;
            updateByEditMode(editMode);
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            settings.VertexCount = (int)vertexCount.Value;
            settings.VertexPercent = barPercent.Value;
            OnApply(settings);
            editMode = true;
        }

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
    }
}
