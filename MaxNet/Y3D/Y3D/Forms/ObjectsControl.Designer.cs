namespace Y3D.Forms
{
    partial class ObjectsControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ObjectsControl));
            this.tabNavLeft = new System.Windows.Forms.TabControl();
            this.tabArea = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.miniToolStrip = new System.Windows.Forms.ToolStrip();
            this.panel1 = new System.Windows.Forms.Panel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.fastObjectListView1 = new BrightIdeasSoftware.FastObjectListView();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.tabNavLeft.SuspendLayout();
            this.panel1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fastObjectListView1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabNavLeft
            // 
            this.tabNavLeft.Controls.Add(this.tabArea);
            this.tabNavLeft.Controls.Add(this.tabPage2);
            this.tabNavLeft.Location = new System.Drawing.Point(3, 3);
            this.tabNavLeft.Name = "tabNavLeft";
            this.tabNavLeft.SelectedIndex = 0;
            this.tabNavLeft.Size = new System.Drawing.Size(200, 544);
            this.tabNavLeft.TabIndex = 0;
            // 
            // tabArea
            // 
            this.tabArea.Location = new System.Drawing.Point(4, 22);
            this.tabArea.Name = "tabArea";
            this.tabArea.Padding = new System.Windows.Forms.Padding(3);
            this.tabArea.Size = new System.Drawing.Size(192, 518);
            this.tabArea.TabIndex = 0;
            this.tabArea.Text = "Area";
            this.tabArea.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(192, 518);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Filter";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // miniToolStrip
            // 
            this.miniToolStrip.AutoSize = false;
            this.miniToolStrip.CanOverflow = false;
            this.miniToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.miniToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.miniToolStrip.Location = new System.Drawing.Point(9, 3);
            this.miniToolStrip.Name = "miniToolStrip";
            this.miniToolStrip.Size = new System.Drawing.Size(542, 25);
            this.miniToolStrip.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.fastObjectListView1);
            this.panel1.Controls.Add(this.toolStrip1);
            this.panel1.Location = new System.Drawing.Point(205, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(542, 540);
            this.panel1.TabIndex = 1;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(542, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // fastObjectListView1
            // 
            this.fastObjectListView1.CellEditUseWholeCell = false;
            this.fastObjectListView1.Location = new System.Drawing.Point(4, 28);
            this.fastObjectListView1.Name = "fastObjectListView1";
            this.fastObjectListView1.ShowGroups = false;
            this.fastObjectListView1.Size = new System.Drawing.Size(535, 383);
            this.fastObjectListView1.TabIndex = 1;
            this.fastObjectListView1.UseCompatibleStateImageBehavior = false;
            this.fastObjectListView1.View = System.Windows.Forms.View.Details;
            this.fastObjectListView1.VirtualMode = true;
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton1.Text = "toolStripButton1";
            // 
            // ObjectsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.tabNavLeft);
            this.Name = "ObjectsControl";
            this.Size = new System.Drawing.Size(750, 550);
            this.tabNavLeft.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fastObjectListView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabNavLeft;
        private System.Windows.Forms.TabPage tabArea;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ToolStrip miniToolStrip;
        private System.Windows.Forms.Panel panel1;
        private BrightIdeasSoftware.FastObjectListView fastObjectListView1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
    }
}
