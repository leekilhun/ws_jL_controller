#pragma once

/*
 * system_dat.h
 *
 * 작성자 : 이길훈
 * 22.04.05
 *  data  객체
 *
 * 22.10.11 데이터 정리
 *
 * 데이터 json 추가 시 addr_e 추가된 항목을 반영한 후 
 * 함수 GetNetConfig에 두 내용을 매칭하는 코드를 넣어야 한다.
 * 
 * 
 */
namespace AP_DATA
{

	/****************************************************
	 *
	 ****************************************************/

	 /// <summary>
	 /// system config 정보 데이터
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

		enum addr_e //
		{
			//serialcomm 
			SERIAL_MCU_NAME, SERIAL_MCU_PORT_NAME, SERIAL_MCU_BAUDRATE,
			addr_max
		};


		GConfig_dat(char* file_info = PRJ_DATA_FILE_PATH_CONFIG);
		virtual ~GConfig_dat();
		int LoadData();
		void WriteData(GConfig_dat::addr_e addr, UTL::parser::json_t data);
		errno_t ReadData(GConfig_dat::addr_e addr, void** p_ret);
		uint32_t GetDataCnt() const;
		bool IsAssignedData() const;

	public:
		/* api 함수 정의*/
		void GetSerialConfig(SERIAL_OBJ obj, serialCfgDat_t& p_data);

	private:
		//bool putData(char** pdata, uint8_t cnt);
		std::mutex m_mutex;
		//parser::json* m_pParser;
		char m_dirFile[PRJ_FILE_DIR_STR_MAX]{};
		bool m_isOpened;
		int  m_dataCnt;

	};
	// end of struct GConfig_dat








}
//end of namespace AP_DATA