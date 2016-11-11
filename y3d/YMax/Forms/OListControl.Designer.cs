namespace YMax.Forms
{
    partial class OListControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OListControl));
            this.yoFastListView = new BrightIdeasSoftware.FastObjectListView();
            this.olvColumnName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnFaces = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnType = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnStatus = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.otypeImageList = new System.Windows.Forms.ImageList(this.components);
            this.listCheck = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBoxInGroup = new System.Windows.Forms.CheckBox();
            this.checkBoxIsolate = new System.Windows.Forms.CheckBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripSplitButton1 = new System.Windows.Forms.ToolStripSplitButton();
            this.addToGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.manageGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSplitButton2 = new System.Windows.Forms.ToolStripSplitButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripDropDownButton2 = new System.Windows.Forms.ToolStripDropDownButton();
            this.groupBoxWithSel = new System.Windows.Forms.GroupBox();
            this.lbVerType = new System.Windows.Forms.Label();
            this.button6 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.btnXref = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.yoFastListView)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.groupBoxWithSel.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // yoFastListView
            // 
            this.yoFastListView.AllColumns.Add(this.olvColumnName);
            this.yoFastListView.AllColumns.Add(this.olvColumnFaces);
            this.yoFastListView.AllColumns.Add(this.olvColumnType);
            this.yoFastListView.AllColumns.Add(this.olvColumnStatus);
            this.yoFastListView.CellEditUseWholeCell = false;
            this.yoFastListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnName,
            this.olvColumnFaces,
            this.olvColumnType,
            this.olvColumnStatus});
            this.yoFastListView.Cursor = System.Windows.Forms.Cursors.Default;
            this.yoFastListView.Location = new System.Drawing.Point(3, 30);
            this.yoFastListView.Name = "yoFastListView";
            this.yoFastListView.ShowGroups = false;
            this.yoFastListView.Size = new System.Drawing.Size(444, 400);
            this.yoFastListView.SmallImageList = this.otypeImageList;
            this.yoFastListView.TabIndex = 1;
            this.yoFastListView.UseCellFormatEvents = true;
            this.yoFastListView.UseCompatibleStateImageBehavior = false;
            this.yoFastListView.UseHotItem = true;
            this.yoFastListView.View = System.Windows.Forms.View.Details;
            this.yoFastListView.VirtualMode = true;
            this.yoFastListView.FormatCell += new System.EventHandler<BrightIdeasSoftware.FormatCellEventArgs>(this.yoFastListView_FormatCell);
            this.yoFastListView.FormatRow += new System.EventHandler<BrightIdeasSoftware.FormatRowEventArgs>(this.yoFastListView_FormatRow);
            this.yoFastListView.DoubleClick += new System.EventHandler(this.yoFastListView_DoubleClick);
            // 
            // olvColumnName
            // 
            this.olvColumnName.AspectName = "Name";
            this.olvColumnName.FillsFreeSpace = true;
            this.olvColumnName.Text = "Name";
            this.olvColumnName.Width = 150;
            // 
            // olvColumnFaces
            // 
            this.olvColumnFaces.AspectName = "";
            this.olvColumnFaces.IsEditable = false;
            this.olvColumnFaces.Text = "Faces";
            this.olvColumnFaces.Width = 100;
            // 
            // olvColumnType
            // 
            this.olvColumnType.AspectName = "";
            this.olvColumnType.IsEditable = false;
            this.olvColumnType.Text = "Type";
            this.olvColumnType.Width = 90;
            // 
            // olvColumnStatus
            // 
            this.olvColumnStatus.Text = "Ver Type";
            this.olvColumnStatus.Width = 80;
            // 
            // otypeImageList
            // 
            this.otypeImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("otypeImageList.ImageStream")));
            this.otypeImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.otypeImageList.Images.SetKeyName(0, "other");
            this.otypeImageList.Images.SetKeyName(1, "shape");
            this.otypeImageList.Images.SetKeyName(2, "camera");
            this.otypeImageList.Images.SetKeyName(3, "light");
            this.otypeImageList.Images.SetKeyName(4, "geometry");
            // 
            // listCheck
            // 
            this.listCheck.AutoSize = true;
            this.listCheck.Location = new System.Drawing.Point(6, 21);
            this.listCheck.Name = "listCheck";
            this.listCheck.Size = new System.Drawing.Size(96, 21);
            this.listCheck.TabIndex = 5;
            this.listCheck.Text = "Check Box";
            this.listCheck.UseVisualStyleBackColor = true;
            this.listCheck.CheckedChanged += new System.EventHandler(this.listCheck_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkBoxInGroup);
            this.groupBox1.Controls.Add(this.checkBoxIsolate);
            this.groupBox1.Controls.Add(this.listCheck);
            this.groupBox1.Location = new System.Drawing.Point(4, 436);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(442, 51);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Setting";
            // 
            // checkBoxInGroup
            // 
            this.checkBoxInGroup.AutoSize = true;
            this.checkBoxInGroup.Location = new System.Drawing.Point(108, 21);
            this.checkBoxInGroup.Name = "checkBoxInGroup";
            this.checkBoxInGroup.Size = new System.Drawing.Size(125, 21);
            this.checkBoxInGroup.TabIndex = 7;
            this.checkBoxInGroup.Text = "Select in group";
            this.checkBoxInGroup.UseVisualStyleBackColor = true;
            this.checkBoxInGroup.CheckedChanged += new System.EventHandler(this.checkBoxInGroup_CheckedChanged);
            // 
            // checkBoxIsolate
            // 
            this.checkBoxIsolate.AutoSize = true;
            this.checkBoxIsolate.Location = new System.Drawing.Point(239, 21);
            this.checkBoxIsolate.Name = "checkBoxIsolate";
            this.checkBoxIsolate.Size = new System.Drawing.Size(112, 21);
            this.checkBoxIsolate.TabIndex = 6;
            this.checkBoxIsolate.Text = "Isolate select";
            this.checkBoxIsolate.UseVisualStyleBackColor = true;
            // 
            // toolStrip1
            // 
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSplitButton1,
            this.toolStripSplitButton2,
            this.toolStripButton1,
            this.toolStripDropDownButton1,
            this.toolStripSeparator1,
            this.toolStripDropDownButton2});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(450, 27);
            this.toolStrip1.TabIndex = 4;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripSplitButton1
            // 
            this.toolStripSplitButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSplitButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToGroupToolStripMenuItem,
            this.manageGroupToolStripMenuItem});
            this.toolStripSplitButton1.Image = global::YMax.Properties.Resources.open;
            this.toolStripSplitButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButton1.Name = "toolStripSplitButton1";
            this.toolStripSplitButton1.Size = new System.Drawing.Size(39, 24);
            this.toolStripSplitButton1.Text = "toolStripSplitButton1";
            // 
            // addToGroupToolStripMenuItem
            // 
            this.addToGroupToolStripMenuItem.Name = "addToGroupToolStripMenuItem";
            this.addToGroupToolStripMenuItem.Size = new System.Drawing.Size(183, 26);
            this.addToGroupToolStripMenuItem.Text = "Move to group";
            // 
            // manageGroupToolStripMenuItem
            // 
            this.manageGroupToolStripMenuItem.Name = "manageGroupToolStripMenuItem";
            this.manageGroupToolStripMenuItem.Size = new System.Drawing.Size(183, 26);
            this.manageGroupToolStripMenuItem.Text = "Manage Group";
            // 
            // toolStripSplitButton2
            // 
            this.toolStripSplitButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSplitButton2.Image = global::YMax.Properties.Resources.tag;
            this.toolStripSplitButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButton2.Name = "toolStripSplitButton2";
            this.toolStripSplitButton2.Size = new System.Drawing.Size(39, 24);
            this.toolStripSplitButton2.Text = "toolStripSplitButton2";
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = global::YMax.Properties.Resources.trash;
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(24, 24);
            this.toolStripButton1.Text = "toolStripButton1";
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton1.Image = global::YMax.Properties.Resources.view;
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(34, 24);
            this.toolStripDropDownButton1.Text = "toolStripDropDownButton1";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 27);
            // 
            // toolStripDropDownButton2
            // 
            this.toolStripDropDownButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripDropDownButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton2.Image")));
            this.toolStripDropDownButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton2.Name = "toolStripDropDownButton2";
            this.toolStripDropDownButton2.Size = new System.Drawing.Size(58, 24);
            this.toolStripDropDownButton2.Text = "More";
            // 
            // groupBoxWithSel
            // 
            this.groupBoxWithSel.Controls.Add(this.lbVerType);
            this.groupBoxWithSel.Controls.Add(this.button6);
            this.groupBoxWithSel.Controls.Add(this.button2);
            this.groupBoxWithSel.Controls.Add(this.button1);
            this.groupBoxWithSel.Controls.Add(this.comboBox1);
            this.groupBoxWithSel.Location = new System.Drawing.Point(223, 493);
            this.groupBoxWithSel.Name = "groupBoxWithSel";
            this.groupBoxWithSel.Size = new System.Drawing.Size(224, 118);
            this.groupBoxWithSel.TabIndex = 5;
            this.groupBoxWithSel.TabStop = false;
            this.groupBoxWithSel.Text = "Group Info";
            // 
            // lbVerType
            // 
            this.lbVerType.AutoSize = true;
            this.lbVerType.Location = new System.Drawing.Point(7, 53);
            this.lbVerType.Name = "lbVerType";
            this.lbVerType.Size = new System.Drawing.Size(110, 17);
            this.lbVerType.TabIndex = 4;
            this.lbVerType.Text = "Version type: ---";
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(188, 21);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(26, 26);
            this.button6.TabIndex = 3;
            this.button6.Text = "..";
            this.button6.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(159, 21);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(26, 26);
            this.button2.TabIndex = 2;
            this.button2.Text = "+";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(131, 21);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(26, 26);
            this.button1.TabIndex = 1;
            this.button1.Text = ">";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(7, 22);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 24);
            this.comboBox1.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button4);
            this.groupBox2.Controls.Add(this.button5);
            this.groupBox2.Controls.Add(this.button3);
            this.groupBox2.Controls.Add(this.btnXref);
            this.groupBox2.Location = new System.Drawing.Point(4, 493);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(213, 118);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Object Detail";
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(7, 53);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(124, 23);
            this.button4.TabIndex = 4;
            this.button4.Text = "Version manager";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(7, 82);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(96, 24);
            this.button5.TabIndex = 3;
            this.button5.Text = "Bake normal";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(69, 21);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(112, 24);
            this.button3.TabIndex = 2;
            this.button3.Text = "Make low poly";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // btnXref
            // 
            this.btnXref.Location = new System.Drawing.Point(7, 22);
            this.btnXref.Name = "btnXref";
            this.btnXref.Size = new System.Drawing.Size(56, 24);
            this.btnXref.TabIndex = 0;
            this.btnXref.Text = "Xref";
            this.btnXref.UseVisualStyleBackColor = true;
            this.btnXref.Click += new System.EventHandler(this.btnXref_Click);
            // 
            // OListControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBoxWithSel);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.yoFastListView);
            this.Controls.Add(this.groupBox1);
            this.Name = "OListControl";
            this.Size = new System.Drawing.Size(450, 614);
            ((System.ComponentModel.ISupportInitialize)(this.yoFastListView)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.groupBoxWithSel.ResumeLayout(false);
            this.groupBoxWithSel.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private BrightIdeasSoftware.FastObjectListView yoFastListView;
        private BrightIdeasSoftware.OLVColumn olvColumnName;
        private BrightIdeasSoftware.OLVColumn olvColumnFaces;
        private BrightIdeasSoftware.OLVColumn olvColumnStatus;
        private System.Windows.Forms.CheckBox listCheck;
        private BrightIdeasSoftware.OLVColumn olvColumnType;
        private System.Windows.Forms.ImageList otypeImageList;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButton1;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButton2;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton2;
        private System.Windows.Forms.ToolStripMenuItem addToGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem manageGroupToolStripMenuItem;
        private System.Windows.Forms.CheckBox checkBoxInGroup;
        private System.Windows.Forms.CheckBox checkBoxIsolate;
        private System.Windows.Forms.GroupBox groupBoxWithSel;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button btnXref;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label lbVerType;
    }
}
