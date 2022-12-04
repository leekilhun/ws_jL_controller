/*
 * uart_moons.hpp
 *
 *  Created on: 2022. 11. 30.
 *      Author: gns2.lee
 */

#ifndef SRC_AP__INC_UART_MOONS_HPP_
#define SRC_AP__INC_UART_MOONS_HPP_


namespace MOTOR
{



#ifdef _USE_HW_RTOS
#define AP_UART_MOONS_LOCK_BEGIN osMutexWait(m_mutex_id, osWaitForever)
#define AP_UART_MOONS_LOCK_END   osMutexRelease(m_mutex_id)
#else
#define AP_UART_MOONS_LOCK_BEGIN
#define AP_UART_MOONS_LOCK_END
#endif


	constexpr int MOONS_COMM_TIMEOUT = 100;
	constexpr int MOONS_PACKET_BUFF_LENGTH = 100;
	constexpr int MOONS_MULTI_WRITE_DATA_LENGTH = 10;

	class uart_moons
	{
		/****************************************************
		 *  data
		 ****************************************************/
	public:
		struct speed_t{
			uint16_t accel{};
			uint16_t decel{};
			uint16_t speed{};
		};
		struct cfg_t
		{
			uint8_t ch{}; uint32_t baud{};
		};

		struct rx_packet_t
		{
			uint8_t   node_id{};
			uint8_t   func_type{};
			uint16_t  reg_addr{};
			uint16_t  reg_length{};
			uint8_t   data_length{};
			uint8_t*  data{};
			uint16_t  check_sum{};
			uint16_t  check_sum_recv{};
			std::array <uint8_t, MOONS_PACKET_BUFF_LENGTH> buffer{};

			rx_packet_t() = default;

			rx_packet_t& operator = (const rx_packet_t& dat){
				if ( this != &dat){
					node_id = dat.node_id;
					func_type = dat.func_type;
					reg_addr = dat.reg_addr;
					reg_length = dat.reg_length;
					data_length = dat.data_length;
					data = dat.data;
					check_sum = dat.check_sum;
					check_sum_recv = dat.check_sum_recv;
					buffer = dat.buffer;
				}
				return *this;
			}

			rx_packet_t& operator = (const rx_packet_t* dat){
				if ( this != dat){
					node_id = dat->node_id;
					func_type = dat->func_type;
					reg_addr = dat->reg_addr;
					reg_length = dat->reg_length;
					data_length = dat->data_length;
					data = dat->data;
					check_sum = dat->check_sum;
					check_sum_recv = dat->check_sum_recv;
					buffer = dat->buffer;
				}
				return *this;
			}


		};

	private:

		struct modbus_t {
			uint8_t   state{};
			uint32_t  prev_ms{};
			uint32_t  resp_ms{};
			uint32_t  index{};
			uint32_t  data_cnt{};
			uint8_t   error{};

			rx_packet_t  rx_packet{};
		};

		bool m_Isconnected;
		cfg_t m_cfg;
		modbus_t m_packet;

		std::array <void* ,AP_DEF_OBJ_MOTOR_ID_MAX> m_objs;
		evt_cb m_func;
		uint32_t m_packet_sending_ms;


#ifdef _USE_HW_RTOS
		osMutexId m_mutex_id;
#endif

		/****************************************************
		 *  Constructor
		 ****************************************************/
	public:
		uart_moons():m_Isconnected{}, m_cfg{}, m_packet{}, m_objs{}, m_func{}
		,m_packet_sending_ms{}
		{
#ifdef _USE_HW_RTOS
			osMutexDef(m_mutex_id);
			m_mutex_id = osMutexCreate (osMutex(m_mutex_id));
#endif

		}
		virtual ~uart_moons (){}

		/****************************************************
		 *  func
		 ****************************************************/

		inline void Init(cfg_t &cfg) {
			m_cfg = cfg;
			if (uartOpen(cfg.ch, cfg.baud))
			{
				m_Isconnected = true;
			}
		}

		inline bool IsConnected() const {
			return m_Isconnected;
		}

		inline bool Recovery() {
			uartClose(m_cfg.ch);
			/* */
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

		inline void AttCallbackFunc(uint8_t instance_no, void* obj, evt_cb cb)
		{
			if ( instance_no == AP_DEF_OBJ_MOTOR_ID_JIG
					|| instance_no == AP_DEF_OBJ_MOTOR_ID_ROLL
					|| instance_no == AP_DEF_OBJ_MOTOR_ID_HIGH)
			{
				m_objs[instance_no] = obj;
			}
			m_func = cb;
		}


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


		inline errno_t JogMove(uint8_t node_id, uart_moons::speed_t& params, bool is_cw = true) {
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





	private:
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

			enum func_e {
				read_HoldingReg = 0x03,
				read_InputReg = 0x04,
				write_SingleReg = 0x06,
				write_MultiReg = 0x10
			};

			constexpr int STATE_WAIT_ID = 				0;
			constexpr int STATE_WAIT_FUNCNTION = 	1;
			constexpr int STATE_WAIT_LENGTH = 		2;
			constexpr int STATE_WAIT_REG_ADDR_L = 3;
			constexpr int STATE_WAIT_REG_ADDR_H = 4;
			constexpr int STATE_WAIT_DATA = 			5;
			constexpr int STATE_WAIT_CHECKSUM_L = 6;
			constexpr int STATE_WAIT_CHECKSUM_H = 7;


			bool ret = false;
			uint8_t rx_data = 0x00;

			uint8_t data_cnt = 0;
			std::array <uint8_t, (MOONS_PACKET_BUFF_LENGTH + 8)> data_arry = {0,};

			while(uartAvailable(m_cfg.ch))
			{
				if(data_cnt <(MOONS_PACKET_BUFF_LENGTH + 8))
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
				m_packet.state = STATE_WAIT_ID;
				m_packet.index = 0;
				m_packet.data_cnt = 0;
				rx_packet->check_sum = 0xffff;
				memset(&rx_packet->buffer[0], 0x00, MOONS_PACKET_BUFF_LENGTH);
			}
			m_packet.prev_ms = millis();


			for (uint8_t i = 0 ; i < data_cnt ; i ++ )
			{
				rx_data = data_arry[i];

				switch (m_packet.state)
				{
					case STATE_WAIT_ID:
					{
						rx_packet->node_id = rx_data;
						m_packet.index = 0;
						m_packet.data_cnt = 0;
						rx_packet->check_sum = 0xffff;
						m_packet.state = STATE_WAIT_FUNCNTION;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;
					}
					break;

					case STATE_WAIT_FUNCNTION:
					{
						rx_packet->func_type = rx_data;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;

						switch (static_cast<func_e>(rx_data))
						{
							case func_e::read_HoldingReg:
								__attribute__((fallthrough));
							case func_e::read_InputReg:
								m_packet.state = STATE_WAIT_LENGTH;
								break;

							case func_e::write_SingleReg:  // skip break;
								__attribute__((fallthrough));
							case func_e::write_MultiReg:   // skip break;
								m_packet.state = STATE_WAIT_REG_ADDR_L;
								break;

							default:
								m_packet.state = STATE_WAIT_ID;
								m_packet.index = 0;
								m_packet.data_cnt = 0;
								rx_packet->check_sum = 0xffff;
								memset(&rx_packet->buffer[0], 0x00, MOONS_PACKET_BUFF_LENGTH);
								i = data_cnt;
								break;

						} //switch (static_cast<func_e>(rx_data))
					}
					break;

					case STATE_WAIT_LENGTH:
					{
						rx_packet->data_length = rx_data;
						m_packet.state = STATE_WAIT_DATA;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;
					}
					break;

					case STATE_WAIT_REG_ADDR_L:
					{
						rx_packet->reg_addr = rx_data;
						m_packet.state = STATE_WAIT_REG_ADDR_H;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;
					}
					break;

					case STATE_WAIT_REG_ADDR_H:
					{
						rx_packet->reg_addr |= (rx_data << 8);
						rx_packet->data_length = 2;// 0x06, 0x10 경우 2 byte;
						m_packet.state = STATE_WAIT_DATA;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;
					}
					break;

					case STATE_WAIT_DATA:
					{
						if (m_packet.data_cnt++ == 0)
						{
							rx_packet->data = &rx_packet->buffer[m_packet.index];
						}
						if (m_packet.data_cnt >= rx_packet->data_length)
						{
							m_packet.state = STATE_WAIT_CHECKSUM_L;
						}
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;
						UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					}
					break;

					case STATE_WAIT_CHECKSUM_L:
					{
						rx_packet->check_sum_recv = rx_data;
						m_packet.state = STATE_WAIT_CHECKSUM_H;
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;
					}
					break;

					case STATE_WAIT_CHECKSUM_H:
					{
						rx_packet->buffer[m_packet.index] = rx_data;
						m_packet.index++;
						rx_packet->check_sum_recv |= (rx_data << 8);
						if (rx_packet->check_sum == rx_packet->check_sum_recv)
						{
							ret = true;
						}
						m_packet.state = STATE_WAIT_ID;
					}
					break;
				}//switch (m_packet.state)
			}// for


			return ret;
		}


	private:
		inline void receiveCplt()	{
			m_packet.resp_ms = millis() - m_packet_sending_ms;
			uint8_t instance_no = M_GetMotorInstanceId(m_packet.rx_packet.node_id);
			if (instance_no < m_objs.size())
			{
				if (m_func && m_objs[instance_no])
				{
					m_func(m_objs[instance_no],&instance_no,&m_packet.rx_packet);
				}
			}

		}


	};


}


#endif /* SRC_AP__INC_UART_MOONS_HPP_ */
