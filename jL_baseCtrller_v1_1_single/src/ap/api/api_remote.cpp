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

				m_cfg.ptr_motors->Move(m_idxMotor,(uint32_t)vel, (uint32_t)acc, (uint32_t)acc ,pos);
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
				m_cfg.ptr_motors->RelMove(m_idxMotor, (uint32_t)vel, (uint32_t)acc, (uint32_t)acc, pos);
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

		case TYPE::CMD_OK_RESPONSE :
			__attribute__((fallthrough));
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





