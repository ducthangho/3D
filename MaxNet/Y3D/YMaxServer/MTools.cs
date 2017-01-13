using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Autodesk.Max;
using System.Reflection;

namespace YMaxServer
{
    public static class MTools
    {
        public static IntPtr GetNativeHandle(this INativeObject obj)
        {
#if MAX2015 || MAX2016 || MAX2017
            return obj.NativePointer;
#else
            return obj.Handle;
#endif

        }
        static Assembly GetWrappersAssembly()
        {
            return Assembly.Load("Autodesk.Max.Wrappers, Version=0.0.0.0, Culture=neutral, PublicKeyToken=null");
        }

        public static int genID()
        {
            return 0;
        }

        public static IIGameMesh AsGameMesh(this IIGameObject obj)
        {
            var type = GetWrappersAssembly().GetType("Autodesk.Max.Wrappers.IGameMesh");
            var constructor = type.GetConstructors()[0];
            // var pointerType = GetWrappersAssembly().GetType("IGameCamera");
            unsafe
            {
                var voidPtr = obj.GetNativeHandle().ToPointer();
                return (IIGameMesh)constructor.Invoke(new object[] { obj.GetNativeHandle(), false });
            }
        }

        public static IGenBoxObject AsBox(this IIGameObject obj)
        {
            var type = GetWrappersAssembly().GetType("Autodesk.Max.Wrappers.IGenBoxObject");
            var constructor = type.GetConstructors()[0];
            unsafe
            {
                var voidPtr = obj.GetNativeHandle().ToPointer();
                return (IGenBoxObject)constructor.Invoke(new object[] { obj.GetNativeHandle(), false });
            }
        }
        public static IInterval Forever
        {
            get { return Loader.Global.Interval.Create(int.MinValue, int.MaxValue); }
        }


    }
}
