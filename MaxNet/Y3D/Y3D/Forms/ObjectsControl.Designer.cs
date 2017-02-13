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
            this.button5 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.gtreeListView = new BrightIdeasSoftware.TreeListView();
            this.olvColumnName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.imageListGroup = new System.Windows.Forms.ImageList(this.components);
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.miniToolStrip = new System.Windows.Forms.ToolStrip();
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.btnAddJob = new System.Windows.Forms.Button();
            this.btnAddTest = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBoxInGroup = new System.Windows.Forms.CheckBox();
            this.checkBoxIsolate = new System.Windows.Forms.CheckBox();
            this.listCheck = new System.Windows.Forms.CheckBox();
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
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
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
            this.tabArea.Controls.Add(this.button5);
            this.tabArea.Controls.Add(this.button4);
            this.tabArea.Controls.Add(this.gtreeListView);
            this.tabArea.Location = new System.Drawing.Point(4, 22);
            this.tabArea.Name = "tabArea";
            this.tabArea.Padding = new System.Windows.Forms.Padding(3);
            this.tabArea.Size = new System.Drawing.Size(188, 518);
            this.tabArea.TabIndex = 0;
            this.tabArea.Text = "Area";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(61, 313);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(54, 23);
            this.button5.TabIndex = 2;
            this.button5.Text = "+ Group";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(7, 313);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(48, 23);
            this.button4.TabIndex = 1;
            this.button4.Text = "+ Area";
            this.button4.UseVisualStyleBackColor = true;
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
            this.gtreeListView.Size = new System.Drawing.Size(188, 306);
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
            this.panel1.Controls.Add(this.groupBox3);
            this.panel1.Controls.Add(this.groupBox2);
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Controls.Add(this.objectListCtrl);
            this.panel1.Controls.Add(this.toolStrip1);
            this.panel1.Location = new System.Drawing.Point(205, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(354, 544);
            this.panel1.TabIndex = 1;
            // 
            // groupBox3
            // 
            this.groupBox3.Location = new System.Drawing.Point(3, 440);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(348, 100);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Object";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button3);
            this.groupBox2.Controls.Add(this.btnAddJob);
            this.groupBox2.Controls.Add(this.btnAddTest);
            this.groupBox2.Location = new System.Drawing.Point(3, 379);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(348, 55);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Actions";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(110, 20);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(79, 23);
            this.button3.TabIndex = 2;
            this.button3.Text = "Load Version";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // btnAddJob
            // 
            this.btnAddJob.Location = new System.Drawing.Point(60, 20);
            this.btnAddJob.Name = "btnAddJob";
            this.btnAddJob.Size = new System.Drawing.Size(44, 23);
            this.btnAddJob.TabIndex = 1;
            this.btnAddJob.Text = "+ Job";
            this.btnAddJob.UseVisualStyleBackColor = true;
            // 
            // btnAddTest
            // 
            this.btnAddTest.Location = new System.Drawing.Point(7, 20);
            this.btnAddTest.Name = "btnAddTest";
            this.btnAddTest.Size = new System.Drawing.Size(47, 23);
            this.btnAddTest.TabIndex = 0;
            this.btnAddTest.Text = "+ Test";
            this.btnAddTest.UseVisualStyleBackColor = true;
            this.btnAddTest.Click += new System.EventHandler(this.btnAddTest_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkBoxInGroup);
            this.groupBox1.Controls.Add(this.checkBoxIsolate);
            this.groupBox1.Controls.Add(this.listCheck);
            this.groupBox1.Location = new System.Drawing.Point(2, 333);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(2);
            this.groupBox1.Size = new System.Drawing.Size(349, 41);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Setting";
            // 
            // checkBoxInGroup
            // 
            this.checkBoxInGroup.AutoSize = true;
            this.checkBoxInGroup.Location = new System.Drawing.Point(81, 17);
            this.checkBoxInGroup.Margin = new System.Windows.Forms.Padding(2);
            this.checkBoxInGroup.Name = "checkBoxInGroup";
            this.checkBoxInGroup.Size = new System.Drawing.Size(97, 17);
            this.checkBoxInGroup.TabIndex = 7;
            this.checkBoxInGroup.Text = "Select in group";
            this.checkBoxInGroup.UseVisualStyleBackColor = true;
            // 
            // checkBoxIsolate
            // 
            this.checkBoxIsolate.AutoSize = true;
            this.checkBoxIsolate.Location = new System.Drawing.Point(179, 17);
            this.checkBoxIsolate.Margin = new System.Windows.Forms.Padding(2);
            this.checkBoxIsolate.Name = "checkBoxIsolate";
            this.checkBoxIsolate.Size = new System.Drawing.Size(88, 17);
            this.checkBoxIsolate.TabIndex = 6;
            this.checkBoxIsolate.Text = "Isolate select";
            this.checkBoxIsolate.UseVisualStyleBackColor = true;
            this.checkBoxIsolate.CheckedChanged += new System.EventHandler(this.checkBoxIsolate_CheckedChanged);
            // 
            // listCheck
            // 
            this.listCheck.AutoSize = true;
            this.listCheck.Location = new System.Drawing.Point(4, 17);
            this.listCheck.Margin = new System.Windows.Forms.Padding(2);
            this.listCheck.Name = "listCheck";
            this.listCheck.Size = new System.Drawing.Size(78, 17);
            this.listCheck.TabIndex = 5;
            this.listCheck.Text = "Check Box";
            this.listCheck.UseVisualStyleBackColor = true;
            this.listCheck.CheckedChanged += new System.EventHandler(this.listCheck_CheckedChanged);
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
            this.objectListCtrl.Size = new System.Drawing.Size(351, 300);
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
            this.toolStripButton1.Image = global::Y3D.Properties.Resources.tag16;
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton1.Text = "toolStripButton1";
            this.toolStripButton1.ToolTipText = "Tags";
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
            this.groupBox2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxInGroup;
        private System.Windows.Forms.CheckBox checkBoxIsolate;
        private System.Windows.Forms.CheckBox listCheck;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button btnAddJob;
        private System.Windows.Forms.Button btnAddTest;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button4;
    }
}
