namespace Y3D.Forms
{
    partial class ObjectsControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ObjectsControl));
            this.tabNavLeft = new System.Windows.Forms.TabControl();
            this.tabArea = new System.Windows.Forms.TabPage();
            this.gtreeListView = new BrightIdeasSoftware.TreeListView();
            this.olvColumnName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.imageListGroup = new System.Windows.Forms.ImageList(this.components);
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.miniToolStrip = new System.Windows.Forms.ToolStrip();
            this.panel1 = new System.Windows.Forms.Panel();
            this.objectListCtrl = new BrightIdeasSoftware.FastObjectListView();
            this.olvColNameO = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColFace = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColTypeO = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.tabNavLeft.SuspendLayout();
            this.tabArea.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gtreeListView)).BeginInit();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.objectListCtrl)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabNavLeft
            // 
            this.tabNavLeft.Controls.Add(this.tabArea);
            this.tabNavLeft.Controls.Add(this.tabPage2);
            this.tabNavLeft.Location = new System.Drawing.Point(3, 3);
            this.tabNavLeft.Name = "tabNavLeft";
            this.tabNavLeft.SelectedIndex = 0;
            this.tabNavLeft.Size = new System.Drawing.Size(196, 544);
            this.tabNavLeft.TabIndex = 0;
            // 
            // tabArea
            // 
            this.tabArea.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tabArea.Controls.Add(this.gtreeListView);
            this.tabArea.Location = new System.Drawing.Point(4, 22);
            this.tabArea.Name = "tabArea";
            this.tabArea.Padding = new System.Windows.Forms.Padding(3);
            this.tabArea.Size = new System.Drawing.Size(188, 518);
            this.tabArea.TabIndex = 0;
            this.tabArea.Text = "Area";
            // 
            // gtreeListView
            // 
            this.gtreeListView.AllColumns.Add(this.olvColumnName);
            this.gtreeListView.CellEditUseWholeCell = false;
            this.gtreeListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnName});
            this.gtreeListView.Cursor = System.Windows.Forms.Cursors.Default;
            this.gtreeListView.Location = new System.Drawing.Point(0, 0);
            this.gtreeListView.Name = "gtreeListView";
            this.gtreeListView.ShowGroups = false;
            this.gtreeListView.Size = new System.Drawing.Size(188, 422);
            this.gtreeListView.SmallImageList = this.imageListGroup;
            this.gtreeListView.TabIndex = 0;
            this.gtreeListView.UseCompatibleStateImageBehavior = false;
            this.gtreeListView.View = System.Windows.Forms.View.Details;
            this.gtreeListView.VirtualMode = true;
            this.gtreeListView.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.gtreeListView_ItemSelectionChanged);
            // 
            // olvColumnName
            // 
            this.olvColumnName.AspectName = "Name";
            this.olvColumnName.FillsFreeSpace = true;
            this.olvColumnName.Text = "Name";
            // 
            // imageListGroup
            // 
            this.imageListGroup.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListGroup.ImageStream")));
            this.imageListGroup.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListGroup.Images.SetKeyName(0, "area");
            this.imageListGroup.Images.SetKeyName(1, "g_blue");
            this.imageListGroup.Images.SetKeyName(2, "g_gray");
            this.imageListGroup.Images.SetKeyName(3, "g_green");
            this.imageListGroup.Images.SetKeyName(4, "g_red");
            this.imageListGroup.Images.SetKeyName(5, "g_yellow");
            this.imageListGroup.Images.SetKeyName(6, "camera");
            this.imageListGroup.Images.SetKeyName(7, "geometry");
            this.imageListGroup.Images.SetKeyName(8, "light");
            this.imageListGroup.Images.SetKeyName(9, "other");
            this.imageListGroup.Images.SetKeyName(10, "shape");
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(188, 518);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Filter";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // miniToolStrip
            // 
            this.miniToolStrip.AutoSize = false;
            this.miniToolStrip.CanOverflow = false;
            this.miniToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.miniToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.miniToolStrip.Location = new System.Drawing.Point(9, 3);
            this.miniToolStrip.Name = "miniToolStrip";
            this.miniToolStrip.Size = new System.Drawing.Size(542, 25);
            this.miniToolStrip.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.objectListCtrl);
            this.panel1.Controls.Add(this.toolStrip1);
            this.panel1.Location = new System.Drawing.Point(205, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(354, 544);
            this.panel1.TabIndex = 1;
            // 
            // objectListCtrl
            // 
            this.objectListCtrl.AllColumns.Add(this.olvColNameO);
            this.objectListCtrl.AllColumns.Add(this.olvColFace);
            this.objectListCtrl.AllColumns.Add(this.olvColTypeO);
            this.objectListCtrl.CellEditUseWholeCell = false;
            this.objectListCtrl.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColNameO,
            this.olvColFace,
            this.olvColTypeO});
            this.objectListCtrl.Cursor = System.Windows.Forms.Cursors.Default;
            this.objectListCtrl.Location = new System.Drawing.Point(0, 28);
            this.objectListCtrl.Name = "objectListCtrl";
            this.objectListCtrl.ShowGroups = false;
            this.objectListCtrl.Size = new System.Drawing.Size(351, 416);
            this.objectListCtrl.SmallImageList = this.imageListGroup;
            this.objectListCtrl.TabIndex = 2;
            this.objectListCtrl.UseCompatibleStateImageBehavior = false;
            this.objectListCtrl.View = System.Windows.Forms.View.Details;
            this.objectListCtrl.VirtualMode = true;
            this.objectListCtrl.Click += new System.EventHandler(this.objectListCtrl_Click);
            // 
            // olvColNameO
            // 
            this.olvColNameO.AspectName = "Name";
            this.olvColNameO.FillsFreeSpace = true;
            this.olvColNameO.Text = "Object";
            this.olvColNameO.Width = 120;
            // 
            // olvColFace
            // 
            this.olvColFace.Text = "Faces";
            this.olvColFace.Width = 90;
            // 
            // olvColTypeO
            // 
            this.olvColTypeO.Text = "Type";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(354, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton1.Text = "toolStripButton1";
            // 
            // ObjectsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.tabNavLeft);
            this.Name = "ObjectsControl";
            this.Size = new System.Drawing.Size(560, 550);
            this.Load += new System.EventHandler(this.ObjectsControl_Load);
            this.tabNavLeft.ResumeLayout(false);
            this.tabArea.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.gtreeListView)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.objectListCtrl)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabNavLeft;
        private System.Windows.Forms.TabPage tabArea;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ToolStrip miniToolStrip;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private BrightIdeasSoftware.TreeListView gtreeListView;
        private BrightIdeasSoftware.FastObjectListView objectListCtrl;
        private BrightIdeasSoftware.OLVColumn olvColumnName;
        private System.Windows.Forms.ImageList imageListGroup;
        private BrightIdeasSoftware.OLVColumn olvColNameO;
        private BrightIdeasSoftware.OLVColumn olvColFace;
        private BrightIdeasSoftware.OLVColumn olvColTypeO;
    }
}
