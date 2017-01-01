﻿namespace YMax.Forms
{
    partial class AnalyzeForm
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
            this.label1 = new System.Windows.Forms.Label();
            this.nMinXref = new System.Windows.Forms.NumericUpDown();
            this.chkAutoGroup = new System.Windows.Forms.CheckBox();
            this.numMinFaceProxy = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.btnCustomColor = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnPresetSaveAs = new System.Windows.Forms.Button();
            this.btnPressetManager = new System.Windows.Forms.Button();
            this.cbPreset = new System.Windows.Forms.ComboBox();
            this.btnAnalyze = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.nMinXref)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinFaceProxy)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 25);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Min faces to be xref";
            // 
            // nMinXref
            // 
            this.nMinXref.Increment = new decimal(new int[] {
            200,
            0,
            0,
            0});
            this.nMinXref.Location = new System.Drawing.Point(164, 25);
            this.nMinXref.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.nMinXref.Maximum = new decimal(new int[] {
            10000000,
            0,
            0,
            0});
            this.nMinXref.Name = "nMinXref";
            this.nMinXref.Size = new System.Drawing.Size(62, 20);
            this.nMinXref.TabIndex = 1;
            // 
            // chkAutoGroup
            // 
            this.chkAutoGroup.AutoSize = true;
            this.chkAutoGroup.Location = new System.Drawing.Point(7, 68);
            this.chkAutoGroup.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.chkAutoGroup.Name = "chkAutoGroup";
            this.chkAutoGroup.Size = new System.Drawing.Size(159, 17);
            this.chkAutoGroup.TabIndex = 2;
            this.chkAutoGroup.Text = "Auto group camera and light";
            this.chkAutoGroup.UseVisualStyleBackColor = true;
            // 
            // numMinFaceProxy
            // 
            this.numMinFaceProxy.Location = new System.Drawing.Point(164, 46);
            this.numMinFaceProxy.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.numMinFaceProxy.Name = "numMinFaceProxy";
            this.numMinFaceProxy.Size = new System.Drawing.Size(62, 20);
            this.numMinFaceProxy.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 46);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(154, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Min faces to use low poly proxy";
            // 
            // btnCustomColor
            // 
            this.btnCustomColor.Location = new System.Drawing.Point(4, 90);
            this.btnCustomColor.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnCustomColor.Name = "btnCustomColor";
            this.btnCustomColor.Size = new System.Drawing.Size(154, 24);
            this.btnCustomColor.TabIndex = 5;
            this.btnCustomColor.Text = "Customize color by face";
            this.btnCustomColor.UseVisualStyleBackColor = true;
            this.btnCustomColor.Click += new System.EventHandler(this.btnCustomColor_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.btnCustomColor);
            this.groupBox1.Controls.Add(this.nMinXref);
            this.groupBox1.Controls.Add(this.numMinFaceProxy);
            this.groupBox1.Controls.Add(this.chkAutoGroup);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(9, 10);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox1.Size = new System.Drawing.Size(335, 128);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Basic setting";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnPresetSaveAs);
            this.groupBox2.Controls.Add(this.btnPressetManager);
            this.groupBox2.Controls.Add(this.cbPreset);
            this.groupBox2.Location = new System.Drawing.Point(9, 143);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox2.Size = new System.Drawing.Size(335, 49);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Preset";
            // 
            // btnPresetSaveAs
            // 
            this.btnPresetSaveAs.Location = new System.Drawing.Point(274, 17);
            this.btnPresetSaveAs.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnPresetSaveAs.Name = "btnPresetSaveAs";
            this.btnPresetSaveAs.Size = new System.Drawing.Size(56, 20);
            this.btnPresetSaveAs.TabIndex = 3;
            this.btnPresetSaveAs.Text = "Save As";
            this.btnPresetSaveAs.UseVisualStyleBackColor = true;
            // 
            // btnPressetManager
            // 
            this.btnPressetManager.Location = new System.Drawing.Point(102, 18);
            this.btnPressetManager.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnPressetManager.Name = "btnPressetManager";
            this.btnPressetManager.Size = new System.Drawing.Size(56, 20);
            this.btnPressetManager.TabIndex = 2;
            this.btnPressetManager.Text = "Manager";
            this.btnPressetManager.UseVisualStyleBackColor = true;
            // 
            // cbPreset
            // 
            this.cbPreset.FormattingEnabled = true;
            this.cbPreset.Location = new System.Drawing.Point(7, 18);
            this.cbPreset.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.cbPreset.Name = "cbPreset";
            this.cbPreset.Size = new System.Drawing.Size(92, 21);
            this.cbPreset.TabIndex = 0;
            // 
            // btnAnalyze
            // 
            this.btnAnalyze.Location = new System.Drawing.Point(186, 202);
            this.btnAnalyze.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnAnalyze.Name = "btnAnalyze";
            this.btnAnalyze.Size = new System.Drawing.Size(98, 24);
            this.btnAnalyze.TabIndex = 8;
            this.btnAnalyze.Text = "Analyze";
            this.btnAnalyze.UseVisualStyleBackColor = true;
            this.btnAnalyze.Click += new System.EventHandler(this.btnAnalyze_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(288, 202);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(56, 24);
            this.btnCancel.TabIndex = 9;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // AnalyzeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(353, 236);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnAnalyze);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "AnalyzeForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Analyze settings";
            ((System.ComponentModel.ISupportInitialize)(this.nMinXref)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMinFaceProxy)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nMinXref;
        private System.Windows.Forms.CheckBox chkAutoGroup;
        private System.Windows.Forms.NumericUpDown numMinFaceProxy;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnCustomColor;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnPresetSaveAs;
        private System.Windows.Forms.Button btnPressetManager;
        private System.Windows.Forms.ComboBox cbPreset;
        private System.Windows.Forms.Button btnAnalyze;
        private System.Windows.Forms.Button btnCancel;
    }
}