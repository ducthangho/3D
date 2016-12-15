using System.Windows.Forms;
using ActionItem = Autodesk.Max.Plugins.ActionItem;

namespace YMax.Actions
{
    public class ObjectManagerItem : ActionItem
    {
        //private Forms.OManagerForm form;
        public override bool ExecuteAction()
        {
            Utilities.YProject.readPInfo(this);
            //Forms.AnalyzeForm anaForm = new Forms.AnalyzeForm();
            //if (anaForm.ShowDialog() == DialogResult.OK)
            //{
            //    if (form == null)
            //        form = new Forms.OManagerForm(this);
            //    form.Show();
            //    form.TopMost = true;
            //    form.BringToFront();
            //    form.WindowState = FormWindowState.Normal;

            //    YMax.Utilities.YOList.ObjectFromMax();
            //    form.updateTree();
            //}
            return true;
        }

        public void Close()
        {
            Utilities.YProject.closeOForm();
            //if (form == null)
            //{
            //    return;
            //}
            //form.Dispose();
            //form = null;
        }

        public override int Id_
        {
            get { return 1; }
        }

        public override string ButtonText
        {
            get { return "Y3D Object Manager"; }
        }

        public override string MenuText
        {
            get { return "&Object Manager"; }
        }

        public override string DescriptionText
        {
            get { return "Group, Area & Object Manager"; }
        }

        public override string CategoryText
        {
            get { return "Y3D"; }
        }

        public override bool IsChecked_
        {
            get { return false; }
        }

        public override bool IsItemVisible
        {
            get { return true; }
        }

        public override bool IsEnabled_
        {
            get { return true; }
        }
    }
}
