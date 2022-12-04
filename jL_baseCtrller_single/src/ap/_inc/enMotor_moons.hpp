/*
 * enMotor_moons.hpp
 *
 *  Created on: 2022. 11. 30.
 *      Author: gns2.lee
 */

#ifndef SRC_AP__INC_ENMOTOR_MOONS_HPP_
#define SRC_AP__INC_ENMOTOR_MOONS_HPP_



namespace MOTOR
{

	class enMotor_moons
	{
		/****************************************************
		 *  data
		 ****************************************************/

		/*
      Hex Value BLu SV STAC6 ST STM
      0x0001     Position Limit
      0x0002     CCW Limit
      0x0004     CW Limit
      0x0008     Over Temp
      0x0010     Excess Regen* Internal Voltage Excess Regen Internal Voltage Internal Voltage
      0x0020     Over Voltage
      0x0040     Under Voltage* Under Voltage Under Voltage Under Voltage Under Voltage
      0x0080     Over Current
      0x0100     Bad Hall Sensor Open Motor Winding
      0x0200     Bad Encoder (not used)
      0x0400     Comm Error
      0x0800     Bad Flash
      0x1000     Wizard Failed No Move
      0x2000     Current Foldback Motor Resistance Out of Range (not used) (not used)
      0x4000     Blank Q Segment
      0x8000     No Move (not used)

		 */
		union MOONS_SS_ALARM_STATUS
		{
			uint16_t	al_status;
			struct
			{
				unsigned	Position_Error : 1;        // = 0x00000001;
				unsigned	CCW_Limit : 1;		         // = 0x00000002;
				unsigned	CW_Limit : 1;			         // = 0x00000004;
				unsigned	Over_Temp : 1;	           // = 0x00000008;
				unsigned	Internal_Voltage : 1;      // = 0x00000010;
				unsigned	Over_Voltage : 1;		       // = 0x00000020;
				unsigned	Under_Voltage : 1;	 	     // = 0x00000040;
				unsigned	Over_Current : 1;	         // = 0x00000080;
				unsigned	Open_Winding : 1;	         // = 0x00000100;
				unsigned	Bad_Encoder : 1;		       // = 0x00000200;
				unsigned	Comm_Error : 1;		         // = 0x00000400;
				unsigned	Bad_Flash : 1;		         // = 0x00000800;
				unsigned	No_Move : 1;               // = 0x00001000;
				unsigned	Current_Foldback : 1;	     // = 0x00002000;
				unsigned	Blank_Q_Segment : 1;	     // = 0x00004000;
				unsigned	NV_Memory_Double_err : 1;	 // = 0x00008000;
			};
		};
		/*

			Hex Value  Status Code bit definition
			0x0001     Motor Enabled (Motor Disabled if this bit = 0)
			0x0002     Sampling (for Quick Tuner)
			0x0004     Drive Fault (check Alarm Code)
			0x0008     In Position (motor is in position)
			0x0010     Moving (motor is moving)
			0x0020     Jogging (currently in jog mode)
			0x0040     Stopping (in the process of stopping from a stop command)
			0x0080     Waiting for an input (executing WI command)
			0x0100     Saving (parameter data is being saved)
			0x0200     Alarm present (check Alarm Code)
			0x0400     Homing (executing an SH command)
			0x0800     Wait Time (executing a WT command)
			0x1000     Q Program is running
			0x2000     Initializing (happens at power up)
			0x4000     not used
			0x8000     not used
			0x
		 */
		union MOONS_SS_DRIVE_STATUS
		{
			uint16_t	sc_status;
			struct
			{
				unsigned	Motor_Enabled : 1;			 // = 0x00000001;
				unsigned	Sampling : 1;			       // = 0x00000002;
				unsigned	Drive_Fault : 1;			   // = 0x00000004;
				unsigned	In_Position : 1;	       // = 0x00000008;
				unsigned	Moving : 1;	             // = 0x00000010;
				unsigned	Jogging : 1;		         // = 0x00000020;
				unsigned	Stopping : 1;			       // = 0x00000040;
				unsigned	Waiting_for_input : 1;	 // = 0x00000080;
				unsigned	Saving : 1;	             // = 0x00000100;
				unsigned	Alarm_present : 1;		   // = 0x00000200;
				unsigned	Homing : 1;		           // = 0x00000400;
				unsigned	WaitTime : 1;		         // = 0x00000800;
				unsigned	QProgram_is_running : 1; // = 0x00001000;
				unsigned	Initializing : 1;			   // = 0x00002000;
				unsigned	not_used0 : 1;		       // = 0x00004000;
				unsigned	not_used1 : 1;			     // = 0x00008000;
			};
		};

		struct moons_data_t {

			MOONS_SS_ALARM_STATUS al_code{};
			MOONS_SS_DRIVE_STATUS drv_status{};
			uint16_t immediate_expanded_input{};
			uint16_t driver_board_inputs{};
			uint32_t encoder_position{};
			uint32_t immediate_abs_position{};
			uint32_t abs_position_command{};
			uint16_t immediate_act_velocity{};
			uint16_t immediate_target_velocity{};
		};


	public:

		/*
		 * modbus 통신에서
		 * 가감속도는 제품 데이터 (rps*s)에 6배수로 데이터를 보내고
		 * 속도는 제품 데이터(rps)에 240배수 데이터를 보낸다.
		 */
		struct motion_param_t {
			uint32_t jog_speed{}; //단위 mmsec
			uint32_t jog_accel{}; //단위 mmsec/ms 목표 속도에 도달할 시간ms
			uint32_t jog_decel{}; //단위  mmsec/ms

			uint32_t move_velocity{}; //단위 mmsec
			uint32_t move_accel{}; //단위 mmsec/ms  목표 속도에 도달할 시간ms
			uint32_t move_decel{}; //단위 mmsec/ms

			uint32_t limit_velocity{}; //한계 속도 mmsec [max_rps * one_turn_move_mm]

			double max_rps{}; // 초당 최대 회전수 (3'000 RPM ==  50rps)
			uint32_t turn_pulse_count{}; // 한 바퀴 발생 엔코더 카운터
			double one_turn_move_mm{}; // 한 바퀴 구동 시 이동되는 실제 거리 mm  (cmd_pos/turn_per_move)*turn_per_pulse


			void Init() {
				constexpr double default_rps = 10.0;
				jog_speed = (uint32_t)(one_turn_move_mm * default_rps);
				jog_accel = 100;
				jog_decel = 100;

				move_velocity = (uint32_t)(one_turn_move_mm * default_rps * 3.0);
				move_accel = 100;
				move_decel = 100;

				limit_velocity = (uint32_t)(one_turn_move_mm * max_rps);
			}


			inline motion_param_t& operator = (const motion_param_t& cfg) {
				if ( this != &cfg){
					this->jog_speed = cfg.jog_speed;
					this->jog_accel = cfg.jog_accel;
					this->jog_decel = cfg.jog_decel;
					this->move_velocity = cfg.move_velocity;
					this->move_accel = cfg.move_accel;
					this->move_decel = cfg.move_decel;
					this->limit_velocity = cfg.limit_velocity;

					this->turn_pulse_count = cfg.turn_pulse_count;
					this->max_rps = cfg.max_rps;
					this->one_turn_move_mm = cfg.one_turn_move_mm;
				}
				return *this;
			}
		};


		struct cfg_t {
			motion_param_t motor_param{};
			uint8_t instance_no{};
			uart_moons* p_comm{};
			uint8_t ch{}; uint32_t baud{};

			cfg_t& operator = (const cfg_t& cfg){
				if ( this != &cfg){
					instance_no = cfg.instance_no;
					p_comm = cfg.p_comm;
					ch = cfg.ch;
					baud = cfg.baud;
					motor_param = cfg.motor_param;
				}
				return *this;
			}

		};




	private:
		uint8_t m_nodeId;
		cfg_t m_cfg;
		uart_moons::rx_packet_t m_receiveData;
		moons_data_t m_motorData;

#ifdef _USE_HW_RTOS
		osMutexId m_mutex_id;
#endif

		/****************************************************
		 *  Constructor
		 ****************************************************/
	public:
		enMotor_moons(uint8_t id): m_nodeId(id), m_cfg{}, m_receiveData()
		,m_motorData{}
		{
#ifdef _USE_HW_RTOS
			osMutexDef(m_mutex_id);
			m_mutex_id = osMutexCreate (osMutex(m_mutex_id));
#endif

		}
		virtual ~enMotor_moons (){}

		/****************************************************
		 *  func
		 ****************************************************/

		inline void Init(cfg_t &cfg) {
			m_cfg = cfg;
			m_cfg.p_comm->AttCallbackFunc(m_cfg.instance_no, this, receiveDataFunc);

		}

		inline uart_moons::rx_packet_t* GetPacketData() {
			return &m_receiveData;
		}

		inline uint8_t GetInstanceNo() {
			return m_cfg.instance_no;
		}


	private:

		void insertData(uart_moons::rx_packet_t* data){
			m_receiveData = data;
			uint8_t* ptr_data = m_receiveData.data ;
			enum func_e {
				read_HoldingReg = 0x03,
				read_InputReg = 0x04,
				write_SingleReg = 0x06,
				write_MultiReg = 0x10
			};

			switch (m_receiveData.func_type) {
				case read_HoldingReg:
				{
					if (m_receiveData.data_length == 24)
					{
						m_motorData.al_code.al_status = (uint16_t)(ptr_data[0] << 8) | (uint16_t)(ptr_data[1] << 0);
						m_motorData.drv_status.sc_status = (uint16_t)(ptr_data[2] << 8) | (uint16_t)(ptr_data[3] << 0);
						m_motorData.immediate_expanded_input = (uint16_t)(ptr_data[4] << 8) | (uint16_t)(ptr_data[5] << 0);
						m_motorData.driver_board_inputs = (uint16_t)(ptr_data[6] << 8) | (uint16_t)(ptr_data[7] << 0);
						m_motorData.encoder_position = (uint32_t)(ptr_data[8] << 24) | (uint32_t)(ptr_data[9] << 16) | (uint32_t)(ptr_data[10] << 8) | (uint32_t)(ptr_data[11] << 0);
						m_motorData.immediate_abs_position = (uint32_t)(ptr_data[12] << 24) | (uint32_t)(ptr_data[13] << 16) | (uint32_t)(ptr_data[14] << 8) | (uint32_t)(ptr_data[15] << 0);
						m_motorData.abs_position_command = (uint32_t)(ptr_data[16] << 24) | (uint32_t)(ptr_data[17] << 16) | (uint32_t)(ptr_data[18] << 8) | (uint32_t)(ptr_data[19] << 0);
						m_motorData.immediate_act_velocity = (uint16_t)(ptr_data[20] << 8) | (uint16_t)(ptr_data[21] << 0);
						m_motorData.immediate_target_velocity = (uint16_t)(ptr_data[22] << 8) | (uint16_t)(ptr_data[23] << 0);
					}
				}
				break;
				case read_InputReg:
				{

				}
				break;
				case write_SingleReg:
				{

				}
				break;
				case write_MultiReg:
				{

				}
				break;
				default:
					break;
			}


		}

		// callback function
		static void receiveDataFunc(void* obj, void* w_parm, void* l_parm) {
			enMotor_moons* ptr_this = (enMotor_moons*)obj;
			if (w_parm == nullptr && obj == nullptr && l_parm == nullptr)
				return;
			//uint8_t instance_no = *((uint8_t*)w_parm);
			ptr_this->insertData((uart_moons::rx_packet_t*)l_parm) ;
		}

	public:
		inline bool IsMotorOn() {
			return m_motorData.drv_status.Motor_Enabled;
		}

		inline errno_t MotorOnOff(bool on_off = true) {
			return m_cfg.p_comm->MotorOnOff(m_nodeId, on_off);
		}

		inline errno_t JogMove(bool is_cw = true) {
			uart_moons::speed_t params {100 *6, 100 * 6, 10 * 240};
			return m_cfg.p_comm->JogMove(m_nodeId,params,is_cw);
		}

		inline errno_t JogStop() {
			return 0;
		}

		inline errno_t MoveStop() {
			return 0;
		}


		inline errno_t MoveRelative(int pos) {
			errno_t ret = ERROR_SUCCESS;

			return ret;
		}

		inline errno_t MoveAbsolutive(int pos) {
			errno_t ret = 0 ; //distancePoint(pos);

			return ret;
		}

		inline void Recovery(){

		}
		/*
		inline errno_t GetMotorStatus(uint32_t& state) {
			errno_t ret = ERROR_SUCCESS;

			return ret;
		}



		inline errno_t GetAlarmCode(uint32_t& code) {
			errno_t ret = ERROR_SUCCESS;

			return ret;
		}

		 */
		inline errno_t GetMotorData(){
			return m_cfg.p_comm->RequestMotorData(m_nodeId);
		}


#if 0
		inline errno_t GetMotorData(moons_data_t& rf_data) {
			errno_t ret = ERROR_SUCCESS;
			if (m_cfg.pComm->IsOpened() == false)
				return -1;
			using func_e = moons_modbus_func_e;
			using reg_e = moons_SS_reg_e;

			constexpr int array_max = 6;
			uint8_t  func = static_cast<uint8_t>(func_e::read_HoldingReg);
			uint16_t regist_no = static_cast<uint16_t>(reg_e::Alarm_Code_AL_f);
			uint16_t data_cnt = 12;
			std::array<uint8_t, array_max> send_data =
					{
							m_cfg.AxisId,
							func,
							(uint8_t)(regist_no >> 8),
							(uint8_t)regist_no ,
							(uint8_t)(data_cnt >> 8),
							(uint8_t)data_cnt
					};

			m_waitResp = true;
			constexpr uint32_t timeout = 300;

			errno_t result = m_cfg.pComm->SendCmdRxResp(send_data.data(), (uint32_t)send_data.size(), timeout);
			if (result == ERROR_SUCCESS)
			{
				bool ok_recv = true;
				uint32_t pre_ms = UTL::millis();
				while (m_waitResp)
				{
					if (UTL::millis() - pre_ms > timeout)
					{
						ok_recv = false;
						m_waitResp = false;
						ret = -1;
					}
					Sleep(10);
				}
				if (ok_recv)
				{
					uint8_t size = (uint8_t)m_vBuffer.size();
					if (size == 24)
					{

						m_mtAlarmCode.al_status = (uint16_t)(m_vBuffer[0] << 8) | (uint16_t)(m_vBuffer[1] << 0);
						m_mtStatus.sc_status = (uint16_t)(m_vBuffer[2] << 8) | (uint16_t)(m_vBuffer[3] << 0);
						rf_data.al_code = m_mtAlarmCode;
						rf_data.drv_status = m_mtStatus;
						rf_data.immediate_expanded_input = (uint16_t)(m_vBuffer[4] << 8) | (uint16_t)(m_vBuffer[5] << 0);
						rf_data.driver_board_inputs = (uint16_t)(m_vBuffer[6] << 8) | (uint16_t)(m_vBuffer[7] << 0);
						rf_data.encoder_position = (uint32_t)(m_vBuffer[8] << 24) | (uint32_t)(m_vBuffer[9] << 16) | (uint32_t)(m_vBuffer[10] << 8) | (uint32_t)(m_vBuffer[11] << 0);
						rf_data.immediate_abs_position = (uint32_t)(m_vBuffer[12] << 24) | (uint32_t)(m_vBuffer[13] << 16) | (uint32_t)(m_vBuffer[14] << 8) | (uint32_t)(m_vBuffer[15] << 0);
						rf_data.abs_position_command = (uint32_t)(m_vBuffer[16] << 24) | (uint32_t)(m_vBuffer[17] << 16) | (uint32_t)(m_vBuffer[18] << 8) | (uint32_t)(m_vBuffer[19] << 0);
						rf_data.immediate_act_velocity = (uint16_t)(m_vBuffer[20] << 8) | (uint16_t)(m_vBuffer[21] << 0);
						rf_data.immediate_target_velocity = (uint16_t)(m_vBuffer[22] << 8) | (uint16_t)(m_vBuffer[23] << 0);
					}

					m_vBuffer.clear();
				}
			}
			return ret;
		}




		inline errno_t GetPosData(moons_pos_data_t& rf_data) {
			errno_t ret = ERROR_SUCCESS;
			if (m_cfg.pComm->IsOpened() == false)
				return -1;

			using func_e = moons_modbus_func_e;
			using reg_e = moons_SS_reg_e;

			constexpr int array_max = 6;
			uint8_t  func = static_cast<uint8_t>(func_e::read_HoldingReg);
			uint16_t regist_no = static_cast<uint16_t>(reg_e::Encoder_Position_IE_EP_e);
			uint16_t data_cnt = 8;
			std::array<uint8_t, array_max> send_data =
					{
							m_cfg.AxisId,
							func,
							(uint8_t)(regist_no >> 8),
							(uint8_t)regist_no ,
							(uint8_t)(data_cnt >> 8),
							(uint8_t)data_cnt
					};

			constexpr uint32_t timeout = 300;
			m_waitResp = true;
			errno_t result = m_cfg.pComm->SendCmdRxResp(send_data.data(), (uint32_t)send_data.size(), timeout);
			if (result == ERROR_SUCCESS)
			{
				bool ok_recv = true;
				uint32_t pre_ms = UTL::millis();
				while (m_waitResp)
				{
					if (UTL::millis() - pre_ms > timeout)
					{
						ok_recv = false;
						m_waitResp = false;
						ret = -1;
					}
					Sleep(10);
				}
				if (ok_recv)
				{
					uint8_t size = (uint8_t)m_vBuffer.size();
					if (size == 16)
					{
						rf_data.encoder_position = (uint32_t)(m_vBuffer[0] << 24) | (uint32_t)(m_vBuffer[1] << 16) | (uint32_t)(m_vBuffer[2] << 8) | (uint32_t)(m_vBuffer[3] << 0);
						rf_data.immediate_abs_position = (uint32_t)(m_vBuffer[4] << 24) | (uint32_t)(m_vBuffer[5] << 16) | (uint32_t)(m_vBuffer[6] << 8) | (uint32_t)(m_vBuffer[7] << 0);
						rf_data.abs_position_command = (uint32_t)(m_vBuffer[8] << 24) | (uint32_t)(m_vBuffer[9] << 16) | (uint32_t)(m_vBuffer[10] << 8) | (uint32_t)(m_vBuffer[11] << 0);
						rf_data.immediate_act_velocity = (uint16_t)(m_vBuffer[12] << 8) | (uint16_t)(m_vBuffer[13] << 0);
						rf_data.immediate_target_velocity = (uint16_t)(m_vBuffer[14] << 8) | (uint16_t)(m_vBuffer[15] << 0);
					}

					m_vBuffer.clear();
				}
			}

			return ret;

		}

		inline errno_t  ClearState() {
			errno_t ret = ERROR_SUCCESS;
			if (m_cfg.pComm->IsOpened() == false)
				return -1;

			using func_e = moons_modbus_func_e;
			using reg_e = moons_SS_reg_e;
			using code_e = moons_SS_Opcode_e;

			constexpr int array_max = 6;
			uint8_t  func = static_cast<uint8_t>(func_e::write_SingleReg);
			uint16_t regist_no = static_cast<uint16_t>(reg_e::Command_Opcode);
			uint16_t vlaue = static_cast<uint16_t>(code_e::alarm_reset);
			std::array<uint8_t, array_max> send_data =
					{
							m_cfg.AxisId,
							func,
							(uint8_t)(regist_no >> 8),
							(uint8_t)regist_no,
							(uint8_t)(vlaue >> 8),
							(uint8_t)vlaue
					};

			constexpr uint32_t timeout = 200;
			m_waitResp = true;
			errno_t result = m_cfg.pComm->SendCmdRxResp(send_data.data(), (uint32_t)send_data.size(), timeout);
			if (result == ERROR_SUCCESS)
			{
				bool ok_recv = true;
				uint32_t pre_ms = UTL::millis();
				while (m_waitResp)
				{
					if (UTL::millis() - pre_ms > timeout)
					{
						ok_recv = false;
						m_waitResp = false;
						ret = -1;
					}
					Sleep(10);
				}
				if (ok_recv)
				{
				}
			}


			return ret;
		}

		inline errno_t  ClearPosition() {
			errno_t ret = ERROR_SUCCESS;
			if (m_cfg.pComm->IsOpened() == false)
				return -1;


			return clearEncoder();
		}

#endif



	}; // end of class



} // end of namespace


#endif /* SRC_AP__INC_ENMOTOR_MOONS_HPP_ */
