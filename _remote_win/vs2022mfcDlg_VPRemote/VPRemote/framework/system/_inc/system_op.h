#pragma once

/* ��ü�� ����ϱ� �� ��� door ���� ������ ����� �� �����ؾ� �Ѵ�.*/
namespace AP_SYS
{


	class MSystemOP :public Icommon
	{
	public:
		enum mode_e
		{
			READY,
			AUTORUN,
			STOP,
			DRY_RUN,
		};

		enum status_e
		{
			INIT,
			ERR_STOP,
			STEP_STOP,
			RUN_READY,
			RUN,
		};


		/// <summary>
		/// Jog SW box
		/// </summary>
		struct jog_t
		{
			/// <summary>
			/// [direction]_[sign] 
			/// sign "P"  +  positivity
			/// sign "N"  -  negativity
			/// </summary>
			enum sw_e
			{
				X_P,
				X_N,
				Y_P,
				Y_N,
				Z_P,
				Z_N,
				T_P,
				T_N,
				_max,
			};
			std::array< uint32_t, sw_e::_max> addr_sw{};
			std::array< uint32_t, sw_e::_max> addr_lamp{};

			jog_t() = default;
			virtual ~jog_t() = default;
			inline uint32_t* SetAddrSwInput(
				uint32_t in_xp, uint32_t in_xn
				, uint32_t in_yp = 0, uint32_t in_yn = 0
				, uint32_t in_zp = 0, uint32_t in_zn = 0
				, uint32_t in_tp = 0, uint32_t in_tn = 0) {
				addr_sw = { in_xp ,in_xn,in_yp, in_yn, in_zp, in_zn, in_tp, in_tn };
				return addr_sw.data();
			}
			inline uint32_t* SetAddrLampOutput(
				uint32_t out_xp, uint32_t out_xn
				, uint32_t out_yp = 0, uint32_t out_yn = 0
				, uint32_t out_zp = 0, uint32_t out_zn = 0
				, uint32_t out_tp = 0, uint32_t out_tn = 0) {
				addr_lamp = { out_xp ,out_xn, out_yp, out_yn, out_zp, out_zn, out_tp, out_tn };
				return addr_lamp.data();
			}

			inline void operator = (const jog_t* p_jog) {
				this->addr_sw = p_jog->addr_sw;
				this->addr_lamp = p_jog->addr_lamp;
			}

		};


		/// <summary>
		/// Operating Panel
		/// </summary>
		struct op_t
		{
			enum sw_e
			{
				START,
				STOP,
				RESET,
				ESTOP,
				_max
			};
			std::array< uint32_t, sw_e::_max> addr_sw{};
			std::array< uint32_t, sw_e::_max> addr_lamp{};

			op_t() = default;
			virtual ~op_t() = default;
			inline uint32_t* SetAddrSwInput(uint32_t in_start, uint32_t in_stop, uint32_t in_reset, uint32_t in_estop)
			{
				addr_sw = { in_start , in_stop, in_reset, in_estop };
				return addr_sw.data();
			}
			inline uint32_t* SetAddrLampOutput(uint32_t out_start, uint32_t out_stop, uint32_t out_reset, uint32_t out_estop) {
				addr_lamp = { out_start ,out_stop, out_reset, out_estop };
				return addr_lamp.data();
			}

			inline void operator = (const op_t* p_op) {
				this->addr_sw = p_op->addr_sw;
				this->addr_lamp = p_op->addr_lamp;
			}



		};

		/// <summary>
		/// tower lamp �� alarm buzzer
		/// </summary>
		struct towerlamp_t
		{
			enum out_e
			{
				RED,
				YELLOW,
				GREEN,
				BUZZER,
				_max
			};
			std::array< uint32_t, out_e::_max> addr_out{};

			towerlamp_t() = default;
			virtual ~towerlamp_t() = default;
			towerlamp_t(uint32_t addr_red, uint32_t addr_yellow, uint32_t addr_green, uint32_t addr_buzzer) {
				addr_out = { addr_red ,addr_yellow ,addr_green ,addr_buzzer };
			}
			inline uint32_t* SetAddrOutput(uint32_t addr_red, uint32_t addr_yellow, uint32_t addr_green, uint32_t addr_buzzer) {
				addr_out = { addr_red ,addr_yellow ,addr_green ,addr_buzzer };
				return addr_out.data();
			}

			inline void operator = (const towerlamp_t* p_tower) {
				this->addr_out = p_tower->addr_out;
			}
		};

		/// <summary>
		/// safety door ��...
		/// </summary>
		struct safety_t
		{
			enum door_e
			{
				FRONT,
				_max
			};

			struct door_t {
				uint32_t addr_io{};
				bool is_flagOn{};
				bool is_ActiveLevelHigh{}; // B���� Active level�� ��� false
			};

			struct plc_t
			{
				uint32_t addr_out_door_lock{};
				uint32_t addr_out_door_release{};
				uint32_t addr_out_state_reset{};

				inline void operator = (const plc_t* p_plc) {
					this->addr_out_door_lock = p_plc->addr_out_door_lock;
					this->addr_out_door_release = p_plc->addr_out_door_release;
					this->addr_out_state_reset = p_plc->addr_out_state_reset;
				}
			};


			plc_t plc;
			std::vector<door_t> doors;
			safety_t() :doors(door_e::_max) {

			}

			virtual ~safety_t() = default;

			inline errno_t SetData_DoorSensor(door_e door_idx, door_t& data) {
				doors[door_idx] = data;
				return ERROR_SUCCESS;
			}


			inline errno_t SetData_PLC(plc_t& data) {
				plc = data;
				return ERROR_SUCCESS;
			}

		};


		// air utility check �� �ý��� � �⺻�� �ʿ��� ��ü�� �����Ͽ�  cfg�� ���  

		struct cfg_t
		{
			uint32_t init_blick_ms{};
			uint32_t error_blick_ms{};
			jog_t jog_box{};
			op_t  op_panel{};
			towerlamp_t tower{};
			safety_t safe_door{};
			std::shared_ptr<IIO> pIo{};
			AP_DATA::GConfig_dat* p_cfg_data{};

			cfg_t()
			{
				init_blick_ms = 3000;
				error_blick_ms = 1000;
				jog_box;
				op_panel;
				tower;
				safe_door;
				pIo = nullptr;
				p_cfg_data = nullptr;
			}

			inline void operator = (const cfg_t* p_cfg) {
				this->jog_box = p_cfg->jog_box;
				this->op_panel = p_cfg->op_panel;
				this->tower = p_cfg->tower;
				this->pIo = p_cfg->pIo;
			}

		};

	private:
		cfg_t m_cfg;
		MSystemOP::status_e m_status;
		MSystemOP::mode_e m_mode;
		uint32_t m_lampIntervaltime;
		uint8_t m_flag;

	public:
		MSystemOP(common::Attribute_t& common_data, cfg_t& cfg)
			: Icommon(common_data), m_cfg(cfg), m_status(), m_mode()
			, m_flag(), m_lampIntervaltime()
		{
		} /*{
			m_status = MSystemOP::status::INIT;
			m_mode = MSystemOP::mode::STOP;
		}*/
		virtual ~MSystemOP() {}


	private:



	public:
		inline MSystemOP::status_e GetStatus() const {
			return m_status;
		}

		inline void SetStatus(MSystemOP::status_e state) {
			m_status = state;
		}

		inline MSystemOP::mode_e GetMode() const {
			return m_mode;
		}

		inline void SetMode(MSystemOP::mode_e md) {
			m_mode = md;
		}

		inline bool GetPressed(MSystemOP::op_t::sw_e op_sw) {
			if (m_cfg.pIo == nullptr)
			{
				ERR_PRINT("GetPressed m_cfg.pIo == nullptr ");
				return false;
			}

			uint32_t addr = 0;
			switch (op_sw)
			{
			case MSystemOP::op_t::START:
				addr = m_cfg.op_panel.addr_sw[op_t::sw_e::START];
				break;

			case MSystemOP::op_t::STOP:
				addr = m_cfg.op_panel.addr_sw[op_t::sw_e::STOP];
				break;

			case MSystemOP::op_t::RESET:
				addr = m_cfg.op_panel.addr_sw[op_t::sw_e::RESET];
				break;

			case MSystemOP::op_t::ESTOP:
				addr = m_cfg.op_panel.addr_sw[op_t::sw_e::ESTOP];
				break;

			default:
				break;
			}


			return  m_cfg.pIo->IsOn(addr);//gpioPinRead(ch);
		}

		inline errno_t ResetSafetyPLC() {
			if (m_cfg.pIo == nullptr)
			{
				ERR_PRINT("ResetSafetyPLC m_cfg.pIo == nullptr ");
				return false;
			}
			return m_cfg.pIo->OutputOn(m_cfg.safe_door.plc.addr_out_state_reset);
		}

		inline errno_t DoorClose() {
			if (m_cfg.pIo == nullptr)
			{
				ERR_PRINT("DoorClose m_cfg.pIo == nullptr ");
				return false;
			}
			return m_cfg.pIo->OutputOn(m_cfg.safe_door.plc.addr_out_door_lock);
		}

		inline errno_t DoorOpen() {
			if (m_cfg.pIo == nullptr)
			{
				ERR_PRINT("DoorOpen m_cfg.pIo == nullptr ");
				return false;
			}
			return m_cfg.pIo->OutputOn(m_cfg.safe_door.plc.addr_out_door_release);
		}



		inline bool IsSafetyDoorOpened() {
			if (m_cfg.pIo == nullptr)
			{
				ERR_PRINT("IsSafetyDoorOpened m_cfg.pIo == nullptr ");
				return false;
			}

			bool ret = true;
			for (MSystemOP::safety_t::door_t& e : m_cfg.safe_door.doors)
			{
				// �켱 sensor ���� 
				if (e.is_ActiveLevelHigh)
				{
					// A���� ���̽� (�»��� active leve�� GND low )
					ret &= m_cfg.pIo->IsOn(e.addr_io);
				}
				else
				{
					//B ���� ���̽� (�� active leve�� 24V high )
					ret &= m_cfg.pIo->IsOff(e.addr_io);
				}
			}
			return ret;
		}


		inline errno_t SetSWStartLamp(bool on = true) {
			errno_t ret = ERROR_SUCCESS;
			if (on)
				ret = m_cfg.pIo->OutputOn(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::START)]);
			else
				ret = m_cfg.pIo->OutputOff(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::START)]);

			return ret;
		}

		inline errno_t SetSWStopLamp(bool on = true) {
			errno_t ret = ERROR_SUCCESS;
			if (on)
				ret = m_cfg.pIo->OutputOn(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::STOP)]);
			else
				ret = m_cfg.pIo->OutputOff(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::STOP)]);

			return ret;
		}

		inline errno_t SetSWResetLamp(bool on = true) {
			errno_t ret = ERROR_SUCCESS;
			if (on)
				ret = m_cfg.pIo->OutputOn(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::RESET)]);
			else
				ret = m_cfg.pIo->OutputOff(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::RESET)]);

			return ret;
		}


		inline errno_t SetTowerLampGreen(bool on = true) {
			errno_t ret = ERROR_SUCCESS;
			if (on)
				ret = m_cfg.pIo->OutputOn(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::GREEN)]);
			else
				ret = m_cfg.pIo->OutputOff(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::GREEN)]);

			return ret;
		}

		inline errno_t SetTowerLampYellow(bool on = true) {
			errno_t ret = ERROR_SUCCESS;
			if (on)
				ret = m_cfg.pIo->OutputOn(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::YELLOW)]);
			else
				ret = m_cfg.pIo->OutputOff(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::YELLOW)]);

			return ret;
		}

		inline errno_t SetTowerLampRed(bool on = true) {
			errno_t ret = ERROR_SUCCESS;
			if (on)
				ret = m_cfg.pIo->OutputOn(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::RED)]);
			else
				ret = m_cfg.pIo->OutputOff(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::RED)]);

			return ret;
		}


		inline errno_t SetTowerBuzzer(bool on = true) {
			errno_t ret = ERROR_SUCCESS;
			if (on)
				ret = m_cfg.pIo->OutputOn(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::BUZZER)]);
			else
				ret = m_cfg.pIo->OutputOff(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::BUZZER)]);

			return ret;
		}




		inline errno_t ToggleSWStartLamp() {
			return m_cfg.pIo->OutputToggle(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::START)]);
		}

		inline errno_t ToggleSWStopLamp() {
			return m_cfg.pIo->OutputToggle(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::STOP)]);
		}

		inline errno_t ToggleSWResetLamp() {
			return m_cfg.pIo->OutputToggle(m_cfg.op_panel.addr_lamp[static_cast<int>(MSystemOP::op_t::sw_e::RESET)]);
		}


		inline errno_t ToggleTowerLampGreen() {
			return m_cfg.pIo->OutputToggle(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::GREEN)]);
		}

		inline errno_t ToggleTowerLampYellow() {
			return m_cfg.pIo->OutputToggle(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::YELLOW)]);
		}

		inline errno_t ToggleTowerLampRed() {
			return m_cfg.pIo->OutputToggle(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::RED)]);
		}

		inline errno_t ToggleTowerBuzzer() {
			return m_cfg.pIo->OutputToggle(m_cfg.tower.addr_out[static_cast<int>(MSystemOP::towerlamp_t::out_e::BUZZER)]);
		}






		/**
			* OpPanel�� Switch �� Tower Lamp�� On/Off�� �����Ѵ�.
			*
			* @param	iState : On/Off Mode
			*		    Mode                     Start	Stop  Reset	TowerR	TowerY	TowerG	Buzzer
			*       0:INIT (�ʱ�ȭ �䱸 ����)      B      B      B      X      X       X       X
			*		1:Step Stop �Ϸ� ����		  X		 O		X	   O	  X		  X		  X
			*		2:Start(Run Ready) ����		  B		 X		X	   X	  O		  O		  X
			*		3:Run ����					  O		 X		X	   X	  X		  O		  X
			*		4:Step Stop ���� ����		  X		 O		X	   B	  X		  X		  X
			*		5:Error Stop ���� ����		  O		 X		X	   B	  X		  B		  O
			*		6:Cycle Stop ���� ����		  O		 X		X	   X	  X		  B		  X
			*		7:Parts Empty ����			  O		 X		X	   X	  B		  O		  O
			*		  (Operator Call ����)
			* @return	Error Code : 0=SUCCESS, �׿�=Error
			*/
		inline errno_t UpdateOpPanelAndTowerLamp() {

			errno_t ret = ERROR_SUCCESS;
			if (m_cfg.pIo == nullptr)
			{
				ERR_PRINT("IsSafetyDoorOpened m_cfg.pIo == nullptr ");
				return -1;
			}


			switch (m_status)
			{
			case MSystemOP::INIT:
			{
				if (UTL::millis() - m_lampIntervaltime > m_cfg.init_blick_ms)
				{
					m_lampIntervaltime = UTL::millis();
					//m_flag ^= 1; //XOR
					//if (m_flag == 1)		
					bool is_on = ((m_flag++) & 1);
					ret += SetSWStartLamp(is_on);
					ret += SetSWStopLamp(is_on);
					ret += SetSWResetLamp(is_on);
					ret += SetTowerLampGreen(is_on);
					ret += SetTowerLampYellow(is_on);
					ret += SetTowerLampRed(is_on);
				}

			}
			break;
			case MSystemOP::ERR_STOP:
			{
				/*off tower lamp*/
				ret += SetTowerBuzzer(false);
				ret += SetTowerLampYellow(false);

				/*off op panel sw lamp*/
				ret += SetSWStartLamp(false);
				ret += SetSWResetLamp(false);

				/*  bink lamp*/
				if (UTL::millis() - m_lampIntervaltime > m_cfg.error_blick_ms)
				{
					m_lampIntervaltime = UTL::millis();
					if (m_cfg.p_cfg_data->IsUseBuzzer())
					{
						ret += ToggleTowerBuzzer();
					}
					ret += ToggleTowerLampGreen();
					ret += ToggleSWStopLamp();
				}


			}
			break;
			case MSystemOP::STEP_STOP:
			{
				/*off tower lamp*/
				ret += SetTowerBuzzer(false);
				ret += SetTowerLampGreen(false);
				ret += SetTowerLampYellow(false);

				/*off op panel sw lamp*/
				ret += SetSWStartLamp(false);
				ret += SetSWResetLamp(false);

				/*  bink lamp*/
				if (UTL::millis() - m_lampIntervaltime > m_cfg.init_blick_ms)
				{
					m_lampIntervaltime = UTL::millis();
					ret += ToggleSWStopLamp();
					ret += ToggleTowerLampRed();
				}

			}
			break;
			case MSystemOP::RUN_READY:
			{
				/*off tower lamp*/
				ret += SetTowerBuzzer(false);
				ret += SetTowerLampRed(false);
				ret += SetTowerLampYellow(false);

				/*off op panel sw lamp*/
				ret += SetSWStopLamp(false);
				ret += SetSWResetLamp(false);

				/*  bink lamp*/
				if (UTL::millis() - m_lampIntervaltime > m_cfg.init_blick_ms)
				{
					m_lampIntervaltime = UTL::millis();
					ret += ToggleSWStartLamp();
				}

				/*  turn on lamp*/
				ret += SetTowerLampGreen();
			}
			break;
			case MSystemOP::RUN:
			{
				/*off tower lamp*/
				ret += SetTowerBuzzer(false);
				ret += SetTowerLampRed(false);
				ret += SetTowerLampYellow(false);


				/*off op panel sw lamp*/
				ret += SetSWStopLamp(false);
				ret += SetSWResetLamp(false);

				/*  turn on lamp*/
				ret += SetTowerLampGreen();
				ret += SetSWStartLamp();
			}
			break;
			default:
				break;
			}// end of switch

			return ret;
		}



	}; // end of class

}