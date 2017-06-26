using Redux;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace YFlow
{
    public class SetBusyAction : IAction
    {
        public bool isBusy { get; set; }
    }

}
