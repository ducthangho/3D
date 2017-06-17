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
            this.tabPageNormal = new System.Windows.Forms.TabPage();
            this.tabPageBake = new System.Windows.Forms.TabPage();
            this.btnLow = new System.Windows.Forms.Button();
            this.btnUnwrap = new System.Windows.Forms.Button();
            this.btnPack = new System.Windows.Forms.Button();
            this.btnBake = new System.Windows.Forms.Button();
            this.lbTestName = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lowTab1 = new Y3D.Projects.LowTab();
            this.unwrapTab1 = new Y3D.Projects.UnwrapTab();
            this.tabSettings.SuspendLayout();
            this.tabPageLow.SuspendLayout();
            this.tabPageUnwrap.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabSettings
            // 
            this.tabSettings.Controls.Add(this.tabPageLow);
            this.tabSettings.Controls.Add(this.tabPageUnwrap);
            this.tabSettings.Controls.Add(this.tabPagePack);
            this.tabSettings.Controls.Add(this.tabPageNormal);
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
            this.tabPageLow.Controls.Add(this.lowTab1);
            this.tabPageLow.Location = new System.Drawing.Point(4, 22);
            this.tabPageLow.Name = "tabPageLow";
            this.tabPageLow.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageLow.Size = new System.Drawing.Size(268, 365);
            this.tabPageLow.TabIndex = 0;
            this.tabPageLow.Text = "Low";
            // 
            // tabPageUnwrap
            // 
            this.tabPageUnwrap.Controls.Add(this.unwrapTab1);
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
            // tabPageNormal
            // 
            this.tabPageNormal.Location = new System.Drawing.Point(4, 22);
            this.tabPageNormal.Name = "tabPageNormal";
            this.tabPageNormal.Size = new System.Drawing.Size(268, 365);
            this.tabPageNormal.TabIndex = 4;
            this.tabPageNormal.Text = "Normal";
            this.tabPageNormal.UseVisualStyleBackColor = true;
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
            this.btnLow.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLow.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLow.Location = new System.Drawing.Point(4, 32);
            this.btnLow.Name = "btnLow";
            this.btnLow.Size = new System.Drawing.Size(44, 22);
            this.btnLow.TabIndex = 0;
            this.btnLow.Tag = "low";
            this.btnLow.Text = "Low";
            this.btnLow.UseVisualStyleBackColor = false;
            this.btnLow.Click += new System.EventHandler(this.btnLow_Click);
            // 
            // btnUnwrap
            // 
            this.btnUnwrap.BackColor = System.Drawing.Color.Silver;
            this.btnUnwrap.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnUnwrap.Location = new System.Drawing.Point(54, 32);
            this.btnUnwrap.Name = "btnUnwrap";
            this.btnUnwrap.Size = new System.Drawing.Size(54, 22);
            this.btnUnwrap.TabIndex = 1;
            this.btnUnwrap.Tag = "unwrap";
            this.btnUnwrap.Text = "Unwrap";
            this.btnUnwrap.UseVisualStyleBackColor = false;
            this.btnUnwrap.Click += new System.EventHandler(this.btnUnwrap_Click);
            // 
            // btnPack
            // 
            this.btnPack.BackColor = System.Drawing.Color.Silver;
            this.btnPack.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnPack.Location = new System.Drawing.Point(114, 32);
            this.btnPack.Name = "btnPack";
            this.btnPack.Size = new System.Drawing.Size(42, 22);
            this.btnPack.TabIndex = 2;
            this.btnPack.Tag = "pack";
            this.btnPack.Text = "Pack";
            this.btnPack.UseVisualStyleBackColor = false;
            // 
            // btnBake
            // 
            this.btnBake.BackColor = System.Drawing.Color.Silver;
            this.btnBake.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnBake.Location = new System.Drawing.Point(66, 60);
            this.btnBake.Name = "btnBake";
            this.btnBake.Size = new System.Drawing.Size(44, 22);
            this.btnBake.TabIndex = 13;
            this.btnBake.Tag = "bake";
            this.btnBake.Text = "Bake";
            this.btnBake.UseVisualStyleBackColor = false;
            // 
            // lbTestName
            // 
            this.lbTestName.AutoSize = true;
            this.lbTestName.BackColor = System.Drawing.Color.DimGray;
            this.lbTestName.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.lbTestName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbTestName.ForeColor = System.Drawing.Color.White;
            this.lbTestName.Location = new System.Drawing.Point(4, 7);
            this.lbTestName.Name = "lbTestName";
            this.lbTestName.Padding = new System.Windows.Forms.Padding(3);
            this.lbTestName.Size = new System.Drawing.Size(57, 19);
            this.lbTestName.TabIndex = 14;
            this.lbTestName.Text = "Name : ";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Silver;
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button1.Location = new System.Drawing.Point(4, 60);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(56, 22);
            this.button1.TabIndex = 15;
            this.button1.Tag = "normal";
            this.button1.Text = "Normal";
            this.button1.UseVisualStyleBackColor = false;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Silver;
            this.panel1.Controls.Add(this.btnLow);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.btnUnwrap);
            this.panel1.Controls.Add(this.btnBake);
            this.panel1.Controls.Add(this.btnPack);
            this.panel1.Controls.Add(this.lbTestName);
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(276, 90);
            this.panel1.TabIndex = 16;
            // 
            // lowTab1
            // 
            this.lowTab1.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.lowTab1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lowTab1.Location = new System.Drawing.Point(3, 3);
            this.lowTab1.Name = "lowTab1";
            this.lowTab1.Size = new System.Drawing.Size(262, 359);
            this.lowTab1.TabIndex = 0;
            // 
            // unwrapTab1
            // 
            this.unwrapTab1.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.unwrapTab1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.unwrapTab1.Location = new System.Drawing.Point(0, 0);
            this.unwrapTab1.Name = "unwrapTab1";
            this.unwrapTab1.Size = new System.Drawing.Size(268, 365);
            this.unwrapTab1.TabIndex = 0;
            // 
            // TestDetailControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.tabSettings);
            this.Name = "TestDetailControl";
            this.Size = new System.Drawing.Size(276, 494);
            this.tabSettings.ResumeLayout(false);
            this.tabPageLow.ResumeLayout(false);
            this.tabPageUnwrap.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TabControl tabSettings;
        private System.Windows.Forms.TabPage tabPageLow;
        private System.Windows.Forms.TabPage tabPageUnwrap;
        private System.Windows.Forms.TabPage tabPagePack;
        private System.Windows.Forms.TabPage tabPageBake;
        private System.Windows.Forms.Button btnLow;
        private System.Windows.Forms.Button btnUnwrap;
        private System.Windows.Forms.Button btnPack;
        private System.Windows.Forms.Button btnBake;
        private System.Windows.Forms.Label lbTestName;
        private LowTab lowTab1;
        private UnwrapTab unwrapTab1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TabPage tabPageNormal;
    }
}
