using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace YFlow
{
    public class ApplicationStates
    {
        public bool isBusy { get; set; }
        public TestDetailComponent.States TestState;
        public ObjectManagerComponent.States ObjectManager;
        
    }
}
