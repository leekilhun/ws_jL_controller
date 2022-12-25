using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace VPRemote.ap
{
  internal class Peeler_remote
  {
    public enum RX_TYPE
    {
      RX_STATE_DATA = 0x00,
      RX_MOTOR_DATA = 0x01,
      RX_MOTOR_POS_VEL = 0x02,
    }

    public enum CMD_TYPE
    {
      FIRM_CTRL = 0x00,
      CONTROL_MOT = 0x10,
      CONTROL_CYL = 0x11,
      CONTROL_VAC = 0x12,
      EEROM_CTRL = 0x13,
    };

    public enum CMD_ID
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

    private System.Timers.Timer _tick_timer;//틱 타이머
    private QBuffer<byte> _qReceiveBuffer = new QBuffer<byte>(4096);
    private PacketData _packet = new PacketData();

    private MCU_st _mcu_receive_data = new MCU_st();

    private Action<string> callback;
    uint _refresh_ms = 0;


    public Peeler_remote()
    {
      SetTimer();
    }

    ~Peeler_remote()
    {

    }

    public void StartTick()
    {
      _tick_timer.Start();
    }

    public void PutReceive_1ByteData(byte data)
    {
      _qReceiveBuffer.Put(data);
    }


    public void AddCallbackFunc(Action<string> callback)
    {
      this.callback = callback;
    }

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


    private void ProcessCmd()
    {
      PacketData receive_data = _packet;
      RX_TYPE type = (RX_TYPE)receive_data.Type;

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



    /// <summary>
    /// 수신된 데이터를 파싱해 분석하는 함수
    /// </summary>
    /// <param name="data"></param>
    /// <returns></returns>
    private bool recivePacket()
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

      List<byte> datas = new List<byte>();
      byte get_data = 0;
      while (_qReceiveBuffer.Available() > 0)
      {
        _qReceiveBuffer.Get(ref get_data);
        datas.Add(get_data);
      }
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
