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
            this.btnNewProject.Location = new System.Drawing.Point(12, 12);
            this.btnNewProject.Name = "btnNewProject";
            this.btnNewProject.Size = new System.Drawing.Size(117, 30);
            this.btnNewProject.TabIndex = 0;
            this.btnNewProject.Text = "New Project";
            this.btnNewProject.UseVisualStyleBackColor = true;
            this.btnNewProject.Click += new System.EventHandler(this.btnNewProject_Click);
            // 
            // openFileProject
            // 
            this.openFileProject.FileName = "openNewProject";
            // 
            // YMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(866, 630);
            this.Controls.Add(this.btnNewProject);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "YMainForm";
            this.Text = "YMainForm";
            this.Activated += new System.EventHandler(this.YMainForm_Activated);
            this.Deactivate += new System.EventHandler(this.YMainForm_Deactivate);
            this.Load += new System.EventHandler(this.YMainForm_Load);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Button btnNewProject;
        private System.Windows.Forms.OpenFileDialog openFileProject;
    }
}