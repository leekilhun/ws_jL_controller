#pragma once

#ifdef USE_HW_ROBOT_FANUC_LIB


#include "fanucSocket.h"

#include "robot_fanuc_reg.h"


namespace FANUC_ROBOT
{

	constexpr int get_io_data_cnt = 32;


	class GFanucRobot:public Icommon
	{
	public:
		struct cfg_t
		{
			char ip_str[FANUC_IP_STR_MAX]{};
			uint32_t  port_no{};
			HWND msgHwnd = {};

			inline int SetIpStr(const char* value) {
				return sprintf_s(ip_str, FANUC_IP_STR_MAX, value);
			}

			inline char* GetIpStr() {
				return &ip_str[0];
			}

			inline void operator = (const cfg_t* p_cfg) {
				this->port_no = p_cfg->port_no;
				this->msgHwnd = p_cfg->msgHwnd;
				this->SetIpStr(p_cfg->ip_str);
			}

		};

	private:
		std::unique_ptr<FanucSocket> m_pFanucLib;

		cfg_t m_cfg;
		bool m_IsSetData;
		bool m_IsConnected;
		bool m_TrdLife;
		DWORD m_TrdId;
		// 포트를 감시할 함수, 스레드의 핸들을 보관
		HANDLE m_hThread;



		/*
		 이하 fanuc data table 관련 변수
		*/

		DataTable* m_pDataTable;
		DataCurPos* m_pCurPos;
		DataCurPos* m_pCurPosUF;
		DataCurPos* m_pCurPosWorld;
		DataPosReg* m_pPosReg;
		DataPosRegMG* m_pPosRegMG;
		
		DataAlarm* m_pAlarm;
		DataAlarm* m_pAlarmCurrent;
		DataAlarm* m_pAlarmPasswd;
		DataString* m_pStrReg;
		DataString* m_pStrRegComment;
		DataNumReg* m_pNumRegInt;


	public:
		DataString* m_pDIComment;
		DataString* m_pDOComment;
		DataString* m_pRIComment;
		DataString* m_pROComment;
		DataString* m_pUIComment;
		DataString* m_pUOComment;
		DataString* m_pSIComment;
		DataString* m_pSOComment;
		DataString* m_pWIComment;
		DataString* m_pWOComment;
		DataString* m_pWSIComment;
		DataString* m_pAIComment;
		DataString* m_pAOComment;
		DataString* m_pGIComment;
		DataString* m_pGOComment;

		std::vector<short> m_SDO;
		std::vector<short> m_SDI;
		std::vector<short> m_RDO;
		std::vector<short> m_RDI;
		std::vector<short> m_SO;
		std::vector<short> m_SI;
		std::vector<short> m_UO;
		std::vector<short> m_UI;
		
		std::vector<std::string> m_DIComments;
		std::vector<std::string> m_DOComments;
		std::vector<std::string> m_RIComments;
		std::vector<std::string> m_ROComments;
		std::vector<std::string> m_SIComments;
		std::vector<std::string> m_SOComments;
		std::vector<std::string> m_GIComments;
		std::vector<std::string> m_GOComments;


		std::vector<int> m_GO;
		std::vector<int> m_GI;

	public:
		GFanucRobot(common::Attribute_t& common_data, cfg_t& cfg) : Icommon(common_data)
			, m_cfg(cfg), m_pFanucLib{}, m_IsConnected{}, m_IsSetData{},
			m_TrdLife{}, m_TrdId{}, m_hThread{},			
			m_pCurPos{}, m_pPosReg{},m_pPosRegMG{}, m_pAlarm{}, m_pAlarmCurrent{},
			m_pStrReg{}, m_pStrRegComment{}, m_pDIComment{},
			m_pDOComment{}, m_pRIComment{}, m_pROComment{}, m_pUIComment{},
			m_pUOComment{}, m_pSIComment{}, m_pSOComment{}, m_pWIComment{},
			m_pWOComment{}, m_pWSIComment{}, m_pAIComment{}, m_pAOComment{},
			m_pGIComment{}, m_pGOComment{}, m_pDataTable{}, m_pAlarmPasswd{},
			m_pCurPosUF{}, m_pCurPosWorld{}, m_pNumRegInt{}

			, m_DIComments{}, m_DOComments{}, m_RIComments {}, m_ROComments{}
			, m_SIComments{}, m_SOComments{}, m_GIComments{}, m_GOComments{}
		{
			m_SDO.resize(get_io_data_cnt);
			m_SDI.resize(get_io_data_cnt);
			m_RDO.resize(get_io_data_cnt);
			m_RDI.resize(get_io_data_cnt);
			m_SO.resize(get_io_data_cnt);
			m_SI.resize(get_io_data_cnt);
			m_UO.resize(get_io_data_cnt);
			m_UI.resize(get_io_data_cnt);
			m_GO.resize(get_io_data_cnt);
			m_GI.resize(get_io_data_cnt);


			m_DIComments.resize(get_io_data_cnt);
			m_DOComments.resize(get_io_data_cnt);
			m_RIComments.resize(get_io_data_cnt);
			m_ROComments.resize(get_io_data_cnt);
			m_SIComments.resize(get_io_data_cnt);
			m_SOComments.resize(get_io_data_cnt);
			m_GIComments.resize(get_io_data_cnt);
			m_GOComments.resize(get_io_data_cnt);
		}

		~GFanucRobot() {
			if (IsConnected()) threadStop();
			if (m_pFanucLib)
			{
				m_pFanucLib.reset();
			}
			m_pFanucLib = nullptr;
		}

	private:

		inline void threadStop(void)
		{
			m_TrdLife = false;
			while (m_TrdId);
			m_IsConnected = false;
		}

		inline void threadRun(void)
		{
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

		void threadJob(void)
		{
			constexpr int start_addr = 1;
			if (m_pFanucLib->IsConnected() == false)
			{
				if (m_IsSetData == false)
					SetDataTable();

				if (m_pFanucLib->Recovery() != ERROR_SUCCESS)
				{
					ERR_PRINT("Failure Fanuc Robot Communication Recovery !");
					return;
				}
				else
				{

				}
			}

			bool ret = false;
			std::vector <short> temp{};

			temp.clear();
			temp.resize(m_RDI.size());
			if (m_pFanucLib->Read_RDI(start_addr, temp, (short)temp.size()))
			{
				m_RDI = temp;
				ret = true;
			}

			temp.clear();
			temp.resize(m_RDO.size());
			if (m_pFanucLib->Read_RDO(start_addr, temp, (short)temp.size()))
			{
				m_RDO = temp;
				ret = true;
			}

			temp.clear();
			temp.resize(m_SDI.size());
			if (m_pFanucLib->Read_SDI(start_addr, temp, (short)temp.size()))
			{
				m_SDI = temp;
				ret = true;
			}

			temp.clear();
			temp.resize(m_SDO.size());
			if (m_pFanucLib->Read_SDO(start_addr, temp, (short)temp.size()))
			{
				m_SDO = temp;
				ret = true;
			}

			temp.clear();
			temp.resize(m_SI.size());
			if (m_pFanucLib->Read_SI(start_addr, temp, (short)temp.size()))
			{
				m_SI = temp;
				ret = true;
			}

			temp.clear();
			temp.resize(m_SO.size());
			if (m_pFanucLib->Read_SO(start_addr, temp, (short)temp.size()))
			{
				m_SO = temp;
				ret = true;
			}

			temp.clear();
			temp.resize(m_UI.size());
			if (m_pFanucLib->Read_UI(start_addr, temp, (short)temp.size()))
			{
				m_UI = temp;
				ret = true;
			}

			temp.clear();
			temp.resize(m_UO.size());
			if (m_pFanucLib->Read_UO(start_addr, temp, (short)temp.size()))
			{
				m_UO = temp;
				ret = true;
			}


			std::vector <int> vint_temp(m_GO.size());
			if (m_pFanucLib->Read_GI(start_addr, vint_temp, (short)vint_temp.size()))
			{
				m_GI = vint_temp;
				ret = true;
			}

			vint_temp.clear();
			vint_temp.resize(m_GO.size());
			if (m_pFanucLib->Read_GO(start_addr, vint_temp, (short)vint_temp.size()))
			{
				m_GO = vint_temp;
				ret = true;
			}


			//std::vector<std::string> v_str(m_DIComments.size());
			std::string ret_str{};
			for (int i = 0; i < m_DIComments.size(); i++)
			{
				if (m_pDIComment->GetValue(i+1, ret_str))
				{
					m_DIComments[i] = ret_str;
				}

				ret = true;
			}
			
			
			for (int i = 0; i < m_DOComments.size(); i++)
			{
				if (m_pDOComment->GetValue(i+1, ret_str))
				{
					m_DOComments[i] = ret_str;
				}

				ret = true;
			}


			if (m_pDataTable->Refresh() == false)
			{
				SetDataTable();
				if (m_pFanucLib->Recovery() != ERROR_SUCCESS)
				{
					ERR_PRINT("Failure Fanuc Robot Communication Recovery !");
					return;
				}
				else
				{

				}
			}
		}

		inline static UINT threadFunc(LPVOID pParam)
		{
			GFanucRobot* pThis = (GFanucRobot*)pParam;

			TRACE("GFanucRobot Thread Start\r\n");

			while (pThis->m_TrdLife)
			{
				pThis->threadJob();

				Sleep(20);
			}

			TRACE("GFanucRobot Thread Terminatet\r\n");
			pThis->m_TrdId = 0;
			pThis->m_hThread = NULL;
			return 0;
		}








	public:
		inline bool Init() {
			bool ret = false;
			FanucSocket::cfg_t lib_cfg{};
			lib_cfg.SetIpStr(m_cfg.ip_str);
			lib_cfg.port_no = m_cfg.port_no;
			m_pFanucLib = std::unique_ptr<FanucSocket>(new FanucSocket(lib_cfg));


			if (m_pFanucLib->Init())
			{
				SetDataTable();
				if (m_pFanucLib->Connect() == ERROR_SUCCESS)
				{
					ret = true;
					threadRun();
				}
			}

			return ret;
		}
		inline bool IsConnected() const {
			return m_IsConnected;
		}

		inline bool Disconnect() {
			threadStop();
			if (m_pFanucLib)
				return m_pFanucLib->Disconnect();
			else
				return true;
		}

		/// <summary>
		/// 사용할 fanua data table을 생성한다. 
		/// </summary>
		inline void SetDataTable() {

			/* Set DataTable  */
			m_pDataTable = m_pFanucLib->GetDataTable1();

			/* Set data table items */
			constexpr int get_alarm_cnt = 5;
			m_pAlarm = m_pDataTable->AddAlarm(FRIF_DATA_TYPE::ALARM_LIST, get_alarm_cnt);
			constexpr int get_alarm_current_cnt = 1;
			m_pAlarmCurrent = m_pDataTable->AddAlarm(FRIF_DATA_TYPE::ALARM_CURRENT, get_alarm_current_cnt);		
			m_pAlarmPasswd = m_pDataTable->AddAlarm(FRIF_DATA_TYPE::ALARM_PASSWORD, 1);

			constexpr int data_group_1 = 1;
			m_pCurPos = m_pDataTable->AddCurPosUF(FRIF_DATA_TYPE::CURPOS, data_group_1, 15);

			m_pCurPosUF = m_pDataTable->AddCurPosUF(FRIF_DATA_TYPE::CURPOS, data_group_1, 15);
			m_pCurPosWorld = m_pDataTable->AddCurPosUF(FRIF_DATA_TYPE::CURPOS, data_group_1, 0);

			/* set register */
			m_pNumRegInt = m_pDataTable->AddNumReg(FRIF_DATA_TYPE::NUMREG_INT, 1/*INT_IDX_START*/, 200/*INT_IDX_END*/);
			//m_pNumRegfloat = (m_pDataTable->AddNumReg(FRIF_DATA_TYPE::NUMREG_REAL, FANUC_NUM_REG_REAL_IDX_START, FANUC_NUM_REG_REAL_IDX_END));	
			constexpr int get_pos_reg_start_idx = 1;
			constexpr int get_pos_reg_end_idx = 100;
			m_pPosReg = m_pDataTable->AddPosReg(FRIF_DATA_TYPE::POSREG, data_group_1, get_pos_reg_start_idx, get_pos_reg_end_idx);
			/* set script */
			m_pDIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::SDI_COMMENT, 1, get_io_data_cnt);
			m_pDOComment = m_pDataTable->AddString(FRIF_DATA_TYPE::SDO_COMMENT, 1, get_io_data_cnt);
			m_pRIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::RDI_COMMENT, 1, get_io_data_cnt);
			m_pROComment = m_pDataTable->AddString(FRIF_DATA_TYPE::RDO_COMMENT, 1, get_io_data_cnt);
			m_pUIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::UI_COMMENT, 1, get_io_data_cnt);
			m_pUOComment = m_pDataTable->AddString(FRIF_DATA_TYPE::UO_COMMENT, 1, get_io_data_cnt);
			m_pSIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::SI_COMMENT, 1, get_io_data_cnt);
			m_pSOComment = m_pDataTable->AddString(FRIF_DATA_TYPE::SO_COMMENT, 1, get_io_data_cnt);
			m_pWIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::WI_COMMENT, 1, get_io_data_cnt);
			m_pWOComment = m_pDataTable->AddString(FRIF_DATA_TYPE::WO_COMMENT, 1, get_io_data_cnt);
			m_pWSIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::WSI_COMMENT, 1, get_io_data_cnt);
			m_pAIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::AI_COMMENT, 1, get_io_data_cnt);
			m_pAOComment = m_pDataTable->AddString(FRIF_DATA_TYPE::AO_COMMENT, 1, get_io_data_cnt);
			m_pGIComment = m_pDataTable->AddString(FRIF_DATA_TYPE::GI_COMMENT, 1, get_io_data_cnt);
			m_pGOComment = m_pDataTable->AddString(FRIF_DATA_TYPE::GO_COMMENT, 1, get_io_data_cnt);


			constexpr int get_reg_command_start_idx = 1;
			constexpr int get_reg_command_end_idx = 20;
			m_pStrReg = m_pDataTable->AddString(FRIF_DATA_TYPE::STRREG, get_reg_command_start_idx, get_reg_command_end_idx);
			m_pStrRegComment = m_pDataTable->AddString(FRIF_DATA_TYPE::STRREG_COMMENT, get_reg_command_start_idx, get_reg_command_end_idx);


			constexpr int timeout = 100;
			m_pFanucLib->SetTimeOutValue(timeout);

			m_IsSetData = true;
		}



		inline robot_coordinate GetCurrRobotXyzwpr() {
			robot_coordinate ret{};
			std::vector<float> xyzwpr(9, 0);// XYZWPR
			std::vector<short> config(7, 0);// Config

			short UF, UT, valid_cart;/*, Valid_joint;*/
			if (m_pCurPos->GetValueXyzwpr(xyzwpr, config, UF, UT, valid_cart))
				ret.Set(xyzwpr, config, UF, UT, valid_cart);

			return ret;
		}

		inline robot_joint GetCurrRobotJoint() {
			robot_joint ret{};
			std::vector<float> joint(9, 0);// J1 ~ J6 reserved 7~ 8 
			short UT, valid_joint;

			if (m_pCurPos->GetValueJoint(joint, UT, valid_joint))
				ret.Set(joint, UT, valid_joint);

			return ret;
		
		}	

		std::vector<short>* ReadRDO() {
			return &m_RDO;
		}

		std::vector<short>* ReadRDI() {
			return &m_RDI;
		}

		io_t<short> GetRobotInData() {
			io_t<short> ret{};
			ret.Set(m_RDI.data(), m_RIComments.data(), m_RDI.size());
			return ret;
		}

		io_t<short> GetRobotOutData() {
			io_t<short> ret{};
			ret.Set(m_RDO.data(), m_ROComments.data(), m_RDO.size());
			return ret;
		}		


		std::vector<short>* ReadSDO() {
			return &m_SDO;
		}

		std::vector<short>* ReadSDI() {
			return &m_SDI;
		}

		io_t<short> GetDigitalInData() {
			io_t<short> ret{};
			ret.Set(m_SDI.data(), m_DIComments.data(), m_SDI.size());
			return ret;
		}

		io_t<short> GetDigitalOutData() {
			io_t<short> ret{};
			ret.Set(m_SDO.data(), m_DOComments.data(), m_SDO.size());
			return ret;
		}


		std::vector<short>* ReadSO() {
			return &m_SO;
		}

		std::vector<short>* ReadSI() {
			return &m_SI;
		}

		io_t<short> GetSystemOPInData() {
			io_t<short> ret{};
			ret.Set(m_SI.data(), m_SIComments.data(), m_SI.size());
			return ret;
		}

		io_t<short> GetSystemOPOutData() {
			io_t<short> ret{};
			ret.Set(m_SO.data(), m_SOComments.data(), m_SO.size());
			return ret;
		}

		std::vector<int>* ReadGO() {
			return &m_GO;
		}

		std::vector<int>* ReadGI() {
			return &m_GI;
		}

		io_t<int> GetGroupInData() {
			io_t<int> ret{};
			ret.Set(m_GI.data(), m_GIComments.data(), m_GI.size());
			return ret;
		}

		io_t<int> GetGroupOutData() {
			io_t<int> ret{};
			ret.Set(m_GO.data(), m_GOComments.data(), m_GO.size());
			return ret;
		}


		std::vector<short>* ReadUO() {
			return &m_UO;
		}

		std::vector<short>* ReadUI() {
			return &m_UI;
		}

		io_t<short> GetUserOPInData() {
			io_t<short> ret{};
			ret.Set(m_UI.data(), m_pUIComment, m_UI.size());
			return ret;
		}

		io_t<short> GetUserOPOutData() {
			io_t<short> ret{};
			ret.Set(m_UO.data(), m_pUOComment, m_UO.size());
			return ret;
		}





		/// <summary>
		/// 디지털 I/O (DI/DO) 상태를 가져온다
		/// </summary>
		/// <param name="addr_reg">주소</param>
		/// <param name="p_value">리턴 변수 주소</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t GetIO_Digital(fanuc_dio_rob_2_sys addr_reg, bool* p_value) {
			errno_t ret = 0;

			constexpr int DATA_CNT = 1;
			std::vector<short> v_dat(DATA_CNT, 0);
			short addr = 0;

			addr = STATIC_CAST(short)(addr_reg);
			bool result = m_pFanucLib->Read_SDO(addr, v_dat, DATA_CNT);
			if (result == true)
			{
				*p_value = (v_dat[0] == 0) ? false : true;
			}
			else
			{
				ret = -2;
			}

			return ret;
		}

		errno_t GetIO_Digital(fanuc_dio_sys_2_rob addr_reg, bool* p_value) {

		}
		

		/*

		/// <summary>
		/// 디지털 I/O (DI/DO) 상태를 가져온다
		/// </summary>
	  /// <param name="addr_reg">주소</param>
		/// <param name="p_value">리턴 변수 주소</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t GetIO_Digital(fanuc_dio_rob_2_sys addr_reg, bool* p_value);
		errno_t GetIO_Digital(fanuc_dio_sys_2_rob addr_reg, bool* p_value);

		/// <summary>
		/// 디지털 I/O (DO) 상태를 변경한다
		/// </summary>
		/// <param name="addr_reg">주소</param>
		/// <param name="value">반영 값</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t SetIO_Digital(fanuc_dio_rob_2_sys addr_reg, bool value);

		/// <summary>
		/// 로봇 엔드 이펙터 IO 상태를 가져온다
		/// </summary>
		/// <param name="addr_reg">주소</param>
		/// <param name="p_value">리턴 변수 주소</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t GetIO_Robot(fanuc_rio_rob_2_sys addr_reg, bool* p_value);

		errno_t GetIO_Robot(fanuc_rio_sys_2_rob addr_reg, bool* p_value);

		/// <summary>
		/// 로봇 엔드 이펙터 IO (RO)상태를 변경한다
		/// </summary>
		/// <param name="addr_reg">주소</param>
		/// <param name="value">반영 값</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t SetIO_Robot(fanuc_rio_rob_2_sys addr_reg, bool value);

		/// <summary>
		/// 시스템 전용 IO 상태를 가져온다
		/// </summary>
		/// <param name="addr_reg">주소</param>
		/// <param name="p_value">리턴 변수 주소</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t GetIO_Sys(fanuc_uio_rob_2_sys addr_reg, bool* p_value);
		errno_t GetIO_Sys(fanuc_uio_sys_2_rob addr_reg, bool* p_value);

		/// <summary>
		/// 조작패널 IO 상태를 가져온다
		/// </summary>
		/// <param name="addr_reg">주소</param>
		/// <param name="p_value">리턴 변수 주소</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t GetIO_CPanel(fanuc_sio_rob_2_sys addr_reg, bool* p_value);
		errno_t GetIO_CPanel(fanuc_sio_sys_2_rob addr_reg, bool* p_value);

		/// <summary>
		/// 조작패널  I/O (SO) 상태를 변경한다
		/// </summary>
		/// <param name="addr_reg">주소</param>
		/// <param name="value">반영 값</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t SetIO_CPanel(fanuc_sio_rob_2_sys addr_reg, bool value);



		errno_t GetIO_Group(fanuc_gio_rob_2_sys addr_reg, int* p_value);
		errno_t GetIO_Group(fanuc_gio_sys_2_rob addr_reg, int* p_value);

		errno_t SetIO_Group(fanuc_gio_rob_2_sys addr_reg, int value);




		/// <summary>
		/// 로봇 좌표 (world) 가져 온다
		/// </summary>
		/// <param name="robot_pos">데이터 변수</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t GetCurrPos_WorldFrame(MPos_Robot& robot_pos);
		/// <summary>
		/// 로봇 좌표 (user frame) 가져 온다
		/// </summary>
		/// <param name="robot_pos">데이터 변수</param>
		/// <returns> success (0) 이외 에러 </returns>
		errno_t GetCurrPosUF(MPos_Robot& robot_pos);

		*/




	};

}


#endif // USE_HW_ROBOT_FANUC_LIB