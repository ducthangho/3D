﻿using System;
using System.ComponentModel;
using Autodesk.Max;
using Autodesk.Max.Plugins;

namespace YMax
{
    public class Loader
    {
        public static IGlobal Global;
        public static IInterface14 Core;
        public static IClass_ID Class_ID;
        public static IIGameScene GameScene;
        public static void AssemblyMain()
        {
            Global = GlobalInterface.Instance;
            Core = Global.COREInterface14;
            GameScene =  Global.IGameInterface;
            // Class_ID = Global.Class_ID.Create(0x8217f123, 0xef980456);
            Class_ID = Global.Class_ID.Create(0x27227747, 0xDD6978);
            Core.AddClass(new Descriptor());
        }

        public static void AssemblyShutdown()
        {

        }
    }
}
