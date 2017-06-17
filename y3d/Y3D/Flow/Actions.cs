using Redux;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace Y3D.Flow.Actions
{
    public class EnterTest : IAction
    {
        public VerTest vtest {get;set;} 
    }

    public class BeginEdit : IAction
    {
        public VerTest vtest { get; set; }
    }

    public class EndEdit : IAction
    {
        public VerTest vtest { get; set; }
    }

    public class ApplyStep : IAction
    {
        public string stepName { get; set; }
    }

    public class SetBusyAction: IAction
    {
        public bool isBusy { get; set; }
    }

    public class EnterStepAction: IAction
    {
        public string stepName { get; set; }
    }

    public class AddTestAction: IAction
    {
        public VerTest vtest;
    }

    public class DeleteTestAction: IAction
    {
        public VerTest vtest;
    }
}
