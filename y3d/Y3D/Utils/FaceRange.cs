using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using y3d.e;

namespace Y3D.Utils
{
    public class FRItem
    {
        public string fr_name;
        public int max_count;

        public FRItem() {}
        public FRItem(string name, int maxc )
        {
            this.fr_name = name;
            this.max_count = maxc;
        }

    }

    public class FaceRange
    {
        //private Forms.FRangeForm frForm;

        public List<FRItem> fr;
        //private IList<IStdMat2> mat;
        public YColor OutOfColor;
        public YColor RiskColor;
        public FaceRange()
        {
            setDefault();
            //mat = new List<IStdMat2>();
            //for (int i = 0; i < fr.Count; i++)
            //{
            //    mat.Add(Loader.Global.NewDefaultStdMat);
            //    mat[i].Name = fr[i].fr_name;
            //    mat[i].SetDiffuse(Loader.Global.Color.Create(fr[i].color),0);
            //}

            //OutOfColor = Loader.Global.Color.Create(0.5, 0.5, 0.5);
            //RiskColor = Loader.Global.Color.Create(1, 0, 0);

            //mat.Add(Loader.Global.NewDefaultStdMat);
            //mat[mat.Count - 1].Name = "YBox Out of range";
            //mat[mat.Count - 1].SetDiffuse(OutOfColor, 0);

            //mat.Add(Loader.Global.NewDefaultStdMat);
            //mat[mat.Count - 1].Name = "YBox Risk";
            //mat[mat.Count - 1].SetDiffuse(RiskColor, 0);
        }

        public void openFRForm()
        {
            //if (frForm == null)
            //frForm = new Forms.FRangeForm();
            //frForm.Show();
            //frForm.TopMost = true;
            //frForm.BringToFront();
            //frForm.WindowState = FormWindowState.Normal;

        }

        public void setDefault ()
        {
            //fr = new List<FRItem>
            //{
            //    new FRItem("Low 1", 100, Loader.Global.BMM_Color_24.Create(0, 155, 1)),
            //    new FRItem("Low 2", 500, Loader.Global.BMM_Color_24.Create(130, 174, 0)),
            //    new FRItem("Medium", 1000, Loader.Global.BMM_Color_24.Create(193, 194, 0)),
            //    new FRItem("High 1", 10000, Loader.Global.BMM_Color_24.Create(195, 118, 0)),
            //    new FRItem("High 2", 1000000, Loader.Global.BMM_Color_24.Create(195, 79, 22))
            //};
        }

        //public void autoRange(int numR, int maxR)
        //{
        //    var step = (int)(maxR / numR);
        //    Array.Clear(f_range, 0, f_range.Length);
        //    f_range = new int[numR];
        //    for (int i = 0; i < numR; i++)
        //    {
        //        f_range[i] = i * step;
        //    }
        //}

        public int getColorId(int faceCounts)
        {
            for (int i = 0; i < fr.Count; i++)
            {
                var from = 0;
                var to = fr[i].max_count;
                if (i>1)
                {
                    from = fr[i - 1].max_count;
                }
                if ((faceCounts >from) && (faceCounts < to)) return i;
                if (i == fr.Count - 1) return -1;
            }
            return -2;
        }

        //public IStdMat2 getMatByFCount(int faceCounts)
        //{
        //    var cid = getColorId(faceCounts);
        //    //Loader.Global.TheListener.EditStream.Printf("\n Color ID:" + cid);
        //    if (cid > 0)
        //    {
        //        return mat[cid - 1];
        //    }
        //    else
        //    {
        //        return mat[mat.Count + cid];
        //    }
        //}
    }
}
