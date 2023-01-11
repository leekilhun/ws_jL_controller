/*
 * uart_api.hpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_UART_REMOTE_HPP_
#define SRC_AP__INC_UART_REMOTE_HPP_

/*

mcu - server, pc - client

TX  (mcu -> pc) provide information
obj_id [option] 0 is all or ignore
| SOF  | tx_Type  | obj_Id | Data Length |Data          |   Checksum   | EOF  |
| :--- |:---------|:-------| :---------- |:-------------|:-------------| :--  |
| 0x4A |  1 byte  | 1 byte | 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

RX (pc -> mcu) request information or action
obj_id [option] 0 is all or ignore
| SOF  | cmd_Type | obj_Id | Data Length |Data          |   Checksum   | EOF  |
| :--- |:---------|:-------| :---------- |:-------------|:-------------| :--  |
| 0x4A |  1 byte  | 1 byte | 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |



 */

namespace RCTRL
{

	//TX  (mcu -> pc) provide information
  enum TX_TYPE:uint8_t
  {
    TX_MCU_STATE_DATA = 0x01,
    TX_MOTOR_DATA 		= 0x02,
		TX_MOTOR_CFG_MOTION_ORIGIN 	= 0x03,

		TX_EEPROM_MOTION_DATA_L 	= 0x11,
		TX_EEPROM_MOTION_DATA_H 	= 0x12,
		TX_EEPROM_CONFIG_DATA   	= 0x13,
		TX_EEPROM_CYLINDER_DATA   = 0x14,
		TX_EEPROM_VACUUM_DATA   	= 0x15,
		TX_EEPROM_SEQUNCE_DATA   	= 0x16,
		TX_EEPROM_LINK_POS_DATA_L = 0x17,
		TX_EEPROM_LINK_POS_DATA_H = 0x18,
		TX_EEPROM_LOG_DATA 				= 0x19,

		TX_OK_RESPONSE						= 0xAA,
  };


	/*enum CMD_TYPE:uint8_t
	{
		CTRL_FIRM                   = 0x00,
		CTRL_ACT	  	              = 0x10,
		CTRL_MOT  	  	            = 0x20,
		WRITE_DATA                  = 0x40,
		READ_DATA				            = 0x50,
		OK_RESPONSE                 = 0xAA,
	};*/


  //RX (pc -> mcu) request information or action
	enum CMD_TYPE:uint8_t
	{
		CMD_READ_ALL_STATE                = 0x00,
		CMD_READ_BOOT_INFO                = 0x01,
		CMD_READ_FIRM_INFO                = 0x02,

		CMD_CTRL_IO_OUT                   = 0x10,
		CMD_CTRL_CYL                      = 0x11,
		CMD_CTRL_VAC                      = 0x12,
		CMD_CTRL_MCU_OPTION_REG 					= 0x13,

		CMD_CTRL_JOB_INITIAL              = 0x1A,
		CMD_CTRL_VIRTUAL_SW               = 0x1B,

		CMD_CTRL_MOT_ORIGIN               = 0x20,
		CMD_CTRL_MOT_ONOFF                = 0x21,
		CMD_CTRL_MOT_MOVE                 = 0x22,
		CMD_CTRL_MOT_STOP                 = 0x23,
		CMD_CTRL_MOT_JOG                  = 0x24,
		CMD_CTRL_MOT_LIMIT                = 0x25,
		CMD_CTRL_MOT_ZEROSET              = 0x26,
		CMD_CTRL_MOT_RELMOVE              = 0x27,
		CMD_CTRL_MOT_CLEAR_ALARM          = 0x28,
		CMD_CTRL_MOT_CHANGE_VEL           = 0x29,
		CMD_CTRL_MOT_MOVE_VEL             = 0x2A,
		CMD_CTRL_MOT_RELMOVE_VEL          = 0x2B,
		CMD_CTRL_MOT_VEL_JOG 							= 0x2C,

		CMD_CTRL_MOTS_ONOFF               = 0x30,
		CMD_CTRL_MOTS_RUN                 = 0x31,
		CMD_CTRL_MOTS_STOP                = 0x32,
		CMD_CTRL_MOTS_REL                 = 0x33,
		CMD_CTRL_MOT_JOG_STOP             = 0x34,
		CMD_CTRL_MOT_LINK_RUN             = 0x35,

		CMD_CTRL_MOT_DATA_MOVE_PARAM      = 0x3A,
		CMD_CTRL_MOT_DATA_INIT_PARAM      = 0x3B,

		CMD_EEPROM_WRITE_MOTOR_POS_DATA_L	= 0x40,
		CMD_EEPROM_WRITE_MOTOR_POS_DATA_H	= 0x41,
		CMD_EEPROM_WRITE_CFG_DATA 				= 0x42,
		CMD_EEPROM_WRITE_CYL_DATA 				= 0x43,
		CMD_EEPROM_WRITE_VAC_DATA 				= 0x44,
		CMD_EEPROM_WRITE_SEQ_DATA 				= 0x45,
		CMD_EEPROM_WRITE_LINK_POS_L       = 0x46,
		CMD_EEPROM_WRITE_LINK_POS_H       = 0x47,

		CMD_EEPROM_READ_MOTOR_POS_DATA		= 0x48,
		//0x49,
		CMD_EEPROM_READ_CFG_DATA 					= 0x4A,
		CMD_EEPROM_READ_CYL_DATA 					= 0x4B,
		CMD_EEPROM_READ_VAC_DATA 					= 0x4C,
		CMD_EEPROM_READ_SEQ_DATA 					= 0x4D,
		CMD_EEPROM_READ_LINK_POS          = 0x4E,
		CMD_EEPROM_READ_LOG								= 0x4F,

		CMD_EEPROM_CLEAR_ROM_DATA 				= 0x50,
		CMD_EEPROM_RELOAD_ROM_DATA 				= 0x51,

		CMD_READ_MCU_DATA                 = 0x60,
		CMD_READ_MOTOR_DATA               = 0x61,
		CMD_READ_MOTION_ORG_CFG_DATA      = 0x62,

		CMD_OK_RESPONSE                   = 0xAA,
	};


	constexpr uint8_t CMD_STX 					= 0x4A;
	constexpr uint8_t CMD_ETX 					= 0x4C;

	constexpr int CMD_MAX_DATA_LENGTH 	= 48;
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
			uint8_t         obj_id{};
			uint16_t        length{};
			uint16_t        check_sum{};
			uint16_t        check_sum_recv{};
			std::array <uint8_t, CMD_MAX_PACKET_LENGTH> buffer{};
			uint8_t   *data;

			rx_packet_t() = default;

			inline void Init(){
				type = 0;
				obj_id = 0;
				length = 0;
				data = 0;
				check_sum = 0;
				check_sum_recv = 0;
				buffer.fill(0);
			}

			rx_packet_t& operator = (const rx_packet_t& dat){
				type = dat.type;
				obj_id = dat.obj_id;
				length = dat.length;
				data = dat.data;
				check_sum = dat.check_sum;
				check_sum_recv = dat.check_sum_recv;
				buffer = dat.buffer;
				return *this;
			}

			rx_packet_t& operator = (const rx_packet_t* dat){
				type = dat->type;
				obj_id = dat->obj_id;
				length = dat->length;
				data = dat->data;
				check_sum = dat->check_sum;
				check_sum_recv = dat->check_sum_recv;
				buffer = dat->buffer;
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

		inline uint8_t GetErrCnt() const {
			return m_packet.error;
		}

		inline uint8_t AddErrCnt()  {
			return ++m_packet.error;
		}

		inline bool Recovery() {
			uartClose(m_cfg.ch);
			/* */
			m_packet.state = 0;
			m_packet.error = 0;
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


		//obj_id [option] 0 is all or ignore
		uint32_t SendData (uint8_t tx_type, uint8_t *p_data, uint8_t length, uint8_t obj_id = 0)
		{
			/*

				| SOF  | Type |obj_id| Data Length |Data          |   Checksum   | EOF  |
				| :--- |:-----|:---- | :---------- |:-------------|:-------------| :--  |
				| 0x4A |1 byte|1 byte| 2 byte(L+H) |Data 0～Data n|2 byte(crc 16)| 0x4C |

			 */
			uint8_t idx = 0;
			uint16_t crc = 0xffff;

			std::array<uint8_t, CMD_MAX_PACKET_LENGTH> value = { };
			value[idx++] = CMD_STX;
			value[idx++] = tx_type;
			UTL::crc16_modbus_update(&crc, tx_type);
			value[idx++] = obj_id;
			UTL::crc16_modbus_update(&crc, obj_id);
			value[idx++] = (uint8_t)(length >> 0);
			UTL::crc16_modbus_update(&crc,(uint8_t)(length >> 0));
			value[idx++] = (uint8_t)(length >> 8);
			UTL::crc16_modbus_update(&crc,(uint8_t)(length >> 8));
			for (uint8_t i = 0; i < length; i++)
			{
				value[idx++] = p_data[i];
				UTL::crc16_modbus_update(&crc,p_data[i]);
			}
			value[idx++] = (uint8_t)(crc >> 0);
			value[idx++] = (uint8_t)(crc >> 8);
			value[idx++] = CMD_ETX;

			m_packet_sending_ms = millis();

			return uartWrite(m_cfg.ch, value.data(), idx);
		}


#if 0

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

#endif



		inline bool receivePacket()	{

			constexpr uint8_t STATE_WAIT_STX 				= 0;
			constexpr uint8_t STATE_WAIT_TYPE 			= 1;
			constexpr uint8_t STATE_WAIT_OBJ_ID 		= 2;
			constexpr uint8_t STATE_WAIT_LENGTH_L 	= 3;
			constexpr uint8_t STATE_WAIT_LENGTH_H 	= 4;
			constexpr uint8_t STATE_WAIT_DATA 			= 5;
			constexpr uint8_t STATE_WAIT_CHECKSUM_L = 6;
			constexpr uint8_t STATE_WAIT_CHECKSUM_H = 7;
			constexpr uint8_t STATE_WAIT_ETX 				= 8;


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
							m_packet.state = STATE_WAIT_TYPE;
							break;
						}

					}
					break;
					case STATE_WAIT_TYPE:
					{
						rx_packet->type = rx_data;
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						m_packet.state = STATE_WAIT_OBJ_ID;
					}
					break;

					case STATE_WAIT_OBJ_ID:
					{
						rx_packet->obj_id = rx_data;
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

						m_packet.index = 0;
						if (rx_packet->length == 0)
						{
							m_packet.state = STATE_WAIT_CHECKSUM_L;
						}
						else if (rx_packet->length < CMD_MAX_DATA_LENGTH)
						{
							m_packet.state = STATE_WAIT_DATA;
						}
						else
						{
							m_packet.state = STATE_WAIT_STX;
						}
					}
					break;

					case STATE_WAIT_DATA:
					{
						if (m_packet.index == 0)
							rx_packet->data = &rx_packet->buffer[m_packet.data_cnt];

						if (m_packet.index < rx_packet->length)
						{
							rx_packet->data[m_packet.index] = rx_data;
							UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
							rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						}
						else// check overflow
						{
							m_packet.state = STATE_WAIT_STX;
							m_packet.index = 0;
						}
						m_packet.index++;
						// check complete
						if (m_packet.index == rx_packet->length)
						{
							m_packet.state = STATE_WAIT_CHECKSUM_L;
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
