namespace VPRemote.ui
{
  partial class ui_pop_reg
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
      this.paneltop = new System.Windows.Forms.Panel();
      this.panel5 = new System.Windows.Forms.Panel();
      this.dataGridView_reg = new System.Windows.Forms.DataGridView();
      this.paneltop.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.dataGridView_reg)).BeginInit();
      this.SuspendLayout();
      // 
      // paneltop
      // 
      this.paneltop.Controls.Add(this.panel5);
      this.paneltop.Dock = System.Windows.Forms.DockStyle.Top;
      this.paneltop.Location = new System.Drawing.Point(0, 0);
      this.paneltop.Name = "paneltop";
      this.paneltop.Size = new System.Drawing.Size(769, 18);
      this.paneltop.TabIndex = 13;
      // 
      // panel5
      // 
      this.panel5.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(31)))), ((int)(((byte)(30)))), ((int)(((byte)(68)))));
      this.panel5.Dock = System.Windows.Forms.DockStyle.Top;
      this.panel5.Location = new System.Drawing.Point(0, 0);
      this.panel5.Name = "panel5";
      this.panel5.Size = new System.Drawing.Size(769, 5);
      this.panel5.TabIndex = 4;
      // 
      // dataGridView_reg
      // 
      this.dataGridView_reg.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.dataGridView_reg.Location = new System.Drawing.Point(21, 57);
      this.dataGridView_reg.Name = "dataGridView_reg";
      this.dataGridView_reg.RowTemplate.Height = 23;
      this.dataGridView_reg.Size = new System.Drawing.Size(474, 225);
      this.dataGridView_reg.TabIndex = 14;
      this.dataGridView_reg.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_reg_CellClick);
      // 
      // ui_pop_reg
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(769, 443);
      this.Controls.Add(this.dataGridView_reg);
      this.Controls.Add(this.paneltop);
      this.Name = "ui_pop_reg";
      this.Text = "ui_pop_reg";
      this.Load += new System.EventHandler(this.ui_pop_reg_Load);
      this.paneltop.ResumeLayout(false);
      ((System.ComponentModel.ISupportInitialize)(this.dataGridView_reg)).EndInit();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.Panel paneltop;
    private System.Windows.Forms.Panel panel5;
    private System.Windows.Forms.DataGridView dataGridView_reg;
  }
}