namespace Y3D.Forms
{
    partial class ProjectForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.dlvListProject = new BrightIdeasSoftware.DataListView();
            this.olvColumnPName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.btnLoadP = new System.Windows.Forms.Button();
            this.btnEditP = new System.Windows.Forms.Button();
            this.btnDelP = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dlvListProject)).BeginInit();
            this.SuspendLayout();
            // 
            // dlvListProject
            // 
            this.dlvListProject.AllColumns.Add(this.olvColumnPName);
            this.dlvListProject.CellEditUseWholeCell = false;
            this.dlvListProject.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnPName});
            this.dlvListProject.DataSource = null;
            this.dlvListProject.Location = new System.Drawing.Point(12, 12);
            this.dlvListProject.Name = "dlvListProject";
            this.dlvListProject.Size = new System.Drawing.Size(349, 188);
            this.dlvListProject.TabIndex = 0;
            this.dlvListProject.UseCompatibleStateImageBehavior = false;
            this.dlvListProject.View = System.Windows.Forms.View.Details;
            // 
            // olvColumnPName
            // 
            this.olvColumnPName.AspectName = "pname";
            this.olvColumnPName.FillsFreeSpace = true;
            this.olvColumnPName.Text = "Project";
            // 
            // btnLoadP
            // 
            this.btnLoadP.Location = new System.Drawing.Point(12, 206);
            this.btnLoadP.Name = "btnLoadP";
            this.btnLoadP.Size = new System.Drawing.Size(75, 23);
            this.btnLoadP.TabIndex = 1;
            this.btnLoadP.Text = "Load Project";
            this.btnLoadP.UseVisualStyleBackColor = true;
            this.btnLoadP.Click += new System.EventHandler(this.btnLoadP_Click);
            // 
            // btnEditP
            // 
            this.btnEditP.Location = new System.Drawing.Point(94, 206);
            this.btnEditP.Name = "btnEditP";
            this.btnEditP.Size = new System.Drawing.Size(75, 23);
            this.btnEditP.TabIndex = 2;
            this.btnEditP.Text = "Edit";
            this.btnEditP.UseVisualStyleBackColor = true;
            // 
            // btnDelP
            // 
            this.btnDelP.Location = new System.Drawing.Point(175, 206);
            this.btnDelP.Name = "btnDelP";
            this.btnDelP.Size = new System.Drawing.Size(37, 23);
            this.btnDelP.TabIndex = 3;
            this.btnDelP.Text = "Del";
            this.btnDelP.UseVisualStyleBackColor = true;
            // 
            // ProjectForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(373, 242);
            this.Controls.Add(this.btnDelP);
            this.Controls.Add(this.btnEditP);
            this.Controls.Add(this.btnLoadP);
            this.Controls.Add(this.dlvListProject);
            this.Name = "ProjectForm";
            this.Text = "Projects Manager";
            this.Load += new System.EventHandler(this.ProjectForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dlvListProject)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private BrightIdeasSoftware.DataListView dlvListProject;
        private BrightIdeasSoftware.OLVColumn olvColumnPName;
        private System.Windows.Forms.Button btnLoadP;
        private System.Windows.Forms.Button btnEditP;
        private System.Windows.Forms.Button btnDelP;
    }
}