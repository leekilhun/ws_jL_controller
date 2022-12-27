
// VPRemoteDlg.h: 헤더 파일
//

#pragma once


// CVPRemoteDlg 대화 상자
class CVPRemoteDlg : public CDialogEx
{
	AP_SYS::MSystem* m_pSystem;
	UINT_PTR m_TimerID;

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
	CListBox m_rxListBox;
public:
	void AddRxList(CString str);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void update();
};
