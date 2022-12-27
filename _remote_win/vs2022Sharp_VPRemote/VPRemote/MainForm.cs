using System;
using System.Timers;
using System.Windows.Forms;
using VPRemote.ap;

namespace VPRemote
{
  public partial class MainForm : Form
  {


    private static System.Timers.Timer _sysTimer;// 날짜 표시
    private delegate void delUpdateTime(); // 날짜 시간 업데이트
    private bool _enable_Timer; // 타이머 제어 변수


    Peeler_remote _peeler_remote = new Peeler_remote();

    //public bool 

    public MainForm() //  MainForm이 생성하면서 호출되는 생성자 함수, 내부 내용이 순차적으로 실행됨
    {
      InitializeComponent();

      _enable_Timer = false;
      //RX_textBox.L



      this.DoubleBuffered = true;
      this.MaximizedBounds = Screen.FromHandle(this.Handle).WorkingArea;


      _peeler_remote.AddCallbackFunc(doUpdateTextBox);
      _peeler_remote.AddCommWriteFunc(SerialCommWrite);

      SetTimer();
      if (SetCSerialComm())
        MessageBox.Show("Com Port Opened!");
      else
        MessageBox.Show("Com Port Open Fail!");

      _peeler_remote.ThreadRun();

    }

    ~MainForm()
    {
    }

    private void SetTimer() // 타이머 셋팅 
    {
      // Create a timer with a two second interval.
      _sysTimer = new System.Timers.Timer(1000);
      // Hook up the Elapsed event for the timer. 
      _sysTimer.Elapsed += OnTimedEvent;
      _sysTimer.AutoReset = true;
      _sysTimer.Enabled = true;
    }


    private bool SetCSerialComm() // 시리얼 포트 오픈
    {
      if (serialPort_peeler.IsOpen == false)
      {
        try
        {
          serialPort_peeler.PortName = "COM4";
          serialPort_peeler.Open();

          _peeler_remote.ThreadRun();
          // _peeler_remote.StartTick();
        }
        catch (Exception ex)
        {
          return false;
          throw ex; // 에러 처리는 나중에
        }

      }
      return serialPort_peeler.IsOpen;
    }


    private void OnTimedEvent(object sender, ElapsedEventArgs e) // 타이머 콜백 메세지 함수
    {
      if (_enable_Timer)
      {
        BeginInvoke(new delUpdateTime(UpdateTime));
      }

    }

    private void UpdateTime() // 타이머 실행 함수
    {
      lblTitleDate.Visible = true;
      lblTitleDate.Text = DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss");
    }


    public static DateTime Delay(int ms)
    {
      DateTime ThisMoment = DateTime.Now;
      TimeSpan duration = new TimeSpan(0, 0, 0, 0, ms);
      DateTime AfterWards = ThisMoment.Add(duration);
      while (AfterWards >= ThisMoment)
      {
        System.Windows.Forms.Application.DoEvents();
        ThisMoment = DateTime.Now;
      }

      return DateTime.Now;
    }

    private void MainForm_Load(object sender, EventArgs e)
    {
      _enable_Timer = true;
    }



    private void btnIoMonitor_Click(object sender, EventArgs e)
    {
      VPRemote.ui.ui_pop_io io_view = new VPRemote.ui.ui_pop_io();
      io_view.Show(this);
    }


    private void SerialPort_peeler_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
    {
      byte[] temp = new byte[serialPort_peeler.BytesToRead];
      serialPort_peeler.Read(temp, 0, temp.Length);

      foreach (var item in temp)
      {
        _peeler_remote.PutReceive_1ByteData(item);
      }
    }

    private void btnPopMotorCtrl_Click(object sender, EventArgs e)
    {

    }

    private void btnPopData_Click(object sender, EventArgs e)
    {
      VPRemote.ui.ui_pop_reg reg_view = new VPRemote.ui.ui_pop_reg();
      reg_view.Show(this);
    }

    public void SerialCommWrite(byte[] buffer, int offset, int count)
    {
      if(serialPort_peeler.IsOpen)
        serialPort_peeler.Write(buffer, offset, count);
      //VPRemote.MainForm.SerialCommWrite(buffer, offset, count);
    }


    public void doUpdateTextBox(string str)
    {
      //


      //
      if (RX_textBox.InvokeRequired)
      {

        RX_textBox.Invoke(new Action(delegate ()
        {
          if (RX_textBox.Lines.Length >= 13)
          {
            RX_textBox.Text = "";
          }

          RX_textBox.Text += DateTime.Now.ToString("[yyyy-MM-dd hh:mm:ss] [CONTROLLER -> PC] ");
          RX_textBox.Text += str;
          RX_textBox.Text += "\r\n";
        }));
      }
      else
      {
        if (RX_textBox.Lines.Length >= 13)
        {
          RX_textBox.Text = "";
        }

        RX_textBox.Text = DateTime.Now.ToString("[yyyy-MM-dd hh:mm:ss] [CONTROLLER -> PC] ");
        RX_textBox.Text += str;
        RX_textBox.Text += "\r\n";
      }
    }

    private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
    {
      _enable_Timer = false;
      _peeler_remote.ThreadStop();
    }




#if false



    private void SerialPortTick(object sender, System.Timers.ElapsedEventArgs e)
    {

      byte get_data = 0;

      if (_qReceiveBuffer.Available() > 0)
      {
        _qReceiveBuffer.Get(ref get_data);

        if ((UTL.millis() - _packet.State.GetPrevTime()) > 100)
        {
          _packet.Buffer.Clear();
          _packet.Checksum = 0xffff;
          _packet.State.SetStep(0);
        }

        if (recivePacket(get_data))
        {
          // 수신이 패킷 정보가 일치할 경우

          /*
          string hex;
          foreach (var elm in _packet.Buffer)
          {
            hex = string.Format("{0:X2} ", elm);
            Console.WriteLine(hex);
          }
          */

          string hex;
          if (RX_textBox.InvokeRequired)
          {

            RX_textBox.Invoke(new Action(delegate ()
            {
              RX_textBox.Text += DateTime.Now.ToString("[yyyy-MM-dd hh:mm:ss] [CONTROLLER -> PC] ");
              foreach (var elm in _packet.Buffer)
              {
                hex = string.Format("{0:X2} ", elm);
                RX_textBox.Text += hex;
              }
              RX_textBox.Text += "\r\n";
            }));
            // RX_textBox.Invoke(new MethodInvoker(delegate { RX_textBox.Text += insert_str; }));
          }
          else
          {
            hex = DateTime.Now.ToString("[yyyy-MM-dd hh:mm:ss] [CONTROLLER -> PC] ");
            foreach (var elm in _packet.Buffer)
            {
              RX_textBox.Text += string.Format("{0:X2} ", elm);
              RX_textBox.Text += hex;
            }
            RX_textBox.Text += "\r\n";
          }









        }


#if false   // 수신된 데이터를 콘솔에 출력해 봄
        _packet.Buffer.Add(get_data);
        string hex;
        foreach (var elm in _packet.Buffer)
        {
          hex = string.Format("{0:X2} ", elm);
          Console.WriteLine(hex);
        }
#endif
      }

#if false  // millis 함수와  Delay 검증  오류는 +-10ms 차이가 발생 아래 테스트 결과 110 ~ 90 딜레이 발생, 10m 이하는 발생되지 않는 경우가 많음
      int pcurr_millis = UTL.millis();
      Delay(100);
      int curr_millis = UTL.millis();
      int cal_curr_millis = curr_millis - pcurr_millis;
      Console.WriteLine(cal_curr_millis.ToString());
      cal_curr_millis = 0;
#endif


#if false  // 수신된 데이터를 1 byte씩 텍스트 박스 표시해 봄
      byte get_data = 0;
      string insert_str = "";
      const int def_8byte_packet_length = 24;
      const int def_include_return_8byte_packet_length = 26;

      if (_qReceiveBuffer.Available() > 0)
      {
        _qReceiveBuffer.Get(ref get_data);
        

        if (RX_textBox.InvokeRequired)
        {
          RX_textBox.Invoke(new Action(delegate ()
          {

            if (RX_textBox.TextLength == def_8byte_packet_length) //처음 8 바이트
              RX_textBox.Text += "\r\n";

            int last_index = RX_textBox.Text.LastIndexOf("\r\n");
            bool is_8byte = (RX_textBox.TextLength - last_index) == def_include_return_8byte_packet_length;
            if (is_8byte)
            {
              RX_textBox.Text += "\r\n";
            }

            insert_str = string.Format("{0:X2} ", get_data);
            RX_textBox.Text += insert_str;
          }));           
         // RX_textBox.Invoke(new MethodInvoker(delegate { RX_textBox.Text += insert_str; }));
        }
        else
        {
          if (RX_textBox.TextLength == def_8byte_packet_length) //처음 8 바이트
            RX_textBox.Text += "\r\n";

          int last_index = RX_textBox.Text.LastIndexOf("\r\n");
          bool is_8byte = (RX_textBox.TextLength - last_index) == def_include_return_8byte_packet_length;
          if (is_8byte)
          {
            RX_textBox.Text += "\r\n";
          }

          insert_str = string.Format("{0:X2} ", get_data);
          RX_textBox.Text += insert_str;
        } 
      }
#endif
    }


    /// <summary>
    /// 수신된 데이터를 파싱해 분석하는 함수
    /// </summary>
    /// <param name="data"></param>
    /// <returns></returns>
    private bool recivePacket(byte rx_data)
    {
      /*

    | SOF  | Type |funcId| Data Length |Data          |   Checksum   | EOF  |
    | :--- |:-----|:---- | :---------- |:-------------|:-------------| :--  |
    | 0x4A |1 byte|1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

     */


      const byte CMD_STX = 0x4A;
      const byte CMD_ETX = 0x4C;


      const byte STATE_WAIT_STX = 0;
      const byte STATE_WAIT_TYPE = 1;
      const byte STATE_WAIT_ID = 2;
      const byte STATE_WAIT_LENGTH_L = 4;
      const byte STATE_WAIT_LENGTH_H = 5;
      const byte STATE_WAIT_DATA = 6;
      const byte STATE_WAIT_CHECKSUM_L = 7;
      const byte STATE_WAIT_CHECKSUM_H = 8;
      const byte STATE_WAIT_ETX = 9;

      switch (_packet.State.GetStep())
      {
        case STATE_WAIT_STX:
          if (rx_data == CMD_STX)
          {
            _packet.Buffer.Clear();
            _packet.Checksum = 0xffff;

            _packet.Buffer.Add(rx_data);
            _packet.State.SetStep(STATE_WAIT_TYPE);
          }
          break;

        case STATE_WAIT_TYPE:
          _packet.Type = rx_data;
          _packet.Buffer.Add(rx_data);
          UTL.crc16_update(ref _packet.Checksum, rx_data);
          _packet.State.SetStep(STATE_WAIT_ID);
          break;

        case STATE_WAIT_ID:
          _packet.ObjID = rx_data;
          _packet.Buffer.Add(rx_data);
          UTL.crc16_update(ref _packet.Checksum, rx_data);
          _packet.State.SetStep(STATE_WAIT_LENGTH_L);
          break;

        case STATE_WAIT_LENGTH_L:
          _packet.Length = rx_data;
          _packet.Buffer.Add(rx_data);
          UTL.crc16_update(ref _packet.Checksum, rx_data);
          _packet.State.SetStep(STATE_WAIT_LENGTH_H);
          break;

        case STATE_WAIT_LENGTH_H:
          _packet.Length |= (ushort)(rx_data << 8);
          _packet.Buffer.Add(rx_data);
          UTL.crc16_update(ref _packet.Checksum, rx_data);
          if (_packet.Length != 0)
          {
            _packet.Data.Clear();
            _packet.State.SetStep(STATE_WAIT_DATA);
          }
          else
          {
            _packet.State.SetStep(STATE_WAIT_STX);
          }
          break;

        case STATE_WAIT_DATA:
          _packet.Buffer.Add(rx_data);
          UTL.crc16_update(ref _packet.Checksum, rx_data);
          _packet.Data.Add(rx_data);

          // check receive completed
          if (_packet.Length == _packet.Data.Count())
          {
            _packet.State.SetStep(STATE_WAIT_CHECKSUM_L);
          }

          break;

        case STATE_WAIT_CHECKSUM_L:
          _packet.Buffer.Add(rx_data);
          _packet.Checksum_recv = rx_data;
          _packet.State.SetStep(STATE_WAIT_CHECKSUM_H);
          break;

        case STATE_WAIT_CHECKSUM_H:
          _packet.Buffer.Add(rx_data);
          _packet.Checksum_recv |= (ushort)(rx_data << 8);
          _packet.State.SetStep(STATE_WAIT_ETX);
          break;

        case STATE_WAIT_ETX:
          if (rx_data == CMD_ETX)
          {
            _packet.Buffer.Add(rx_data);
            if (_packet.Checksum == _packet.Checksum_recv)
            {
              return true;
            }
          }
          _packet.State.SetStep(STATE_WAIT_STX);
          break;

      }
      //switch (m_packet.state)



      return false;
    }

#endif



  }
  // end of public partial class MainForm : Form
}
// end of namespace VPRemote
