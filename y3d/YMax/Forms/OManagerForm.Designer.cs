namespace YMax.Forms
{
    partial class OManagerForm
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
            this.tabSidebar = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.infoLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.oProgressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.sizeModeBtn = new System.Windows.Forms.ToolStripSplitButton();
            this.sModeBtn = new System.Windows.Forms.ToolStripMenuItem();
            this.mModeBtn = new System.Windows.Forms.ToolStripMenuItem();
            this.lModeBtn = new System.Windows.Forms.ToolStripMenuItem();
            this.oFromMaxBtn = new System.Windows.Forms.ToolStripButton();
            this.saveBtn = new System.Windows.Forms.ToolStripButton();
            this.oListControl1 = new YMax.Forms.OListControl();
            this.agSidebar1 = new YMax.Forms.AGSidebar();
            this.filterSidebar1 = new YMax.Forms.FilterSidebar();
            this.tabSidebar.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabSidebar
            // 
            this.tabSidebar.Controls.Add(this.tabPage1);
            this.tabSidebar.Controls.Add(this.tabPage2);
            this.tabSidebar.Controls.Add(this.tabPage3);
            this.tabSidebar.Location = new System.Drawing.Point(0, 35);
            this.tabSidebar.Name = "tabSidebar";
            this.tabSidebar.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.tabSidebar.SelectedIndex = 0;
            this.tabSidebar.Size = new System.Drawing.Size(234, 618);
            this.tabSidebar.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.agSidebar1);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(226, 589);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Group";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.filterSidebar1);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(226, 589);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Filter";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            this.tabPage3.Location = new System.Drawing.Point(4, 25);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(226, 589);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Custom";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.infoLabel,
            this.oProgressBar});
            this.statusStrip1.Location = new System.Drawing.Point(0, 649);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(688, 25);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // infoLabel
            // 
            this.infoLabel.Name = "infoLabel";
            this.infoLabel.Size = new System.Drawing.Size(38, 20);
            this.infoLabel.Text = "Info:";
            // 
            // oProgressBar
            // 
            this.oProgressBar.Name = "oProgressBar";
            this.oProgressBar.Size = new System.Drawing.Size(100, 19);
            // 
            // toolStrip1
            // 
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sizeModeBtn,
            this.oFromMaxBtn,
            this.saveBtn});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(688, 27);
            this.toolStrip1.TabIndex = 3;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // sizeModeBtn
            // 
            this.sizeModeBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.sizeModeBtn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sModeBtn,
            this.mModeBtn,
            this.lModeBtn});
            this.sizeModeBtn.Image = global::YMax.Properties.Resources.size;
            this.sizeModeBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.sizeModeBtn.Name = "sizeModeBtn";
            this.sizeModeBtn.Size = new System.Drawing.Size(39, 24);
            this.sizeModeBtn.Text = "Window size";
            this.sizeModeBtn.ButtonClick += new System.EventHandler(this.sizeModeBtn_ButtonClick);
            // 
            // sModeBtn
            // 
            this.sModeBtn.Name = "sModeBtn";
            this.sModeBtn.Size = new System.Drawing.Size(139, 26);
            this.sModeBtn.Text = "Small";
            this.sModeBtn.Click += new System.EventHandler(this.sModeBtn_Click);
            // 
            // mModeBtn
            // 
            this.mModeBtn.Name = "mModeBtn";
            this.mModeBtn.Size = new System.Drawing.Size(139, 26);
            this.mModeBtn.Text = "Medium";
            this.mModeBtn.Click += new System.EventHandler(this.mModeBtn_Click);
            // 
            // lModeBtn
            // 
            this.lModeBtn.Name = "lModeBtn";
            this.lModeBtn.Size = new System.Drawing.Size(139, 26);
            this.lModeBtn.Text = "Large";
            this.lModeBtn.Click += new System.EventHandler(this.lModeBtn_Click);
            // 
            // oFromMaxBtn
            // 
            this.oFromMaxBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.oFromMaxBtn.Image = global::YMax.Properties.Resources.down;
            this.oFromMaxBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.oFromMaxBtn.Name = "oFromMaxBtn";
            this.oFromMaxBtn.Size = new System.Drawing.Size(24, 24);
            this.oFromMaxBtn.Text = "Load data from Maxs";
            this.oFromMaxBtn.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.oFromMaxBtn.Click += new System.EventHandler(this.oFromMaxBtn_Click);
            // 
            // saveBtn
            // 
            this.saveBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveBtn.Image = global::YMax.Properties.Resources.save;
            this.saveBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveBtn.Name = "saveBtn";
            this.saveBtn.Size = new System.Drawing.Size(24, 24);
            this.saveBtn.Text = "Save";
            this.saveBtn.Click += new System.EventHandler(this.saveBtn_Click);
            // 
            // oListControl1
            // 
            this.oListControl1.Location = new System.Drawing.Point(236, 35);
            this.oListControl1.Name = "oListControl1";
            this.oListControl1.Size = new System.Drawing.Size(450, 614);
            this.oListControl1.TabIndex = 1;
            // 
            // agSidebar1
            // 
            this.agSidebar1.Location = new System.Drawing.Point(1, 5);
            this.agSidebar1.Name = "agSidebar1";
            this.agSidebar1.Size = new System.Drawing.Size(225, 580);
            this.agSidebar1.TabIndex = 0;
            // 
            // filterSidebar1
            // 
            this.filterSidebar1.Location = new System.Drawing.Point(0, 0);
            this.filterSidebar1.Name = "filterSidebar1";
            this.filterSidebar1.Size = new System.Drawing.Size(225, 580);
            this.filterSidebar1.TabIndex = 0;
            // 
            // OManagerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(688, 674);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.oListControl1);
            this.Controls.Add(this.tabSidebar);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "OManagerForm";
            this.Text = "OManagerForm";
            this.TopMost = true;
            this.Activated += new System.EventHandler(this.OManagerForm_Activated);
            this.Deactivate += new System.EventHandler(this.OManagerForm_Deactivate);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OManagerForm_FormClosed);
            this.Load += new System.EventHandler(this.OManagerForm_Load);
            this.tabSidebar.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabSidebar;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private AGSidebar agSidebar1;
        private FilterSidebar filterSidebar1;
        private OListControl oListControl1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton oFromMaxBtn;
        private System.Windows.Forms.ToolStripStatusLabel infoLabel;
        private System.Windows.Forms.ToolStripProgressBar oProgressBar;
        private System.Windows.Forms.ToolStripButton saveBtn;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.ToolStripSplitButton sizeModeBtn;
        private System.Windows.Forms.ToolStripMenuItem sModeBtn;
        private System.Windows.Forms.ToolStripMenuItem mModeBtn;
        private System.Windows.Forms.ToolStripMenuItem lModeBtn;
    }
}