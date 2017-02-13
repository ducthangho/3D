namespace Y3D.Forms
{
    partial class ProjectControl
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
            this.btnDelP = new System.Windows.Forms.Button();
            this.btnEditP = new System.Windows.Forms.Button();
            this.btnLoadP = new System.Windows.Forms.Button();
            this.dlvListProject = new BrightIdeasSoftware.DataListView();
            this.olvColumnPName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnPath = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.btnNewBlank = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btnNewFromMax = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dlvListProject)).BeginInit();
            this.SuspendLayout();
            // 
            // btnDelP
            // 
            this.btnDelP.Location = new System.Drawing.Point(242, 433);
            this.btnDelP.Name = "btnDelP";
            this.btnDelP.Size = new System.Drawing.Size(37, 23);
            this.btnDelP.TabIndex = 7;
            this.btnDelP.Text = "Del";
            this.btnDelP.UseVisualStyleBackColor = true;
            this.btnDelP.Click += new System.EventHandler(this.btnDelP_Click);
            // 
            // btnEditP
            // 
            this.btnEditP.Location = new System.Drawing.Point(190, 433);
            this.btnEditP.Name = "btnEditP";
            this.btnEditP.Size = new System.Drawing.Size(46, 23);
            this.btnEditP.TabIndex = 6;
            this.btnEditP.Text = "Edit";
            this.btnEditP.UseVisualStyleBackColor = true;
            this.btnEditP.Click += new System.EventHandler(this.btnEditP_Click);
            // 
            // btnLoadP
            // 
            this.btnLoadP.Location = new System.Drawing.Point(108, 433);
            this.btnLoadP.Name = "btnLoadP";
            this.btnLoadP.Size = new System.Drawing.Size(75, 23);
            this.btnLoadP.TabIndex = 5;
            this.btnLoadP.Text = "Load Project";
            this.btnLoadP.UseVisualStyleBackColor = true;
            this.btnLoadP.Click += new System.EventHandler(this.btnLoadP_Click);
            // 
            // dlvListProject
            // 
            this.dlvListProject.AllColumns.Add(this.olvColumnPName);
            this.dlvListProject.AllColumns.Add(this.olvColumnPath);
            this.dlvListProject.CellEditUseWholeCell = false;
            this.dlvListProject.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnPName,
            this.olvColumnPath});
            this.dlvListProject.Cursor = System.Windows.Forms.Cursors.Default;
            this.dlvListProject.DataSource = null;
            this.dlvListProject.FullRowSelect = true;
            this.dlvListProject.Location = new System.Drawing.Point(108, 239);
            this.dlvListProject.Name = "dlvListProject";
            this.dlvListProject.Size = new System.Drawing.Size(349, 188);
            this.dlvListProject.TabIndex = 4;
            this.dlvListProject.UseCompatibleStateImageBehavior = false;
            this.dlvListProject.View = System.Windows.Forms.View.Details;
            this.dlvListProject.SelectedIndexChanged += new System.EventHandler(this.dlvListProject_SelectedIndexChanged);
            // 
            // olvColumnPName
            // 
            this.olvColumnPName.AspectName = "Pname";
            this.olvColumnPName.Groupable = false;
            this.olvColumnPName.HeaderFont = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.olvColumnPName.Text = "Project";
            this.olvColumnPName.Width = 150;
            // 
            // olvColumnPath
            // 
            this.olvColumnPath.FillsFreeSpace = true;
            this.olvColumnPath.Groupable = false;
            this.olvColumnPath.Text = "Path";
            // 
            // btnNewBlank
            // 
            this.btnNewBlank.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnNewBlank.Location = new System.Drawing.Point(169, 92);
            this.btnNewBlank.Name = "btnNewBlank";
            this.btnNewBlank.Size = new System.Drawing.Size(110, 84);
            this.btnNewBlank.TabIndex = 8;
            this.btnNewBlank.Text = "New Blank Project";
            this.btnNewBlank.UseVisualStyleBackColor = true;
            this.btnNewBlank.Click += new System.EventHandler(this.btnNewBlank_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(258, 196);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(43, 25);
            this.label1.TabIndex = 9;
            this.label1.Text = "OR";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // btnNewFromMax
            // 
            this.btnNewFromMax.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnNewFromMax.Location = new System.Drawing.Point(285, 92);
            this.btnNewFromMax.Name = "btnNewFromMax";
            this.btnNewFromMax.Size = new System.Drawing.Size(110, 84);
            this.btnNewFromMax.TabIndex = 10;
            this.btnNewFromMax.Text = "New From 3DMAX";
            this.btnNewFromMax.UseVisualStyleBackColor = true;
            this.btnNewFromMax.Click += new System.EventHandler(this.btnNewFromMax_Click);
            // 
            // ProjectControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.Controls.Add(this.btnNewFromMax);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnNewBlank);
            this.Controls.Add(this.btnDelP);
            this.Controls.Add(this.btnEditP);
            this.Controls.Add(this.btnLoadP);
            this.Controls.Add(this.dlvListProject);
            this.Name = "ProjectControl";
            this.Size = new System.Drawing.Size(560, 550);
            ((System.ComponentModel.ISupportInitialize)(this.dlvListProject)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnDelP;
        private System.Windows.Forms.Button btnEditP;
        private System.Windows.Forms.Button btnLoadP;
        private BrightIdeasSoftware.DataListView dlvListProject;
        private BrightIdeasSoftware.OLVColumn olvColumnPName;
        private BrightIdeasSoftware.OLVColumn olvColumnPath;
        private System.Windows.Forms.Button btnNewBlank;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnNewFromMax;
    }
}
