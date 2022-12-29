// ui\popMoonsControl.cpp: 구현 파일
//

#include "pch.h"
#include "VPRemote.h"

#include "afxdialogex.h"
#include "ui\popMoonsControl.h"


// CpopMoonsControl 대화 상자

IMPLEMENT_DYNAMIC(Cui_PopMoonsControl, CDialogEx)

Cui_PopMoonsControl::Cui_PopMoonsControl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOONSMODBUS_DIALOG, pParent)
{

}

Cui_PopMoonsControl::~Cui_PopMoonsControl()
{
}

void Cui_PopMoonsControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Cui_PopMoonsControl, CDialogEx)
	ON_BN_CLICKED(IDC_MT_IO_OUT_1, &Cui_PopMoonsControl::OnBnClickedMtIoOut1)
	ON_BN_CLICKED(IDC_MT_IO_OUT_2, &Cui_PopMoonsControl::OnBnClickedMtIoOut2)
	ON_BN_CLICKED(IDC_MT_IO_OUT_3, &Cui_PopMoonsControl::OnBnClickedMtIoOut3)
	ON_BN_CLICKED(IDC_MT_IO_OUT_4, &Cui_PopMoonsControl::OnBnClickedMtIoOut4)
	ON_BN_CLICKED(IDC_MOONS_BTN_ORG, &Cui_PopMoonsControl::OnBnClickedMoonsBtnOrg)
	ON_BN_CLICKED(IDC_MOONS_BTN_POSMODE_ABSMOVE_START, &Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeAbsmoveStart)
	ON_BN_CLICKED(IDC_MOONS_BTN_POSMODE_ABSMOVE_STOP, &Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeAbsmoveStop)
	ON_BN_CLICKED(IDC_MOONS_BTN_POSMODE_RELMOVE_START, &Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeRelmoveStart)
	ON_BN_CLICKED(IDC_MOONS_BTN_POSMODE_RELMOVE_STOP, &Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeRelmoveStop)
	ON_BN_CLICKED(IDC_MOONS_BTN_JOG_POSMODE_MOVE_STOP, &Cui_PopMoonsControl::OnBnClickedMoonsBtnJogPosmodeMoveStop)
	ON_BN_CLICKED(IDC_MOONS_BTN_JOG_MOVE_CCW, &Cui_PopMoonsControl::OnBnClickedMoonsBtnJogMoveCcw)
	ON_BN_CLICKED(IDC_MOONS_BTN_JOG_MOVE_CW, &Cui_PopMoonsControl::OnBnClickedMoonsBtnJogMoveCw)
	ON_BN_CLICKED(IDC_MOONS_BTN_ALARM_RESET, &Cui_PopMoonsControl::OnBnClickedMoonsBtnAlarmReset)
	ON_BN_CLICKED(IDC_MOONS_BTN_GET_STATE, &Cui_PopMoonsControl::OnBnClickedMoonsBtnGetState)
	ON_BN_CLICKED(IDC_MOONS_BTN_MOTOR_ENCODER_ZERO, &Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorEncoderZero)
	ON_BN_CLICKED(IDC_MOONS_BTN_MOTOR_SEEKHOME, &Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorSeekhome)
	ON_BN_CLICKED(IDC_MOONS_BTN_MOTOR_OFF, &Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorOff)
	ON_BN_CLICKED(IDC_MOONS_BTN_MOTOR_ON, &Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorOn)
END_MESSAGE_MAP()


// CpopMoonsControl 메시지 처리기




void Cui_PopMoonsControl::OnBnClickedMtIoOut1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMtIoOut2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMtIoOut3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMtIoOut4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnOrg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeAbsmoveStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeAbsmoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeAbsmoveStop2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeRelmoveStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeRelmoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnJogPosmodeMoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnJogMoveCcw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnJogMoveCw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnAlarmReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnGetState()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorEncoderZero()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorSeekhome()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
