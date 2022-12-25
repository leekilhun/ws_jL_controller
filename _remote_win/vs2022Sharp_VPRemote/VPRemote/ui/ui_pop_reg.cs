using System;
using System.Windows.Forms;

namespace VPRemote.ui
{
  public partial class ui_pop_reg : Form
  {
    public ui_pop_reg()
    {
      InitializeComponent();


      // 콤보박스 추가
      DataGridViewComboBoxColumn comboBoxColumn = new DataGridViewComboBoxColumn();
      // 체크박스 추가
      DataGridViewCheckBoxColumn checkBoxColumn = new DataGridViewCheckBoxColumn();
      //버튼 추가
      DataGridViewButtonColumn buttonColumn = new DataGridViewButtonColumn();

      comboBoxColumn.HeaderText = "TestCombo";
      comboBoxColumn.Name = "combo";
      comboBoxColumn.Items.AddRange("Test1", "Test2", "Test3", "Test4", "Test5");

      checkBoxColumn.HeaderText = "Check";
      checkBoxColumn.Name = "check";

      buttonColumn.HeaderText = "Button";
      buttonColumn.Name = "button";

      dataGridView_reg.Columns.Add("column0", "Number");
      dataGridView_reg.Columns.Add(comboBoxColumn);
      dataGridView_reg.Columns.Add(checkBoxColumn);
      dataGridView_reg.Columns.Add(buttonColumn);

      dataGridView_reg.Columns[0].Width = 50;
      dataGridView_reg.Columns[1].Width = 200;
      dataGridView_reg.Columns[2].Width = 80;
      dataGridView_reg.Columns[3].Width = 80;

      dataGridView_reg.AllowUserToAddRows = false;

      for (int i = 0; i < 12; i++)
      {
        dataGridView_reg.Rows.Add(false);
        dataGridView_reg[0, i].Value = i.ToString();
        dataGridView_reg[1, i].Value = "Test1";
        dataGridView_reg[2, i].Value = false;
        dataGridView_reg[3, i].Value = "수정";

      }


      /*
      dataGridView_reg[0, 0].Value = i.ToString();
      dataGridView_reg[1, 0].Value = fileInfo.FullName.ToString();
      dataGridView_reg[2, 0].Value = fileInfo.Length.ToString();
      dataGridView_reg[3, 0].Value = fileInfo.Extension.ToString();
      dataGridView_reg[4, 0].Value = fileInfo.LastWriteTime.ToString();
      dataGridView_reg[5, 0].Value = "Test1";
      dataGridView_reg[6, 0].Value = false;
      dataGridView_reg[7, 0].Value = "수정";
      */
    }

    private void ui_pop_reg_Load(object sender, EventArgs e)
    {
      MainForm main = (MainForm)Owner;

    }

    private void dataGridView_reg_CellClick(object sender, DataGridViewCellEventArgs e)
    {
      try
      {
        if (dataGridView_reg.CurrentCell != null)
        {
          /*
                    // 모든 행의 CheckBox값을 False로 설정
                    for (int i = 0; i < dataGridView_reg.RowCount; i++)
                    {
                      dataGridView_reg[6, i].Value = false; // 6은 CheckBox가 있는 열 번호
                    }
                    // 현재 선택된 행의 CheckBox값을 True로 설정
                    for (int i = 0; i < dataGridView_reg.SelectedRows.Count; i++)
                    {
                      //Convert.ToInt32 => string을 int로 변환
                      intchange = Convert.ToInt32(dataGridView_reg.SelectedRows[i].Cells[0].Value.ToString());
                      dataGridView_reg[6, intchange].Value = true;
                    }
            */
        }
      }
      catch (Exception ex)
      {
        MessageBox.Show(ex.Message);
      }
    }
  }
}
