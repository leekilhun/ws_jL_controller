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
      this.SuspendLayout();
      // 
      // lblTitleDate
      // 
      this.lblTitleDate.BackColor = System.Drawing.Color.Transparent;
      this.lblTitleDate.Font = new System.Drawing.Font("Tahoma", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.lblTitleDate.ForeColor = System.Drawing.Color.DarkRed;
      this.lblTitleDate.Location = new System.Drawing.Point(681, 9);
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
      this.RX_textBox.Location = new System.Drawing.Point(12, 477);
      this.RX_textBox.Multiline = true;
      this.RX_textBox.Name = "RX_textBox";
      this.RX_textBox.Size = new System.Drawing.Size(922, 174);
      this.RX_textBox.TabIndex = 6;
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Font = new System.Drawing.Font("굴림", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
      this.label1.Location = new System.Drawing.Point(12, 448);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(158, 15);
      this.label1.TabIndex = 7;
      this.label1.Text = "수신 된 Packet Data";
      // 
      // MainForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(958, 676);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.RX_textBox);
      this.Controls.Add(this.lblTitleDate);
      this.Name = "MainForm";
      this.Text = "MainForm";
      this.Load += new System.EventHandler(this.MainForm_Load);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label lblTitleDate;
    private System.IO.Ports.SerialPort serialPort_peeler;
    private System.Windows.Forms.TextBox RX_textBox;
    private System.Windows.Forms.Label label1;
  }
}

