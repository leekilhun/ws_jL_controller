// popMcuData.cpp: 구현 파일
//

#include "pch.h"
#include "VPRemote.h"
#include "afxdialogex.h"
#include "popMcuData.h"

#include "VPRemoteDlg.h"
// Cui_popMcuData 대화 상자


constexpr wchar_t* DEF_MCU_DATA_POS_NAME_1(L"ready");
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_2(L"loc 1");
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_3(L"loc 2");
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_4(L"loc 3");
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_5(L"loc 4");
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_6(L"loc 5");
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_7(L"loc 6");
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_8(L"loc 7");

constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_0(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_0(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_1(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_1(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_2(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_2(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_3(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_3(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_4(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_4(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_5(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_5(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_6(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_6(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_7(L"100");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_7(L"500");

constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_1(L"jig forback");
constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_2(L"jig openclose");
constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_3(L"hold updown");
constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_4(L"vinyl push");
constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_5(L"cyl 5");
constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_6(L"cyl 6");
constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_7(L"cyl 7");
constexpr wchar_t* DEF_MCU_DATA_CYL_NAME_8(L"cyl 8");

constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_1(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_2(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_3(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_4(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_5(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_6(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_7(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_8(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_9(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_10(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_11(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_12(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_13(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_14(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_15(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CYLCFG_16(L"500");

constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_1(L"vinyl on");
constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_2(L"vac 2");
constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_3(L"vac 3");
constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_4(L"vac 4");
constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_5(L"vac 5");
constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_6(L"vac 6");
constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_7(L"vac 7");
constexpr wchar_t* DEF_MCU_DATA_VAC_NAME_8(L"vac 8");

constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_1(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_2(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_3(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_4(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_5(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_6(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_7(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_8(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_9(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_10(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_11(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_12(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_13(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_14(L"500");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_15(L"1000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_VACCFG_16(L"500");


constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_1(L"cfg 1");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_2(L"cfg 2");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_3(L"cfg 3");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_4(L"cfg 4");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_5(L"cfg 5");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_6(L"cfg 6");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_7(L"cfg 7");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_8(L"cfg 8");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_1(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_2(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_3(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_4(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_5(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_6(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_7(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_8(L"200");

constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_1(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_2(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_3(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_4(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_5(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_6(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_7(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_8(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_1(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_2(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_3(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_4(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_5(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_6(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_7(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_8(L"200");


IMPLEMENT_DYNAMIC(Cui_PopMcuData, CDialogEx)

Cui_PopMcuData::Cui_PopMcuData(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MCU_DATA, pParent), m_TimerID{}, m_pParent{}, m_pSystem{}, m_pPeeler{}
{
	m_pParent = (CVPRemoteDlg*)pParent;
	m_pSystem = (AP_SYS::MSystem*)(CVPRemoteApp*)theApp.GetSystem();
	m_pPeeler = m_pSystem->GetModulePeelerComponent();
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
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_POS_VEL_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnPosVelRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_POS_VEL_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnPosVelWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ACT_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnActRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ACT_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnActWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_CFG_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnCfgRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_CFG_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnCfgWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_SEQ_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnSeqRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_SEQ_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnSeqWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ROM_WRITE_INIT, &Cui_PopMcuData::OnBnClickedMcuDataBtnRomWriteInit)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ROM_UPDATE, &Cui_PopMcuData::OnBnClickedMcuDataBtnRomUpdate)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_VAC_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnVacRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_VAC_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnVacWrite)
END_MESSAGE_MAP()


// Cui_popMcuData 메시지 처리기


BOOL Cui_PopMcuData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	((CButton*)GetDlgItem(IDC_MCU_DATA_RADIO_MT_ID0))->SetCheck(TRUE);
	
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_1))->SetWindowText(DEF_MCU_DATA_POS_NAME_1);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_2))->SetWindowText(DEF_MCU_DATA_POS_NAME_2);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_3))->SetWindowText(DEF_MCU_DATA_POS_NAME_3);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_4))->SetWindowText(DEF_MCU_DATA_POS_NAME_4);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_5))->SetWindowText(DEF_MCU_DATA_POS_NAME_5);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_6))->SetWindowText(DEF_MCU_DATA_POS_NAME_6);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_7))->SetWindowText(DEF_MCU_DATA_POS_NAME_7);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_8))->SetWindowText(DEF_MCU_DATA_POS_NAME_8);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_0))->SetWindowText(DEF_MCU_DATA_EDIT_POS_0);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_0))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_0);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_1))->SetWindowText(DEF_MCU_DATA_EDIT_POS_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_1))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_2))->SetWindowText(DEF_MCU_DATA_EDIT_POS_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_2))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_3))->SetWindowText(DEF_MCU_DATA_EDIT_POS_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_3))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_4))->SetWindowText(DEF_MCU_DATA_EDIT_POS_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_4))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_5))->SetWindowText(DEF_MCU_DATA_EDIT_POS_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_5))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_6))->SetWindowText(DEF_MCU_DATA_EDIT_POS_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_6))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_7))->SetWindowText(DEF_MCU_DATA_EDIT_POS_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_7))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_7);

	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_1))->SetWindowText(DEF_MCU_DATA_CYL_NAME_1);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_2))->SetWindowText(DEF_MCU_DATA_CYL_NAME_2);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_3))->SetWindowText(DEF_MCU_DATA_CYL_NAME_3);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_4))->SetWindowText(DEF_MCU_DATA_CYL_NAME_4);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_5))->SetWindowText(DEF_MCU_DATA_CYL_NAME_5);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_6))->SetWindowText(DEF_MCU_DATA_CYL_NAME_6);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_7))->SetWindowText(DEF_MCU_DATA_CYL_NAME_7);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CYL_NAME_8))->SetWindowText(DEF_MCU_DATA_CYL_NAME_8);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_1))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_2))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_3))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_4))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_5))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_6))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_7))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_8))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_8);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_9))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_9);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_10))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_10);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_11))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_11);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_12))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_12);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_13))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_13);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_14))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_14);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_15))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_15);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_16))->SetWindowText(DEF_MCU_DATA_EDIT_CYLCFG_16);

	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_1))->SetWindowText(DEF_MCU_DATA_VAC_NAME_1);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_2))->SetWindowText(DEF_MCU_DATA_VAC_NAME_2);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_3))->SetWindowText(DEF_MCU_DATA_VAC_NAME_3);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_4))->SetWindowText(DEF_MCU_DATA_VAC_NAME_4);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_5))->SetWindowText(DEF_MCU_DATA_VAC_NAME_5);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_6))->SetWindowText(DEF_MCU_DATA_VAC_NAME_6);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_7))->SetWindowText(DEF_MCU_DATA_VAC_NAME_7);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_VAC_NAME_8))->SetWindowText(DEF_MCU_DATA_VAC_NAME_8);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_1))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_2))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_3))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_4))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_5))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_6))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_7))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_8))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_8);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_9))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_9);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_10))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_10);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_11))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_11);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_12))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_12);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_13))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_13);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_14))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_14);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_15))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_15);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_16))->SetWindowText(DEF_MCU_DATA_EDIT_VACCFG_16);

	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_1))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_1);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_2))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_2);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_3))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_3);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_4))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_4);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_5))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_5);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_6))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_6);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_7))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_7);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_8))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_8);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_1))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_2))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_3))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_4))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_5))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_6))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_7))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_8))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_8);
	
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_1))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_1);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_2))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_2);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_3))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_3);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_4))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_4);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_5))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_5);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_6))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_6);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_7))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_7);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_8))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_8);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_1))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_2))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_3))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_4))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_5))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_6))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_7))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_8))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_8);

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
		if (MessageBox(L" whould you like to reload datas ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		{
			refreshData();
		}
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
	if (((CButton*)GetDlgItem(IDC_MCU_DATA_RADIO_MT_ID2))->GetCheck())
	{
		m_pParent->m_motorIdx = MCU_OBJ::MOTOR_HIGH;
	}
	else if (((CButton*)GetDlgItem(IDC_MCU_DATA_RADIO_MT_ID1))->GetCheck())
	{
		m_pParent->m_motorIdx = MCU_OBJ::MOTOR_ROLL;
	}
	else //if (((CButton*)GetDlgItem(IDC_MCU_DATA_RADIO_MT_ID0))->GetCheck())
	{
		m_pParent->m_motorIdx = MCU_OBJ::MOTOR_JIG;
	}

}


void Cui_PopMcuData::OnBnClickedMcuDataBtnPosVelRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pPeeler->ReadROMData_MotorPos(m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - pose data read");
	}

	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnPosVelWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};

	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_0));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_0));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_1));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_1));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_2));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_2));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_3));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_3));

	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_4));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_4));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_5));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_5));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_6));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_6));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_7));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_7));

	if (m_pPeeler->WriteROM_PosData(datas, m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - pose data write");
	}
	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnActRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pPeeler->ReadROMData_CylinderData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - cylinder data read");
		return;
	}

	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnActWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};

	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_1));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_2));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_3));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_4));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_5));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_6));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_7));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_8));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_9));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_10));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_11));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_12));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_13));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_14));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_15));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CYLCFG_16));
	if (m_pPeeler->WriteROM_CyliderData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - cylinder data write");
		return;
	}	

	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnCfgRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pPeeler->ReadROMData_ConfigData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - config data read");
	}

	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnCfgWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};

	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_1));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_2));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_3));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_4));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_5));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_6));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_7));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_8));
	if (m_pPeeler->WriteROM_ConfigData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - config data write");
	}

	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnSeqRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pPeeler->ReadROMData_SequenceData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! -  sequence data read");
	}
	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnSeqWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};

	auto make_packet = [&datas](auto offset, auto source)->uint8_t
	{
		for (int i = 0; i < sizeof(source); i++)
		{
			datas.emplace_back((uint8_t)(source >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(source));
	};

	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_1));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_2));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_3));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_4));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_5));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_6));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_7));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_8));
	if (m_pPeeler->WriteROM_SequenceData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! -  sequence data write");
	}
	refreshData();

}


void Cui_PopMcuData::OnBnClickedMcuDataBtnRomWriteInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};

	auto make_packet_dw = [&datas](auto offset, auto source)->uint8_t
	{
		int x_data = _wtoi(source);
		for (int i = 0; i < sizeof(uint32_t); i++)
		{
			datas.emplace_back((uint8_t)(x_data >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(uint32_t));
	};

	auto make_packet_w = [&datas](auto offset, auto source)->uint8_t
	{
		int x_data = _wtoi(source);
		for (int i = 0; i < sizeof(uint16_t); i++)
		{
			datas.emplace_back((uint8_t)(x_data >> (i * 8)));
		}
		return (uint8_t)(offset + sizeof(uint16_t));
	};

	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_0);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_0);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_1);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_1);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_2);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_2);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_3);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_3);

	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_4);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_4);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_5);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_5);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_6);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_6);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_7);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_7);

	if (m_pPeeler->WriteROM_PosData(datas, m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - pose data write ");
	}


	idx = 0;
	datas.clear();
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_1);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_2);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_3);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_4);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_5);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_6);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_7);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_8);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_9);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_10);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_11);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_12);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_13);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_14);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_15);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CYLCFG_16);
	if (m_pPeeler->WriteROM_CyliderData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - cylinder data write ");
		return;
	}


	idx = 0; datas.clear();
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_1);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_2);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_3);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_4);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_5);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_6);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_7);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_8);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_9);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_10);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_11);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_12);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_13);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_14);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_15);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_VACCFG_16);
	if (m_pPeeler->WriteROM_VacuumData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - vacuum data write ");
		return;
	}

	idx = 0; datas.clear();
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_1);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_2);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_3);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_4);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_5);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_6);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_7);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_8);
	if (m_pPeeler->WriteROM_ConfigData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - config data write ");
	}

	idx = 0; datas.clear();
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_1);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_2);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_3);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_4);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_5);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_6);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_7);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_8);
	if (m_pPeeler->WriteROM_SequenceData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - sequence data write ");
	}

	refreshData();

}


void Cui_PopMcuData::refreshData()
{
	// TODO: 여기에 구현 코드 추가.
	constexpr int axis_pose_data_cnt = 8;
	{
		CString str{};

		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[0 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_0))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[0 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_0))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[1 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[1 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[2 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[2 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[3 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[3 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[4 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[4 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[5 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[5 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[6 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[6 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[7 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_7))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuAxisDat[7 + (axis_pose_data_cnt * m_pParent->m_motorIdx)].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_7))->SetWindowText(str);

		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[0].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[0].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[1].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[1].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[2].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[2].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[3].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_7))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[3].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_8))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[4].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_9))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[4].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_10))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[5].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_11))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[5].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_12))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[6].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_13))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[6].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_14))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[7].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_15))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[7].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CYLCFG_16))->SetWindowText(str);

		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[0].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[0].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[1].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[1].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[2].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[2].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[3].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_7))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[3].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_8))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[4].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_9))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[4].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_10))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[5].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_11))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[5].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_12))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[6].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_13))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[6].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_14))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[7].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_15))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuCylinderDat[7].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_16))->SetWindowText(str);

		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[0].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[0].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[1].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[1].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[2].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[2].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[3].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_7))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[3].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_8))->SetWindowText(str);

		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[0].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[0].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[1].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[1].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[2].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[2].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[3].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_7))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[3].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_8))->SetWindowText(str);

	}
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnRomUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnVacRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pPeeler->ReadROMData_VacuumData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - vacuum data read");
		return;
	}

	refreshData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnVacWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	uint8_t idx = 0;
	std::vector<uint8_t> datas{};

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
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_1));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_2));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_3));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_4));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_5));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_6));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_7));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_8));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_9));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_10));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_11));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_12));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_13));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_14));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_15));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_VACCFG_16));

	if (m_pPeeler->WriteROM_VacuumData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - vacuum data write ");
		return;
	}


	refreshData();
}
