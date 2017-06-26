using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;

namespace YFlow.ObjectManagerComponent
{
    public class States
    {
        public bool isBusy { get; set; }
        public UserTestData TestData { get; set; }
        public string CurrentLayer { get; set; }
        public Dictionary<string, bool> TestInScence;
        public VerTest CurrentTest { get; set; }

        public Google.Protobuf.Collections.RepeatedField<VerTest> ListTest { get; set; }

        public YObject CurrentObject { get; set; }
        //public YObject CurrentObject { get; set; }
        public YAreaList CurrentYAL { get; set; }

        public bool isSaved { get; set; }
        public bool autoSave { get; set; }
    }
}
