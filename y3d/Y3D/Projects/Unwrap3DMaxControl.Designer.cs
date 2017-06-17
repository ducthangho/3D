namespace Y3D.Projects
{
    partial class Unwrap3DMaxControl
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
            this.label1 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.btnApply = new System.Windows.Forms.Button();
            this.nAngle = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.nSpacing = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.nAngle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nSpacing)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(12, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(119, 16);
            this.label1.TabIndex = 52;
            this.label1.Text = "Unwrap 3DMaxs";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(86, 108);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(55, 23);
            this.button2.TabIndex = 51;
            this.button2.Text = "Preset";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // btnApply
            // 
            this.btnApply.Image = global::Y3D.Properties.Resources.start;
            this.btnApply.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnApply.Location = new System.Drawing.Point(15, 108);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(65, 23);
            this.btnApply.TabIndex = 50;
            this.btnApply.Text = "Apply";
            this.btnApply.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnApply.UseVisualStyleBackColor = true;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // nAngle
            // 
            this.nAngle.DecimalPlaces = 2;
            this.nAngle.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nAngle.Location = new System.Drawing.Point(64, 44);
            this.nAngle.Name = "nAngle";
            this.nAngle.Size = new System.Drawing.Size(120, 20);
            this.nAngle.TabIndex = 49;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 46);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 13);
            this.label2.TabIndex = 48;
            this.label2.Text = "Angle";
            // 
            // nSpacing
            // 
            this.nSpacing.DecimalPlaces = 3;
            this.nSpacing.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nSpacing.Location = new System.Drawing.Point(64, 72);
            this.nSpacing.Name = "nSpacing";
            this.nSpacing.Size = new System.Drawing.Size(120, 20);
            this.nSpacing.TabIndex = 54;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 72);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(46, 13);
            this.label3.TabIndex = 53;
            this.label3.Text = "Spacing";
            // 
            // Unwrap3DMaxControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.nSpacing);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.btnApply);
            this.Controls.Add(this.nAngle);
            this.Controls.Add(this.label2);
            this.Name = "Unwrap3DMaxControl";
            ((System.ComponentModel.ISupportInitialize)(this.nAngle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nSpacing)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button btnApply;
        private System.Windows.Forms.NumericUpDown nAngle;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown nSpacing;
        private System.Windows.Forms.Label label3;
    }
}
