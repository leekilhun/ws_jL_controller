#pragma once
/*
moons rs485 modbus 통신을 위한 통신 클래스로
모터 객체에 shared된다.


*/

#ifndef USE_HW_COMM_MOONS

namespace GMoons
{


#define MOONS_COMM_BUFF_MAX  4096

	constexpr int MOONS_COMM_TIMEOUT = 1000;
	constexpr int MOONS_PACKET_BUFF_LENGTH = 100;
	constexpr int MOONS_MULTI_WRITE_DATA_LENGTH = 10;

	class GMoons_comm
	{
		/****************************************************
		 *	data
		 ****************************************************/
	public:
		struct cfg_t
		{
			IComm* pComm{};
			uint8_t base_node_id{};

			inline void operator = (const cfg_t* p_cfg) {
				this->base_node_id = p_cfg->base_node_id;
				this->pComm = p_cfg->pComm;
			}

		};

	private:
		struct tx_packet_t
		{
			uint8_t   node_id{};
			uint8_t   func_type{};
			uint16_t  reg_addr{};
			uint16_t  reg_length{};
			uint16_t  content{};  // case moons motor
			uint8_t   content_cnt{};

			uint16_t   check_sum{};
			uint16_t   check_sum_recv{};
			uint8_t   buffer[MOONS_PACKET_BUFF_LENGTH]{};
		};

		struct rx_packet_t
		{
			uint8_t   node_id{};
			uint8_t   func_type{};
			uint16_t  reg_addr{};
			uint16_t  reg_length{};
			uint8_t   data_length{};
			uint8_t* data{};
			uint16_t  check_sum{};
			uint16_t  check_sum_recv{};
			uint8_t   buffer[MOONS_PACKET_BUFF_LENGTH]{};
		};


		struct modbus_t {
			uint8_t   state{};
			uint32_t  pre_time{};
			uint32_t  index{};
			uint32_t  data_cnt{};
			uint8_t   error{};

			tx_packet_t  tx_packet;
			rx_packet_t  rx_packet;
		};


	private:
		UTL::_que<uint8_t> m_Que;
		modbus_t m_packet;
		std::mutex  m_mutax;
		void* m_cbObj;
		evt_cb m_func;

		bool m_TrdLock;
		bool m_TrdLife;
		HANDLE m_hThread;
		DWORD m_TrdId;


		cfg_t  m_commCfg;


		/****************************************************
		   *	Constructor
		   ****************************************************/
	public:
		GMoons_comm(cfg_t& cfg) :m_commCfg(cfg), m_cbObj(nullptr), m_func()
			, m_TrdLock(false), m_hThread(), m_TrdId()
		{
			m_commCfg.pComm->AttCallbackFunc(this, receiveDataFunc);
			errno_t ret = m_commCfg.pComm->Open();
			if (ret != ERROR_SUCCESS)
			{
				ERR_PRINT("serial port open error : error_code[%d] ", ret);
			}
			LTRACE("Init moons_motor : serial port open \n");
			threadRun();
		}

		virtual ~GMoons_comm() {
			threadStop();
		}

		/****************************************************
		 *	func
		 ****************************************************/
	private:

		inline void threadStop(void) {
			m_TrdLife = false;
			while (m_TrdId);
		}
		inline void threadRun(void) {
			m_TrdLife = true;
			// 포트 감시 스레드 생성
			m_hThread = CreateThread(
				(LPSECURITY_ATTRIBUTES)NULL,
				0,
				(LPTHREAD_START_ROUTINE)threadFunc,
				(LPVOID)this,
				0,
				&m_TrdId
			);
		}
		inline void threadJob(void) {
			if (m_TrdLock == false)
			{
				if (receivePacket() == true)
				{
					receiveCplt();
				}
			}
		}


		void receiveCplt()
		{

		}



		bool receivePacket()
		{
			// TODO: 여기에 구현 코드 추가.
			constexpr int STATE_WAIT_ID = 0;
			constexpr int STATE_WAIT_FUNCNTION = 1;
			constexpr int STATE_WAIT_LENGTH = 2;
			constexpr int STATE_WAIT_REG_ADDR_L = 3;
			constexpr int STATE_WAIT_REG_ADDR_H = 4;
			constexpr int STATE_WAIT_DATA = 5;
			constexpr int STATE_WAIT_CHECKSUM_L = 6;
			constexpr int STATE_WAIT_CHECKSUM_H = 7;

			enum func_e {
				read_HoldingReg,
				read_InputReg,
				write_SingleReg,
				write_MultiReg,
			};

			bool ret = false;
			uint8_t rx_data = 0x00;
			std::array<uint8_t, MOONS_PACKET_BUFF_LENGTH> arry_data{ 0, };
			int data_length = m_Que.Available() % MOONS_PACKET_BUFF_LENGTH;
			if (data_length)
			{
				m_Que.Read(arry_data.data(), data_length);
			}
			else
			{
				return false;
			}


			rx_packet_t* rx_packet = &m_packet.rx_packet;

			if (UTL::millis() - m_packet.pre_time >= 100)
			{
				m_packet.state = STATE_WAIT_ID;
				m_packet.index = 0;
				m_packet.data_cnt = 0;
				rx_packet->check_sum = 0xffff;
				memset(&rx_packet->buffer[0], 0x00, MOONS_PACKET_BUFF_LENGTH);
			}
			m_packet.pre_time = UTL::millis();


			for (int i = 0; i < data_length; i++)
			{
				rx_data = arry_data[i];
				switch (m_packet.state)
				{
				case STATE_WAIT_ID:
					rx_packet->node_id = rx_data;
					m_packet.state = STATE_WAIT_FUNCNTION;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;
					break;

				case STATE_WAIT_FUNCNTION:
					rx_packet->func_type = rx_data;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;

					switch (static_cast<func_e>(rx_data))
					{
					case read_HoldingReg:
						__fallthrough;
					case read_InputReg:
						m_packet.state = STATE_WAIT_LENGTH;
						break;

					case write_SingleReg:  // skip break;
						__fallthrough;
					case write_MultiReg:   // skip break;
						__fallthrough;
					default:
						m_packet.state = STATE_WAIT_REG_ADDR_L;
						break;
					}
					break;

				case STATE_WAIT_LENGTH:
					rx_packet->data_length = rx_data;
					m_packet.state = STATE_WAIT_DATA;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;
					break;

				case STATE_WAIT_REG_ADDR_L:
					rx_packet->reg_addr = rx_data;
					m_packet.state = STATE_WAIT_REG_ADDR_H;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;
					break;

				case STATE_WAIT_REG_ADDR_H:
					rx_packet->reg_addr |= (rx_data << 8);
					rx_packet->data_length = 2;// 0x06, 0x10 경우 2 byte; 
					m_packet.state = STATE_WAIT_DATA;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;
					break;

				case STATE_WAIT_DATA:
					if (m_packet.data_cnt++ == 0)
					{
						rx_packet->data = &rx_packet->buffer[m_packet.index];
					}
					if (m_packet.data_cnt >= rx_packet->data_length)
					{
						m_packet.state = STATE_WAIT_CHECKSUM_L;
					}
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;
					UTL::crc16_modbus_update(&rx_packet->check_sum, rx_data);
					break;


				case STATE_WAIT_CHECKSUM_L:
					rx_packet->check_sum_recv = rx_data;
					m_packet.state = STATE_WAIT_CHECKSUM_H;
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;
					break;

				case STATE_WAIT_CHECKSUM_H:
					rx_packet->buffer[m_packet.index++ % MOONS_PACKET_BUFF_LENGTH] = rx_data;
					rx_packet->check_sum_recv |= (rx_data << 8);
					if (rx_packet->check_sum == rx_packet->check_sum_recv)
					{
						ret = true;
					}
					m_packet.state = STATE_WAIT_ID;
					break;
				}

			}


			return ret;
		}



		inline int sendData(char* p_data, uint32_t length)
		{

			int ret = ERROR_SUCCESS;

			std::vector<char> v_str(length);
			for (size_t i = 0; i < length; i++) {
				v_str[i] = p_data[i];
			}
			uint8_t idx = 0;
			uint16_t result_crc = 0xFFFF;
			uint16_t ret_crc = (uint16_t)UTL::crc16_modbus_cal((uint8_t*)p_data, length);

			v_str.emplace_back((uint8_t)ret_crc);
			v_str.emplace_back((uint8_t)(ret_crc >> 8));

			if (this->sendPacketWaitRxResp((uint8_t*)v_str.data(), (uint32_t)v_str.size(), MOONS_COMM_TIMEOUT) != true)
			{
				ERR_PRINT("fail! moons_motor send packet data");
				ret = -1;
			}

			return ret;
		}



		inline bool sendPacketWaitRxResp(uint8_t* p_data, uint32_t length, uint32_t timeout) {
			bool  ret = false;
			uint32_t time_pre;

			m_TrdLock = true;

			m_commCfg.pComm->SendData((char*)p_data, length);

			time_pre = UTL::millis();

			while (1)
			{
				if (receivePacket() == true)
				{
					ret = true;
					break;
				}

				if (UTL::millis() - time_pre >= timeout)
				{
					ERR_PRINT("wait packet timeout!  wait response packet data");
					break;
				}
				Sleep(10);
			}


			m_TrdLock = false;

			return ret;
		}








		static UINT threadFunc(LPVOID pParam) {
			GMoons_comm* pThis = (GMoons_comm*)pParam;
			LTRACE("moons_motor Thread Start\n\r");

			while (pThis->m_TrdLife)
			{
				pThis->threadJob();

				Sleep(10);
			}

			LTRACE("moons_motor Thread Terminatet\n\r");
			pThis->m_TrdId = 0;
			pThis->m_hThread = NULL;
			return 0;
		}



		static void receiveDataFunc(void* obj, void* w_parm, void* l_parm) {
			GMoons_comm* pThis = (GMoons_comm*)obj;
			if (w_parm == nullptr)
				return;
			int length = *((int*)w_parm);


			int index = 0;
			for (index = 0; index < length; index++)
			{
				uint8_t data = *((uint8_t*)l_parm + index);

				pThis->m_Que.Put(data);

			}
		}






	public:
		//uint32_t SendReceiveData();

		//errno_t SendCmd(uint8_t cmd, uint8_t* p_data, uint8_t length);
		//errno_t SendResp(uint8_t cmd, uint8_t* p_data, uint8_t length);
		//errno_t SendCmdRxResp(uint8_t cmd, uint8_t* p_data, uint8_t length);

		//uint32_t ReadByte(uint8_t* resp, uint32_t len);
		//uint32_t WriteByte(uint8_t* send, uint32_t len);

	};

}
#endif // #ifndef USE_HW_COMM_MOONS