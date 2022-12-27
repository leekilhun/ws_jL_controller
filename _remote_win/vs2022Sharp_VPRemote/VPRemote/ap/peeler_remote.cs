using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;

namespace VPRemote.ap
{

  /*

	mcu - server, pc - client

	TX (pc -> mcu) request information or action
	obj_id [option] 0 is all or ignore
	| SOF  | cmd_Type | obj_Id | Data Length |Data          |   Checksum   | EOF  |
	| :--- |:---------|:-------| :---------- |:-------------|:-------------| :--  |
	| 0x4A |  1 byte  | 1 byte | 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

	RX  (mcu -> pc) provide information
	obj_id [option] 0 is all or ignore
	| SOF  | rx_Type  | obj_Id | Data Length |Data          |   Checksum   | EOF  |
	| :--- |:---------|:-------| :---------- |:-------------|:-------------| :--  |
	| 0x4A |  1 byte  | 1 byte | 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

	 */


  internal class Peeler_remote
  {
    enum STEP
    {
      STEP_INIT,
      STEP_TODO,
      STEP_TIMEOUT,
      STEP_WAIT_RETURN,
      STEP_REQUEST_MCU_DATE,
      STEP_REQUEST_MCU_DATE_START,
      STEP_REQUEST_MCU_DATE_WAIT,
      STEP_REQUEST_MCU_DATE_END,
    }


    const byte MODULE_PEELER_CMD_STX = 0x4A;
    const byte MODULE_PEELER_CMD_ETX = 0x4C;

    public enum RX_TYPE
    {
      RX_STATE_DATA = 0x00,
      RX_MOTOR_DATA = 0x01,
      RX_MOTOR_POS_VEL = 0x02,
    }


    public enum CMD_TYPE : byte
    {
      CMD_READ_ALL_STATE = 0x00,
      CMD_READ_BOOT_INFO = 0x01,
      CMD_READ_FIRM_INFO = 0x02,

      CMD_CTRL_IO_OUT = 0x10,
      CMD_CTRL_CYL = 0x11,
      CMD_CTRL_VAC = 0x12,
      CMD_CTRL_JOB_INITIAL = 0x1A,
      CMD_CTRL_VIRTUAL_SW = 0x1B,

      CMD_CTRL_MOT_ORIGIN = 0x20,
      CMD_CTRL_MOT_ONOFF = 0x21,
      CMD_CTRL_MOT_RUN = 0x22,
      CMD_CTRL_MOT_STOP = 0x23,
      CMD_CTRL_MOT_JOG = 0x24,
      CMD_CTRL_MOT_LIMIT = 0x25,
      CMD_CTRL_MOT_ZEROSET = 0x26,
      CMD_CTRL_MOT_RELMOVE = 0x27,
      CMD_CTRL_MOT_CLEAR_ALARM = 0x28,
      CMD_CTRL_MOT_CHANGE_VEL = 0x29,
      CMD_CTRL_MOTS_ONOFF = 0x2A,
      CMD_CTRL_MOTS_RUN = 0x2B,
      CMD_CTRL_MOTS_STOP = 0x2C,
      CMD_CTRL_MOTS_REL = 0x2D,

      CMD_WRITE_MOTOR_POS_DATA = 0x40,
      CMD_WRITE_CFG_DATA = 0x41,
      CMD_WRITE_CYL_DATA = 0x42,
      CMD_WRITE_VAC_DATA = 0x43,
      CMD_WRITE_SEQ_DATA = 0x44,
      CMD_CLEAR_ROM_DATA = 0x45,

      CMD_READ_MCU_DATA = 0x50,
      CMD_RELOAD_ROM_DATA = 0x51,

      CMD_OK_RESPONSE = 0xAA,
    };


    public struct MCU_st
    {
      public ushort mcu_state_reg;
      public ushort mcu_option_reg;
      public uint mcu_error_reg;
      public ushort mcu_input_reg;
      public ushort mcu_output_reg;
      public ushort motor_drv_status;
      public ushort motor_alarm_status;
      public ushort motor_encoder_pos;
      public ushort motor_imm_abs_position;
      public ushort motor_abs_position;
      public ushort motor_imm_target_velocity;
      public ushort motor_imm_act_velocit;
    }


#if false

      memcpy(&m_txBuffer[0],&m_cfg.ptr_mcu_reg->state_reg.ap_state,2);
			memcpy(&m_txBuffer[2],&m_cfg.ptr_mcu_reg->option_reg.ap_option,2);
			memcpy(&m_txBuffer[4],&m_cfg.ptr_mcu_reg->error_reg.ap_error,4);
			memcpy(&m_txBuffer[8],&m_cfg.ptr_io->m_in.data, 4);
			memcpy(&m_txBuffer[12],&m_cfg.ptr_io->m_out.data, 4);

			memcpy(&m_txBuffer[16],&motor_data.drv_status.sc_status, 2);
			memcpy(&m_txBuffer[18],&motor_data.al_code.al_status, 2);
			memcpy(&m_txBuffer[20],&motor_data.encoder_position, 4 );				// 엔코더 위치값
			memcpy(&m_txBuffer[24],&motor_data.immediate_abs_position, 4 ); // 목표 위치값
			memcpy(&m_txBuffer[28],&motor_data.abs_position_command, 4 );   // 현재
			memcpy(&m_txBuffer[32],&motor_data.immediate_target_velocity, 4 ); // 타켓 속도
			memcpy(&m_txBuffer[34],&motor_data.immediate_act_velocity, 4 );	   // 현재 속도

#endif
    private Thread _thread = null;
    private bool _IsThreadLife = false;

    //private System.Timers.Timer _tick_timer;//틱 타이머
    private QBuffer<byte> _qReceiveBuffer = new QBuffer<byte>(4096);
    private PacketData _packet = new PacketData();

    private MCU_st _mcu_receive_data = new MCU_st();

    private Action<string> callback;
    private Action<byte[], int, int> commWrite;
    uint _refresh_ms = 0;


    private StateMachine _step = new StateMachine();

    public Peeler_remote()
    {
      byte[] sc = null;
      //SetTimer(); -> tick에서 thread로 변경
    }

    ~Peeler_remote()
    {
      if (_thread != null)
        threadStop();
    }

    public void ThreadRun()
    {
      _thread = new Thread(threadFunc);
      _step.SetStep(0);
      _IsThreadLife = true;
      _thread.Start();
    }
    public void ThreadStop()
    {
      threadStop();
    }

    private void threadStop()
    {
      if (_thread == null)
        return;

      _IsThreadLife = false;
      if (_thread.IsAlive)   // Thread가 동작 중 일 경우 
      {
        _IsThreadLife = false;
        //_thread.Abort();  // Thred를 강제 종료
      }
      _thread = null;
    }

    private void threadJob()
    {
      doRunStep();

      if (recivePacket())
      {

        Ret_OkResponse();

        string hex = "";
        foreach (var elm in _packet.Buffer)
        {
          hex += string.Format("{0:X2} ", elm);
        }
        _refresh_ms = (uint)UTL.millis() - _refresh_ms;
        hex += "response ms : " + _refresh_ms.ToString();
        _refresh_ms = (uint)UTL.millis();
        Console.WriteLine(hex);

        if (callback != null)
          callback.Invoke(hex);

        ProcessCmd();
      }
    }


    public void threadFunc()
    {
      try
      {
        // DisplayMessage("IO Thread가 시작하였습니다.");
        while (_IsThreadLife)
        {
          threadJob();
          Thread.Sleep(10);
        }

      }

      catch (ThreadInterruptedException exInterrupt)
      {
        Trace.WriteLine(exInterrupt.Message);
      }
      catch (Exception ex)
      {
        //eventLog(ex.Message);
        Trace.WriteLine(ex.Message);
      }

    }

    private void doRunStep()
    {

      switch ((STEP)_step.GetStep())
      {
        case STEP.STEP_INIT:
          {
            _step.SetStep((byte)STEP.STEP_TODO);
          }
          break;

        case STEP.STEP_TODO:
          {
            _step.SetStep((byte)STEP.STEP_REQUEST_MCU_DATE);
          }
          break;

        case STEP.STEP_TIMEOUT:
          _step.SetStep((byte)STEP.STEP_TODO);
          break;

        case STEP.STEP_WAIT_RETURN:
          break;

        case STEP.STEP_REQUEST_MCU_DATE:
          _step.SetStep((byte)STEP.STEP_REQUEST_MCU_DATE_START);
          break;

        case STEP.STEP_REQUEST_MCU_DATE_START:
          _step.SetStep((byte)STEP.STEP_REQUEST_MCU_DATE_WAIT);
          _packet.OkResponse = false;
          requestMcuState();
          break;

        case STEP.STEP_REQUEST_MCU_DATE_WAIT:
          if (_step.MoreThan(100))
          {
            _step.SetStep((byte)STEP.STEP_TIMEOUT);
            break;
          }
          if (_packet.OkResponse == false)
            break;

          _step.SetStep((byte)STEP.STEP_REQUEST_MCU_DATE_END);
          break;

        case STEP.STEP_REQUEST_MCU_DATE_END:
          _step.SetStep((byte)STEP.STEP_TODO);
          break;

        default:
          break;
      }
      //end of switch
    }


    //public void StartTick()
    //{
    //  _tick_timer.Start();
    //}

    public void PutReceive_1ByteData(byte data)
    {
      _qReceiveBuffer.Put(data);
    }


    public void AddCallbackFunc(Action<string> callback)
    {
      this.callback = callback;
    }

    public void AddCommWriteFunc(Action<byte[], int, int> func)
    {
      this.commWrite = func;
    }

#if false

    private void SetTimer() // 타이머 셋팅 
    {
      // tick timer (주기적으로 실행할 함수를 호출할 타이머)
      _tick_timer = new System.Timers.Timer();
      _tick_timer.Interval = 1; // 10ms 주기
      _tick_timer.Elapsed += new System.Timers.ElapsedEventHandler(SerialPortTick);

    }

    private void SerialPortTick(object sender, System.Timers.ElapsedEventArgs e)
    {
      if (recivePacket())
      {
        string hex = "";
        foreach (var elm in _packet.Buffer)
        {
          hex += string.Format("{0:X2} ", elm);
        }
        _refresh_ms = (uint)UTL.millis() - _refresh_ms;
        hex += "response ms : " + _refresh_ms.ToString();
        _refresh_ms = (uint)UTL.millis();
        Console.WriteLine(hex);

        if (callback != null)
          callback.Invoke(hex);

        ProcessCmd();
      }


#if false
      byte get_data = 0;

      if (_qReceiveBuffer.Available() > 0)
      {
        _qReceiveBuffer.Get(ref get_data);

        if ((UTL.millis() - _packet.State.GetPrevTime()) > 1000)
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

          ProcessCmd();
        }
      }
#endif
    }

#endif

    private void ProcessCmd()
    {
      PacketData receive_data = _packet;
      RX_TYPE type = (RX_TYPE)receive_data.RXType;

      switch (type)
      {
        case RX_TYPE.RX_STATE_DATA:
          {
            int idx = 0;
            byte[] datas = receive_data.Data.ToArray();
            _mcu_receive_data.mcu_state_reg = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.mcu_state_reg);
            _mcu_receive_data.mcu_option_reg = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.mcu_option_reg);
            _mcu_receive_data.mcu_error_reg = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.mcu_error_reg);
            _mcu_receive_data.mcu_input_reg = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.mcu_input_reg);
            _mcu_receive_data.mcu_output_reg = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.mcu_output_reg);

            /*
            _mcu_receive_data.motor_drv_status = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.motor_drv_status);
            _mcu_receive_data.motor_alarm_status = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.motor_alarm_status);
            _mcu_receive_data.motor_encoder_pos = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.motor_encoder_pos);
            _mcu_receive_data.motor_imm_abs_position = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.motor_imm_abs_position);
            _mcu_receive_data.motor_abs_position = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.motor_abs_position);
            _mcu_receive_data.motor_imm_target_velocity = BitConverter.ToUInt16(datas, idx);
            idx += Marshal.SizeOf(_mcu_receive_data.motor_imm_target_velocity);
            _mcu_receive_data.motor_imm_act_velocit = BitConverter.ToUInt16(datas, idx);
            */
          }
          break;

        case RX_TYPE.RX_MOTOR_DATA:
          break;

        case RX_TYPE.RX_MOTOR_POS_VEL:
          break;


        default:
          break;
      }
    }






    public int SendCmd(List<byte> datas)
    {
      List<byte> packets = new List<byte>();
      //std::vector<uint8_t> datas{ };
      /*

			 | SOF  | Type |funcId| Data Length |Data          |   Checksum   | EOF  |
			 | :--- |:-----|:---- | :---------- |:-------------|:-------------| :--  |
			 | 0x4A |1 byte|1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |
					|-> crc                                            crc <- |
			*/
      ushort crc = 0xffff;
      packets.Add(MODULE_PEELER_CMD_STX);
      foreach (var item in datas)
      {
        packets.Add(item);
        UTL.crc16_update(ref crc, item);
      }
      packets.Add((byte)(crc >> 0));
      packets.Add((byte)(crc >> 8));
      packets.Add((byte)(MODULE_PEELER_CMD_ETX));

      byte[] sc = packets.ToArray();
      if (commWrite != null)
      {
        commWrite.Invoke(sc, 0, packets.Count);
      }

      return 0;
    }

    //inline bool SendCmdRxResp(uint8_t* p_data, uint32_t length, uint32_t timeout)
    //{

    //  return false;
    //}


    public int Ret_OkResponse()
    {
      _packet.OkResponse = true;
      List<byte> datas = new List<byte>();
      datas.Add((byte)CMD_TYPE.CMD_OK_RESPONSE);
      datas.Add(0x00);
      datas.Add(0x00);
      datas.Add(0x00);

      return SendCmd(datas);
    }



    private int requestMcuState()
    {
      List<byte> datas = new List<byte>();
      datas.Add((byte)CMD_TYPE.CMD_READ_MCU_DATA);
      datas.Add(0x00);
      datas.Add(0x00);
      datas.Add(0x00);

      return SendCmd(datas);
    }





    /// <summary>
    /// 수신된 데이터를 파싱해 분석하는 함수
    /// </summary>
    /// <returns></returns>
    private bool recivePacket()
    {
      /*

			 | SOF  | rx_Type | obj_Id| Data Length |Data          |   Checksum   | EOF  |
			 | :--- |:--------|:----- | :---------- |:-------------|:-------------| :--  |
			 | 0x4A | 1 byte  | 1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

			*/


      const byte CMD_STX = 0x4A;
      const byte CMD_ETX = 0x4C;


      const byte STATE_WAIT_STX = 0;
      const byte STATE_WAIT_TYPE = 1;
      const byte STATE_WAIT_ID = 2;
      const byte STATE_WAIT_LENGTH_L = 3;
      const byte STATE_WAIT_LENGTH_H = 4;
      const byte STATE_WAIT_DATA = 5;
      const byte STATE_WAIT_CHECKSUM_L = 6;
      const byte STATE_WAIT_CHECKSUM_H = 7;
      const byte STATE_WAIT_ETX = 8;

      List<byte> datas = new List<byte>();
      byte get_data = 0;
      while (_qReceiveBuffer.Available() > 0)
      {
        _qReceiveBuffer.Get(ref get_data);
        datas.Add(get_data);
      }

      if (datas.Count < 1)
        return false;
      /*
      string hex = "";
      foreach (var elm in datas)
      {
        hex += string.Format("{0:X2} ", elm);
      }
      Console.WriteLine(hex);
      */
      if ((UTL.millis() - _packet.State.GetPrevTime()) > 100)
      {
        _packet.Buffer.Clear();
        _packet.Checksum = 0xffff;
        _packet.State.SetStep(0);
      }


      foreach (var rx_data in datas)
      {
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
            _packet.RXType = rx_data;
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
            if (_packet.Length == (ushort)_packet.Data.Count)
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
            _packet.Buffer.Add(rx_data);
            if (rx_data == CMD_ETX)
            {
              if (_packet.Checksum == _packet.Checksum_recv)
              {
                return true;
              }
            }
            _packet.State.SetStep(STATE_WAIT_STX);
            break;

        }
        //switch (m_packet.state)
      }




      return false;
    }



  }
  // end of class

}

//end of namespace VPRemote.ap
