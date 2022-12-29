
// VPRemoteDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "VPRemote.h"


#include "VPRemoteDlg.h"
#include "afxdialogex.h"

//3. 추가할 헤더
#include "ui/popMoonsControl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVPRemoteDlg 대화 상자

static void receiveMainDlgCB(void* obj, void* w_parm, void* l_parm);

CVPRemoteDlg::CVPRemoteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VPREMOTE_DIALOG, pParent),m_popMotor{}, m_pSystem{}, m_TimerID{}
	, m_lockUpdate{}
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
	ON_BN_CLICKED(IDC_PEELER_MAIN_POP_MOTOR_CTRL, &CVPRemoteDlg::OnBnClickedPeelerMainPopMotorCtrl)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PEELER_MAIN_MOTOR_STOP, &CVPRemoteDlg::OnBnClickedPeelerMainMotorStop)
	ON_BN_CLICKED(IDC_PEELER_MAIN_OP_START, &CVPRemoteDlg::OnBnClickedPeelerMainOpStart)
	ON_BN_CLICKED(IDC_PEELER_MAIN_OP_STOP, &CVPRemoteDlg::OnBnClickedPeelerMainOpStop)
	ON_BN_CLICKED(IDC_PEELER_MAIN_OP_RESET, &CVPRemoteDlg::OnBnClickedPeelerMainOpReset)
	ON_BN_CLICKED(IDC_PEELER_MAIN_INIT, &CVPRemoteDlg::OnBnClickedPeelerMainInit)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CHECK_MOTOR_ONOFF, &CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOnoff)
	ON_BN_CLICKED(IDC_PEELER_MAIN_MOTOR_RESET, &CVPRemoteDlg::OnBnClickedPeelerMainMotorReset)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CHECK_MOTOR_ORG, &CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOrg)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_1, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderFor)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_2, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderBack)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_3, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigOpen)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_4, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigGrip)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_5, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigClose)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_6, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylLock)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_7, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylUnlock)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_8, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylPush)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_9, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylBack)
	ON_BN_CLICKED(IDC_CHECK_STATE_0, &CVPRemoteDlg::OnBnClickedCheckState0)
	ON_BN_CLICKED(IDC_CHECK_STATE_1, &CVPRemoteDlg::OnBnClickedCheckState1)
	ON_BN_CLICKED(IDC_CHECK_STATE_2, &CVPRemoteDlg::OnBnClickedCheckState2)
	ON_BN_CLICKED(IDC_CHECK_STATE_3, &CVPRemoteDlg::OnBnClickedCheckState3)
	ON_BN_CLICKED(IDC_CHECK_STATE_4, &CVPRemoteDlg::OnBnClickedCheckState4)
	ON_BN_CLICKED(IDC_CHECK_STATE_5, &CVPRemoteDlg::OnBnClickedCheckState5)
	ON_BN_CLICKED(IDC_CHECK_STATE_6, &CVPRemoteDlg::OnBnClickedCheckState6)
	ON_BN_CLICKED(IDC_CHECK_STATE_7, &CVPRemoteDlg::OnBnClickedCheckState7)
	ON_BN_CLICKED(IDC_CHECK_STATE_8, &CVPRemoteDlg::OnBnClickedCheckState8)
	ON_BN_CLICKED(IDC_CHECK_STATE_9, &CVPRemoteDlg::OnBnClickedCheckState9)
	ON_BN_CLICKED(IDC_CHECK_STATE_10, &CVPRemoteDlg::OnBnClickedCheckState10)
	ON_BN_CLICKED(IDC_CHECK_STATE_11, &CVPRemoteDlg::OnBnClickedCheckState11)
	ON_BN_CLICKED(IDC_CHECK_STATE_12, &CVPRemoteDlg::OnBnClickedCheckState12)
	ON_BN_CLICKED(IDC_CHECK_STATE_13, &CVPRemoteDlg::OnBnClickedCheckState13)
	ON_BN_CLICKED(IDC_CHECK_STATE_14, &CVPRemoteDlg::OnBnClickedCheckState14)
	ON_BN_CLICKED(IDC_CHECK_STATE_15, &CVPRemoteDlg::OnBnClickedCheckState15)

	ON_BN_CLICKED(IDC_CHECK_STATE_16, &CVPRemoteDlg::OnBnClickedCheckOption0)
	ON_BN_CLICKED(IDC_CHECK_STATE_17, &CVPRemoteDlg::OnBnClickedCheckOption1)
	ON_BN_CLICKED(IDC_CHECK_STATE_18, &CVPRemoteDlg::OnBnClickedCheckOption2)
	ON_BN_CLICKED(IDC_CHECK_STATE_19, &CVPRemoteDlg::OnBnClickedCheckOption3)
	ON_BN_CLICKED(IDC_CHECK_STATE_20, &CVPRemoteDlg::OnBnClickedCheckOption4)
	ON_BN_CLICKED(IDC_CHECK_STATE_21, &CVPRemoteDlg::OnBnClickedCheckOption5)
	ON_BN_CLICKED(IDC_CHECK_STATE_22, &CVPRemoteDlg::OnBnClickedCheckOption6)
	ON_BN_CLICKED(IDC_CHECK_STATE_23, &CVPRemoteDlg::OnBnClickedCheckOption7)
	ON_BN_CLICKED(IDC_CHECK_STATE_24, &CVPRemoteDlg::OnBnClickedCheckOption8)
	ON_BN_CLICKED(IDC_CHECK_STATE_25, &CVPRemoteDlg::OnBnClickedCheckOption9)
	ON_BN_CLICKED(IDC_CHECK_STATE_26, &CVPRemoteDlg::OnBnClickedCheckOption10)
	ON_BN_CLICKED(IDC_CHECK_STATE_27, &CVPRemoteDlg::OnBnClickedCheckOption11)
	ON_BN_CLICKED(IDC_CHECK_STATE_28, &CVPRemoteDlg::OnBnClickedCheckOption12)
	ON_BN_CLICKED(IDC_CHECK_STATE_29, &CVPRemoteDlg::OnBnClickedCheckOption13)
	ON_BN_CLICKED(IDC_CHECK_STATE_30, &CVPRemoteDlg::OnBnClickedCheckOption14)
	ON_BN_CLICKED(IDC_CHECK_STATE_31, &CVPRemoteDlg::OnBnClickedCheckOption15)

	ON_BN_CLICKED(IDC_CHECK_STATE_96, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_97, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_98, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_99, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_100, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_101, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_102, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_103, &CVPRemoteDlg::OnBnClickedCheckSetOutput)

	ON_BN_CLICKED(IDC_CHECK_STATE_104, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_105, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_106, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_107, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_108, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_109, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_110, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_111, &CVPRemoteDlg::OnBnClickedCheckSetOutput)

	ON_BN_CLICKED(IDC_CHECK_STATE_112, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_113, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_114, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_115, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_116, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_117, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_118, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_119, &CVPRemoteDlg::OnBnClickedCheckSetOutput)

	ON_BN_CLICKED(IDC_CHECK_STATE_120, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_121, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_122, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_123, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_124, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_125, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_126, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_127, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
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
	if (m_lockUpdate)
		return;

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


void CVPRemoteDlg::OnBnClickedPeelerMainPopMotorCtrl()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_popMotor == nullptr)
	{
		m_popMotor = new Cui_PopMoonsControl(this);
		m_popMotor->Create(IDD_MOONSMODBUS_DIALOG, this);
	}
	m_popMotor->ShowWindow(SW_SHOW);
}


void CVPRemoteDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_popMotor)
	{
		delete m_popMotor;
	}
	m_popMotor = nullptr;
}


void CVPRemoteDlg::OnBnClickedPeelerMainMotorStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainOpStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainOpStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainOpReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMotorReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOrg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderFor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


afx_msg void CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigGrip()
{

}
afx_msg void CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigClose()
{

}

void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylLock()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylUnlock()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylPush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedCheckState0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedCheckState2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState12()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState13()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState14()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState15()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CVPRemoteDlg::OnBnClickedCheckOption0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckOption1()
{
}

void CVPRemoteDlg::OnBnClickedCheckOption2()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption3()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption4()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption5()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption6()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption7()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption8()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption9()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption10()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption11()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption12()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption13()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption14()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption15()
{
}

void CVPRemoteDlg::OnBnClickedCheckSetOutput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_lockUpdate = true;

	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	
	uint32_t output_reg = 0;
	for (int i = 0; i < 32; i++)
	{
		output_reg |= (((CButton*)GetDlgItem(IDC_CHECK_STATE_96 + i))->GetCheck()<<i);
	}

	m_lockUpdate = false;
}
