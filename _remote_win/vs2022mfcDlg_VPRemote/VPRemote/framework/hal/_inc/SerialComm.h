#pragma once

/*
 * SerialComm.h
 *
 * �ۼ��� : �̱���
 * 22.04.04
 *  rs232 ����� ���� hal component
 *  interface IComm ���
 *
 * 22.09.05
 */



#ifdef USE_HW_SERIAL_COMM

namespace HAL
{


#define SERIAL_COMM_BUFF_MAX	4096

	class GSerialComm :public IComm
	{
	public:

		/*enum callbackObj_e
		{
			cbObj_module_peeler,
			cbObj_max,
		};*/

		struct cfg_t
		{
			char portname[PRJ_NAME_STR_LENGTH_MAX]{};
			uint32_t baudrate{};
			uint8_t  port_no{};
			HANDLE serial_handle = {};

			inline int SetPortName(const char* value) {
				if (value == nullptr) { return -1; };
				return sprintf_s(portname, PRJ_NAME_STR_LENGTH_MAX, value);
			}


			inline char* GetPortName() {
				return &portname[0];
			}

			inline void GetPortName(TCHAR* p_value, uint32_t len = PRJ_NAME_STR_LENGTH_MAX) {
				TCHAR tmp[PRJ_NAME_STR_LENGTH_MAX] = { 0, };
				UTL::trans::CharToLPTSTR(portname, tmp, PRJ_NAME_STR_LENGTH_MAX);
				wsprintf(p_value, tmp);
			}

			inline void operator = (const cfg_t* p_cfg) {
				this->baudrate = p_cfg->baudrate;
				this->port_no = p_cfg->port_no;
				this->SetPortName(p_cfg->portname);
			}

		};

		enum class error_code
		{
			Success,
			GetCommState,
			SerCommState,
			SetCommMask,
			SetupComm,
			PurgeComm,
			ClearCommError,
			GetCommTimeouts,
			SetCommTimeouts,
			existPort,
			emptyPort,
			invalidHandleValue,
			failCreateEventRead,
			failCreateEventWrite,
			failCreateThread,
		};


		/****************************************************
		 *	data
		 ****************************************************/
	private:
		//SerialComport* m_pSerial;

	public:
		bool m_IsOpened;
		bool m_TrdLife;
		cfg_t m_Serialcfg;
		UTL::_que<uint8_t> m_Que{ SERIAL_COMM_BUFF_MAX };

		uint8_t  m_received_data;

		//void* m_cbObj[callbackObj_e::cbObj_max];
		void* m_cbObj;
		evt_cb m_func;



		HANDLE m_hThread;
		DWORD m_TrdId;

		/****************************************************
		 *	Constructor
		 ****************************************************/
	public:
		GSerialComm(common::Attribute_t common_data, cfg_t& cfg);
		virtual ~GSerialComm();


		/****************************************************
		 *	func
		 ****************************************************/
	private:
		errno_t openPort(char* port_name, uint32_t baud);
		bool closePort();

		void threadStop(void);
		void threadRun(void);
		void threadJob(void);

		static UINT threadFunc(LPVOID pParam);

		/// <summary>
		/// port�� �����Ͱ� ���ŵǸ� ȣ��� �Լ�
		/// </summary>
		/// <param name="obj"></param>
		/// <param name="w_parm"></param>
		/// <param name="l_parm"></param>
		static void cbFunc(void* obj, void* w_parm, void* l_parm);

		uint32_t serialAvailable();
		uint32_t readData();
	public:
		errno_t PortOpen(char* port_name, uint32_t baud);


		errno_t Open();
		void Close();
		void Recovery();
		bool IsOpened() const;
		void AttCallbackFunc(void* obj, evt_cb cb);
		int SendData(char* p_str, uint32_t length);

		uint8_t Read();
		uint32_t Write(uint8_t* p_data, uint32_t length);
		uint32_t CommPrintf(char* fmt, ...);


	};


}


#endif