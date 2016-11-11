using Autodesk.Max;
using Autodesk.Max.IQuadMenuContext;
using Autodesk.Max.Plugins;
using System.Collections.Generic;

namespace YMax
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
            try
            {
                if (actionTable != null)
                {
                    Loader.Global.COREInterface.ActionManager.DeactivateActionTable(actionCallback, idActionTable);
                }
                // Clean 4up menu
                if (menu != null)
                {
                    Loader.Global.COREInterface.MenuManager.UnRegisterMenu(menu);
                    Loader.Global.ReleaseIMenu(menu);
                    foreach (var item in items)
                    {
                        Loader.Global.ReleaseIMenuItem(item);
                    }
                    //Loader.Global.ReleaseIMenuItem(menuItemY3D);
                    Loader.Global.ReleaseIMenuItem(menuItem);

                    menu = null;
                    menuItem = null;
                }
            }
            catch
            {
                // Fails silently
            }
        }

        private void addMenuItem(int aId)
        {
            //actionTable[aId].ButtonText
            var mItem = Loader.Global.IMenuItem;
            mItem.Title = actionTable[aId].MenuText;
            mItem.ActionItem = actionTable[aId];
            items.Add(mItem);
            menu.AddItem(mItem, -1);
        }

        public override uint Start
        {
            get
            {
                IIActionManager actionManager = Loader.Core.ActionManager;
                IIMenuManager menuManager = Loader.Core.MenuManager;
                items = new List<IIMenuItem>();
                // Set up global actions
                idActionTable = (uint)actionManager.NumActionTables;
                string actionTableName = "Y3D";
                actionTable = Loader.Global.ActionTable.Create(idActionTable, 0, ref actionTableName);
                actionTable.AppendOperation(new Actions.Y3DMainActionItem());
                actionTable.AppendOperation(new Actions.ObjectManagerItem());
                actionCallback = new Actions.Y3DActionCallback();
                actionManager.RegisterActionTable(actionTable);
                actionManager.ActivateActionTable(actionCallback as ActionCallback, idActionTable);

                // Set up menu
                menu = menuManager.FindMenu("Y3D");

                if (menu != null)
                {
                    menuManager.UnRegisterMenu(menu);
                    Loader.Global.ReleaseIMenu(menu);
                    menu = null;
                }

                // Main menu
                menu = Loader.Global.IMenu;
                menu.Title = "Y3D";
                menuManager.RegisterMenu(menu, 0);

                // Launch option
                //menuItemY3D = Loader.Global.IMenuItem;
                //menuItemY3D.Title = "&Y3D Bake Tool";
                //menuItemY3D.ActionItem = actionTable[0];

                //menu.AddItem(menuItemY3D, -1);

                for (int i = 0; i < actionTable.Count; i++)
                {
                    addMenuItem(i);
                }

                menuItem = Loader.Global.IMenuItem;
                menuItem.SubMenu = menu;
                menuManager.MainMenuBar.AddItem(menuItem, -1);

                Loader.Global.COREInterface.MenuManager.UpdateMenuBar();


                Loader.Global.RegisterNotification(Utilities.YProject.fileSaveHandler, null, SystemNotificationCode.FilePreSave);
                //Loader.Global.RegisterNotification(Utilities.YProject.zun1, null, SystemNotificationCode.Custom1);
                Loader.Global.ISceneEventManager.RegisterCallback(Utilities.YProject.ycommand, false, 50, true);

                Actions.YExitCommand a = new Actions.YExitCommand();
                Loader.Core.RegisterExitMAXCallback(a);
                //Loader.Global.BroadcastNotification(SystemNotificationCode.FilePreSave);
                //Loader.Core.regi
                return 0;
            }
        }
    }
}
