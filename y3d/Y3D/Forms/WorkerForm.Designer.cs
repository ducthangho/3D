namespace Y3D.Forms
{
    partial class WorkerForm
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(WorkerForm));
            this.dlvWorker = new BrightIdeasSoftware.DataListView();
            this.olvColumnName = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnAddr = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.olvColumnStatus = ((BrightIdeasSoftware.OLVColumn)(new BrightIdeasSoftware.OLVColumn()));
            this.workerimglist = new System.Windows.Forms.ImageList(this.components);
            this.btnNew = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnDel = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnStart = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dlvWorker)).BeginInit();
            this.SuspendLayout();
            // 
            // dlvWorker
            // 
            this.dlvWorker.AllColumns.Add(this.olvColumnName);
            this.dlvWorker.AllColumns.Add(this.olvColumnAddr);
            this.dlvWorker.AllColumns.Add(this.olvColumnStatus);
            this.dlvWorker.CellEditUseWholeCell = false;
            this.dlvWorker.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.olvColumnName,
            this.olvColumnAddr,
            this.olvColumnStatus});
            this.dlvWorker.Cursor = System.Windows.Forms.Cursors.Default;
            this.dlvWorker.DataSource = null;
            this.dlvWorker.FullRowSelect = true;
            this.dlvWorker.Location = new System.Drawing.Point(12, 12);
            this.dlvWorker.Name = "dlvWorker";
            this.dlvWorker.ShowGroups = false;
            this.dlvWorker.Size = new System.Drawing.Size(320, 169);
            this.dlvWorker.SmallImageList = this.workerimglist;
            this.dlvWorker.TabIndex = 0;
            this.dlvWorker.UseCompatibleStateImageBehavior = false;
            this.dlvWorker.View = System.Windows.Forms.View.Details;
            this.dlvWorker.Click += new System.EventHandler(this.dlvWorker_Click);
            // 
            // olvColumnName
            // 
            this.olvColumnName.AspectName = "Wname";
            this.olvColumnName.Text = "Worker";
            this.olvColumnName.Width = 80;
            // 
            // olvColumnAddr
            // 
            this.olvColumnAddr.AspectName = "IpAddress";
            this.olvColumnAddr.FillsFreeSpace = true;
            this.olvColumnAddr.Groupable = false;
            this.olvColumnAddr.Text = "IP Address";
            // 
            // olvColumnStatus
            // 
            this.olvColumnStatus.AspectName = "status";
            this.olvColumnStatus.Text = "Status";
            this.olvColumnStatus.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.olvColumnStatus.Width = 50;
            // 
            // workerimglist
            // 
            this.workerimglist.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("workerimglist.ImageStream")));
            this.workerimglist.TransparentColor = System.Drawing.Color.Transparent;
            this.workerimglist.Images.SetKeyName(0, "c_gray");
            this.workerimglist.Images.SetKeyName(1, "c_green");
            this.workerimglist.Images.SetKeyName(2, "c_yellow");
            this.workerimglist.Images.SetKeyName(3, "start");
            this.workerimglist.Images.SetKeyName(4, "stop");
            this.workerimglist.Images.SetKeyName(5, "c_red");
            // 
            // btnNew
            // 
            this.btnNew.Location = new System.Drawing.Point(12, 187);
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(75, 32);
            this.btnNew.TabIndex = 1;
            this.btnNew.Text = "New Worker";
            this.btnNew.UseVisualStyleBackColor = true;
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(261, 187);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(71, 32);
            this.btnClose.TabIndex = 5;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnDel
            // 
            this.btnDel.Enabled = false;
            this.btnDel.Image = global::Y3D.Properties.Resources.trash;
            this.btnDel.Location = new System.Drawing.Point(183, 187);
            this.btnDel.Name = "btnDel";
            this.btnDel.Size = new System.Drawing.Size(39, 32);
            this.btnDel.TabIndex = 4;
            this.btnDel.UseVisualStyleBackColor = true;
            this.btnDel.Click += new System.EventHandler(this.btnDel_Click);
            // 
            // btnStop
            // 
            this.btnStop.Enabled = false;
            this.btnStop.Image = global::Y3D.Properties.Resources.stop;
            this.btnStop.Location = new System.Drawing.Point(138, 187);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(39, 32);
            this.btnStop.TabIndex = 3;
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnStart
            // 
            this.btnStart.Enabled = false;
            this.btnStart.Image = global::Y3D.Properties.Resources.start;
            this.btnStart.Location = new System.Drawing.Point(93, 187);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(39, 32);
            this.btnStart.TabIndex = 2;
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // WorkerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(344, 222);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnDel);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnStart);
            this.Controls.Add(this.btnNew);
            this.Controls.Add(this.dlvWorker);
            this.Name = "WorkerForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "WorkerForm";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.WorkerForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dlvWorker)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private BrightIdeasSoftware.OLVColumn olvColumnName;
        private BrightIdeasSoftware.OLVColumn olvColumnAddr;
        private BrightIdeasSoftware.OLVColumn olvColumnStatus;
        private System.Windows.Forms.ImageList workerimglist;
        private System.Windows.Forms.Button btnNew;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Button btnDel;
        private System.Windows.Forms.Button btnClose;
        private BrightIdeasSoftware.DataListView dlvWorker;
    }
}