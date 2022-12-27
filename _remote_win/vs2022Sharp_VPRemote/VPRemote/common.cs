using System;
using System.Collections.Generic;
using System.Linq;

/*
 *  common.cs
 *
 *  Created on: 2022. 12. 22.
 *      Author: gns2.lee@samsung.com
 * 
 *  네임스페이스 VPRemote 전역으로 접근 사용할 공통적인 함수, 클래스를 만듬
 */



namespace VPRemote
{

  public static class UTL
  {
    public static Int32 millis()
    {
      return (Environment.TickCount & Int32.MaxValue);
    }

    public static void crc16_update(ref ushort crc_in, byte data)
    {
      const ushort poly = 0xa001;
      ushort crc = crc_in;
      byte i;
      /* Exclusive-OR the byte with the CRC */
      crc ^= data; //*(pDataBuffer + iByte);

      /* Loop through all 8 data bits */
      for (i = 0; i <= 7; i++)
      {
        /* If the LSB is 1, shift the CRC and XOR the polynomial mask with the CRC */
        // Note - the bit test is performed before the rotation, so can't move the << here
        if (((crc & 0x0001) == 1))
        {
          crc >>= 1;
          crc ^= poly;
        }
        else
        {
          // Just rotate it
          crc >>= 1;
        }
      }
      crc_in = crc;
    }

    public static bool IsUppercase(string s)
    {
      return s.All(char.IsUpper);
    }

    public static bool IsLowercase(string s)
    {
      return s.All(char.IsLower);
    }

    public static string NormalizeString(string input)
    {
      return input.ToLower().Trim().Replace(",", "");
    }

  }


  internal class QBuffer<Typename>
  {

    private Typename[] _buffers;
    private uint _head;
    private uint _tail;
    private uint _length;


    public QBuffer(uint size = 4096)
    {
      this._head = 0;
      this._tail = 0;
      this._length = size;
      _buffers = new Typename[size];

    }

    public bool Put(Typename value)
    {
      bool ret = true;
      uint next_in;
      next_in = (_head + 1) % _length;
      if (next_in != _tail)
      {
        _buffers[_head % _length] = value;
        _head = next_in;
      }
      else
        ret = false;

      return ret;
    }

    public bool Get(ref Typename ref_data)
    {
      bool ret = true;
      ref_data = _buffers[_tail];
      if (_tail != _head)
      {
        _tail = (_tail + 1) % _length;
      }
      else
        ret = false;

      return ret;
    }



    public uint Available()
    {
      return (_length + (_head - _tail)) % _length;
    }

  }

  internal class StateMachine
  {

    private byte _state;
    private int _prev_ms;
    private int _resp_ms;

    public byte GetStep()
    {
      return _state;
    }

    public int GetRespTime()
    {
      return _resp_ms;
    }

    public int GetPrevTime()
    {
      return _prev_ms;
    }

    public void SetStep(byte state)
    {
      _resp_ms = UTL.millis() - _prev_ms;
      _prev_ms = UTL.millis();
      _state = state;
    }

    public bool LessThan(int msec)
    {
      if ((UTL.millis() - _prev_ms) < msec)
        return true;
      return false;
    }


    public bool MoreThan(int msec)
    {
      return !LessThan(msec);
    }

    /*    struct rx_packet_st
        {
          public byte type;
          public byte func_id;
          public ushort length;
          public ushort check_sum;
          public ushort check_sum_recv;
          public List<byte> buffer;

          public rx_packet_st()
          {
            type = 0;
            func_id = 0;
            length = 0;
            check_sum = 0;
            check_sum_recv = 0;
            buffer = new List<byte>();
          }
        }


        struct packet_st
        {
          public byte state;
          public uint prev_ms;
          public uint resp_ms;
          public uint index;
          public uint data_cnt;
          public byte error;
          public rx_packet_st rx_packet;

        }*/
  }


  class PacketData
  {
    public byte RXType;
    public byte ObjID;
    public ushort Length;
    public ushort Checksum;
    public ushort Checksum_recv;
    public bool OkResponse;
    public List<byte> Buffer = new List<byte>();
    public List<byte> Data = new List<byte>();
    public StateMachine State = new StateMachine();

  }

}
