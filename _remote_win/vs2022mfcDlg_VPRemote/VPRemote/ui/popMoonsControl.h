#pragma once

// CpopMoonsControl 대화 상자

class Cui_PopMoonsControl : public CDialogEx
{
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
	afx_msg void OnBnClickedMoonsBtnPosmodeAbsmoveStop2();
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
};
