using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Autodesk.Max;
using System.Runtime.InteropServices;

namespace YMax.Forms
{
    public partial class YMainForm : Form
    {
        private readonly Actions.Y3DMainActionItem yAction;
        public YMainForm(Actions.Y3DMainActionItem yAction)
        {
            InitializeComponent();
            this.yAction = yAction;
        }
        public YMainForm()
        {
            InitializeComponent();
        }

        private void YMainForm_Load(object sender, EventArgs e)
        {
            // Can the given object be expanded?
            this.treeListView1.CanExpandGetter = delegate (Object x) {
                return (x is ArtistExample) || (x is AlbumExample);
            };

            // What objects should belong underneath the given model object?
            this.treeListView1.ChildrenGetter = delegate (Object x) {
                if (x is ArtistExample)
                    return ((ArtistExample)x).Albums;
                if (x is AlbumExample)
                    return ((AlbumExample)x).Songs;

                throw new ArgumentException("Should be Artist or Album");
            };

            // Which image should be used for which model
            this.olvColumn1.ImageGetter = delegate (Object x) {
                if (x is ArtistExample)
                    return "user";
                else if (x is AlbumExample)
                    return "folder";
                else
                    return "song";
            };

            // Format the size so it looks like "1.1GB"
            this.olvColumn2.AspectToStringConverter = delegate (object x) {
                long size = (long)x;
                int[] limits = new int[] { 1024 * 1024 * 1024, 1024 * 1024, 1024 };
                string[] units = new string[] { "GB", "MB", "KB" };

                for (int i = 0; i < limits.Length; i++)
                {
                    if (size >= limits[i])
                        return String.Format("{0:#,##0.##} " + units[i], ((double)size / limits[i]));
                }

                return String.Format("{0} bytes", size);
                ;
            };

            // Draw stars depending on the value of the rating
            this.olvColumn4.Renderer = new BrightIdeasSoftware.MultiImageRenderer("star", 5, 0, 100);

            this.treeListView1.SetObjects(Song.GetArtists());

   
        }

        private void YMainForm_Activated(object sender, EventArgs e)
        {
            Loader.Global.DisableAccelerators();
        }

        private void YMainForm_Deactivate(object sender, EventArgs e)
        {
            Loader.Global.EnableAccelerators();
        }


        private void button1_Click(object sender, EventArgs e)
        {
            Loader.Global.BroadcastNotification(SystemNotificationCode.FilePreSave);

        }

        private void btnCustom1_Click(object sender, EventArgs e)
        {
            //Utilities.YOList.activeArea.
            //IntPtr unmanagedPointer = Marshal.AllocHGlobal(Utilities.YOList.activeArea.CalculateSize());
            //Marshal.Copy(Utilities.YOList.activeArea., 0, unmanagedPointer, Utilities.YOList.activeArea.CalculateSize());.
            //int number = 1024;

            //unsafe
            //{
            //    //char* c;
            //    byte* p = (byte*)&number;

            //}
            Loader.Global.BroadcastNotification(SystemNotificationCode.Custom1);
        }
    }
}
