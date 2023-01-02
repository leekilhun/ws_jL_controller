// popMcuData.cpp: 구현 파일
//

#include "pch.h"
#include "VPRemote.h"
#include "afxdialogex.h"
#include "popMcuData.h"

#include "VPRemoteDlg.h"
// Cui_popMcuData 대화 상자

IMPLEMENT_DYNAMIC(Cui_PopMcuData, CDialogEx)

Cui_PopMcuData::Cui_PopMcuData(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MCU_DATA, pParent), m_TimerID{}, m_pParent{}
{
	m_pParent = (CVPRemoteDlg*)pParent;
}

Cui_PopMcuData::~Cui_PopMcuData()
{
}

void Cui_PopMcuData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Cui_PopMcuData, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Cui_popMcuData 메시지 처리기


BOOL Cui_PopMcuData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Cui_PopMcuData::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER_POP_MCU_DATA_DLG, 10, NULL);
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


void Cui_PopMcuData::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	update();
	CDialogEx::OnTimer(nIDEvent);
}


void Cui_PopMcuData::update()
{
	// TODO: 여기에 구현 코드 추가.
}
