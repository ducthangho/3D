namespace Y3D.Forms
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnTabTest = new System.Windows.Forms.Button();
            this.btnTabObject = new System.Windows.Forms.Button();
            this.btnTabProject = new System.Windows.Forms.Button();
            this.objectsControl1 = new Y3D.Projects.ObjectsControl();
            this.testControl1 = new Y3D.Tests.TestControl();
            this.projectControl1 = new Y3D.Projects.ProjectControl();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.panel1.Controls.Add(this.btnTabTest);
            this.panel1.Controls.Add(this.btnTabObject);
            this.panel1.Controls.Add(this.btnTabProject);
            this.panel1.Location = new System.Drawing.Point(2, 24);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(40, 550);
            this.panel1.TabIndex = 4;
            // 
            // btnTabTest
            // 
            this.btnTabTest.BackColor = System.Drawing.Color.Transparent;
            this.btnTabTest.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnTabTest.FlatAppearance.BorderSize = 0;
            this.btnTabTest.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnTabTest.Image = global::Y3D.Properties.Resources.test_off;
            this.btnTabTest.Location = new System.Drawing.Point(4, 84);
            this.btnTabTest.Name = "btnTabTest";
            this.btnTabTest.Size = new System.Drawing.Size(32, 32);
            this.btnTabTest.TabIndex = 2;
            this.btnTabTest.Tag = "test";
            this.btnTabTest.UseVisualStyleBackColor = false;
            this.btnTabTest.Click += new System.EventHandler(this.btnTabTest_Click);
            // 
            // btnTabObject
            // 
            this.btnTabObject.BackColor = System.Drawing.Color.Transparent;
            this.btnTabObject.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnTabObject.FlatAppearance.BorderSize = 0;
            this.btnTabObject.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnTabObject.Image = global::Y3D.Properties.Resources.omana_off;
            this.btnTabObject.Location = new System.Drawing.Point(4, 46);
            this.btnTabObject.Name = "btnTabObject";
            this.btnTabObject.Size = new System.Drawing.Size(32, 32);
            this.btnTabObject.TabIndex = 1;
            this.btnTabObject.Tag = "omana";
            this.btnTabObject.UseVisualStyleBackColor = false;
            this.btnTabObject.Click += new System.EventHandler(this.btnTabObject_Click);
            // 
            // btnTabProject
            // 
            this.btnTabProject.BackColor = System.Drawing.Color.Transparent;
            this.btnTabProject.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnTabProject.FlatAppearance.BorderSize = 0;
            this.btnTabProject.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnTabProject.Image = global::Y3D.Properties.Resources.buffer24_off;
            this.btnTabProject.Location = new System.Drawing.Point(4, 8);
            this.btnTabProject.Name = "btnTabProject";
            this.btnTabProject.Size = new System.Drawing.Size(32, 32);
            this.btnTabProject.TabIndex = 0;
            this.btnTabProject.Tag = "buffer24";
            this.btnTabProject.UseVisualStyleBackColor = false;
            this.btnTabProject.Click += new System.EventHandler(this.btnTabProject_Click);
            // 
            // objectsControl1
            // 
            this.objectsControl1.Location = new System.Drawing.Point(45, 24);
            this.objectsControl1.Margin = new System.Windows.Forms.Padding(4);
            this.objectsControl1.Name = "objectsControl1";
            this.objectsControl1.Size = new System.Drawing.Size(560, 550);
            this.objectsControl1.TabIndex = 3;
            this.objectsControl1.Tag = "omana";
            this.objectsControl1.Visible = false;
            // 
            // testControl1
            // 
            this.testControl1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.testControl1.Location = new System.Drawing.Point(44, 24);
            this.testControl1.Margin = new System.Windows.Forms.Padding(4);
            this.testControl1.Name = "testControl1";
            this.testControl1.Size = new System.Drawing.Size(561, 550);
            this.testControl1.TabIndex = 2;
            this.testControl1.Tag = "test";
            this.testControl1.Visible = false;
            // 
            // projectControl1
            // 
            this.projectControl1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.projectControl1.Location = new System.Drawing.Point(45, 24);
            this.projectControl1.Margin = new System.Windows.Forms.Padding(4);
            this.projectControl1.Name = "projectControl1";
            this.projectControl1.Size = new System.Drawing.Size(560, 550);
            this.projectControl1.TabIndex = 5;
            this.projectControl1.Tag = "buffer24";
            this.projectControl1.UseSelectable = true;
            this.projectControl1.Load += new System.EventHandler(this.projectControl1_Load);
            // 
            // YMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(606, 576);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.objectsControl1);
            this.Controls.Add(this.testControl1);
            this.Controls.Add(this.projectControl1);
            this.DisplayHeader = false;
            this.MaximizeBox = false;
            this.Name = "YMainForm";
            this.Padding = new System.Windows.Forms.Padding(20, 30, 20, 20);
            this.Resizable = false;
            this.Style = MetroFramework.MetroColorStyle.Lime;
            this.Text = "Y3D Project";
            this.TopMost = true;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.YMainForm_FormClosed);
            this.Load += new System.EventHandler(this.YMainForm_Load);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private Y3D.Tests.TestControl testControl1;
        private Y3D.Projects.ObjectsControl objectsControl1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnTabProject;
        private System.Windows.Forms.Button btnTabObject;
        private System.Windows.Forms.Button btnTabTest;
        private Y3D.Projects.ProjectControl projectControl1;
    }
}

