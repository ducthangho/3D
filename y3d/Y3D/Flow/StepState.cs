using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using y3d.e;

namespace Y3D.Flow.States
{
    public class StepState
    {
        public bool Editing { get; set; }
    }

    public class TestDetailState
    {
        public int CurrentTab { get; set; }
        public string CurrentStep { get; set; }
    }

    public class ObjectManagerState
    {
        public bool isBusy { get; set; }
        public UserTestData TestData { get; set; }
        public string CurrentLayer { get; set; }
        public Dictionary<string, bool> TestInScence;
        public VerTest CurrentTest { get; set; }
    }
}
