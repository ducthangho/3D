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
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnMainMenu = new System.Windows.Forms.Button();
            this.metroContextMenu1 = new MetroFramework.Controls.MetroContextMenu(this.components);
            this.workerManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.metroPanel1 = new MetroFramework.Controls.MetroPanel();
            this.btnTabTest = new System.Windows.Forms.Button();
            this.btnTabObject = new System.Windows.Forms.Button();
            this.btnTabProject = new System.Windows.Forms.Button();
            this.btnExpand = new System.Windows.Forms.Button();
            this.projectControl1 = new Y3D.Projects.ProjectControl();
            this.objectsControl1 = new Y3D.Projects.ObjectsControl();
            this.testControl1 = new Y3D.Tests.TestControl();
            this.panel1.SuspendLayout();
            this.metroContextMenu1.SuspendLayout();
            this.metroPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.panel1.Controls.Add(this.btnTabTest);
            this.panel1.Controls.Add(this.btnTabObject);
            this.panel1.Controls.Add(this.btnTabProject);
            this.panel1.Location = new System.Drawing.Point(0, 5);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(40, 584);
            this.panel1.TabIndex = 4;
            // 
            // btnMainMenu
            // 
            this.btnMainMenu.BackColor = System.Drawing.Color.YellowGreen;
            this.btnMainMenu.ContextMenuStrip = this.metroContextMenu1;
            this.btnMainMenu.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnMainMenu.ForeColor = System.Drawing.Color.White;
            this.btnMainMenu.Location = new System.Drawing.Point(43, 9);
            this.btnMainMenu.Name = "btnMainMenu";
            this.btnMainMenu.Size = new System.Drawing.Size(50, 23);
            this.btnMainMenu.TabIndex = 6;
            this.btnMainMenu.Text = "Setting";
            this.btnMainMenu.UseVisualStyleBackColor = false;
            this.btnMainMenu.Click += new System.EventHandler(this.btnMainMenu_Click);
            // 
            // metroContextMenu1
            // 
            this.metroContextMenu1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.workerManagerToolStripMenuItem});
            this.metroContextMenu1.Name = "metroContextMenu1";
            this.metroContextMenu1.Size = new System.Drawing.Size(163, 48);
            // 
            // workerManagerToolStripMenuItem
            // 
            this.workerManagerToolStripMenuItem.Image = global::Y3D.Properties.Resources.net;
            this.workerManagerToolStripMenuItem.Name = "workerManagerToolStripMenuItem";
            this.workerManagerToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.workerManagerToolStripMenuItem.Text = "Worker manager";
            this.workerManagerToolStripMenuItem.Click += new System.EventHandler(this.workerManagerToolStripMenuItem_Click);
            // 
            // metroPanel1
            // 
            this.metroPanel1.Controls.Add(this.projectControl1);
            this.metroPanel1.Controls.Add(this.objectsControl1);
            this.metroPanel1.Controls.Add(this.testControl1);
            this.metroPanel1.HorizontalScrollbarBarColor = true;
            this.metroPanel1.HorizontalScrollbarHighlightOnWheel = false;
            this.metroPanel1.HorizontalScrollbarSize = 10;
            this.metroPanel1.Location = new System.Drawing.Point(43, 39);
            this.metroPanel1.Name = "metroPanel1";
            this.metroPanel1.Size = new System.Drawing.Size(505, 550);
            this.metroPanel1.TabIndex = 8;
            this.metroPanel1.VerticalScrollbarBarColor = true;
            this.metroPanel1.VerticalScrollbarHighlightOnWheel = false;
            this.metroPanel1.VerticalScrollbarSize = 10;
            // 
            // btnTabTest
            // 
            this.btnTabTest.BackColor = System.Drawing.Color.Transparent;
            this.btnTabTest.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.btnTabTest.FlatAppearance.BorderSize = 0;
            this.btnTabTest.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnTabTest.Image = global::Y3D.Properties.Resources.test_off;
            this.btnTabTest.Location = new System.Drawing.Point(5, 80);
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
            this.btnTabObject.Location = new System.Drawing.Point(5, 42);
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
            this.btnTabProject.Location = new System.Drawing.Point(5, 4);
            this.btnTabProject.Name = "btnTabProject";
            this.btnTabProject.Size = new System.Drawing.Size(32, 32);
            this.btnTabProject.TabIndex = 0;
            this.btnTabProject.Tag = "buffer24";
            this.btnTabProject.UseVisualStyleBackColor = false;
            this.btnTabProject.Click += new System.EventHandler(this.btnTabProject_Click);
            // 
            // btnExpand
            // 
            this.btnExpand.BackColor = System.Drawing.Color.YellowGreen;
            this.btnExpand.ContextMenuStrip = this.metroContextMenu1;
            this.btnExpand.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnExpand.ForeColor = System.Drawing.Color.White;
            this.btnExpand.Location = new System.Drawing.Point(94, 9);
            this.btnExpand.Name = "btnExpand";
            this.btnExpand.Size = new System.Drawing.Size(23, 23);
            this.btnExpand.TabIndex = 9;
            this.btnExpand.Text = "<";
            this.btnExpand.UseVisualStyleBackColor = false;
            this.btnExpand.Click += new System.EventHandler(this.btnExpand_Click);
            // 
            // projectControl1
            // 
            this.projectControl1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.projectControl1.Location = new System.Drawing.Point(0, 0);
            this.projectControl1.Margin = new System.Windows.Forms.Padding(4);
            this.projectControl1.Name = "projectControl1";
            this.projectControl1.Size = new System.Drawing.Size(494, 550);
            this.projectControl1.TabIndex = 5;
            this.projectControl1.Tag = "buffer24";
            this.projectControl1.UseSelectable = true;
            this.projectControl1.Load += new System.EventHandler(this.projectControl1_Load);
            // 
            // objectsControl1
            // 
            this.objectsControl1.Location = new System.Drawing.Point(0, 0);
            this.objectsControl1.Margin = new System.Windows.Forms.Padding(4);
            this.objectsControl1.Name = "objectsControl1";
            this.objectsControl1.Size = new System.Drawing.Size(505, 550);
            this.objectsControl1.TabIndex = 3;
            this.objectsControl1.Tag = "omana";
            this.objectsControl1.Visible = false;
            // 
            // testControl1
            // 
            this.testControl1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.testControl1.Location = new System.Drawing.Point(0, 0);
            this.testControl1.Margin = new System.Windows.Forms.Padding(4);
            this.testControl1.Name = "testControl1";
            this.testControl1.Size = new System.Drawing.Size(494, 550);
            this.testControl1.TabIndex = 2;
            this.testControl1.Tag = "test";
            this.testControl1.Visible = false;
            // 
            // YMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ClientSize = new System.Drawing.Size(350, 594);
            this.Controls.Add(this.btnExpand);
            this.Controls.Add(this.metroPanel1);
            this.Controls.Add(this.btnMainMenu);
            this.Controls.Add(this.panel1);
            this.DisplayHeader = false;
            this.Location = new System.Drawing.Point(40, 40);
            this.MaximizeBox = false;
            this.Name = "YMainForm";
            this.Padding = new System.Windows.Forms.Padding(0, 30, 0, 0);
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Style = MetroFramework.MetroColorStyle.Lime;
            this.Text = "Y3D Project";
            this.TopMost = true;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.YMainForm_FormClosed);
            this.Load += new System.EventHandler(this.YMainForm_Load);
            this.panel1.ResumeLayout(false);
            this.metroContextMenu1.ResumeLayout(false);
            this.metroPanel1.ResumeLayout(false);
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
        private System.Windows.Forms.Button btnMainMenu;
        private MetroFramework.Controls.MetroContextMenu metroContextMenu1;
        private System.Windows.Forms.ToolStripMenuItem workerManagerToolStripMenuItem;
        private MetroFramework.Controls.MetroPanel metroPanel1;
        private System.Windows.Forms.Button btnExpand;
    }
}

