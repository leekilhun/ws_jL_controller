#pragma once

/*
 * system_dat.h
 *
 * �ۼ��� : �̱���
 * 22.04.05
 *  data  ��ü
 *
 * 22.10.11 ������ ����
 *
 * ������ json �߰� �� addr_e �߰��� �׸��� �ݿ��� �� 
 * �Լ� GetNetConfig�� �� ������ ��Ī�ϴ� �ڵ带 �־�� �Ѵ�.
 * 
 * 
 */
namespace AP_DATA
{
	using namespace AP_OBJ;

	/****************************************************
	 *
	 ****************************************************/

	 /// <summary>
	 /// system config ���� ������
	 /// </summary>
	struct GConfig_dat : public UTL::parser::json
	{

		struct netCfgDat_t
		{
			char* name{};
			char* ip_str{};
			uint32_t port_no{};
			char* mode_str{};

			inline void Init() {
				name = nullptr;
				ip_str = nullptr;
				mode_str = nullptr;
				port_no = 0;
			}
		};

		struct serialCfgDat_t
		{
			char* name{};
			char* port_str{};
			uint32_t baudrate{};

		};

		enum class addr_e //
		{
			//common
			MODEL_NAME, MODEL_DIR, PASSWORD, LANGUAGE_SELECT, RUN_MODE, MACHINE_NO, MACHINE_NAME,
			//operation
			USE_BUZZER, USE_DOOR, USE_MES, SHIFT_A_TIME, SHIFT_C_TIME, RETAY_MAX_CNT,
			//serialcomm 
			SERIAL_MOONS_NAME, SERIAL_MOONS_PORT_NAME, SERIAL_MOONS_BAUDRATE,
			SERIAL_ORIENTAL_NAME, SERIAL_ORIENTAL_PORT_NAME, SERIAL_ORIENTAL_BAUDRATE,
			SERIAL_SICK_NAME, SERIAL_SICK_PORT_NAME, SERIAL_SICK_BAUDRATE,
			SERIAL_AGV_NAME, SERIAL_AGV_PORT_NAME, SERIAL_AGV_BAUDRATE,

			//netsocket
			UPS_0_NET_NAME, UPS_0_NET_IP, UPS_0_NET_PORT, UPS_0_NET_MODE,
			UPS_1_NET_NAME, UPS_1_NET_IP, UPS_1_NET_PORT, UPS_1_NET_MODE,
			FANUC_NET_NAME, FANUC_NET_IP, FANUC_NET_PORT, FANUC_NET_MODE,
			fastech_io_i16_CB_NET_NAME_0, fastech_io_i16_CB_NET_IP_0, fastech_io_i16_CB_PORT_0, fastech_io_i16_CB_MODE_0,
			fastech_io_i16_CB_NET_NAME_1, fastech_io_i16_CB_NET_IP_1, fastech_io_i16_CB_PORT_1, fastech_io_i16_CB_MODE_1,
			fastech_io_o16_CB_NET_NAME_2, fastech_io_o16_CB_NET_IP_2, fastech_io_o16_CB_PORT_2, fastech_io_o16_CB_MODE_2,
			fastech_io_o16_CB_NET_NAME_3, fastech_io_o16_CB_NET_IP_3, fastech_io_o16_CB_PORT_3, fastech_io_o16_CB_MODE_3,
			fastech_io_i16o16_TC_NET_NAME_0, fastech_io_i16o16_TC_NET_IP_0, fastech_io_i16o16_TC_PORT_0, fastech_io_i16o16_TC_MODE_0,
			fastech_io_i16o16_TC_NET_NAME_1, fastech_io_i16o16_TC_NET_IP_1, fastech_io_i16o16_TC_PORT_1, fastech_io_i16o16_TC_MODE_1,
			fastech_io_i16o16_SL_NET_NAME_0, fastech_io_i16o16_SL_NET_IP_0, fastech_io_i16o16_SL_PORT_0, fastech_io_i16o16_SL_MODE_0,
			fastech_io_i16o16_SL_NET_NAME_1, fastech_io_i16o16_SL_NET_IP_1, fastech_io_i16o16_SL_PORT_1, fastech_io_i16o16_SL_MODE_1,
			
			fastech_io_i16o16_ROBOT_NET_NAME, 
			fastech_io_i16o16_ROBOT_NET_IP, 
			fastech_io_i16o16_ROBOT_NET_PORT, 
			fastech_io_i16o16_ROBOT_NET_MODE,


			fastech_mot_lib_fastech_tapeLoader_NAME, 
			fastech_mot_lib_fastech_tapeLoader_IP, 
			fastech_mot_lib_fastech_tapeLoader_PORT, 
			fastech_mot_lib_fastech_tapeLoader_MODE,

			fastech_mot_lib_fastech_old_tape_NAME, 
			fastech_mot_lib_fastech_old_tape_IP, 
			fastech_mot_lib_fastech_old_tape_PORT, 
			fastech_mot_lib_fastech_old_tape_MODE,

			fastech_mot_lib_fastech_new_tape_NAME, 
			fastech_mot_lib_fastech_new_tape_IP, 
			fastech_mot_lib_fastech_new_tape_PORT, 
			fastech_mot_lib_fastech_new_tape_MODE,
			_max
		};


		GConfig_dat(char* file_info = PRJ_DATA_FILE_PATH_CONFIG);
		virtual ~GConfig_dat();
		int LoadData();
		void WriteData(GConfig_dat::addr_e addr, UTL::parser::json_t data);
		errno_t ReadData(GConfig_dat::addr_e addr, void** p_ret);
		uint32_t GetDataCnt() const;
		bool IsAssignedData() const;

	public:
		/* api �Լ� ����*/
		bool IsOpened() const;
		bool IsUseDoorOpenSensor();
		bool IsUseBuzzer();
		char* GetMoonsPortName();
		int GetMoonsBaudrate();
		int GetMoonsPortNo();
		char* GetOrientalPortName();
		int GetOrientalPortNo();
		int GetOrientalBaudrate();
		char* GetSickPortName();
		int GetSickBaudrate();

		void GetSerialConfig(SERIAL_OBJ obj, serialCfgDat_t* p_data);
		void GetNetConfig(NET_OBJ obj, netCfgDat_t* p_data);

	private:
		//bool putData(char** pdata, uint8_t cnt);
		std::mutex m_mutex;
		//parser::json* m_pParser;
		char m_dirFile[PRJ_FILE_DIR_STR_MAX]{};
		bool m_isOpened;
		int  m_dataCnt;

	};



	/****************************************************
	 * Axis parameter Data
	 ****************************************************/
	constexpr uint32_t AXIS_DATA_ELEMENT_MAX = 17;  //  name������ ��� ����
	struct GAxis_dat : public UTL::parser::json
	{

		struct dat_t
		{
			char name[PRJ_NAME_STR_LENGTH_MAX]{};
			double  CWSWLimit{};
			double  CCWSWLimit{};
			double  HomeFastVelocity{};
			double  HomeSlowVelocity{};
			double  HomeAccelerate{};
			double  HomeOffset{};
			double  JogSlow{};
			double  JogFast{};
			double  RunAccelerate{};
			double  RunVelocity{};
			double  SlowRunVelocity{};
			double  LimitTime{};
			double  OriginLimitTime{};
			double  StabilityTime{};
			double  Tolerance{};
			double  PassOffset{};
			double  StopDecelerate{};

			inline uint32_t DataLength() const {
				return AXIS_DATA_ELEMENT_MAX;
			}
			inline void SetName(const char* str) {
				memcpy_s(name, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}
			/*inline void operator = (const char* str) {
				memcpy_s(name, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}*/


		};


		//AP_OBJ  AXIS_OBJ�� ������
		enum class addr_e
		{
			oriental_rewind,

			fastech_tapeLoader,
			moons_ForBack_TU,

			fastech_splicingLoader_L,
			fastech_splicingLoader_R,
			moons_ForBack_SU,
			moons_UpDown_L,
			moons_UpDown_R,
			_max
		};


		GAxis_dat::dat_t axis_dat[static_cast<uint8_t>(GAxis_dat::addr_e::_max)];

		/* constructor*/
		GAxis_dat(char* file_info = PRJ_DATA_FILE_PATH_AXIS);
		virtual ~GAxis_dat();

		/* api �Լ� ����*/
		bool IsAssignedData() const;
		bool IsOpened() const;
		int LoadData();
		uint32_t GetDataCnt() const;
		GAxis_dat::dat_t* GetData(GAxis_dat::addr_e addr);

		inline char* AxisList_to_string(GAxis_dat::addr_e addr)
		{
			switch (addr)
			{
			case AP_DATA::GAxis_dat::addr_e::oriental_rewind:
				return "oriental_rewind";
			case AP_DATA::GAxis_dat::addr_e::fastech_tapeLoader:
				return "fastech_tapeLoader";
			case AP_DATA::GAxis_dat::addr_e::moons_ForBack_TU:
				return "moons_ForBack_TU";
			case AP_DATA::GAxis_dat::addr_e::fastech_splicingLoader_L:
				return "fastech_splicingLoader_L";
			case AP_DATA::GAxis_dat::addr_e::fastech_splicingLoader_R:
				return "fastech_splicingLoader_R";
			case AP_DATA::GAxis_dat::addr_e::moons_ForBack_SU:
				return "moons_ForBack_SU";
			case AP_DATA::GAxis_dat::addr_e::moons_UpDown_L:
				return "moons_UpDown_L";
			case AP_DATA::GAxis_dat::addr_e::moons_UpDown_R:
				return "moons_UpDown_R";
			default:
				return "";
			}


			
		}


		//void WriteData(GAxis_dat::addr_e addr, parser::json_t data);
	   // errno_t ReadData(GAxis_dat::addr_e addr, void** p_ret);


		//void WriteData(GAxis_dat::addr_e addr, GAxis_dat::dat_t data);

		//GAxis_dat::dat_t ReadData(GAxis_dat::addr_e addr);
		//void LoadRomData();
	   // bool ClearRomData();

	private:

		std::mutex m_mutex;
		char m_dirFile[PRJ_FILE_DIR_STR_MAX]{};
		bool m_isOpened;
		int  m_dataCnt;

		void assignData(addr_e addr);
	};

	/****************************************************
	 * motion location Data
	 ****************************************************/
	constexpr uint32_t MOTION_DATA_ELEMENT_MAX = 2;  //  name������ ��� ����
	struct GMotion_dat : public UTL::parser::json
	{
		struct dat_t
		{
			char idx_name[PRJ_NAME_STR_LENGTH_MAX]{};
			char pos_desc[PRJ_NAME_STR_LENGTH_MAX]{};
			double position{};

			inline uint32_t DataLength() const {
				return MOTION_DATA_ELEMENT_MAX;
			}

			inline void SetName(const char* str) {
				memcpy_s(idx_name, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}

			inline void SetDesc(const char* str) {
				memcpy_s(pos_desc, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}

		};
		
		enum class addr_e //
		{
			moon_ForBack_TU_forward,
			moon_ForBack_TU_backward,
			moon_ForBack_SU_forward,
			moon_ForBack_SU_backward,
			moon_UpDown_L_up,
			moon_UpDown_L_down,
			moon_UpDown_R_up,
			moon_UpDown_R_down,

			_max,
		};

		bool m_isOpened{};
		int m_dataCnt{};
		GMotion_dat::dat_t motion_dat[static_cast<uint8_t>(GMotion_dat::addr_e::_max)]{};
		
		GMotion_dat(char* file_info = PRJ_DATA_FILE_PATH_MOTION);
		virtual ~GMotion_dat();

		int LoadData();
		uint32_t GetDataCnt() const;
		bool IsAssignedData() const;
		bool IsOpened() const;
		void assignData(GMotion_dat::addr_e addr);
		double GetPositionData(GMotion_dat::addr_e addr);
	};
	// end of struct GMotion_dat 

	/****************************************************
	 * Cyilnder Data
	 ****************************************************/
	constexpr uint32_t CYLINDER_DATA_ELEMENT_MAX = 5;//  name������ ��� ����
	struct GCylinder_dat : public UTL::parser::json
	{
		struct dat_t
		{
			char name[PRJ_NAME_STR_LENGTH_MAX]{};
			char CylType[PRJ_NAME_STR_LENGTH_MAX]{};
			char SolType[PRJ_NAME_STR_LENGTH_MAX]{};
			double  MovingTime{};
			double  SettlingTime1{};
			double  SettlingTime2{};

			inline uint32_t DataLength() const {
				return CYLINDER_DATA_ELEMENT_MAX;
			}
			inline void SetName(const char* str) {
				memcpy_s(name, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}
			inline void SetCylinderType(const char* str) {
				memcpy_s(CylType, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}
			inline void SetSolenoidType(const char* str) {
				memcpy_s(SolType, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}

		};

		/* multiply by length 4*/
		enum class addr_e //
		{
			TC_cutterUnit_FB, TC_pressGuide_R_UD,
			TC_pressGuide_L_UD, TC_cutterKnife_UD,
			TC_tapePosition_UD, TC_holeDetector_UD, TC_centeringTape_OC,
			SL_loderUD_R_UD, SL_loderUD_L_UD,
			SL_loderFB_R_FB, SL_loderFB_L_FB,
			SL_tapePullOut_R_FB, SL_tapePullOut_L_FB,
			_max
		};

		GCylinder_dat::dat_t cyl_act_dat[static_cast<uint8_t>(GCylinder_dat::addr_e::_max)]{};

		GCylinder_dat(char* file_info = PRJ_DATA_FILE_PATH_CYLINDER);
		virtual ~GCylinder_dat();

#if 0
		void WriteData(GCylinder_dat::addr_e addr, GCylinder_dat::dat_t data);

		GCylinder_dat::dat_t ReadData(GCylinder_dat::addr_e addr);
		void LoadRomData();
		bool ClearRomData();
#endif

		/* api �Լ� ����*/
		bool IsOpened() const;
		int LoadData();
		uint32_t GetDataCnt() const;
		bool IsAssignedData() const;
		GCylinder_dat::dat_t* GetData(GCylinder_dat::addr_e addr);

		inline char* CylinderList_ToString(GCylinder_dat::addr_e addr)
		{
			return cyl_act_dat[static_cast<uint8_t>(addr)].name;
		}

	private:

		std::mutex m_mutex;
		char m_dirFile[PRJ_FILE_DIR_STR_MAX]{};
		bool m_isOpened;
		int  m_dataCnt;

		void assignData(addr_e addr);

	};


	/****************************************************
	 * Vacuum Data
	 ****************************************************/
	constexpr uint32_t VACUUM_DATA_ELEMENT_MAX = 5;//  name������ ��� ����

	struct GVacuum_dat : public UTL::parser::json
	{
		struct dat_t
		{
			char name[PRJ_NAME_STR_LENGTH_MAX]{};
			char VacType[PRJ_NAME_STR_LENGTH_MAX]{};
			char SolType[PRJ_NAME_STR_LENGTH_MAX]{};
			double  TurningTime{};
			double  OnSettlingTime{};
			double  OffSettlingTime{};

			inline uint32_t DataLength() const {
				return VACUUM_DATA_ELEMENT_MAX;
			}
			inline void SetName(const char* str) {
				memcpy_s(name, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}
			inline void SetVacuumType(const char* str) {
				memcpy_s(VacType, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}
			inline void SetSolenoidType(const char* str) {
				memcpy_s(SolType, PRJ_NAME_STR_LENGTH_MAX, str, strnlen_s(str, PRJ_NAME_STR_LENGTH_MAX));
			}
		};

		enum class addr_e //
		{
			first_vac,
			vac_1, vac_2, vac_3,
			vac_4, vac_5, vac_6, vac_7,
			_max
		};

		GVacuum_dat::dat_t vac_act_dat[static_cast<uint8_t>(GVacuum_dat::addr_e::_max)]{};

		GVacuum_dat(char* file_info = PRJ_DATA_FILE_PATH_VACUUM);
		virtual ~GVacuum_dat();

#if 0
		void WriteData(GVacuum_dat::addr_e addr, GVacuum_dat::dat_t data);
		GVacuum_dat::dat_t ReadData(GVacuum_dat::addr_e addr);
		void LoadRomData();
		bool ClearRomData();

#endif

		/* api �Լ� ����*/
		bool IsOpened() const;
		int LoadData();
		bool IsAssignedData() const;
		uint32_t GetDataCnt() const;
		GVacuum_dat::dat_t* GetData(GVacuum_dat::addr_e addr);
		inline char* VacuumList_ToString(GVacuum_dat::addr_e addr)
		{
			return vac_act_dat[static_cast<uint8_t>(addr)].name;
		}

	private:

		std::mutex m_mutex;
		char m_dirFile[PRJ_FILE_DIR_STR_MAX]{};
		bool m_isOpened;
		int  m_dataCnt;

		void assignData(addr_e addr);
	};



	struct GErrDescript_dat : public UTL::parser::json
	{
		struct dat_t
		{
			uint16_t  parm1;
			uint16_t  parm2;
		};

		enum class addr_e //
		{
			seq_0, seq_1, seq_2, seq_3,
			seq_4, seq_5, seq_6, seq_7,
			_max
		};

		std::mutex m_mutex;

		GErrDescript_dat::dat_t sequencing_dat[static_cast<uint8_t>(GErrDescript_dat::addr_e::_max)]{};


		GErrDescript_dat(char* file_info = PRJ_DATA_FILE_PATH_ERR_DESC);
		virtual ~GErrDescript_dat();


	};









	/****************************************************
	 *	controller�� mcu eeprom data ������ �д´�
	 ****************************************************/

	 /// <summary>
	 /// eeprom ap config data
	 /// </summary>
	struct GCtrlBoard_dat
	{
		struct dat_t
		{
			uint16_t  parm1;
			uint16_t  parm2;
		};

		enum class addr_e //
		{
			ap_mot_cfg, ap_1, ap_2, ap_3, ap_4, ap_5, ap_6, ap_7,
			_max
		};

		std::mutex m_mutex;

		GCtrlBoard_dat::dat_t apcfg_dat[static_cast<uint8_t>(GCtrlBoard_dat::addr_e::_max)]{};

		void WriteData(GCtrlBoard_dat::addr_e addr, GCtrlBoard_dat::dat_t data);
		GCtrlBoard_dat::dat_t ReadData(GCtrlBoard_dat::addr_e addr);
		GCtrlBoard_dat::dat_t* GetData(GCtrlBoard_dat::addr_e addr);
		void LoadRomData();
		bool ClearRomData();
		void Write_MotCfg(GCtrlBoard_dat::dat_t data);
		GCtrlBoard_dat::dat_t Read_MotCfg();
	};







}