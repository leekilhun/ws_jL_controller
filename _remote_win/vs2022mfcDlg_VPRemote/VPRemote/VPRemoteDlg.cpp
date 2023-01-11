
// VPRemoteDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "VPRemote.h"


#include "VPRemoteDlg.h"
#include "afxdialogex.h"

//3. 추가할 헤더
#include "ui/popMoonsControl.h"
#include "ui/popMcuData.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVPRemoteDlg 대화 상자
//for status bar .... indicator...  
constexpr int STATUSBAR_IDX_COMM = 0;
constexpr int STATUSBAR_IDX_RESULT = 1;
constexpr int STATUSBAR_IDX_VER = 2;

constexpr wchar_t* DEF_CHECK_STATE_0(L"auto running");
constexpr wchar_t* DEF_CHECK_STATE_1(L"emg stop");
constexpr wchar_t* DEF_CHECK_STATE_2(L"auto ready");
constexpr wchar_t* DEF_CHECK_STATE_3(L"auto stop");
constexpr wchar_t* DEF_CHECK_STATE_4(L"detect safety");
constexpr wchar_t* DEF_CHECK_STATE_5(L"motor on");
constexpr wchar_t* DEF_CHECK_STATE_6(L"init cplt");
constexpr wchar_t* DEF_CHECK_STATE_7(L"alarm status");

constexpr wchar_t* DEF_CHECK_STATE_8(L"state");
constexpr wchar_t* DEF_CHECK_STATE_9(L"state");
constexpr wchar_t* DEF_CHECK_STATE_10(L"state");
constexpr wchar_t* DEF_CHECK_STATE_11(L"state");
constexpr wchar_t* DEF_CHECK_STATE_12(L"state");
constexpr wchar_t* DEF_CHECK_STATE_13(L"state");
constexpr wchar_t* DEF_CHECK_STATE_14(L"request init");
constexpr wchar_t* DEF_CHECK_STATE_15(L"all_check_ok");



//option
constexpr wchar_t* DEF_CHECK_STATE_16(L"use beep");
constexpr wchar_t* DEF_CHECK_STATE_17(L"use dryrun");
constexpr wchar_t* DEF_CHECK_STATE_18(L"skip door");
constexpr wchar_t* DEF_CHECK_STATE_19(L"skip safety");
constexpr wchar_t* DEF_CHECK_STATE_20(L"option");
constexpr wchar_t* DEF_CHECK_STATE_21(L"option");
constexpr wchar_t* DEF_CHECK_STATE_22(L"option");
constexpr wchar_t* DEF_CHECK_STATE_23(L"option");

constexpr wchar_t* DEF_CHECK_STATE_24(L"option");
constexpr wchar_t* DEF_CHECK_STATE_25(L"option");
constexpr wchar_t* DEF_CHECK_STATE_26(L"option");
constexpr wchar_t* DEF_CHECK_STATE_27(L"option");
constexpr wchar_t* DEF_CHECK_STATE_28(L"option");
constexpr wchar_t* DEF_CHECK_STATE_29(L"option");
constexpr wchar_t* DEF_CHECK_STATE_30(L"option");
constexpr wchar_t* DEF_CHECK_STATE_31(L"option");



// error
constexpr wchar_t* DEF_CHECK_STATE_32(L"no error");
constexpr wchar_t* DEF_CHECK_STATE_33(L"comm lcd");
constexpr wchar_t* DEF_CHECK_STATE_34(L"comm motor");
constexpr wchar_t* DEF_CHECK_STATE_35(L"err");
constexpr wchar_t* DEF_CHECK_STATE_36(L"err");
constexpr wchar_t* DEF_CHECK_STATE_37(L"err");
constexpr wchar_t* DEF_CHECK_STATE_38(L"motor err");
constexpr wchar_t* DEF_CHECK_STATE_39(L"motor init");

constexpr wchar_t* DEF_CHECK_STATE_40(L"limit_err_motor");
constexpr wchar_t* DEF_CHECK_STATE_41(L"cyl interlock");
constexpr wchar_t* DEF_CHECK_STATE_42(L"err");
constexpr wchar_t* DEF_CHECK_STATE_43(L"err");
constexpr wchar_t* DEF_CHECK_STATE_44(L"cyl timeout");
constexpr wchar_t* DEF_CHECK_STATE_45(L"vac timeout");
constexpr wchar_t* DEF_CHECK_STATE_46(L"mot timeout");
constexpr wchar_t* DEF_CHECK_STATE_47(L"seq timeout");

constexpr wchar_t* DEF_CHECK_STATE_48(L"err");
constexpr wchar_t* DEF_CHECK_STATE_49(L"err");
constexpr wchar_t* DEF_CHECK_STATE_50(L"err");
constexpr wchar_t* DEF_CHECK_STATE_51(L"err");
constexpr wchar_t* DEF_CHECK_STATE_52(L"err");
constexpr wchar_t* DEF_CHECK_STATE_53(L"err");
constexpr wchar_t* DEF_CHECK_STATE_54(L"err");
constexpr wchar_t* DEF_CHECK_STATE_55(L"err");

constexpr wchar_t* DEF_CHECK_STATE_56(L"err");
constexpr wchar_t* DEF_CHECK_STATE_57(L"err");
constexpr wchar_t* DEF_CHECK_STATE_58(L"err");
constexpr wchar_t* DEF_CHECK_STATE_59(L"err");
constexpr wchar_t* DEF_CHECK_STATE_60(L"err");
constexpr wchar_t* DEF_CHECK_STATE_61(L"err");
constexpr wchar_t* DEF_CHECK_STATE_62(L"err");
constexpr wchar_t* DEF_CHECK_STATE_63(L"err");



// input
constexpr wchar_t* DEF_CHECK_STATE_64(L"V on vinyl");
constexpr wchar_t* DEF_CHECK_STATE_65(L"on phone");
constexpr wchar_t* DEF_CHECK_STATE_66(L"mode manual");
constexpr wchar_t* DEF_CHECK_STATE_67(L"mode auto");
constexpr wchar_t* DEF_CHECK_STATE_68(L"in 04");
constexpr wchar_t* DEF_CHECK_STATE_69(L"in 05");
constexpr wchar_t* DEF_CHECK_STATE_70(L"in 06");
constexpr wchar_t* DEF_CHECK_STATE_71(L"safety");

constexpr wchar_t* DEF_CHECK_STATE_72(L"in 10");
constexpr wchar_t* DEF_CHECK_STATE_73(L"in 11");
constexpr wchar_t* DEF_CHECK_STATE_74(L"in 12");
constexpr wchar_t* DEF_CHECK_STATE_75(L"in 13");
constexpr wchar_t* DEF_CHECK_STATE_76(L"in 14");
constexpr wchar_t* DEF_CHECK_STATE_77(L"in 15");
constexpr wchar_t* DEF_CHECK_STATE_78(L"in 16");
constexpr wchar_t* DEF_CHECK_STATE_79(L"in 17");

constexpr wchar_t* DEF_CHECK_STATE_80(L"C jig for");
constexpr wchar_t* DEF_CHECK_STATE_81(L"C jig back");
constexpr wchar_t* DEF_CHECK_STATE_82(L"C jig open");
constexpr wchar_t* DEF_CHECK_STATE_83(L"C jig close");
constexpr wchar_t* DEF_CHECK_STATE_84(L"C push");
constexpr wchar_t* DEF_CHECK_STATE_85(L"C push back");
constexpr wchar_t* DEF_CHECK_STATE_86(L"C grip up");
constexpr wchar_t* DEF_CHECK_STATE_87(L"C grip down");

constexpr wchar_t* DEF_CHECK_STATE_88(L"in 30");
constexpr wchar_t* DEF_CHECK_STATE_89(L"S vinyl");
constexpr wchar_t* DEF_CHECK_STATE_90(L"in 32");
constexpr wchar_t* DEF_CHECK_STATE_91(L"in 33");
constexpr wchar_t* DEF_CHECK_STATE_92(L"in 34");
constexpr wchar_t* DEF_CHECK_STATE_93(L"in 35");
constexpr wchar_t* DEF_CHECK_STATE_94(L"in 36");
constexpr wchar_t* DEF_CHECK_STATE_95(L"in 37");

//output
constexpr wchar_t* DEF_CHECK_STATE_96(L"out 00");
constexpr wchar_t* DEF_CHECK_STATE_97(L"out 01");
constexpr wchar_t* DEF_CHECK_STATE_98(L"out 02");
constexpr wchar_t* DEF_CHECK_STATE_99(L"out 03");
constexpr wchar_t* DEF_CHECK_STATE_100(L"M jig dir");
constexpr wchar_t* DEF_CHECK_STATE_101(L"M jig pulse");
constexpr wchar_t* DEF_CHECK_STATE_102(L"M Roll dir");
constexpr wchar_t* DEF_CHECK_STATE_103(L"M Roll pulse");

constexpr wchar_t* DEF_CHECK_STATE_104(L"out 10");
constexpr wchar_t* DEF_CHECK_STATE_105(L"out 11");
constexpr wchar_t* DEF_CHECK_STATE_106(L"out 12");
constexpr wchar_t* DEF_CHECK_STATE_107(L"out 13");
constexpr wchar_t* DEF_CHECK_STATE_108(L"out 14");
constexpr wchar_t* DEF_CHECK_STATE_109(L"out 15");
constexpr wchar_t* DEF_CHECK_STATE_110(L"out 16");
constexpr wchar_t* DEF_CHECK_STATE_111(L"out 17");

constexpr wchar_t* DEF_CHECK_STATE_112(L"C jig for");
constexpr wchar_t* DEF_CHECK_STATE_113(L"C jig back");
constexpr wchar_t* DEF_CHECK_STATE_114(L"C jig open");
constexpr wchar_t* DEF_CHECK_STATE_115(L"C jig close");
constexpr wchar_t* DEF_CHECK_STATE_116(L"C push");
constexpr wchar_t* DEF_CHECK_STATE_117(L"C push back");
constexpr wchar_t* DEF_CHECK_STATE_118(L"C grip up");
constexpr wchar_t* DEF_CHECK_STATE_119(L"C grip down");

constexpr wchar_t* DEF_CHECK_STATE_120(L"V on vinyl");
constexpr wchar_t* DEF_CHECK_STATE_121(L"V blow vinyl");
constexpr wchar_t* DEF_CHECK_STATE_122(L"out 32");
constexpr wchar_t* DEF_CHECK_STATE_123(L"out 33");
constexpr wchar_t* DEF_CHECK_STATE_124(L"out 34");
constexpr wchar_t* DEF_CHECK_STATE_125(L"out 35");
constexpr wchar_t* DEF_CHECK_STATE_126(L"out 36");
constexpr wchar_t* DEF_CHECK_STATE_127(L"out 37");






static UINT BASED_CODE indicators[] =
{
	IDS_INDICATOR_COMM_STATUS,
	IDS_INDICATOR_MOTOR_STATUS,
	IDS_INDICATOR_INFOR
};

static void receiveMainDlgCB(void* obj, void* w_parm, void* l_parm);

CVPRemoteDlg::CVPRemoteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VPREMOTE_DIALOG, pParent), m_popMotor{}, m_popMcuData{}, m_pSystem{}, m_TimerID{}
	, m_lockUpdate{}, m_statusBar{}
	, m_motorIdx{}
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSystem = (AP_SYS::MSystem*)(CVPRemoteApp*)theApp.GetSystem();
	m_pSystem->GetModulePeelerComponent()->AttCallbackFunc(this, receiveMainDlgCB);
}

void CVPRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_LIST_RX, m_rxListBox);
}

BEGIN_MESSAGE_MAP(CVPRemoteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PEELER_MAIN_POP_MOTOR_CTRL, &CVPRemoteDlg::OnBnClickedPeelerMainPopMotorCtrl)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PEELER_MAIN_MOTOR_STOP, &CVPRemoteDlg::OnBnClickedPeelerMainMotorStop)
	ON_BN_CLICKED(IDC_PEELER_MAIN_OP_START, &CVPRemoteDlg::OnBnClickedPeelerMainOpStart)
	ON_BN_CLICKED(IDC_PEELER_MAIN_OP_STOP, &CVPRemoteDlg::OnBnClickedPeelerMainOpStop)
	ON_BN_CLICKED(IDC_PEELER_MAIN_OP_RESET, &CVPRemoteDlg::OnBnClickedPeelerMainOpReset)
	ON_BN_CLICKED(IDC_PEELER_MAIN_INIT, &CVPRemoteDlg::OnBnClickedPeelerMainInit)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CHECK_MOTOR_ONOFF, &CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOnoff)
	ON_BN_CLICKED(IDC_PEELER_MAIN_MOTOR_RESET, &CVPRemoteDlg::OnBnClickedPeelerMainMotorReset)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CHECK_MOTOR_ORG, &CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOrg)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_1, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderFor)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_2, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderBack)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_3, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigOpen)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_4, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigGrip)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_5, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigClose)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_6, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylHoldUp)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_7, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylHoldDown)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_8, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylPush)
	ON_BN_CLICKED(IDC_PEELER_MAIN_CYL_9, &CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylBack)
	ON_BN_CLICKED(IDC_CHECK_STATE_0, &CVPRemoteDlg::OnBnClickedCheckState0)
	ON_BN_CLICKED(IDC_CHECK_STATE_1, &CVPRemoteDlg::OnBnClickedCheckState1)
	ON_BN_CLICKED(IDC_CHECK_STATE_2, &CVPRemoteDlg::OnBnClickedCheckState2)
	ON_BN_CLICKED(IDC_CHECK_STATE_3, &CVPRemoteDlg::OnBnClickedCheckState3)
	ON_BN_CLICKED(IDC_CHECK_STATE_4, &CVPRemoteDlg::OnBnClickedCheckState4)
	ON_BN_CLICKED(IDC_CHECK_STATE_5, &CVPRemoteDlg::OnBnClickedCheckState5)
	ON_BN_CLICKED(IDC_CHECK_STATE_6, &CVPRemoteDlg::OnBnClickedCheckState6)
	ON_BN_CLICKED(IDC_CHECK_STATE_7, &CVPRemoteDlg::OnBnClickedCheckState7)
	ON_BN_CLICKED(IDC_CHECK_STATE_8, &CVPRemoteDlg::OnBnClickedCheckState8)
	ON_BN_CLICKED(IDC_CHECK_STATE_9, &CVPRemoteDlg::OnBnClickedCheckState9)
	ON_BN_CLICKED(IDC_CHECK_STATE_10, &CVPRemoteDlg::OnBnClickedCheckState10)
	ON_BN_CLICKED(IDC_CHECK_STATE_11, &CVPRemoteDlg::OnBnClickedCheckState11)
	ON_BN_CLICKED(IDC_CHECK_STATE_12, &CVPRemoteDlg::OnBnClickedCheckState12)
	ON_BN_CLICKED(IDC_CHECK_STATE_13, &CVPRemoteDlg::OnBnClickedCheckState13)
	ON_BN_CLICKED(IDC_CHECK_STATE_14, &CVPRemoteDlg::OnBnClickedCheckState14)
	ON_BN_CLICKED(IDC_CHECK_STATE_15, &CVPRemoteDlg::OnBnClickedCheckState15)

	ON_BN_CLICKED(IDC_CHECK_STATE_16, &CVPRemoteDlg::OnBnClickedCheckOption0)
	ON_BN_CLICKED(IDC_CHECK_STATE_17, &CVPRemoteDlg::OnBnClickedCheckOption1)
	ON_BN_CLICKED(IDC_CHECK_STATE_18, &CVPRemoteDlg::OnBnClickedCheckOption2)
	ON_BN_CLICKED(IDC_CHECK_STATE_19, &CVPRemoteDlg::OnBnClickedCheckOption3)
	ON_BN_CLICKED(IDC_CHECK_STATE_20, &CVPRemoteDlg::OnBnClickedCheckOption4)
	ON_BN_CLICKED(IDC_CHECK_STATE_21, &CVPRemoteDlg::OnBnClickedCheckOption5)
	ON_BN_CLICKED(IDC_CHECK_STATE_22, &CVPRemoteDlg::OnBnClickedCheckOption6)
	ON_BN_CLICKED(IDC_CHECK_STATE_23, &CVPRemoteDlg::OnBnClickedCheckOption7)
	ON_BN_CLICKED(IDC_CHECK_STATE_24, &CVPRemoteDlg::OnBnClickedCheckOption8)
	ON_BN_CLICKED(IDC_CHECK_STATE_25, &CVPRemoteDlg::OnBnClickedCheckOption9)
	ON_BN_CLICKED(IDC_CHECK_STATE_26, &CVPRemoteDlg::OnBnClickedCheckOption10)
	ON_BN_CLICKED(IDC_CHECK_STATE_27, &CVPRemoteDlg::OnBnClickedCheckOption11)
	ON_BN_CLICKED(IDC_CHECK_STATE_28, &CVPRemoteDlg::OnBnClickedCheckOption12)
	ON_BN_CLICKED(IDC_CHECK_STATE_29, &CVPRemoteDlg::OnBnClickedCheckOption13)
	ON_BN_CLICKED(IDC_CHECK_STATE_30, &CVPRemoteDlg::OnBnClickedCheckOption14)
	ON_BN_CLICKED(IDC_CHECK_STATE_31, &CVPRemoteDlg::OnBnClickedCheckOption15)

	ON_BN_CLICKED(IDC_CHECK_STATE_96, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_97, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_98, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_99, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_100, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_101, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_102, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_103, &CVPRemoteDlg::OnBnClickedCheckSetOutput)

	ON_BN_CLICKED(IDC_CHECK_STATE_104, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_105, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_106, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_107, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_108, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_109, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_110, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_111, &CVPRemoteDlg::OnBnClickedCheckSetOutput)

	ON_BN_CLICKED(IDC_CHECK_STATE_112, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_113, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_114, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_115, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_116, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_117, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_118, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_119, &CVPRemoteDlg::OnBnClickedCheckSetOutput)

	ON_BN_CLICKED(IDC_CHECK_STATE_120, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_121, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_122, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_123, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_124, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_125, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_126, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_CHECK_STATE_127, &CVPRemoteDlg::OnBnClickedCheckSetOutput)
	ON_BN_CLICKED(IDC_PEELER_MAIN_POP_MCU_DATA, &CVPRemoteDlg::OnBnClickedPeelerMainPopMcuData)
	ON_BN_CLICKED(IDC_MAIN_CHECK_STEP_1, &CVPRemoteDlg::OnBnClickedMainCheckStep1)
	ON_BN_CLICKED(IDC_MAIN_CHECK_STEP_2, &CVPRemoteDlg::OnBnClickedMainCheckStep2)
	ON_BN_CLICKED(IDC_MAIN_CHECK_STEP_3, &CVPRemoteDlg::OnBnClickedMainCheckStep3)
	ON_BN_CLICKED(IDC_MAIN_CHECK_STEP_4, &CVPRemoteDlg::OnBnClickedMainCheckStep4)
	ON_BN_CLICKED(IDC_MAIN_CHECK_STEP_5, &CVPRemoteDlg::OnBnClickedMainCheckStep5)
	ON_BN_CLICKED(IDC_MAIN_CHECK_STEP_6, &CVPRemoteDlg::OnBnClickedMainCheckStep6)
	ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC1, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc1)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC2, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc2)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC3, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc3)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC4, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc4)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC5, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc5)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC6, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc6)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC7, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc7)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC8, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc8)
		ON_BN_CLICKED(IDC_PEELER_MAIN_MOVE_FUNC9, &CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc9)
		END_MESSAGE_MAP()


// CVPRemoteDlg 메시지 처리기

BOOL CVPRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CString str{};


	{
		CRect rect;

		m_statusBar.Create(this);  //create status bar...  
		m_statusBar.SetIndicators(indicators, 3);  //set indicator...  


		//setting status bar....  
		this->GetClientRect(&rect);  //get client rect...  
		m_statusBar.SetPaneInfo(STATUSBAR_IDX_COMM, IDS_INDICATOR_COMM_STATUS, SBPS_NORMAL, rect.Width() / 3);
		m_statusBar.SetPaneInfo(STATUSBAR_IDX_RESULT, IDS_INDICATOR_MOTOR_STATUS, SBPS_NORMAL, rect.Width() / 3);
		m_statusBar.SetPaneInfo(STATUSBAR_IDX_VER, IDS_INDICATOR_INFOR, SBPS_NORMAL, rect.Width() / 3);
		m_statusBar.MoveWindow(rect.left, rect.bottom - 25, rect.Width(), 25);

		m_statusBar.SetPaneText(STATUSBAR_IDX_COMM, L"serial Communication");
		m_statusBar.SetPaneText(STATUSBAR_IDX_RESULT, L"motor status");
		m_statusBar.SetPaneText(STATUSBAR_IDX_VER, L"mcu fw version");

	}
	{
		//state
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_0))->SetWindowText(DEF_CHECK_STATE_0);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_1))->SetWindowText(DEF_CHECK_STATE_1);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_2))->SetWindowText(DEF_CHECK_STATE_2);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_3))->SetWindowText(DEF_CHECK_STATE_3);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_4))->SetWindowText(DEF_CHECK_STATE_4);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_5))->SetWindowText(DEF_CHECK_STATE_5);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_6))->SetWindowText(DEF_CHECK_STATE_6);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_7))->SetWindowText(DEF_CHECK_STATE_7);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_8))->SetWindowText(DEF_CHECK_STATE_8);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_9))->SetWindowText(DEF_CHECK_STATE_9);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_10))->SetWindowText(DEF_CHECK_STATE_10);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_11))->SetWindowText(DEF_CHECK_STATE_11);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_12))->SetWindowText(DEF_CHECK_STATE_12);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_13))->SetWindowText(DEF_CHECK_STATE_13);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_14))->SetWindowText(DEF_CHECK_STATE_14);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_15))->SetWindowText(DEF_CHECK_STATE_15);
	}

	{
		//option
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_16))->SetWindowText(DEF_CHECK_STATE_16);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_17))->SetWindowText(DEF_CHECK_STATE_17);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_18))->SetWindowText(DEF_CHECK_STATE_18);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_19))->SetWindowText(DEF_CHECK_STATE_19);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_20))->SetWindowText(DEF_CHECK_STATE_20);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_21))->SetWindowText(DEF_CHECK_STATE_21);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_22))->SetWindowText(DEF_CHECK_STATE_22);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_23))->SetWindowText(DEF_CHECK_STATE_23);
																  
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_24))->SetWindowText(DEF_CHECK_STATE_24);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_25))->SetWindowText(DEF_CHECK_STATE_25);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_26))->SetWindowText(DEF_CHECK_STATE_26);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_27))->SetWindowText(DEF_CHECK_STATE_27);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_28))->SetWindowText(DEF_CHECK_STATE_28);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_29))->SetWindowText(DEF_CHECK_STATE_29);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_30))->SetWindowText(DEF_CHECK_STATE_30);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_31))->SetWindowText(DEF_CHECK_STATE_31);
	}

	{
		// error
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_32))->SetWindowText(DEF_CHECK_STATE_32);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_33))->SetWindowText(DEF_CHECK_STATE_33);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_34))->SetWindowText(DEF_CHECK_STATE_34);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_35))->SetWindowText(DEF_CHECK_STATE_35);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_36))->SetWindowText(DEF_CHECK_STATE_36);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_37))->SetWindowText(DEF_CHECK_STATE_37);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_38))->SetWindowText(DEF_CHECK_STATE_38);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_39))->SetWindowText(DEF_CHECK_STATE_39);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_40))->SetWindowText(DEF_CHECK_STATE_40);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_41))->SetWindowText(DEF_CHECK_STATE_41);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_42))->SetWindowText(DEF_CHECK_STATE_42);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_43))->SetWindowText(DEF_CHECK_STATE_43);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_44))->SetWindowText(DEF_CHECK_STATE_44);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_45))->SetWindowText(DEF_CHECK_STATE_45);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_46))->SetWindowText(DEF_CHECK_STATE_46);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_47))->SetWindowText(DEF_CHECK_STATE_47);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_48))->SetWindowText(DEF_CHECK_STATE_48);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_49))->SetWindowText(DEF_CHECK_STATE_49);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_50))->SetWindowText(DEF_CHECK_STATE_50);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_51))->SetWindowText(DEF_CHECK_STATE_51);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_52))->SetWindowText(DEF_CHECK_STATE_52);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_53))->SetWindowText(DEF_CHECK_STATE_53);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_54))->SetWindowText(DEF_CHECK_STATE_54);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_55))->SetWindowText(DEF_CHECK_STATE_55);
																  
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_56))->SetWindowText(DEF_CHECK_STATE_56);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_57))->SetWindowText(DEF_CHECK_STATE_57);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_58))->SetWindowText(DEF_CHECK_STATE_58);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_59))->SetWindowText(DEF_CHECK_STATE_59);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_60))->SetWindowText(DEF_CHECK_STATE_60);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_61))->SetWindowText(DEF_CHECK_STATE_61);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_62))->SetWindowText(DEF_CHECK_STATE_62);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_63))->SetWindowText(DEF_CHECK_STATE_63);
	}

	{
		// input
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_64))->SetWindowText(DEF_CHECK_STATE_64);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_65))->SetWindowText(DEF_CHECK_STATE_65);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_66))->SetWindowText(DEF_CHECK_STATE_66);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_67))->SetWindowText(DEF_CHECK_STATE_67);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_68))->SetWindowText(DEF_CHECK_STATE_68);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_69))->SetWindowText(DEF_CHECK_STATE_69);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_70))->SetWindowText(DEF_CHECK_STATE_70);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_71))->SetWindowText(DEF_CHECK_STATE_71);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_72))->SetWindowText(DEF_CHECK_STATE_72);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_73))->SetWindowText(DEF_CHECK_STATE_73);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_74))->SetWindowText(DEF_CHECK_STATE_74);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_75))->SetWindowText(DEF_CHECK_STATE_75);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_76))->SetWindowText(DEF_CHECK_STATE_76);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_77))->SetWindowText(DEF_CHECK_STATE_77);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_78))->SetWindowText(DEF_CHECK_STATE_78);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_79))->SetWindowText(DEF_CHECK_STATE_79);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_80))->SetWindowText(DEF_CHECK_STATE_80);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_81))->SetWindowText(DEF_CHECK_STATE_81);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_82))->SetWindowText(DEF_CHECK_STATE_82);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_83))->SetWindowText(DEF_CHECK_STATE_83);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_84))->SetWindowText(DEF_CHECK_STATE_84);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_85))->SetWindowText(DEF_CHECK_STATE_85);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_86))->SetWindowText(DEF_CHECK_STATE_86);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_87))->SetWindowText(DEF_CHECK_STATE_87);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_88))->SetWindowText(DEF_CHECK_STATE_88);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_89))->SetWindowText(DEF_CHECK_STATE_89);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_90))->SetWindowText(DEF_CHECK_STATE_90);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_91))->SetWindowText(DEF_CHECK_STATE_91);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_92))->SetWindowText(DEF_CHECK_STATE_92);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_93))->SetWindowText(DEF_CHECK_STATE_93);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_94))->SetWindowText(DEF_CHECK_STATE_94);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_95))->SetWindowText(DEF_CHECK_STATE_95);

		//output
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_96))->SetWindowText(DEF_CHECK_STATE_96);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_97))->SetWindowText(DEF_CHECK_STATE_97);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_98))->SetWindowText(DEF_CHECK_STATE_98);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_99))->SetWindowText(DEF_CHECK_STATE_99);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_100))->SetWindowText(DEF_CHECK_STATE_100);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_101))->SetWindowText(DEF_CHECK_STATE_101);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_102))->SetWindowText(DEF_CHECK_STATE_102);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_103))->SetWindowText(DEF_CHECK_STATE_103);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_104))->SetWindowText(DEF_CHECK_STATE_104);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_105))->SetWindowText(DEF_CHECK_STATE_105);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_106))->SetWindowText(DEF_CHECK_STATE_106);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_107))->SetWindowText(DEF_CHECK_STATE_107);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_108))->SetWindowText(DEF_CHECK_STATE_108);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_109))->SetWindowText(DEF_CHECK_STATE_109);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_110))->SetWindowText(DEF_CHECK_STATE_110);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_111))->SetWindowText(DEF_CHECK_STATE_111);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_112))->SetWindowText(DEF_CHECK_STATE_112);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_113))->SetWindowText(DEF_CHECK_STATE_113);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_114))->SetWindowText(DEF_CHECK_STATE_114);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_115))->SetWindowText(DEF_CHECK_STATE_115);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_116))->SetWindowText(DEF_CHECK_STATE_116);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_117))->SetWindowText(DEF_CHECK_STATE_117);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_118))->SetWindowText(DEF_CHECK_STATE_118);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_119))->SetWindowText(DEF_CHECK_STATE_119);

		((CStatic*)GetDlgItem(IDC_CHECK_STATE_120))->SetWindowText(DEF_CHECK_STATE_120);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_121))->SetWindowText(DEF_CHECK_STATE_121);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_122))->SetWindowText(DEF_CHECK_STATE_122);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_123))->SetWindowText(DEF_CHECK_STATE_123);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_124))->SetWindowText(DEF_CHECK_STATE_124);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_125))->SetWindowText(DEF_CHECK_STATE_125);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_126))->SetWindowText(DEF_CHECK_STATE_126);
		((CStatic*)GetDlgItem(IDC_CHECK_STATE_127))->SetWindowText(DEF_CHECK_STATE_127);
	}
	{
		// output
	}
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CVPRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CVPRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


static void receiveMainDlgCB(void* obj, void* w_parm, void* l_parm)
{
	CVPRemoteDlg* p_this = (CVPRemoteDlg*)obj;

	HAL::ModulePeeler::packet_st rec_data = (*(HAL::ModulePeeler::packet_st*)(l_parm));

	CString content;
	CString str;
	std::array<char, 40> datetime{ 0, };
	UTL::trans::DateFormStr(datetime.data(), (int)datetime.size(), UTL::trans::DataTimeFormat_e::YYYYMMDD_HHMMSS_UU);
	str = UTL::trans::CharToCString(datetime.data());

	if (rec_data.is_tx)
		str.AppendFormat(L" [PC -→> DRIVER] PACKET : ");
	else
		str.AppendFormat(L" [PC <←- DRIVER] PACKET : ");
	for (int i = 0; i < rec_data.buffer_idx; i++)
	{
		str.AppendFormat(L"%02X ", rec_data.buffer[i]);
	}

	str.AppendFormat(L" response ms: %04d", rec_data.resp_ms);

	p_this->AddRxList(str);
#if 0
	int length = *((int*)w_parm);
	int index = 0;

	for (index = 0; index < length; index++)
	{
		uint8_t data = *((uint8_t*)l_parm + index);

	}
#endif

}


void CVPRemoteDlg::AddRxList(CString str)
{
	// TODO: 여기에 구현 코드 추가.
	if (m_rxListBox.GetCount() == 13)
	{
		m_rxListBox.ResetContent();
	}
	m_rxListBox.AddString(str);

}


void CVPRemoteDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER_MAIN_DLG, 50, NULL);
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


void CVPRemoteDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	update();
	CDialogEx::OnTimer(nIDEvent);
}


void CVPRemoteDlg::update()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_lockUpdate)
		return;

	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();

	if (m_popMcuData)
	{
		if (m_popMcuData->IsWindowVisible())
		{

			return;
		}
	}


	if (m_popMotor)
	{
		if (m_popMotor->IsWindowVisible())
		{
			HAL::ModulePeeler::moons_data_st motor_data{};
			peeler->GetMotorState(m_motorIdx);
			peeler->GetMotorData(motor_data, m_motorIdx);
			return;
		}
	}
	//end of if (m_popMcuData->IsWindowVisible())


	{
		peeler->GetMcuState();
		peeler->m_stateReg;

		for (int i = 0; i < 16; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_STATE_0 + i))->SetCheck(((peeler->m_stateReg.ap_state >> i) & 1));
		}

		for (int i = 0; i < 16; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_STATE_16 + i))->SetCheck(((peeler->m_optionReg.ap_option >> i) & 1));
		}

		for (int i = 0; i < 32; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_STATE_32 + i))->SetCheck(((peeler->m_errorReg.ap_error >> i) & 1));
		}
		for (int i = 0; i < 32; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_STATE_64 + i))->SetCheck(((peeler->m_inReg >> i) & 1));
		}
		for (int i = 0; i < 32; i++)
		{
			((CButton*)GetDlgItem(IDC_CHECK_STATE_96 + i))->SetCheck(((peeler->m_outReg >> i) & 1));
		}
	}

	{		
		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_1))->SetCheck(peeler->IsOpenCyl(MCU_OBJ::CYL_PHONE_FORBACK));
		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_2))->SetCheck(peeler->IsCloseCyl(MCU_OBJ::CYL_PHONE_FORBACK));

		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_3))->SetCheck(peeler->IsOpenCyl(MCU_OBJ::CYL_PHONE_OPENCLOSE));
		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_4))->SetCheck(peeler->IsGripCyl(MCU_OBJ::CYL_PHONE_OPENCLOSE));
		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_5))->SetCheck(peeler->IsCloseCyl(MCU_OBJ::CYL_PHONE_OPENCLOSE));

		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_6))->SetCheck(peeler->IsOpenCyl(MCU_OBJ::CYL_VINYLHOLD_UPDOWN));
		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_7))->SetCheck(peeler->IsCloseCyl(MCU_OBJ::CYL_VINYLHOLD_UPDOWN));

		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_8))->SetCheck(peeler->IsOpenCyl(MCU_OBJ::CYL_VINYL_PUSHBACK));
		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CYL_9))->SetCheck(peeler->IsCloseCyl(MCU_OBJ::CYL_VINYL_PUSHBACK));


		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CHECK_VAC))->SetCheck(peeler->IsOnVac(MCU_OBJ::VAC_VINYL));

		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CHECK_VINYL))->SetCheck(peeler->IsDetectVinyl());

		((CButton*)GetDlgItem(IDC_PEELER_MAIN_CHECK_MOTOR_ORG))->SetCheck(peeler->m_stateReg.system_origin_cplt);

	}



	{
		IComm* ptr_comm = m_pSystem->GetSerialCommComponent();
		if (ptr_comm->IsOpened())
			m_statusBar.SetPaneText(STATUSBAR_IDX_COMM, L"Serial Comm Connected ");
		else
			m_statusBar.SetPaneText(STATUSBAR_IDX_COMM, L"Serial Comm Disconnected");

		if (peeler->m_stateReg.motor_on)
		{
			((CButton*)GetDlgItem(IDC_PEELER_MAIN_CHECK_MOTOR_ONOFF))->SetCheck(TRUE);
			m_statusBar.SetPaneText(STATUSBAR_IDX_RESULT, L"Motors Enable");
		}
		else
		{
			((CButton*)GetDlgItem(IDC_PEELER_MAIN_CHECK_MOTOR_ONOFF))->SetCheck(FALSE);
			m_statusBar.SetPaneText(STATUSBAR_IDX_RESULT, L"Motors Disable");
		}
	}

}


void CVPRemoteDlg::LockUpdate(bool is_release)
{
	m_lockUpdate = !(is_release);
}

void CVPRemoteDlg::OnBnClickedPeelerMainPopMotorCtrl()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_popMotor == nullptr)
	{
		m_popMotor = new Cui_PopMoonsControl(this);
		m_popMotor->Create(IDD_MOONSMODBUS_DIALOG, this);
	}
	m_popMotor->ShowWindow(SW_SHOW);
}


void CVPRemoteDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_popMotor)
	{
		delete m_popMotor;
	}
	m_popMotor = nullptr;


	if (m_popMcuData)
	{
		delete m_popMcuData;
	}
	m_popMcuData = nullptr;
}



void CVPRemoteDlg::OnBnClickedPeelerMainOpStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->VitualSW(HAL::ModulePeeler::sw_e::sw_start);
}


void CVPRemoteDlg::OnBnClickedPeelerMainOpStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->VitualSW(HAL::ModulePeeler::sw_e::sw_stop);
}


void CVPRemoteDlg::OnBnClickedPeelerMainOpReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->VitualSW(HAL::ModulePeeler::sw_e::sw_reset);
}


void CVPRemoteDlg::OnBnClickedPeelerMainInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->RequestInitial();

}

void CVPRemoteDlg::OnBnClickedPeelerMainMotorStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->Stop(MCU_OBJ::MOTOR_MAX);
}

void CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	if (peeler->m_stateReg.motor_on)
		peeler->MotorOnOff(MCU_OBJ::MOTOR_MAX, false);
	else
		peeler->MotorOnOff(MCU_OBJ::MOTOR_MAX, true);
}


void CVPRemoteDlg::OnBnClickedPeelerMainMotorReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->ClearState(MCU_OBJ::MOTOR_MAX);
}


void CVPRemoteDlg::OnBnClickedPeelerMainCheckMotorOrg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->MotorOrigin(MCU_OBJ::MOTOR_MAX);
	peeler->ZereSetMotorEncoder(MCU_OBJ::MOTOR_MAX);
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderFor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_PHONE_FORBACK, true);
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_LoaderBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_PHONE_FORBACK, false);
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_PHONE_OPENCLOSE, true);
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigGrip()
{
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	uint32_t pre_ms = UTL::millis();
	constexpr uint32_t timedelay = 1000;
	uint8_t retray_cnt = 3;
	while (retray_cnt)
	{
		peeler->CylOnOff(MCU_OBJ::CYL_PHONE_OPENCLOSE, false, true);

		while (UTL::millis() - pre_ms < timedelay)
		{
			if (peeler->IsGripCyl(MCU_OBJ::CYL_PHONE_OPENCLOSE))
			{
				return;
			}
			else
			{
				pre_ms = UTL::millis();
				retray_cnt--;
			}
		}

	}
}
void CVPRemoteDlg::OnBnClickedPeelerMainCyl_JigClose()
{
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_PHONE_OPENCLOSE, false);
}

void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylHoldUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_VINYLHOLD_UPDOWN, true);
}


void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylHoldDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_VINYLHOLD_UPDOWN, false);
}

void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_VINYL_PUSHBACK, false);
}

void CVPRemoteDlg::OnBnClickedPeelerMainCyl_VinylPush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->CylOnOff(MCU_OBJ::CYL_VINYL_PUSHBACK, true);
}

void CVPRemoteDlg::OnBnClickedCheckState0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedCheckState2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState12()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState13()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState14()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckState15()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CVPRemoteDlg::OnBnClickedCheckOption0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CVPRemoteDlg::OnBnClickedCheckOption1()
{
	SetMcuOptionBit(HAL::ModulePeeler::mcu_option_e::USE_BEEP);
}

void CVPRemoteDlg::OnBnClickedCheckOption2()
{
	SetMcuOptionBit(HAL::ModulePeeler::mcu_option_e::USE_DRYRUN);
}
void CVPRemoteDlg::OnBnClickedCheckOption3()
{
	SetMcuOptionBit(HAL::ModulePeeler::mcu_option_e::SKIP_DOORSENSOR);
}
void CVPRemoteDlg::OnBnClickedCheckOption4()
{
	SetMcuOptionBit(HAL::ModulePeeler::mcu_option_e::SKIP_SAFETYSENSOR);
}
void CVPRemoteDlg::OnBnClickedCheckOption5()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption6()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption7()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption8()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption9()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption10()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption11()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption12()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption13()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption14()
{
}
void CVPRemoteDlg::OnBnClickedCheckOption15()
{
}

void CVPRemoteDlg::OnBnClickedCheckSetOutput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_lockUpdate = true;

	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();

	uint32_t output_reg = 0;
	for (int i = 0; i < 32; i++)
	{
		output_reg |= (((CButton*)GetDlgItem(IDC_CHECK_STATE_96 + i))->GetCheck() << i);
	}
	peeler->SetIO_Output(output_reg);

	m_lockUpdate = false;
}


void CVPRemoteDlg::OnBnClickedPeelerMainPopMcuData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.m_popMcuData
	if (m_popMcuData == nullptr)
	{
		m_popMcuData = new Cui_PopMcuData(this);
		m_popMcuData->Create(IDD_MCU_DATA, this);
	}
	m_popMcuData->ShowWindow(SW_SHOW);
}


void CVPRemoteDlg::SetMcuOptionBit(HAL::ModulePeeler::mcu_option_e bit_reg, bool enable)
{
	// TODO: 여기에 구현 코드 추가.
	HAL::ModulePeeler* peeler = m_pSystem->GetModulePeelerComponent();
	peeler->GetMcuState();
	HAL::ModulePeeler::AP_OPT_REG reg = peeler->m_optionReg;
	if (enable)
	{
		reg.ap_option = reg.ap_option | (1 << bit_reg);
	}
	peeler->SetOptionReg(reg.ap_option);

}


void CVPRemoteDlg::OnBnClickedMainCheckStep1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedMainCheckStep2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedMainCheckStep3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedMainCheckStep4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedMainCheckStep5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedMainCheckStep6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CVPRemoteDlg::OnBnClickedPeelerMainMoveFunc9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
