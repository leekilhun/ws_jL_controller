// ui\popMoonsControl.cpp: 구현 파일
//

#include "pch.h"
#include "VPRemote.h"

#include "afxdialogex.h"
#include "popMoonsControl.h"

#include "VPRemoteDlg.h"
// CpopMoonsControl 대화 상자

constexpr int select_cw = 0;
constexpr int select_ccw = 1;

constexpr int select_low_level = 0;
constexpr int select_high_level = 1;


constexpr wchar_t* DEF_MOONS_EDIT_JOG_VELOCITY(L"10");
constexpr wchar_t* DEF_MOONS_EDIT_JOG_ACCDEC(L"100");

constexpr wchar_t* DEF_MOONS_EDIT_INIT_VELOCITY(L"3");
constexpr wchar_t* DEF_MOONS_EDIT_INIT_OFFSET(L"0");
constexpr wchar_t* DEF_MOONS_EDIT_INIT_ACC_DEC(L"100");

constexpr wchar_t* DEF_MOONS_EDIT_MOVE_VELOCITY(L"15");
constexpr wchar_t* DEF_MOONS_EDIT_MOVE_DECEL(L"100");
constexpr wchar_t* DEF_MOONS_EDIT_MOVE_ACCEL(L"100");

constexpr wchar_t* DEF_MOONS_EDIT_POSMODE_ABSMOVE_POS(L"20000");
constexpr wchar_t* DEF_MOONS_EDIT_POSMODE_RELMOVE_POS(L"20000");





IMPLEMENT_DYNAMIC(Cui_PopMoonsControl, CDialogEx)

Cui_PopMoonsControl::Cui_PopMoonsControl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOONSMODBUS_DIALOG, pParent), m_TimerID{}, m_pParent{}, m_pSystem{}
	, m_txtDriveStatus{}, m_txtAlarmStatus{}, m_txtActSpeed{}, m_txtTargetSpeed{}, m_txtEncodePos{}, m_txtCmdPos{}
{
	m_pParent = (CVPRemoteDlg*)pParent;
	m_pSystem = (AP_SYS::MSystem*)(CVPRemoteApp*)theApp.GetSystem();
	m_pPeeler = m_pSystem->GetModulePeelerComponent();
}

Cui_PopMoonsControl::~Cui_PopMoonsControl()
{
}

void Cui_PopMoonsControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOONS_TXT_DRIVESTATUS, m_txtDriveStatus);
	DDX_Control(pDX, IDC_MOONS_TXT_ALARMCODE, m_txtAlarmStatus);
	DDX_Control(pDX, IDC_MOONS_TXT_ACTUALSPEED, m_txtActSpeed);
	DDX_Control(pDX, IDC_MOONS_TXT_TARGETSPEED, m_txtTargetSpeed);
	DDX_Control(pDX, IDC_MOONS_TXT_ENCODERPOSITION, m_txtEncodePos);
	DDX_Control(pDX, IDC_MOONS_TXT_CMD_POSITION, m_txtCmdPos);
	DDX_Control(pDX, IDC_MOONS_CMB_POSMOVE_DIR, m_cmbDirection);
	DDX_Control(pDX, IDC_MOONS_COMB_INIT_DIR, m_cmbInitDir);
	DDX_Control(pDX, IDC_MOONS_COMB_INIT_X_PIN, m_cmbInitXPin);
	DDX_Control(pDX, IDC_MOONS_COMB_INIT_X_LEVEL, m_cmbInitLvl);
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
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MOONS_BTN_MOVE_PARAM_SET, &Cui_PopMoonsControl::OnBnClickedMoonsBtnMoveParamSet)
	ON_BN_CLICKED(IDC_MOONS_BTN_INIT_PARAM_SET, &Cui_PopMoonsControl::OnBnClickedMoonsBtnInitParamSet)
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
	m_pPeeler->MotorOrigin(m_pParent->m_motorIdx);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeAbsmoveStart()
{
	int value = GetDlgItemInt(IDC_MOONS_EDIT_POSMODE_ABSMOVE_POS);
	m_pPeeler->Move(m_pParent->m_motorIdx, value);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeAbsmoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pPeeler->Stop(m_pParent->m_motorIdx);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeRelmoveStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int value = GetDlgItemInt(IDC_MOONS_EDIT_POSMODE_RELMOVE_POS);
	int dir = m_cmbDirection.GetCurSel();
	if (dir == select_ccw)
	{
		value *= -1;
	}

	m_pPeeler->RelMove(m_pParent->m_motorIdx, value);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnPosmodeRelmoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pPeeler->Stop(m_pParent->m_motorIdx);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnJogPosmodeMoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pPeeler->Stop(m_pParent->m_motorIdx);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnJogMoveCcw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};
	m_pParent->LockUpdate();
	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	idx = 0;
	datas.clear();
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MOONS_EDIT_JOG_VELOCITY));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MOONS_EDIT_JOG_ACCDEC));
	idx = make_packet(idx, -1000);



	m_pPeeler->JogMove(m_pParent->m_motorIdx, datas);
	m_pParent->LockUpdate(true);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnJogMoveCw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};
	m_pParent->LockUpdate();
	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	idx = 0;
	datas.clear();
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MOONS_EDIT_JOG_VELOCITY));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MOONS_EDIT_JOG_ACCDEC));
	idx = make_packet(idx, 1000);



	m_pPeeler->JogMove(m_pParent->m_motorIdx, datas);
	m_pParent->LockUpdate(true);
	//m_pPeeler->JogMove(m_pParent->m_motorIdx);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnAlarmReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.ClearState
	m_pPeeler->ClearState(m_pParent->m_motorIdx);

}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnGetState()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorEncoderZero()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pPeeler->ZereSetMotorEncoder(m_pParent->m_motorIdx);
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorSeekhome()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pPeeler->MotorOnOff(m_pParent->m_motorIdx, false);
	//~(((CButton*)GetDlgItem(IDC_MOONS_CHECK_MOTOR_ON))->GetCheck())
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMotorOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pPeeler->MotorOnOff(m_pParent->m_motorIdx, true);
	//~(((CButton*)GetDlgItem(IDC_MOONS_CHECK_MOTOR_ON))->GetCheck())
}


BOOL Cui_PopMoonsControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	((CButton*)GetDlgItem(IDC_MOONS_RADIO_NODE_1))->SetCheck(TRUE);



	{
		CString str{};
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_JOG_VELOCITY))->SetWindowText(DEF_MOONS_EDIT_JOG_VELOCITY);
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_JOG_ACCDEC))->SetWindowText(DEF_MOONS_EDIT_JOG_ACCDEC);

		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_MOVE_VELOCITY))->SetWindowText(DEF_MOONS_EDIT_MOVE_VELOCITY);
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_MOVE_DECEL))->SetWindowText(DEF_MOONS_EDIT_MOVE_DECEL);
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_MOVE_ACCEL))->SetWindowText(DEF_MOONS_EDIT_MOVE_ACCEL);
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_INIT_VELOCITY))->SetWindowText(DEF_MOONS_EDIT_INIT_VELOCITY);
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_INIT_ACC_DEC))->SetWindowText(DEF_MOONS_EDIT_INIT_ACC_DEC);
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_INIT_OFFSET))->SetWindowText(DEF_MOONS_EDIT_INIT_OFFSET);

		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_POSMODE_ABSMOVE_POS))->SetWindowText(DEF_MOONS_EDIT_POSMODE_ABSMOVE_POS);
		((CEdit*)GetDlgItem(IDC_MOONS_EDIT_POSMODE_RELMOVE_POS))->SetWindowText(DEF_MOONS_EDIT_POSMODE_RELMOVE_POS);
	}

	{
		m_cmbDirection.InsertString(select_cw, L"CW");
		m_cmbDirection.InsertString(select_ccw, L"CCW");
		m_cmbDirection.SetCurSel(0);

		m_cmbInitDir.InsertString(select_cw, L"CW");
		m_cmbInitDir.InsertString(select_ccw, L"CCW");
		m_cmbInitDir.SetCurSel(0);

		m_cmbInitXPin.InsertString(0, L"3");
		m_cmbInitXPin.InsertString(1, L"4");
		m_cmbInitXPin.InsertString(2, L"5");
		m_cmbInitXPin.SetCurSel(2);

		m_cmbInitLvl.InsertString(0, L"L");
		m_cmbInitLvl.InsertString(1, L"H");
		m_cmbInitLvl.SetCurSel(0);


	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Cui_PopMoonsControl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER_POP_MOTOR_DATA_DLG, 10, NULL);
	}
	else
	{
		if (m_TimerID != 0)
		{
			KillTimer(m_TimerID);
			m_TimerID = 0;
		}
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void Cui_PopMoonsControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	update();
	CDialogEx::OnTimer(nIDEvent);
}


void Cui_PopMoonsControl::update()
{
	// TODO: 여기에 구현 코드 추가.
	if (((CButton*)GetDlgItem(IDC_MOONS_RADIO_NODE_3))->GetCheck())
	{
		m_pParent->m_motorIdx = MCU_OBJ::MOTOR_HIGH;
	}
	else if (((CButton*)GetDlgItem(IDC_MOONS_RADIO_NODE_2))->GetCheck())
	{
		m_pParent->m_motorIdx = MCU_OBJ::MOTOR_ROLL;
	}
	else //if (((CButton*)GetDlgItem(IDC_MOONS_RADIO_NODE_1))->GetCheck())
	{
		m_pParent->m_motorIdx = MCU_OBJ::MOTOR_JIG;
	}




	HAL::ModulePeeler::moons_data_st motor_data{};
	m_pPeeler->GetMotorData(motor_data, m_pParent->m_motorIdx);

	CString str{};

	if (motor_data.drv_status.Motor_Enabled)
	{
		str.Format(L"Motor Enable");
		((CButton*)GetDlgItem(IDC_MOONS_CHECK_MOTOR_ON))->SetWindowText(str);
		((CButton*)GetDlgItem(IDC_MOONS_CHECK_MOTOR_ON))->SetCheck(TRUE);
	}
	else
	{
		str.Format(L"Motor Disable");
		((CButton*)GetDlgItem(IDC_MOONS_CHECK_MOTOR_ON))->SetWindowText(str);
		((CButton*)GetDlgItem(IDC_MOONS_CHECK_MOTOR_ON))->SetCheck(FALSE);
	}

	str.Format(L"0x%04X", motor_data.drv_status.sc_status);
	m_txtDriveStatus.SetWindowText(str);

	str.Format(L"0x%04X", motor_data.al_code.al_status);
	m_txtAlarmStatus.SetWindowText(str);

	str.Format(L"%d", motor_data.immediate_act_velocity);
	m_txtActSpeed.SetWindowText(str);

	str.Format(L"%d", motor_data.immediate_target_velocity);
	m_txtTargetSpeed.SetWindowText(str);

	str.Format(L"%d", motor_data.encoder_position);
	m_txtEncodePos.SetWindowText(str);

	str.Format(L"%d", motor_data.immediate_abs_position);
	m_txtCmdPos.SetWindowText(str);


	for (int i = 0; i < 16; i++)
	{
		((CButton*)GetDlgItem(IDC_MT_STATE_1 + i))->SetCheck(((motor_data.drv_status.sc_status >> i) & 1));
	}


	{
		((CButton*)GetDlgItem(IDC_MT_FT_ALARM_1))->SetCheck(motor_data.al_code.Position_Error);
		((CButton*)GetDlgItem(IDC_MT_FT_ALARM_2))->SetCheck(motor_data.al_code.Over_Temp);
		((CButton*)GetDlgItem(IDC_MT_FT_ALARM_3))->SetCheck(motor_data.al_code.Over_Voltage);
		((CButton*)GetDlgItem(IDC_MT_FT_ALARM_4))->SetCheck(motor_data.al_code.Internal_Voltage);
		((CButton*)GetDlgItem(IDC_MT_FT_ALARM_5))->SetCheck(motor_data.al_code.Over_Current);
		((CButton*)GetDlgItem(IDC_MT_FT_ALARM_6))->SetCheck(motor_data.al_code.Open_Winding);
		((CButton*)GetDlgItem(IDC_MT_FT_ALARM_7))->SetCheck(motor_data.al_code.Bad_Encoder);


		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_1))->SetCheck(motor_data.al_code.CCW_Limit);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_2))->SetCheck(motor_data.al_code.CW_Limit);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_3))->SetCheck(motor_data.al_code.Under_Voltage);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_4))->SetCheck(motor_data.al_code.No_Move);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_5))->SetCheck(motor_data.al_code.Current_Foldback);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_6))->SetCheck(motor_data.al_code.Blank_Q_Segment);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_7))->SetCheck(motor_data.al_code.Bad_Flash);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_8))->SetCheck(motor_data.al_code.NV_Memory_Double_err);
		((CButton*)GetDlgItem(IDC_MT_WR_ALARM_9))->SetCheck(motor_data.al_code.Comm_Error);
	}

	{
		for (int i = 0; i < 8; i++)
		{
			((CButton*)GetDlgItem(IDC_MT_IO_IN_01 + i))->SetCheck(((motor_data.driver_board_inputs >> i) & 1));
		}
		/*
		for (int i = 0; i < 4; i++)
		{

			IDC_MT_IO_OUT_1;
			IDC_MT_IO_OUT_2;
			IDC_MT_IO_OUT_3;
			IDC_MT_IO_OUT_4;
		}
		*/

	}

}


void Cui_PopMoonsControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	/*
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	// Select a bitmap into the memory device context
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, 100, 100);
	memDC.SelectObject(&bmp);

	// Perform the drawing operation
	memDC.FillSolidRect(0, 0, 100, 100, RGB(255, 0, 0));

	// Copy the off-screen bitmap to the window
	dc.BitBlt(0, 0, 100, 100, &memDC, 0, 0, SRCCOPY);
	*/
}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnMoveParamSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};
	m_pParent->LockUpdate();
	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	idx = 0;
	datas.clear();
	idx = make_packet(idx, GetDlgItemInt(IDC_MOONS_EDIT_MOVE_VELOCITY));
	idx = make_packet(idx, GetDlgItemInt(IDC_MOONS_EDIT_MOVE_DECEL));
	idx = make_packet(idx, GetDlgItemInt(IDC_MOONS_EDIT_MOVE_ACCEL));

	if (m_pPeeler->WriteMoonsParam_Move(datas, m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - set move data");
	}
	m_pParent->LockUpdate(true);

}


void Cui_PopMoonsControl::OnBnClickedMoonsBtnInitParamSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};

	m_pParent->LockUpdate();
	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	using param_t = HAL::ModulePeeler::origin_param_st;
	param_t params{};
	CString str;
	int dir = (m_cmbInitDir.GetCurSel() == select_cw ? 1000 : -1000);
	uint8_t x_pin = (uint8_t)(m_cmbInitXPin.GetCurSel() + 3);
	uint8_t x_level = (m_cmbInitLvl.GetCurSel() == select_low_level ? 'L' : 'H');

	idx = 0;
	datas.clear();
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MOONS_EDIT_INIT_VELOCITY));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MOONS_EDIT_INIT_ACC_DEC));
	idx = make_packet(idx, GetDlgItemInt(IDC_MOONS_EDIT_INIT_OFFSET));
	idx = make_packet(idx, dir);
	idx = make_packet(idx, x_pin);
	idx = make_packet(idx, x_level);

	if (m_pPeeler->WriteMoonsParam_Init(datas, m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - set init data");
	}
	m_pParent->LockUpdate(true);

}
