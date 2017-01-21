namespace YMax.Forms
{
    partial class YMainForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(YMainForm));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.btnNewProject = new System.Windows.Forms.Button();
            this.openFileProject = new System.Windows.Forms.OpenFileDialog();
            this.btnLoadProject = new System.Windows.Forms.Button();
            this.olvProjects = new BrightIdeasSoftware.ObjectListView();
            this.olvPName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvPPath = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            ((System.ComponentModel.ISupportInitialize)(this.olvProjects)).BeginInit();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "star");
            this.imageList1.Images.SetKeyName(1, "song");
            this.imageList1.Images.SetKeyName(2, "user");
            this.imageList1.Images.SetKeyName(3, "folder");
            // 
            // btnNewProject
            // 
            this.btnNewProject.Location = new System.Drawing.Point(330, 12);
            this.btnNewProject.Margin = new System.Windows.Forms.Padding(2);
            this.btnNewProject.Name = "btnNewProject";
            this.btnNewProject.Size = new System.Drawing.Size(88, 24);
            this.btnNewProject.TabIndex = 0;
            this.btnNewProject.Text = "New Project";
            this.btnNewProject.UseVisualStyleBackColor = true;
            this.btnNewProject.Click += new System.EventHandler(this.btnNewProject_Click);
            // 
            // openFileProject
            // 
            this.openFileProject.FileName = "openNewProject";
            // 
            // btnLoadProject
            // 
            this.btnLoadProject.Location = new System.Drawing.Point(330, 40);
            this.btnLoadProject.Margin = new System.Windows.Forms.Padding(2);
            this.btnLoadProject.Name = "btnLoadProject";
            this.btnLoadProject.Size = new System.Drawing.Size(88, 24);
            this.btnLoadProject.TabIndex = 2;
            this.btnLoadProject.Text = "Load Project";
            this.btnLoadProject.UseVisualStyleBackColor = true;
            // 
            // olvProjects
            // 
            this.olvProjects.AllColumns.Add(this.olvPName);
            this.olvProjects.AllColumns.Add(this.olvPPath);
            this.olvProjects.CellEditUseWholeCell = false;
            this.olvProjects.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvPName,
            this.olvPPath});
            this.olvProjects.Cursor = System.Windows.Forms.Cursors.Default;
            this.olvProjects.Location = new System.Drawing.Point(12, 12);
            this.olvProjects.Name = "olvProjects";
            this.olvProjects.Size = new System.Drawing.Size(313, 151);
            this.olvProjects.TabIndex = 3;
            this.olvProjects.UseCompatibleStateImageBehavior = false;
            this.olvProjects.View = System.Windows.Forms.View.Details;
            this.olvProjects.SelectedIndexChanged += new System.EventHandler(this.olvProjects_SelectedIndexChanged);
            // 
            // olvPName
            // 
            this.olvPName.Text = "Project Name";
            this.olvPName.Width = 122;
            // 
            // olvPPath
            // 
            this.olvPPath.FillsFreeSpace = true;
            this.olvPPath.Text = "Path";
            // 
            // YMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(428, 186);
            this.Controls.Add(this.olvProjects);
            this.Controls.Add(this.btnLoadProject);
            this.Controls.Add(this.btnNewProject);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "YMainForm";
            this.Text = "YMainForm";
            this.Activated += new System.EventHandler(this.YMainForm_Activated);
            this.Deactivate += new System.EventHandler(this.YMainForm_Deactivate);
            this.Load += new System.EventHandler(this.YMainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.olvProjects)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Button btnNewProject;
        private System.Windows.Forms.OpenFileDialog openFileProject;
        private System.Windows.Forms.Button btnLoadProject;
        private BrightIdeasSoftware.ObjectListView olvProjects;
        private BrightIdeasSoftware.OLVColumn olvPName;
        private BrightIdeasSoftware.OLVColumn olvPPath;
    }
}