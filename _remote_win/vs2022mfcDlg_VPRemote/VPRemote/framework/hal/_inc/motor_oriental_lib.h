#pragma once

#ifdef USE_HW_MOTOR_ORIENTAL_LIB




// ## Library �߰��ϱ�
#pragma comment(lib, "lib/oriental/AZ/x64/INA_AZ_DLL.lib")

#include "INA_AZ_DLL.h"

constexpr int DEF_AZ_RET_SUCCESS = 1;
constexpr int DEF_AZ_RET_FAIL = 0;
constexpr int DEF_AZ_INC_DATA_NO = 15;


constexpr errno_t DEF_AZ_ERR_SUCCESS = 0;
constexpr errno_t DEF_AZ_ERR_RET_FAIL = -1;
//
#define AZ_LOG_WRITE(fmt, ...)		m_log.syslog_Write(SYSLOG::level::info, m_Attribute.GetObjectID(), m_Attribute.GetLogFileName(), (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define AZ_ERR_WRITE(fmt, ...)		m_log.syslog_Write(SYSLOG::level::err, m_Attribute.GetObjectID(), m_Attribute.GetLogFileName(), (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)

namespace GOriental
{
	using namespace LIB_ORIENTAL;
	using namespace std;

	class GOriental_motor_lib : public IMotionLib
	{
		/****************************************************
		 *	data
		 ****************************************************/
	public:
		struct cfg_t
		{
			//IComm* pComm{};
			int			base_node_id{};
			int			port_no{};								// �ø�����Ʈ
			int			baudrate{};					// Baudrate
			int			comm_delay_ms{};					// Delay Time �ʱⰪ - 35ms
			int			default_velocity{};		// 

		};
	private:

		/****************************************************
		 *	Oriental Moter
		 ****************************************************/


		 /**
		 *	Oriental Motor - Parameter
		 */
		INA_Serial	m_Serial;					    // -- �ø��� ��� ����ü ����
		int			m_SerialIndex;				// -- �ø��� ��� INDEX
		bool		m_IsConnect;					// -- �ø��� ��� ���� ����

		int			m_DataNo = 0;		// 
		mutex       m_mutex;


		GOriental_motor_lib::cfg_t m_cfg;

		/****************************************************
		 *	Constructor
		 ****************************************************/
	public:
		inline GOriental_motor_lib(common::Attribute_t& common_data, cfg_t& cfg)
			:Icommon(common_data), /*m_AxisId(0),*/ m_cfg(cfg), m_mutex()
			, m_Serial(), m_IsConnect(), m_SerialIndex()/*, m_regAxis()*/
		{

			AZ_LOG_WRITE("Initialize oriental_motor[%d]", m_Attribute.GetInstanceNo());

			errno_t ret_init = Initialize();
			if (ret_init != DEF_AZ_ERR_SUCCESS)
			{
				//m_plog->syslog_Write()
				AZ_ERR_WRITE("oriental_motor[%d] FAIL : Initialize ", m_Attribute.GetInstanceNo());
				TRACE("FAIL : Oriental Motor Initialize \n");
			}

#ifdef PRJ_USE_FUNC_CLI
			cli::cliAdd(this, "oriental", cliOrientalTest);
#endif


			}
		virtual ~GOriental_motor_lib()
		{
			AZ_Disconnect();

		}


		/****************************************************
	   *	func
	   ****************************************************/
	private:

#ifdef PRJ_USE_FUNC_CLI

		inline static void GOriental_motor::cliOrientalTest(void* obj, cli::cli_args_t* args)
		{
			GOriental_motor* pThis = (GOriental_motor*)obj;

			bool ret = true;
			if (args->argc == 3)
			{
				if (args->isStr(0, "write_dio_out"))
				{

				}
				else if (args->isStr(0, "write_rio_out"))
				{

				}
				else
				{
					ret = false;
				}
			}
			else	if (args->argc == 2)
			{
				int value = (int)args->getData(1);
				if (args->isStr(0, "jog"))
				{
					errno_t result = pThis->JogPitchMove(value);
					if (result == DEF_AZ_ERR_SUCCESS)
					{
						cli::cliPrintf("JogPitchMove [%d] success\n", value);
					}
					else
					{
						cli::cliPrintf("JogPitchMove faile\n");
					}
				}
				else if (args->isStr(0, "motion"))
				{
					errno_t result = pThis->AZ_Start(value);
					if (result == DEF_AZ_ERR_SUCCESS)
					{
						cli::cliPrintf("AZ_Start [%d] success\n", value);
					}
					else
					{
						cli::cliPrintf("AZ_Start faile\n");
					}

				}
				else
				{
					ret = false;
				}
			}
			else if (args->argc == 1)
			{
				errno_t result = 0;
				if (args->isStr(0, "motor_off"))
				{
					result = pThis->AZ_FreeOn();
					if (result == DEF_AZ_ERR_SUCCESS)
					{
						cli::cliPrintf("motor_off success\n");
					}
					else
					{
						cli::cliPrintf("motor_off faile\n");
					}
					/*while (cliKeepLoop())
					{

					}*/
				}
				else if (args->isStr(0, "motor_on"))
				{
					result = pThis->AZ_FreeOff();
					if (result == DEF_AZ_ERR_SUCCESS)
					{
						cli::cliPrintf("motor_on success\n");
					}
					else
					{
						cli::cliPrintf("motor_on faile\n");
					}
				}
				else if (args->isStr(0, "alarm_reset"))
				{
					result = pThis->AZ_Alarm_Reset();
					if (result == DEF_AZ_ERR_SUCCESS)
					{
						cli::cliPrintf("alarm_reset success\n");
					}
					else
					{
						cli::cliPrintf("alarm_reset faile\n");
					}
				}
				else if (args->isStr(0, "get_alarm"))
				{
					result = pThis->AZ_Get_Current_Alarm();
					cli::cliPrintf("get_alarm return value : %d \n", result);

				}
				else if (args->isStr(0, "get_flag"))
				{
					cli::cliPrintf("get_flag return value : %d \n", pThis->GetFlagStatus());
				}
				else if (args->isStr(0, "move"))
				{
					result = pThis->Move();
					cli::cliPrintf("move ret - %d \n", result);

				}
				else if (args->isStr(0, "init"))
				{
					pThis->Initialize();
				}
				else
				{
					ret = false;
				}
			}
			else
			{
				ret = false;
			}


			if (ret == false)
			{
				cli::cliPrintf("oriental motor_on\n");
				cli::cliPrintf("oriental motor_off\n");
				cli::cliPrintf("oriental alarm_reset\n");
				cli::cliPrintf("oriental get_alarm\n");
				cli::cliPrintf("oriental init\n");
				cli::cliPrintf("oriental move\n");
				cli::cliPrintf("oriental get_flag\n");

				cli::cliPrintf("oriental jog value[1~1000]\n");
				cli::cliPrintf("oriental motion no[1~5]\n");

			}

		}


#endif


	public:


		/**
		*	Oriental Moter - Status
		*/


		/*inline bool AZ_IsHome() const {
			return m_regAxis.AZ_FLAG_HOME_END;
		}
		inline bool AZ_IsReady() const {
			return m_regAxis.AZ_FLAG_READY;
		}
		inline bool AZ_IsInfo() const {
			return m_regAxis.AZ_FLAG_INFO;
		}
		inline bool AZ_IsAlarm() const {
			return m_regAxis.AZ_FLAG_ALM_A;
		}
		inline bool AZ_IsBusy() const {
			return m_regAxis.AZ_FLAG_SYS_BSY;
		}*/



		/**
		*	Oriental Moter - Connecting
		*/
		inline errno_t AZ_Connect() {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			if (m_IsConnect)
				return ret;

			int az_ret = INA_AZ_INITIALIZE(m_SerialIndex, m_cfg.port_no, m_cfg.baudrate);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�		
				AZ_ERR_WRITE("INA_AZ_INITIALIZE err_ret : %d", az_ret);

				AZ_Disconnect();
				return DEF_AZ_ERR_RET_FAIL;
			}
			// �ø��� ��ü ����
			m_Serial = INA_AZ_GET_SERIAL(m_SerialIndex);
			Sleep(100);

			az_ret = INA_AZ_SET_DELAY(m_SerialIndex, m_cfg.comm_delay_ms);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�		
				AZ_ERR_WRITE("INA_AZ_SET_DELAY err_ret : %d", az_ret);
				AZ_Disconnect();
				return DEF_AZ_ERR_RET_FAIL;
			}
			m_IsConnect = true;

			return ret;
		}
		inline errno_t AZ_Disconnect() {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = 0;
			// �����ִ� �� ��� �ݱ�. ��� �ȴ����� �翬�� ����.
			for (int i = 0; i < m_SerialIndex; i++)
				az_ret = INA_AZ_UNINITIALIZE(i);  // retrun 1 : Success

			az_ret = INA_AZ_UNINITIALIZE(m_SerialIndex);  // retrun 1 : Success
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_UNINITIALIZE err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			m_IsConnect = false;
			return ret;
		}
		inline errno_t AZ_Set_Serial() {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;

			az_ret = INA_AZ_SET_SERIAL(m_SerialIndex, m_Serial);  // retrun 1 : Success
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_SERIAL err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}

			return ret;
		}


		inline int AZ_getFlagStatus(int node_id)
		{
			//return INA_AZ_GET_CURRENT_DRIVER_OUTPUT_LOW(m_SerialIndex, m_cfg.base_node_id + node_id);
			// 0 home-end,  1 in-pos, 2 motor on(PLS-RDY), 3 READY, 4 MOVE, 5 ALM_B 
			return INA_AZ_GET_CURRENT_DIRECT_IO_HIGH(m_SerialIndex, m_cfg.base_node_id + node_id);

		}


		/**
		*	Oriental Moter - ����
		*/
		inline errno_t AZ_Move(int node_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			az_ret = INA_AZ_SET_FWD(m_SerialIndex, m_cfg.base_node_id + node_id, 0, TRUE);  // retrun 1 : Success
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("AZ_Move err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}

			return ret;
		}
		inline errno_t AZ_Move_For(int node_id, BOOL bOnOff) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			az_ret = INA_AZ_SET_FWD(m_SerialIndex, m_cfg.base_node_id + node_id, 0, bOnOff);  // retrun 1 : Success
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("AZ_Move_For err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}

			return ret;
		}

		inline errno_t  AZ_Start(int node_id, int data_no) {	// Data No ���������� ����
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			m_DataNo = data_no;

			// Move
			az_ret = INA_AZ_SET_START(m_SerialIndex, m_cfg.base_node_id + node_id, data_no);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_START err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			Sleep(200);

			return ret;

		}
		inline errno_t AZ_Home(int node_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			if (ERROR_FAILED_FUNCTION == INA_AZ_SET_CURRENT_DRIVER_INPUT_LOW(m_SerialIndex, m_cfg.base_node_id + node_id, 4, TRUE))
				//if (ERROR_FAILED_FUNCTION == INA_AZ_SET_HOME(m_nSerialIndex, m_iSlaveStartID + iAxisID))
			{
				// �˶�
			}

			Sleep(10);

			if (ERROR_FAILED_FUNCTION == INA_AZ_SET_CURRENT_DRIVER_INPUT_LOW(m_SerialIndex, m_cfg.base_node_id + node_id, 4, FALSE))
			{
				// �˶�
			}

			return ret;
		}


		inline errno_t AZ_Stop(int node_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;

			az_ret = INA_AZ_SET_STOP(m_SerialIndex, m_cfg.base_node_id + node_id);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_STOP err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			return ret;
		}



		inline errno_t AZ_FreeOn(int node_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			const int motor_free = 1;

			az_ret = INA_AZ_SET_FREE(m_SerialIndex, m_cfg.base_node_id + node_id, motor_free);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("AZ_FreeOn err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}

			return ret;
		}
		inline errno_t AZ_FreeOff(int node_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			const int motor_on = 0;

			az_ret = INA_AZ_SET_FREE(m_SerialIndex, m_cfg.base_node_id + node_id, motor_on);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("AZ_FreeOff err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}

			return ret;
		}


		/**
		*	Oriental Moter - Alarm, Reset, ... etc
		*/

		inline errno_t AZ_Alarm_Reset(int node_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			const int motor_off = 0;

			az_ret = INA_AZ_SET_ALARM_RESET(m_SerialIndex, m_cfg.base_node_id + node_id);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_ALARM_RESET err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}

			return ret;
		}

		inline int AZ_Get_Current_Alarm(int node_id) {
			int ret = 0;
			ret = INA_AZ_GET_CURRENT_ALARM(m_SerialIndex, m_cfg.base_node_id + node_id);

			AZ_ERR_WRITE("INA_AZ_GET_CURRENT_ALARM return value : %d", ret);
			return ret;
		}

		inline errno_t AZ_P_PReset(int node_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			const int motor_off = 0;

			az_ret = INA_AZ_SET_P_PRESET(m_SerialIndex, m_cfg.base_node_id + node_id);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_P_PRESET err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}

			return ret;
		}



		/**
	*	Oriental Moter - Setting
	*/
		inline errno_t AZ_SetOperatingCurrent(int node_id, int data_no, int value) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			int get_value = value;
			if (get_value > 1000)
				get_value = 1000;
			else if (get_value < 1)
				get_value = 1;

			az_ret = INA_AZ_SET_DATA_OPERATING_CURRENT(m_SerialIndex, m_cfg.base_node_id + node_id, data_no, get_value);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_DATA_OPERATING_CURRENT err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			else
			{
				Sleep(200);
			}

			return ret;
		}

		inline int AZ_GetOperatingCurrent(int node_id, int data_no) {
			return INA_AZ_GET_DATA_OPERATING_CURRENT(m_SerialIndex, m_cfg.base_node_id + node_id, data_no);
		}

		inline int AZ_GetDataVelocity(int node_id, int data_no) {
			return INA_AZ_GET_DATA_VELOCITY(m_SerialIndex, m_cfg.base_node_id + node_id, data_no);
		}

		inline int AZ_GetCurrentVelocity(int node_id) {
			return INA_AZ_GET_CURRENT_VELOCITY(m_SerialIndex, m_cfg.base_node_id + node_id);
		}


		inline errno_t AZ_Set_Velocity(int node_id, int data_no, int value) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			int set_value = value;

			//if (data_no == 0 || data_no == 1)
				m_DataNo = data_no;

			az_ret = INA_AZ_SET_DATA_VELOCITY(m_SerialIndex, m_cfg.base_node_id + node_id, data_no, set_value);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_DATA_VELOCITY err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			else
			{
				Sleep(200);
			}

			return ret;
		}


		inline errno_t AZ_Set_Torque(int node_id, int data_no, int value) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			int set_value = value;

			az_ret = INA_AZ_SET_DATA_OPERATING_CURRENT(m_SerialIndex, m_cfg.base_node_id + node_id, data_no, set_value);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("AZ_Set_Torque err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			else
			{
				Sleep(200);
			}

			return ret;
		}

		inline errno_t AZ_Set_Torque_For(int node_id, int value) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			int set_value = value;
			const int data_for = 0;

			az_ret = INA_AZ_SET_DATA_OPERATING_CURRENT(m_SerialIndex, m_cfg.base_node_id + node_id, data_for, set_value);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("AZ_Set_Torque_For err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			else
			{
				Sleep(200);
			}

			return ret;
		}

		inline errno_t AZ_Set_Torque_Back(int node_id, int value) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;
			int set_value = value;
			const int data_back = 1;

			az_ret = INA_AZ_SET_DATA_OPERATING_CURRENT(m_SerialIndex, m_cfg.base_node_id + node_id, data_back, set_value);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("AZ_Set_Torque_Back err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			else
			{
				Sleep(200);
			}

			return ret;
		}











		/* IMotionLib  �Լ�*/

		inline errno_t Initialize() {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = 0; // retrun 1 : Success , 0 : fail

			ret = AZ_Connect();
			if (ret != DEF_AZ_ERR_SUCCESS) {
				AZ_ERR_WRITE("oriental AZ_Connect err_ret : %d", ret);
				ret = -1;
			}
			return  ret;
		}




		inline errno_t WaitDone(short axis_id, unsigned short timeout) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();

			m_mutex.unlock();
			return ret;
		}

		inline bool IsMotorOn(short axis_id) {
			m_mutex.lock();
			bool ret = false;
			ret = ((AZ_getFlagStatus(axis_id) >> 2) & 1); // PLS_RDY flag return
			m_mutex.unlock();
			return ret;
		}

		inline bool IsAxisDone(short axis_id) {
			m_mutex.lock();
			bool ret = false;
			ret = ((AZ_getFlagStatus(axis_id) >> 1) & 1); //  in_pos flag return
			m_mutex.unlock();
			return ret;
		}

		inline errno_t GetAxisStatus(short axis_id, DWORD& p_state) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();
			p_state = (DWORD)AZ_getFlagStatus(axis_id);
			m_mutex.unlock();
			return ret;
		}

		inline errno_t ClearPosition(short axis_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();
			ret = INA_AZ_SET_P_PRESET(m_SerialIndex, m_cfg.base_node_id + axis_id);
			m_mutex.unlock();
			return ret;
		}

		inline errno_t AlarmReset(short axis_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();

			if (INA_AZ_SET_CURRENT_DRIVER_INPUT_LOW(m_SerialIndex, m_cfg.base_node_id + axis_id, 4, FALSE) == ERROR_FAILED_FUNCTION)
			{
				ret = ERROR_FAILED_FUNCTION;
				// error
			}

			/** �� �߻� Event ���� */
			if ((INA_AZ_SET_ALARM_RESET(m_SerialIndex, m_cfg.base_node_id + axis_id)) == ERROR_FAILED_FUNCTION)
			{
				ret = ERROR_FAILED_FUNCTION;
				// error
			}
			m_mutex.unlock();
			return ret;
		}

		inline errno_t GetAlarmType(short axis_id, DWORD& p_state) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();
			p_state = (DWORD)INA_AZ_GET_CURRENT_ALARM(m_SerialIndex, m_cfg.base_node_id + axis_id);

			m_mutex.unlock();
			return ret;
		}

		inline errno_t GetCommandPosition(short axis_id, long& p_position) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();
			p_position = (long)INA_AZ_GET_CURRENT_FEEDBACK_POSITION(m_SerialIndex, m_cfg.base_node_id + axis_id);

			m_mutex.unlock();
			return ret;
		}

		inline errno_t GetActualPosition(short axis_id, long& p_position) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();
			p_position = (long)INA_AZ_GET_CURRENT_POSITION(m_SerialIndex, m_cfg.base_node_id + axis_id);

			m_mutex.unlock();
			return ret;
		}



		inline errno_t JogPitchMove(int node_id, int position, DWORD velocity, short accel, short decel)
		{
			m_mutex.lock();
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			int az_ret = DEF_AZ_RET_SUCCESS;

			DWORD dwVelocity = velocity;
			short siAccel = accel;
			short siDecel = decel;

			//// **1202 -> msec ������ ����
			//siAccel = (short)siAccel / 4;
			//siDecel = (short)siDecel / 4;

			//// **050112
			//if (siAccel <= 0) siAccel = 1;
			//if (siDecel <= 0) siDecel = 1;


			az_ret = INA_AZ_SET_JOG_OPERATING_SPEED(m_SerialIndex, m_cfg.base_node_id + node_id, dwVelocity);
			az_ret = INA_AZ_SET_JOG_TRAVEL_AMOUNT(m_SerialIndex, m_cfg.base_node_id + node_id, abs(position));

			if (position > 0)
			{
				if ((az_ret = INA_AZ_SET_JOG_P(m_SerialIndex, m_cfg.base_node_id + node_id, TRUE)) != DEF_AZ_RET_SUCCESS)
				{
					AZ_ERR_WRITE("error : INA_AZ_SET_JOG_P");
					ret = DEF_AZ_ERR_RET_FAIL;
				}
			}
			else
			{
				if ((az_ret = INA_AZ_SET_JOG_N(m_SerialIndex, m_cfg.base_node_id + node_id, TRUE)) != DEF_AZ_RET_SUCCESS)
				{
					AZ_ERR_WRITE("error : INA_AZ_SET_JOG_N");
					ret = DEF_AZ_ERR_RET_FAIL;
				}
			}
			m_mutex.unlock();
			return ret;
		}


		inline errno_t JogMove(short axis_id, DWORD velocity, bool is_cw) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();

			int az_ret = DEF_AZ_RET_SUCCESS;

			//INA_AZ_SET_DATA_VELOCITY(m_SerialIndex, m_cfg.base_node_id + axis_id, 0, velocity);
			INA_AZ_SET_JOG_OPERATING_SPEED(m_SerialIndex, m_cfg.base_node_id + axis_id, velocity);
			INA_AZ_SET_JOG_TRAVEL_AMOUNT(m_SerialIndex, m_cfg.base_node_id + axis_id, 10000);

			if (is_cw)
			{
				az_ret = INA_AZ_SET_JOG_N(m_SerialIndex, m_cfg.base_node_id + axis_id, TRUE);
			}
			else
			{
				az_ret = INA_AZ_SET_JOG_P(m_SerialIndex, m_cfg.base_node_id + axis_id, TRUE);
			}

			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("Move err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			else
			{
				Sleep(200);
			}

			m_mutex.unlock();
			return ret;
		}

		inline errno_t JogStop(short axis_id) {
			//bool ret = INA_AZ_SET_STOP(m_SerialIndex, m_cfg.base_node_id + (int)axis_id);
			int reg = INA_AZ_GET_CURRENT_DRIVER_INPUT_LOW(m_SerialIndex, m_cfg.base_node_id + axis_id);

			bool is_active_jog_p = reg >> 12 & 1;
			bool is_active_jog_n = reg >> 13 & 1;
			bool ret = true;
			if (is_active_jog_p)
				ret = INA_AZ_SET_JOG_P(m_SerialIndex, m_cfg.base_node_id + axis_id, FALSE);

			if (is_active_jog_n)
				ret = INA_AZ_SET_JOG_N(m_SerialIndex, m_cfg.base_node_id + axis_id, FALSE);

			

			return (MoveStop(axis_id) == DEF_AZ_RET_SUCCESS)? ERROR_SUCCESS: -1;
		}

		inline errno_t MoveStop(short axis_id) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();

			constexpr int estop_no_alarm = 0;
			constexpr int decl_stop = 1;
			constexpr int estop_with_alarm = 2;
			constexpr int decl_stop_with_alarm = 3;

			ret = (errno_t)INA_AZ_SET_STOP(m_SerialIndex, m_cfg.base_node_id + axis_id);
			ret = (errno_t)INA_AZ_SET_FW_LS_RV_LS_INPUT_ACTION(m_SerialIndex, m_cfg.base_node_id + axis_id, decl_stop);
			
			if (ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("INA_AZ_SET_STOP err_ret : %d", ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}
			m_mutex.unlock();
			return ret;
		}

		inline errno_t Move(short axis_id, long position, DWORD velocity, bool wait_done = false) {
			errno_t ret = DEF_AZ_ERR_SUCCESS;
			m_mutex.lock();

			int az_ret = DEF_AZ_RET_SUCCESS;

			az_ret = INA_AZ_SET_DATA_VELOCITY(m_SerialIndex, m_cfg.base_node_id + axis_id, DEF_AZ_INC_DATA_NO, velocity);
			az_ret = INA_AZ_SET_DATA_POSITION(m_SerialIndex, m_cfg.base_node_id + axis_id, DEF_AZ_INC_DATA_NO, position);
			if (az_ret != DEF_AZ_RET_SUCCESS)
			{
				// �˶�
				AZ_ERR_WRITE("Move err_ret : %d", az_ret);
				ret = DEF_AZ_ERR_RET_FAIL;
			}


			//������ ��� ��ȯ �� ��� ����� ���� �𸣰��� // 2022.11.15
			if (position > 0)
			{
			}
			else
			{

			}

			//az_ret = INA_AZ_SET_FWD(m_SerialIndex, m_cfg.base_node_id + axis_id, DEF_AZ_INC_DATA_NO, TRUE);
			//if (az_ret != DEF_AZ_RET_SUCCESS)
			//{
			//	// �˶�
			//	AZ_ERR_WRITE("Move err_ret : %d", az_ret);
			//	ret = DEF_AZ_ERR_RET_FAIL;
			//}
			//else
			//{
			//	Sleep(200);
			//}


			m_mutex.unlock();
			return ret;
		}

		inline errno_t OriginAxis(short axis_id) {
			m_mutex.lock();
			errno_t ret;
			ret = AZ_Home(m_cfg.base_node_id + axis_id);
			m_mutex.unlock();
			return ret;
		}


		inline errno_t MotorOnOff(short axis_id, bool on_off)
		{
			m_mutex.lock();
			errno_t ret = ERROR_SUCCESS;
			if (on_off)
			{
				ret = AZ_FreeOff(m_cfg.base_node_id + axis_id);
			}
			else
			{
				ret = AZ_FreeOn(m_cfg.base_node_id + axis_id);
			}

			m_mutex.unlock();
			return ret;
		}













		// �߰��ϱ�
		// INA_AZ_SET_DELAY(m_nSerialIndex, nDelay); 


#if 0 // test oriental motor lib 
	//  packet frame 
	//  slave_addr   func code    data     crc-16
	//    8 bit        8 bit    N��8 bit    16 bit
		{
			uint8_t crc_data[6] = { 0x01,0x03,0x18,0x40,0x00,0x06 };
			uint16_t ret_crc = 0;
			ret_crc = util::crc16_modbus_cal(&crc_data[0], 6);


			int az_ret = 0; // retrun 1 : Success , 0 : fail
			constexpr int AZ_RET_SUCCESS = 1;
			constexpr int AZ_RET_FAIL = 0;
			// �ʱ�ȭ
			az_ret = INA_AZ_INITIALIZE(m_nSerialIndex, m_nPort, m_dwBaud);

			// �ø��� ��ü ����
			m_Serial = INA_AZ_GET_SERIAL(m_nSerialIndex);
			Sleep(100);

			az_ret = INA_AZ_SET_DELAY(m_nSerialIndex, nDelay);


			Sleep(100);
			//az_ret = INA_AZ_SET_STOP(m_nSerialIndex, m_iSlaveStartID);
			Sleep(100);
			stAxis.iStatus = INA_AZ_GET_CURRENT_DRIVER_OUTPUT_LOW(m_nSerialIndex, m_iSlaveStartID);
			Sleep(100);

			//ALARM �ʱ�ȭ �Լ�
			az_ret = INA_AZ_SET_ALARM_RESET(m_nSerialIndex, m_iSlaveStartID);
			Sleep(100);





			// test code1
			// SafeMoveReel_AZ_Wind

			constexpr int AZ_CW_T20 = 0;
			constexpr int AZ_CCW_T20 = 1;
			constexpr int AZ_CW_T50 = 2;
			constexpr int AZ_CW_T40 = 3;
			constexpr int AZ_CW_T30 = 4;

			bool is_ready = stAxis.AZ_FLAG_READY;

			if (is_ready == false)
			{
				AfxMessageBox(L"motor not ready!");
				goto az_end;
			}

			// POSITION �ʱ�ȭ �Լ�
			az_ret = INA_AZ_SET_P_PRESET(m_nSerialIndex, m_iSlaveStartID);
			Sleep(100);

			// �̵�
		// AZ_CW_T20 0 - �ӵ�  2000 ��ũ 20% (�� ���� ����)
		// AZ_CCW_T20 1 - �ӵ� -2000 ��ũ 20% 
		// AZ_CW_T50 2 - �ӵ�  2000 ��ũ 50% (�� ���� ����)

			az_ret = INA_AZ_SET_START(m_nSerialIndex, m_iSlaveStartID, AZ_CW_T20);
			if (az_ret != AZ_RET_SUCCESS)
			{
				AfxMessageBox(L"motor return fail!");
				goto az_end;
			}

			Sleep(1000);
			az_ret = INA_AZ_SET_STOP(m_nSerialIndex, m_iSlaveStartID);
			if (az_ret != AZ_RET_SUCCESS)
			{
				AfxMessageBox(L"motor return fail!");
				goto az_end;
			}


			az_ret = INA_AZ_SET_START(m_nSerialIndex, m_iSlaveStartID, AZ_CCW_T20);
			if (az_ret != AZ_RET_SUCCESS)
			{
				AfxMessageBox(L"motor return fail!");
				goto az_end;
			}

			Sleep(1000);
			az_ret = INA_AZ_SET_STOP(m_nSerialIndex, m_iSlaveStartID);
			if (az_ret != AZ_RET_SUCCESS)
			{
				AfxMessageBox(L"motor return fail!");
				goto az_end;
			}



			goto az_end;

			constexpr int AZ_OFF = 0;
			constexpr int AZ_ON = 1;

			//DRIVER INPUT COMMAND �Լ�
			int memory_addr = 1; //Memory Address ��ȣ ( 0 ~ 255 )
			int bit_no = 0;
			az_ret = INA_AZ_SET_START(m_nSerialIndex, m_iSlaveStartID, memory_addr); // START �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_HOME(m_nSerialIndex, m_iSlaveStartID); // HOME �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_STOP(m_nSerialIndex, m_iSlaveStartID); // STOP �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_MS0(m_nSerialIndex, m_iSlaveStartID, AZ_ON); // MS0 �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_MS1(m_nSerialIndex, m_iSlaveStartID, AZ_ON); // MS1 �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_MS2(m_nSerialIndex, m_iSlaveStartID, AZ_ON); // MS2 �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_CLEAR(m_nSerialIndex, m_iSlaveStartID); // BIT �Է� ��ȣ �ʱ�ȭ �Լ�

			// servo off
			az_ret = INA_AZ_SET_FREE(m_nSerialIndex, m_iSlaveStartID, AZ_ON);

			az_ret = INA_AZ_SET_FWD(m_nSerialIndex, m_iSlaveStartID, memory_addr, AZ_ON); // FWD �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_RVS(m_nSerialIndex, m_iSlaveStartID, memory_addr, AZ_ON);//	RVS �Է� ��ȣ SET �Լ�



			az_ret = INA_AZ_SET_JOG_P(m_nSerialIndex, m_iSlaveStartID, AZ_ON); //JOG_P �Է� ��ȣ SET �Լ�
			az_ret = INA_AZ_SET_JOG_P(m_nSerialIndex, m_iSlaveStartID, AZ_OFF); // JOG_N �Է� ��ȣ SET �Լ�
			//int  az_state = stAxis.AZ_FLAG_INFO;


			//Return Value -  �Լ� �� MONITOR ��� ������ ��
			az_ret = INA_AZ_GET_CURRENT_ALARM(m_nSerialIndex, m_iSlaveStartID); //���� �˶� ���� GET �Լ� 
			az_ret = INA_AZ_GET_CURRENT_POSITION(m_nSerialIndex, m_iSlaveStartID); //���� ��ġ ���� GET �Լ� 
			az_ret = INA_AZ_GET_CURRENT_VELOCITY(m_nSerialIndex, m_iSlaveStartID); //���� ���� �ӵ� ���� GET�Լ� 
			az_ret = INA_AZ_GET_CURRENT_FEEDBACK_POSITION(m_nSerialIndex, m_iSlaveStartID); //���� FEEDBACK ��ġ GET �Լ� 
			az_ret = INA_AZ_GET_CURRENT_FEEDBACK_VELOCITY(m_nSerialIndex, m_iSlaveStartID); //���� FEEDBACK �ӵ� GET �Լ�



			// DRIVER �Է� ���� �Լ�
			az_ret = INA_AZ_SET_CURRENT_DRIVER_INPUT_HIGH(m_nSerialIndex, m_iSlaveStartID, bit_no, AZ_ON); // Driver �Է� ���� HIGH SET�Լ� 
			az_ret = INA_AZ_GET_CURRENT_DRIVER_INPUT_HIGH(m_nSerialIndex, m_iSlaveStartID);// Driver �Է� ���� HIGH GET�Լ�
			az_ret = INA_AZ_SET_CURRENT_DRIVER_INPUT_LOW(m_nSerialIndex, m_iSlaveStartID, bit_no, AZ_ON); //  Driver �Է� ���� LOW SET�Լ� 
			az_ret = INA_AZ_GET_CURRENT_DRIVER_INPUT_LOW(m_nSerialIndex, m_iSlaveStartID); //  Driver �Է� ���� LOW GET�Լ�


			//OPERATING DATA R/W COMMAND �Լ�
			int set_value = 1;// �Լ� ���� ����
			/*
				1:���� ��ġ ����											2: ��� ��ġ ���� ���� ��ġ ����
				3: ��� ��ġ ���� ���� ��ġ ���� )		7: ���� ���� ��ġ����
				8:Round ���� ��ġ ����								9: Round ��ó ��ġ ����
				10: Round FWD ���� ���� ��ġ ����			11: Round RVS ���� ���� ��ġ ����
				12: Round ���� ����									13: Round ��ó ����
				14: Round FWD ���� ����							15: Round RVS ���� ����
				16: ���� ���� �ӵ� ����								17: ���� ���� ����
				18: ���� ���� (Torque)								20: ���� ��ġ ���� ����
				21: ��� ��ġ ���� ���� ���� ��ġ ���� )
				22: ��� ��ġ ���� ���� ���� ��ġ ����

				*/
			az_ret = INA_AZ_SET_DATA_POSITION_MODE(m_nSerialIndex, m_iSlaveStartID, memory_addr, set_value); //		���� ��� SET �Լ�
			az_ret = INA_AZ_GET_DATA_POSITION_MODE(m_nSerialIndex, m_iSlaveStartID, memory_addr);            // ���� ��� GET �Լ�


			int pos_val = 0;//-2,147,483,648~2,147,483,647step
			az_ret = INA_AZ_SET_DATA_POSITION(m_nSerialIndex, m_iSlaveStartID, memory_addr, pos_val); // ��ġ SET �Լ�
			az_ret = INA_AZ_GET_DATA_POSITION(m_nSerialIndex, m_iSlaveStartID, memory_addr);          // ��ġ GET �Լ�
			int vel_val = 0;//-4,000,000~4,000,000Hz
			az_ret = INA_AZ_SET_DATA_VELOCITY(m_nSerialIndex, m_iSlaveStartID, memory_addr, vel_val); //�ӵ� SET �Լ�
			az_ret = INA_AZ_GET_DATA_VELOCITY(m_nSerialIndex, m_iSlaveStartID, memory_addr);          //�ӵ� GET �Լ�

			int acc_val = 0;//1,000,000,000(1=0.001kHz)
			az_ret = INA_AZ_SET_DATA_ACC_RATE(m_nSerialIndex, m_iSlaveStartID, memory_addr, acc_val); // ����(�⵿, ����) SET �Լ�
			az_ret = INA_AZ_GET_DATA_ACC_RATE(m_nSerialIndex, m_iSlaveStartID, memory_addr); //			���� �⵿ ���� ) GET �Լ�
			int dec_val = 0;//1,000,000,000(1=0.001kHz)
			az_ret = INA_AZ_SET_DATA_DEC_RATE(m_nSerialIndex, m_iSlaveStartID, memory_addr, dec_val); //			���� ���� SET �Լ�
			az_ret = INA_AZ_GET_DATA_DEC_RATE(m_nSerialIndex, m_iSlaveStartID, memory_addr); //			���� ���� ) GET �Լ�


			int current_val = 0;//: 0~1000(1= 0.1%)
			az_ret = INA_AZ_SET_DATA_OPERATING_CURRENT(m_nSerialIndex, m_iSlaveStartID, memory_addr, current_val); //���� ���� SET �Լ�
			az_ret = INA_AZ_GET_DATA_OPERATING_CURRENT(m_nSerialIndex, m_iSlaveStartID, memory_addr); // ���� ���� GET �Լ�
			int delaytime = 0;// 0~65535(1 = 0.001s)
			az_ret = INA_AZ_SET_DATA_DELAY_TIME(m_nSerialIndex, m_iSlaveStartID, memory_addr, delaytime); // ���� ���� ���� SET �Լ�
			az_ret = INA_AZ_GET_DATA_DELAY_TIME(m_nSerialIndex, m_iSlaveStartID, memory_addr); // ���� ���� ���� GET �Լ�

			int link_val = 0; //0: �������� ����, 1 : ���� ����, 2 : �ڵ� ����, 3 : ���� ����
			az_ret = INA_AZ_SET_DATA_LINK(m_nSerialIndex, m_iSlaveStartID, memory_addr, link_val); //	���� SET �Լ�
			az_ret = INA_AZ_GET_DATA_LINK(m_nSerialIndex, m_iSlaveStartID, memory_addr);           //	���� GET �Լ�
			int next_data_no = 0; // -256: STOP, 2 : +2, 1 : (+1), 0~255 : ���� No.
			az_ret = INA_AZ_SET_DATA_NEXT_DATA_NO(m_nSerialIndex, m_iSlaveStartID, memory_addr, next_data_no); //	���� ��� SET �Լ�
			az_ret = INA_AZ_GET_DATA_NEXT_DATA_NO(m_nSerialIndex, m_iSlaveStartID, memory_addr);               //	���� ��� GET �Լ�


			int area_offset = 0; //-2,147,483,648~2,147,483,647step
			az_ret = INA_AZ_SET_DATA_AREA_OFFSET(m_nSerialIndex, m_iSlaveStartID, memory_addr, area_offset); //      OFFSET(AREA) SET �Լ�
			az_ret = INA_AZ_GET_DATA_AREA_OFFSET(m_nSerialIndex, m_iSlaveStartID, memory_addr); // OFFSET(AREA) GET �Լ�
			int area_width = 0; // -1 : ��ȿ , 0~4,194,303 : 1step ������ ����
			az_ret = INA_AZ_SET_DATA_AREA_WIDTH(m_nSerialIndex, m_iSlaveStartID, memory_addr, area_width); //    ��(AREA) SET �Լ�
			az_ret = INA_AZ_GET_DATA_AREA_WIDTH(m_nSerialIndex, m_iSlaveStartID, memory_addr); //    ��(AREA) GET �Լ�



			int loop_cnt = 0; //0: ���� ((-), 2~255: Loop Ƚ�� (loop2{~loop256{)
			az_ret = INA_AZ_SET_DATA_LOOP_COUNT(m_nSerialIndex, m_iSlaveStartID, memory_addr, loop_cnt); //  Count(Loop) SET �Լ�
			az_ret = INA_AZ_GET_DATA_LOOP_COUNT(m_nSerialIndex, m_iSlaveStartID, memory_addr); //       Count(Loop) GET �Լ�
			int loop_offset = 0;// -4,194,304 ~ 4,194,303step
			az_ret = INA_AZ_SET_DATA_LOOP_OFFSET(m_nSerialIndex, m_iSlaveStartID, memory_addr, loop_offset); //       ��ġ Offset(Loop) SET �Լ�
			az_ret = INA_AZ_GET_DATA_LOOP_OFFSET(m_nSerialIndex, m_iSlaveStartID, memory_addr); //       ��ġ Offset(Loop) GET �Լ�
			int  loop_end = 0; //0: ���� ((-) , 1: }L End
			az_ret = INA_AZ_SET_DATA_LOOP_END(m_nSerialIndex, m_iSlaveStartID, memory_addr, loop_end); //       ����(Loop) S ET �Լ�
			az_ret = INA_AZ_GET_DATA_LOOP_END(m_nSerialIndex, m_iSlaveStartID, memory_addr); //       ����(L oop) GET �Լ�


			int low_io_event = 0; // - 1: ���� ((-) , 0~31: ���� I/O Event ��ȣ (0~31)
			az_ret = INA_AZ_SET_DATA_LOW_IO_EVENT(m_nSerialIndex, m_iSlaveStartID, memory_addr, low_io_event); //        �� Event SET �Լ�
			az_ret = INA_AZ_GET_DATA_LOW_IO_EVENT(m_nSerialIndex, m_iSlaveStartID, memory_addr); //       �� Event GET �Լ�
			int high_io_event = 0; // - 1: ���� ((-) , 0~31: ���� I/O Event ��ȣ (0~31)
			az_ret = INA_AZ_SET_DATA_HIGH_IO_EVENT(m_nSerialIndex, m_iSlaveStartID, memory_addr, high_io_event); //       �� Event SET �Լ�
			az_ret = INA_AZ_GET_DATA_HIGH_IO_EVENT(m_nSerialIndex, m_iSlaveStartID, memory_addr); //       �� Event GET �Լ�




			//goto end
		az_end:

			Sleep(100);
		}


#endif



		};



	}
#endif //USE_HW_MOTOR_ORIENTAL_LIB














