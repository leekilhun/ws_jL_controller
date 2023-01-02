#pragma once
#include "afxdialogex.h"


// Cui_popMcuData 대화 상자
class CVPRemoteDlg;

class Cui_PopMcuData : public CDialogEx
{
	UINT_PTR m_TimerID;
	CVPRemoteDlg* m_pParent;

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
};
