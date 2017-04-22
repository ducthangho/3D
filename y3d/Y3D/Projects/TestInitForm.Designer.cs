namespace Y3D.Projects
{
    partial class TestInitForm
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
            this.metroComboBox1 = new MetroFramework.Controls.MetroComboBox();
            this.metroLabel1 = new MetroFramework.Controls.MetroLabel();
            this.txtNote = new MetroFramework.Controls.MetroTextBox();
            this.metroLabel2 = new MetroFramework.Controls.MetroLabel();
            this.btnMakeTest = new MetroFramework.Controls.MetroTile();
            this.btnPresetManager = new MetroFramework.Controls.MetroButton();
            this.SuspendLayout();
            // 
            // metroComboBox1
            // 
            this.metroComboBox1.FormattingEnabled = true;
            this.metroComboBox1.ItemHeight = 23;
            this.metroComboBox1.Items.AddRange(new object[] {
            "Default",
            "Preset 1",
            "Preset 2"});
            this.metroComboBox1.Location = new System.Drawing.Point(23, 93);
            this.metroComboBox1.Name = "metroComboBox1";
            this.metroComboBox1.Size = new System.Drawing.Size(121, 29);
            this.metroComboBox1.Style = MetroFramework.MetroColorStyle.Lime;
            this.metroComboBox1.TabIndex = 0;
            this.metroComboBox1.UseSelectable = true;
            // 
            // metroLabel1
            // 
            this.metroLabel1.AutoSize = true;
            this.metroLabel1.Location = new System.Drawing.Point(23, 71);
            this.metroLabel1.Name = "metroLabel1";
            this.metroLabel1.Size = new System.Drawing.Size(99, 19);
            this.metroLabel1.TabIndex = 1;
            this.metroLabel1.Text = "Settings Presets";
            // 
            // txtNote
            // 
            // 
            // 
            // 
            this.txtNote.CustomButton.Image = null;
            this.txtNote.CustomButton.Location = new System.Drawing.Point(263, 1);
            this.txtNote.CustomButton.Name = "";
            this.txtNote.CustomButton.Size = new System.Drawing.Size(27, 27);
            this.txtNote.CustomButton.Style = MetroFramework.MetroColorStyle.Blue;
            this.txtNote.CustomButton.TabIndex = 1;
            this.txtNote.CustomButton.Theme = MetroFramework.MetroThemeStyle.Light;
            this.txtNote.CustomButton.UseSelectable = true;
            this.txtNote.CustomButton.Visible = false;
            this.txtNote.Lines = new string[0];
            this.txtNote.Location = new System.Drawing.Point(23, 153);
            this.txtNote.MaxLength = 32767;
            this.txtNote.Name = "txtNote";
            this.txtNote.PasswordChar = '\0';
            this.txtNote.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.txtNote.SelectedText = "";
            this.txtNote.SelectionLength = 0;
            this.txtNote.SelectionStart = 0;
            this.txtNote.ShortcutsEnabled = true;
            this.txtNote.Size = new System.Drawing.Size(291, 29);
            this.txtNote.TabIndex = 2;
            this.txtNote.UseSelectable = true;
            this.txtNote.WaterMarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(109)))), ((int)(((byte)(109)))), ((int)(((byte)(109)))));
            this.txtNote.WaterMarkFont = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Pixel);
            // 
            // metroLabel2
            // 
            this.metroLabel2.AutoSize = true;
            this.metroLabel2.Location = new System.Drawing.Point(23, 131);
            this.metroLabel2.Name = "metroLabel2";
            this.metroLabel2.Size = new System.Drawing.Size(38, 19);
            this.metroLabel2.TabIndex = 3;
            this.metroLabel2.Text = "Note";
            // 
            // btnMakeTest
            // 
            this.btnMakeTest.ActiveControl = null;
            this.btnMakeTest.Location = new System.Drawing.Point(23, 188);
            this.btnMakeTest.Name = "btnMakeTest";
            this.btnMakeTest.Size = new System.Drawing.Size(89, 40);
            this.btnMakeTest.Style = MetroFramework.MetroColorStyle.Lime;
            this.btnMakeTest.TabIndex = 5;
            this.btnMakeTest.Text = "Make Test";
            this.btnMakeTest.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.btnMakeTest.TileTextFontWeight = MetroFramework.MetroTileTextWeight.Bold;
            this.btnMakeTest.UseSelectable = true;
            this.btnMakeTest.Click += new System.EventHandler(this.btnMakeTest_Click);
            // 
            // btnPresetManager
            // 
            this.btnPresetManager.Location = new System.Drawing.Point(151, 93);
            this.btnPresetManager.Name = "btnPresetManager";
            this.btnPresetManager.Size = new System.Drawing.Size(29, 29);
            this.btnPresetManager.TabIndex = 6;
            this.btnPresetManager.Text = "...";
            this.btnPresetManager.UseSelectable = true;
            // 
            // TestInitForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(337, 243);
            this.Controls.Add(this.btnPresetManager);
            this.Controls.Add(this.btnMakeTest);
            this.Controls.Add(this.metroLabel2);
            this.Controls.Add(this.txtNote);
            this.Controls.Add(this.metroLabel1);
            this.Controls.Add(this.metroComboBox1);
            this.Name = "TestInitForm";
            this.Style = MetroFramework.MetroColorStyle.Lime;
            this.Text = "New Test";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MetroFramework.Controls.MetroComboBox metroComboBox1;
        private MetroFramework.Controls.MetroLabel metroLabel1;
        private MetroFramework.Controls.MetroTextBox txtNote;
        private MetroFramework.Controls.MetroLabel metroLabel2;
        private MetroFramework.Controls.MetroTile btnMakeTest;
        private MetroFramework.Controls.MetroButton btnPresetManager;
    }
}