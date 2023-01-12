#pragma once
#include "afxdialogex.h"


// Cui_popMcuData 대화 상자
class CVPRemoteDlg;

class Cui_PopMcuData : public CDialogEx
{
	UINT_PTR m_TimerID;
	CVPRemoteDlg* m_pParent;
	AP_SYS::MSystem* m_pSystem;
	HAL::ModulePeeler* m_pPeeler;

	DECLARE_DYNAMIC(Cui_PopMcuData)

public:
	Cui_PopMcuData(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Cui_PopMcuData();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MCU_DATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void update();
public:
	afx_msg void OnBnClickedMcuDataBtnPosVelRead();
	afx_msg void OnBnClickedMcuDataBtnPosVelWrite();
	afx_msg void OnBnClickedMcuDataBtnCylinderRead();
	afx_msg void OnBnClickedMcuDataBtnCylinderWrite();
	afx_msg void OnBnClickedMcuDataBtnCfgRead();
	afx_msg void OnBnClickedMcuDataBtnCfgWrite();
	afx_msg void OnBnClickedMcuDataBtnSeqRead();
	afx_msg void OnBnClickedMcuDataBtnSeqWrite();
	afx_msg void OnBnClickedMcuDataBtnRomWriteInit();
private:
	void refreshData();
public:
	afx_msg void OnBnClickedMcuDataBtnRomUpdate();
	afx_msg void OnBnClickedMcuDataBtnVacRead();
	afx_msg void OnBnClickedMcuDataBtnVacWrite();
private:
	void readAllData();
public:
	afx_msg void OnBnClickedMcuDataBtnRomReadAll();
	afx_msg void OnBnClickedMcuDataBtnLinkVelRead();
	afx_msg void OnBnClickedMcuDataBtnLinkVelWrite();
	afx_msg void OnStnClickedMcuDataPosName1();
	afx_msg void OnStnClickedMcuDataPosName2();
	afx_msg void OnStnClickedMcuDataPosName3();
	afx_msg void OnStnClickedMcuDataPosName4();
	afx_msg void OnStnClickedMcuDataPosName5();
	afx_msg void OnStnClickedMcuDataPosName6();
	afx_msg void OnStnClickedMcuDataPosName7();
	afx_msg void OnStnClickedMcuDataPosName8();
	afx_msg void OnStnClickedMcuDataLinkName1();
	afx_msg void OnStnClickedMcuDataLinkName2();
	afx_msg void OnStnClickedMcuDataLinkName3();
	afx_msg void OnStnClickedMcuDataLinkName4();
	afx_msg void OnStnClickedMcuDataLinkName5();
	afx_msg void OnStnClickedMcuDataLinkName6();
	afx_msg void OnStnClickedMcuDataLinkName7();
	afx_msg void OnStnClickedMcuDataLinkName8();
private:
	void movePoseData(int pose, int vel, bool is_link = false);
public:
	afx_msg void OnBnClickedIdcMcuDataBtnMoveStop();
private:
	int constrainRps(int id);
	int constrainDist(int id);
public:
	afx_msg void OnBnClickedMcuDataRadioMtId0();
	afx_msg void OnBnClickedMcuDataRadioMtId1();
	afx_msg void OnBnClickedMcuDataRadioMtId2();
private:
	void updatePoseData();
};
