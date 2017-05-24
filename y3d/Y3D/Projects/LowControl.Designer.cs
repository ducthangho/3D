namespace Y3D.Projects
{
    partial class LowControl
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnDelete = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.cmbLowType = new MetroFramework.Controls.MetroComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.labelNoSetting = new System.Windows.Forms.Label();
            this.lowSettingContainer = new System.Windows.Forms.TableLayoutPanel();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.WhiteSmoke;
            this.panel1.Controls.Add(this.btnDelete);
            this.panel1.Controls.Add(this.btnAdd);
            this.panel1.Controls.Add(this.cmbLowType);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(273, 44);
            this.panel1.TabIndex = 2;
            // 
            // btnDelete
            // 
            this.btnDelete.Image = global::Y3D.Properties.Resources.remove16;
            this.btnDelete.Location = new System.Drawing.Point(216, 9);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(28, 28);
            this.btnDelete.TabIndex = 4;
            this.btnDelete.UseVisualStyleBackColor = true;
            this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.Image = global::Y3D.Properties.Resources.add16;
            this.btnAdd.Location = new System.Drawing.Point(182, 9);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(28, 28);
            this.btnAdd.TabIndex = 3;
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // cmbLowType
            // 
            this.cmbLowType.FormattingEnabled = true;
            this.cmbLowType.ItemHeight = 23;
            this.cmbLowType.Items.AddRange(new object[] {
            "(none)",
            "XREF",
            "3DS MAX",
            "BLENDER",
            "MESHLAB"});
            this.cmbLowType.Location = new System.Drawing.Point(51, 8);
            this.cmbLowType.Name = "cmbLowType";
            this.cmbLowType.Size = new System.Drawing.Size(125, 29);
            this.cmbLowType.TabIndex = 2;
            this.cmbLowType.UseSelectable = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.DimGray;
            this.label1.Location = new System.Drawing.Point(17, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(28, 15);
            this.label1.TabIndex = 1;
            this.label1.Text = "App";
            // 
            // labelNoSetting
            // 
            this.labelNoSetting.AutoSize = true;
            this.labelNoSetting.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelNoSetting.ForeColor = System.Drawing.SystemColors.AppWorkspace;
            this.labelNoSetting.Location = new System.Drawing.Point(65, 178);
            this.labelNoSetting.Name = "labelNoSetting";
            this.labelNoSetting.Size = new System.Drawing.Size(133, 29);
            this.labelNoSetting.TabIndex = 3;
            this.labelNoSetting.Text = "(no setting)";
            // 
            // lowSettingContainer
            // 
            this.lowSettingContainer.ColumnCount = 1;
            this.lowSettingContainer.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.lowSettingContainer.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.lowSettingContainer.Location = new System.Drawing.Point(0, 43);
            this.lowSettingContainer.Name = "lowSettingContainer";
            this.lowSettingContainer.RowCount = 1;
            this.lowSettingContainer.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.lowSettingContainer.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.lowSettingContainer.Size = new System.Drawing.Size(273, 341);
            this.lowSettingContainer.TabIndex = 4;
            // 
            // LowControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.Controls.Add(this.labelNoSetting);
            this.Controls.Add(this.lowSettingContainer);
            this.Controls.Add(this.panel1);
            this.Name = "LowControl";
            this.Size = new System.Drawing.Size(273, 384);
            this.Load += new System.EventHandler(this.LowControl_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private MetroFramework.Controls.MetroComboBox cmbLowType;
        private System.Windows.Forms.Button btnDelete;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.Label labelNoSetting;
        private System.Windows.Forms.TableLayoutPanel lowSettingContainer;
    }
}
