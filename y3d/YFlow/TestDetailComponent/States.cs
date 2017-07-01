using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace YFlow.TestDetailComponent
{
    public enum EditStateEnum
    {
        Editing,
        EndEdit,
        Ready,
        Save,
        Delete
    }

    public class States
    {
        public int CurrentTab { get; set; }
        public string CurrentStep { get; set; }

        public EditStateEnum TestStepState { get; set; }
    }
}
