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
		CYL_PHONE_FORBACK,
		CYL_VINYLHOLD_UPDOWN,
		CYL_VINYL_PUSHBACK,

		CYL_MAX,

	};

	enum VAC
	{
		VAC_VINYL,

		VAC_MAX,
	};

}


namespace HAL
{
	constexpr int MODULE_PEELER_MAX_DATA_LENGTH = 64;
	constexpr int MODULE_PEELER_MAX_PACKET_LENGTH = (MODULE_PEELER_MAX_DATA_LENGTH + 8);

	const uint8_t MODULE_PEELER_CMD_STX = 0x4A;
	const uint8_t MODULE_PEELER_CMD_ETX = 0x4C;

	constexpr uint16_t MODBUS_MULTIPLE_PARAM_VEL = 240;
	constexpr uint16_t MODBUS_MULTIPLE_PARAM_ACC = 6;

	const uint8_t MODULE_PEELER_EEPROM_MCU_CONFIG_DATA_MAX = 8;
	const uint8_t MODULE_PEELER_EEPROM_MCU_SEQUNCE_DATA_MAX = 8;
	const uint8_t MODULE_PEELER_EEPROM_MCU_AXIS_DATA_MAX = 24;
	const uint8_t MODULE_PEELER_EEPROM_MCU_LINKPOSE_DATA_MAX = 8;
	const uint8_t MODULE_PEELER_EEPROM_MCU_CYLINDER_DATA_MAX = 8;
	const uint8_t MODULE_PEELER_EEPROM_MCU_VACUUM_DATA_MAX = 8;
	const uint8_t MODULE_PEELER_EEPROM_MCU_LOG_DATA_MAX = 32;

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

		enum sw_e
		{
			sw_start,
			sw_stop,
			sw_reset,
			sw_max,
		};

		enum eeprom_data_e:uint8_t
		{
			rom_pose_l,
			rom_pose_h,
			rom_config,
			rom_cylinder,
			rom_vacuum,
			rom_sequence,
			rom_max
		};

		//RX  (mcu -> pc) provide information
		enum RX_TYPE :uint8_t
		{
			RX_MCU_STATE_DATA = 0x01,
			RX_MOTOR_DATA = 0x02,
			RX_MOTOR_CFG_MOTION_ORIGIN = 0x03,

			RX_EEPROM_MOTION_DATA_L = 0x11,
			RX_EEPROM_MOTION_DATA_H = 0x12,
			RX_EEPROM_CONFIG_DATA = 0x13,
			RX_EEPROM_CYLINDER_DATA = 0x14,
			RX_EEPROM_VACUUM_DATA = 0x15,
			RX_EEPROM_SEQUNCE_DATA = 0x16,
			RX_EEPROM_LINK_POS_DATA_L = 0x17,
			RX_EEPROM_LINK_POS_DATA_H = 0x18,
			RX_EEPROM_LOG_DATA = 0x19,

			RX_OK_RESPONSE = 0xAA,
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
			CMD_CTRL_MCU_OPTION_REG = 0x13,

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
			CMD_CTRL_MOT_VEL_JOG = 0x2C,

			CMD_CTRL_MOTS_ONOFF = 0x30,
			CMD_CTRL_MOTS_RUN = 0x31,
			CMD_CTRL_MOTS_STOP = 0x32,
			CMD_CTRL_MOTS_REL = 0x33,
			CMD_CTRL_MOT_JOG_STOP = 0x34,
			CMD_CTRL_MOT_LINK_RUN = 0x35,

			CMD_CTRL_MOT_DATA_MOVE_PARAM = 0x3A,
			CMD_CTRL_MOT_DATA_INIT_PARAM = 0x3B,

			CMD_EEPROM_WRITE_MOTOR_POS_DATA_L = 0x40,
			CMD_EEPROM_WRITE_MOTOR_POS_DATA_H = 0x41,
			CMD_EEPROM_WRITE_CFG_DATA = 0x42,
			CMD_EEPROM_WRITE_CYL_DATA = 0x43,
			CMD_EEPROM_WRITE_VAC_DATA = 0x44,
			CMD_EEPROM_WRITE_SEQ_DATA = 0x45,
			CMD_EEPROM_WRITE_LINK_POS_L = 0x46,
			CMD_EEPROM_WRITE_LINK_POS_H = 0x47,

			CMD_EEPROM_READ_MOTOR_POS_DATA = 0x48,
			//0x49,
			CMD_EEPROM_READ_CFG_DATA = 0x4A,
			CMD_EEPROM_READ_CYL_DATA = 0x4B,
			CMD_EEPROM_READ_VAC_DATA = 0x4C,
			CMD_EEPROM_READ_SEQ_DATA = 0x4D,
			CMD_EEPROM_READ_LINK_POS = 0x4E,
			CMD_EEPROM_READ_LOG = 0x4F,

			CMD_EEPROM_CLEAR_ROM_DATA = 0x50,
			CMD_EEPROM_RELOAD_ROM_DATA = 0x51,

			CMD_READ_MCU_DATA = 0x60,
			CMD_READ_MOTOR_DATA = 0x61,
			CMD_READ_MOTION_ORG_CFG_DATA = 0x62,

			CMD_DO_JOB_TASK_1 = 0x80,
			CMD_DO_JOB_TASK_2 = 0x81,
			CMD_DO_JOB_TASK_3 = 0x82,
			CMD_DO_JOB_TASK_4 = 0x83,
			CMD_DO_JOB_TASK_5 = 0x84,
			CMD_DO_JOB_TASK_6 = 0x85,
			CMD_DO_JOB_TASK_7 = 0x86,
			CMD_DO_JOB_TASK_8 = 0x87,




			CMD_OK_RESPONSE = 0xAA,
		};



		enum mcu_err_e
		{
			no_error,
			no_response_lcd,
			no_response_mot,
			reserved_03,
			reserved_04,
			reserved_05,
			reserved_06,
			motor_not_ready,

			motor_limit,
			reserved_11,
			reserved_12,
			reserved_13,
			cyl_timeout,
			vac_timeout,
			move_timout,
			seq_timeout,
			_max
		};
		enum mcu_state_e
		{
			AUTO_RUNNING,
			EMG_STOP,
			AUTO_READY,
			AUTO_STOP,
			DETECT_AREA_SEN,
			MOTOR_ON,
			ORG_COMPLETED,
			ALARM_STATUS,

			STATE_10,
			STATE_11,
			STATE_12,
			STATE_13,
			STATE_14,
			STATE_15,
			REQUEST_INITIAL,
			ALL_CHECK_OK,
		};

		enum mcu_option_e
		{
			USE_BEEP,
			USE_DRYRUN,
			SKIP_DOORSENSOR,
			SKIP_SAFETYSENSOR,
			OPTION_04,
			OPTION_05,
			OPTION_06,
			OPTION_07,

			OPTION_10,
			OPTION_11,
			OPTION_12,
			OPTION_13,
			OPTION_14,
			OPTION_15,
			OPTION_16,
			OPTION_17,
		};

		enum in_e// in_name
		{
			in_vinyl_vac_on,
			in_jig_on_phone,
			in_mode_key_manual,
			in_modde_key_auto,
			in_04,
			in_05,
			in_06,
			in_safety_sensor, //

			in10, // reserved
			in11,
			in12,
			in13,
			in14,
			in15,
			in16,
			in17,

			in_cyl_jig_for, // ext_io 1
			in_cyl_jig_back,
			in_cyl_jig_open,
			in_cyl_jig_close,
			in_cyl_vinyl_push,
			in_cyl_vinyl_back,
			in_cyl_vinylhold_up,
			in_cyl_vinylhold_down,

			in_30,
			in_grip_vinyle_detect,
			in32,
			in33,
			in34,
			in35,
			in36,
			in37,

		};

		enum out_e//out_name
		{
			out00,
			out01,
			out02,
			out03,
			out_jig_axis_dir,
			out_jig_axis_pulse,
			out_roll_axis_dir,
			out_roll_axis_pulse,

			out10, // reserved
			out11,
			out12,
			out13,
			out14,
			out15,
			out16,
			out17,

			out_cyl_jig_for, // ext_io 1
			out_cyl_jig_back,
			out_cyl_jig_open,
			out_cyl_jig_close,
			out_cyl_vinyl_push,
			out_cyl_vinyl_back,
			out_cyl_vinylhold_up,
			out_cyl_vinylhold_down,

			out_vac_detect_on,
			out_vac_detect_blow,
			out32,
			out33,
			out34,
			out35,
			out36,
			out37,
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
			bool is_tx{};
			uint8_t   rx_type{};
			uint8_t   obj_id{};
			uint16_t  data_length{};
			uint32_t  resp_ms{};

			uint16_t checksum{};
			uint16_t rx_checksum{};
			std::array<uint8_t, MODULE_PEELER_MAX_PACKET_LENGTH> buffer{};
			uint8_t buffer_idx{};
			uint8_t* data{};
			uint16_t data_cnt{};

			UTL::stateMachinep_st state{};
			packet_st() {}

			packet_st& operator = (const packet_st& dat) {
				is_tx = dat.is_tx;
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

		struct mcu_config_dat_st
		{
			uint16_t  parm1;
			uint16_t  parm2;
		};

		struct mcu_sequence_dat_st
		{
			uint16_t  parm1;
			uint16_t  parm2;
		};

		struct mcu_axis_dat_st
		{
			int       cmd_pos{};
			uint32_t  cmd_vel{};
		};

		struct mcu_link_pos_dat_st
		{
			int       cmd_pos{};
			uint32_t  cmd_vel{};
		};

		struct mcu_cyl_dat_st
		{
			uint16_t timeout{};
			uint16_t on_settling{};
		};

		struct mcu_vac_dat_st
		{
			uint16_t timeout{};
			uint16_t on_settling{};
		};


		struct mcu_log_dat_st {
			struct head_st {
				uint8_t  header{};
				uint8_t  error_no{};
				uint8_t  obj_id{};
				uint8_t  step_no{};

				head_st() = default;

				head_st& operator = (const head_st& dat) {
					this->header = dat.header;
					this->error_no = dat.error_no;
					this->obj_id = dat.obj_id;
					this->step_no = dat.step_no;
					return *this;
				}
			};

			struct dat_t {
				head_st head{};
				std::vector<char> log_v{};

				dat_t() = default;

				dat_t& operator = (const dat_t& dat) {
					head = dat.head;
					log_v = dat.log_v;
					return *this;
				}

			};


			head_st head{};
			dat_t data{};
		};

	private:
		cfg_t  m_cfg;
		packet_st m_packetData;

		void* m_cbObj;
		evt_cb m_func;

		bool m_TrdLife;
		HANDLE m_hThread;
		DWORD m_TrdId;
		std::mutex m_mutex;
	public:
		AP_ERR_REG m_errorReg;
		AP_OPT_REG m_optionReg;
		AP_STATE_REG m_stateReg;
		uint32_t m_inReg;
		uint32_t m_outReg;
		uint8_t m_currStep;
		uint8_t m_logHead;
		uint8_t m_logTail;
		bool m_waitResp;

		std::array<moons_data_st, MCU_OBJ::MOTOR_MAX> m_motorData;
		std::array<motion_param_st, MCU_OBJ::MOTOR_MAX> m_motionParam;
		std::array<origin_param_st, MCU_OBJ::MOTOR_MAX>  m_originParam;
		std::array<mcu_config_dat_st, MODULE_PEELER_EEPROM_MCU_CONFIG_DATA_MAX> m_mcuConfigDat;
		std::array<mcu_sequence_dat_st, MODULE_PEELER_EEPROM_MCU_SEQUNCE_DATA_MAX> m_mcuSequnceDat;
		std::array<mcu_axis_dat_st, MODULE_PEELER_EEPROM_MCU_AXIS_DATA_MAX> m_mcuAxisDat;
		std::array<mcu_link_pos_dat_st, MODULE_PEELER_EEPROM_MCU_LINKPOSE_DATA_MAX> m_mcuLinkPoseDat;
		std::array<mcu_cyl_dat_st, MODULE_PEELER_EEPROM_MCU_CYLINDER_DATA_MAX> m_mcuCylinderDat;
		std::array<mcu_vac_dat_st, MODULE_PEELER_EEPROM_MCU_VACUUM_DATA_MAX> m_mcuVacuumDat;
		std::array<mcu_log_dat_st, MODULE_PEELER_EEPROM_MCU_LOG_DATA_MAX> m_mcuLogDat;
		//mcu_log_dat_st m_mcuLogDat;
		UTL::_que <mcu_log_dat_st*> m_queMcuLog;

		std::array<bool, rom_max> m_WaitData;
		/****************************************************
		 *	Constructor
		 ****************************************************/
	public:
		ModulePeeler(common::Attribute_t common_data, ModulePeeler::cfg_t& cfg)
			:Icommon(common_data), m_cfg(cfg), m_packetData{}, m_cbObj{}, m_func{}, m_TrdLife{}, m_hThread{}, m_TrdId{}
			, m_errorReg{}, m_optionReg{}, m_stateReg{}, m_inReg{}, m_outReg{}
			, m_currStep{}, m_logHead{}, m_logTail{}, m_waitResp {}
			, m_motorData{}, m_motionParam {}, m_originParam{}, m_mcuConfigDat{}, m_mcuSequnceDat{}, m_mcuAxisDat{}, m_mcuLinkPoseDat{}
			, m_mcuCylinderDat{}, m_mcuVacuumDat{}, m_mcuLogDat{}, m_queMcuLog{}, m_WaitData {}
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
			std::lock_guard<std::mutex> lock(m_mutex);

			std::cout << "RxData.Type : " << std::hex << (int)m_packetData.rx_type << std::endl;
			
			if (m_packetData.rx_type != RX_OK_RESPONSE)
				Ret_OkResponse();
			else
				return;

			auto ret_data = [&](auto offset, auto& dest)->uint8_t
			{
				memcpy(&dest, &m_packetData.data[offset], sizeof(dest));
				return (uint8_t)(offset + sizeof(dest));
			};


			auto ret_ushort = [&](auto a) ->uint16_t {
				return (m_packetData.data[a] | (m_packetData.data[a + 1] << 8));
			};

			auto ret_uint = [&](auto a)->uint32_t {
				return (
					(m_packetData.data[a] << 0)
					| (m_packetData.data[a + 1] << 8)
					| (m_packetData.data[a + 2] << 16)
					| (m_packetData.data[a + 3] << 24));
			};

			auto ret_int = [&](auto a) ->int {
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
				uint8_t idx = 0;
				idx = ret_data(idx, m_stateReg.ap_state);
				idx = ret_data(idx, m_optionReg.ap_option);
				idx = ret_data(idx, m_errorReg.ap_error);
				idx = ret_data(idx, m_inReg);
				idx = ret_data(idx, m_outReg);
				idx = ret_data(idx, m_currStep);
				idx = ret_data(idx, m_logHead);
				idx = ret_data(idx, m_logTail);
			}
			break;
			case RX_MOTOR_DATA:
			{
				if (m_packetData.obj_id < MCU_OBJ::MOTOR_MAX)
				{
					uint8_t idx = 0;
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].drv_status.sc_status);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].al_code.al_status);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].immediate_expanded_input);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].driver_board_inputs);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].encoder_position);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].immediate_abs_position);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].abs_position_command);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].immediate_act_velocity);
					idx = ret_data(idx, m_motorData[m_packetData.obj_id].immediate_target_velocity);
				}

			}
			break;
			case RX_MOTOR_CFG_MOTION_ORIGIN:
			{

				if (m_packetData.obj_id < MCU_OBJ::MOTOR_MAX)
				{

					uint8_t idx = 0;
					idx = ret_data(idx, m_motionParam[m_packetData.obj_id].jog_accelC);
					idx = ret_data(idx, m_motionParam[m_packetData.obj_id].jog_speedC);
					idx = ret_data(idx, m_motionParam[m_packetData.obj_id].move_accelC);
					idx = ret_data(idx, m_motionParam[m_packetData.obj_id].move_speedC);
					idx = ret_data(idx, m_originParam[m_packetData.obj_id].accel);
					idx = ret_data(idx, m_originParam[m_packetData.obj_id].home_x_no);
					idx = ret_data(idx, m_originParam[m_packetData.obj_id].home_x_level);
					idx = ret_data(idx, m_originParam[m_packetData.obj_id].find_home_dir);
					idx = ret_data(idx, m_originParam[m_packetData.obj_id].speed);

				}
			}
			break;

			case RX_EEPROM_MOTION_DATA_L:
			{
				m_WaitData[rom_pose_l] = false;
				if (m_packetData.obj_id < MCU_OBJ::MOTOR_MAX)
				{
					m_mcuAxisDat[0 + (8 * m_packetData.obj_id)] = { ret_int(0), ret_uint(4) };
					m_mcuAxisDat[1 + (8 * m_packetData.obj_id)] = { ret_int(8), ret_uint(12) };
					m_mcuAxisDat[2 + (8 * m_packetData.obj_id)] = { ret_int(16), ret_uint(20) };
					m_mcuAxisDat[3 + (8 * m_packetData.obj_id)] = { ret_int(24), ret_uint(28) };
				}
			}
			break;
			case RX_EEPROM_MOTION_DATA_H:
			{
				m_WaitData[rom_pose_h] = false;
				if (m_packetData.obj_id < MCU_OBJ::MOTOR_MAX)
				{
					m_mcuAxisDat[4 + (8 * m_packetData.obj_id)] = { ret_int(0), ret_uint(4) };
					m_mcuAxisDat[5 + (8 * m_packetData.obj_id)] = { ret_int(8), ret_uint(12) };
					m_mcuAxisDat[6 + (8 * m_packetData.obj_id)] = { ret_int(16), ret_uint(20) };
					m_mcuAxisDat[7 + (8 * m_packetData.obj_id)] = { ret_int(24), ret_uint(28) };
				}

			}
			break;
			case RX_EEPROM_CONFIG_DATA:
			{
				m_WaitData[rom_config] = false;
				for (int i = 0; i < MODULE_PEELER_EEPROM_MCU_CONFIG_DATA_MAX; i++)
				{
					m_mcuConfigDat[i] = { 
						ret_ushort(0 + (i * sizeof(mcu_config_dat_st)))
					, ret_ushort(2 + (i * sizeof(mcu_config_dat_st))) };
				}

			}
			break;

			case RX_EEPROM_CYLINDER_DATA:
			{
				m_WaitData[rom_cylinder] = false;
				for (int i = 0; i < MODULE_PEELER_EEPROM_MCU_CYLINDER_DATA_MAX; i++)
				{
					m_mcuCylinderDat[i] = { 
						ret_ushort(0 + (i * sizeof(mcu_cyl_dat_st)))
					, ret_ushort(2 + (i * sizeof(mcu_cyl_dat_st))) 
					};
				}

			}
			break;

			case RX_EEPROM_VACUUM_DATA:
			{
				m_WaitData[rom_vacuum] = false;
				for (int i = 0; i < MODULE_PEELER_EEPROM_MCU_VACUUM_DATA_MAX; i++)
				{
					m_mcuVacuumDat[i] = { 
						ret_ushort(0 + (i * sizeof(mcu_vac_dat_st)))
					, ret_ushort(2 + (i * sizeof(mcu_vac_dat_st)))
					};
				}

			}
			break;
			case RX_EEPROM_SEQUNCE_DATA:
			{
				m_WaitData[rom_sequence] = false;
				for (int i = 0; i < MODULE_PEELER_EEPROM_MCU_SEQUNCE_DATA_MAX; i++)
				{
					m_mcuSequnceDat[i] = { 
						ret_ushort(0 + (i * sizeof(mcu_sequence_dat_st)))
					, ret_ushort(2 + (i * sizeof(mcu_sequence_dat_st)))
					};
				}

			}
			break;

			case RX_EEPROM_LINK_POS_DATA_L:
			{
				m_mcuLinkPoseDat[0] = { ret_int(0), ret_uint(4) };
				m_mcuLinkPoseDat[1] = { ret_int(8), ret_uint(12) };
				m_mcuLinkPoseDat[2] = { ret_int(16), ret_uint(20) };
				m_mcuLinkPoseDat[3] = { ret_int(24), ret_uint(28) };
			}
			break;

			case RX_EEPROM_LINK_POS_DATA_H:
			{
				m_mcuLinkPoseDat[4] = { ret_int(0), ret_uint(4) };
				m_mcuLinkPoseDat[5] = { ret_int(8), ret_uint(12) };
				m_mcuLinkPoseDat[6] = { ret_int(16), ret_uint(20) };
				m_mcuLinkPoseDat[7] = { ret_int(24), ret_uint(28) };
			}
			break;

			case RX_EEPROM_LOG_DATA:
			{
				uint8_t idx = 0;
				idx = ret_data(idx, m_mcuLogDat[m_packetData.obj_id]);
				m_queMcuLog.Put(&m_mcuLogDat[m_packetData.obj_id]);
				//std::vector<char> vdata{};
				//for (uint8_t i = 0; i < m_packetData.data_length; i++)
				//{
				//	vdata.emplace_back(m_packetData.data[i]);
				//}

				//m_queMcuLog.Put(vdata);
			}
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

			std::lock_guard<std::mutex> lock(m_mutex);

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
					m_packetData.state.SetStep(STATE_WAIT_CHECKSUM_L);
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
			//std::lock_guard<std::mutex> lock(m_mutex);
			if (m_waitResp)
				return -1;
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

			if (m_cbObj)
			{
				packet_st send_data{};			 
				for (auto& elm : datas)
				{
					send_data.buffer[send_data.buffer_idx] = elm;
					send_data.buffer_idx++;
				}
				send_data.is_tx = true;
				//m_func(m_cbObj, nullptr, &send_data);
			}
			std::cout << "TxData.Datas: ";
			for (auto& i : datas) {
				std::cout << std::hex <<(int)i << " ";
			}
			std::cout << std::endl;

			return m_cfg.p_Comm->SendData((char*)datas.data(), (uint32_t)datas.size());

		}

		inline bool SendCmdRxResp(uint8_t* p_data, uint32_t length, uint32_t timeout = 200) {
			if (SendCmd(p_data, length) > 0)
			{
				uint32_t pre_ms = UTL::millis();

				m_waitResp = true;
				while (m_waitResp)
				{
					if ((UTL::millis() - pre_ms) > timeout)
					{
						m_waitResp = false;
						return false;
					}

					Sleep(50);
				}
				return true;
			}
			return false;
		}

		inline bool SendCmdRxResp(uint8_t* p_data, uint32_t length, eeprom_data_e type, uint32_t timeout = 200) {
			bool ret = false;
			if (SendCmd(p_data, length) > 0)
			{
				uint32_t pre_ms = UTL::millis();
				ret = true;
				m_WaitData[type] = true;
				while (m_WaitData[type])
				{
					if ((UTL::millis() - pre_ms) > timeout)
					{
						ret = false;
						break;
					}
					Sleep(50);
				}
			}
			return ret;
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

		inline errno_t GetLogMsg(uint8_t log_idx) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_READ_LOG);
			//obj_id
			datas.emplace_back(log_idx);
			//length
			datas.emplace_back((uint8_t)sizeof(log_idx));
			datas.emplace_back(0x00);
			//data - log addr
			datas.emplace_back(log_idx);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
				return ERROR_SUCCESS;
			else
				return -1;
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


		inline errno_t ReadROMData_MotorPos(MCU_OBJ::MOTOR obj_id) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_READ_MOTOR_POS_DATA);
			//obj_id
			datas.emplace_back((uint8_t)obj_id);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			m_WaitData[rom_pose_h] = true;
			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_LinkPos() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_READ_LINK_POS);
			//obj_id
			datas.emplace_back(0);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_ConfigData() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_READ_CFG_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_CylinderData() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_READ_CYL_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_VacuumData() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_READ_VAC_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReadROMData_SequenceData() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_READ_SEQ_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(),500))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ReloadROMData() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_EEPROM_RELOAD_ROM_DATA);
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
			datas.emplace_back(CMD_EEPROM_CLEAR_ROM_DATA);
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

	

		inline errno_t WriteROM_CyliderData(std::vector<uint8_t>& vdatas) {
			std::vector<uint8_t> datas{};
			uint16_t length = (uint16_t)vdatas.size();
			datas.emplace_back(CMD_EEPROM_WRITE_CYL_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(length >> 0);
			datas.emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas.emplace_back(vdatas[i]);
			}

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t WriteROM_VacuumData(std::vector<uint8_t>& vdatas) {
			std::vector<uint8_t> datas{};
			uint16_t length = (uint16_t)vdatas.size();
			datas.emplace_back(CMD_EEPROM_WRITE_VAC_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(length >> 0);
			datas.emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas.emplace_back(vdatas[i]);
			}

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t WriteROM_SequenceData(std::vector<uint8_t>& vdatas) {
			std::vector<uint8_t> datas{};
			uint16_t length = (uint16_t)vdatas.size();
			datas.emplace_back(CMD_EEPROM_WRITE_SEQ_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(length >> 0);
			datas.emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas.emplace_back(vdatas[i]);
			}

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t WriteROM_ConfigData(std::vector<uint8_t>& vdatas) {
			std::vector<uint8_t> datas{};
			uint16_t length = (uint16_t)vdatas.size();
			datas.emplace_back(CMD_EEPROM_WRITE_CFG_DATA);
			//obj_id
			datas.emplace_back(0x00);
			//length
			datas.emplace_back(length >> 0);
			datas.emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas.emplace_back(vdatas[i]);
			}

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}


		inline errno_t WriteROM_PosData(std::vector<uint8_t>& vdatas, MCU_OBJ::MOTOR obj_id) {
			enum { low, high, max };
			std::vector<uint8_t> datas[max]{};
			uint16_t length = (uint16_t)(vdatas.size() / 2);
			datas[low].emplace_back(CMD_EEPROM_WRITE_MOTOR_POS_DATA_L);
			datas[high].emplace_back(CMD_EEPROM_WRITE_MOTOR_POS_DATA_H);
			//obj_id
			datas[low].emplace_back((uint8_t)obj_id);
			datas[high].emplace_back((uint8_t)obj_id);
			//length
			datas[low].emplace_back(length >> 0);
			datas[low].emplace_back(length >> 8);
			datas[high].emplace_back(length >> 0);
			datas[high].emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas[low].emplace_back(vdatas[i]);
				datas[high].emplace_back(vdatas[length + i]);
			}

			if (SendCmdRxResp(datas[low].data(), (uint32_t)datas->size(), 500))
			{
				if (SendCmdRxResp(datas[high].data(), (uint32_t)datas->size(), 500))
					return ERROR_SUCCESS;
				else
					return -1;
			}
			else
				return -1;

		}

		inline errno_t WriteROM_LinkPosData(std::vector<uint8_t>& vdatas) {
			enum { low, high, max };
			std::vector<uint8_t> datas[max]{};
			uint16_t length = (uint16_t)(vdatas.size() / 2);
			datas[low].emplace_back(CMD_EEPROM_WRITE_LINK_POS_L);
			datas[high].emplace_back(CMD_EEPROM_WRITE_LINK_POS_H);
			//obj_id
			datas[low].emplace_back(0);
			datas[high].emplace_back(0);
			//length
			datas[low].emplace_back(length >> 0);
			datas[low].emplace_back(length >> 8);
			datas[high].emplace_back(length >> 0);
			datas[high].emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas[low].emplace_back(vdatas[i]);
				datas[high].emplace_back(vdatas[length + i]);
			}

			if (SendCmdRxResp(datas[low].data(), (uint32_t)datas->size(), 500))
			{
				if (SendCmdRxResp(datas[high].data(), (uint32_t)datas->size(), 500))
					return ERROR_SUCCESS;
				else
					return -1;
			}
			else
				return -1;

		}


		inline errno_t WriteMoonsParam_Move(std::vector<uint8_t>& vdatas, MCU_OBJ::MOTOR obj_id) {
			std::vector<uint8_t> datas{};
			uint16_t length = (uint16_t)vdatas.size();
			datas.emplace_back(CMD_CTRL_MOT_DATA_MOVE_PARAM);
			//obj_id
			datas.emplace_back((uint8_t)obj_id);
			//length
			datas.emplace_back(length >> 0);
			datas.emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas.emplace_back(vdatas[i]);
			}

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
			{
				return ERROR_SUCCESS;
			}
			else
				return -1;
		}


		inline errno_t WriteMoonsParam_Init(std::vector<uint8_t>& vdatas, MCU_OBJ::MOTOR obj_id) {
			std::vector<uint8_t> datas{};
			uint16_t length = (uint16_t)vdatas.size();
			datas.emplace_back(CMD_CTRL_MOT_DATA_INIT_PARAM);
			//obj_id
			datas.emplace_back((uint8_t)obj_id);
			//length
			datas.emplace_back(length >> 0);
			datas.emplace_back(length >> 8);

			for (uint16_t i = 0; i < length; i++)
			{
				datas.emplace_back(vdatas[i]);
			}

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size(), 500))
			{
				return ERROR_SUCCESS;
			}
			else
				return -1;
		}


		inline errno_t UpdateMcuOpVirtualSw(uint8_t sw) {

			return 0;
		}


		inline bool IsDetectVinyl() {
			return m_inReg & 1 << in_grip_vinyle_detect;
		}

		inline bool IsOpenCyl(MCU_OBJ::CYL obj_id) {
			switch (obj_id)
			{
			case MCU_OBJ::CYL_PHONE_OPENCLOSE:
				return m_inReg & 1 << in_cyl_jig_open;
			case MCU_OBJ::CYL_PHONE_FORBACK:
				return m_inReg & 1 << in_cyl_jig_for;
			case MCU_OBJ::CYL_VINYLHOLD_UPDOWN:
				return m_inReg & 1 << in_cyl_vinylhold_up;
			case MCU_OBJ::CYL_VINYL_PUSHBACK:
				return m_inReg & 1 << in_cyl_vinyl_push;
			default:
				return false;
			}

		}

		inline bool IsCloseCyl(MCU_OBJ::CYL obj_id) {
			return !IsOpenCyl(obj_id);
		}


		inline bool IsGripCyl(MCU_OBJ::CYL obj_id) {
			return (!IsOpenCyl(obj_id) && !IsCloseCyl(obj_id));
		}

		inline bool IsOnVac(MCU_OBJ::VAC obj_id) {
			return m_inReg & 1 << in_vinyl_vac_on;
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
		inline errno_t CylGrip(MCU_OBJ::CYL obj_id) {
			return CylOnOff(obj_id, false);
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

		inline errno_t MoveLinkPose(int pos, int vel) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_LINK_RUN);
			datas.emplace_back(0);
			enum {
				pos1, pos2, pos3, pos4,
				vel1, vel2, vel3, vel4,
				_max
			};
			//length
			datas.emplace_back(uint8_t(_max >> 0));
			datas.emplace_back(uint8_t(_max >> 8));
			//data
			datas.emplace_back(uint8_t(pos >> 0));
			datas.emplace_back(uint8_t(pos >> 8));
			datas.emplace_back(uint8_t(pos >> 16));
			datas.emplace_back(uint8_t(pos >> 24));

			datas.emplace_back(uint8_t(vel >> 0));
			datas.emplace_back(uint8_t(vel >> 8));
			datas.emplace_back(uint8_t(vel >> 16));
			datas.emplace_back(uint8_t(vel >> 24));

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


		inline errno_t JogMove(MCU_OBJ::MOTOR mot_id, std::vector<uint8_t>& vdatas) {
			std::vector<uint8_t> datas{};
			uint16_t length = (uint16_t)vdatas.size();
			datas.emplace_back(CMD_CTRL_MOT_VEL_JOG);
			//obj_id
			datas.emplace_back((uint8_t)mot_id);
			//length
			datas.emplace_back(length >> 0);
			datas.emplace_back(length >> 8);
			//data
			for (uint16_t i = 0; i < length; i++)
			{
				datas.emplace_back(vdatas[i]);
			}
	
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


		inline errno_t SetOptionReg(uint16_t option_reg) {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MCU_OPTION_REG);
			datas.emplace_back(0x00);
			datas.emplace_back((uint8_t)sizeof(option_reg));
			datas.emplace_back(0x00);

			datas.emplace_back((uint8_t)(option_reg >> 0));
			datas.emplace_back((uint8_t)(option_reg >> 8));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t VitualSW(sw_e sw) 	{
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_VIRTUAL_SW);
			datas.emplace_back((uint8_t)sw);
			//length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			//data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;

		}

		inline errno_t RequestInitial() {
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_JOB_INITIAL);
			datas.emplace_back(0x00);
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