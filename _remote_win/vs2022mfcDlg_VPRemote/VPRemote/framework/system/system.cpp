#include "pch.h"

#include "system.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define SYS_NO_ERR_RETURN

using namespace AP_SYS;
using namespace AP_OBJ;


MSystem::MSystem() :m_ProjectDir()
, m_pSerialComm(), m_pObjinfo(nullptr), m_pCfgDat{}, m_pModulePeeler{}
{

	init();
}

MSystem::~MSystem()
{
	terminate();
	init();
}

void MSystem::init()
{
	m_pObjinfo = nullptr;
	m_pSerialComm = nullptr;
	m_pModulePeeler = nullptr;
}

void MSystem::terminate()
{
	stopThreads();
	destroy();
}

void MSystem::destroy()
{

	if (m_pModulePeeler) { delete m_pModulePeeler; } m_pModulePeeler = nullptr;
	if (m_pSerialComm) { delete m_pSerialComm; }  m_pSerialComm = nullptr;
	if (m_pCfgDat) { delete m_pCfgDat; } m_pCfgDat = nullptr;
	if (m_pObjinfo) { delete m_pObjinfo; } m_pObjinfo = nullptr;
}

void MSystem::startThreads()
{
	m_pSerialComm->Open();
}

void MSystem::stopThreads()
{
	

}

void MSystem::getMcuData()
{
}

errno_t MSystem::_createSerialCommComponents()
{
	errno_t ret = ERROR_SUCCESS;

	assert(m_pCfgDat);

	AP_DATA::GConfig_dat::serialCfgDat_t data{};

	m_pCfgDat->GetSerialConfig(AP_OBJ::SERIAL_OBJ::module_peeler, data);


	HAL::GSerialComm::cfg_t cfg = {};
	cfg.SetPortName(data.port_str);
	cfg.port_no = 5; // ignore
	cfg.baudrate = data.baudrate;


	int serial_idx = static_cast<int>(SERIAL_OBJ::module_peeler);
	int obj_id = HL_SERIAL_BASE + serial_idx;
	m_pSerialComm = new HAL::GSerialComm(m_pObjinfo->GetCommonData(obj_id), cfg);

	if (m_pObjinfo->RegisterComponent(obj_id, (Icommon*)m_pSerialComm) != ERROR_SUCCESS)
	{
		ERR_PRINT("RegisterComponent Result Fail!(HL_SERIAL_BASE, m_pSerialComm)\n");
#ifndef SYS_NO_ERR_RETURN
		return -1;
#endif
	}


	return ret;
}

errno_t MSystem::_createBasicHWComponents()
{
	errno_t ret = ERROR_SUCCESS;
	HAL::ModulePeeler::cfg_t cfg{};
	cfg.p_Comm = GetSerialCommComponent();

	int obj_id = ML_COMPONENTS_BASE + 0;
	m_pModulePeeler = new HAL::ModulePeeler(m_pObjinfo->GetCommonData(obj_id), cfg);
	if (m_pObjinfo->RegisterComponent(obj_id, (Icommon*)m_pModulePeeler) != ERROR_SUCCESS)
	{
		ERR_PRINT("RegisterComponent Result Fail!(ML_COMPONENTS_BASE, m_pModulePeeler)\n");
#ifndef SYS_NO_ERR_RETURN
		return -1;
#endif
	}
	return ret;
}

errno_t MSystem::Initialize()
{
	errno_t ret = ERROR_SUCCESS;
	assert(this != nullptr);

	m_pObjinfo = new MSystemObject();

	m_pCfgDat = new AP_DATA::GConfig_dat();
	std::string table;
	int file_size = m_pCfgDat->LoadData();
	if (file_size > 0)
	{
		if (m_pCfgDat->IsAssignedData())
		{
			table.reserve(size_t(file_size) + 1);
			table = m_pCfgDat->PrintData();
		}
		else
		{
			ERR_PRINT("System Config file 내용이 assinged 상태가 아닙니다.");
			AfxMessageBox(L"System Config file 내용이 assinged 상태가 아닙니다.");
			return -1;
		}

	}
	else
	{
		ERR_PRINT("System Config file를 열지 못했습니다.");
		AfxMessageBox(L"System Config file를 열지 못했습니다.");
		return -1;
	}

	if (_createSerialCommComponents() != ERROR_SUCCESS)
	{
		ERR_PRINT(" _createSerialCommComponents func fail");
		AfxMessageBox(L"_createSerialCommComponents 완료하지 못했습니다.");
	}
	
	if (_createBasicHWComponents() != ERROR_SUCCESS)
	{
		ERR_PRINT(" _createBasicHWComponents func fail");
		AfxMessageBox(L"_createBasicHWComponents 완료하지 못했습니다.");
	}

	startThreads();

	return ret;
}


IComm* MSystem::GetSerialCommComponent() const
{
	return m_pSerialComm;
}

ModulePeeler* AP_SYS::MSystem::GetModulePeelerComponent() const
{
	return m_pModulePeeler;
}

