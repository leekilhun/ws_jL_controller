/*
 * uart_api.hpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_UART_REMOTE_HPP_
#define SRC_AP__INC_UART_REMOTE_HPP_

/*

| SOF  | Type |funcId| Data Length |Data          |   Checksum   | EOF  |
| :--- |:-----|:---- | :---------- |:-------------|:-------------| :--  |
| 0x4A |1 byte|1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

 */

namespace RCTRL
{

	enum CMDTYPE
	{
		FIRM_CTRL       = 0x00,
		CONTROL_MOT     = 0x10,
		CONTROL_CYL     = 0x11,
		CONTROL_VAC     = 0x12,
		EEROM_CTRL      = 0x13,

	};

	enum RXCMD
	{
		CMD_READ_ALL_STATE          = 0x00,
		CMD_READ_BOOT_INFO          = 0x01,
		CMD_READ_FIRM_INFO          = 0x02,
		CMD_CONTROL_IO_OUT          = 0x10,
		CMD_CONTROL_MOT_ORIGIN      = 0x11,
		CMD_CONTROL_MOT_ONOFF       = 0x12,
		CMD_CONTROL_MOT_RUN         = 0x13,
		CMD_CONTROL_MOT_STOP        = 0x14,
		CMD_CONTROL_MOT_JOG         = 0x15,
		CMD_CONTROL_MOT_LIMIT       = 0x16,
		CMD_MOT_ENCODER_ZEROSET     = 0x17,
		CMD_CONTROL_CYL             = 0x18,
		CMD_CONTROL_VAC             = 0x19,
		CMD_CONTROL_MOT_RELMOVE     = 0x1A,
		CMD_CONTROL_MOT_CLEAR_ALARM = 0x1B,
		CMD_CONTROL_JOB_INITIAL     = 0x1C,
		CMD_CONTROL_MOT_CHANGE_VEL  = 0x1D,
		CMD_CONTROL_MOTS_ONOFF      = 0x60,
		CMD_CONTROL_MOTS_RUN        = 0x61,
		CMD_CONTROL_MOTS_STOP       = 0x62,
		CMD_CONTROL_MOTS_REL        = 0x63,
		CMD_AP_CONFIG_WRITE         = 0x20,
		CMD_READ_MCU_DATA           = 0x21,
		CMD_RELOAD_ROM_DATA         = 0x22,
		CMD_CLEAR_ROM_DATA          = 0x23,
		CMD_WRITE_MOTOR_POS_DATA    = 0x30,
		CMD_WRITE_AP_DATA           = 0x31,
		CMD_WRITE_CYL_DATA          = 0x32,
		CMD_WRITE_VAC_DATA          = 0x33,
		CMD_WRITE_SEQ_DATA          = 0x34,
		CMD_WRITE_LINK_DATA         = 0x35,
		CMD_EVENT_MCU_VIRTUAL_SW    = 0x40,
		CMD_READ_LOG_FRONT          = 0x50,
		CMD_READ_LOG_REAR           = 0x51,
		CMD_TEST_STRESS             = 0x52,
		CMD_START_SEND_MCU_STATE    = 0xB0,
		CMD_STOP_SEND_MCU_STATE     = 0xB1,
		CMD_MOTOR_PARM_SET          = 0xAC,
		CMD_MOTOR_PARM_GET          = 0xAE,

		CMD_OK_RESPONSE             = 0xAA,
		CMD_TIMEOUT_RESPONSE        = 0xAB,
	};


	enum ERRNO
	{
		WRONG_CMD               = 0x01,
		IO_OUT                  = 0x02,
		MOT_ORIGIN              = 0x03,
		BUF_OVF                 = 0x06,
		INVALID_FW              = 0x07,
		FW_CRC                  = 0x08
	};

	constexpr uint8_t CMD_STX 					= 0x4A;
	constexpr uint8_t CMD_ETX 					= 0x4C;

	constexpr int CMD_MAX_DATA_LENGTH 	= 24;
	constexpr int CMD_MAX_PACKET_LENGTH =(CMD_MAX_DATA_LENGTH + 8);
	constexpr int PACKET_BUFF_LENGTH 		= CMD_MAX_PACKET_LENGTH;

#ifdef _USE_HW_RTOS
#define AP_UART_RCTL_LOCK_BEGIN osMutexWait(m_mutex_id, osWaitForever)
#define AP_UART_RCTL_LOCK_END   osMutexRelease(m_mutex_id)
#else
#define AP_UART_RCTL_LOCK_BEGIN
#define AP_UART_RCTL_LOCK_END
#endif




	class uart_remote
	{
		/****************************************************
		 *  data
		 ****************************************************/
	public:
		struct cfg_t
		{
			uint8_t ch{}; uint32_t baud{};
		};


		struct rx_packet_t
		{
			uint8_t         type{};
			uint8_t         func_id{};
			uint16_t        length{};
			uint16_t        check_sum{};
			uint16_t        check_sum_recv{};
			std::array <uint8_t, CMD_MAX_PACKET_LENGTH> buffer{};
			uint8_t   *data;

			rx_packet_t() = default;

			rx_packet_t& operator = (const rx_packet_t& dat){
				if ( this != &dat){
					type = dat.type;
					func_id = dat.func_id;
					length = dat.length;
					data = dat.data;
					check_sum = dat.check_sum;
					check_sum_recv = dat.check_sum_recv;
					buffer = dat.buffer;
				}
				return *this;
			}

			rx_packet_t& operator = (const rx_packet_t* dat){
				if ( this != dat){
					type = dat->type;
					func_id = dat->func_id;
					length = dat->length;
					data = dat->data;
					check_sum = dat->check_sum;
					check_sum_recv = dat->check_sum_recv;
					buffer = dat->buffer;
				}
				return *this;
			}
		} ;




	private:

		struct packet_t {
			uint8_t   state{};
			//uint8_t   wait_flag{};
			uint32_t  prev_ms{};
			uint32_t  resp_ms{};
			uint32_t  index{};
			uint32_t  data_cnt{};
			uint8_t   error{};
			rx_packet_t  rx_packet{};
		};

		bool m_Isconnected;
		cfg_t m_cfg;
		packet_t m_packet;

		void* m_obj;
		evt_cb m_func;
		uint32_t m_packet_sending_ms;


#ifdef _USE_HW_RTOS
		osMutexId m_mutex_id;
#endif

		/****************************************************
		 *  Constructor
		 ****************************************************/
	public:
		uart_remote():m_Isconnected{}, m_cfg{}, m_packet{}, m_obj{}, m_func{}
		,m_packet_sending_ms{}
		{
#ifdef _USE_HW_RTOS
			osMutexDef(m_mutex_id);
			m_mutex_id = osMutexCreate (osMutex(m_mutex_id));
#endif

		}
		virtual ~uart_remote (){}

		/****************************************************
		 *  func
		 ****************************************************/

		inline void Init(cfg_t &cfg) {
			m_cfg = cfg;
			if (uartOpen(cfg.ch, cfg.baud))
			{
				m_packet.state = 0;
				m_Isconnected = true;
			}
		}


		inline bool Recovery() {
			uartClose(m_cfg.ch);
			/* */
			m_packet.state = 0;
			return uartOpen(m_cfg.ch, m_cfg.baud);
		}

		inline bool ReceiveProcess(){
			bool ret = false;
			if (receivePacket())
			{
				receiveCplt();
				ret = true;
			}
			return ret;
		}

		inline void AttCallbackFunc(void* obj, evt_cb cb)
		{
			m_obj = obj;
			m_func = cb;
		}



		bool SendCmd(uint8_t cmd, uint8_t *p_data, uint32_t length)
		{
			uint32_t value = 0;
			std::array<uint8_t, PACKET_BUFF_LENGTH> temp = { 0, };
			bool ret = false;

			if (length > 4)
			{
				value = utilDwToInt(p_data);
			}
			else if (length == 1)
			{
				value = (uint32_t)p_data[0];
			}

			int len = 0;

			//

			//
			if (ret)
			{
				value = 0;
				len = strnlen((const char*)temp.data(), PACKET_BUFF_LENGTH);
				temp[len++] = 0xff;
				temp[len++] = 0xff;
				temp[len++] = 0xff;
				m_packet_sending_ms = millis();

				value = uartWrite(m_cfg.ch, temp.data(), len);
				ret = (value > 0)?true:false;
			}
			return ret;
		}


		bool SendData(uint8_t cmd, uint8_t idx, uint8_t *p_data, uint8_t length)
		{
			bool ret = false;
			m_packet_sending_ms = millis();
			ret = uartWrite(m_cfg.ch, p_data, length);
			return (bool)ret;
		}


		bool SendCmdRxResp(uint8_t cmd, uint8_t *p_data, uint32_t length, uint32_t timeout = 100)
		{
			bool  ret = false;
			uint32_t time_pre;

			if (SendCmd(cmd, p_data, length))
			{
				time_pre = millis();

				while(millis()-time_pre < timeout)
				{
					if (ReceiveProcess() == true)
					{
						ret = true;
						break;
					}
				}
			}

			return ret;
		}


	private:

		inline errno_t SendCmdRxResp(uint8_t* ptr_data, uint32_t size, uint32_t timeout){
			if (m_Isconnected != true)
				return -1;
			if (uartWrite(m_cfg.ch, ptr_data, size))
			{
				uint32_t pre_ms = millis();
				bool result = true;
				while (receivePacket() == false)
				{
					if ((millis() - pre_ms) > timeout)
					{
						result = false;
						break;
					}
				}
				if (result)
				{
					receiveCplt();
					return ERROR_SUCCESS;
				}
			}
			return -1;
		}


		inline bool receivePacket()	{

			constexpr uint8_t STATE_WAIT_STX 				= 0;
			constexpr uint8_t STATE_WAIT_PAGE_NO 		= 1;
			constexpr uint8_t STATE_WAIT_TYPE 			= 2;
			constexpr uint8_t STATE_WAIT_FUNC_ID 		= 3;
			constexpr uint8_t STATE_WAIT_LENGTH_L 	= 4;
			constexpr uint8_t STATE_WAIT_LENGTH_H 	= 5;
			constexpr uint8_t STATE_WAIT_DATA 			= 6;
			constexpr uint8_t STATE_WAIT_CHECKSUM_L = 7;
			constexpr uint8_t STATE_WAIT_CHECKSUM_H = 8;
			constexpr uint8_t STATE_WAIT_ETX 				= 9;


			bool ret = false;
			uint8_t rx_data = 0x00;

			uint8_t data_cnt = 0;
			std::array <uint8_t, (CMD_MAX_PACKET_LENGTH)> data_arry = {0,};

			while(uartAvailable(m_cfg.ch))
			{
				if(data_cnt <(CMD_MAX_PACKET_LENGTH))
				{
					data_arry[data_cnt++] = uartRead(m_cfg.ch);
				}
				else
				{
					break;
				}
			}//while

			if (data_cnt == 0)
			{
				return false;
			}

			rx_packet_t* rx_packet = &m_packet.rx_packet;

			if (millis() - m_packet.prev_ms >= 100)
			{
				m_packet.state = STATE_WAIT_STX;
				m_packet.index = 0;
				m_packet.data_cnt = 0;
				rx_packet->check_sum = 0xffff;
				memset(&rx_packet->buffer[0], 0x00, CMD_MAX_DATA_LENGTH);
			}
			m_packet.prev_ms = millis();


			for (uint8_t i = 0 ; i < data_cnt ; i ++ )
			{
				rx_data = data_arry[i];

				switch (m_packet.state)
				{
					case STATE_WAIT_STX:
					{
						if (rx_data == CMD_STX)
						{
							//m_packet.wait_flag = 1;
							m_packet.data_cnt = 0;
							rx_packet->check_sum = 0xffff;
							memset(&rx_packet->buffer[0], 0, CMD_MAX_PACKET_LENGTH);
							rx_packet->buffer[m_packet.data_cnt++] = rx_data;
							m_packet.state = STATE_WAIT_PAGE_NO;
							break;
						}

					}
					break;
					case STATE_WAIT_TYPE:
					{
						rx_packet->type = rx_data;
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						m_packet.state = STATE_WAIT_FUNC_ID;
					}
					break;

					case STATE_WAIT_FUNC_ID:
					{
						rx_packet->func_id = rx_data;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;

						rx_packet->length = 0;
						m_packet.state = STATE_WAIT_LENGTH_L;
					}
					break;

					case STATE_WAIT_LENGTH_L:
					{
						rx_packet->length = rx_data;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						m_packet.state = STATE_WAIT_LENGTH_H;
					}
					break;

					case STATE_WAIT_LENGTH_H:
					{
						rx_packet->length |= (rx_data << 8);
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;

						if (rx_packet->length < CMD_MAX_DATA_LENGTH)
						{
							m_packet.index = 0;
							m_packet.state = STATE_WAIT_DATA;
							rx_packet->data = &rx_packet->buffer[m_packet.data_cnt];
						}
						else
						{
							m_packet.state = STATE_WAIT_STX;
						}
					}
					break;

					case STATE_WAIT_DATA:
					{
						if (m_packet.index <= rx_packet->length) // check overflow
						{
							rx_packet->data[m_packet.index++] = rx_data;
							UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
							rx_packet->buffer[m_packet.data_cnt++] = rx_data;

							if (m_packet.index == rx_packet->length)
							{
								m_packet.state = STATE_WAIT_CHECKSUM_L;
							}
						}
						else
						{
							m_packet.state = STATE_WAIT_STX;
							m_packet.index = 0;
							break;
						}
					}
					break;
					case STATE_WAIT_CHECKSUM_L:
					{
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						rx_packet->check_sum_recv = rx_data;
						m_packet.state = STATE_WAIT_CHECKSUM_H;
					}
					break;
					case STATE_WAIT_CHECKSUM_H:
					{
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						rx_packet->check_sum_recv |= rx_data << 8;
						m_packet.state = STATE_WAIT_ETX;
					}
					break;

					case STATE_WAIT_ETX:
					{
						if (rx_data == CMD_ETX)
						{
							rx_packet->buffer[m_packet.data_cnt++] = rx_data;
							if (rx_packet->check_sum == rx_packet->check_sum_recv)
							{
								ret = true;
							}
						}
						m_packet.state = STATE_WAIT_STX;
					}
					break;
				}
				//switch (m_packet.state)
			}
			// end of for (uint8_t i = 0 ; i < data_cnt ; i ++ )


			return ret;
		}


	private:
		inline void receiveCplt()	{
			m_packet.resp_ms = millis() - m_packet_sending_ms;
			if (m_func && m_obj)
			{
				m_func(m_obj,nullptr,&m_packet.rx_packet);
			}
		}


	};


}//end of namespace AP


#endif /* SRC_AP__INC_UART_HPP_ */
