/*
 * uart_nextion.hpp
 *
 *  Created on: 2022. 12. 1.
 *      Author: gns2.lee
 */

#ifndef SRC_AP__INC_UART_NEXTION_HPP_
#define SRC_AP__INC_UART_NEXTION_HPP_

namespace NXLCD
{

	enum RXCMD
	{
		REQ_BEEP   =               0x00,
		WRITE_OUT_REG  =           0x03,
		CURR_PAGE_NO   =           0x04,
		UPATE_MCU_REG  =           0x05,
		CTRL_MOTOR      =          0x06,
		SAVE_MOTOR_DATA =          0x07,
		REQ_MOTOR_DATA =           0x08,
		SAVE_AP_CFG   =            0x09,
		REQ_AP_CFG   =             0x0a,
		SAVE_CYL_DATA =            0x0b,
		REQ_CYL_DATA  =            0x0c,
		SAVE_VAC_DATA =            0x0d,
		REQ_VAC_DATA  =            0x0e,
		SAVE_SEQ_DATA =            0x0f,
		REQ_SEQ_DATA  =            0x10,
		REQ_SEQ_INIT =             0x11,
		CTRL_CYL    =              0x12,
		CTRL_VAC  =                0x13,
		RESP_BKCMD =               0xaa,

	};


	enum TXCMD
	{
		MOTOR_CURR_POS   =         0x01,
		MOTOR_CURR_VEL =           0x02,
		IO_DATA_REG    =           0x03,
		MCU_DATA_REG   =           0x04,
		MCU_MOTOR_DATA   =         0x05,
		MCU_AP_CFG     =           0x06,
		MCU_CYLINDER_DATA  =       0x07,
		MCU_VACUUM_DATA  =         0x08,
		MCU_SEQUENCE_DATA   =      0x09,
		LCD_BKCMDLEVEL  =          0x11,
		LCD_START_REPARSEMODE =    0x12,
		LCD_END_REPARSEMODE =      0x13,
		LCD_CHANGE_PAGE =          0x14,
		LCD_REQUSET_PAGE =         0x15,
		LCD_RESET_RECEIVE_CNT=     0x16,
		POP_ALARM_MSG   =          0x50,
		OK_RESPONSE =              0x70,


	};

	enum REPLYCMD
	{
		BKCMD_OK  =                 0x66,
		GETVALUE_TXT =              0x70,
		GETVALUE_NUM =              0x71,
	};



	constexpr uint8_t CMD_STX1 = 0x48;
	constexpr uint8_t CMD_STX2 = 0x4A;
	constexpr uint8_t CMD_ETX1 = 0x4A;
	constexpr uint8_t CMD_ETX2 = 0x4C;

	constexpr int NEXTION_COMM_TIMEOUT = 100;
	constexpr int NEXTION_PACKET_BUFF_LENGTH = 32;

	constexpr int CMD_MAX_DATA_LENGTH = 24;
	constexpr int CMD_MAX_PACKET_LENGTH =(CMD_MAX_DATA_LENGTH + 8);

	constexpr int CMD_STATE_REPLY_PAGE_NO = 11;





#ifdef _USE_HW_RTOS
#define AP_UART_NEXTION_LOCK_BEGIN osMutexWait(m_mutex_id, osWaitForever)
#define AP_UART_NEXTION_LOCK_END   osMutexRelease(m_mutex_id)
#else
#define AP_UART_NEXTION_LOCK_BEGIN
#define AP_UART_NEXTION_LOCK_END
#endif




	class uart_nextion
	{
		/****************************************************
		 *  data
		 ****************************************************/
	public:


		/*
		– Level 0 is Off – no pass/fail will be returned
		– Level 1 is OnSuccess, only when last serial command successful.
		– Level 2 is OnFailure, only when last serial command failed
		– Level 3 is Always, returns 0x00 to 0x23 result of serial command.
		 */
		enum bkCMdLevel
		{
			NO_RETURN,
			ONLY_SUCCESS,
			ONLY_FAIL,
			ALWAYS_RETURN,
		};

		enum page_e
		{
			INIT,
			MAIN,
			MANUAL,
			TEACH,
			IO,
			DATA,
			LOG,
		};



		struct cfg_t
		{
			uint8_t ch{}; uint32_t baud{};
		};


		struct rx_packet_t
		{
			page_e  				page_no{};
			uint8_t         type{};
			uint8_t         obj_id{};
			uint16_t        length{};
			uint16_t        check_sum{};
			uint16_t        check_sum_recv{};
			std::array <uint8_t, CMD_MAX_PACKET_LENGTH> buffer{};
			uint8_t   *data;

			rx_packet_t() = default;

			rx_packet_t& operator = (const rx_packet_t& dat){
				if ( this != &dat){
					page_no = dat.page_no;
					type = dat.type;
					obj_id = dat.obj_id;
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
					page_no = dat->page_no;
					type = dat->type;
					obj_id = dat->obj_id;
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
			uint8_t   wait_flag{};
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
		uart_nextion():m_Isconnected{}, m_cfg{}, m_packet{}, m_obj{}, m_func{}
		,m_packet_sending_ms{}
		{
#ifdef _USE_HW_RTOS
			osMutexDef(m_mutex_id);
			m_mutex_id = osMutexCreate (osMutex(m_mutex_id));
#endif

		}
		virtual ~uart_nextion (){}

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
			std::array<uint8_t, NEXTION_PACKET_BUFF_LENGTH> temp = { 0, };
			bool ret = false;

			if (length > 4)
			{
				value = utilDwToInt(p_data);
			}
			else if (length == 1)
			{
				value = (uint32_t)p_data[0];
			}

			//value = p_data[3]<<24|p_data[2]<<16|p_data[1]<<8|p_data[0];

			int len = 0;
			switch (cmd)
			{
				case TXCMD::IO_DATA_REG:
				{
					sprintf((char*)temp.data(),"cc_in_out_reg=%d",(int)value);//"cc_in_out_reg=%d"
					ret = true;
				}
				break;
				case TXCMD::MCU_DATA_REG:
				{
					sprintf((char*)temp.data(), "cc_mcu_state_reg=%d",(int)value);
					ret = true;
				}
				break;
				case TXCMD::MOTOR_CURR_POS:
				{
					sprintf((char*)temp.data(), "cc_mot1_act_pos=%d",(int)value);
					ret = true;
				}
				break;
				case TXCMD::MOTOR_CURR_VEL:
				{
					sprintf((char*)temp.data(), "cc_mot1_act_vel=%d",(int)value);
					ret = true;
				}
				break;
				case TXCMD::LCD_BKCMDLEVEL:
				{
					sprintf((char*)temp.data(), "bkcmd=%d",(int)value);
					ret =true;
				}
				break;
				case TXCMD::LCD_START_REPARSEMODE:
				{
					sprintf((char*)temp.data(), "recmod=%d",(int)value);
					ret =true;
				}
				break;
				case TXCMD::LCD_END_REPARSEMODE:
				{
					sprintf((char*)temp.data(), "DRAKJHSUYDGBNCJHGJKSHBDN");
					ret =true;
				}
				break;
				case TXCMD::LCD_CHANGE_PAGE:
				{
					sprintf((char*)temp.data(), "page %d",(int)value);
					ret =true;
				}
				break;
				case TXCMD::LCD_REQUSET_PAGE:
				{
					sprintf((char*)temp.data(), "sendme");
					ret =true;
				}
				break;
				case TXCMD::LCD_RESET_RECEIVE_CNT:
				{
					sprintf((char*)temp.data(), "nDatCnt.val=1");
					ret =true;
				}
				break;
				case TXCMD::POP_ALARM_MSG:
				{
				}
				break;
				case TXCMD::OK_RESPONSE:
				{
					sprintf((char*)temp.data(), "isResp.val=1");
					ret =true;
				}
				break;
				default:
					break;
			}

			if (ret)
			{
				value = 0;
				len = strnlen((const char*)temp.data(), NEXTION_PACKET_BUFF_LENGTH);
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




#if 0
inline errno_t RequestMotorData(uint8_t node_id){
	if (m_Isconnected != true)
		return -1;

	constexpr int array_max = 6;
	constexpr int read_HoldingReg = 3;
	constexpr int Alarm_Code_AL_f = 0;
	uint8_t  func = read_HoldingReg;
	uint16_t regist_no = Alarm_Code_AL_f;
	uint16_t data_cnt = 12;
	std::array<uint8_t, array_max> send_data =
			{
					node_id,
					func,
					(uint8_t)(regist_no >> 8),
					(uint8_t)(regist_no >> 0),
					(uint8_t)(data_cnt >> 8),
					(uint8_t)(data_cnt >> 0)
			};


	if (uartWrite(m_cfg.ch, send_data.data(), (uint32_t)send_data.size()))
	{
		return ERROR_SUCCESS;
	}
	return -1;
}


inline errno_t MotorOnOff(uint8_t node_id, bool on_off = true){
	if (m_Isconnected != true)
		return -1;

	constexpr int array_max = 6;
	constexpr int write_SingleReg = 6;
	constexpr int Command_Opcode = 124;
	constexpr uint16_t motor_on =  159;
	constexpr uint16_t motor_off =  158;
	uint8_t  func = write_SingleReg;
	uint16_t regist_no = Command_Opcode;
	uint16_t value = (on_off ? motor_on:motor_off);
	std::array<uint8_t, array_max> send_data =
			{
					node_id,
					func,
					(uint8_t)(regist_no >> 8),
					(uint8_t)(regist_no >> 0),
					(uint8_t)(value >> 8),
					(uint8_t)(value >> 0)
			};


	if (uartWrite(m_cfg.ch, send_data.data(), (uint32_t)send_data.size()))
	{
		return ERROR_SUCCESS;
	}
	return -1;
}


inline errno_t JogMove(uint8_t node_id, uart_nextion::speed_t& params, bool is_cw = true) {
	if (m_Isconnected != true)
		return -1;

	constexpr int array_max = 13;
	constexpr int write_MultiReg = 16;
	constexpr int Jog_Accel_JA_ = 46;

	uint8_t  func = write_MultiReg;
	uint16_t start_reg_no = Jog_Accel_JA_;
	enum {jog_accel, jog_decel, jog_speed, _max};
	constexpr uint16_t reg_cnt = _max;
	constexpr uint8_t byte_cnt = 6;
	std::array<uint16_t, reg_cnt> value = {
			(uint16_t)params.accel,
			(uint16_t)params.decel,
			(uint16_t)params.speed
	};
	if (is_cw == false)
	{
		int a = (int)(value[2]);
		a *= -1;
		value[2] = (uint16_t)a;
	}

	std::array<uint8_t, array_max> send_data = {
			node_id,
			func,
			(uint8_t)(start_reg_no >> 8),
			(uint8_t)(start_reg_no),
			(uint8_t)(reg_cnt >> 8),
			(uint8_t)(reg_cnt >> 0),
			byte_cnt,
			(uint8_t)((value[jog_accel] >> 8) & 0xff),
			(uint8_t)((value[jog_accel] >> 0) & 0xff) ,
			(uint8_t)((value[jog_decel] >> 8) & 0xff),
			(uint8_t)((value[jog_decel] >> 0) & 0xff) ,
			(uint8_t)((value[jog_speed] >> 8) & 0xff),
			(uint8_t)((value[jog_speed] >> 0) & 0xff)
	};


	constexpr uint32_t timeout = 100;
	uint8_t retray_cnt = 3;
	while (retray_cnt)
	{
		errno_t result = SendCmdRxResp(send_data.data(), (uint32_t)send_data.size(), timeout);
		if (result == ERROR_SUCCESS)
		{
			//retray_cnt = 0;
			jogging(node_id);
			return ERROR_SUCCESS;
		}
		else
		{
			retray_cnt--;
		}
	}
	return -1;
}

inline errno_t MoveStop(uint8_t node_id) {
	if (m_Isconnected != true)
		return -1;

	constexpr int array_max = 6;
	constexpr int write_SingleReg = 6;
	constexpr int Command_Opcode = 124;
	constexpr int stop_move_kill_Buffer_normal_decel = 226;
	uint8_t  func = write_SingleReg;
	uint16_t regist_no = Command_Opcode;
	uint16_t vlaue = stop_move_kill_Buffer_normal_decel;
	std::array<uint8_t, array_max> send_data =	{
			node_id,
			func,
			(uint8_t)(regist_no >> 8),
			(uint8_t)(regist_no >> 0),
			(uint8_t)(vlaue >> 8),
			(uint8_t)(vlaue >> 0)
	};


	constexpr uint32_t timeout = 100;
	uint8_t retray_cnt = 3;
	while (retray_cnt)
	{
		errno_t result = SendCmdRxResp(send_data.data(), (uint32_t)send_data.size(), timeout);
		if (result == ERROR_SUCCESS)
		{
			return ERROR_SUCCESS;
		}
		else
		{
			retray_cnt--;
		}
	}
	return -1;

}


#endif



	private:


#if 0
inline errno_t jogging(uint8_t node_id,bool start = true){
	if (m_Isconnected != true)
		return -1;

	constexpr int array_max = 6;
	constexpr int write_SingleReg = 6;
	constexpr int Command_Opcode = 124;
	constexpr uint16_t start_jogging = 150;
	constexpr uint16_t stop_jogging = 216;

	uint8_t  func = write_SingleReg;
	uint16_t regist_no = Command_Opcode;
	uint16_t vlaue = (start ? start_jogging : stop_jogging);

	std::array<uint8_t, array_max> send_data =	{
			node_id,
			func,
			(uint8_t)(regist_no >> 8),
			(uint8_t)(regist_no >> 0),
			(uint8_t)(vlaue >> 8),
			(uint8_t)(vlaue >> 0)
	};


	constexpr uint32_t timeout = 100;
	uint8_t retray_cnt = 3;
	while (retray_cnt)
	{
		errno_t result = SendCmdRxResp(send_data.data(), (uint32_t)send_data.size(), timeout);
		if (result == ERROR_SUCCESS)
		{
			if (start == false)
				return MoveStop(node_id);
			return ERROR_SUCCESS;
		}
		else
		{
			retray_cnt--;
		}
	}
	return -1;

}

#endif


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

	constexpr uint8_t STATE_WAIT_STX = 0;
	constexpr uint8_t STATE_WAIT_PAGE_NO = 1;
	constexpr uint8_t STATE_WAIT_TYPE = 2;
	constexpr uint8_t STATE_WAIT_OBJ_ID = 3;
	constexpr uint8_t STATE_WAIT_LENGTH_L = 4;
	constexpr uint8_t STATE_WAIT_LENGTH_H = 5;
	constexpr uint8_t STATE_WAIT_DATA = 6;
	constexpr uint8_t STATE_WAIT_CHECKSUM = 7;
	constexpr uint8_t STATE_WAIT_ETX = 8;
	constexpr uint8_t STATE_WAIT_REPLY_PAGE_NO = 11;





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
					/* reply packet*/
					if (rx_data == REPLYCMD::BKCMD_OK)
					{
						rx_packet->type = rx_data;
						m_packet.state = STATE_WAIT_REPLY_PAGE_NO;
						break;
					}

					/* order packet*/
					if (rx_data == CMD_STX1)
					{
						m_packet.wait_flag = 1;
						m_packet.data_cnt = 0;
						rx_packet->check_sum = 0xffff;
						memset(&rx_packet->buffer[0], 0, CMD_MAX_PACKET_LENGTH);
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						break;
					}

					if (m_packet.wait_flag)
					{
						m_packet.wait_flag = 0;
						if (rx_data == CMD_STX2)
						{
							m_packet.state = STATE_WAIT_PAGE_NO;
							rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						}
						else
						{
							m_packet.state = STATE_WAIT_STX;
						}
					}
				}
				break;

				case STATE_WAIT_REPLY_PAGE_NO:
				{
					if (rx_data <= (uint8_t)(LOG))
					{
						rx_packet->page_no = (page_e)rx_data;
					}
					m_packet.state = STATE_WAIT_STX;
					ret = true;

				}
				break;

				case STATE_WAIT_PAGE_NO:
				{
					rx_packet->page_no = (page_e)rx_data;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					rx_packet->buffer[m_packet.data_cnt++] = rx_data;
					m_packet.state = STATE_WAIT_TYPE;
				}
				break;

				case STATE_WAIT_TYPE:
				{

					rx_packet->type = rx_data;
					rx_packet->buffer[m_packet.data_cnt++] = rx_data;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);

					if (rx_packet->type == RXCMD::CURR_PAGE_NO)
					{
						m_packet.state = STATE_WAIT_CHECKSUM;
						break;
					}
					else if (rx_packet->type == RXCMD::REQ_BEEP)
					{
						m_packet.state = STATE_WAIT_STX;
						ret = true;
						break;
					}

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

					if (rx_packet->length == 0)
					{
						m_packet.state = STATE_WAIT_CHECKSUM;
					}
					else if (rx_packet->length >= CMD_MAX_DATA_LENGTH)
					{
						m_packet.state = STATE_WAIT_STX;
					}
					else
					{
						m_packet.index = 0;
						m_packet.state = STATE_WAIT_DATA;
						rx_packet->data = &rx_packet->buffer[m_packet.data_cnt];
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
							m_packet.state = STATE_WAIT_CHECKSUM;
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
				case STATE_WAIT_CHECKSUM:
				{
					if (m_packet.wait_flag)
					{
						m_packet.wait_flag = 0;
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						rx_packet->check_sum_recv |= rx_data << 8;
						m_packet.state = STATE_WAIT_ETX;
					}
					else
					{
						m_packet.wait_flag = 1;
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						rx_packet->check_sum_recv = rx_data;
					}
				}
				break;

				case STATE_WAIT_ETX:
				{
					if (rx_data == CMD_ETX1)
					{
						rx_packet->buffer[m_packet.data_cnt++] = rx_data;
						m_packet.wait_flag = 1;
						break;
					}

					if (m_packet.wait_flag)
					{
						m_packet.wait_flag = 0;
						if (rx_data == CMD_ETX2)
						{
							rx_packet->buffer[m_packet.data_cnt++] = rx_data;
							if (rx_packet->check_sum == rx_packet->check_sum_recv)
							{
								ret = true;
							}
						}
					}

					m_packet.state = STATE_WAIT_STX;
				}
				break;
			}//switch (m_packet.state)
		}// for


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


#endif /* SRC_AP__INC_UART_NEXTION_HPP_ */
