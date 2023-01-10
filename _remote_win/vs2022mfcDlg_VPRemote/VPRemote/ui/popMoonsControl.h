#pragma once

// CpopMoonsControl 대화 상자
class CVPRemoteDlg;

class Cui_PopMoonsControl : public CDialogEx
{
	UINT_PTR m_TimerID;
	CVPRemoteDlg* m_pParent;
	AP_SYS::MSystem* m_pSystem;
	HAL::ModulePeeler* m_pPeeler;


	DECLARE_DYNAMIC(Cui_PopMoonsControl)

public:
	Cui_PopMoonsControl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Cui_PopMoonsControl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOONSMODBUS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMtIoOut1();
	afx_msg void OnBnClickedMtIoOut2();
	afx_msg void OnBnClickedMtIoOut3();
	afx_msg void OnBnClickedMtIoOut4();
	afx_msg void OnBnClickedMoonsBtnOrg();
	afx_msg void OnBnClickedMoonsBtnPosmodeAbsmoveStart();
	afx_msg void OnBnClickedMoonsBtnPosmodeAbsmoveStop();
	afx_msg void OnBnClickedMoonsBtnPosmodeRelmoveStart();
	afx_msg void OnBnClickedMoonsBtnPosmodeRelmoveStop();
	afx_msg void OnBnClickedMoonsBtnJogPosmodeMoveStop();
	afx_msg void OnBnClickedMoonsBtnJogMoveCcw();
	afx_msg void OnBnClickedMoonsBtnJogMoveCw();
	afx_msg void OnBnClickedMoonsBtnAlarmReset();
	afx_msg void OnBnClickedMoonsBtnGetState();
	afx_msg void OnBnClickedMoonsBtnMotorEncoderZero();
	afx_msg void OnBnClickedMoonsBtnMotorSeekhome();
	afx_msg void OnBnClickedMoonsBtnMotorOff();
	afx_msg void OnBnClickedMoonsBtnMotorOn();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void update();
	CEdit m_txtDriveStatus;
	CEdit m_txtAlarmStatus;
	CEdit m_txtActSpeed;
	CEdit m_txtTargetSpeed;
	CEdit m_txtEncodePos;
	CEdit m_txtCmdPos;
public:
	afx_msg void OnPaint();
private:
	CComboBox m_cmbDirection;
public:
	afx_msg void OnBnClickedMoonsBtnMoveParamSet();
	afx_msg void OnBnClickedMoonsBtnInitParamSet();
};
