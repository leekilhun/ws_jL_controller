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

	constexpr uint16_t MODBUS_MULTIPLE_PARAM_VEL = 240;
	constexpr uint16_t MODBUS_MULTIPLE_PARAM_ACC = 6;
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




	public:

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

			moons_data_t() = default;

			moons_data_t& operator = (const moons_data_t& data){
				immediate_expanded_input	= data.immediate_expanded_input;
				driver_board_inputs 			= data.driver_board_inputs;
				encoder_position 					= data.encoder_position;
				immediate_abs_position 		= data.immediate_abs_position;
				abs_position_command 			= data.abs_position_command;
				immediate_act_velocity 		= data.immediate_act_velocity;
				immediate_target_velocity = data.immediate_target_velocity;
				al_code										= data.al_code;
				drv_status								= data.drv_status;
				return *(this);
			}
		};


		/*
		 * modbus 통신에서
		 * 가감속도는 제품 데이터 (rps*s)에 6배수로 데이터를 보내고
		 * 속도는 제품 데이터(rps)에 240배수 데이터를 보낸다.
		 */
		struct motion_param_t {
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

			motion_param_t() = default;

			void Init() {
				constexpr float default_rps = 10.0;
				constexpr float default_rpss = 100.0;
				constexpr float default_dist_turn = 10.0;// default distance at turn (mm/s) 초당 이동 거리
				constexpr uint32_t defaul_gear_ratio = 20'000;
				one_turn_move_mm = default_dist_turn;//
				jog_speedC = (uint32_t)(default_rps  * MODBUS_MULTIPLE_PARAM_VEL);
				jog_accelC = (uint32_t)(default_rpss * MODBUS_MULTIPLE_PARAM_ACC);
				jog_decelC = jog_accelC;

				max_rps = 50.0;// 초당 최대 회전수 (3'000 RPM ==  50rps)
				move_speedC = jog_speedC;
				move_accelC = jog_accelC;
				move_decelC = jog_accelC;
				electronic_gear_ratio = defaul_gear_ratio;
				//limit_velocity = (uint32_t)(one_turn_move_mm * max_rps);
			}


			inline motion_param_t& operator = (const motion_param_t& cfg) {
				if ( this != &cfg){
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

		struct origin_param_t		{
			uint16_t accel{}; //rpss
			uint16_t decel{}; //rpss
			uint16_t speed{}; //rps
			uint32_t offset{};
			char home_x_no{}; // '1' ~ '5'
			int find_home_dir{}; // 1 CW, -1 CCW
			char home_x_level{}; //'L' Low, 'H' High,

			origin_param_t(){
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

			inline origin_param_t& operator = (const origin_param_t& cfg) {
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
			inline void operator = (const origin_param_t* p_cfg) {
				this->accel = p_cfg->accel;
				this->decel = p_cfg->decel;
				this->speed = p_cfg->speed;
				this->offset = p_cfg->offset;

				this->home_x_no = p_cfg->home_x_no;
				this->find_home_dir = p_cfg->find_home_dir;
				this->home_x_level = p_cfg->home_x_level;
			}

		};


		struct cfg_t {
			motion_param_t motor_param{};
			origin_param_t origin_param{};
			uint8_t instance_no{};
			axis_dat* p_apAxisDat{};
			MCU_REG::ap_reg* p_apReg{};
			ap_dat* p_apCfgDat{};
			uart_moons* p_comm{};

			cfg_t() = default;

			cfg_t& operator = (const cfg_t& cfg){
				if ( this != &cfg){
					instance_no = cfg.instance_no;
					p_comm = cfg.p_comm;
					p_apReg = cfg.p_apReg;
					p_apCfgDat = cfg.p_apCfgDat;
					p_apAxisDat = cfg.p_apAxisDat;
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
		uint8_t m_commErrCnt;
#ifdef _USE_HW_RTOS
		osMutexId m_mutex_id;
#endif

		/****************************************************
		 *  Constructor
		 ****************************************************/
	public:
		enMotor_moons(uint8_t id): m_nodeId(id), m_cfg{}, m_receiveData()
		,m_motorData{}, m_commErrCnt{}
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

		inline void GetMotorData(moons_data_t& ret_data) {
			ret_data = m_motorData;
		}
		inline uint8_t GetInstanceNo() {
			return m_cfg.instance_no;
		}

		inline uint8_t GetErrCnt() const {
			return m_commErrCnt;
		}

		inline uint8_t AddErrCnt()  {
			m_cfg.p_comm->ResetCommFlag();
			return ++m_commErrCnt;
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


		inline errno_t SetMoveDistSpeed(uint32_t vel, uint32_t acc, uint32_t dec, int dist){
			m_cfg.motor_param.move_accelC = acc * MODBUS_MULTIPLE_PARAM_ACC;
			m_cfg.motor_param.move_decelC = dec * MODBUS_MULTIPLE_PARAM_ACC;
			m_cfg.motor_param.move_speedC = vel * MODBUS_MULTIPLE_PARAM_VEL;
			uart_moons::speed_t params {
				(uint16_t)m_cfg.motor_param.move_accelC,
			  (uint16_t)m_cfg.motor_param.move_decelC,
				(uint16_t)m_cfg.motor_param.move_speedC };

			return m_cfg.p_comm->SetMove(m_nodeId, params, dist);
		}


		inline errno_t JogMove(uart_moons::speed_t& param, bool is_cw = true){
			m_cfg.motor_param.jog_accelC = (param.accel *=MODBUS_MULTIPLE_PARAM_ACC);
			m_cfg.motor_param.jog_decelC = (param.decel *=MODBUS_MULTIPLE_PARAM_ACC);
			m_cfg.motor_param.jog_speedC = (param.speed *=MODBUS_MULTIPLE_PARAM_VEL);
			return m_cfg.p_comm->JogMove(m_nodeId,param,is_cw);
		}



		inline errno_t JogMove(bool is_cw = true) {
			uart_moons::speed_t params {
				(uint16_t)m_cfg.motor_param.jog_accelC,
				(uint16_t)m_cfg.motor_param.jog_decelC,
				(uint16_t)m_cfg.motor_param.jog_speedC };
			return m_cfg.p_comm->JogMove(m_nodeId,params,is_cw);
		}

		inline errno_t JogStop() {
			return m_cfg.p_comm->JogStop(m_nodeId);
		}

		inline errno_t MoveStop() {
			return m_cfg.p_comm->MoveStop(m_nodeId);
		}

		inline errno_t MoveRelative(uint32_t vel, uint32_t acc, uint32_t dec, int dist) {
			errno_t ret = ERROR_SUCCESS;
			ret = SetMoveDistSpeed(vel, acc, dec, dist);
			if (ret == ERROR_SUCCESS)
				return m_cfg.p_comm->MoveRelactive(m_nodeId);
			else
				return ret;
		}

		inline errno_t MoveRelative(int pos) {
			errno_t ret = ERROR_SUCCESS;
			ret = m_cfg.p_comm->DistancePoint(m_nodeId, pos);
			if (ret == ERROR_SUCCESS)
				return m_cfg.p_comm->MoveRelactive(m_nodeId);
			else
				return ret;
		}


		inline errno_t MoveAbsolutive(uint32_t vel, uint32_t acc, uint32_t dec, int dist) {
			errno_t ret = ERROR_SUCCESS;
			ret = SetMoveDistSpeed(vel, acc, dec, dist);
			if (ret == ERROR_SUCCESS)
				return m_cfg.p_comm->moveAbsolutive(m_nodeId);
			else
				return ret;
		}

		inline errno_t MoveAbsolutive(int pos) {
			errno_t ret = ERROR_SUCCESS;
			ret = m_cfg.p_comm->DistancePoint(m_nodeId, pos);
			if (ret == ERROR_SUCCESS)
				return m_cfg.p_comm->moveAbsolutive(m_nodeId);
			else
				return ret;
		}

		inline void Recovery(){
			m_commErrCnt = 0;
		}


		inline bool IsCommAlarm() const{
			return m_motorData.al_code.Comm_Error;
		}

		inline errno_t GetMotorData(){
			return m_cfg.p_comm->RequestMotorData(m_nodeId);
		}

		inline errno_t SetOriginParam(origin_param_t& param) {
			m_cfg.origin_param = param;
			m_cfg.origin_param.accel = param.accel;
			m_cfg.origin_param.decel = param.decel;
			m_cfg.origin_param.speed = param.speed;
			uart_moons::speed_t params {
				    (uint16_t)(param.accel * MODBUS_MULTIPLE_PARAM_ACC),
						(uint16_t)(param.decel * MODBUS_MULTIPLE_PARAM_ACC),
						(uint16_t)(param.speed * MODBUS_MULTIPLE_PARAM_VEL)};

			return  m_cfg.p_comm->SetOriginParam(m_nodeId, params, param.find_home_dir);
		}


		inline errno_t OriginMotor(){
			return  m_cfg.p_comm->OriginAxis(m_nodeId, m_cfg.origin_param.home_x_no, m_cfg.origin_param.home_x_level);
		}


		inline errno_t ClearState(){
			return m_cfg.p_comm->ClearState(m_nodeId);
		}

		inline errno_t ClearEncoder(){
			return m_cfg.p_comm->ClearEncoder(m_nodeId);
		}


	}; // end of class



} // end of namespace


#endif /* SRC_AP__INC_ENMOTOR_MOONS_HPP_ */
