using Redux;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace YFlow.TestDetailComponent
{
    //public class BeginEditAction : IAction
    //{
    //    public YEventList EventList { get; set; }
    //}

    //public class EndEditAction : IAction
    //{
    //    public YEventList EventList { get; set; }
    //}

    public class ApplyStepAction : IAction
    {
        public string stepName { get; set; }
        public YEvent StepEvent;
    }

    public class ExitStepAction : IAction
    {
        public string stepName { get; set; }
        public YEvent StepEvent;
    }

    public class EnterStepAction : IAction
    {
        public string stepName { get; set; }
    }

    public class InitTestAction : IAction
    {
        public VerTest vtest;
    }

    public class SwitchStepAction : IAction
    {
        public string oldStep { get; set; }
        public string newStep { get; set; }
    }

    public class SwitchStateAction : IAction
    {
        public EditStateEnum NewState;
    }
}
