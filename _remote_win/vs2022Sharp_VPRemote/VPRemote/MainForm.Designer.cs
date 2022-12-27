namespace VPRemote
{
  partial class MainForm
  {
    /// <summary>
    /// 필수 디자이너 변수입니다.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// 사용 중인 모든 리소스를 정리합니다.
    /// </summary>
    /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form 디자이너에서 생성한 코드

    /// <summary>
    /// 디자이너 지원에 필요한 메서드입니다. 
    /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container();
      this.lblTitleDate = new System.Windows.Forms.Label();
      this.serialPort_peeler = new System.IO.Ports.SerialPort(this.components);
      this.RX_textBox = new System.Windows.Forms.TextBox();
      this.label1 = new System.Windows.Forms.Label();
      this.btnPopIO = new System.Windows.Forms.Button();
      this.panel1 = new System.Windows.Forms.Panel();
      this.btnPopData = new System.Windows.Forms.Button();
      this.btnPopMotorCtrl = new System.Windows.Forms.Button();
      this.paneltop = new System.Windows.Forms.Panel();
      this.panel5 = new System.Windows.Forms.Panel();
      this.panel1.SuspendLayout();
      this.paneltop.SuspendLayout();
      this.SuspendLayout();
      // 
      // lblTitleDate
      // 
      this.lblTitleDate.BackColor = System.Drawing.Color.Transparent;
      this.lblTitleDate.Font = new System.Drawing.Font("Tahoma", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.lblTitleDate.ForeColor = System.Drawing.Color.DarkRed;
      this.lblTitleDate.Location = new System.Drawing.Point(695, 8);
      this.lblTitleDate.Name = "lblTitleDate";
      this.lblTitleDate.Size = new System.Drawing.Size(253, 34);
      this.lblTitleDate.TabIndex = 5;
      this.lblTitleDate.Text = "System Date";
      this.lblTitleDate.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      this.lblTitleDate.Visible = false;
      // 
      // serialPort_peeler
      // 
      this.serialPort_peeler.BaudRate = 115200;
      this.serialPort_peeler.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort_peeler_DataReceived);
      // 
      // RX_textBox
      // 
      this.RX_textBox.BackColor = System.Drawing.SystemColors.Info;
      this.RX_textBox.Enabled = false;
      this.RX_textBox.Font = new System.Drawing.Font("돋움", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
      this.RX_textBox.Location = new System.Drawing.Point(5, 499);
      this.RX_textBox.Multiline = true;
      this.RX_textBox.Name = "RX_textBox";
      this.RX_textBox.Size = new System.Drawing.Size(945, 174);
      this.RX_textBox.TabIndex = 6;
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Font = new System.Drawing.Font("굴림", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
      this.label1.Location = new System.Drawing.Point(7, 473);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(158, 15);
      this.label1.TabIndex = 7;
      this.label1.Text = "수신 된 Packet Data";
      // 
      // btnPopIO
      // 
      this.btnPopIO.Location = new System.Drawing.Point(17, 8);
      this.btnPopIO.Margin = new System.Windows.Forms.Padding(2);
      this.btnPopIO.Name = "btnPopIO";
      this.btnPopIO.Size = new System.Drawing.Size(156, 46);
      this.btnPopIO.TabIndex = 8;
      this.btnPopIO.Text = "IO Monitor";
      this.btnPopIO.UseVisualStyleBackColor = true;
      this.btnPopIO.Click += new System.EventHandler(this.btnIoMonitor_Click);
      // 
      // panel1
      // 
      this.panel1.BackColor = System.Drawing.Color.Azure;
      this.panel1.Controls.Add(this.btnPopData);
      this.panel1.Controls.Add(this.btnPopMotorCtrl);
      this.panel1.Controls.Add(this.btnPopIO);
      this.panel1.Location = new System.Drawing.Point(762, 51);
      this.panel1.Margin = new System.Windows.Forms.Padding(2);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size(186, 443);
      this.panel1.TabIndex = 9;
      // 
      // btnPopData
      // 
      this.btnPopData.Location = new System.Drawing.Point(17, 142);
      this.btnPopData.Margin = new System.Windows.Forms.Padding(2);
      this.btnPopData.Name = "btnPopData";
      this.btnPopData.Size = new System.Drawing.Size(156, 46);
      this.btnPopData.TabIndex = 10;
      this.btnPopData.Text = "Data View_Edit";
      this.btnPopData.UseVisualStyleBackColor = true;
      this.btnPopData.Click += new System.EventHandler(this.btnPopData_Click);
      // 
      // btnPopMotorCtrl
      // 
      this.btnPopMotorCtrl.Location = new System.Drawing.Point(17, 72);
      this.btnPopMotorCtrl.Margin = new System.Windows.Forms.Padding(2);
      this.btnPopMotorCtrl.Name = "btnPopMotorCtrl";
      this.btnPopMotorCtrl.Size = new System.Drawing.Size(156, 46);
      this.btnPopMotorCtrl.TabIndex = 9;
      this.btnPopMotorCtrl.Text = "Motor Control";
      this.btnPopMotorCtrl.UseVisualStyleBackColor = true;
      this.btnPopMotorCtrl.Click += new System.EventHandler(this.btnPopMotorCtrl_Click);
      // 
      // paneltop
      // 
      this.paneltop.Controls.Add(this.panel5);
      this.paneltop.Controls.Add(this.lblTitleDate);
      this.paneltop.Dock = System.Windows.Forms.DockStyle.Top;
      this.paneltop.Location = new System.Drawing.Point(0, 0);
      this.paneltop.Name = "paneltop";
      this.paneltop.Size = new System.Drawing.Size(954, 47);
      this.paneltop.TabIndex = 13;
      // 
      // panel5
      // 
      this.panel5.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(31)))), ((int)(((byte)(30)))), ((int)(((byte)(68)))));
      this.panel5.Dock = System.Windows.Forms.DockStyle.Top;
      this.panel5.Location = new System.Drawing.Point(0, 0);
      this.panel5.Name = "panel5";
      this.panel5.Size = new System.Drawing.Size(954, 5);
      this.panel5.TabIndex = 4;
      // 
      // MainForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(954, 676);
      this.Controls.Add(this.paneltop);
      this.Controls.Add(this.panel1);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.RX_textBox);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
      this.Name = "MainForm";
      this.Text = "MainForm";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
      this.Load += new System.EventHandler(this.MainForm_Load);
      this.panel1.ResumeLayout(false);
      this.paneltop.ResumeLayout(false);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label lblTitleDate;
    private System.IO.Ports.SerialPort serialPort_peeler;
    private System.Windows.Forms.TextBox RX_textBox;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Button btnPopIO;
    private System.Windows.Forms.Panel panel1;
    private System.Windows.Forms.Button btnPopMotorCtrl;
    private System.Windows.Forms.Button btnPopData;
    private System.Windows.Forms.Panel paneltop;
    private System.Windows.Forms.Panel panel5;
  }
}

