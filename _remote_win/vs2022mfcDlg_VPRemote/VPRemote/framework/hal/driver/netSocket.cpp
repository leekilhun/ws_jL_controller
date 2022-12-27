#include "pch.h"
#include "hal_def.h"

#include "netSocket.h"

#ifdef USE_HW_NET_SOCKET


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define NETSOCKET_MAX_CLIENT       5
#define NETSOCKET_MAX_BUFF_SIZE    4096

unsigned long g_irecvBytes = 0;
void CALLBACK CompletionRoutine(DWORD error, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags);



void CALLBACK CompletionRoutine(DWORD error, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	g_irecvBytes = szRecvBytes;
}




GNetSocket::GNetSocket(common::Attribute_t common_data, GNetSocket::cfg_t& cfg) :Icommon(common_data)
, m_SocketCnt(0), m_clientSocket(0), m_clientEvent()
, m_SockArray(), m_IsConnected(), m_IsReceveCplt(), m_ClientsAddr()/*, m_EventArray()*/
, m_ServAddr({}), m_ClntAddr({})
, m_TrdLife(false), m_IsOpened(false), m_TrdId(), m_hThread(), m_Cfg(cfg)
{

	m_recvBytes = 0;
	m_dataBuf = {};
	m_func = {};
	m_cbObj = nullptr;
	//memset(m_IsReceveCplt, 0, sizeof(m_IsReceveCplt));
	//memset(m_clientSocket, 0, sizeof(m_clientSocket));
	memset(m_EventArray, 0, sizeof(m_EventArray));


}

GNetSocket::~GNetSocket()
{
	if (m_IsOpened)
	{
		Close();
	}
}

//netSocket::err_e netSocket::iniComm()
//{
//  netSocket::err_e ret = netSocket::err_e::SUCCESS;
//  switch (m_Cfg.mode)
//  {
//  case netSocket::mode_e::TCP_Server:
//    ret = startServer();
//    break;
//
//  case netSocket::mode_e::TCP_Client:
//    ret = startClient();
//    break;
//
//  case netSocket::mode_e::UDP_Client:
//    break;
//
//  default:
//    break;
//  }
//
//  return ret;
//}

GNetSocket::err_e GNetSocket::initServerSocket()
{
	WSADATA wSaData = { 0, };
	SOCKET hServSock = NULL;
	SOCKADDR_IN servAddr = { 0, };

	if (WSAStartup(MAKEWORD(2, 2), &wSaData) != 0)
		return err_e::WSA_START_UP;

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		return err_e::SOCKET;

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(u_short(m_Cfg.port_no));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		return err_e::BIND;

	WSAEVENT wsaEvent;
	wsaEvent = WSACreateEvent();
	if (WSAEventSelect(hServSock, wsaEvent, FD_ACCEPT) == SOCKET_ERROR)
		return err_e::WSAEVENT_SELECT;

	if (listen(hServSock, NETSOCKET_MAX_CLIENT) == SOCKET_ERROR)
		return err_e::LISTEN;

	m_SocketCnt = 0;
	m_SockArray[m_SocketCnt] = hServSock;
	m_EventArray[m_SocketCnt] = wsaEvent;
	m_SocketCnt++;

	return err_e::SUCCESS;
}

GNetSocket::err_e GNetSocket::initClientSocket()
{
	WSADATA wSaData = { 0, };

	if (WSAStartup(MAKEWORD(2, 2), &wSaData) != 0)
		return err_e::WSA_START_UP;

	m_clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (m_clientSocket == INVALID_SOCKET)
		return err_e::SOCKET;

	memset(&m_ServAddr, 0, sizeof(m_ServAddr));
	m_ServAddr.sin_family = AF_INET;

	int result = inet_pton(AF_INET, m_Cfg.GetIpStr(), &m_ServAddr.sin_addr.s_addr);//inet_addr(m_Cfg.GetIpStr());
	if (result != 1)
	{
		TRACE("fail - inet_pton(AF_INET, m_Cfg.GetIpStr(), &m_ServAddr.sin_addr.s_addr) ");
	}
	m_ServAddr.sin_port = htons(m_Cfg.port_no);

	m_TrdLife = true;
	GNetSocket::err_e ret = connClientSocket();
	if (ret != err_e::SUCCESS)
		return ret;

	m_clientEvent = WSACreateEvent();
	WSAEventSelect(m_clientSocket, m_clientEvent, FD_READ | FD_CLOSE);


	return err_e::SUCCESS;
}

GNetSocket::err_e GNetSocket::startServer()
{



	return GNetSocket::err_e();
}

bool GNetSocket::closeSocket()
{
	bool ret = false;
	switch (m_Cfg.mode)
	{
	case GNetSocket::mode_e::TCP_Server:
		for (uint32_t i = 0; i < m_SocketCnt; i++)
		{
			if (m_SockArray[i % WSA_MAXIMUM_WAIT_EVENTS] != NULL)
			{
				closesocket(m_SockArray[i % WSA_MAXIMUM_WAIT_EVENTS]);
				m_SockArray[i % WSA_MAXIMUM_WAIT_EVENTS] = NULL;
			}
		}

		WSACleanup();
		memset(m_IsConnected, 0, sizeof(m_IsConnected));
		break;

	case GNetSocket::mode_e::TCP_Client:
		if (m_clientSocket != NULL)
		{
			WSACloseEvent(m_clientEvent);
			closesocket(m_clientSocket);
			m_clientSocket = NULL;
			WSACleanup();
			m_IsConnected[1] = FALSE;
		}
		break;

	case GNetSocket::mode_e::UDP_Client:
		break;

	default:
		break;
	}

	m_IsOpened = false;
	return false;
}

GNetSocket::err_e GNetSocket::startClient()
{


	return GNetSocket::err_e();
}

GNetSocket::err_e GNetSocket::connClientSocket()
{
	GNetSocket::err_e ret = err_e::SUCCESS;

	while (m_TrdLife)
	{
		int ret = connect(m_clientSocket, (SOCKADDR*)&(m_ServAddr), sizeof(m_ServAddr));
		if (ret != SOCKET_ERROR)
			break;

		TRACE("Socket ���� ���� Timeout \n");
		Sleep(500);
	}

	m_IsOpened = true;
	TRACE("Socket ���� \n");

	char message[256];

	int length = 0;
	sprintf_s(message, sizeof(message), "Socket ���� IP : %s", m_Cfg.GetIpStr());
	if (m_cbObj)
	{
		size_t length = strlen(message);
		m_func(m_cbObj, &length, &message);
	}
	//if (m_Cfg.msgHwnd != NULL)
	//	::PostMessage(m_Cfg.msgHwnd, m_uiConnectMessage, 1, (LPARAM)(message));  // client id, LPARAM
	return ret;
}

void GNetSocket::threadStop(void)
{
	m_TrdLife = false;
	while (m_TrdId);

}

void GNetSocket::threadRun(void)
{
	m_TrdLife = true;
	// ��Ʈ ���� ������ ����
	m_hThread = CreateThread(
		(LPSECURITY_ATTRIBUTES)NULL,
		0,
		(LPTHREAD_START_ROUTINE)threadFunc,
		(LPVOID)this,
		0,
		&m_TrdId
	);

}

GNetSocket::err_e GNetSocket::threadJob_Server(void)
{
	GNetSocket::err_e ret = GNetSocket::err_e::SUCCESS;
	int result = 0;

	if (m_IsOpened == false)
	{
		return GNetSocket::err_e::NOT_CONNECTED;
	}

	//result = WSAWaitForMultipleEvents(m_SocketCnt, m_EventArray, FALSE, 900, FALSE);
	//if (result == WSA_WAIT_TIMEOUT)
	//  return err_e::SUCCESS;
	//if (result == WSA_WAIT_FAILED)
	//  return err_e::NOT_CONNECTED;

	//int index = result - WSA_WAIT_EVENT_0;
	int index = 0;
	SOCKADDR_IN clnt_addr;

	WSAEVENT wsaEvent, dummyEvent;
	WSANETWORKEVENTS netEvents;
	SOCKET hClntSock;

	char message[NETSOCKET_MAX_BUFF_SIZE];
	bool	is_receive_cplt = false;


	unsigned long flags = 0;
	WSAOVERLAPPED overlapped;

	dummyEvent = WSACreateEvent();

	while (m_TrdLife)
	{
		for (uint32_t i = index; i < m_SocketCnt; i++)
		{
			result = WSAWaitForMultipleEvents(1, &(m_EventArray[i]), TRUE, 0, FALSE);
			if (result == WSA_WAIT_TIMEOUT || result == WSA_WAIT_FAILED)
				continue;

			WSAEnumNetworkEvents(m_SockArray[i % WSA_MAXIMUM_WAIT_EVENTS], m_EventArray[i % WSA_MAXIMUM_WAIT_EVENTS], &netEvents);

			if (netEvents.lNetworkEvents & FD_ACCEPT) // ���� ��û�� ���
			{
				if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				{
					ret = GNetSocket::err_e::ACCEPT;
					break;
				}

				int client_addr_size = sizeof(clnt_addr);
				hClntSock = accept(m_SockArray[i % WSA_MAXIMUM_WAIT_EVENTS], (SOCKADDR*)&clnt_addr, &client_addr_size);
				if (hClntSock == INVALID_SOCKET)
				{
					ret = GNetSocket::err_e::ACCEPT;
					break;
				}
				wsaEvent = WSACreateEvent();
				WSAEventSelect(hClntSock, wsaEvent, FD_READ | FD_CLOSE);

				m_SockArray[m_SocketCnt % WSA_MAXIMUM_WAIT_EVENTS] = hClntSock;
				m_EventArray[m_SocketCnt % WSA_MAXIMUM_WAIT_EVENTS] = wsaEvent;
				m_ClientsAddr[m_SocketCnt % WSA_MAXIMUM_WAIT_EVENTS] = clnt_addr;
				m_IsConnected[m_SocketCnt % WSA_MAXIMUM_WAIT_EVENTS] = true;

				m_SocketCnt++;
				TRACE("���� ���� :%d \n", hClntSock);


				// �������� ������ Ŭ���̾�Ʈ ip �ּҴ� ��� �� �� �ִ°�?
				// �ּ� ���� char 14  - ccc.ccc.ccc.cccc
				sprintf_s(message, sizeof(message), "accepted client ip [%s]", m_ClientsAddr[m_SocketCnt % WSA_MAXIMUM_WAIT_EVENTS].sin_zero);

				if (m_cbObj)
				{
					m_func(m_cbObj, 0, &message);
				}

				if (!(m_SocketCnt < WSA_MAXIMUM_WAIT_EVENTS)) // Client������ �ʰ��ϸ� Accept�� ������� �ʴ´�.
				{
					WSAEventSelect(m_SockArray[0], m_EventArray[0], 0);
				}

			}

			if (netEvents.lNetworkEvents & FD_READ) // ������ ������ ���
			{
				if (netEvents.iErrorCode[FD_READ_BIT] != 0)
				{
					ret = GNetSocket::err_e::READ;
					break;
				}

				is_receive_cplt = false;
				flags = 0;
				memset(message, 0, sizeof(message));
				memset(&overlapped, 0, sizeof(overlapped));
				m_dataBuf.len = NETSOCKET_MAX_BUFF_SIZE - 1;
				m_dataBuf.buf = message;

				if (WSARecv(m_SockArray[i], &m_dataBuf, 1, &g_irecvBytes, &flags, &overlapped, CompletionRoutine) == SOCKET_ERROR)
				{
					ret = GNetSocket::err_e::READ;
					break;
				}

				if (g_irecvBytes > 0)
				{
					is_receive_cplt = true;

					//���� ������ Q ���ۿ� ����

					/* strBuf.Format("%s", message);
					pMain->m_strQue[eSingleSessionMate].Push(strBuf);
					pMain->m_bReceived[eSingleSessionMate] = TRUE;*/

				}

				break;
			}

			if (netEvents.lNetworkEvents & FD_CLOSE)
			{
				WSACloseEvent(m_EventArray[i]);
				closesocket(m_SockArray[i]);
				TRACE("Socket ���� :%d \n", m_SockArray[i]);

				m_IsConnected[i % WSA_MAXIMUM_WAIT_EVENTS] = false;

				m_SocketCnt--;
				for (uint32_t j = i; j < m_SocketCnt; j++)
				{
					//m_SockArray[j % WSA_MAXIMUM_WAIT_EVENTS] = m_SockArray[j + 1];
					//m_EventArray[j % WSA_MAXIMUM_WAIT_EVENTS] = m_EventArray[j + 1];
					//m_IsConnected[j % WSA_MAXIMUM_WAIT_EVENTS] = m_IsConnected[j + 1];
					//m_IsReceveCplt[i % WSA_MAXIMUM_WAIT_EVENTS] = m_IsReceveCplt[j + 1];

					/*pMain->m_strQue[j].Reset();
					for (;;)
					{
					  if (pMain->m_strQue[j + 1].IsEmpty()) break;
					  strBuf = pMain->m_strQue[j + 1].Pop();
					  pMain->m_strQue[j].Push(strBuf);
					}*/
				}

				/*sprintf_s(message, sizeof(message), "%s", pMain->m_strMateIP[i].GetBuffer());
				if (pMain->m_hNotifyHandle != NULL)
				  ::PostMessage(pMain->m_hNotifyHandle, pMain->m_uiCloseMessage, i, (LPARAM)(pMain));*/

				if (m_SocketCnt == WSA_MAXIMUM_WAIT_EVENTS) // Client������ ���� ������ Accept�� ����Ѵ�.
				{
					WSAEventSelect(m_SockArray[0], m_EventArray[0], FD_ACCEPT);
				}
			}
		}

		UTL::delay(2);
	}



	return ret;
}

GNetSocket::err_e GNetSocket::threadJob_Client(void)
{
	GNetSocket::err_e ret = GNetSocket::err_e::SUCCESS;
	int result = 0;
	int index = 0;

	WSAEVENT dummyEvent;
	WSANETWORKEVENTS netEvents;

	char message[NETSOCKET_MAX_BUFF_SIZE];
	bool	is_receive_cplt = false;


	unsigned long flags = 0;
	WSAOVERLAPPED overlapped;

	dummyEvent = WSACreateEvent();

	initClientSocket();

	m_IsOpened = true;

	while (m_TrdLife)
	{
		result = WSAWaitForMultipleEvents(1, &m_clientEvent, TRUE, 1000, FALSE);
		if (result == WSA_WAIT_TIMEOUT || result == WSA_WAIT_FAILED)
			continue;

		WSAEnumNetworkEvents(m_clientSocket, m_clientEvent, &netEvents);

		if (netEvents.lNetworkEvents & FD_READ) // 
		{
			if (netEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				ret = GNetSocket::err_e::READ;
				break;
			}

			is_receive_cplt = false;
			flags = 0;
			memset(message, 0, sizeof(message));
			memset(&overlapped, 0, sizeof(overlapped));
			m_dataBuf.len = NETSOCKET_MAX_BUFF_SIZE - 1;
			m_dataBuf.buf = message;

			if (WSARecv(m_clientSocket, &m_dataBuf, 1, &g_irecvBytes, &flags, &overlapped, CompletionRoutine) == SOCKET_ERROR)
			{
				ret = GNetSocket::err_e::READ;
				break;
			}


			result = WSAWaitForMultipleEvents(1, &dummyEvent, FALSE, WSA_INFINITE, TRUE);
			if (result != WAIT_IO_COMPLETION)
			{
				ret = GNetSocket::err_e::READ;
				break;
			}

			if (g_irecvBytes > 0)
			{
				/*if(message[g_irecvBytes-1] == pMain->m_cEndChar)
				{
				  message[g_irecvBytes-1] = 0x00;
				  bReceiveAll = TRUE;
				}*/
				//for (int i = 0; i < g_irecvBytes; i++)
				//{
				//	//if (message[i] == pMain->m_cEndChar)
				//	{
				//		message[i] = 0x00;
				//		bReceiveAll = TRUE;
				//	}
				//}
				is_receive_cplt = TRUE;
				//strBuf.Format("%s", message);
				//pMain->m_strQue[eSingleSessionMate].Push(strBuf);

				if (is_receive_cplt)
				{
					//m_IsReceveCplt[1] = true;

					//sprintf_s(message, sizeof(message), "%s", "connection cplt"); 
					if (m_cbObj)
					{
						m_func(m_cbObj, &g_irecvBytes, &message);
					}


					is_receive_cplt = false;
					m_dataBuf = {};

				}
			}
		}

		if (netEvents.lNetworkEvents & FD_CLOSE)
		{
			//			if(netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
			//			{
			//				nResult = ERR_CLOSE;
			//				//break;
			//			}
			closeSocket();
			TRACE("Socket close \n");

			sprintf_s(message, sizeof(message), "%s", "disconnected, try again connecting");
			if (m_cbObj)
			{
				m_func(m_cbObj, 0, &message);
			}

			initClientSocket();	///		
		}

		UTL::delay(2);
	}

	return ret;
}

GNetSocket::err_e GNetSocket::threadJob_ClientUDP(void)
{
	return GNetSocket::err_e::SUCCESS;
}



bool GNetSocket::IsOpened() const
{
	return m_IsOpened;
}

void GNetSocket::AttCallbackFunc(void* obj, evt_cb cb)
{
	m_cbObj = obj;
	m_func = cb;
}

errno_t GNetSocket::Open()
{
	GNetSocket::err_e ret = GNetSocket::err_e::SUCCESS;
	if (m_IsOpened)
		return static_cast<errno_t>(ret);

	/*
	ret = iniComm();
	if (ret != netSocket::err_e::SUCCESS)
	  return ret;*/

	threadRun();

	return static_cast<errno_t>(ret);
}

void GNetSocket::Close()
{
	//stop thread
	threadStop();
	// close port
	closeSocket();
}

void GNetSocket::Recovery()
{
}

int GNetSocket::SendData(char* p_str, uint32_t length)
{
	int result = 0;
	SOCKET sock;

	if (m_Cfg.mode == GNetSocket::mode_e::TCP_Client)
	{
		sock = m_clientSocket;
	}
	else
	{
		//�ӽ÷�
		sock = m_SockArray[2];
	}

	unsigned long flags = 0;
	WSAOVERLAPPED overlapped;

	memset(&overlapped, 0, sizeof(overlapped));
	WSAEVENT eVent = WSACreateEvent();
	overlapped.hEvent = eVent;


	m_dataBuf.len = length;
	m_dataBuf.buf = p_str;

	result = WSASend(sock, &m_dataBuf, 1, &g_irecvBytes, 0, &overlapped, CompletionRoutine);
	if (result == SOCKET_ERROR)
	{
		return -1;
	}


	result = WSAWaitForMultipleEvents(1, &eVent, FALSE, WSA_INFINITE, TRUE);

	if (result != WAIT_IO_COMPLETION)
		return -1;

	return result;
}



UINT GNetSocket::threadFunc(LPVOID pParam)
{
	GNetSocket* pThis = (GNetSocket*)pParam;
	GNetSocket::err_e ret = GNetSocket::err_e::SUCCESS;
	TRACE("ethernet socket Thread Start\n\r");

	while (pThis->m_TrdLife)
	{

		switch (pThis->m_Cfg.mode)
		{
		case GNetSocket::mode_e::TCP_Server:
			ret = pThis->threadJob_Server();
			break;

		case GNetSocket::mode_e::TCP_Client:
			ret = pThis->threadJob_Client();
			break;

		case GNetSocket::mode_e::UDP_Client:
			ret = pThis->threadJob_ClientUDP();
			break;

		default:
			break;
		}

		if (ret != GNetSocket::err_e::SUCCESS)
			break;

		UTL::delay(2);
	}

	TRACE("ethernet socket Thread Terminatet\n\r");
	pThis->m_TrdId = 0;
	pThis->m_hThread = NULL;
	return 0;
}


#endif