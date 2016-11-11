namespace YMax.Forms
{
    partial class AGSidebar
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AGSidebar));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cbIsolateGroup = new System.Windows.Forms.CheckBox();
            this.cbMultiGAction = new System.Windows.Forms.ComboBox();
            this.mulCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button10 = new System.Windows.Forms.Button();
            this.button9 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.runActBtn = new System.Windows.Forms.Button();
            this.actCombo = new System.Windows.Forms.ComboBox();
            this.gtreeListView = new BrightIdeasSoftware.TreeListView();
            this.olvColumnName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.btnApplyView = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gtreeListView)).BeginInit();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "area");
            this.imageList1.Images.SetKeyName(1, "g_green");
            this.imageList1.Images.SetKeyName(2, "g_red");
            this.imageList1.Images.SetKeyName(3, "g_gray");
            this.imageList1.Images.SetKeyName(4, "g_blue");
            this.imageList1.Images.SetKeyName(5, "g_yellow");
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnApplyView);
            this.groupBox1.Controls.Add(this.cbIsolateGroup);
            this.groupBox1.Controls.Add(this.cbMultiGAction);
            this.groupBox1.Controls.Add(this.mulCheckBox);
            this.groupBox1.Location = new System.Drawing.Point(3, 457);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(219, 120);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Display";
            // 
            // cbIsolateGroup
            // 
            this.cbIsolateGroup.AutoSize = true;
            this.cbIsolateGroup.Location = new System.Drawing.Point(7, 49);
            this.cbIsolateGroup.Name = "cbIsolateGroup";
            this.cbIsolateGroup.Size = new System.Drawing.Size(153, 21);
            this.cbIsolateGroup.TabIndex = 6;
            this.cbIsolateGroup.Text = "Isolate group select";
            this.cbIsolateGroup.UseVisualStyleBackColor = true;
            // 
            // cbMultiGAction
            // 
            this.cbMultiGAction.Enabled = false;
            this.cbMultiGAction.FormattingEnabled = true;
            this.cbMultiGAction.Items.AddRange(new object[] {
            "Move to..",
            "Toggle Visible",
            "Remove"});
            this.cbMultiGAction.Location = new System.Drawing.Point(104, 18);
            this.cbMultiGAction.Name = "cbMultiGAction";
            this.cbMultiGAction.Size = new System.Drawing.Size(108, 24);
            this.cbMultiGAction.TabIndex = 5;
            // 
            // mulCheckBox
            // 
            this.mulCheckBox.AutoSize = true;
            this.mulCheckBox.Location = new System.Drawing.Point(7, 21);
            this.mulCheckBox.Name = "mulCheckBox";
            this.mulCheckBox.Size = new System.Drawing.Size(91, 21);
            this.mulCheckBox.TabIndex = 4;
            this.mulCheckBox.Text = "Checkbox";
            this.mulCheckBox.UseVisualStyleBackColor = true;
            this.mulCheckBox.CheckedChanged += new System.EventHandler(this.mulCheckBox_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button10);
            this.groupBox2.Controls.Add(this.button9);
            this.groupBox2.Controls.Add(this.button8);
            this.groupBox2.Controls.Add(this.button7);
            this.groupBox2.Controls.Add(this.button6);
            this.groupBox2.Controls.Add(this.runActBtn);
            this.groupBox2.Controls.Add(this.actCombo);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(3, 368);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(219, 89);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Action";
            // 
            // button10
            // 
            this.button10.Image = global::YMax.Properties.Resources.collapse;
            this.button10.Location = new System.Drawing.Point(183, 21);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(30, 30);
            this.button10.TabIndex = 6;
            this.button10.UseVisualStyleBackColor = true;
            // 
            // button9
            // 
            this.button9.Image = global::YMax.Properties.Resources.edit;
            this.button9.Location = new System.Drawing.Point(87, 21);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(30, 30);
            this.button9.TabIndex = 5;
            this.button9.UseVisualStyleBackColor = true;
            // 
            // button8
            // 
            this.button8.Image = global::YMax.Properties.Resources.trash;
            this.button8.Location = new System.Drawing.Point(123, 21);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(30, 30);
            this.button8.TabIndex = 4;
            this.button8.UseVisualStyleBackColor = true;
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(47, 21);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(34, 30);
            this.button7.TabIndex = 3;
            this.button7.Text = "+g";
            this.button7.UseVisualStyleBackColor = true;
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(7, 21);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(34, 30);
            this.button6.TabIndex = 2;
            this.button6.Text = "+a";
            this.button6.UseVisualStyleBackColor = true;
            // 
            // runActBtn
            // 
            this.runActBtn.Location = new System.Drawing.Point(161, 57);
            this.runActBtn.Name = "runActBtn";
            this.runActBtn.Size = new System.Drawing.Size(52, 26);
            this.runActBtn.TabIndex = 1;
            this.runActBtn.Text = "Run";
            this.runActBtn.UseVisualStyleBackColor = true;
            this.runActBtn.Click += new System.EventHandler(this.runActBtn_Click);
            // 
            // actCombo
            // 
            this.actCombo.FormattingEnabled = true;
            this.actCombo.Items.AddRange(new object[] {
            "Bake Flow",
            "View in Blender with FBX"});
            this.actCombo.Location = new System.Drawing.Point(7, 59);
            this.actCombo.Name = "actCombo";
            this.actCombo.Size = new System.Drawing.Size(148, 24);
            this.actCombo.TabIndex = 0;
            // 
            // gtreeListView
            // 
            this.gtreeListView.AllColumns.Add(this.olvColumnName);
            this.gtreeListView.CellEditUseWholeCell = false;
            this.gtreeListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnName});
            this.gtreeListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.gtreeListView.Location = new System.Drawing.Point(3, 3);
            this.gtreeListView.Name = "gtreeListView";
            this.gtreeListView.ShowGroups = false;
            this.gtreeListView.Size = new System.Drawing.Size(219, 359);
            this.gtreeListView.SmallImageList = this.imageList1;
            this.gtreeListView.TabIndex = 3;
            this.gtreeListView.UseCompatibleStateImageBehavior = false;
            this.gtreeListView.View = System.Windows.Forms.View.Details;
            this.gtreeListView.VirtualMode = true;
            this.gtreeListView.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.gtreeListView_ItemSelectionChanged);
            this.gtreeListView.DoubleClick += new System.EventHandler(this.gtreeListView_DoubleClick);
            // 
            // olvColumnName
            // 
            this.olvColumnName.AspectName = "Name";
            this.olvColumnName.FillsFreeSpace = true;
            this.olvColumnName.Text = "Name";
            // 
            // btnApplyView
            // 
            this.btnApplyView.Location = new System.Drawing.Point(7, 77);
            this.btnApplyView.Name = "btnApplyView";
            this.btnApplyView.Size = new System.Drawing.Size(110, 37);
            this.btnApplyView.TabIndex = 7;
            this.btnApplyView.Text = "View selected";
            this.btnApplyView.UseVisualStyleBackColor = true;
            this.btnApplyView.Click += new System.EventHandler(this.btnApplyView_Click);
            // 
            // AGSidebar
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gtreeListView);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "AGSidebar";
            this.Size = new System.Drawing.Size(225, 580);
            this.Load += new System.EventHandler(this.AGSidebar_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.gtreeListView)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox mulCheckBox;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button runActBtn;
        private System.Windows.Forms.ComboBox actCombo;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.ImageList imageList1;
        private BrightIdeasSoftware.TreeListView gtreeListView;
        private BrightIdeasSoftware.OLVColumn olvColumnName;
        private System.Windows.Forms.ComboBox cbMultiGAction;
        private System.Windows.Forms.CheckBox cbIsolateGroup;
        private System.Windows.Forms.Button btnApplyView;
    }
}
