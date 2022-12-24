using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;
using System.Xml.Linq;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TrackBar;

namespace VPRemote
{
    public partial class MainForm : Form
    {
        public enum CMDTYPE
        {
            FIRM_CTRL = 0x00,
            CONTROL_MOT = 0x10,
            CONTROL_CYL = 0x11,
            CONTROL_VAC = 0x12,
            EEROM_CTRL = 0x13,
        };

        public enum CMDID
        {
            CMD_READ_ALL_STATE = 0x00,
            CMD_READ_BOOT_INFO = 0x01,
            CMD_READ_FIRM_INFO = 0x02,
            CMD_CONTROL_IO_OUT = 0x10,
            CMD_CONTROL_MOT_ORIGIN = 0x11,
            CMD_CONTROL_MOT_ONOFF = 0x12,
            CMD_CONTROL_MOT_RUN = 0x13,
            CMD_CONTROL_MOT_STOP = 0x14,
            CMD_CONTROL_MOT_JOG = 0x15,
            CMD_CONTROL_MOT_LIMIT = 0x16,
            CMD_MOT_ENCODER_ZEROSET = 0x17,
            CMD_CONTROL_CYL = 0x18,
            CMD_CONTROL_VAC = 0x19,
            CMD_CONTROL_MOT_RELMOVE = 0x1A,
            CMD_CONTROL_MOT_CLEAR_ALARM = 0x1B,
            CMD_CONTROL_JOB_INITIAL = 0x1C,
            CMD_CONTROL_MOT_CHANGE_VEL = 0x1D,
            CMD_CONTROL_MOTS_ONOFF = 0x60,
            CMD_CONTROL_MOTS_RUN = 0x61,
            CMD_CONTROL_MOTS_STOP = 0x62,
            CMD_CONTROL_MOTS_REL = 0x63,
            CMD_AP_CONFIG_WRITE = 0x20,
            CMD_READ_MCU_DATA = 0x21,
            CMD_RELOAD_ROM_DATA = 0x22,
            CMD_CLEAR_ROM_DATA = 0x23,
            CMD_WRITE_MOTOR_POS_DATA = 0x30,
            CMD_WRITE_AP_DATA = 0x31,
            CMD_WRITE_CYL_DATA = 0x32,
            CMD_WRITE_VAC_DATA = 0x33,
            CMD_WRITE_SEQ_DATA = 0x34,
            CMD_WRITE_LINK_DATA = 0x35,
            CMD_EVENT_MCU_VIRTUAL_SW = 0x40,
            CMD_READ_LOG_FRONT = 0x50,
            CMD_READ_LOG_REAR = 0x51,
            CMD_TEST_STRESS = 0x52,
            CMD_START_SEND_MCU_STATE = 0xB0,
            CMD_STOP_SEND_MCU_STATE = 0xB1,
            CMD_MOTOR_PARM_SET = 0xAC,
            CMD_MOTOR_PARM_GET = 0xAE,

            CMD_OK_RESPONSE = 0xAA,
            CMD_TIMEOUT_RESPONSE = 0xAB,
        };



        private static System.Timers.Timer _sysTimer;// 날짜 표시
        private delegate void delUpdateTime(); // 날짜 시간 업데이트
        private bool _enable_Timer; // 타이머 제어 변수


        private System.Timers.Timer _tick_timer;//틱 타이머
        private QBuffer<byte> _qReceiveBuffer = new QBuffer<byte>(4096);
        private PacketData _packet = new PacketData();




        public MainForm() //  MainForm이 생성하면서 호출되는 생성자 함수, 내부 내용이 순차적으로 실행됨
        {
            InitializeComponent();

            _enable_Timer = false;

            SetTimer();
            SetCSerialComm();

        }


        private void SetTimer() // 타이머 셋팅 
        {
            // Create a timer with a two second interval.
            _sysTimer = new System.Timers.Timer(1000);
            // Hook up the Elapsed event for the timer. 
            _sysTimer.Elapsed += OnTimedEvent;
            _sysTimer.AutoReset = true;
            _sysTimer.Enabled = true;


            // tick timer (주기적으로 실행할 함수를 호출할 타이머)
            _tick_timer = new System.Timers.Timer();
            _tick_timer.Interval = 1; // 50ms 주기
            _tick_timer.Elapsed += new System.Timers.ElapsedEventHandler(SerialPortTick);

        }


        private bool SetCSerialComm() // 시리얼 포트 오픈
        {
            if (serialPort_peeler.IsOpen == false)
            {
                try
                {
                    serialPort_peeler.PortName = "COM5";
                    serialPort_peeler.Open();

                    _tick_timer.Start();
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


        private void SerialPort_peeler_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            byte[] temp = new byte[serialPort_peeler.BytesToRead];
            serialPort_peeler.Read(temp, 0, temp.Length);

            foreach (var item in temp)
            {
                _qReceiveBuffer.Put(item);
            }
        }
    }
    // end of public partial class MainForm : Form
}
// end of namespace VPRemote
