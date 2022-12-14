
// VPRemoteDlg.h: 헤더 파일
//

#pragma once

class Cui_PopMoonsControl;
class Cui_PopMcuData;

// CVPRemoteDlg 대화 상자
class CVPRemoteDlg : public CDialogEx
{
	Cui_PopMoonsControl* m_popMotor;
	Cui_PopMcuData* m_popMcuData;
	AP_SYS::MSystem* m_pSystem;
	uint8_t m_StepNo;
	UINT_PTR m_TimerID;
	bool m_lockUpdate;
	CStatusBar		m_statusBar;


public:
	MCU_OBJ::MOTOR m_motorIdx;

// 생성입니다.
public:
	CVPRemoteDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VPREMOTE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 수신 데이터 표시
	UTL::_que <CString> m_queStepLog;
	CListBox m_rxListBox;
public:
	void AddRxList(CString str);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void update();

	const wchar_t* stepName(uint8_t step_no);
public:
	void LockUpdate(bool is_release = false);
	afx_msg void OnBnClickedPeelerMainPopMotorCtrl();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedPeelerMainMotorStop();
	afx_msg void OnBnClickedPeelerMainOpStart();
	afx_msg void OnBnClickedPeelerMainOpStop();
	afx_msg void OnBnClickedPeelerMainOpReset();
	afx_msg void OnBnClickedPeelerMainInit();
	afx_msg void OnBnClickedPeelerMainCheckMotorOnoff();
	afx_msg void OnBnClickedPeelerMainMotorReset();
	afx_msg void OnBnClickedPeelerMainCheckMotorOrg();
	afx_msg void OnBnClickedPeelerMainCyl_LoaderFor();
	afx_msg void OnBnClickedPeelerMainCyl_LoaderBack();
	afx_msg void OnBnClickedPeelerMainCyl_JigOpen();
	afx_msg void OnBnClickedPeelerMainCyl_JigGrip();
	afx_msg void OnBnClickedPeelerMainCyl_JigClose();
	afx_msg void OnBnClickedPeelerMainCyl_VinylHoldUp();
	afx_msg void OnBnClickedPeelerMainCyl_VinylBack();
	afx_msg void OnBnClickedPeelerMainCyl_VinylPush();
	afx_msg void OnBnClickedPeelerMainCyl_VinylHoldDown();
	afx_msg void OnBnClickedCheckState0();
	afx_msg void OnBnClickedCheckState1();
	afx_msg void OnBnClickedCheckState2();
	afx_msg void OnBnClickedCheckState3();
	afx_msg void OnBnClickedCheckState4();
	afx_msg void OnBnClickedCheckState5();
	afx_msg void OnBnClickedCheckState6();
	afx_msg void OnBnClickedCheckState7();
	afx_msg void OnBnClickedCheckState8();
	afx_msg void OnBnClickedCheckState9();
	afx_msg void OnBnClickedCheckState10();
	afx_msg void OnBnClickedCheckState11();
	afx_msg void OnBnClickedCheckState12();
	afx_msg void OnBnClickedCheckState13();
	afx_msg void OnBnClickedCheckState14();
	afx_msg void OnBnClickedCheckState15();

	afx_msg void OnBnClickedCheckOption0();
	afx_msg void OnBnClickedCheckOption1();
	afx_msg void OnBnClickedCheckOption2();
	afx_msg void OnBnClickedCheckOption3();
	afx_msg void OnBnClickedCheckOption4();
	afx_msg void OnBnClickedCheckOption5();
	afx_msg void OnBnClickedCheckOption6();
	afx_msg void OnBnClickedCheckOption7();
	afx_msg void OnBnClickedCheckOption8();
	afx_msg void OnBnClickedCheckOption9();
	afx_msg void OnBnClickedCheckOption10();
	afx_msg void OnBnClickedCheckOption11();
	afx_msg void OnBnClickedCheckOption12();
	afx_msg void OnBnClickedCheckOption13();
	afx_msg void OnBnClickedCheckOption14();
	afx_msg void OnBnClickedCheckOption15();
	afx_msg void OnBnClickedCheckSetOutput();
	afx_msg void OnBnClickedPeelerMainPopMcuData();

	void SetMcuOptionBit(HAL::ModulePeeler::mcu_option_e bit_reg, bool enable = true);
	afx_msg void OnBnClickedMainCheckStep1();
	afx_msg void OnBnClickedMainCheckStep2();
	afx_msg void OnBnClickedMainCheckStep3();
	afx_msg void OnBnClickedMainCheckStep4();
	afx_msg void OnBnClickedMainCheckStep5();
	afx_msg void OnBnClickedMainCheckStep6();
	afx_msg void OnBnClickedPeelerMainMoveFunc1();
	afx_msg void OnBnClickedPeelerMainMoveFunc2();
	afx_msg void OnBnClickedPeelerMainMoveFunc3();
	afx_msg void OnBnClickedPeelerMainMoveFunc4();
	afx_msg void OnBnClickedPeelerMainMoveFunc5();
	afx_msg void OnBnClickedPeelerMainMoveFunc6();
	afx_msg void OnBnClickedPeelerMainMoveFunc7();
	afx_msg void OnBnClickedPeelerMainMoveFunc8();
	afx_msg void OnBnClickedPeelerMainMoveFunc9();


private:
	CListBox m_listStepLog;
	void updateStepLog();
	CListBox m_listAlarmLog;
public:
	afx_msg void OnBnClickedMainBtnGetLog();
private:
	void updateMcuLog();
};
