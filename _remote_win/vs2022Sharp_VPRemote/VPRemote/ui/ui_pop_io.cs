using FontAwesome.Sharp;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace VPRemote.ui
{
  public partial class ui_pop_io : Form
  {

    private struct RGBColors
    {
      public static Color color1 = Color.FromArgb(172, 126, 241);
      public static Color color2 = Color.FromArgb(249, 118, 176);
      public static Color color3 = Color.FromArgb(253, 138, 114);
      public static Color color4 = Color.FromArgb(95, 77, 221);
      public static Color color5 = Color.FromArgb(249, 88, 155);
      public static Color color6 = Color.FromArgb(24, 161, 251);
    }

    private IconButton _currentBtn;

    public ui_pop_io()
    {
      InitializeComponent();
    }

    private void ui_pop_io_Load(object sender, EventArgs e)
    {
      MainForm main = (MainForm)Owner;
    }

    private void ActivateButton(object senderBtn)
    {
      if (senderBtn != null)
      {
        DisableButton();

        //Botton                
        _currentBtn = (IconButton)senderBtn;
        _currentBtn.BackColor = Color.Orange;
        _currentBtn.ForeColor = Color.DarkSlateBlue; ;
        _currentBtn.IconChar = IconChar.ToggleOn;
        _currentBtn.IconColor = Color.DarkRed;

      }

    }

    private void DisableButton()
    {
      if (_currentBtn != null)
      {
        _currentBtn.BackColor = Color.White;
        _currentBtn.ForeColor = Color.MidnightBlue;
        _currentBtn.IconChar = IconChar.ToggleOff;
        _currentBtn.TextAlign = ContentAlignment.MiddleLeft;
        _currentBtn.IconColor = Color.DimGray;
        _currentBtn.TextImageRelation = TextImageRelation.ImageBeforeText;
        _currentBtn.ImageAlign = ContentAlignment.MiddleLeft;

      }

    }

    private void btnIO_0_Click(object sender, EventArgs e)
    {
      ActivateButton(sender);
    }


  }
}
