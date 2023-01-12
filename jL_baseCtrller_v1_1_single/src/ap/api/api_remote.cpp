/*
 * api_remote.cpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */


#include "ap.h"
#include "api_remote.hpp"



constexpr uint8_t STEP_INIT 													= 0x00;
constexpr uint8_t	STEP_TODO 													= 0x01;
constexpr uint8_t	STEP_TIMEOUT												= 0x02;
constexpr uint8_t	STEP_WAIT_RETURN										= 0x03;
constexpr uint8_t	STEP_MCU_STATE											= 0x04;
constexpr uint8_t	STEP_MCU_STATE_START								= 0x05;
constexpr uint8_t	STEP_MCU_STATE_WAIT 								= 0x06;
constexpr uint8_t	STEP_MCU_STATE_END 									= 0x07;

constexpr uint8_t	STEP_MOTOR_DATA 										= 0x08;
constexpr uint8_t	STEP_MOTOR_DATA_START   						= 0x09;
constexpr uint8_t	STEP_MOTOR_DATA_WAIT  							= 0x0a;
constexpr uint8_t	STEP_MOTOR_DATA_END 								= 0x0b;

constexpr uint8_t	STEP_MOTOR_CFG_MOTION_ORIGIN				= 0x0c;
constexpr uint8_t	STEP_MOTOR_CFG_MOTION_ORIGIN_START	= 0x0d;
constexpr uint8_t	STEP_MOTOR_CFG_MOTION_ORIGIN_WAIT		= 0x0e;
constexpr uint8_t	STEP_MOTOR_CFG_MOTION_ORIGIN_END		= 0x0f;

constexpr uint8_t	STEP_ROMDATA_POS_VEL_L							= 0x10;
constexpr uint8_t	STEP_ROMDATA_POS_VEL_L_START				= 0x11;
constexpr uint8_t	STEP_ROMDATA_POS_VEL_L_WAIT					= 0x12;
constexpr uint8_t	STEP_ROMDATA_POS_VEL_L_END					= 0x13;

constexpr uint8_t	STEP_ROMDATA_POS_VEL_H							= 0x14;
constexpr uint8_t	STEP_ROMDATA_POS_VEL_H_START				= 0x15;
constexpr uint8_t	STEP_ROMDATA_POS_VEL_H_WAIT					= 0x16;
constexpr uint8_t	STEP_ROMDATA_POS_VEL_H_END					= 0x17;

constexpr uint8_t	STEP_ROMDATA_LINK_POS_L							= 0x18;
constexpr uint8_t	STEP_ROMDATA_LINK_POS_L_START				= 0x19;
constexpr uint8_t	STEP_ROMDATA_LINK_POS_L_WAIT				= 0x1A;
constexpr uint8_t	STEP_ROMDATA_LINK_POS_L_END					= 0x1B;

constexpr uint8_t	STEP_ROMDATA_LINK_POS_H							= 0x1C;
constexpr uint8_t	STEP_ROMDATA_LINK_POS_H_START				= 0x1D;
constexpr uint8_t	STEP_ROMDATA_LINK_POS_H_WAIT				= 0x1E;
constexpr uint8_t	STEP_ROMDATA_LINK_POS_H_END					= 0x1F;

constexpr uint8_t	STEP_LOG_DATA_L							        = 0x20;
constexpr uint8_t	STEP_LOG_DATA_L_START				        = 0x21;
constexpr uint8_t	STEP_LOG_DATA_L_WAIT				        = 0x22;
constexpr uint8_t	STEP_LOG_DATA_L_END					        = 0x23;

constexpr uint8_t	STEP_LOG_DATA_H							        = 0x24;
constexpr uint8_t	STEP_LOG_DATA_H_START				        = 0x25;
constexpr uint8_t	STEP_LOG_DATA_H_WAIT				        = 0x26;
constexpr uint8_t	STEP_LOG_DATA_H_END					        = 0x27;


constexpr uint8_t STEP_DELAY_WAIT											= 30;
constexpr uint8_t RETRY_CNT_MAX												= 3;
constexpr uint8_t	COMM_TIMEOUT_MAX 										= 100;
constexpr uint8_t COMM_ERR_RECOVERY_LIMIT_CNT         = 10;


void api_remote::ThreadJob()
{
	doRunStep();

	m_cfg.ptr_comm->ReceiveProcess();
}



void api_remote::doRunStep()
{

	using tx_t = RCTRL::TX_TYPE;
	auto make_packet = [&](auto offset, auto source)->uint8_t
			{
		memcpy(&m_txBuffer[offset],&source, sizeof(source));
		return (uint8_t)(offset+sizeof(source));
			};


	switch(m_step.GetStep())
	{
		case STEP_INIT:
		{
			m_step.SetStep(STEP_TODO);
		}
		break;

		/*######################################################
		     to do
		  ######################################################*/
		case STEP_TODO:
		{

		}
		break;
		/*######################################################
				  timeout
			######################################################*/
		case STEP_TIMEOUT:
		{
			m_cfg.ptr_comm->AddErrCnt();
			if (m_cfg.ptr_comm->GetErrCnt() >= COMM_ERR_RECOVERY_LIMIT_CNT)
			{
				m_cfg.ptr_comm->Recovery();
			}
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_TODO);
		}
		break;
		/*######################################################
				  wait return
			######################################################*/
		case STEP_WAIT_RETURN:
		{
			if (m_step.MoreThan(COMM_TIMEOUT_MAX))
			{
				m_step.SetStep(STEP_TIMEOUT);
				break;
			}

			// check return flag
			if (m_waitReplyOK)
				break;

			m_step.SetStep(STEP_TODO);
		}
		break;
		/*######################################################
				STEP_STATE_UPDATE
			######################################################*/
		case STEP_MCU_STATE:
		{
			m_step.retry_cnt = 0;
			m_txBuffer.fill(0);
			m_step.SetStep(STEP_MCU_STATE_START);
		}
		break;

		case STEP_MCU_STATE_START:
		{
			uint8_t idx = 0;	//uint8_t size = 0;

			idx = make_packet(idx, m_cfg.ptr_mcu_reg->state_reg.ap_state);
			idx = make_packet(idx, m_cfg.ptr_mcu_reg->option_reg.ap_option);
			idx = make_packet(idx, m_cfg.ptr_mcu_reg->error_reg.ap_error);
			idx = make_packet(idx, m_cfg.ptr_io->m_in.data);
			idx = make_packet(idx, m_cfg.ptr_io->m_out.data);
			idx = make_packet(idx, m_cfg.ptr_task->m_step.curr_step);
			idx = make_packet(idx, m_cfg.ptr_log->m_Head);
			idx = make_packet(idx, m_cfg.ptr_log->m_Tail);
			m_cfg.ptr_comm->SendData((uint8_t)tx_t::TX_MCU_STATE_DATA, m_txBuffer.data(), idx);

			m_waitReplyOK = true;
			m_step.SetStep(STEP_MCU_STATE_WAIT);
			m_elaps = millis();
		}
		break;

		case STEP_MCU_STATE_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))
				break;

			// check return flag
			if (m_waitReplyOK)
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_MCU_STATE_START);
					break;
				}
				else
				{
					m_step.SetStep(STEP_TIMEOUT);
					break;
				}
			}

			m_step.SetStep(STEP_MCU_STATE_END);
		}
		break;

		case STEP_MCU_STATE_END:
		{
			m_waitReplyOK = false;
			m_step.SetStep(STEP_TODO);
		}
		break;

		/*######################################################
				STEP_MOTOR_DATA
			######################################################*/
		case STEP_MOTOR_DATA:
		{
			//memset(&m_txBuffer[0], 0, UI_RCTRL_MAX_BUFFER_LENGTH);
			m_txBuffer.fill(0);
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_MOTOR_DATA_START);
		}
		break;

		case STEP_MOTOR_DATA_START:
		{
			uint8_t idx = 0;
			MOTOR::enMotor_moons* p_motor{};
			MOTOR::enMotor_moons::moons_data_t motor_data{};

			p_motor = m_cfg.ptr_motors->GetMotorObject(m_idxMotor);//m_idxMotor
			p_motor->GetMotorData(motor_data);

			idx = make_packet(idx, motor_data.drv_status.sc_status);//2
			idx = make_packet(idx, motor_data.al_code.al_status);//2
			idx = make_packet(idx, motor_data.immediate_expanded_input);//2
			idx = make_packet(idx, motor_data.driver_board_inputs);//2
			idx = make_packet(idx, motor_data.encoder_position);//4
			idx = make_packet(idx, motor_data.immediate_abs_position);//4
			idx = make_packet(idx, motor_data.abs_position_command);//4
			idx = make_packet(idx, motor_data.immediate_act_velocity);//2
			idx = make_packet(idx, motor_data.immediate_target_velocity);//2

			m_cfg.ptr_comm->SendData((uint8_t)tx_t::TX_MOTOR_DATA, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;

			m_step.SetStep(STEP_MOTOR_DATA_WAIT);
		}
		break;

		case STEP_MOTOR_DATA_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))
				break;

			// check return flag
			if (m_waitReplyOK)
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_MOTOR_DATA_START);
					break;
				}
				else
				{
					m_step.SetStep(STEP_TIMEOUT);
					break;
				}
			}

			m_step.SetStep(STEP_MOTOR_DATA_END);
		}
		break;

		case STEP_MOTOR_DATA_END:
		{
			m_waitReplyOK = false;
			m_step.SetStep(STEP_TODO);
		}
		break;

		/*######################################################
				STEP_MOTOR_CFG_MOTION_ORIGIN
			######################################################*/
		case STEP_MOTOR_CFG_MOTION_ORIGIN:
		{
			m_txBuffer.fill(0);
			m_step.retry_cnt = 0;
			m_waitReplyOK = true;
			m_step.SetStep(STEP_MOTOR_CFG_MOTION_ORIGIN_START);
		}
		break;

		case STEP_MOTOR_CFG_MOTION_ORIGIN_START:
		{

			uint8_t idx = 0;

			using motor_t = MOTOR::enMotor_moons;
			motor_t* p_motor{};
			motor_t::motion_param_t motion_cfg{};
			motor_t::origin_param_t origin_cfg{};

			p_motor = m_cfg.ptr_motors->GetMotorObject(m_idxMotor);


			p_motor->GetMotionParamData(motion_cfg);
			p_motor->GetOriginParamData(origin_cfg);
			idx = make_packet(idx, motion_cfg.jog_accelC);//4
			idx = make_packet(idx, motion_cfg.jog_speedC);//4
			idx = make_packet(idx, motion_cfg.move_accelC);//4
			idx = make_packet(idx, motion_cfg.move_speedC);//4
			idx = make_packet(idx, origin_cfg.accel);//2
			idx = make_packet(idx, origin_cfg.home_x_no);//1
			idx = make_packet(idx, origin_cfg.home_x_level);//1
			idx = make_packet(idx, origin_cfg.find_home_dir);//4
			idx = make_packet(idx, origin_cfg.speed);//2

			// total 26 bytes
			m_cfg.ptr_comm->SendData((uint8_t)tx_t::TX_MOTOR_CFG_MOTION_ORIGIN, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_MOTOR_CFG_MOTION_ORIGIN_WAIT);
		}
		break;

		case STEP_MOTOR_CFG_MOTION_ORIGIN_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))
				break;

			// check return flag
			if (m_waitReplyOK)
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_MOTOR_CFG_MOTION_ORIGIN_START);
					break;
				}
				else
				{
					m_step.SetStep(STEP_TIMEOUT);
					break;
				}
			}

			m_step.SetStep(STEP_MOTOR_CFG_MOTION_ORIGIN_END);
		}
		break;

		case STEP_MOTOR_CFG_MOTION_ORIGIN_END:
		{
			m_waitReplyOK = false;
			m_step.SetStep(STEP_TODO);
		}
		break;

		/*######################################################
				STEP_ROMDATA_POS_VEL_L
			######################################################*/
		case STEP_ROMDATA_POS_VEL_L:
		{
			m_txBuffer.fill(0);
			m_step.retry_cnt = 0;
			m_waitReplyOK = true;
			m_step.SetStep(STEP_ROMDATA_POS_VEL_L_START);
		}
		break;

		case STEP_ROMDATA_POS_VEL_L_START:
		{
			uint8_t idx = 0;

			constexpr uint8_t data_size = 8;/*static_cast<uint8_t>(mt_jig_pos_addr::_max)*/
			axis_dat::dat_t* p_data{};
			p_data =  m_cfg.ptr_axis_data->cmd_pos_dat;
			idx = make_packet(idx, p_data[0 + ( data_size * m_idxMotor)] );//8
			idx = make_packet(idx, p_data[1 + ( data_size * m_idxMotor)] );//8
			idx = make_packet(idx, p_data[2 + ( data_size * m_idxMotor)] );//8
			idx = make_packet(idx, p_data[3 + ( data_size * m_idxMotor)] );//8

			// total 32 bytes
			m_cfg.ptr_comm->SendData((uint8_t)tx_t::TX_EEPROM_MOTION_DATA_L, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_ROMDATA_POS_VEL_L_WAIT);

		}
		break;

		case STEP_ROMDATA_POS_VEL_L_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))
				break;

			// check return flag
			if (m_waitReplyOK)
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_ROMDATA_POS_VEL_L_START);
					break;
				}
				else
				{
					m_step.SetStep(STEP_TIMEOUT);
					break;
				}
			}

			m_step.SetStep(STEP_ROMDATA_POS_VEL_L_END);
		}
		break;

		case STEP_ROMDATA_POS_VEL_L_END:
		{

			m_step.SetStep(STEP_ROMDATA_POS_VEL_H);
		}
		break;
		/*######################################################
				STEP_ROMDATA_POS_VEL_H
			######################################################*/
		case STEP_ROMDATA_POS_VEL_H:
		{
			m_txBuffer.fill(0);
			m_waitReplyOK = true;
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_ROMDATA_POS_VEL_H_START);
		}
		break;

		case STEP_ROMDATA_POS_VEL_H_START:
		{
			uint8_t idx = 0;

			constexpr uint8_t data_size = 8;/*static_cast<uint8_t>(mt_jig_pos_addr::_max)*/
			axis_dat::dat_t* p_data{};
			p_data =  m_cfg.ptr_axis_data->cmd_pos_dat;
			idx = make_packet(idx, p_data[4 + ( data_size * m_idxMotor)] );//8
			idx = make_packet(idx, p_data[5 + ( data_size * m_idxMotor)] );//8
			idx = make_packet(idx, p_data[6 + ( data_size * m_idxMotor)] );//8
			idx = make_packet(idx, p_data[7 + ( data_size * m_idxMotor)] );//8

			// total 32 bytes
			m_cfg.ptr_comm->SendData((uint8_t)tx_t::TX_EEPROM_MOTION_DATA_H, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_ROMDATA_POS_VEL_H_WAIT);
		}
		break;

		case STEP_ROMDATA_POS_VEL_H_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))
				break;

			// check return flag
			if (m_waitReplyOK)
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_ROMDATA_POS_VEL_H_START);
					break;
				}
				else
				{
					m_step.SetStep(STEP_TIMEOUT);
					break;
				}
			}

			m_step.SetStep(STEP_ROMDATA_POS_VEL_H_END);
		}
		break;

		case STEP_ROMDATA_POS_VEL_H_END:
		{

			m_step.SetStep(STEP_TODO);
		}
		break;

		/*######################################################
				STEP_ROMDATA_LINK_POS_L
			######################################################*/
		case STEP_ROMDATA_LINK_POS_L:
		{
			m_step.retry_cnt = 0;
			m_txBuffer.fill(0);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_ROMDATA_LINK_POS_L_START);
		}
		break;

		case STEP_ROMDATA_LINK_POS_L_START:
		{
			uint8_t idx = 0;

			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[0] );//8
			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[1] );//8
			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[2] );//8
			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[3] );//8

			// total 36 bytes
			m_cfg.ptr_comm->SendData((uint8_t)tx_t::TX_EEPROM_LINK_POS_DATA_L, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_ROMDATA_LINK_POS_L_WAIT);

		}
		break;

		case STEP_ROMDATA_LINK_POS_L_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))
				break;

			// check return flag
			if (m_waitReplyOK)
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_ROMDATA_LINK_POS_L_START);
					break;
				}
				else
				{
					m_step.SetStep(STEP_TIMEOUT);
					break;
				}
			}
			m_step.SetStep(STEP_ROMDATA_LINK_POS_L_END);
		}
		break;

		case STEP_ROMDATA_LINK_POS_L_END:
		{

			m_step.SetStep(STEP_ROMDATA_LINK_POS_H);
		}
		break;
		/*######################################################
				STEP_ROMDATA_LINK_POS_H
			######################################################*/
		case STEP_ROMDATA_LINK_POS_H:
		{
			if(m_step.LessThan(50))
				return;
			m_step.retry_cnt = 0;
			m_txBuffer.fill(0);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_ROMDATA_LINK_POS_H_START);
		}
		break;

		case STEP_ROMDATA_LINK_POS_H_START:
		{
			uint8_t idx = 0;

			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[4] );//8
			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[5] );//8
			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[6] );//8
			idx = make_packet(idx, m_cfg.ptr_linkPose_data->linkpose_dat[7] );//8

			// total 36 bytes
			m_cfg.ptr_comm->SendData((uint8_t)tx_t::TX_EEPROM_LINK_POS_DATA_H, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_ROMDATA_LINK_POS_H_WAIT);

		}
		break;

		case STEP_ROMDATA_LINK_POS_H_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))
				break;

			// check return flag
			if (m_waitReplyOK)
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_ROMDATA_LINK_POS_H_START);
					break;
				}
				else
				{
					m_step.SetStep(STEP_TIMEOUT);
					break;
				}
			}
			m_step.SetStep(STEP_ROMDATA_LINK_POS_H_END);
		}
		break;

		case STEP_ROMDATA_LINK_POS_H_END:
		{

			m_step.SetStep(STEP_TODO);
		}
		break;

		//

		default:
			break;
	}
	// end of switch
}


void api_remote::ProcessCmd(RCTRL::uart_remote::rx_packet_t* ptr_data){
	m_receiveData.Init();
	m_receiveData = ptr_data;

	m_waitReplyOK = false;

	if (m_receiveData.type != RCTRL::CMD_TYPE::CMD_OK_RESPONSE)
		ok_Response();
	else
		return;

	auto make_packet = [&](auto offset, auto source)->uint8_t
			{
		memcpy(&m_txBuffer[offset],&source, sizeof(source));
		return (uint8_t)(offset+sizeof(source));
			};

	auto ret_data = [&](auto offset, auto &dest)->uint8_t
			{
		memcpy(&dest, &m_receiveData.data[offset],sizeof(dest));
		return (uint8_t)(offset+sizeof(dest));
			};
	using TYPE = RCTRL::CMD_TYPE;

	switch (m_receiveData.type)
	{
		case TYPE::CMD_CTRL_MCU_OPTION_REG:
		{
			uint32_t option_reg = (m_receiveData.data[0]<<0) | (m_receiveData.data[1]<<8) ;
			m_cfg.ptr_mcu_reg->SetOptionRegister(option_reg);
		}
		break;
		case TYPE::CMD_CTRL_JOB_INITIAL:
		{
			m_cfg.ptr_task->Initialize();
		}
		break;

		case TYPE::CMD_CTRL_VIRTUAL_SW:
		{
			if ((cnAuto::sw_e)m_receiveData.obj_id == cnAuto::sw_e::sw_reset)
				m_cfg.ptr_auto->ResetSw();
			else if ((cnAuto::sw_e)m_receiveData.obj_id == cnAuto::sw_e::sw_start)
				m_cfg.ptr_auto->StartSw();
			else if ((cnAuto::sw_e)m_receiveData.obj_id == cnAuto::sw_e::sw_stop)
				m_cfg.ptr_auto->StopSw();
		}
		break;

		case TYPE::CMD_CTRL_VAC:
		{
			AP_OBJ::VAC vac_id{};
			if (m_receiveData.obj_id < AP_OBJ::VAC::VAC_MAX)
				vac_id = (AP_OBJ::VAC)m_receiveData.obj_id;
			else
				break;

			bool senser_skip = (bool)(m_receiveData.data[1]&1);
			if (m_receiveData.data[0])
				m_cfg.ptr_task->VacOn(vac_id, senser_skip);
			else
				m_cfg.ptr_task->VacOff(vac_id, senser_skip);
		}
		break;
		case TYPE::CMD_CTRL_CYL:
		{
			AP_OBJ::CYL cyl_id{};
			if (m_receiveData.obj_id < AP_OBJ::CYL::CYL_MAX)
				cyl_id = (AP_OBJ::CYL)m_receiveData.obj_id;
			else
				break;

			bool senser_skip = (bool)(m_receiveData.data[1]&1);
			if (m_receiveData.data[0])
				m_cfg.ptr_task->CylOpen(cyl_id, senser_skip);
			else
				m_cfg.ptr_task->CylClose(cyl_id, senser_skip);
		}
		break;

		case TYPE::CMD_CTRL_IO_OUT:
		{
			uint32_t out_reg = utilDwToUint(m_receiveData.data);
			m_cfg.ptr_io->SetOutputReg(out_reg);
		}
		break;

		case TYPE::CMD_CTRL_MOT_ONOFF:
		{
			m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			bool on_off = bool(m_receiveData.data[0]&1);
			m_cfg.ptr_motors->MotorOnOff(on_off, m_idxMotor);
		}
		break;

		case TYPE::CMD_CTRL_MOT_ZEROSET:
		{
			m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			m_cfg.ptr_motors->MotorZeroEncode(m_idxMotor);
		}
		break;

		case TYPE::CMD_CTRL_MOT_ORIGIN:
		{
			m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			m_cfg.ptr_motors->Origin(m_idxMotor);
		}
		break;

		case TYPE::CMD_CTRL_MOT_MOVE:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;

				int pos = utilDwToInt(&m_receiveData.data[0]);

				m_cfg.ptr_motors->Move(m_idxMotor,pos);
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_RELMOVE:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;

				int pos = utilDwToInt(&m_receiveData.data[0]);

				m_cfg.ptr_motors->RelMove(m_idxMotor, pos);
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_MOVE_VEL:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				int pos = 0;
				uint16_t acc = 0;uint16_t vel = 0;
				uint8_t idx = 0;
				idx = ret_data(idx, pos);
				idx = ret_data(idx, vel);
				idx = ret_data(idx, acc);
				/*int pos = utilDwToInt(&m_receiveData.data[0]);

				uint16_t vel = (uint16_t)(m_receiveData.data[4] << 0)
												 | (uint16_t)(m_receiveData.data[5] << 8);

				uint16_t acc = (uint16_t)(m_receiveData.data[6] << 0)
												 | (uint16_t)(m_receiveData.data[7] << 8);*/

				m_cfg.ptr_motors->Move(m_idxMotor ,pos ,(uint32_t)vel, (uint32_t)acc, (uint32_t)acc);
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_RELMOVE_VEL:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				int pos = 0;
				uint16_t acc = 0;uint16_t vel = 0;
				uint8_t idx = 0;
				idx = ret_data(idx, pos);
				idx = ret_data(idx, vel);
				idx = ret_data(idx, acc);
				/*
				int pos = utilDwToInt(&m_receiveData.data[0]);
				uint16_t vel = (uint16_t)(m_receiveData.data[4] << 0)
												 | (uint16_t)(m_receiveData.data[5] << 8);

				uint16_t acc = (uint16_t)(m_receiveData.data[6] << 0)
												 | (uint16_t)(m_receiveData.data[7] << 8);
				*/
				m_cfg.ptr_motors->RelMove(m_idxMotor, pos , (uint32_t)vel, (uint32_t)acc, (uint32_t)acc);
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_STOP:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				m_cfg.ptr_motors->MotorMoveStop(m_idxMotor);
			}
			else
			{
				m_cfg.ptr_motors->MotorMoveStop();
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_JOG:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				bool is_cw = bool(m_receiveData.data[0]&1);
				m_cfg.ptr_motors->MotorJogMove(m_idxMotor,is_cw);
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_JOG_STOP:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				m_cfg.ptr_motors->MotorJogMoveStop(m_idxMotor);
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_VEL_JOG:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				ok_Response();
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				int dir = 0;
				MOTOR:: uart_moons::speed_t param{};
				uint8_t idx = 0;
				idx = ret_data(idx, param.speed);
				idx = ret_data(idx, param.accel);
				idx = ret_data(idx, dir);
				param.decel = param.accel;

				m_cfg.ptr_motors->MotorJogMove(m_idxMotor, param, (dir<0?false:true));
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_LINK_RUN:
		{
			uint32_t pos =0; uint32_t vel = 0;
			uint8_t idx = 0;
			idx = ret_data(idx, pos);
			idx = ret_data(idx, vel);

			m_cfg.ptr_motors->LinkMove(pos, vel);//
		}
		break;

		case TYPE::CMD_CTRL_MOT_CLEAR_ALARM:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				m_cfg.ptr_motors->MotorClearAlarm(m_idxMotor);
			}
		}
		break;

		case TYPE::CMD_CTRL_MOT_DATA_MOVE_PARAM:
		{
			ok_Response();
			m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			uint32_t vel =0; uint32_t acc = 0; uint32_t dec = 0;
			uint8_t idx = 0;
			idx = ret_data(idx, vel);
			idx = ret_data(idx, acc);
			idx = ret_data(idx, dec);

			m_cfg.ptr_motors->SetParamDataMove(m_idxMotor, vel, acc, dec);//
		}
		break;

		case TYPE::CMD_CTRL_MOT_DATA_INIT_PARAM:
		{
			ok_Response();
			m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			uint8_t idx = 0;
			MOTOR::enMotor_moons::origin_param_t param{};

			idx = ret_data(idx, param.speed);
			idx = ret_data(idx, param.accel);
			idx = ret_data(idx, param.offset);
			idx = ret_data(idx, param.find_home_dir);
			idx = ret_data(idx, param.home_x_no);
			idx = ret_data(idx, param.home_x_level);
			param.decel = param.accel;
			m_cfg.ptr_motors->SetParamDataInit(m_idxMotor, param);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_MOTOR_POS_DATA_L:
		{
			if (m_receiveData.obj_id < AP_OBJ::MOTOR_MAX)
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			else
				break;

			m_cfg.ptr_task->WriteROMData_Pose(m_receiveData);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_MOTOR_POS_DATA_H:
		{
			if (m_receiveData.obj_id < AP_OBJ::MOTOR_MAX)
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			else
				break;

			ok_Response();
			m_cfg.ptr_task->WriteROMData_Pose(m_receiveData, false);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_CFG_DATA:
		{
			m_cfg.ptr_task->WriteROMData_Config(m_receiveData);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_CYL_DATA:
		{
			m_cfg.ptr_task->WriteROMData_Cylinder(m_receiveData);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_VAC_DATA:
		{
			m_cfg.ptr_task->WriteROMData_Vacuum(m_receiveData);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_SEQ_DATA:
		{
			m_cfg.ptr_task->WriteROMData_Sequence(m_receiveData);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_LINK_POS_L:
		{
			m_cfg.ptr_task->WriteROMData_LinkPose(m_receiveData);
		}
		break;

		case TYPE::CMD_EEPROM_WRITE_LINK_POS_H:
		{
			ok_Response();
			m_cfg.ptr_task->WriteROMData_LinkPose(m_receiveData, false);
		}
		break;

		case TYPE::CMD_EEPROM_READ_MOTOR_POS_DATA:
		{
			if (m_receiveData.obj_id < AP_OBJ::MOTOR_MAX)
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			else
				break;
			ok_Response();
			m_step.SetStep(STEP_ROMDATA_POS_VEL_L);

		}
		break;

		case TYPE::CMD_EEPROM_READ_CFG_DATA:
		{
			uint8_t idx = 0;
			for (uint8_t i = 0 ; i < APDAT_CNT_MAX ; i++ )
			{
				idx = make_packet(idx, m_cfg.ptr_cfg_data->apcfg_dat[i]);// total 32
			}
			ok_Response();
			m_cfg.ptr_comm->SendData((uint8_t)RCTRL::TX_TYPE::TX_EEPROM_CONFIG_DATA, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_WAIT_RETURN);
		}
		break;

		case TYPE::CMD_EEPROM_READ_CYL_DATA:
		{
			uint8_t idx = 0;
			for (uint8_t i = 0 ; i < APDAT_CYL_ACT_DATA_CNT_MAX ; i++ )
			{
				idx = make_packet(idx, m_cfg.ptr_cyl_data->cyl_act_dat[i]);// total 32
			}
			ok_Response();
			m_cfg.ptr_comm->SendData((uint8_t)RCTRL::TX_TYPE::TX_EEPROM_CYLINDER_DATA, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_WAIT_RETURN);
		}
		break;

		case TYPE::CMD_EEPROM_READ_VAC_DATA:
		{
			uint8_t idx = 0;
			for (uint8_t i = 0 ; i < APDAT_VAC_ACT_DATA_CNT_MAX ; i++ )
			{
				idx = make_packet(idx, m_cfg.ptr_vac_data->vac_act_dat[i]);// total 32
			}
			ok_Response();
			m_cfg.ptr_comm->SendData((uint8_t)RCTRL::TX_TYPE::TX_EEPROM_VACUUM_DATA, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_WAIT_RETURN);

		}
		break;

		case TYPE::CMD_EEPROM_READ_SEQ_DATA:
		{
			uint8_t idx = 0;
			for (uint8_t i = 0 ; i < APDAT_SEQ_DATA_CNT_MAX ; i++ )
			{
				idx = make_packet(idx, m_cfg.ptr_sequence_data->sequencing_dat[i]);// total 32
			}
			ok_Response();
			m_cfg.ptr_comm->SendData((uint8_t)RCTRL::TX_TYPE::TX_EEPROM_SEQUNCE_DATA, m_txBuffer.data(), idx, (uint8_t)m_idxMotor);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_WAIT_RETURN);
		}
		break;

		case TYPE::CMD_EEPROM_READ_LINK_POS:
		{
			ok_Response();
			m_step.SetStep(STEP_ROMDATA_LINK_POS_L);
		}
		break;

		case TYPE::CMD_EEPROM_READ_LOG:
		{
			ok_Response();

			uint8_t idx = 0;
			log_dat::dat_t* p_data{};
			uint8_t addr = m_receiveData.data[0];

			p_data = m_cfg.ptr_log->Pop(addr);
			idx = make_packet(idx,*(p_data));

			m_cfg.ptr_comm->SendData((uint8_t)RCTRL::TX_TYPE::TX_EEPROM_LOG_DATA, m_txBuffer.data(), idx, addr);
			m_waitReplyOK = true;
			m_step.SetStep(STEP_WAIT_RETURN);
		}
		break;

		case TYPE::CMD_EEPROM_CLEAR_ROM_DATA:
		{
			m_cfg.ptr_task->ClearROMData();
		}
		break;

		case TYPE::CMD_EEPROM_RELOAD_ROM_DATA:
		{
			m_cfg.ptr_task->ReloadROMData();
		}
		break;

		case TYPE::CMD_READ_MOTOR_DATA:
		{
			if (m_receiveData.obj_id < (uint8_t)AP_OBJ::MOTOR_MAX)
			{
				m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
				m_step.SetStep(STEP_MOTOR_DATA);
			}
		}
		break;

		case TYPE::CMD_READ_MOTION_ORG_CFG_DATA:
		{
			m_idxMotor = (AP_OBJ::MOTOR)m_receiveData.obj_id;
			m_step.SetStep(STEP_MOTOR_CFG_MOTION_ORIGIN);
		}
		break;

		case TYPE::CMD_READ_MCU_DATA:
		{
			//ok_Response();
			m_step.SetStep(STEP_MCU_STATE);
		}
		break;
		case TYPE::CMD_DO_JOB_TASK_1:
		{
			ok_Response();
			m_cfg.ptr_task->DoTask_JobReady();
			/*if(m_cfg.ptr_task->DoTask_JobReady() != ERROR_SUCCESS)
			{

			}*/

		}
		break;
		case TYPE::CMD_DO_JOB_TASK_2:
		{

		}
		break;
		case TYPE::CMD_DO_JOB_TASK_3:
		{

		}
		break;
		case TYPE::CMD_DO_JOB_TASK_4:
		{

		}
		break;
		case TYPE::CMD_DO_JOB_TASK_5:
		{

		}
		break;
		case TYPE::CMD_DO_JOB_TASK_6:
		{

		}
		break;
		case TYPE::CMD_DO_JOB_TASK_7:
		{

		}
		break;
		case TYPE::CMD_DO_JOB_TASK_8:
		{

		}
		break;
		case TYPE::CMD_OK_RESPONSE :
			__attribute__((fallthrough));
		default:
			break;
	}// switch (m_receiveData.type)


}





