namespace Y3D.Projects
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
            this.btnPreview = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.button5 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.gtreeListView = new BrightIdeasSoftware.TreeListView();
            this.olvColumnName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.imageListGroup = new System.Windows.Forms.ImageList(this.components);
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.miniToolStrip = new System.Windows.Forms.ToolStrip();
            this.panelObjList = new System.Windows.Forms.Panel();
            this.objectListCtrl = new BrightIdeasSoftware.FastObjectListView();
            this.olvColNameO = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColFace = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColTypeO = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripInfo = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageTest = new System.Windows.Forms.TabPage();
            this.olvLocalTest = new BrightIdeasSoftware.FastObjectListView();
            this.olvId = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvNote = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvInScene = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.tabPageVersion = new System.Windows.Forms.TabPage();
            this.btnTestLoad = new System.Windows.Forms.Button();
            this.btnTestSetting = new System.Windows.Forms.Button();
            this.btnTestEdit = new System.Windows.Forms.Button();
            this.btnTestDel = new System.Windows.Forms.Button();
            this.btnAddTest = new System.Windows.Forms.Button();
            this.checkBoxIsolate = new System.Windows.Forms.CheckBox();
            this.checkBoxInGroup = new System.Windows.Forms.CheckBox();
            this.listCheck = new System.Windows.Forms.CheckBox();
            this.panelEditTest = new System.Windows.Forms.Panel();
            this.btnSave2File = new System.Windows.Forms.Button();
            this.testDetailControl1 = new Y3D.Projects.TestDetailControl();
            this.btnBack = new System.Windows.Forms.Button();
            this.tabNavLeft.SuspendLayout();
            this.tabArea.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gtreeListView)).BeginInit();
            this.panelObjList.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.objectListCtrl)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPageTest.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.olvLocalTest)).BeginInit();
            this.panelEditTest.SuspendLayout();
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
            this.tabArea.Controls.Add(this.btnPreview);
            this.tabArea.Controls.Add(this.label1);
            this.tabArea.Controls.Add(this.pictureBox1);
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
            // btnPreview
            // 
            this.btnPreview.Image = global::Y3D.Properties.Resources.start;
            this.btnPreview.Location = new System.Drawing.Point(58, 348);
            this.btnPreview.Name = "btnPreview";
            this.btnPreview.Size = new System.Drawing.Size(20, 20);
            this.btnPreview.TabIndex = 5;
            this.btnPreview.UseVisualStyleBackColor = true;
            this.btnPreview.Click += new System.EventHandler(this.btnPreview_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 352);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Preview";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBox1.ImageLocation = "";
            this.pictureBox1.InitialImage = global::Y3D.Properties.Resources.noimage;
            this.pictureBox1.Location = new System.Drawing.Point(7, 371);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(175, 141);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
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
            // panelObjList
            // 
            this.panelObjList.Controls.Add(this.objectListCtrl);
            this.panelObjList.Controls.Add(this.toolStrip1);
            this.panelObjList.Controls.Add(this.panel2);
            this.panelObjList.Controls.Add(this.checkBoxIsolate);
            this.panelObjList.Controls.Add(this.checkBoxInGroup);
            this.panelObjList.Controls.Add(this.listCheck);
            this.panelObjList.Location = new System.Drawing.Point(205, 0);
            this.panelObjList.Name = "panelObjList";
            this.panelObjList.Size = new System.Drawing.Size(290, 544);
            this.panelObjList.TabIndex = 1;
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
            this.objectListCtrl.Location = new System.Drawing.Point(3, 28);
            this.objectListCtrl.Name = "objectListCtrl";
            this.objectListCtrl.ShowGroups = false;
            this.objectListCtrl.Size = new System.Drawing.Size(290, 300);
            this.objectListCtrl.SmallImageList = this.imageListGroup;
            this.objectListCtrl.TabIndex = 2;
            this.objectListCtrl.UseCompatibleStateImageBehavior = false;
            this.objectListCtrl.View = System.Windows.Forms.View.Details;
            this.objectListCtrl.VirtualMode = true;
            this.objectListCtrl.ItemsChanged += new System.EventHandler<BrightIdeasSoftware.ItemsChangedEventArgs>(this.objectListCtrl_ItemsChanged);
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
            this.toolStripInfo,
            this.toolStripButton1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(290, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripInfo
            // 
            this.toolStripInfo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripInfo.Image = global::Y3D.Properties.Resources.info_16;
            this.toolStripInfo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripInfo.Name = "toolStripInfo";
            this.toolStripInfo.Size = new System.Drawing.Size(23, 22);
            this.toolStripInfo.Text = "toolStripButton2";
            this.toolStripInfo.Click += new System.EventHandler(this.toolStripInfo_Click);
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
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.panel2.Controls.Add(this.tabControl1);
            this.panel2.Controls.Add(this.btnTestLoad);
            this.panel2.Controls.Add(this.btnTestSetting);
            this.panel2.Controls.Add(this.btnTestEdit);
            this.panel2.Controls.Add(this.btnTestDel);
            this.panel2.Controls.Add(this.btnAddTest);
            this.panel2.Location = new System.Drawing.Point(3, 360);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(290, 184);
            this.panel2.TabIndex = 8;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPageTest);
            this.tabControl1.Controls.Add(this.tabPageVersion);
            this.tabControl1.Location = new System.Drawing.Point(3, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(284, 122);
            this.tabControl1.TabIndex = 4;
            // 
            // tabPageTest
            // 
            this.tabPageTest.Controls.Add(this.olvLocalTest);
            this.tabPageTest.Location = new System.Drawing.Point(4, 22);
            this.tabPageTest.Name = "tabPageTest";
            this.tabPageTest.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTest.Size = new System.Drawing.Size(276, 96);
            this.tabPageTest.TabIndex = 0;
            this.tabPageTest.Text = "Local Test";
            this.tabPageTest.UseVisualStyleBackColor = true;
            // 
            // olvLocalTest
            // 
            this.olvLocalTest.AllColumns.Add(this.olvId);
            this.olvLocalTest.AllColumns.Add(this.olvNote);
            this.olvLocalTest.AllColumns.Add(this.olvInScene);
            this.olvLocalTest.CellEditUseWholeCell = false;
            this.olvLocalTest.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvId,
            this.olvNote,
            this.olvInScene});
            this.olvLocalTest.Cursor = System.Windows.Forms.Cursors.Default;
            this.olvLocalTest.FullRowSelect = true;
            this.olvLocalTest.Location = new System.Drawing.Point(3, 3);
            this.olvLocalTest.Name = "olvLocalTest";
            this.olvLocalTest.ShowGroups = false;
            this.olvLocalTest.Size = new System.Drawing.Size(270, 90);
            this.olvLocalTest.SmallImageList = this.imageListGroup;
            this.olvLocalTest.TabIndex = 3;
            this.olvLocalTest.UseCompatibleStateImageBehavior = false;
            this.olvLocalTest.View = System.Windows.Forms.View.Details;
            this.olvLocalTest.VirtualMode = true;
            this.olvLocalTest.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.olvLocalTest_MouseDoubleClick);
            // 
            // olvId
            // 
            this.olvId.AspectName = "Id";
            this.olvId.Text = "ID";
            this.olvId.Width = 75;
            // 
            // olvNote
            // 
            this.olvNote.AspectName = "Vnote";
            this.olvNote.FillsFreeSpace = true;
            this.olvNote.Text = "Note";
            this.olvNote.Width = 80;
            // 
            // olvInScene
            // 
            this.olvInScene.Text = "In scene";
            // 
            // tabPageVersion
            // 
            this.tabPageVersion.Location = new System.Drawing.Point(4, 22);
            this.tabPageVersion.Name = "tabPageVersion";
            this.tabPageVersion.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageVersion.Size = new System.Drawing.Size(276, 96);
            this.tabPageVersion.TabIndex = 1;
            this.tabPageVersion.Text = "Version";
            this.tabPageVersion.UseVisualStyleBackColor = true;
            // 
            // btnTestLoad
            // 
            this.btnTestLoad.Image = global::Y3D.Properties.Resources.open_16;
            this.btnTestLoad.Location = new System.Drawing.Point(232, 131);
            this.btnTestLoad.Name = "btnTestLoad";
            this.btnTestLoad.Size = new System.Drawing.Size(23, 23);
            this.btnTestLoad.TabIndex = 8;
            this.btnTestLoad.UseVisualStyleBackColor = true;
            // 
            // btnTestSetting
            // 
            this.btnTestSetting.Image = global::Y3D.Properties.Resources.setting_16;
            this.btnTestSetting.Location = new System.Drawing.Point(261, 131);
            this.btnTestSetting.Name = "btnTestSetting";
            this.btnTestSetting.Size = new System.Drawing.Size(23, 23);
            this.btnTestSetting.TabIndex = 7;
            this.btnTestSetting.UseVisualStyleBackColor = true;
            // 
            // btnTestEdit
            // 
            this.btnTestEdit.Image = global::Y3D.Properties.Resources.edit_16;
            this.btnTestEdit.Location = new System.Drawing.Point(65, 131);
            this.btnTestEdit.Name = "btnTestEdit";
            this.btnTestEdit.Size = new System.Drawing.Size(51, 23);
            this.btnTestEdit.TabIndex = 6;
            this.btnTestEdit.UseVisualStyleBackColor = true;
            this.btnTestEdit.Click += new System.EventHandler(this.btnTestEdit_Click);
            // 
            // btnTestDel
            // 
            this.btnTestDel.Image = global::Y3D.Properties.Resources.trash16;
            this.btnTestDel.Location = new System.Drawing.Point(122, 131);
            this.btnTestDel.Name = "btnTestDel";
            this.btnTestDel.Size = new System.Drawing.Size(23, 23);
            this.btnTestDel.TabIndex = 5;
            this.btnTestDel.UseVisualStyleBackColor = true;
            this.btnTestDel.Click += new System.EventHandler(this.btnTestDel_Click);
            // 
            // btnAddTest
            // 
            this.btnAddTest.BackColor = System.Drawing.Color.DarkSeaGreen;
            this.btnAddTest.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddTest.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddTest.ForeColor = System.Drawing.Color.White;
            this.btnAddTest.Location = new System.Drawing.Point(8, 131);
            this.btnAddTest.Name = "btnAddTest";
            this.btnAddTest.Size = new System.Drawing.Size(51, 23);
            this.btnAddTest.TabIndex = 0;
            this.btnAddTest.Text = "+ Test";
            this.btnAddTest.UseVisualStyleBackColor = false;
            this.btnAddTest.Click += new System.EventHandler(this.btnAddTest_Click);
            // 
            // checkBoxIsolate
            // 
            this.checkBoxIsolate.AutoSize = true;
            this.checkBoxIsolate.Location = new System.Drawing.Point(6, 333);
            this.checkBoxIsolate.Margin = new System.Windows.Forms.Padding(2);
            this.checkBoxIsolate.Name = "checkBoxIsolate";
            this.checkBoxIsolate.Size = new System.Drawing.Size(88, 17);
            this.checkBoxIsolate.TabIndex = 6;
            this.checkBoxIsolate.Text = "Isolate select";
            this.checkBoxIsolate.UseVisualStyleBackColor = true;
            this.checkBoxIsolate.CheckedChanged += new System.EventHandler(this.checkBoxIsolate_CheckedChanged);
            // 
            // checkBoxInGroup
            // 
            this.checkBoxInGroup.AutoSize = true;
            this.checkBoxInGroup.Location = new System.Drawing.Point(98, 333);
            this.checkBoxInGroup.Margin = new System.Windows.Forms.Padding(2);
            this.checkBoxInGroup.Name = "checkBoxInGroup";
            this.checkBoxInGroup.Size = new System.Drawing.Size(97, 17);
            this.checkBoxInGroup.TabIndex = 7;
            this.checkBoxInGroup.Text = "Select in group";
            this.checkBoxInGroup.UseVisualStyleBackColor = true;
            this.checkBoxInGroup.CheckedChanged += new System.EventHandler(this.checkBoxInGroup_CheckedChanged);
            // 
            // listCheck
            // 
            this.listCheck.AutoSize = true;
            this.listCheck.Location = new System.Drawing.Point(199, 333);
            this.listCheck.Margin = new System.Windows.Forms.Padding(2);
            this.listCheck.Name = "listCheck";
            this.listCheck.Size = new System.Drawing.Size(78, 17);
            this.listCheck.TabIndex = 5;
            this.listCheck.Text = "Check Box";
            this.listCheck.UseVisualStyleBackColor = true;
            this.listCheck.CheckedChanged += new System.EventHandler(this.listCheck_CheckedChanged);
            // 
            // panelEditTest
            // 
            this.panelEditTest.Controls.Add(this.btnSave2File);
            this.panelEditTest.Controls.Add(this.testDetailControl1);
            this.panelEditTest.Controls.Add(this.btnBack);
            this.panelEditTest.Location = new System.Drawing.Point(205, 3);
            this.panelEditTest.Name = "panelEditTest";
            this.panelEditTest.Size = new System.Drawing.Size(290, 538);
            this.panelEditTest.TabIndex = 9;
            // 
            // btnSave2File
            // 
            this.btnSave2File.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.btnSave2File.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSave2File.Image = global::Y3D.Properties.Resources.save;
            this.btnSave2File.Location = new System.Drawing.Point(81, 3);
            this.btnSave2File.Name = "btnSave2File";
            this.btnSave2File.Size = new System.Drawing.Size(38, 32);
            this.btnSave2File.TabIndex = 8;
            this.btnSave2File.UseVisualStyleBackColor = false;
            this.btnSave2File.Click += new System.EventHandler(this.btnSave2File_Click);
            // 
            // testDetailControl1
            // 
            this.testDetailControl1.Location = new System.Drawing.Point(10, 41);
            this.testDetailControl1.Name = "testDetailControl1";
            this.testDetailControl1.Size = new System.Drawing.Size(276, 494);
            this.testDetailControl1.TabIndex = 7;
            // 
            // btnBack
            // 
            this.btnBack.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.btnBack.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnBack.Image = global::Y3D.Properties.Resources.back16;
            this.btnBack.Location = new System.Drawing.Point(11, 3);
            this.btnBack.Name = "btnBack";
            this.btnBack.Size = new System.Drawing.Size(64, 32);
            this.btnBack.TabIndex = 6;
            this.btnBack.Text = "Back";
            this.btnBack.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnBack.UseVisualStyleBackColor = false;
            this.btnBack.Click += new System.EventHandler(this.btnBack_Click);
            // 
            // ObjectsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabNavLeft);
            this.Controls.Add(this.panelObjList);
            this.Controls.Add(this.panelEditTest);
            this.Name = "ObjectsControl";
            this.Size = new System.Drawing.Size(505, 550);
            this.Load += new System.EventHandler(this.ObjectsControl_Load);
            this.tabNavLeft.ResumeLayout(false);
            this.tabArea.ResumeLayout(false);
            this.tabArea.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gtreeListView)).EndInit();
            this.panelObjList.ResumeLayout(false);
            this.panelObjList.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.objectListCtrl)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPageTest.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.olvLocalTest)).EndInit();
            this.panelEditTest.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabNavLeft;
        private System.Windows.Forms.TabPage tabArea;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ToolStrip miniToolStrip;
        private System.Windows.Forms.Panel panelObjList;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private BrightIdeasSoftware.TreeListView gtreeListView;
        private BrightIdeasSoftware.FastObjectListView objectListCtrl;
        private BrightIdeasSoftware.OLVColumn olvColumnName;
        private System.Windows.Forms.ImageList imageListGroup;
        private BrightIdeasSoftware.OLVColumn olvColNameO;
        private BrightIdeasSoftware.OLVColumn olvColFace;
        private BrightIdeasSoftware.OLVColumn olvColTypeO;
        private System.Windows.Forms.CheckBox checkBoxInGroup;
        private System.Windows.Forms.CheckBox checkBoxIsolate;
        private System.Windows.Forms.CheckBox listCheck;
        private System.Windows.Forms.Button btnAddTest;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button4;
        private BrightIdeasSoftware.FastObjectListView olvLocalTest;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button btnPreview;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageTest;
        private System.Windows.Forms.TabPage tabPageVersion;
        private System.Windows.Forms.Button btnTestDel;
        private BrightIdeasSoftware.OLVColumn olvId;
        private BrightIdeasSoftware.OLVColumn olvNote;
        private System.Windows.Forms.Button btnTestEdit;
        private System.Windows.Forms.Button btnTestSetting;
        private System.Windows.Forms.Button btnTestLoad;
        private System.Windows.Forms.ToolStripButton toolStripInfo;
        private System.Windows.Forms.Panel panelEditTest;
        private System.Windows.Forms.Button btnBack;
        private BrightIdeasSoftware.OLVColumn olvInScene;
        private TestDetailControl testDetailControl1;
        private System.Windows.Forms.Button btnSave2File;
    }
}
