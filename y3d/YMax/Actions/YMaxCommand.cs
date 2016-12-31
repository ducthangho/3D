using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows.Forms;

using Autodesk.Max;
using Autodesk.Max.Plugins;

namespace YMax.Actions
{
    class YMaxCommand: INodeEventCallback
    {
        //public override void NameChanged(ITab<UIntPtr> nodes)
        //{
        //    for (int i = 0; i < nodes.Count; i++)
        //    {
        //        using (var n = Loader.Global.NodeEventNamespace.GetNodeByKey(nodes[(IntPtr)i]))
        //        {
        //            if (n == null) throw new ArgumentNullException("n");
        //            Debug.WriteLine(n.Name);
        //            Debug.WriteLine(n.Handle);
        //            // DO SOMETHING...
        //        }
        //    }
        //}
        public override void Deleted(ITab<UIntPtr> nodes)
        {
            base.Deleted(nodes);
        }
        public override void SelectionChanged(ITab<UIntPtr> nodes)
        {
            //base.SelectionChanged(nodes);
            if (nodes.Count > 0)
            {
                Loader.Core.PushPrompt("Y3D: da chon : ");
                for (int i = 0; i < nodes.Count; i++)
                {
                    var n = Loader.Global.NodeEventNamespace.GetNodeByKey(nodes[i]);
                    if (n == null) continue;
                    if (n.Selected)
                    {
                        //MessageBox.Show(n.Name);
                        if (Utilities.YProject.form != null)
                            Utilities.YProject.form.selectByNode(n);
                        break;
                        //Loader.Core.PushPrompt("," + n.Name);
                    }
                }

            }
        }
    }

    class YExitCommand : ExitMAXCallback
    {
        public override bool Exit(IntPtr hWnd)
        {
            Utilities.YProject.closeAllForm();
            return true;
        }
    }

}
