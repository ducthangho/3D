namespace YMax.Forms
{
    partial class FRangeForm
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
            this.olvRange = new BrightIdeasSoftware.ObjectListView();
            this.olvColumnName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnRange = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnColor = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.olvRange)).BeginInit();
            this.SuspendLayout();
            // 
            // olvRange
            // 
            this.olvRange.AllColumns.Add(this.olvColumnName);
            this.olvRange.AllColumns.Add(this.olvColumnRange);
            this.olvRange.AllColumns.Add(this.olvColumnColor);
            this.olvRange.CellEditUseWholeCell = false;
            this.olvRange.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnName,
            this.olvColumnRange,
            this.olvColumnColor});
            this.olvRange.Cursor = System.Windows.Forms.Cursors.Default;
            this.olvRange.HasCollapsibleGroups = false;
            this.olvRange.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.olvRange.Location = new System.Drawing.Point(12, 12);
            this.olvRange.Name = "olvRange";
            this.olvRange.Size = new System.Drawing.Size(364, 175);
            this.olvRange.TabIndex = 0;
            this.olvRange.UseCellFormatEvents = true;
            this.olvRange.UseCompatibleStateImageBehavior = false;
            this.olvRange.UseHotItem = true;
            this.olvRange.View = System.Windows.Forms.View.Details;
            this.olvRange.FormatCell += new System.EventHandler<BrightIdeasSoftware.FormatCellEventArgs>(this.olvRange_FormatCell);
            // 
            // olvColumnName
            // 
            this.olvColumnName.AspectName = "fr_name";
            this.olvColumnName.FillsFreeSpace = true;
            this.olvColumnName.Groupable = false;
            this.olvColumnName.Text = "Name";
            this.olvColumnName.Width = 120;
            // 
            // olvColumnRange
            // 
            this.olvColumnRange.AspectName = "max_count";
            this.olvColumnRange.Groupable = false;
            this.olvColumnRange.Text = "Range";
            this.olvColumnRange.Width = 150;
            // 
            // olvColumnColor
            // 
            this.olvColumnColor.AspectName = "color";
            this.olvColumnColor.Groupable = false;
            this.olvColumnColor.Text = "Color";
            this.olvColumnColor.Width = 50;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 193);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(30, 30);
            this.button1.TabIndex = 1;
            this.button1.Text = "+";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(48, 193);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(30, 30);
            this.button2.TabIndex = 2;
            this.button2.Text = "-";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(220, 195);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 30);
            this.button3.TabIndex = 3;
            this.button3.Text = "OK";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(301, 194);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 30);
            this.button4.TabIndex = 4;
            this.button4.Text = "Cancel";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // FRangeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(388, 236);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.olvRange);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "FRangeForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "FRangeForm";
            this.Activated += new System.EventHandler(this.FRangeForm_Activated);
            this.Deactivate += new System.EventHandler(this.FRangeForm_Deactivate);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FRangeForm_FormClosed);
            this.Load += new System.EventHandler(this.FRangeForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.olvRange)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private BrightIdeasSoftware.ObjectListView olvRange;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private BrightIdeasSoftware.OLVColumn olvColumnName;
        private BrightIdeasSoftware.OLVColumn olvColumnRange;
        private BrightIdeasSoftware.OLVColumn olvColumnColor;
    }
}