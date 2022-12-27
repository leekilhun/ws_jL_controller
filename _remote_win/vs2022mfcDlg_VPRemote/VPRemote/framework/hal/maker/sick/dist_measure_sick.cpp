#include "pch.h"
#include "hal_def.h"

#include "_inc/dist_measure_sick.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef USE_HW_DIST_SENSOR_SICK



#define SICK_PACKET_STEP_CHECK_HEADER               0x00
#define SICK_PACKET_STEP_RECEIVE_DATA               0x01

#define LOG_WRITE(fmt, ...)		m_log.syslog_Write(SYSLOG::level::info, m_Attribute.GetObjectID(), m_Attribute.GetLogFileName(), (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define _TRACE(fmt, ...)   m_log.syslog_Write(SYSLOG::level::info, m_Attribute.GetObjectID(), m_Attribute.GetLogFileName(), (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__);\
                           TRACE(fmt, ##__VA_ARGS__)

GDistSensor_sick::GDistSensor_sick(common::Attribute_t common_data, GDistSensor_sick::cfg_t& cfg)
  :Icommon(common_data), m_cfg(cfg), m_TrdLife(), m_hThread(), m_TrdId(), m_lastValue()
  , m_packetData(), m_receiveTime(), m_IsReceiveContinue(false)
{
  m_cfg.p_Comm->AttCallbackFunc(this, receiveDataCB);

  LOG_WRITE("Init distSensor_sick[%d] : ", m_Attribute.GetInstanceNo());
}

GDistSensor_sick::~GDistSensor_sick()
{
  Close();
}

bool GDistSensor_sick::receiveMeausrData()
{
  constexpr int empty = 0;
  bool ret = false;
  uint8_t data = 0;
  if (m_Que.Available() == empty)
  {
    return ret;
  }
  else
  {
    m_Que.Get(&data);
  }

  switch (m_packetData.state)
  {
  case SICK_PACKET_STEP_CHECK_HEADER:
    if (data == DEF_SICK_COMM_STX)
    {
      m_packetData.init();
      m_packetData.index = 0;
      m_packetData.buffer[m_packetData.index++] = data;


      m_packetData.state = SICK_PACKET_STEP_RECEIVE_DATA;      
    }
    break;

  case SICK_PACKET_STEP_RECEIVE_DATA:
    m_packetData.buffer[m_packetData.index] = data;
    if (data == DEF_SICK_COMM_ETX)
    {
      m_packetData.state = SICK_PACKET_STEP_CHECK_HEADER;
      ret = true;
    }
    else
    {
      if (m_packetData.data == nullptr)
      {
        m_packetData.data = &m_packetData.buffer[m_packetData.index];
      }
      m_packetData.data_len++;
      m_packetData.index++;
    }
    break;

  default:
    break;
  }
  //m_packetData
  return ret;
}

bool GDistSensor_sick::receiveContinueData()
{
  bool ret = false;
  constexpr int empty = 0;
  uint8_t data = 0;
  if (m_Que.Available() == empty)
  {
    return ret;
  }
  else
  {
    m_Que.Get(&data);
  }

  switch (data)
  {
  case DEF_SICK_COMM_CR:  // xxx.xxxx<cr> carrige return ������ ����
  {
    m_IsReceiveContinue = true;
    double val = 0.0;
    val = atof((const char*)m_packetData.data);
    m_QmesureValue.Put(val);
    ret = true;
  }
  break;

  case '0': case '1': case '2': case '3': case '4': case '5':    // ���ڰ������̸�
  case '6': case '7': case '8': case '9': case '-': case '.':    // -, . �� ���� ��
  {
    if (m_packetData.data == nullptr)
    {
      m_packetData.data = &m_packetData.buffer[m_packetData.index];
    }
    m_packetData.buffer[m_packetData.index % SICK_PACKET_BUFF_MAX] = data;

    m_packetData.data_len++;
    m_packetData.index++;
  }
  break;

  default:
    break;
  }


  return ret;
}


bool GDistSensor_sick::receiveRespone()
{
  bool ret = false;
  constexpr int empty = 0;
  uint8_t data = 0;
  if (m_Que.Available() == empty)
  {
    return ret;
  }
  else
  {
    m_Que.Get(&data);
  }


  switch (m_packetData.state)
  {
  case SICK_PACKET_STEP_CHECK_HEADER:
    if (data == DEF_SICK_COMM_STX)
    {
      m_packetData.init();
      m_packetData.index = 0;
      m_packetData.buffer[m_packetData.index++] = data;

      m_packetData.state = SICK_PACKET_STEP_RECEIVE_DATA;
    }
    break;

  case SICK_PACKET_STEP_RECEIVE_DATA:
    m_packetData.buffer[m_packetData.index] = data;
    if (data == DEF_SICK_COMM_ETX)
    {
      m_packetData.state = SICK_PACKET_STEP_CHECK_HEADER;
      ret = true;
    }
    else
    {
      if (m_packetData.data == nullptr)
      {
        m_packetData.data = &m_packetData.buffer[m_packetData.index];
      }
      m_packetData.data_len++;
      m_packetData.index++;
    }
    break;

  default:
    break;
  }
  //m_packetData



#if 0

  constexpr uint8_t DEF_INDEX_MAX = 64;
  uint8_t temp[DEF_INDEX_MAX] = { 0, };

  memcpy(&temp, p_data, length);

  for (size_t i = 0; i < length; i++)
  {
    switch (temp[i])
    {
    case '?':  // response - Failure
      break;

    case '>':  // response - confirm
      break;

    case ' ':  // response - seperating value with argument
      break;

    case '0': case '1': case '2': case '3': case '4': case '5':    // ���ڰ������̸�
    case '6': case '7': case '8': case '9': case '-': case '.':    // -, . �� ���� ��
      break;

    default:
      break;
    }
  }


#endif

  return ret;
}


void GDistSensor_sick::threadStop(void)
{
  m_TrdLife = false;
  while (m_TrdId);
}

void GDistSensor_sick::threadRun(void)
{
  m_TrdLife = true;
  // ��Ʈ ���� ������ ����
  m_hThread = CreateThread(
    (LPSECURITY_ATTRIBUTES)NULL,
    0,
    (LPTHREAD_START_ROUTINE)threadFunc,
    (LPVOID)this,
    0,
    &m_TrdId
  );
}

void GDistSensor_sick::threadJob(void)
{
  if (m_Que.Available() == false)
  {
    CmdMeasureDist();
  }

}

UINT GDistSensor_sick::threadFunc(LPVOID pParam)
{
  GDistSensor_sick* pThis = (GDistSensor_sick*)pParam;

  TRACE("distSensor_sick Thread Start\n\r");

  while (pThis->m_TrdLife)
  {
    pThis->threadJob();

    Sleep(10);
  }

  TRACE("distSensor_sick Thread Terminatet\n\r");
  pThis->m_TrdId = 0;
  pThis->m_hThread = NULL;
  return 0;
}

void GDistSensor_sick::receiveDataCB(void* obj, void* w_parm, void* l_parm)
{
  GDistSensor_sick* p_this = (GDistSensor_sick*)obj;
  if (w_parm == nullptr)
    return;
  int length = *((int*)w_parm);
  int index = 0;
  for (index = 0; index < length; index++)
  {
    uint8_t data = *((uint8_t*)l_parm + index);

    p_this->m_Que.Put(data);
    if (p_this->m_IsReceiveContinue)
    {
      p_this->receiveContinueData();
    }
    /*if (data == DEF_SICK_COMM_ETX || data == DEF_SICK_COMM_CR)
    {
      p_this->receivePacket();
    }*/
  }
}

errno_t GDistSensor_sick::Open()
{

  errno_t ret = 0;
  ret = m_cfg.p_Comm->Open();

  if (ret == 0)
  {

    threadRun();
  }
  else
  {   
      _TRACE("fail - dist sensor open\n\r");
   // TRACE("fail - dist sensor open\n\r");
  }

 
  return ret;
}

void GDistSensor_sick::Close()
{
  threadStop();
}

bool GDistSensor_sick::IsOpened() const
{
  return m_TrdLife;
}

void GDistSensor_sick::Recovery()
{
}

double GDistSensor_sick::GetDist()
{
  double ret = 0.0;
  m_QmesureValue.Get(&ret);
  return ret;
}

double GDistSensor_sick::CmdMeasureDist()
{
  double ret = 0.0;
  constexpr uint8_t DEF_INDEX_MAX = 200;
  char temp[DEF_INDEX_MAX] = { 0, };
  uint8_t index = 0;
  temp[index++] = DEF_SICK_COMM_STX;

  index += sprintf_s(&temp[index], 199, DEF_SICK_CMD_MEASURE);
  temp[index++] = DEF_SICK_COMM_ETX;

  m_cfg.p_Comm->SendData(temp, index);
  m_packetData.init();
  m_packetData.index = 0;
  uint32_t pre_ms = UTL::millis();

  while (1)
  {
    if (UTL::millis() - pre_ms > 200)
    {
      _TRACE("sensor receive data tiemout\n\r");
      break;
    }

    if (receiveMeausrData())
    {
      // save data
      m_receiveTime = UTL::millis() - pre_ms;
      //getData(m_packetData.data, m_packetData.data_len);
      ret = atof((const char*)m_packetData.data);
      m_QmesureValue.Put(ret);
      break;
    }

    UTL::delay(5);
  }

  return ret;
}


bool GDistSensor_sick::CmdStartMeasure()
{
  bool ret = false;

  constexpr uint8_t DEF_INDEX_MAX = 200;
  char temp[DEF_INDEX_MAX] = { 0, };
  uint8_t index = 0;
  temp[index++] = DEF_SICK_COMM_STX;

  index += sprintf_s(&temp[index], 199, DEF_SICK_CMD_START_MEASURE);
  temp[index++] = DEF_SICK_COMM_ETX;

  m_cfg.p_Comm->SendData(temp, index);
  m_packetData.init();
  m_packetData.index = 0;

  uint32_t pre_ms = UTL::millis();

  while (1)
  {
    if (UTL::millis() - pre_ms > 200)
    {
      _TRACE("sensor receive data tiemout\n\r");
      break;
    }

    if (receiveContinueData())
    {
      m_receiveTime = UTL::millis() - pre_ms;
      break;
    }

    UTL::delay(5);
  }


  return ret;
}

bool GDistSensor_sick::CmdStopMeasure()
{
  bool ret = false;

  constexpr uint8_t DEF_INDEX_MAX = 200;
  char temp[DEF_INDEX_MAX] = { 0, };
  uint8_t index = 0;
  temp[index++] = DEF_SICK_COMM_STX;

  index += sprintf_s(&temp[index], 199, DEF_SICK_CMD_STOP_MEASURE);
  temp[index++] = DEF_SICK_COMM_ETX;

  m_cfg.p_Comm->SendData(temp, index);
  m_packetData.init();
  m_packetData.index = 0;

  uint32_t pre_ms = UTL::millis();

  while (1)
  {
    if (UTL::millis() - pre_ms > 500)
    {
      _TRACE("sensor receive data tiemout\n\r");
      break;
    }



    if (receiveRespone())
    {
      m_receiveTime = UTL::millis() - pre_ms;
      ret = * (m_packetData.data) == DEF_SICK_COMM_RET_OK ? true : false;
      if (ret)
      {
        m_IsReceiveContinue = false;
      }
      break;
    }

    UTL::delay(5);
  }

  return ret;
}

#endif
