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
            this.btnThangTest = new System.Windows.Forms.Button();
            this.btnTestD1 = new System.Windows.Forms.Button();
            this.btnGrpcStart = new System.Windows.Forms.Button();
            this.btnGrpcClient = new System.Windows.Forms.Button();
            this.btnGrpcStop = new System.Windows.Forms.Button();
            this.btnTestMerge = new System.Windows.Forms.Button();
            this.btnTestMisc = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnCustom1
            // 
            this.btnCustom1.Location = new System.Drawing.Point(9, 10);
            this.btnCustom1.Margin = new System.Windows.Forms.Padding(2);
            this.btnCustom1.Name = "btnCustom1";
            this.btnCustom1.Size = new System.Drawing.Size(85, 29);
            this.btnCustom1.TabIndex = 6;
            this.btnCustom1.Text = "Call Custom 1";
            this.btnCustom1.UseVisualStyleBackColor = true;
            this.btnCustom1.Click += new System.EventHandler(this.btnCustom1_Click);
            // 
            // btnCustom2
            // 
            this.btnCustom2.Location = new System.Drawing.Point(98, 10);
            this.btnCustom2.Margin = new System.Windows.Forms.Padding(2);
            this.btnCustom2.Name = "btnCustom2";
            this.btnCustom2.Size = new System.Drawing.Size(77, 29);
            this.btnCustom2.TabIndex = 5;
            this.btnCustom2.Text = "Pre file save";
            this.btnCustom2.UseVisualStyleBackColor = true;
            this.btnCustom2.Click += new System.EventHandler(this.btnCustom2_Click);
            // 
            // btnThangTest
            // 
            this.btnThangTest.Location = new System.Drawing.Point(180, 10);
            this.btnThangTest.Margin = new System.Windows.Forms.Padding(2);
            this.btnThangTest.Name = "btnThangTest";
            this.btnThangTest.Size = new System.Drawing.Size(73, 29);
            this.btnThangTest.TabIndex = 7;
            this.btnThangTest.Text = "Thang test";
            this.btnThangTest.UseVisualStyleBackColor = true;
            this.btnThangTest.Click += new System.EventHandler(this.btnThangTest_Click);
            // 
            // btnTestD1
            // 
            this.btnTestD1.Location = new System.Drawing.Point(258, 11);
            this.btnTestD1.Margin = new System.Windows.Forms.Padding(2);
            this.btnTestD1.Name = "btnTestD1";
            this.btnTestD1.Size = new System.Drawing.Size(73, 28);
            this.btnTestD1.TabIndex = 8;
            this.btnTestD1.Text = "Test D1";
            this.btnTestD1.UseVisualStyleBackColor = true;
            this.btnTestD1.Click += new System.EventHandler(this.btnTestD1_Click);
            // 
            // btnGrpcStart
            // 
            this.btnGrpcStart.Location = new System.Drawing.Point(9, 56);
            this.btnGrpcStart.Margin = new System.Windows.Forms.Padding(2);
            this.btnGrpcStart.Name = "btnGrpcStart";
            this.btnGrpcStart.Size = new System.Drawing.Size(103, 28);
            this.btnGrpcStart.TabIndex = 9;
            this.btnGrpcStart.Text = "Grpc start server";
            this.btnGrpcStart.UseVisualStyleBackColor = true;
            this.btnGrpcStart.Click += new System.EventHandler(this.btnGrpcStart_Click);
            // 
            // btnGrpcClient
            // 
            this.btnGrpcClient.Location = new System.Drawing.Point(117, 56);
            this.btnGrpcClient.Margin = new System.Windows.Forms.Padding(2);
            this.btnGrpcClient.Name = "btnGrpcClient";
            this.btnGrpcClient.Size = new System.Drawing.Size(89, 28);
            this.btnGrpcClient.TabIndex = 10;
            this.btnGrpcClient.Text = "Grpc Client";
            this.btnGrpcClient.UseVisualStyleBackColor = true;
            this.btnGrpcClient.Click += new System.EventHandler(this.btnGrpcClient_Click);
            // 
            // btnGrpcStop
            // 
            this.btnGrpcStop.Location = new System.Drawing.Point(242, 56);
            this.btnGrpcStop.Margin = new System.Windows.Forms.Padding(2);
            this.btnGrpcStop.Name = "btnGrpcStop";
            this.btnGrpcStop.Size = new System.Drawing.Size(89, 28);
            this.btnGrpcStop.TabIndex = 11;
            this.btnGrpcStop.Text = "Stop grpc";
            this.btnGrpcStop.UseVisualStyleBackColor = true;
            this.btnGrpcStop.Click += new System.EventHandler(this.btnGrpcStop_Click);
            // 
            // btnTestMerge
            // 
            this.btnTestMerge.Location = new System.Drawing.Point(336, 10);
            this.btnTestMerge.Name = "btnTestMerge";
            this.btnTestMerge.Size = new System.Drawing.Size(90, 29);
            this.btnTestMerge.TabIndex = 12;
            this.btnTestMerge.Text = "Test Merge";
            this.btnTestMerge.UseVisualStyleBackColor = true;
            this.btnTestMerge.Click += new System.EventHandler(this.btnTestMerge_Click);
            // 
            // btnTestMisc
            // 
            this.btnTestMisc.Location = new System.Drawing.Point(9, 110);
            this.btnTestMisc.Name = "btnTestMisc";
            this.btnTestMisc.Size = new System.Drawing.Size(75, 28);
            this.btnTestMisc.TabIndex = 13;
            this.btnTestMisc.Text = "test linh tinh";
            this.btnTestMisc.UseVisualStyleBackColor = true;
            this.btnTestMisc.Click += new System.EventHandler(this.btnTestMisc_Click);
            // 
            // YTestForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(477, 298);
            this.Controls.Add(this.btnTestMisc);
            this.Controls.Add(this.btnTestMerge);
            this.Controls.Add(this.btnGrpcStop);
            this.Controls.Add(this.btnGrpcClient);
            this.Controls.Add(this.btnGrpcStart);
            this.Controls.Add(this.btnTestD1);
            this.Controls.Add(this.btnThangTest);
            this.Controls.Add(this.btnCustom1);
            this.Controls.Add(this.btnCustom2);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "YTestForm";
            this.Text = "YTestForm";
            this.Activated += new System.EventHandler(this.YTestForm_Activated);
            this.Deactivate += new System.EventHandler(this.YTestForm_Deactivate);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.YTestForm_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnCustom1;
        private System.Windows.Forms.Button btnCustom2;
        private System.Windows.Forms.Button btnThangTest;
        private System.Windows.Forms.Button btnTestD1;
        private System.Windows.Forms.Button btnGrpcStart;
        private System.Windows.Forms.Button btnGrpcClient;
        private System.Windows.Forms.Button btnGrpcStop;
        private System.Windows.Forms.Button btnTestMerge;
        private System.Windows.Forms.Button btnTestMisc;
    }
}