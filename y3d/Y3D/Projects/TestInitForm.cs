﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using y3d.e;

namespace Y3D.Projects
{
    public partial class TestInitForm : MetroFramework.Forms.MetroForm
    {
        public y3d.e.InitTestPreset testPreset = null;
        public string note { get; set; }

        public InitTestPreset defaultInit()
        {
            InitTestPreset tp = new InitTestPreset();
            tp.Unwrap = new EUnwrap();
            tp.Unwrap.Max3D = new MaxUnwrap();
            tp.Unwrap.Max3D.Angle = 66;
            return tp;
        }

        public TestInitForm()
        {
            InitializeComponent();
            this.AcceptButton = btnMakeTest;
            testPreset = defaultInit();
        }

        private void btnMakeTest_Click(object sender, EventArgs e)
        {
            note = txtNote.Text;
            this.DialogResult = DialogResult.OK;
        }
    }
}