// popMcuData.cpp: 구현 파일
//

#include "pch.h"
#include "VPRemote.h"
#include "afxdialogex.h"
#include "popMcuData.h"

#include "VPRemoteDlg.h"
// Cui_popMcuData 대화 상자

// mt  -  motor
enum motor_idx {
	mt_jig, mt_roll, mt_high, mt_max
};
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_1[mt_max]{ (L"ready"),(L"ready"),(L"ready") };
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_2[mt_max]{ (L"sticky"),(L"sticky"),(L"sticky") };
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_3[mt_max]{ (L"trash"),(L"trash"),(L"up sticky") };
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_4[mt_max]{ (L"loc 3"),(L"loc 3"),(L"trash") };
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_5[mt_max]{ (L"loc 4"),(L"loc 4"),(L"loc 4") };
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_6[mt_max]{ (L"loc 5"),(L"loc 5"),(L"loc 5") };
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_7[mt_max]{ (L"loc 6"),(L"loc 6"),(L"loc 6") };
constexpr wchar_t* DEF_MCU_DATA_POS_NAME_8[mt_max]{ (L"loc 7"),(L"loc 7"),(L"loc 7") };
												  
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_0[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_0[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_1[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_1[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_2[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_2[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_3[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_3[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_4[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_4[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_5[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_5[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_6[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_6[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };
constexpr wchar_t* DEF_MCU_DATA_EDIT_POS_7[mt_max]{ (L"10000") ,(L"10000"),(L"10000")};
constexpr wchar_t* DEF_MCU_DATA_EDIT_VEL_7[mt_max]{ (L"10")    ,(L"10")   ,(L"10")   };

constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_1(L"link ready");
constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_2(L"roll slow");
constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_3(L"roll fast");
constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_4(L"roll clear");
constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_5(L"link pose 5");
constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_6(L"link pose 6");
constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_7(L"link pose 7");
constexpr wchar_t* DEF_MCU_DATA_LINK_NAME_8(L"link pose 8");

constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_0(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_0(L"10");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_1(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_1(L"10");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_2(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_2(L"10");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_3(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_3(L"10");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_4(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_4(L"10");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_5(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_5(L"10");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_6(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_6(L"10");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_POS_7(L"10000");
constexpr wchar_t* DEF_MCU_DATA_EDIT_LINK_VEL_7(L"10");


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
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_9(L"cfg 9");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_10(L"cfg 10");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_11(L"cfg 11");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_12(L"cfg 12");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_13(L"cfg 13");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_14(L"cfg 14");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_15(L"cfg 15");
constexpr wchar_t* DEF_MCU_DATA_CONFIG_NAME_16(L"cfg 16");


constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_1(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_2(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_3(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_4(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_5(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_6(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_7(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_8(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_9(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_10(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_11(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_12(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_13(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_14(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_15(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_CONFIG_16(L"200");

constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_1(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_2(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_3(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_4(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_5(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_6(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_7(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_8(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_9(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_10(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_11(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_12(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_13(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_14(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_15(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_SEQ_NAME_16(L"sequence 1");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_1(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_2(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_3(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_4(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_5(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_6(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_7(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_8(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_9(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_10(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_11(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_12(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_13(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_14(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_15(L"200");
constexpr wchar_t* DEF_MCU_DATA_EDIT_SEQ_16(L"200");

constexpr wchar_t* DEF_MCU_DATA_MOTOR_RPS_MIN(L"1");
constexpr wchar_t* DEF_MCU_DATA_MOTOR_RPS_MAX(L"50");
#define MOONS_MOTORS_VELOCITY_RPS_MAX  50
#define MOONS_MOTORS_VELOCITY_RPS_MIN  1

#define mcr_constrain_rps(v,id)   ((GetDlgItemInt(id))<(MOONS_MOTORS_VELOCITY_RPS_MIN)?(MOONS_MOTORS_VELOCITY_RPS_MIN)\
														 :((GetDlgItemInt(id))>(MOONS_MOTORS_VELOCITY_RPS_MAX)?(MOONS_MOTORS_VELOCITY_RPS_MAX)\
                              :(GetDlgItemInt(id))))
//((GetDlgItemInt(id))<(low)?(low):((GetDlgItemInt(id))>(high)?(high):(GetDlgItemInt(id))))
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
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ACT_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnCylinderRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ACT_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnCylinderWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_CFG_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnCfgRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_CFG_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnCfgWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_SEQ_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnSeqRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_SEQ_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnSeqWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ROM_WRITE_INIT, &Cui_PopMcuData::OnBnClickedMcuDataBtnRomWriteInit)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ROM_UPDATE, &Cui_PopMcuData::OnBnClickedMcuDataBtnRomUpdate)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_VAC_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnVacRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_VAC_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnVacWrite)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_ROM_READ_ALL, &Cui_PopMcuData::OnBnClickedMcuDataBtnRomReadAll)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_LINK_VEL_READ, &Cui_PopMcuData::OnBnClickedMcuDataBtnLinkVelRead)
	ON_BN_CLICKED(IDC_MCU_DATA_BTN_LINK_VEL_WRITE, &Cui_PopMcuData::OnBnClickedMcuDataBtnLinkVelWrite)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_1, &Cui_PopMcuData::OnStnClickedMcuDataPosName1)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_2, &Cui_PopMcuData::OnStnClickedMcuDataPosName2)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_3, &Cui_PopMcuData::OnStnClickedMcuDataPosName3)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_4, &Cui_PopMcuData::OnStnClickedMcuDataPosName4)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_5, &Cui_PopMcuData::OnStnClickedMcuDataPosName5)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_6, &Cui_PopMcuData::OnStnClickedMcuDataPosName6)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_7, &Cui_PopMcuData::OnStnClickedMcuDataPosName7)
	ON_STN_CLICKED(IDC_MCU_DATA_POS_NAME_8, &Cui_PopMcuData::OnStnClickedMcuDataPosName8)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_1, &Cui_PopMcuData::OnStnClickedMcuDataLinkName1)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_2, &Cui_PopMcuData::OnStnClickedMcuDataLinkName2)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_3, &Cui_PopMcuData::OnStnClickedMcuDataLinkName3)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_4, &Cui_PopMcuData::OnStnClickedMcuDataLinkName4)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_5, &Cui_PopMcuData::OnStnClickedMcuDataLinkName5)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_6, &Cui_PopMcuData::OnStnClickedMcuDataLinkName6)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_7, &Cui_PopMcuData::OnStnClickedMcuDataLinkName7)
	ON_STN_CLICKED(IDC_MCU_DATA_LINK_NAME_8, &Cui_PopMcuData::OnStnClickedMcuDataLinkName8)
	ON_BN_CLICKED(IDC_IDC_MCU_DATA_BTN_MOVE_STOP, &Cui_PopMcuData::OnBnClickedIdcMcuDataBtnMoveStop)
	ON_BN_CLICKED(IDC_MCU_DATA_RADIO_MT_ID0, &Cui_PopMcuData::OnBnClickedMcuDataRadioMtId0)
	ON_BN_CLICKED(IDC_MCU_DATA_RADIO_MT_ID1, &Cui_PopMcuData::OnBnClickedMcuDataRadioMtId1)
	ON_BN_CLICKED(IDC_MCU_DATA_RADIO_MT_ID2, &Cui_PopMcuData::OnBnClickedMcuDataRadioMtId2)
END_MESSAGE_MAP()


// Cui_popMcuData 메시지 처리기


BOOL Cui_PopMcuData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	((CButton*)GetDlgItem(IDC_MCU_DATA_RADIO_MT_ID0))->SetCheck(TRUE);
	
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_1))->SetWindowText(DEF_MCU_DATA_POS_NAME_1[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_2))->SetWindowText(DEF_MCU_DATA_POS_NAME_2[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_3))->SetWindowText(DEF_MCU_DATA_POS_NAME_3[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_4))->SetWindowText(DEF_MCU_DATA_POS_NAME_4[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_5))->SetWindowText(DEF_MCU_DATA_POS_NAME_5[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_6))->SetWindowText(DEF_MCU_DATA_POS_NAME_6[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_7))->SetWindowText(DEF_MCU_DATA_POS_NAME_7[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_8))->SetWindowText(DEF_MCU_DATA_POS_NAME_8[m_pParent->m_motorIdx]);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_0))->SetWindowText(DEF_MCU_DATA_EDIT_POS_0[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_0))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_0[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_1))->SetWindowText(DEF_MCU_DATA_EDIT_POS_1[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_1))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_1[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_2))->SetWindowText(DEF_MCU_DATA_EDIT_POS_2[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_2))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_2[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_3))->SetWindowText(DEF_MCU_DATA_EDIT_POS_3[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_3))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_3[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_4))->SetWindowText(DEF_MCU_DATA_EDIT_POS_4[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_4))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_4[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_5))->SetWindowText(DEF_MCU_DATA_EDIT_POS_5[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_5))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_5[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_6))->SetWindowText(DEF_MCU_DATA_EDIT_POS_6[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_6))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_6[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_POS_7))->SetWindowText(DEF_MCU_DATA_EDIT_POS_7[m_pParent->m_motorIdx]);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VEL_7))->SetWindowText(DEF_MCU_DATA_EDIT_VEL_7[m_pParent->m_motorIdx]);

	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_1))->SetWindowText(DEF_MCU_DATA_LINK_NAME_1);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_2))->SetWindowText(DEF_MCU_DATA_LINK_NAME_2);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_3))->SetWindowText(DEF_MCU_DATA_LINK_NAME_3);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_4))->SetWindowText(DEF_MCU_DATA_LINK_NAME_4);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_5))->SetWindowText(DEF_MCU_DATA_LINK_NAME_5);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_6))->SetWindowText(DEF_MCU_DATA_LINK_NAME_6);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_7))->SetWindowText(DEF_MCU_DATA_LINK_NAME_7);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_LINK_NAME_8))->SetWindowText(DEF_MCU_DATA_LINK_NAME_8);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_1))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_0);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_1))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_0);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_2))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_2))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_3))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_3))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_4))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_4))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_5))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_5))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_6))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_6))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_7))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_7))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_8))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_POS_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_8))->SetWindowText(DEF_MCU_DATA_EDIT_LINK_VEL_7);
																				
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
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_9))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_9);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_10))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_10);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_11))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_11);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_12))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_12);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_13))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_13);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_14))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_14);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_15))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_15);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_CONFIG_NAME_16))->SetWindowText(DEF_MCU_DATA_CONFIG_NAME_16);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_1))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_2))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_3))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_4))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_5))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_6))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_7))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_8))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_8);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_9))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_9);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_10))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_10);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_11))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_11);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_12))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_12);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_13))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_13);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_14))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_14);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_15))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_15);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_16))->SetWindowText(DEF_MCU_DATA_EDIT_CONFIG_16);
	
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_1))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_1);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_2))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_2);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_3))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_3);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_4))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_4);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_5))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_5);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_6))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_6);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_7))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_7);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_8))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_8);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_9))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_9);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_10))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_10);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_11))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_11);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_12))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_12);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_13))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_13);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_14))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_14);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_15))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_15);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_SEQ_NAME_16))->SetWindowText(DEF_MCU_DATA_SEQ_NAME_16);

	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_1))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_1);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_2))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_2);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_3))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_3);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_4))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_4);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_5))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_5);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_6))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_6);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_7))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_7);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_8))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_8);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_9))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_9);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_10))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_10);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_11))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_11);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_12))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_12);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_13))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_13);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_14))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_14);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_15))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_15);
	((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_16))->SetWindowText(DEF_MCU_DATA_EDIT_SEQ_16);

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
		if (MessageBox(L" Would you like to reload datas ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
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
	m_pParent->LockUpdate();
	if (m_pPeeler->ReadROMData_MotorPos(m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - pose data read");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnPosVelWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
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
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_0));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_1));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_1));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_2));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_2));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_3));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_3));

	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_4));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_4));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_5));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_5));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_6));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_6));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_7));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_VEL_7));

	if (m_pPeeler->WriteROM_PosData(datas, m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - pose data write");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnCylinderRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
	if (m_pPeeler->ReadROMData_CylinderData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - cylinder data read");
	}

	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnCylinderWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	m_pParent->LockUpdate();
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
	}	

	m_pParent->LockUpdate(true); 
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnCfgRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
	if (m_pPeeler->ReadROMData_ConfigData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - config data read");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnCfgWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
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
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_9));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_10));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_11));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_12));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_13));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_14));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_15));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_CONFIG_16));
	if (m_pPeeler->WriteROM_ConfigData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - config data write");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnSeqRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
	if(m_pPeeler->ReadROMData_SequenceData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! -  sequence data read");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnSeqWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
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
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_9));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_10));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_11));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_12));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_13));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_14));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_15));
	idx = make_packet(idx, (uint16_t)GetDlgItemInt(IDC_MCU_DATA_EDIT_SEQ_16));
	if (m_pPeeler->WriteROM_SequenceData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! -  sequence data write");
	}
	m_pParent->LockUpdate(true);

}


void Cui_PopMcuData::OnBnClickedMcuDataBtnRomWriteInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
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

	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_0[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_0[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_1[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_1[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_2[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_2[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_3[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_3[m_pParent->m_motorIdx]);

	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_4[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_4[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_5[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_5[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_6[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_6[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_POS_7[m_pParent->m_motorIdx]);
	idx = make_packet_dw(idx, DEF_MCU_DATA_EDIT_VEL_7[m_pParent->m_motorIdx]);

	if (m_pPeeler->WriteROM_PosData(datas, m_pParent->m_motorIdx) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - pose data write ");
		m_pParent->LockUpdate(true);
		return;
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
		m_pParent->LockUpdate(true);
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
		m_pParent->LockUpdate(true);
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
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_9);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_10);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_11);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_12);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_13);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_14);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_15);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_CONFIG_16);
	if (m_pPeeler->WriteROM_ConfigData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - config data write ");
		m_pParent->LockUpdate(true);
		return;
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
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_9);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_10);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_11);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_12);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_13);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_14);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_15);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_SEQ_16);
	if (m_pPeeler->WriteROM_SequenceData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - sequence data write ");
		m_pParent->LockUpdate(true);
		return;
	}

	idx = 0; datas.clear();
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_0);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_0);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_1);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_1);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_2);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_2);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_3);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_3);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_4);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_4);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_5);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_5);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_6);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_6);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_POS_7);
	idx = make_packet_w(idx, DEF_MCU_DATA_EDIT_LINK_VEL_7);
	if (m_pPeeler->WriteROM_LinkPosData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - link pose data write ");
		m_pParent->LockUpdate(true);
		return;
	}

	m_pParent->LockUpdate(true);

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
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[4].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_9))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[4].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_10))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[5].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_11))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[5].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_12))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[6].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_13))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[6].on_settling);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_14))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[7].timeout);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_VACCFG_15))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuVacuumDat[7].on_settling);
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
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[4].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_9))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[4].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_10))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[5].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_11))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[5].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_12))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[6].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_13))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[6].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_14))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[7].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_15))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuConfigDat[7].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_CONFIG_16))->SetWindowText(str);


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
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[4].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_9))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[4].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_10))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[5].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_11))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[5].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_12))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[6].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_13))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[6].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_14))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[7].parm1);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_15))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuSequnceDat[7].parm2);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_SEQ_16))->SetWindowText(str);


		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[0].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[0].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_1))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[1].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[1].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_2))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[2].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[2].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_3))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[3].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[3].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_4))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[4].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[4].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_5))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[5].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[5].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_6))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[6].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_7))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[6].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_7))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[7].cmd_pos);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_POS_8))->SetWindowText(str);
		str.Format(L"%d", m_pPeeler->m_mcuLinkPoseDat[7].cmd_vel);
		((CEdit*)GetDlgItem(IDC_MCU_DATA_EDIT_LINK_VEL_8))->SetWindowText(str);
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
	m_pParent->LockUpdate();
	if (m_pPeeler->ReadROMData_VacuumData() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - vacuum data read");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnVacWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
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
	}

	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::readAllData()
{

  if (m_pPeeler->ReadROMData_MotorPos(m_pParent->m_motorIdx) != ERROR_SUCCESS)
  {
    //AfxMessageBox(L"comm error! - pose data read");
	}
	uint32_t pre_ms = UTL::millis();
	while (1)
	{
		if (UTL::millis() - pre_ms > 200)
		{
			break;
		}
		Sleep(10);
	}

  if (m_pPeeler->ReadROMData_ConfigData() != ERROR_SUCCESS)
  {
    //AfxMessageBox(L"comm error! - config data read");
	}
	pre_ms = UTL::millis();
	while (1)
	{
		if (UTL::millis() - pre_ms > 200)
		{
			break;
		}
		Sleep(10);
	}
	if (m_pPeeler->ReadROMData_CylinderData() != ERROR_SUCCESS)
  {
    //AfxMessageBox(L"comm error! - cylinder data read");

  }
	pre_ms = UTL::millis();
	while (1)
	{
		if (UTL::millis() - pre_ms > 200)
		{
			break;
		}
		Sleep(10);
	}
	if (m_pPeeler->ReadROMData_SequenceData() != ERROR_SUCCESS)
	{
		//AfxMessageBox(L"comm error! -  sequence data read");

	}

	pre_ms = UTL::millis();
	while (1)
	{
		if (UTL::millis() - pre_ms > 200)
		{
			break;
		}
		Sleep(10);
	}

  if (m_pPeeler->ReadROMData_VacuumData() != ERROR_SUCCESS)
  {
    //AfxMessageBox(L"comm error! - vacuum data read");

  }
	pre_ms = UTL::millis();
	while (1)
	{
		if (UTL::millis() - pre_ms > 200)
		{
			break;
		}
		Sleep(10);
	}
  if (m_pPeeler->ReadROMData_SequenceData() != ERROR_SUCCESS)
  {
    //AfxMessageBox(L"comm error! -  sequence data read");

  }
	pre_ms = UTL::millis();
	while (1)
	{
		if (UTL::millis() - pre_ms > 200)
		{
			break;
		}
		Sleep(10);
	}

	if (m_pPeeler->ReadROMData_CylinderData() != ERROR_SUCCESS)
	{
		//AfxMessageBox(L"comm error! - cylinder data read");

	}

}


void Cui_PopMcuData::OnBnClickedMcuDataBtnRomReadAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	readAllData();
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnLinkVelRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
	if (m_pPeeler->ReadROMData_LinkPos() != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - link pose data read");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnBnClickedMcuDataBtnLinkVelWrite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pParent->LockUpdate();
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

	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_1));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_1));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_2));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_2));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_3));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_3));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_4));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_4));

	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_5));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_5));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_6));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_6));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_7));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_7));
	idx = make_packet(idx, GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_8));
	idx = make_packet(idx, constrainRps(IDC_MCU_DATA_EDIT_LINK_VEL_8));

	if (m_pPeeler->WriteROM_LinkPosData(datas) != ERROR_SUCCESS)
	{
		AfxMessageBox(L"comm error! - link pose data write");
	}
	m_pParent->LockUpdate(true);
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_0), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_0));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_1), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_1));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_2), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_2));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_3), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_3));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_4), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_4));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_5), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_5));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_6), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_6));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataPosName8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_POS_7), GetDlgItemInt(IDC_MCU_DATA_EDIT_VEL_7));
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_1), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_1), true);
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_2), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_2), true);
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_3), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_3), true);
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_4), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_4), true);
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_5), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_5), true);
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_6), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_6), true);
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_7), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_7), true);
	}
}


void Cui_PopMcuData::OnStnClickedMcuDataLinkName8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (MessageBox(L" Would you like to drive the motor ?", L"확인", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		movePoseData(GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_POS_7), GetDlgItemInt(IDC_MCU_DATA_EDIT_LINK_VEL_8), true);
	}
}


void Cui_PopMcuData::movePoseData(int pose, int vel, bool is_link )
{
	// TODO: 여기에 구현 코드 추가.
  if (is_link)
  {
    if (m_pPeeler->MoveLinkPose(pose, vel) != ERROR_SUCCESS)
    {
      AfxMessageBox(L"comm error! - link pose move ");
    }
	return;
  }

  // else link
  if (m_pPeeler->Move(m_pParent->m_motorIdx, pose, (uint16_t)vel) != ERROR_SUCCESS)
  {
	  AfxMessageBox(L"comm error! - pose data move ");
  }
}


void Cui_PopMcuData::OnBnClickedIdcMcuDataBtnMoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pPeeler->Stop(m_pParent->m_motorIdx);
}


int Cui_PopMcuData::constrainRps(int id)
{
	// TODO: 여기에 구현 코드 추가.
		if (GetDlgItemInt(id) < MOONS_MOTORS_VELOCITY_RPS_MIN)
		{
			SetDlgItemText(id, DEF_MCU_DATA_MOTOR_RPS_MIN);
			return MOONS_MOTORS_VELOCITY_RPS_MIN;
		}
		else if (GetDlgItemInt(id) > MOONS_MOTORS_VELOCITY_RPS_MAX)
		{
			SetDlgItemText(id, DEF_MCU_DATA_MOTOR_RPS_MAX);
			return MOONS_MOTORS_VELOCITY_RPS_MAX;
		}

		return GetDlgItemInt(id);

}


void Cui_PopMcuData::OnBnClickedMcuDataRadioMtId0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	m_pParent->m_motorIdx = MCU_OBJ::MOTOR_JIG;
	updatePoseData();
}


void Cui_PopMcuData::OnBnClickedMcuDataRadioMtId1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	m_pParent->m_motorIdx = MCU_OBJ::MOTOR_ROLL;
	updatePoseData();
}


void Cui_PopMcuData::OnBnClickedMcuDataRadioMtId2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pParent->m_motorIdx = MCU_OBJ::MOTOR_HIGH;
	updatePoseData();
}


void Cui_PopMcuData::updatePoseData()
{
	// TODO: 여기에 구현 코드 추가.
	CString str{};
	constexpr int axis_pose_data_cnt = 8;


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

	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_1))->SetWindowText(DEF_MCU_DATA_POS_NAME_1[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_2))->SetWindowText(DEF_MCU_DATA_POS_NAME_2[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_3))->SetWindowText(DEF_MCU_DATA_POS_NAME_3[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_4))->SetWindowText(DEF_MCU_DATA_POS_NAME_4[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_5))->SetWindowText(DEF_MCU_DATA_POS_NAME_5[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_6))->SetWindowText(DEF_MCU_DATA_POS_NAME_6[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_7))->SetWindowText(DEF_MCU_DATA_POS_NAME_7[m_pParent->m_motorIdx]);
	((CStatic*)GetDlgItem(IDC_MCU_DATA_POS_NAME_8))->SetWindowText(DEF_MCU_DATA_POS_NAME_8[m_pParent->m_motorIdx]);
}



