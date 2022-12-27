
// VPRemoteDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "VPRemote.h"
#include "VPRemoteDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVPRemoteDlg 대화 상자

static void receiveMainDlgCB(void* obj, void* w_parm, void* l_parm);

CVPRemoteDlg::CVPRemoteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VPREMOTE_DIALOG, pParent), m_pSystem{}, m_TimerID{}
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSystem = (AP_SYS::MSystem*)(CVPRemoteApp*)theApp.GetSystem();
	m_pSystem->GetModulePeelerComponent()->AttCallbackFunc(this, receiveMainDlgCB);
}

void CVPRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_LIST_RX, m_rxListBox);
}

BEGIN_MESSAGE_MAP(CVPRemoteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVPRemoteDlg 메시지 처리기

BOOL CVPRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CVPRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CVPRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


static void receiveMainDlgCB(void* obj, void* w_parm, void* l_parm) 
{
	CVPRemoteDlg* p_this = (CVPRemoteDlg*)obj;	

	HAL::ModulePeeler::packet_st rec_data = (*(HAL::ModulePeeler::packet_st*)(l_parm));
	
	CString content;
	CString str;
	std::array<char, 40> datetime{ 0, };
	UTL::trans::DateFormStr(datetime.data(), (int)datetime.size(), UTL::trans::DataTimeFormat_e::YYYYMMDD_HHMMSS_UU);
	str = UTL::trans::CharToCString(datetime.data());
	str.AppendFormat(L" [PC <←- DRIVER] PACKET : ");
	for (int i = 0; i < rec_data.buffer_idx; i++)
	{
		str.AppendFormat(L"%02X ", rec_data.buffer[i]);
	}

	str.AppendFormat(L" response ms: %04d", rec_data.resp_ms);

	p_this->AddRxList(str);
#if 0
	int length = *((int*)w_parm);
	int index = 0;

	for (index = 0; index < length; index++)
	{
		uint8_t data = *((uint8_t*)l_parm + index);
		
	}
#endif

}


void CVPRemoteDlg::AddRxList(CString str)
{
	// TODO: 여기에 구현 코드 추가.
	if (m_rxListBox.GetCount() == 13)
	{
		m_rxListBox.ResetContent();
	}
	m_rxListBox.AddString(str);
	
}


void CVPRemoteDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER_MAIN_DLG, 10, NULL);
	}
	else
	{
		if (m_TimerID != 0)
		{
			KillTimer(m_TimerID);
			m_TimerID = 0;
		}
	}
}


void CVPRemoteDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	update();
	CDialogEx::OnTimer(nIDEvent);
}


void CVPRemoteDlg::update()
{
	// TODO: 여기에 구현 코드 추가.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->GetMcuState();
	peeler->m_stateReg; 

	for (int i = 0; i < 16; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_STATE_0 + i))->SetCheck(((peeler->m_stateReg.ap_state >> i) & 1));
	}
	
	for (int i = 0; i < 16; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_STATE_16 + i))->SetCheck(((peeler->m_optionReg.ap_option >> i) & 1));
	}

	for (int i = 0; i < 32; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_STATE_32 + i))->SetCheck(((peeler->m_errorReg.ap_error >> i) & 1));
	}
	for (int i = 0; i < 32; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_STATE_64 + i))->SetCheck(((peeler->m_inReg >> i) & 1));
	}
	for (int i = 0; i < 32; i++)
	{
		((CButton*)GetDlgItem(IDC_CHECK_STATE_96 + i))->SetCheck(((peeler->m_outReg >> i) & 1));
	}

}
