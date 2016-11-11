namespace YMax.Forms
{
    partial class FilterSidebar
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
            this.checkShape = new System.Windows.Forms.CheckBox();
            this.checkGeo = new System.Windows.Forms.CheckBox();
            this.checkLight = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnHCInvert = new System.Windows.Forms.Button();
            this.btnHCNone = new System.Windows.Forms.Button();
            this.btnHCAll = new System.Windows.Forms.Button();
            this.checkCam = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.olvFaceRange = new BrightIdeasSoftware.ObjectListView();
            this.olvColumnRange = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnColor = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.btnEditFRange = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.objectListView1 = new BrightIdeasSoftware.ObjectListView();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.btnFilterSearch = new System.Windows.Forms.Button();
            this.cbExclude = new System.Windows.Forms.CheckBox();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.olvFaceRange)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.objectListView1)).BeginInit();
            this.SuspendLayout();
            // 
            // checkShape
            // 
            this.checkShape.AutoSize = true;
            this.checkShape.Location = new System.Drawing.Point(6, 75);
            this.checkShape.Name = "checkShape";
            this.checkShape.Size = new System.Drawing.Size(71, 21);
            this.checkShape.TabIndex = 2;
            this.checkShape.Text = "Shape";
            this.checkShape.UseVisualStyleBackColor = true;
            // 
            // checkGeo
            // 
            this.checkGeo.AutoSize = true;
            this.checkGeo.Location = new System.Drawing.Point(6, 21);
            this.checkGeo.Name = "checkGeo";
            this.checkGeo.Size = new System.Drawing.Size(92, 21);
            this.checkGeo.TabIndex = 0;
            this.checkGeo.Text = "Geometry";
            this.checkGeo.UseVisualStyleBackColor = true;
            // 
            // checkLight
            // 
            this.checkLight.AutoSize = true;
            this.checkLight.Location = new System.Drawing.Point(6, 48);
            this.checkLight.Name = "checkLight";
            this.checkLight.Size = new System.Drawing.Size(61, 21);
            this.checkLight.TabIndex = 1;
            this.checkLight.Text = "Light";
            this.checkLight.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnHCInvert);
            this.groupBox2.Controls.Add(this.btnHCNone);
            this.groupBox2.Controls.Add(this.btnHCAll);
            this.groupBox2.Controls.Add(this.checkCam);
            this.groupBox2.Controls.Add(this.checkShape);
            this.groupBox2.Controls.Add(this.checkLight);
            this.groupBox2.Controls.Add(this.checkGeo);
            this.groupBox2.Location = new System.Drawing.Point(3, 75);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(219, 131);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Hide by category";
            // 
            // btnHCInvert
            // 
            this.btnHCInvert.Location = new System.Drawing.Point(158, 73);
            this.btnHCInvert.Name = "btnHCInvert";
            this.btnHCInvert.Size = new System.Drawing.Size(55, 23);
            this.btnHCInvert.TabIndex = 6;
            this.btnHCInvert.Text = "Invert";
            this.btnHCInvert.UseVisualStyleBackColor = true;
            // 
            // btnHCNone
            // 
            this.btnHCNone.Location = new System.Drawing.Point(158, 46);
            this.btnHCNone.Name = "btnHCNone";
            this.btnHCNone.Size = new System.Drawing.Size(55, 23);
            this.btnHCNone.TabIndex = 5;
            this.btnHCNone.Text = "None";
            this.btnHCNone.UseVisualStyleBackColor = true;
            // 
            // btnHCAll
            // 
            this.btnHCAll.Location = new System.Drawing.Point(158, 21);
            this.btnHCAll.Name = "btnHCAll";
            this.btnHCAll.Size = new System.Drawing.Size(55, 23);
            this.btnHCAll.TabIndex = 4;
            this.btnHCAll.Text = "All";
            this.btnHCAll.UseVisualStyleBackColor = true;
            // 
            // checkCam
            // 
            this.checkCam.AutoSize = true;
            this.checkCam.Location = new System.Drawing.Point(6, 102);
            this.checkCam.Name = "checkCam";
            this.checkCam.Size = new System.Drawing.Size(79, 21);
            this.checkCam.TabIndex = 3;
            this.checkCam.Text = "Camera";
            this.checkCam.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.olvFaceRange);
            this.groupBox1.Controls.Add(this.btnEditFRange);
            this.groupBox1.Location = new System.Drawing.Point(3, 212);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(219, 191);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Show by face count";
            // 
            // olvFaceRange
            // 
            this.olvFaceRange.AllColumns.Add(this.olvColumnRange);
            this.olvFaceRange.AllColumns.Add(this.olvColumnColor);
            this.olvFaceRange.CellEditUseWholeCell = false;
            this.olvFaceRange.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnRange,
            this.olvColumnColor});
            this.olvFaceRange.Cursor = System.Windows.Forms.Cursors.Default;
            this.olvFaceRange.Location = new System.Drawing.Point(6, 21);
            this.olvFaceRange.Name = "olvFaceRange";
            this.olvFaceRange.Size = new System.Drawing.Size(206, 133);
            this.olvFaceRange.TabIndex = 2;
            this.olvFaceRange.UseCompatibleStateImageBehavior = false;
            this.olvFaceRange.View = System.Windows.Forms.View.Details;
            // 
            // olvColumnRange
            // 
            this.olvColumnRange.FillsFreeSpace = true;
            this.olvColumnRange.Text = "Faces";
            this.olvColumnRange.Width = 140;
            // 
            // olvColumnColor
            // 
            this.olvColumnColor.Text = "Color";
            this.olvColumnColor.Width = 50;
            // 
            // btnEditFRange
            // 
            this.btnEditFRange.Location = new System.Drawing.Point(6, 160);
            this.btnEditFRange.Name = "btnEditFRange";
            this.btnEditFRange.Size = new System.Drawing.Size(124, 24);
            this.btnEditFRange.TabIndex = 1;
            this.btnEditFRange.Text = "Edit face range";
            this.btnEditFRange.UseVisualStyleBackColor = true;
            this.btnEditFRange.Click += new System.EventHandler(this.btnEditFRange_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.button1);
            this.groupBox3.Controls.Add(this.objectListView1);
            this.groupBox3.Location = new System.Drawing.Point(3, 409);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(219, 168);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Show by Area (cm2)";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(6, 138);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(124, 24);
            this.button1.TabIndex = 3;
            this.button1.Text = "Edit area range";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // objectListView1
            // 
            this.objectListView1.CellEditUseWholeCell = false;
            this.objectListView1.Cursor = System.Windows.Forms.Cursors.Default;
            this.objectListView1.Location = new System.Drawing.Point(6, 21);
            this.objectListView1.Name = "objectListView1";
            this.objectListView1.Size = new System.Drawing.Size(204, 111);
            this.objectListView1.TabIndex = 0;
            this.objectListView1.UseCompatibleStateImageBehavior = false;
            this.objectListView1.View = System.Windows.Forms.View.Details;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(9, 12);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(149, 22);
            this.textBox1.TabIndex = 6;
            // 
            // btnFilterSearch
            // 
            this.btnFilterSearch.Image = global::YMax.Properties.Resources.search;
            this.btnFilterSearch.Location = new System.Drawing.Point(160, 12);
            this.btnFilterSearch.Name = "btnFilterSearch";
            this.btnFilterSearch.Size = new System.Drawing.Size(55, 23);
            this.btnFilterSearch.TabIndex = 7;
            this.btnFilterSearch.UseVisualStyleBackColor = true;
            // 
            // cbExclude
            // 
            this.cbExclude.AutoSize = true;
            this.cbExclude.Location = new System.Drawing.Point(9, 41);
            this.cbExclude.Name = "cbExclude";
            this.cbExclude.Size = new System.Drawing.Size(79, 21);
            this.cbExclude.TabIndex = 8;
            this.cbExclude.Text = "Exclude";
            this.cbExclude.UseVisualStyleBackColor = true;
            // 
            // FilterSidebar
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.cbExclude);
            this.Controls.Add(this.btnFilterSearch);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Name = "FilterSidebar";
            this.Size = new System.Drawing.Size(225, 580);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.olvFaceRange)).EndInit();
            this.groupBox3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.objectListView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkShape;
        private System.Windows.Forms.CheckBox checkGeo;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox checkCam;
        private System.Windows.Forms.Button btnHCInvert;
        private System.Windows.Forms.Button btnHCNone;
        private System.Windows.Forms.Button btnHCAll;
        private System.Windows.Forms.GroupBox groupBox1;
        private BrightIdeasSoftware.ObjectListView olvFaceRange;
        private System.Windows.Forms.Button btnEditFRange;
        private BrightIdeasSoftware.OLVColumn olvColumnRange;
        private BrightIdeasSoftware.OLVColumn olvColumnColor;
        private System.Windows.Forms.CheckBox checkLight;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button1;
        private BrightIdeasSoftware.ObjectListView objectListView1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button btnFilterSearch;
        private System.Windows.Forms.CheckBox cbExclude;
    }
}
