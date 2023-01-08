#include "pch.h"
#include "system_def.h"

#include "system_dat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AP_DATA;
using namespace UTL;

constexpr int SYS_DATA_RESERVE_CNT = 20;

GConfig_dat::GConfig_dat(char* file_info) :parser::json(file_info, GConfig_dat::addr_e::addr_max + SYS_DATA_RESERVE_CNT)/*m_pParser(nullptr)*/
, m_isOpened(false), m_dataCnt(0)
{
	sprintf_s(m_dirFile, PRJ_FILE_DIR_STR_MAX - 1, file_info);
	//m_pParser = new parser::json(m_dirFile, static_cast<uint8_t>(conf_dat::addr_e::_max));
}

GConfig_dat::~GConfig_dat()
{
}

int GConfig_dat::LoadData()
{
	m_dataCnt = parser::json::LoadData();
	if (m_dataCnt > 0)
	{
		m_isOpened = true;
	}
	return m_dataCnt;
}

void GConfig_dat::WriteData(GConfig_dat::addr_e addr, parser::json_t data)
{
	m_mutex.lock();


	m_mutex.unlock();
}

errno_t GConfig_dat::ReadData(GConfig_dat::addr_e addr, void** p_ret)
{
	errno_t ret = ERROR_SUCCESS;

	if (m_isOpened == false)
	{
		return -1;
	}

	parser::json_t config_data = parser::json::ReadData(static_cast<uint32_t>(addr));


	if (config_data.is_array)
	{
		switch (config_data.key_value->type)
		{
		case parser::valueType_e::boolian:
			*(p_ret) = (void*)&config_data.key_value->yesorno;
			break;

		case parser::valueType_e::integer:
			*(p_ret) = (void*)&config_data.key_value->number;
			break;

		case parser::valueType_e::realnumber:
			*(p_ret) = (void*)&config_data.key_value->realnumber;
			break;

		case parser::valueType_e::string:
			*(p_ret) = (void*)&config_data.key_value->str_value;
			break;
		case parser::valueType_e::none:
			__fallthrough;
		default:
			ret = -1;
			break;
		}
	}
	else
	{
		switch (config_data.type)
		{
		case parser::valueType_e::boolian:
			*(p_ret) = (void*)&config_data.yesorno;
			break;
		case parser::valueType_e::integer:
			*(p_ret) = (void*)&config_data.number;
			break;
		case parser::valueType_e::realnumber:
			*(p_ret) = (void*)&config_data.realnumber;
			break;
		case parser::valueType_e::string:
			*(p_ret) = (void*)&config_data.str_value;
			break;
		case parser::valueType_e::none:
			__fallthrough;
		default:
			ret = -1;
			break;
		}
	}

	return ret;
}



uint32_t GConfig_dat::GetDataCnt() const
{
	return parser::json::GetDataCnt();
}

bool GConfig_dat::IsAssignedData() const
{
	return (GetDataCnt() == addr_max);
}


void GConfig_dat::GetSerialConfig(SERIAL_OBJ obj, serialCfgDat_t& p_data)
{
	
	if (m_isOpened)
	{
		uint32_t addr;
		addr = static_cast<uint32_t>(GConfig_dat::addr_e::SERIAL_MCU_NAME);
		p_data.name = parser::json::ReadData(addr).key_value->str_value;
		addr = static_cast<uint32_t>(GConfig_dat::addr_e::SERIAL_MCU_PORT_NAME);
		p_data.port_str = parser::json::ReadData(addr).key_value->str_value;
		addr = static_cast<uint32_t>(GConfig_dat::addr_e::SERIAL_MCU_BAUDRATE);
		p_data.baudrate = parser::json::ReadData(addr).key_value->number;

	}
}

