namespace Y3D.Projects
{
    partial class LowMaxControl
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
            this.lockVertexPosition = new System.Windows.Forms.CheckBox();
            this.label8 = new System.Windows.Forms.Label();
            this.preventFlippedNormal = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            this.favorCompactFaces = new System.Windows.Forms.CheckBox();
            this.label6 = new System.Windows.Forms.Label();
            this.maxNormalMode = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.maxOMode = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.vertexCount = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.barPercent = new XComponent.SliderBar.MACTrackBar();
            this.btnEditMode = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.vertexCount)).BeginInit();
            this.SuspendLayout();
            // 
            // lockVertexPosition
            // 
            this.lockVertexPosition.AutoSize = true;
            this.lockVertexPosition.Location = new System.Drawing.Point(126, 245);
            this.lockVertexPosition.Name = "lockVertexPosition";
            this.lockVertexPosition.Size = new System.Drawing.Size(15, 14);
            this.lockVertexPosition.TabIndex = 27;
            this.lockVertexPosition.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 245);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(104, 13);
            this.label8.TabIndex = 26;
            this.label8.Text = "Lock Vertex Position";
            // 
            // preventFlippedNormal
            // 
            this.preventFlippedNormal.AutoSize = true;
            this.preventFlippedNormal.Location = new System.Drawing.Point(126, 221);
            this.preventFlippedNormal.Name = "preventFlippedNormal";
            this.preventFlippedNormal.Size = new System.Drawing.Size(15, 14);
            this.preventFlippedNormal.TabIndex = 25;
            this.preventFlippedNormal.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 221);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(122, 13);
            this.label7.TabIndex = 24;
            this.label7.Text = "Prevent Flipped Normals";
            // 
            // favorCompactFaces
            // 
            this.favorCompactFaces.AutoSize = true;
            this.favorCompactFaces.Location = new System.Drawing.Point(126, 196);
            this.favorCompactFaces.Name = "favorCompactFaces";
            this.favorCompactFaces.Size = new System.Drawing.Size(15, 14);
            this.favorCompactFaces.TabIndex = 23;
            this.favorCompactFaces.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 196);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(111, 13);
            this.label6.TabIndex = 22;
            this.label6.Text = "Favor Compact Faces";
            // 
            // maxNormalMode
            // 
            this.maxNormalMode.FormattingEnabled = true;
            this.maxNormalMode.Items.AddRange(new object[] {
            "Crunch normals",
            "Protect normals",
            "Exclude normals"});
            this.maxNormalMode.Location = new System.Drawing.Point(126, 166);
            this.maxNormalMode.Name = "maxNormalMode";
            this.maxNormalMode.Size = new System.Drawing.Size(128, 21);
            this.maxNormalMode.TabIndex = 21;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 169);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(70, 13);
            this.label5.TabIndex = 20;
            this.label5.Text = "Normal Mode";
            // 
            // maxOMode
            // 
            this.maxOMode.FormattingEnabled = true;
            this.maxOMode.Items.AddRange(new object[] {
            "Crunch borders",
            "Protect borders",
            "Exclude borders"});
            this.maxOMode.Location = new System.Drawing.Point(126, 140);
            this.maxOMode.Name = "maxOMode";
            this.maxOMode.Size = new System.Drawing.Size(128, 21);
            this.maxOMode.TabIndex = 19;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 143);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(94, 13);
            this.label4.TabIndex = 18;
            this.label4.Text = "Optimization Mode";
            // 
            // vertexCount
            // 
            this.vertexCount.Location = new System.Drawing.Point(126, 113);
            this.vertexCount.Name = "vertexCount";
            this.vertexCount.Size = new System.Drawing.Size(128, 20);
            this.vertexCount.TabIndex = 17;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 120);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(68, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "Vertex Count";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(94, 13);
            this.label2.TabIndex = 14;
            this.label2.Text = "Vertex Percent (%)";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(6, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(138, 16);
            this.label1.TabIndex = 30;
            this.label1.Text = "Low Poly 3DS MAX";
            // 
            // barPercent
            // 
            this.barPercent.BackColor = System.Drawing.Color.Transparent;
            this.barPercent.BorderColor = System.Drawing.SystemColors.ActiveBorder;
            this.barPercent.Font = new System.Drawing.Font("Verdana", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.barPercent.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(123)))), ((int)(((byte)(125)))), ((int)(((byte)(123)))));
            this.barPercent.IndentHeight = 6;
            this.barPercent.LargeChange = 20;
            this.barPercent.Location = new System.Drawing.Point(9, 60);
            this.barPercent.Maximum = 100;
            this.barPercent.Minimum = 0;
            this.barPercent.Name = "barPercent";
            this.barPercent.Size = new System.Drawing.Size(245, 47);
            this.barPercent.SmallChange = 10;
            this.barPercent.TabIndex = 33;
            this.barPercent.TickColor = System.Drawing.Color.FromArgb(((int)(((byte)(148)))), ((int)(((byte)(146)))), ((int)(((byte)(148)))));
            this.barPercent.TickFrequency = 10;
            this.barPercent.TickHeight = 4;
            this.barPercent.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.barPercent.TrackerColor = System.Drawing.Color.YellowGreen;
            this.barPercent.TrackerSize = new System.Drawing.Size(16, 16);
            this.barPercent.TrackLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(93)))), ((int)(((byte)(90)))));
            this.barPercent.TrackLineHeight = 3;
            this.barPercent.TrackLineSelectedColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(93)))), ((int)(((byte)(90)))));
            this.barPercent.Value = 50;
            this.barPercent.ValueChanged += new XComponent.SliderBar.ValueChangedHandler(this.barPercent_ValueChanged);
            // 
            // btnEditMode
            // 
            this.btnEditMode.Image = global::Y3D.Properties.Resources.edit_16;
            this.btnEditMode.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnEditMode.Location = new System.Drawing.Point(9, 275);
            this.btnEditMode.Name = "btnEditMode";
            this.btnEditMode.Size = new System.Drawing.Size(119, 27);
            this.btnEditMode.TabIndex = 34;
            this.btnEditMode.Text = "Start Edit Mode";
            this.btnEditMode.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnEditMode.UseVisualStyleBackColor = true;
            this.btnEditMode.Click += new System.EventHandler(this.btnEditMode_Click);
            // 
            // LowMaxControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btnEditMode);
            this.Controls.Add(this.barPercent);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lockVertexPosition);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.preventFlippedNormal);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.favorCompactFaces);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.maxNormalMode);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.maxOMode);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.vertexCount);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Name = "LowMaxControl";
            this.Load += new System.EventHandler(this.LowMaxControl_Load);
            ((System.ComponentModel.ISupportInitialize)(this.vertexCount)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox lockVertexPosition;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.CheckBox preventFlippedNormal;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.CheckBox favorCompactFaces;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox maxNormalMode;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox maxOMode;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown vertexCount;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private XComponent.SliderBar.MACTrackBar barPercent;
        private System.Windows.Forms.Button btnEditMode;
    }
}
