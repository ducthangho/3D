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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.projectsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectManagerMItem = new System.Windows.Forms.ToolStripMenuItem();
            this.systemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.workersManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testForm1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnTabTest = new System.Windows.Forms.Button();
            this.btnTabObject = new System.Windows.Forms.Button();
            this.btnTabProject = new System.Windows.Forms.Button();
            this.objectsControl1 = new Y3D.Forms.ObjectsControl();
            this.testControl1 = new Y3D.Forms.TestControl();
            this.projectControl1 = new Y3D.Forms.ProjectControl();
            this.menuStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.projectsToolStripMenuItem,
            this.objectsToolStripMenuItem,
            this.systemToolStripMenuItem,
            this.testToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(606, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // projectsToolStripMenuItem
            // 
            this.projectsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveProjectToolStripMenuItem});
            this.projectsToolStripMenuItem.Name = "projectsToolStripMenuItem";
            this.projectsToolStripMenuItem.Size = new System.Drawing.Size(56, 20);
            this.projectsToolStripMenuItem.Text = "Project";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveProjectToolStripMenuItem
            // 
            this.saveProjectToolStripMenuItem.Name = "saveProjectToolStripMenuItem";
            this.saveProjectToolStripMenuItem.Size = new System.Drawing.Size(138, 22);
            this.saveProjectToolStripMenuItem.Text = "Save Project";
            // 
            // objectsToolStripMenuItem
            // 
            this.objectsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.objectManagerMItem});
            this.objectsToolStripMenuItem.Name = "objectsToolStripMenuItem";
            this.objectsToolStripMenuItem.Size = new System.Drawing.Size(54, 20);
            this.objectsToolStripMenuItem.Text = "Object";
            // 
            // objectManagerMItem
            // 
            this.objectManagerMItem.Name = "objectManagerMItem";
            this.objectManagerMItem.Size = new System.Drawing.Size(159, 22);
            this.objectManagerMItem.Text = "Object Manager";
            this.objectManagerMItem.Click += new System.EventHandler(this.objectManagerMItem_Click);
            // 
            // systemToolStripMenuItem
            // 
            this.systemToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.workersManagerToolStripMenuItem});
            this.systemToolStripMenuItem.Name = "systemToolStripMenuItem";
            this.systemToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.systemToolStripMenuItem.Text = "System";
            // 
            // workersManagerToolStripMenuItem
            // 
            this.workersManagerToolStripMenuItem.Name = "workersManagerToolStripMenuItem";
            this.workersManagerToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.workersManagerToolStripMenuItem.Text = "Workers manager";
            this.workersManagerToolStripMenuItem.Click += new System.EventHandler(this.workersManagerToolStripMenuItem_Click);
            // 
            // testToolStripMenuItem
            // 
            this.testToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.testForm1ToolStripMenuItem});
            this.testToolStripMenuItem.Name = "testToolStripMenuItem";
            this.testToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.testToolStripMenuItem.Text = "Test";
            // 
            // testForm1ToolStripMenuItem
            // 
            this.testForm1ToolStripMenuItem.Name = "testForm1ToolStripMenuItem";
            this.testForm1ToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.testForm1ToolStripMenuItem.Text = "Test Form 1";
            this.testForm1ToolStripMenuItem.Click += new System.EventHandler(this.testForm1ToolStripMenuItem_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.panel1.Controls.Add(this.btnTabTest);
            this.panel1.Controls.Add(this.btnTabObject);
            this.panel1.Controls.Add(this.btnTabProject);
            this.panel1.Location = new System.Drawing.Point(0, 27);
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
            this.objectsControl1.Location = new System.Drawing.Point(46, 27);
            this.objectsControl1.Name = "objectsControl1";
            this.objectsControl1.Size = new System.Drawing.Size(560, 550);
            this.objectsControl1.TabIndex = 3;
            this.objectsControl1.Tag = "omana";
            this.objectsControl1.Visible = false;
            // 
            // testControl1
            // 
            this.testControl1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.testControl1.Location = new System.Drawing.Point(46, 27);
            this.testControl1.Name = "testControl1";
            this.testControl1.Size = new System.Drawing.Size(561, 550);
            this.testControl1.TabIndex = 2;
            this.testControl1.Tag = "test";
            this.testControl1.Visible = false;
            // 
            // projectControl1
            // 
            this.projectControl1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.projectControl1.Location = new System.Drawing.Point(47, 27);
            this.projectControl1.Name = "projectControl1";
            this.projectControl1.Size = new System.Drawing.Size(560, 550);
            this.projectControl1.TabIndex = 5;
            this.projectControl1.Tag = "buffer24";
            // 
            // YMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.ClientSize = new System.Drawing.Size(606, 576);
            this.Controls.Add(this.projectControl1);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.objectsControl1);
            this.Controls.Add(this.testControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.Name = "YMainForm";
            this.Text = "Y3D";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.YMainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem projectsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objectsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem systemToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testForm1ToolStripMenuItem;
        private TestControl testControl1;
        private ObjectsControl objectsControl1;
        private System.Windows.Forms.ToolStripMenuItem objectManagerMItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveProjectToolStripMenuItem;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnTabProject;
        private System.Windows.Forms.ToolStripMenuItem workersManagerToolStripMenuItem;
        private System.Windows.Forms.Button btnTabObject;
        private System.Windows.Forms.Button btnTabTest;
        private ProjectControl projectControl1;
    }
}

