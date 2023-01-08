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
		/* api �Լ� ����*/
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