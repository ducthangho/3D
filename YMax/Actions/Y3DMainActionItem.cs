using System.Windows.Forms;
using ActionItem = Autodesk.Max.Plugins.ActionItem;

namespace YMax.Actions
{
    public class Y3DMainActionItem : ActionItem
    {
        private Forms.YMainForm form;
        public override bool ExecuteAction()
        {
            if (form == null)
                form = new Forms.YMainForm(this);
            form.Show();
            //form.TopMost = true;
            form.BringToFront();
            form.WindowState = FormWindowState.Normal;
            return true;
        }

        public void Close()
        {
            if (form == null)
            {
                return;
            }
            form.Dispose();
            form = null;
        }

        public override int Id_
        {
            get { return 1; }
        }

        public override string ButtonText
        {
            get { return "Project Manager"; }
        }

        public override string MenuText
        {
            get { return "&Project Manager"; }
        }

        public override string DescriptionText
        {
            get { return "Y3D Project Manager"; }
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
