namespace Y3D.Users
{
    partial class AuthForm
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
            this.textLoginUser = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textLoginPass = new System.Windows.Forms.TextBox();
            this.textRegPass1 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textRegUser = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textRegPass2 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnLogin = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnSignUp = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "User";
            // 
            // textLoginUser
            // 
            this.textLoginUser.Location = new System.Drawing.Point(70, 28);
            this.textLoginUser.Name = "textLoginUser";
            this.textLoginUser.Size = new System.Drawing.Size(122, 20);
            this.textLoginUser.TabIndex = 1;
            this.textLoginUser.Enter += new System.EventHandler(this.textLoginUser_Enter);
            this.textLoginUser.Leave += new System.EventHandler(this.textLoginUser_Leave);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Password";
            // 
            // textLoginPass
            // 
            this.textLoginPass.Location = new System.Drawing.Point(70, 54);
            this.textLoginPass.Name = "textLoginPass";
            this.textLoginPass.PasswordChar = '*';
            this.textLoginPass.Size = new System.Drawing.Size(122, 20);
            this.textLoginPass.TabIndex = 3;
            this.textLoginPass.Enter += new System.EventHandler(this.textLoginPass_Enter);
            this.textLoginPass.Leave += new System.EventHandler(this.textLoginPass_Leave);
            // 
            // textRegPass1
            // 
            this.textRegPass1.Location = new System.Drawing.Point(111, 54);
            this.textRegPass1.Name = "textRegPass1";
            this.textRegPass1.PasswordChar = '*';
            this.textRegPass1.Size = new System.Drawing.Size(122, 20);
            this.textRegPass1.TabIndex = 10;
            this.textRegPass1.Enter += new System.EventHandler(this.textRegPass1_Enter);
            this.textRegPass1.Leave += new System.EventHandler(this.textRegPass1_Leave);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(14, 57);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Password";
            // 
            // textRegUser
            // 
            this.textRegUser.Location = new System.Drawing.Point(111, 28);
            this.textRegUser.Name = "textRegUser";
            this.textRegUser.Size = new System.Drawing.Size(122, 20);
            this.textRegUser.TabIndex = 8;
            this.textRegUser.Enter += new System.EventHandler(this.textRegUser_Enter);
            this.textRegUser.Leave += new System.EventHandler(this.textRegUser_Leave);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(14, 31);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "User";
            // 
            // textRegPass2
            // 
            this.textRegPass2.Location = new System.Drawing.Point(111, 77);
            this.textRegPass2.Name = "textRegPass2";
            this.textRegPass2.PasswordChar = '*';
            this.textRegPass2.Size = new System.Drawing.Size(122, 20);
            this.textRegPass2.TabIndex = 12;
            this.textRegPass2.Enter += new System.EventHandler(this.textRegPass2_Enter);
            this.textRegPass2.Leave += new System.EventHandler(this.textRegPass2_Leave);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 80);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(91, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "Confirm Password";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnLogin);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.textLoginUser);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textLoginPass);
            this.groupBox1.Location = new System.Drawing.Point(14, 80);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(216, 153);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "LOGIN";
            // 
            // btnLogin
            // 
            this.btnLogin.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLogin.Image = global::Y3D.Properties.Resources.start;
            this.btnLogin.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnLogin.Location = new System.Drawing.Point(129, 80);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(63, 30);
            this.btnLogin.TabIndex = 4;
            this.btnLogin.Text = "Login";
            this.btnLogin.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnLogin.UseVisualStyleBackColor = true;
            this.btnLogin.Click += new System.EventHandler(this.btnLogin_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.btnSignUp);
            this.groupBox2.Controls.Add(this.textRegPass2);
            this.groupBox2.Controls.Add(this.textRegUser);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.textRegPass1);
            this.groupBox2.Location = new System.Drawing.Point(236, 80);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(256, 153);
            this.groupBox2.TabIndex = 14;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "REGISTER";
            // 
            // btnSignUp
            // 
            this.btnSignUp.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSignUp.Image = global::Y3D.Properties.Resources.user24;
            this.btnSignUp.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnSignUp.Location = new System.Drawing.Point(147, 103);
            this.btnSignUp.Name = "btnSignUp";
            this.btnSignUp.Size = new System.Drawing.Size(86, 30);
            this.btnSignUp.TabIndex = 6;
            this.btnSignUp.Text = "Sign Up";
            this.btnSignUp.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.btnSignUp.UseVisualStyleBackColor = true;
            this.btnSignUp.Click += new System.EventHandler(this.btnSignUp_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(145, 22);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(67, 31);
            this.label6.TabIndex = 15;
            this.label6.Text = "Y3D";
            // 
            // AuthForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(503, 245);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "AuthForm";
            this.Style = MetroFramework.MetroColorStyle.Lime;
            this.Text = "Đăng Nhập";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.AuthForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textLoginUser;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textLoginPass;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.Button btnSignUp;
        private System.Windows.Forms.TextBox textRegPass1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textRegUser;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textRegPass2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label6;
    }
}