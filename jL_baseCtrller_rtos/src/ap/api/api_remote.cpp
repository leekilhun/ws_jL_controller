/*
 * api_remote.cpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */


#include "ap.h"
#include "api_remote.hpp"


void api_remote::ProcessCmd(RCTRL::uart_remote::rx_packet_t* ptr_data){
	m_receiveData = ptr_data;
  using TYPE = RCTRL::CMDTYPE;
	switch (m_receiveData.type)
	{
		case TYPE::FIRM_CTRL :
		{

		}
		break;

		case TYPE::EEROM_CTRL:
		{

		}
		break;

		case TYPE::CONTROL_MOT:
		{

		}
		break;

		case TYPE::CONTROL_CYL:
		{

		}
		break;

		case TYPE::CONTROL_VAC:
		{

		}
		break;

		default:
			break;
	}// switch (m_receiveData.type)

#if 0
  m_Packet.is_wait_resp = false;
  m_Packet.reqMotor_id = m_Packet.rx_packet.motor_id;
  switch (m_Packet.rx_packet.cmd_type)
  {
    case UI_REMOTECTRL_CMD_START_SEND_MCU_STATE:
      m_lockSendMsg = false;
      response();

      break;

    case UI_REMOTECTRL_CMD_STOP_SEND_MCU_STATE:
      m_lockSendMsg = true;
      m_Packet.is_wait_resp = true;  //stop communication that sends mcu status to pc
      response();
      break;

    case UI_REMOTECTRL_CMD_OK_RESPONSE:
      break;

    case UI_REMOTECTRL_CMD_READ_ALL_STATE:
      //eventDispatch(UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA);
      PcUpdate();
      break;

    case UI_REMOTECTRL_CMD_READ_BOOT_INFO:
      break;

    case UI_REMOTECTRL_CMD_READ_FIRM_INFO:
      break;

    case UI_REMOTECTRL_CMD_CONTROL_IO_OUT:
    {
      bool value = (bool)m_Packet.rx_packet.data[0];
      uint32_t addr = utilDwToUint(&m_Packet.rx_packet.data[1]);
      m_pAp->ControlIO(addr, value);
      response();
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_JOG:
    {
      //eventDispatch(UI_REMOTECTRL_STEP_CTRL_AXIS_MOVE_JOG);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_Packet.rx_packet.data[0]);
      bool is_cw = (bool)(m_Packet.rx_packet.data[4]);
      if(m_pMotors->m_Cfg.p_Fm[m_Packet.reqMotor_id].JogMove(cmd_vel, is_cw) == OK)
      {
        response();
      }
      else
      {
        //timeout
        response(ret_e::timeout);
      }
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_LIMIT:
    {
      //eventDispatch(UI_REMOTECTRL_STEP_CTRL_AXIS_MOVE_JOG);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_Packet.rx_packet.data[0]);
      bool is_cw = (bool)(m_Packet.rx_packet.data[4]);
      if(m_pMotors->m_Cfg.p_Fm[m_Packet.reqMotor_id].MoveToLimit(cmd_vel, is_cw) == enFaxis::err_e::success)
      {
        response();
      }
      else
      {
        //timeout
        response(ret_e::timeout);
      }
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_ORIGIN:
    {
      // eventDispatch(UI_REMOTECTRL_STEP_CTRL_AXIS_MOVE_ORIGIN);
      if(m_pMotors->m_Cfg.p_Fm[m_Packet.reqMotor_id].MoveOrigin() == enFaxis::err_e::success)
      {
        response();
      }
      else
      {
        //timeout
        response(ret_e::timeout);
      }
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_ONOFF:
    {
      //eventDispatch(UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF);
      // check response ok , 상태 확인은 따로 해야함
      m_Packet.is_wait_resp = true;
      if(m_pMotors->MotorOnOff((bool)m_Packet.rx_packet.data[0], m_Packet.reqMotor_id) == OK)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      m_Packet.is_wait_resp = false;
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_RUN:
    {
      // eventDispatch(UI_REMOTECTRL_STEP_CTRL_AXIS_RUN);
      m_Packet.is_wait_resp = true;
      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_Packet.rx_packet.data[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_Packet.rx_packet.data[4]);
      uint32_t acc = 0;
      acc = utilDwToUint(&m_Packet.rx_packet.data[8]);
      uint32_t decel = 0;
      decel = utilDwToUint(&m_Packet.rx_packet.data[12]);
      if (m_pMotors->Move(m_Packet.reqMotor_id,cmd_pos, cmd_vel, acc, decel) == OK)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      m_Packet.is_wait_resp = false;
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_STOP:
    {
      //eventDispatch(UI_REMOTECTRL_STEP_CTRL_AXIS_STOP);
      if (m_pMotors->Stop(m_Packet.reqMotor_id) == OK)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
    }
    break;


    case UI_REMOTECTRL_CMD_CONTROL_MOTS_ONOFF:
    {
      //motors control on off
      m_Packet.is_wait_resp = true;
      if(m_pMotors->MotorOnOff((bool)m_Packet.rx_packet.data[0]) == OK)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      m_Packet.is_wait_resp = false;
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOTS_RUN:
    { //motors control
      m_Packet.is_wait_resp = true;

      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_Packet.rx_packet.data[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_Packet.rx_packet.data[4]);
      uint32_t acc = 0;
      acc = utilDwToUint(&m_Packet.rx_packet.data[8]);
      uint32_t decel = 0;
      decel = utilDwToUint(&m_Packet.rx_packet.data[12]);
      if (m_pMotors->LinkMove(cmd_pos, cmd_vel, acc, decel) == OK)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }

      m_Packet.is_wait_resp = false;
    }
    break;
    case UI_REMOTECTRL_CMD_CONTROL_MOTS_REL:
    { //motors control
      m_Packet.is_wait_resp = true;

      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_Packet.rx_packet.data[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_Packet.rx_packet.data[4]);
      uint32_t acc = 0;
      acc = utilDwToUint(&m_Packet.rx_packet.data[8]);
      uint32_t decel = 0;
      decel = utilDwToUint(&m_Packet.rx_packet.data[12]);
      if (m_pMotors->LinkMoveRel(cmd_pos, cmd_vel, acc, decel) == OK)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }

      m_Packet.is_wait_resp = false;
    }
    break;
    case UI_REMOTECTRL_CMD_CONTROL_MOTS_STOP:
      //motors control on off
      break;


    case UI_REMOTECTRL_CMD_CONTROL_CYL:
    {
      switch (m_Packet.rx_packet.data[0])
      {
        case AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG:
        case AP_DEF_OBJ_CYLINDER_ID_SQUEEZE:
        {
          switch (m_Packet.rx_packet.data[1])
          {
            case UI_REMOTECTRL_CYL_ACT_CLOSE:
              m_pAp->CylClose(m_Packet.rx_packet.data[0], true);
              break;

            case UI_REMOTECTRL_CYL_ACT_GRIP:
              m_pAp->GripPhone(m_Packet.rx_packet.data[0]);
              break;

            case UI_REMOTECTRL_CYL_ACT_OPEN:
            default:
              m_pAp->CylOpen(m_Packet.rx_packet.data[0], false);
              break;
          }
        }
        break;
        default:
          break;
      }
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_CHANGE_VEL:
    {
      m_Packet.is_wait_resp = true;
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_Packet.rx_packet.data[0]);
      if (m_pMotors->ChangeSpeed(m_Packet.reqMotor_id,cmd_vel) == OK)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      m_Packet.is_wait_resp = false;
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_VAC:
    {
      // data 0 - id, data 1 - on off
      bool on_off = (bool)m_Packet.rx_packet.data[1];
      switch (m_Packet.rx_packet.data[0])
      {
        case AP_DEF_OBJ_VACUUM_ID_PHONE_JIG:
        {
          if (on_off)
          {
            m_pAp->VacOn(m_Packet.rx_packet.data[0], false);
          }
          else
          {
            m_pAp->VacOff(m_Packet.rx_packet.data[0], false);
          }
        }
        break;
        default:
          break;
      }
    }
    break;
    case UI_REMOTECTRL_CMD_CONTROL_MOT_RELMOVE:
    {
      int cmd_dist = 0;
      uint32_t cmd_vel = 0;
      cmd_dist = utilDwToInt(&m_Packet.rx_packet.data[0]);
      cmd_vel =utilDwToUint(&m_Packet.rx_packet.data[4]);
      m_pAp->RelMove(m_Packet.reqMotor_id, cmd_dist, cmd_vel);
    }
    break;

    case UI_REMOTECTRL_CMD_AP_CONFIG_WRITE:
    {
      m_pApReg->SetConfReg(m_Packet.rx_packet.data[0]);
    }
    // m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE);
    break;

    case UI_REMOTECTRL_CMD_READ_MCU_DATA:
    {/*
    if (eventDispatch(UI_REMOTECTRL_STEP_DATA_SEND_ALL_DATA) == uiRemoteCtrl::ret_e::cplt)
    {
      response();
    }
    else
    {
      response(ret_e::timeout);
    }*/
      if (sendData(UI_ROM_DATA_TYPE_MOTOR_POS) == uiRemoteCtrl::ret_e::cplt)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }

      if (sendData(UI_ROM_DATA_TYPE_AP) == uiRemoteCtrl::ret_e::cplt)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }

      if (sendData(UI_ROM_DATA_TYPE_CYL) == uiRemoteCtrl::ret_e::cplt)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      if (sendData(UI_ROM_DATA_TYPE_VAC) == uiRemoteCtrl::ret_e::cplt)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      if (sendData(UI_ROM_DATA_TYPE_SEQ) == uiRemoteCtrl::ret_e::cplt)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }

      if (sendData(UI_ROM_DATA_TYPE_LINK) == uiRemoteCtrl::ret_e::cplt)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }


    }
    break;

    case UI_REMOTECTRL_CMD_RELOAD_ROM_DATA:
      m_Packet.is_wait_resp = true;
      if(m_pAp->ReloadRomData())
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      m_Packet.is_wait_resp = false;
      break;
    case UI_REMOTECTRL_CMD_CLEAR_ROM_DATA:
      m_Packet.is_wait_resp = true;
      if(m_pAp->ClearRomData())
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      m_Packet.is_wait_resp = false;
      break;

    case UI_REMOTECTRL_CMD_WRITE_MOTOR_POS_DATA:
      m_Packet.is_wait_resp = true;
      m_pAp->WriteRomData(UI_ROM_DATA_TYPE_MOTOR_POS, &m_Packet.rx_packet.data[0], m_Packet.rx_packet.length);
      //writeData(UI_ROM_DATA_TYPE_MOTOR_POS, &m_Packet.rx_packet.data[0], m_Packet.rx_packet.length);
      response();
      m_Packet.is_wait_resp = false;
      break;

    case UI_REMOTECTRL_CMD_WRITE_AP_DATA:
      m_Packet.is_wait_resp = true;
      m_pAp->WriteRomData(UI_ROM_DATA_TYPE_AP, &m_Packet.rx_packet.data[0], m_Packet.rx_packet.length);
      response();
      m_Packet.is_wait_resp = false;
      break;

    case UI_REMOTECTRL_CMD_WRITE_CYL_DATA:
      m_Packet.is_wait_resp = true;
      m_pAp->WriteRomData(UI_ROM_DATA_TYPE_CYL, &m_Packet.rx_packet.data[0], m_Packet.rx_packet.length);
      response();
      m_Packet.is_wait_resp = false;
      break;

    case UI_REMOTECTRL_CMD_WRITE_VAC_DATA:
      m_Packet.is_wait_resp = true;
      m_pAp->WriteRomData(UI_ROM_DATA_TYPE_VAC, &m_Packet.rx_packet.data[0], m_Packet.rx_packet.length);
      response();
      m_Packet.is_wait_resp = false;
      break;

    case UI_REMOTECTRL_CMD_WRITE_SEQ_DATA:
      m_Packet.is_wait_resp = true;
      m_pAp->WriteRomData(UI_ROM_DATA_TYPE_SEQ, &m_Packet.rx_packet.data[0], m_Packet.rx_packet.length);
      response();
      m_Packet.is_wait_resp = false;
      break;

    case UI_REMOTECTRL_CMD_WRITE_LINK_DATA:
      m_Packet.is_wait_resp = true;
      m_pAp->WriteRomData(UI_ROM_DATA_TYPE_LINK, &m_Packet.rx_packet.data[0], m_Packet.rx_packet.length);
      response();
      m_Packet.is_wait_resp = false;
      break;

    case UI_REMOTECTRL_CMD_MOTOR_PARM_GET:
    {
      int ret_value = 0;
      if (m_pMotors->m_Cfg.p_Fm[m_Packet.reqMotor_id].GetMotorParameter(&ret_value,m_Packet.rx_packet.data[0]) == enFaxis::err_e::success)
      {
        m_retParamData.index = m_Packet.rx_packet.data[0];
        memcpy(&m_retParamData.data, &ret_value, sizeof(m_retParamData.data));
        retGetValue();
        //response();
      }
      else
      {
        response(ret_e::timeout);
      }



    }
    break;

    case UI_REMOTECTRL_CMD_MOTOR_PARM_SET:
    {
      int value = utilDwToInt(&m_Packet.rx_packet.data[1]);
      if (m_pMotors->m_Cfg.p_Fm[m_Packet.reqMotor_id].SetMotorParameter(m_Packet.rx_packet.data[0], value) == enFaxis::err_e::success)
      {
        response();
      }
      else
      {
        response(ret_e::timeout);
      }
      // response();
    }
    break;

    case UI_REMOTECTRL_CMD_CONTROL_MOT_CLEAR_ALARM:
      m_pMotors->m_Cfg.p_Fm[m_Packet.reqMotor_id].ClearState();
      break;

    case UI_REMOTECTRL_CMD_MOT_ENCODER_ZEROSET:
      m_pMotors->m_Cfg.p_Fm[m_Packet.reqMotor_id].ClearPos();
      break;

    case UI_REMOTECTRL_CMD_EVENT_MCU_VIRTUAL_SW:
    {
      switch (m_Packet.rx_packet.data[0])
      {
        case _GPIO_OP_SW_START:
          m_pAuto->UiStartSw();
          break;

        case _GPIO_OP_SW_STOP:
          m_pApReg->SetRunState(AP_REG_AUTO_RUNNING, false);
          m_pAuto->StopSw();
          m_pAp->ResetStep();
          break;

        case _GPIO_OP_SW_RESET:
          m_pAuto->ResetSw();
          m_pAp->ResetStep();
          break;

        case _GPIO_OP_SW_ESTOP:
        default:
          break;
      }
      response();
    }
    break;
    case UI_REMOTECTRL_CMD_CONTROL_JOB_INITIAL:
    {
      m_pAp->Initialize();
      m_pJob->Initialize();
    }
    break;
    case UI_REMOTECTRL_CMD_TEST_STRESS:
    {
      uint32_t cnt = utilDwToUint(&m_Packet.rx_packet.data[1]) ;
      if (m_Packet.rx_packet.data[0])
      {
        m_pAp->TestStressVertical(cnt);
      }
      else
      {
        m_pAp->TestStressHolizontal(cnt);
      }
    }
    break;
    default:
      // cmdRobotro_SendCmd(m_pCmd, m_pCmd->rx_packet.cmd_type, UI_REMOTECTRL_ERR_WRONG_CMD, NULL);
      break;
  }

  m_pre_time = millis();
#endif

}




