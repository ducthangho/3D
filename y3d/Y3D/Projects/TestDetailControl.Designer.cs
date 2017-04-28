namespace Y3D.Projects
{
    partial class TestDetailControl
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
            this.tabSettings = new System.Windows.Forms.TabControl();
            this.tabPageLow = new System.Windows.Forms.TabPage();
            this.tabPageUnwrap = new System.Windows.Forms.TabPage();
            this.tabPagePack = new System.Windows.Forms.TabPage();
            this.tabPageBake = new System.Windows.Forms.TabPage();
            this.btnLow = new System.Windows.Forms.Button();
            this.btnUnwrap = new System.Windows.Forms.Button();
            this.btnPack = new System.Windows.Forms.Button();
            this.btnBake = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.labelTestName = new System.Windows.Forms.Label();
            this.lowControl1 = new Y3D.Projects.LowControl();
            this.tabSettings.SuspendLayout();
            this.tabPageLow.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabSettings
            // 
            this.tabSettings.Controls.Add(this.tabPageLow);
            this.tabSettings.Controls.Add(this.tabPageUnwrap);
            this.tabSettings.Controls.Add(this.tabPagePack);
            this.tabSettings.Controls.Add(this.tabPageBake);
            this.tabSettings.Location = new System.Drawing.Point(0, 99);
            this.tabSettings.Name = "tabSettings";
            this.tabSettings.SelectedIndex = 0;
            this.tabSettings.Size = new System.Drawing.Size(276, 391);
            this.tabSettings.TabIndex = 12;
            // 
            // tabPageLow
            // 
            this.tabPageLow.BackColor = System.Drawing.Color.Gainsboro;
            this.tabPageLow.Controls.Add(this.lowControl1);
            this.tabPageLow.Location = new System.Drawing.Point(4, 22);
            this.tabPageLow.Name = "tabPageLow";
            this.tabPageLow.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageLow.Size = new System.Drawing.Size(268, 365);
            this.tabPageLow.TabIndex = 0;
            this.tabPageLow.Text = "Low poly";
            // 
            // tabPageUnwrap
            // 
            this.tabPageUnwrap.Location = new System.Drawing.Point(4, 22);
            this.tabPageUnwrap.Name = "tabPageUnwrap";
            this.tabPageUnwrap.Size = new System.Drawing.Size(268, 365);
            this.tabPageUnwrap.TabIndex = 1;
            this.tabPageUnwrap.Text = "Unwrap";
            this.tabPageUnwrap.UseVisualStyleBackColor = true;
            // 
            // tabPagePack
            // 
            this.tabPagePack.Location = new System.Drawing.Point(4, 22);
            this.tabPagePack.Name = "tabPagePack";
            this.tabPagePack.Size = new System.Drawing.Size(268, 365);
            this.tabPagePack.TabIndex = 2;
            this.tabPagePack.Text = "Pack";
            this.tabPagePack.UseVisualStyleBackColor = true;
            // 
            // tabPageBake
            // 
            this.tabPageBake.Location = new System.Drawing.Point(4, 22);
            this.tabPageBake.Name = "tabPageBake";
            this.tabPageBake.Size = new System.Drawing.Size(268, 365);
            this.tabPageBake.TabIndex = 3;
            this.tabPageBake.Text = "Bake";
            this.tabPageBake.UseVisualStyleBackColor = true;
            // 
            // btnLow
            // 
            this.btnLow.BackColor = System.Drawing.Color.DarkSeaGreen;
            this.btnLow.Location = new System.Drawing.Point(3, 49);
            this.btnLow.Name = "btnLow";
            this.btnLow.Size = new System.Drawing.Size(68, 25);
            this.btnLow.TabIndex = 0;
            this.btnLow.Text = "Low poly";
            this.btnLow.UseVisualStyleBackColor = false;
            this.btnLow.Click += new System.EventHandler(this.btnLow_Click);
            // 
            // btnUnwrap
            // 
            this.btnUnwrap.BackColor = System.Drawing.Color.Silver;
            this.btnUnwrap.Location = new System.Drawing.Point(77, 49);
            this.btnUnwrap.Name = "btnUnwrap";
            this.btnUnwrap.Size = new System.Drawing.Size(63, 25);
            this.btnUnwrap.TabIndex = 1;
            this.btnUnwrap.Text = "Unwrap";
            this.btnUnwrap.UseVisualStyleBackColor = false;
            this.btnUnwrap.Click += new System.EventHandler(this.btnUnwrap_Click);
            // 
            // btnPack
            // 
            this.btnPack.BackColor = System.Drawing.Color.Silver;
            this.btnPack.Location = new System.Drawing.Point(146, 49);
            this.btnPack.Name = "btnPack";
            this.btnPack.Size = new System.Drawing.Size(54, 25);
            this.btnPack.TabIndex = 2;
            this.btnPack.Text = "Pack";
            this.btnPack.UseVisualStyleBackColor = false;
            // 
            // btnBake
            // 
            this.btnBake.BackColor = System.Drawing.Color.Silver;
            this.btnBake.Location = new System.Drawing.Point(206, 49);
            this.btnBake.Name = "btnBake";
            this.btnBake.Size = new System.Drawing.Size(64, 25);
            this.btnBake.TabIndex = 13;
            this.btnBake.Text = "Bake";
            this.btnBake.UseVisualStyleBackColor = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.labelTestName);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.btnBake);
            this.groupBox1.Controls.Add(this.btnLow);
            this.groupBox1.Controls.Add(this.btnPack);
            this.groupBox1.Controls.Add(this.btnUnwrap);
            this.groupBox1.Location = new System.Drawing.Point(0, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(276, 90);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Details";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(62, 13);
            this.label1.TabIndex = 14;
            this.label1.Text = "Test Name:";
            // 
            // labelTestName
            // 
            this.labelTestName.AutoSize = true;
            this.labelTestName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTestName.Location = new System.Drawing.Point(70, 25);
            this.labelTestName.Name = "labelTestName";
            this.labelTestName.Size = new System.Drawing.Size(0, 13);
            this.labelTestName.TabIndex = 15;
            // 
            // lowControl1
            // 
            this.lowControl1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.lowControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lowControl1.Location = new System.Drawing.Point(3, 3);
            this.lowControl1.Name = "lowControl1";
            this.lowControl1.Size = new System.Drawing.Size(262, 359);
            this.lowControl1.TabIndex = 8;
            // 
            // TestDetailControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.tabSettings);
            this.Name = "TestDetailControl";
            this.Size = new System.Drawing.Size(276, 494);
            this.tabSettings.ResumeLayout(false);
            this.tabPageLow.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TabControl tabSettings;
        private System.Windows.Forms.TabPage tabPageLow;
        private LowControl lowControl1;
        private System.Windows.Forms.TabPage tabPageUnwrap;
        private System.Windows.Forms.TabPage tabPagePack;
        private System.Windows.Forms.TabPage tabPageBake;
        private System.Windows.Forms.Button btnLow;
        private System.Windows.Forms.Button btnUnwrap;
        private System.Windows.Forms.Button btnPack;
        private System.Windows.Forms.Button btnBake;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label labelTestName;
        private System.Windows.Forms.Label label1;
    }
}
