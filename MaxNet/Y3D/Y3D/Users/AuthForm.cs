using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Y3D.Users
{
    public partial class AuthForm : Form
    {
        public AuthForm()
        {
            InitializeComponent();
        }

        private void AuthForm_Load(object sender, EventArgs e)
        {
            Y3D.Projects.Utils.initSystem();
            var ym = Auth.getMachineInfo();
        }

        private void btnSignUp_Click(object sender, EventArgs e)
        {
            if (textRegUser.TextLength==0)
            {
                MessageBox.Show("Username cannot be empty!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (textRegPass1.Text != textRegPass2.Text)
            {
                MessageBox.Show("Password does not match the confirm password!","Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
                return;
            }
            var rep = Auth.signUp(textRegUser.Text, textRegPass1.Text);
            if (!rep.Rep.Error)
            {
                this.Hide();
                var fm = new Forms.YMainForm();
                fm.Show();
            } else
            {
                MessageBox.Show(rep.Rep.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            if (textLoginUser.TextLength == 0)
            {
                MessageBox.Show("Username cannot be empty!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            var rep = Auth.signIn(textLoginUser.Text, textLoginPass.Text);
            if (!rep.Rep.Error)
            {
                this.Hide();
                var fm = new Forms.YMainForm();
                fm.Show();
            }
            else
            {
                MessageBox.Show(rep.Rep.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

        }

        private void textLoginUser_Enter(object sender, EventArgs e)
        {
            this.AcceptButton = btnLogin;
        }

        private void textLoginUser_Leave(object sender, EventArgs e)
        {
            this.AcceptButton = null;
        }

        private void textLoginPass_Enter(object sender, EventArgs e)
        {
            this.AcceptButton = btnLogin;
        }

        private void textLoginPass_Leave(object sender, EventArgs e)
        {
            this.AcceptButton = null;
        }

        private void textRegUser_Enter(object sender, EventArgs e)
        {
            this.AcceptButton = btnSignUp;
        }

        private void textRegPass1_Enter(object sender, EventArgs e)
        {
            this.AcceptButton = btnSignUp;
        }

        private void textRegPass2_Enter(object sender, EventArgs e)
        {
            this.AcceptButton = btnSignUp;
        }

        private void textRegUser_Leave(object sender, EventArgs e)
        {
            this.AcceptButton = null;
        }

        private void textRegPass1_Leave(object sender, EventArgs e)
        {
            this.AcceptButton = null;
        }

        private void textRegPass2_Leave(object sender, EventArgs e)
        {
            this.AcceptButton = null;
        }
    }
}
