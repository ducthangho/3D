namespace Y3D.Projects
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
            this.label1 = new System.Windows.Forms.Label();
            this.mtNew3DMax = new MetroFramework.Controls.MetroTile();
            ((System.ComponentModel.ISupportInitialize)(this.dlvListProject)).BeginInit();
            this.SuspendLayout();
            // 
            // btnDelP
            // 
            this.btnDelP.Location = new System.Drawing.Point(198, 424);
            this.btnDelP.Name = "btnDelP";
            this.btnDelP.Size = new System.Drawing.Size(37, 23);
            this.btnDelP.TabIndex = 7;
            this.btnDelP.Text = "Del";
            this.btnDelP.UseVisualStyleBackColor = true;
            this.btnDelP.Click += new System.EventHandler(this.btnDelP_Click);
            // 
            // btnEditP
            // 
            this.btnEditP.Location = new System.Drawing.Point(146, 424);
            this.btnEditP.Name = "btnEditP";
            this.btnEditP.Size = new System.Drawing.Size(46, 23);
            this.btnEditP.TabIndex = 6;
            this.btnEditP.Text = "Edit";
            this.btnEditP.UseVisualStyleBackColor = true;
            this.btnEditP.Click += new System.EventHandler(this.btnEditP_Click);
            // 
            // btnLoadP
            // 
            this.btnLoadP.Location = new System.Drawing.Point(64, 424);
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
            this.dlvListProject.Location = new System.Drawing.Point(10, 230);
            this.dlvListProject.Name = "dlvListProject";
            this.dlvListProject.Size = new System.Drawing.Size(280, 188);
            this.dlvListProject.TabIndex = 4;
            this.dlvListProject.UseCompatibleStateImageBehavior = false;
            this.dlvListProject.View = System.Windows.Forms.View.Details;
            this.dlvListProject.SelectedIndexChanged += new System.EventHandler(this.dlvListProject_SelectedIndexChanged);
            this.dlvListProject.DoubleClick += new System.EventHandler(this.dlvListProject_DoubleClick);
            // 
            // olvColumnPName
            // 
            this.olvColumnPName.AspectName = "Pname";
            this.olvColumnPName.Groupable = false;
            this.olvColumnPName.HeaderFont = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.olvColumnPName.Text = "Project";
            this.olvColumnPName.Width = 130;
            // 
            // olvColumnPath
            // 
            this.olvColumnPath.FillsFreeSpace = true;
            this.olvColumnPath.Groupable = false;
            this.olvColumnPath.Text = "Path";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(129, 182);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(43, 25);
            this.label1.TabIndex = 9;
            this.label1.Text = "OR";
            // 
            // mtNew3DMax
            // 
            this.mtNew3DMax.ActiveControl = null;
            this.mtNew3DMax.Location = new System.Drawing.Point(77, 112);
            this.mtNew3DMax.Name = "mtNew3DMax";
            this.mtNew3DMax.Size = new System.Drawing.Size(149, 52);
            this.mtNew3DMax.Style = MetroFramework.MetroColorStyle.Lime;
            this.mtNew3DMax.TabIndex = 11;
            this.mtNew3DMax.Text = "New From 3DMAX";
            this.mtNew3DMax.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.mtNew3DMax.TileTextFontWeight = MetroFramework.MetroTileTextWeight.Bold;
            this.mtNew3DMax.UseSelectable = true;
            this.mtNew3DMax.Click += new System.EventHandler(this.mtNew3DMax_Click);
            // 
            // ProjectControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.Controls.Add(this.mtNew3DMax);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnDelP);
            this.Controls.Add(this.btnEditP);
            this.Controls.Add(this.btnLoadP);
            this.Controls.Add(this.dlvListProject);
            this.Name = "ProjectControl";
            this.Size = new System.Drawing.Size(300, 560);
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
        private System.Windows.Forms.Label label1;
        private MetroFramework.Controls.MetroTile mtNew3DMax;
    }
}
