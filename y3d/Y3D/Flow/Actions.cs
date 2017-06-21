using Redux;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace Y3D.Flow.Actions
{
    // Object Manager Actions
    public class EnterTestAction : IAction
    {
        public VerTest vtest {get;set;} 
    }

    public class ExitTestAction : IAction
    {
        public VerTest vtest { get; set; }
    }

    public class AddTestAction : IAction
    {
        public string OName;
        public string Note;
        public InitTestPreset Preset;
    }

    public class DeleteTestAction : IAction
    {
        public VerTest vtest;
    }



    public class BeginEdit : IAction
    {
        public YEventList EventList { get; set; }
    }

    public class EndEdit : IAction
    {
        public YEventList EventList { get; set; }
    }

    public class ApplyStep : IAction
    {
        public string stepName { get; set; }
        public YEvent StepEvent;
    }

    public class SetBusyAction: IAction
    {
        public bool isBusy { get; set; }
    }

    public class EnterStepAction: IAction
    {
        public string stepName { get; set; }
    }


}
