#pragma once


#ifdef USE_HW_NET_SOCKET

#include <winsock2.h> // �������α׷��� �Լ�����
#define SOCKET_IP_STR_MAX      26


//https://aronglife.tistory.com/entry/NetworkTCPIP-%EC%86%8C%EC%BC%93%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D4%EC%9C%88%EB%8F%84%EC%9A%B0-%EC%84%9C%EB%B2%84-%EA%B5%AC%ED%98%84?category=910808

class GNetSocket:public IComm
{

  /****************************************************
  *	data
  ****************************************************/
public:
  enum class mode_e {
    TCP_Server,
    TCP_Client,
    UDP_Server,
    UDP_Client,
  };

  struct cfg_t
  {
    GNetSocket::mode_e mode{};
    char ip_str[SOCKET_IP_STR_MAX]{};
    uint32_t  port_no{};
    HWND msgHwnd = {};

	inline int SetIpStr(const char* value) {
		return sprintf_s(ip_str, SOCKET_IP_STR_MAX, value);

	}

	inline int SetNetMode(char* value)	{
        int ret = ERROR_SUCCESS;
		if (UTL::IsEqualStr("TCP_CLIENT", value))
		{
            mode = mode_e::TCP_Client;
		}
		else if (UTL::IsEqualStr("TCP_SERVER", value))
		{
            mode = mode_e::TCP_Server;
		}
		else if (UTL::IsEqualStr("UDP_CLIENT", value))
		{
            mode = mode_e::UDP_Client;
		}
		else //if (util::IsEqualStr("UDP_SERVER", (char*)(p_ret)))
		{
            ret = -1;
		}
        return ret;
	}

    inline char* GetIpStr() {
      return &ip_str[0];
    }

    inline void operator = (const cfg_t* p_cfg) {
      this->mode = p_cfg->mode;
      this->port_no = p_cfg->port_no;
      this->msgHwnd = p_cfg->msgHwnd;
      this->SetIpStr(p_cfg->ip_str);
    }

  };

private:
  cfg_t m_Cfg;

  enum class err_e
  {
    SUCCESS,
    NOT_SUPPORTED = 1,
    WSA_START_UP,
    CREATE_MUTEX,
    SOCKET,
    BIND,
    LISTEN,
    ACCEPT,
    WSAEVENT_SELECT,
    READ,
    CLOSE,
    CONNECT,
    NOT_CONNECTED,
    SENDED,
    CLIENT_NO_OVER,
  };

public:

  //servo 
  SOCKET m_SockArray[WSA_MAXIMUM_WAIT_EVENTS];		  //ù��° : Server Socket, �׿� : Client Socket
  WSAEVENT m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];		//ù��° : Server Event, �׿� : Client Event
  bool m_IsConnected[WSA_MAXIMUM_WAIT_EVENTS];		  //���� ���� Array
  bool m_IsReceveCplt[WSA_MAXIMUM_WAIT_EVENTS];		  //
  SOCKADDR_IN m_ClientsAddr[WSA_MAXIMUM_WAIT_EVENTS];
  uint32_t m_SocketCnt;
  WSABUF	m_dataBuf;
  unsigned long	m_recvBytes;

  //client
  SOCKET m_clientSocket;
  WSAEVENT  m_clientEvent;
  SOCKADDR_IN m_ServAddr, m_ClntAddr;

  //
  void* m_cbObj;
  evt_cb m_func;
  
  //
  bool m_TrdLife;
  bool m_IsOpened;
  DWORD m_TrdId;

  // ��Ʈ�� ������ �Լ�, �������� �ڵ��� ����
  HANDLE m_hThread;
  /****************************************************
   *	Constructor
   ****************************************************/
public:
  GNetSocket(common::Attribute_t common_data, GNetSocket::cfg_t& cfg);
  ~GNetSocket();

  /****************************************************
   *	func
   ****************************************************/
private:
  /** Socket �ʱ�ȭ*/
  //netSocket::err_e iniComm();
  GNetSocket::err_e initServerSocket();
  GNetSocket::err_e initClientSocket();

  /** Server�� ����� �����Ѵ�. */
  GNetSocket::err_e startServer();
  bool closeSocket();

  /** Server�� ����� �����Ѵ�. */
  GNetSocket::err_e startClient();


  /// <summary>
  /// server�� ����� �õ��Ѵ�.
  /// ���� �� �ݺ������� �����
  /// </summary>
  /// <returns></returns>
  GNetSocket::err_e connClientSocket();

  void threadStop(void);
  void threadRun(void);
  GNetSocket::err_e threadJob_Server(void);
  GNetSocket::err_e threadJob_Client(void);
  GNetSocket::err_e threadJob_ClientUDP(void);
  static UINT threadFunc(LPVOID pParam);
public:
  bool IsOpened() const;
  void AttCallbackFunc(void* obj, evt_cb cb);
  errno_t Open();
  void Close();
  void Recovery();
  int SendData(char* p_str, uint32_t length);

  /*
  int SendCharacter(char* pChar, int nLength, CString strMateIP = "");
  int ReadData(CString& str, CString strMateIP = "");
  BOOL IsMateConnected(CString strMateIP = "");
  int AddClient(CString strClientIP);
  BOOL IsMateMsgReceived(CString strMateIP = "");
  */



};



#endif