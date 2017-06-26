using Redux;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace YFlow.ObjectManagerComponent
{
    public class EnterTestAction : IAction
    {
        public VerTest vtest { get; set; }
    }

    public class ExitTestAction : IAction
    {
    }

    public class AddTestAction : IAction
    {
        //public string OName;
        //public string Note;
        //public InitTestPreset Preset;
        public VerTest vtest;
    }

    public class DeleteTestAction : IAction
    {
        public VerTest vtest;
    }

    public class SaveTestAction : IAction { }

    public class SelectObjectAction : IAction {
        public string oname;
    }

    public class ResetYAreaAction : IAction { }
    public class SetYAreaAction : IAction {
        public YAreaList yal;       
    }



}
