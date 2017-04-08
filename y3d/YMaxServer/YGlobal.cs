using Autodesk.Max;
using Autodesk.Max.IQuadMenuContext;
using Autodesk.Max.Plugins;
using System.Collections.Generic;

namespace YMaxServer
{
    public class YGlobal : GUP
    {
        IIMenu menu;
        IIMenuItem menuItem;
        IList<IIMenuItem> items;
        uint idActionTable;
        IActionTable actionTable;
        IActionCallback actionCallback;

        public override void Stop()
        {
            rpc.YLoaderServer.Stop();
            //try
            //{
            //    if (actionTable != null)
            //    {
            //        Loader.Global.COREInterface.ActionManager.DeactivateActionTable(actionCallback, idActionTable);
            //    }
            //    // Clean 4up menu
            //    if (menu != null)
            //    {
            //        Loader.Global.COREInterface.MenuManager.UnRegisterMenu(menu);
            //        Loader.Global.ReleaseIMenu(menu);
            //        foreach (var item in items)
            //        {
            //            Loader.Global.ReleaseIMenuItem(item);
            //        }
            //        //Loader.Global.ReleaseIMenuItem(menuItemY3D);
            //        Loader.Global.ReleaseIMenuItem(menuItem);

            //        menu = null;
            //        menuItem = null;
            //    }
            //}
            //catch
            //{
            //    // Fails silently
            //}
        }
        
        public override uint Start
        {
            get
            {
                //Loader.Global.RegisterNotification(Utils.YProject.fileOpenHandle, null, SystemNotificationCode.FilePostOpen);
                //Loader.Global.RegisterNotification(Utilities.YProject.zun1, null, SystemNotificationCode.Custom1);
                //Loader.Global.ISceneEventManager.RegisterCallback(Utils.YProject.ycommand, false, 50, true);

                //Loader.Global.RegisterNotification(Utils.YProject.thang_test1, null, SystemNotificationCode.Custom7); // gan vao custom7

                //Actions.YExitCommand a = new Actions.YExitCommand();
                //Loader.Core.RegisterExitMAXCallback(a);

                //Utils.YProject.initSystem();
                //Loader.Core.DisableSceneRedraw();

                //Loader.Global.BroadcastNotification(SystemNotificationCode.FilePreSave);
                //Loader.Core.regi
                Loader.Core.RegisterExitMAXCallback(new Actions.YExitCommand());
                rpc.YLoaderServer.Start();
                return 0;
            }
        }
    }
}
