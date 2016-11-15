namespace YMax.Forms
{
    partial class YTestForm
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
            this.btnCustom1 = new System.Windows.Forms.Button();
            this.btnCustom2 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnCustom1
            // 
            this.btnCustom1.Location = new System.Drawing.Point(12, 12);
            this.btnCustom1.Name = "btnCustom1";
            this.btnCustom1.Size = new System.Drawing.Size(113, 36);
            this.btnCustom1.TabIndex = 6;
            this.btnCustom1.Text = "Call Custom 1";
            this.btnCustom1.UseVisualStyleBackColor = true;
            this.btnCustom1.Click += new System.EventHandler(this.btnCustom1_Click);
            // 
            // btnCustom2
            // 
            this.btnCustom2.Location = new System.Drawing.Point(131, 12);
            this.btnCustom2.Name = "btnCustom2";
            this.btnCustom2.Size = new System.Drawing.Size(103, 36);
            this.btnCustom2.TabIndex = 5;
            this.btnCustom2.Text = "Pre file save";
            this.btnCustom2.UseVisualStyleBackColor = true;
            this.btnCustom2.Click += new System.EventHandler(this.btnCustom2_Click);
            // 
            // YTestForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(636, 367);
            this.Controls.Add(this.btnCustom1);
            this.Controls.Add(this.btnCustom2);
            this.Name = "YTestForm";
            this.Text = "YTestForm";
            this.Activated += new System.EventHandler(this.YTestForm_Activated);
            this.Deactivate += new System.EventHandler(this.YTestForm_Deactivate);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCustom1;
        private System.Windows.Forms.Button btnCustom2;
    }
}