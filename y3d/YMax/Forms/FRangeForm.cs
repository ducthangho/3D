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
using BrightIdeasSoftware;

namespace YMax.Forms
{
    public partial class FRangeForm : Form
    {
        public FRangeForm()
        {
            InitializeComponent();
            setupList();
        }

        private void FRangeForm_Load(object sender, EventArgs e)
        {
            olvRange.SetObjects(Utilities.YProject.face_range.fr);
            //this.olvColumnColor.AspectToStringConverter = delegate (object x)
            //{
            //    IBMM_Color_24 c = (IBMM_Color_24)x;

            //    switch (t)
            //    {
            //        case YObject.Types.ObjectType.Geometry:
            //            return "G";
            //        default:
            //            return "-";
            //    }

            //    //return s[0].ToString();
            //};
        }

        public void setupList()
        {
            olvRange.CellEditActivation = BrightIdeasSoftware.ObjectListView.CellEditActivateMode.DoubleClick;
            //olvRange.UseCellFormatEvents = true;
            //olvRange.FormatCell += delegate (object sender, FormatCellEventArgs e)
            //{
            //    //e.SubItem.ForeColor = Color.YellowGreen;
            //    Utilities.FRItem f = (Utilities.FRItem)e.Model;
            //    if (e.ColumnIndex == this.olvColumnColor.Index)
            //    {
            //        e.SubItem.Text = "";
            //        //e.SubItem.Decoration.SubItem.ForeColor = Color.Green;
            //        e.SubItem.BackColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
            //        //e.SubItem.ForeColor = Color.Green;
            //    } else if (e.ColumnIndex == olvColumnRange.Index)
            //    {
            //        e.SubItem.ForeColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
            //    }
            //};

        }

        //private void olvRange_FormatRow(object sender, FormatRowEventArgs e)
        //{
           
        //    Utilities.FRItem f = (Utilities.FRItem)e.Model;
        //    e.Item.BackColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
        //}

        private void FRangeForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.olvRange.Clear();
        }

        private void FRangeForm_Activated(object sender, EventArgs e)
        {
            Loader.Global.DisableAccelerators();
        }

        private void FRangeForm_Deactivate(object sender, EventArgs e)
        {
            Loader.Global.EnableAccelerators();
        }

        private void olvRange_FormatCell(object sender, FormatCellEventArgs e)
        {
            Utilities.FRItem f = (Utilities.FRItem)e.Model;
            if (e.ColumnIndex == this.olvColumnColor.Index)
            {
                e.SubItem.Text = "";
                //e.SubItem.Decoration.SubItem.ForeColor = Color.Green;
                e.SubItem.BackColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
                //e.SubItem.ForeColor = Color.Green;
            }
            else if (e.ColumnIndex == olvColumnRange.Index)
            {
                e.SubItem.ForeColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
            }
        }

        //private void olvRange_FormatCell(object sender, FormatCellEventArgs e)
        //{
        //    e.SubItem.ForeColor = Color.YellowGreen;
        //}

        //private void olvRange_FormatRow(object sender, FormatRowEventArgs e)
        //{
        //    Utilities.FRItem f = (Utilities.FRItem)e.Model;
        //    e.Item.ForeColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
        //}

        //private void olvRange_FormatCell(object sender, FormatCellEventArgs e)
        //{
        //    e.SubItem.ForeColor = Color.YellowGreen;
        //    //if (e.ColumnIndex == this.olvColumnColor.Index)
        //    //{
        //    //    Utilities.FRItem f = (Utilities.FRItem)e.Model;
        //    //    e.SubItem.Text = f.fr_name;
        //    //    e.SubItem.Decoration.SubItem.ForeColor = Color.Green;
        //    //    //e.SubItem.ForeColor = (Color)Color.FromArgb(f.color.R, f.color.G, f.color.B);
        //    //    e.SubItem.ForeColor = Color.Green;
        //    //}
        //}
    }
}
