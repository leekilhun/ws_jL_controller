#pragma once




#ifdef USE_HW_MODULE_PEELER



namespace MCU_OBJ
{
	enum MOTOR
	{
		MOTOR_JIG,
		MOTOR_ROLL,
		MOTOR_HIGH,
		MOTOR_MAX,

	};

	enum CYL
	{
		CYL_PHONE_OPENCLOSE,
		CYL_PHONE_LOCKUNLOCK,
		CYL_PHONE_FORBACK,
		CYL_VINYL_GRIP,
		CYL_VINYL_PUSH,

		CYL_MAX,

	};

	enum VAC
	{
		VAC_PHONE,

		VAC_MAX,
	};

}


namespace HAL
{
	constexpr int MODULE_PEELER_MAX_DATA_LENGTH = 48;
	constexpr int MODULE_PEELER_MAX_PACKET_LENGTH = (MODULE_PEELER_MAX_DATA_LENGTH + 8);

	const uint8_t MODULE_PEELER_CMD_STX = 0x4A;
	const uint8_t MODULE_PEELER_CMD_ETX = 0x4C;

	constexpr uint16_t MODBUS_MULTIPLE_PARAM_VEL = 240;
	constexpr uint16_t MODBUS_MULTIPLE_PARAM_ACC = 6;

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


	class ModulePeeler :public Icommon
	{
		/****************************************************
		 *	data
		 ****************************************************/
	public:

		//RX  (mcu -> pc) provide information
		enum RX_TYPE :uint8_t
		{
			RX_MCU_STATE_DATA = 0x00,
			RX_MOTOR_DATA = 0x01,
			RX_MOTOR_POS_VEL = 0x02,
		};

		//enum txCmd_e :uint8_t
		//{
		//	TX_CTRL_FIRM = 0x00,
		//	TX_CTRL_ACT = 0x10,
		//	TX_CTRL_MOT = 0x20,
		//	TX_WRITE_DATA = 0x40,
		//	TX_READ_DATA = 0x50,
		//	TX_OK_RESPONSE = 0xAA,
		//};


		//TX(pc->mcu) request information or action
		enum CMD_TYPE :uint8_t
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
			CMD_CTRL_MOT_MOVE = 0x22,
			CMD_CTRL_MOT_STOP = 0x23,
			CMD_CTRL_MOT_JOG = 0x24,
			CMD_CTRL_MOT_LIMIT = 0x25,
			CMD_CTRL_MOT_ZEROSET = 0x26,
			CMD_CTRL_MOT_RELMOVE = 0x27,
			CMD_CTRL_MOT_CLEAR_ALARM = 0x28,
			CMD_CTRL_MOT_CHANGE_VEL = 0x29,
			CMD_CTRL_MOT_MOVE_VEL = 0x2A,
			CMD_CTRL_MOT_RELMOVE_VEL = 0x2B,

			CMD_CTRL_MOTS_ONOFF = 0x30,
			CMD_CTRL_MOTS_RUN = 0x31,
			CMD_CTRL_MOTS_STOP = 0x32,
			CMD_CTRL_MOTS_REL = 0x33,
			CMD_CTRL_MOT_JOG_STOP = 0x34,

			CMD_WRITE_MOTOR_POS_DATA = 0x40,
			CMD_WRITE_CFG_DATA = 0x41,
			CMD_WRITE_CYL_DATA = 0x42,
			CMD_WRITE_VAC_DATA = 0x43,
			CMD_WRITE_SEQ_DATA = 0x44,
			CMD_READ_MOTOR_POS_DATA = 0x45,
			CMD_READ_CFG_DATA = 0x46,
			CMD_READ_CYL_DATA = 0x47,
			CMD_READ_VAC_DATA = 0x48,
			CMD_READ_SEQ_DATA = 0x49,
			CMD_CLEAR_ROM_DATA = 0x4A,
			CMD_RELOAD_ROM_DATA = 0x4B,

			CMD_READ_MCU_DATA = 0x50,
			CMD_READ_MOTOR_DATA =0x51,
			


			CMD_OK_RESPONSE = 0xAA,
		};


		union AP_ERR_REG
		{
			uint32_t ap_error{};
			struct
			{
				unsigned no_error : 1;
				unsigned no_resp_lcd : 1;
				unsigned no_resp_mot : 1;
				unsigned err_03 : 1;
				unsigned err_04 : 1;
				unsigned err_05 : 1;
				unsigned err_state_motor : 1;
				unsigned not_ready_motor : 1;

				unsigned limit_err_motor : 1;
				unsigned cylinder_interlock : 1;
				unsigned err_12 : 1;
				unsigned err_13 : 1;
				unsigned cylinder_timeout : 1;
				unsigned vacuum_timeout : 1;
				unsigned motor_timeout : 1;
				unsigned sequnce_timeout : 1;

				unsigned err_20 : 1;
				unsigned err_21 : 1;
				unsigned err_22 : 1;
				unsigned err_23 : 1;
				unsigned err_24 : 1;
				unsigned err_25 : 1;
				unsigned err_26 : 1;
				unsigned err_27 : 1;

				unsigned err_30 : 1;
				unsigned err_31 : 1;
				unsigned err_32 : 1;
				unsigned err_33 : 1;
				unsigned err_34 : 1;
				unsigned err_35 : 1;
				unsigned err_36 : 1;
				unsigned err_37 : 1;
			};
		};


		union AP_OPT_REG
		{
			uint16_t ap_option{};
			struct
			{
				unsigned use_beep : 1;
				unsigned use_dryrun : 1;
				unsigned skip_doorsensor : 1;
				unsigned skip_safesensor : 1;
				unsigned option_04 : 1;
				unsigned option_05 : 1;
				unsigned option_06 : 1;
				unsigned option_07 : 1;

				unsigned option_10 : 1;
				unsigned option_11 : 1;
				unsigned option_12 : 1;
				unsigned option_13 : 1;
				unsigned option_14 : 1;
				unsigned option_15 : 1;
				unsigned option_16 : 1;
				unsigned option_17 : 1;
			};
		};

		union AP_STATE_REG
		{

			uint16_t ap_state{};
			struct {
				unsigned auto_running : 1;
				unsigned emg_stop : 1;
				unsigned auto_ready : 1;
				unsigned auto_stop : 1;
				unsigned detect_safe_sensor : 1;
				unsigned motor_on : 1;
				unsigned system_origin_cplt : 1;
				unsigned alarm_status : 1;

				unsigned state_10 : 1;
				unsigned state_11 : 1;
				unsigned state_12 : 1;
				unsigned state_13 : 1;
				unsigned state_14 : 1;
				unsigned state_15 : 1;
				unsigned request_initial : 1;
				unsigned all_check_ok : 1;
			};
		};
		/////////////////////////
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




	public:

		struct moons_data_st {

			MOONS_SS_ALARM_STATUS al_code{};
			MOONS_SS_DRIVE_STATUS drv_status{};
			uint16_t immediate_expanded_input{};
			uint16_t driver_board_inputs{};
			uint32_t encoder_position{};
			uint32_t immediate_abs_position{};
			uint32_t abs_position_command{};
			uint16_t immediate_act_velocity{};
			uint16_t immediate_target_velocity{};

			moons_data_st() = default;

			moons_data_st& operator = (const moons_data_st& data) {
				al_code = data.al_code;
				drv_status = data.drv_status;
				immediate_expanded_input = data.immediate_expanded_input;
				driver_board_inputs = data.driver_board_inputs;
				encoder_position = data.encoder_position;
				immediate_abs_position = data.immediate_abs_position;
				abs_position_command = data.abs_position_command;
				immediate_act_velocity = data.immediate_act_velocity;
				immediate_target_velocity = data.immediate_target_velocity;
				return *(this);
			}
		};


		/*
		 * modbus 통신에서
		 * 가감속도는 제품 데이터 (rps*s)에 6배수로 데이터를 보내고
		 * 속도는 제품 데이터(rps)에 240배수 데이터를 보낸다.
		 */
		struct motion_param_st {
			uint32_t jog_speedC{}; //단위 mmsec
			uint32_t jog_accelC{}; //단위 mmsec/ms 목표 속도에 도달할 시간ms
			uint32_t jog_decelC{}; //단위  mmsec/ms

			uint32_t move_speedC{}; //단위 mmsec
			uint32_t move_accelC{}; //단위 mmsec/ms  목표 속도에 도달할 시간ms
			uint32_t move_decelC{}; //단위 mmsec/ms

			//uint32_t limit_velocity{}; //한계 속도 mmsec [max_rps * one_turn_move_mm]

			float max_rps{}; // 초당 최대 회전수 (3'000 RPM ==  50rps)
			uint32_t electronic_gear_ratio{}; // 한 바퀴 필요 카운터
			float one_turn_move_mm{}; // 한 바퀴 구동 시 이동되는 실제 거리 mm  (cmd_pos/turn_per_move)*turn_per_pulse

			motion_param_st() = default;

			void Init() {
				constexpr float default_rps = 10.0;
				constexpr float default_rpss = 100.0;
				constexpr float default_dist_turn = 10.0;// default distance at turn (mm/s) 초당 이동 거리
				constexpr uint32_t defaul_gear_ratio = 20'000;
				one_turn_move_mm = default_dist_turn;//
				jog_speedC = (uint32_t)(default_rps * MODBUS_MULTIPLE_PARAM_VEL);
				jog_accelC = (uint32_t)(default_rpss * MODBUS_MULTIPLE_PARAM_ACC);
				jog_decelC = jog_accelC;

				max_rps = 50.0;// 초당 최대 회전수 (3'000 RPM ==  50rps)
				move_speedC = jog_speedC;
				move_accelC = jog_accelC;
				move_decelC = jog_accelC;
				electronic_gear_ratio = defaul_gear_ratio;
				//limit_velocity = (uint32_t)(one_turn_move_mm * max_rps);
			}


			inline motion_param_st& operator = (const motion_param_st& cfg) {
				if (this != &cfg) {
					this->jog_speedC = cfg.jog_speedC;
					this->jog_accelC = cfg.jog_accelC;
					this->jog_decelC = cfg.jog_decelC;
					this->move_speedC = cfg.move_speedC;
					this->move_accelC = cfg.move_accelC;
					this->move_decelC = cfg.move_decelC;
					//this->limit_velocity = cfg.limit_velocity;

					this->electronic_gear_ratio = cfg.electronic_gear_ratio;
					this->max_rps = cfg.max_rps;
					this->one_turn_move_mm = cfg.one_turn_move_mm;
				}
				return *this;
			}
		};

		struct origin_param_st {
			uint16_t accel{}; //rpss
			uint16_t decel{}; //rpss
			uint16_t speed{}; //rps
			uint32_t offset{};
			char home_x_no{}; // '1' ~ '5'
			int find_home_dir{}; // 1 CW, -1 CCW
			char home_x_level{}; //'L' Low, 'H' High,

			origin_param_st() {
				Init();
			}

			void Init() {
				accel = 100;
				decel = 100;
				speed = 10;
				offset = 0;
				home_x_no = '5'; // home sens
				home_x_level = 'L';
				find_home_dir = 1;
			}

			inline origin_param_st& operator = (const origin_param_st& cfg) {
				if (this != &cfg) {
					this->accel = cfg.accel;
					this->decel = cfg.decel;
					this->speed = cfg.speed;
					this->offset = cfg.offset;

					this->home_x_no = cfg.home_x_no;
					this->find_home_dir = cfg.find_home_dir;
					this->home_x_level = cfg.home_x_level;

				}
				return *this;
			}
			inline void operator = (const origin_param_st* p_cfg) {
				this->accel = p_cfg->accel;
				this->decel = p_cfg->decel;
				this->speed = p_cfg->speed;
				this->offset = p_cfg->offset;

				this->home_x_no = p_cfg->home_x_no;
				this->find_home_dir = p_cfg->find_home_dir;
				this->home_x_level = p_cfg->home_x_level;
			}

		};

		/////////////////////////

		struct cfg_t
		{
			IComm* p_Comm{};
		};


		struct packet_st
		{
			uint8_t   rx_type{};
			uint8_t   obj_id{};
			uint16_t   data_length{};
			uint32_t resp_ms{};

			uint16_t checksum{};
			uint16_t rx_checksum{};
			std::array<uint8_t, MODULE_PEELER_MAX_PACKET_LENGTH> buffer{};
			uint8_t buffer_idx{};
			uint8_t* data{};
			uint16_t data_cnt{};

			UTL::stateMachinep_st state{};
			packet_st() {}

			packet_st& operator = (const packet_st& dat) {
				rx_type = dat.rx_type;
				obj_id = dat.obj_id;
				data_length = dat.data_length;
				resp_ms = dat.resp_ms;
				checksum = dat.checksum;
				rx_checksum = dat.rx_checksum;
				data = dat.data;
				buffer_idx = dat.buffer_idx;
				buffer = dat.buffer;
				data_cnt = dat.data_cnt;
				return *this;
			}

			inline uint8_t BufferAdd(uint8_t rx_data) {
				buffer[buffer_idx] = rx_data;
				buffer_idx++;
				return buffer_idx;
			}

			inline void BufferClear() {
				buffer.fill(0);
				buffer_idx = 0;
				checksum = 0xffff;
				state.SetStep(0);
			}


		};

	private:
		cfg_t  m_cfg;
		packet_st m_packetData;

		void* m_cbObj;
		evt_cb m_func;

		bool m_TrdLife;
		HANDLE m_hThread;
		DWORD m_TrdId;

	public:
		AP_ERR_REG m_errorReg;
		AP_OPT_REG m_optionReg;
		AP_STATE_REG m_stateReg;
		uint32_t m_inReg;
		uint32_t m_outReg;
		bool m_waitResp;

		moons_data_st m_motorData[MCU_OBJ::MOTOR_MAX];

		/****************************************************
		 *	Constructor
		 ****************************************************/
	public:
		ModulePeeler(common::Attribute_t common_data, ModulePeeler::cfg_t& cfg)
			:Icommon(common_data), m_cfg(cfg), m_packetData{}, m_cbObj{}, m_func{}, m_TrdLife{}, m_hThread{}, m_TrdId{}
			, m_errorReg{}, m_optionReg{}, m_stateReg{}, m_inReg{}, m_outReg{}, m_waitResp{}
			, m_motorData{}
		{
			m_cfg.p_Comm->AttCallbackFunc(this, receiveDataCB);

		}

		virtual ~ModulePeeler() {

		}

		/****************************************************
		 *	func
		 ****************************************************/
	public:
		inline void AttCallbackFunc(void* obj, evt_cb cb) {
			m_cbObj = obj;
			m_func = cb;
		}

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
			/*if (m_Que.Available() == false)
			{
				CmdMeasureDist();
			}*/

		}

		inline static UINT threadFunc(LPVOID pParam) {
			ModulePeeler* pThis = (ModulePeeler*)pParam;

			TRACE("ModulePeeler Thread Start\n\r");

			while (pThis->m_TrdLife)
			{
				pThis->threadJob();

				Sleep(10);
			}

			TRACE("ModulePeeler Thread Terminatet\n\r");
			pThis->m_TrdId = 0;
			pThis->m_hThread = NULL;
			return 0;
		}

		inline void processCplt() {
			Ret_OkResponse();

			auto ret_ushort = [&](auto a) {
				return (m_packetData.data[a] | (m_packetData.data[a + 1] << 8));
			};

			auto ret_uint = [&](auto a) {
				return (
					(m_packetData.data[a] << 0)
					| (m_packetData.data[a + 1] << 8)
					| (m_packetData.data[a + 2] << 16)
					| (m_packetData.data[a + 3] << 24));
			};

			m_packetData.resp_ms = UTL::millis() - m_packetData.resp_ms;
			if (m_cbObj)
			{
				//int idx = (int)(m_packetData.buffer_idx);
				//m_func(m_cbObj, &idx, m_packetData.buffer.data());
				packet_st packet_data = m_packetData;
				m_func(m_cbObj, nullptr, &packet_data);
			}
			m_packetData.resp_ms = UTL::millis();

			switch (m_packetData.rx_type)
			{
			case RX_MCU_STATE_DATA:
			{
				m_stateReg.ap_state = ret_ushort(0);
				m_optionReg.ap_option = ret_ushort(2);
				m_errorReg.ap_error = ret_uint(4);
				m_inReg = ret_uint(8);
				m_outReg = ret_uint(12);
			}
			break;
			case RX_MOTOR_DATA:
			{
				if (m_packetData.obj_id < MCU_OBJ::MOTOR_MAX)
				{
					m_motorData[m_packetData.obj_id].drv_status.sc_status = ret_ushort(0);
					m_motorData[m_packetData.obj_id].al_code.al_status = ret_ushort(2);
					m_motorData[m_packetData.obj_id].immediate_expanded_input = ret_ushort(4);
					m_motorData[m_packetData.obj_id].driver_board_inputs = ret_ushort(6);
					m_motorData[m_packetData.obj_id].encoder_position = ret_uint(8);
					m_motorData[m_packetData.obj_id].immediate_abs_position = ret_uint(12);
					m_motorData[m_packetData.obj_id].abs_position_command = ret_uint(16);
					m_motorData[m_packetData.obj_id].immediate_act_velocity = ret_ushort(20);
					m_motorData[m_packetData.obj_id].immediate_target_velocity = ret_ushort(22);
				}
				











					/*
					
			idx = make_packet(idx, motor_data.drv_status.sc_status);//2
			idx = make_packet(idx, motor_data.al_code.al_status);//2
			idx = make_packet(idx, motor_data.immediate_expanded_input);//2
			idx = make_packet(idx, motor_data.driver_board_inputs);//2
			idx = make_packet(idx, motor_data.encoder_position);//4
			idx = make_packet(idx, motor_data.immediate_abs_position);//4
			idx = make_packet(idx, motor_data.abs_position_command);//4
			idx = make_packet(idx, motor_data.immediate_act_velocity);//2
			idx = make_packet(idx, motor_data.immediate_target_velocity);//2
					
					
					*/
			}
			break;
			case RX_MOTOR_POS_VEL:
				break;
			default:
				break;
			}



		}

		inline static void receiveDataCB(void* obj, void* w_parm, void* l_parm) {
			ModulePeeler* p_this = (ModulePeeler*)obj;
			if (w_parm == nullptr)
				return;
			int length = *((int*)w_parm);
			int index = 0;
			
			for (index = 0; index < length; index++)
			{
				uint8_t data = *((uint8_t*)l_parm + index);

				if (p_this->recievePacket(data))
				{
					p_this->m_waitResp = false;
					p_this->processCplt();
				}
			}

		}

		inline bool recievePacket(uint8_t rx_data) {


			//if ((UTL::millis() - m_packetData.state.GetPrevTime()) > 1000)
			if (m_packetData.state.MoreThan(1000))
			{
				m_packetData.BufferClear();
			}



			/*

			 | SOF  | rx_Type | obj_Id| Data Length |Data          |   Checksum   | EOF  |
			 | :--- |:--------|:----- | :---------- |:-------------|:-------------| :--  |
			 | 0x4A | 1 byte  | 1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

			*/


			const uint8_t CMD_STX = 0x4A;
			const uint8_t CMD_ETX = 0x4C;


			const uint8_t STATE_WAIT_STX = 0;
			const uint8_t STATE_WAIT_TYPE = 1;
			const uint8_t STATE_WAIT_ID = 2;
			const uint8_t STATE_WAIT_LENGTH_L = 3;
			const uint8_t STATE_WAIT_LENGTH_H = 4;
			const uint8_t STATE_WAIT_DATA = 5;
			const uint8_t STATE_WAIT_CHECKSUM_L = 6;
			const uint8_t STATE_WAIT_CHECKSUM_H = 7;
			const uint8_t STATE_WAIT_ETX = 8;


			switch (m_packetData.state.GetStep())
			{
			case STATE_WAIT_STX:
				if (rx_data == CMD_STX)
				{
					m_packetData.buffer.fill(0);
					m_packetData.buffer_idx = 0;
					m_packetData.checksum = 0xffff;

					m_packetData.buffer[m_packetData.buffer_idx] = rx_data;
					m_packetData.buffer_idx++;
					m_packetData.state.SetStep(STATE_WAIT_TYPE);
				}
				break;

			case STATE_WAIT_TYPE:
				m_packetData.rx_type = rx_data;
				m_packetData.BufferAdd(rx_data);
				UTL::crc16_update(&m_packetData.checksum, rx_data);
				m_packetData.state.SetStep(STATE_WAIT_ID);
				break;

			case STATE_WAIT_ID:
				m_packetData.obj_id = rx_data;
				m_packetData.BufferAdd(rx_data);
				UTL::crc16_update(&m_packetData.checksum, rx_data);
				m_packetData.state.SetStep(STATE_WAIT_LENGTH_L);
				break;

			case STATE_WAIT_LENGTH_L:
				m_packetData.data_length = rx_data;
				m_packetData.BufferAdd(rx_data);
				UTL::crc16_update(&m_packetData.checksum, rx_data);
				m_packetData.state.SetStep(STATE_WAIT_LENGTH_H);
				break;

			case STATE_WAIT_LENGTH_H:
				m_packetData.data_length |= (uint16_t)(rx_data << 8);
				m_packetData.BufferAdd(rx_data);
				UTL::crc16_update(&m_packetData.checksum, rx_data);
				if (m_packetData.data_length != 0)
				{
					if (m_packetData.data_length < MODULE_PEELER_MAX_DATA_LENGTH)
					{
						m_packetData.data_cnt = 0;
						m_packetData.state.SetStep(STATE_WAIT_DATA);
					}
					else
					{
						m_packetData.state.SetStep(STATE_WAIT_STX);
					}
				}
				else
				{
					m_packetData.state.SetStep(STATE_WAIT_STX);
				}
				break;

			case STATE_WAIT_DATA:
				if (m_packetData.data_cnt == 0)
					m_packetData.data = &m_packetData.buffer[m_packetData.buffer_idx];

				m_packetData.BufferAdd(rx_data);
				UTL::crc16_update(&m_packetData.checksum, rx_data);
				// check receive completed
				m_packetData.data_cnt++;
				if (m_packetData.data_length == m_packetData.data_cnt)
				{
					m_packetData.state.SetStep(STATE_WAIT_CHECKSUM_L);
				}

				break;

			case STATE_WAIT_CHECKSUM_L:
				m_packetData.BufferAdd(rx_data);
				m_packetData.rx_checksum = rx_data;
				m_packetData.state.SetStep(STATE_WAIT_CHECKSUM_H);
				break;

			case STATE_WAIT_CHECKSUM_H:
				m_packetData.BufferAdd(rx_data);
				m_packetData.rx_checksum |= (uint16_t)(rx_data << 8);
				m_packetData.state.SetStep(STATE_WAIT_ETX);
				break;

			case STATE_WAIT_ETX:
				m_packetData.BufferAdd(rx_data);
				if (rx_data == CMD_ETX)
				{
					if (m_packetData.checksum == m_packetData.rx_checksum)
					{
						return true;
					}
				}
				m_packetData.state.SetStep(STATE_WAIT_STX);
				break;

			}
			//switch (m_packet.state)

			return false;
		}



	public:
		inline errno_t Open() {
			errno_t ret = 0;
			ret = m_cfg.p_Comm->Open();

			if (ret == 0)
			{

				threadRun();
			}
			else
			{
				TRACE("fail - module_peeler open\n\r");
				// TRACE("fail - dist sensor open\n\r");
			}
			return ret;
		}


		inline void Close() {
			threadStop();
		}

		inline bool IsOpened() const {
			return m_TrdLife;
		}

		inline void Recovery() {
		}


		inline int SendCmd(uint8_t* p_data, uint32_t length) {
			std::vector<uint8_t> datas{};
			/*

			 | SOF  | Type |funcId| Data Length |Data          |   Checksum   | EOF  |
			 | :--- |:-----|:---- | :---------- |:-------------|:-------------| :--  |
			 | 0x4A |1 byte|1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |
					|-> crc                                            crc <- |
			*/
			uint16_t crc = 0xffff;
			datas.emplace_back(MODULE_PEELER_CMD_STX);
			for (uint32_t i = 0; i < length; i++)
			{
				datas.emplace_back(p_data[i]);
				UTL::crc16_update(&crc, p_data[i]);
			}
			datas.emplace_back((uint8_t)(crc >> 0));
			datas.emplace_back((uint8_t)(crc >> 8));
			datas.emplace_back(MODULE_PEELER_CMD_ETX);

			return m_cfg.p_Comm->SendData((char*)datas.data(), (uint32_t)datas.size());

		}

		inline bool SendCmdRxResp(uint8_t* p_data, uint32_t length, uint32_t timeout = 200) {
			if (SendCmd(p_data, length) > 0)
			{				
				m_waitResp = true;
				uint32_t pre_ms = UTL::millis();
				bool result = true;
				while (m_waitResp)
				{
					if ((UTL::millis() - pre_ms) > timeout)
					{
						return true;
					}
				}
			}
			return false;
		}


		inline void GetMotorData(moons_data_st& ret_data, MCU_OBJ::MOTOR obj_id) {
			ret_data = m_motorData[obj_id];
		}



		inline errno_t Ret_OkResponse() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_OK_RESPONSE);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			return SendCmd(datas.data(), (uint32_t)datas.size());
		}

		inline errno_t GetMcuState() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_MCU_DATA);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			return SendCmd(datas.data(), (uint32_t)datas.size());
		}

		inline errno_t GetMotorState(MCU_OBJ::MOTOR obj_id) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_MOTOR_DATA);
			datas.emplace_back((uint8_t)obj_id);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			return SendCmd(datas.data(), (uint32_t)datas.size());
		}






#if 0
		/* IO */
		inline bool IsOn(uint32_t addr) {

			return false;
		}
		inline bool IsOff(uint32_t addr) {
			return !IsOn(addr);
		}
		inline errno_t OutputOn(uint32_t addr) {
			return 0;
		}
		inline errno_t OutputOff(uint32_t addr) {
			return 0;
		}

		inline errno_t OutputToggle(uint32_t addr) {
			return errno_t();
		}
#endif

		inline errno_t SetIO_Output(uint32_t out_reg) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_IO_OUT);
			datas.emplace_back(0x00);
			datas.emplace_back((uint8_t)sizeof(out_reg));
			datas.emplace_back(0x00);

			datas.emplace_back((uint8_t)(out_reg >> 0));
			datas.emplace_back((uint8_t)(out_reg >> 8));
			datas.emplace_back((uint8_t)(out_reg >> 16));
			datas.emplace_back((uint8_t)(out_reg >> 24));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline void BuzzerOnOff(bool on_off) {

		}

		inline void DryRunOnOff(bool on_off) {

		}

		inline void SkipAreaSensorOnOff(bool on_off) {

		}

		inline void SkipAllSensorOnOff(bool on_off) {

		}

		inline errno_t ReadROMData_MotorPos() {

			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_MOTOR_POS_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_ConfigData() {

			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_CFG_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_CylinderData() {

			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_CYL_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_VacuumData() {

			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_VAC_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_SequenceData() {

			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_SEQ_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ClearROMData() {

			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CLEAR_ROM_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t WriteROMData(uint32_t data_type = 0, void* parm = nullptr) {

	
		}

		inline errno_t UpdateMcuOpVirtualSw(uint8_t sw) {

			return 0;
		}



		inline bool IsOpenCyl(MCU_OBJ::CYL obj_id) {
			return false;
		}
		inline bool IsOnVac(MCU_OBJ::VAC obj_id) {
			return false;
		}
		inline bool IsGripCyl(MCU_OBJ::CYL obj_id) {
			return false;
		}
		inline errno_t CylOnOff(MCU_OBJ::CYL obj_id, bool on_off, bool senser_skip = false) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_CYL);
			datas.emplace_back((uint8_t)obj_id);
			//length
			datas.emplace_back(0x02);
			datas.emplace_back(0x00);
			//data
      datas.emplace_back((uint8_t)(on_off ? 1 : 0));
			datas.emplace_back((uint8_t)(senser_skip ? 1 : 0));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}
		inline errno_t CylGrip(MCU_OBJ::CYL obj_id){
			CylOnOff(obj_id, true);
			return 0;
		}
		inline errno_t VacOnOff(MCU_OBJ::VAC obj_id, bool on_off, bool senser_skip = false) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_VAC);
			datas.emplace_back((uint8_t)obj_id);
			//length
			datas.emplace_back(0x02);
			datas.emplace_back(0x00);
			//data
			datas.emplace_back((uint8_t)(on_off ? 1 : 0));
			datas.emplace_back((uint8_t)(senser_skip ? 1 : 0));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}
		inline errno_t JobInitial() {
			return 0;
		}

		inline void ClearRomData() {

		}
		inline void ReloadRomData() {

		}

		inline errno_t MotorOnOff(MCU_OBJ::MOTOR mot_id, bool on_off) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_ONOFF);
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(0x01);
			datas.emplace_back(0x00);
			//data
			datas.emplace_back((uint8_t)(on_off ? 1 : 0));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t  ClearState(MCU_OBJ::MOTOR mot_id) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_CLEAR_ALARM);
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			//data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}
		inline int RelMove(MCU_OBJ::MOTOR mot_id, int cmd_pos, uint16_t cmd_vel = 100, uint16_t acc = 100) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_RELMOVE);
			datas.emplace_back((uint8_t)mot_id);
			enum {
				pos1, pos2, pos3, pos4,
				vel1, vel2,
				accdec1, accdec2,
				_max
			};
			//length
			datas.emplace_back(uint8_t(_max >> 0));
			datas.emplace_back(uint8_t(_max >> 8));
			//data
			datas.emplace_back(uint8_t(cmd_pos >> 0));
			datas.emplace_back(uint8_t(cmd_pos >> 8));
			datas.emplace_back(uint8_t(cmd_pos >> 16));
			datas.emplace_back(uint8_t(cmd_pos >> 24));

			datas.emplace_back(uint8_t(cmd_vel >> 0));
			datas.emplace_back(uint8_t(cmd_vel >> 8));

			datas.emplace_back(uint8_t(acc >> 0));
			datas.emplace_back(uint8_t(acc >> 8));



			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline int Move(MCU_OBJ::MOTOR mot_id, int cmd_pos, uint16_t cmd_vel = 100, uint16_t acc = 100) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_MOVE);
			datas.emplace_back((uint8_t)mot_id);
			enum {
				pos1,pos2,pos3,pos4,
				vel1,vel2,
				accdec1,accdec2, 
				_max
			};
			//length
			datas.emplace_back(uint8_t(_max >> 0));
			datas.emplace_back(uint8_t(_max >> 8));
			//data
			datas.emplace_back(uint8_t(cmd_pos >> 0));
			datas.emplace_back(uint8_t(cmd_pos >> 8));
			datas.emplace_back(uint8_t(cmd_pos >> 16));
			datas.emplace_back(uint8_t(cmd_pos >> 24));

			datas.emplace_back(uint8_t(cmd_vel >> 0));
			datas.emplace_back(uint8_t(cmd_vel >> 8));

			datas.emplace_back(uint8_t(acc >> 0));
			datas.emplace_back(uint8_t(acc >> 8));



			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t Stop(MCU_OBJ::MOTOR mot_id) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_STOP);
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			//data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t JogMove(MCU_OBJ::MOTOR mot_id, bool is_cw = true) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_JOG);
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(0x01);
			datas.emplace_back(0x00);
			//data
			datas.emplace_back((uint8_t)(is_cw ? 1 : 0));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t JogStop(MCU_OBJ::MOTOR mot_id) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_JOG_STOP);
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			//data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}


		inline errno_t ZereSetMotorEncoder(MCU_OBJ::MOTOR mot_id) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_ZEROSET);
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			//data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t MotorOrigin(MCU_OBJ::MOTOR mot_id) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_ORIGIN);
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			//data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

#if 0


		errno_t ZereSetMotorEncoder(uint8_t id);//(mot_e id = mot_e::_all);
		errno_t MotorOrigin(uint8_t id) //(mot_e id = mot_e::_all);

			// 모터  on/off 통신 결과만 리턴/ 상태 확인은 별도로 
			errno_t MotorsRun(int cmd_pos, uint32_t cmd_vel = 100, uint32_t acc = 100, uint32_t decel = 100, bool is_rel = false);

		errno_t MotorOnOff(mot_e id, bool on_off);
		bool IsMotorOn(mot_e id = mot_e::_all);
		errno_t WaitDone(mot_e id, uint8_t mode);
		bool IsMoving(mot_e id = mot_e::_all);

		bool IsAxisDone(mot_e id = mot_e::_all);
		errno_t  ClearState(mot_e id = mot_e::_all);
		uint32_t  GetMotorState(mot_e id = mot_e::_all);
		int Move(mot_e id, int cmd_pos, uint32_t cmd_vel = 100, uint32_t acc = 100, uint32_t decel = 100);
		//축을 정지한다. (한개의 축에 대한 정지)
		int Stop(mot_e id);
		//축을 등속이동에 대해 정지한다.
		int VStop(mot_e id);
		// 축을 비상정지한다.
		int EStop(mot_e id);

		int JogMove(mot_e id, uint32_t cmd_vel = 1000, bool is_cw = true);
		int JogMove(mot_e id, uint32_t jog_pos, uint32_t cmd_vel = 1000, bool is_cw = true);
		int MoveToLimit(mot_e id, uint32_t cmd_vel = 1000, bool is_plus = true);
		int RelMove(mot_e id, int cmd_pos, uint32_t cmd_vel = 500);
		// open, lock, up state is released by air press(false)

		void GetLastLogMsg(uint8_t* p_data);
		void TestSTressHori(uint32_t cnt);
		void TestSTressVeri(uint32_t cnt);
		void SetRequestMotorID(mot_e id);
		uint8_t GetRequestMotorID() const;


		errno_t ChangeMotorSpeed(mot_e id, uint32_t cmd_vel);




#endif










	};

}
//end of namespace

#endif