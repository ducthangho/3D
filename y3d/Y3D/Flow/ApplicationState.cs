﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using y3d.e;

namespace Y3D.Flow.States
{
    public class ApplicationState
    {
        public StepState stepState { get; set; }
        public bool isBusy { get; set; }
        public UserTestData TestData { get; set; }
        public Dictionary<string, bool> TestInScence;
        public VerTest CurrentTest { get; set; }
    }
}
