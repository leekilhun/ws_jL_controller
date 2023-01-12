/*
 * cnTasks.cpp
 *
 *  Created on: 2023. 1. 11.
 *      Author: gns2.lee
 */
#include "ap.h"
#include "cnTasks.hpp"

#define PROC_ALARM(p_head, p_err)                 m_cfg.p_AutoManger->AUTO_ALARM(p_head, p_err)

constexpr uint8_t STEP_INIT 																					= 0x00;
constexpr uint8_t	STEP_TODO 													                = 0x01;
constexpr uint8_t	STEP_TIMEOUT												                = 0x02;
constexpr uint8_t	STEP_WAIT_DONE  										                = 0x03;
constexpr uint8_t	STEP_WAIT_RETURN										                = 0x04;

constexpr uint8_t	STEP_DO_READY_MOVE_HIGH						                  = 0x05;
constexpr uint8_t	STEP_DO_READY_MOVE_HIGH_START			                  = 0x06;
constexpr uint8_t	STEP_DO_READY_MOVE_HIGH_WAIT 	                      = 0x07;
constexpr uint8_t	STEP_DO_READY_MOVE_HIGH_END 				                = 0x08;
constexpr uint8_t	STEP_DO_READY_MOVE_ROLL					                    = 0x09;
constexpr uint8_t	STEP_DO_READY_MOVE_ROLL_START			                  = 0x0a;
constexpr uint8_t	STEP_DO_READY_MOVE_ROLL_WAIT 								        = 0x0b;
constexpr uint8_t	STEP_DO_READY_MOVE_ROLL_END 								        = 0x0c;
constexpr uint8_t	STEP_DO_READY_MOVE_JIG					                    = 0x0d;
constexpr uint8_t	STEP_DO_READY_MOVE_JIG_START			                  = 0x0e;
constexpr uint8_t	STEP_DO_READY_MOVE_JIG_WAIT 								        = 0x0f;
constexpr uint8_t	STEP_DO_READY_MOVE_JIG_END 									        = 0x10;
constexpr uint8_t	STEP_DO_READY_CYL_JIG_OPEN			                    = 0x11;
constexpr uint8_t	STEP_DO_READY_CYL_JIG_OPEN_START	                  = 0x12;
constexpr uint8_t	STEP_DO_READY_CYL_JIG_OPEN_WAIT 						        = 0x13;
constexpr uint8_t	STEP_DO_READY_CYL_JIG_OPEN_END 							        = 0x14;
constexpr uint8_t	STEP_DO_STICKY_CYL_JIG_CLOSE		                    = 0x15;
constexpr uint8_t	STEP_DO_STICKY_CYL_JIG_CLOSE_START                  = 0x16;
constexpr uint8_t	STEP_DO_STICKY_CYL_JIG_CLOSE_WAIT 					        = 0x17;
constexpr uint8_t	STEP_DO_STICKY_CYL_JIG_CLOSE_END 						        = 0x18;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_READY		                  = 0x19;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_READY_START                = 0x1a;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_READY_WAIT 				        = 0x1b;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_READY_END 					        = 0x1c;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON               = 0x1d;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_START         = 0x1e;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_WAIT          = 0x1f;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_END 	        = 0x20;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW                  = 0x21;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_START            = 0x22;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_WAIT 				    = 0x23;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_END 					    = 0x24;
constexpr uint8_t	STEP_DO_STICKY_CHECK_VAC_ON_VINYL                   = 0x25;
constexpr uint8_t	STEP_DO_STICKY_CHECK_VAC_ON_VINYL_START		          = 0x26;
constexpr uint8_t	STEP_DO_STICKY_CHECK_VAC_ON_VINYL_WAIT 		  		    = 0x27;
constexpr uint8_t	STEP_DO_STICKY_CHECK_VAC_ON_VINYL_END 					    = 0x28;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_FAST                  = 0x29;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_START            = 0x2a;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_WAIT 				    = 0x2b;
constexpr uint8_t	STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_END 					    = 0x2c;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_UP		                      = 0x2d;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_UP_START                   = 0x2e;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_UP_WAIT 				            = 0x2f;
constexpr uint8_t	STEP_DO_STICKY_MOVE_HIGH_UP_END 					          = 0x30;
constexpr uint8_t	STEP_DO_RETRUN_MOVE_JIG_OPEN				                = 0x31;
constexpr uint8_t	STEP_DO_RETRUN_MOVE_JIG_OPEN_START			            = 0x32;
constexpr uint8_t	STEP_DO_RETRUN_MOVE_JIG_OPEN_WAIT 								  = 0x33;
constexpr uint8_t	STEP_DO_RETRUN_MOVE_JIG_OPEN_END 									  = 0x34;
constexpr uint8_t	STEP_DO_TRASH_MOVE_ROLL						                  = 0x35;
constexpr uint8_t	STEP_DO_TRASH_MOVE_ROLL_START            						= 0x36;
constexpr uint8_t	STEP_DO_TRASH_MOVE_ROLL_WAIT 				                = 0x37;
constexpr uint8_t	STEP_DO_TRASH_MOVE_ROLL_END 					              = 0x38;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_UP			                    = 0x39;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_UP_START      						  = 0x3a;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_UP_WAIT 	                  = 0x3b;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_UP_END 	                  = 0x3c;
constexpr uint8_t	STEP_DO_TRASH_CHECK_VINYL						                = 0x3d;
constexpr uint8_t	STEP_DO_TRASH_CHECK_VINYL_START            					= 0x3e;
constexpr uint8_t	STEP_DO_TRASH_CHECK_VINYL_WAIT 				              = 0x3f;
constexpr uint8_t	STEP_DO_TRASH_CHECK_VINYL_END 					            = 0x40;
constexpr uint8_t	STEP_DO_TRASH_LINK_MOVE_CLEAR				                = 0x41;
constexpr uint8_t	STEP_DO_TRASH_LINK_MOVE_CLEAR_START        					= 0x42;
constexpr uint8_t	STEP_DO_TRASH_LINK_MOVE_CLEAR_WAIT 		              = 0x43;
constexpr uint8_t	STEP_DO_TRASH_LINK_MOVE_CLEAR_END 			            = 0x44;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_DOWN			                  = 0x45;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_DOWN_START      						= 0x46;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_DOWN_WAIT 	                = 0x47;
constexpr uint8_t	STEP_DO_TRASH_REMOVE_CYL_DOWN_END 	                = 0x48;
constexpr uint8_t	STEP_DO_TRASH_PUSH_CYL_PUSH 				                = 0x49;
constexpr uint8_t	STEP_DO_TRASH_PUSH_CYL_PUSH_START        					  = 0x4a;
constexpr uint8_t	STEP_DO_TRASH_PUSH_CYL_PUSH_WAIT 		                = 0x4b;
constexpr uint8_t	STEP_DO_TRASH_PUSH_CYL_PUSH_END 			              = 0x4c;
constexpr uint8_t	STEP_DO_PEEL_VINYL_FINISH   				                = 0x4d;
constexpr uint8_t	STEP_DO_PEEL_VINYL_FINISH_START          					  = 0x4e;
constexpr uint8_t	STEP_DO_PEEL_VINYL_FINISH_WAIT 	  	                = 0x4f;
constexpr uint8_t	STEP_DO_PEEL_VINYL_FINISH_END 			                = 0x50;


constexpr uint16_t STEP_DELAY_WAIT										                = 500;
constexpr uint8_t RETRY_CNT_MAX												                = 3;
constexpr uint16_t	COMM_TIMEOUT_MAX 									                = 1000;
constexpr uint8_t COMM_ERR_RECOVERY_LIMIT_CNT                         = 10;

void cnTasks::ThreadJob()
{
	switch (m_cfg.p_AutoManger->GetOPMode())
	{
		case enOp::mode_e::STOP:
			m_IsAutoReady = false;
			/*if (millis() - m_pre_time >= 1000 * 1)
	    {
	      m_step.SetStep(CN_JOB_STEP_TODO);
	    }*/
			break;

		case enOp::mode_e::DRY_RUN:			__attribute__((fallthrough));
		case enOp::mode_e::AUTORUN:			__attribute__((fallthrough));
		case enOp::mode_e::READY:
		{
			switch (m_cfg.p_AutoManger->GetOPStatus())
			{
				case enOp::status_e::ERR_STOP:
					m_IsAutoReady = false;
					break;

				case enOp::status_e::RUN_READY:
					m_IsAutoReady = true;
					break;

				case enOp::status_e::RUN:
					if (!m_cfg.p_AutoManger->IsDetectAreaSensor())
					{
						doRunStep();
					}
					break;

				case enOp::status_e::STEP_STOP:
					break;
				/////////////////
				default:		break;
			}
			// switch(m_cfg.p_AutoManger->GetOPStatus())
		}
		break;

		default:
			break;
	}
	// switch(m_cfg.p_AutoManger->GetOPMode())
	doRunStep();
}




void cnTasks::doRunStep()
{
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
			m_step.retry_cnt = 0;		m_step.wait_resp = false;
			m_step.SetStep(STEP_TODO);
		}
		break;
		/*######################################################
					  wait return
			######################################################*/
		case STEP_WAIT_RETURN:
		{
			if (m_step.MoreThan(COMM_TIMEOUT_MAX)){	m_step.SetStep(STEP_TIMEOUT);	break;}

			// check return flag
			m_step.SetStep(STEP_TODO);
		}
		break;

		/*######################################################
					  wait done
			######################################################*/
		case STEP_WAIT_DONE:
		{
			if (m_step.MoreThan(COMM_TIMEOUT_MAX)){	m_step.SetStep(STEP_TIMEOUT);	break; }


			// check return flag

			if (m_step.wait_step) {	m_step.SetStep(m_step.wait_step); m_step.wait_resp = false; break; 	}
			m_step.SetStep(STEP_TODO);
		}
		break;
		/*######################################################
					STEP_DO_READY_MOVE_HIGH
			######################################################*/
		case STEP_DO_READY_MOVE_HIGH:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_READY_MOVE_HIGH_START);
		}
		break;

		case STEP_DO_READY_MOVE_HIGH_START:
		{
			// 1. move - z-axis, 2. move roll-axis,  3. move jig-axis
			// 4. open jig
			/*
			while ((m_cfg.p_motors->Move(AP_OBJ::MOTOR_HIGH, 10, 100, 100, 100000) != ERROR_SUCCESS)
					|| (m_step.retry_cnt++ < RETRY_CNT_MAX))
			{
				if (m_cfg.p_motors->IsInPose(AP_OBJ::MOTOR_HIGH))  {break;}
				if (m_cfg.p_motors->IsMotorRun(AP_OBJ::MOTOR_HIGH)){break;}

			}
      */
			axis_dat::dat_t pose_data = m_cfg.p_apAxisDat->ReadData(static_cast<uint8_t>(mt_high_pos_addr::ready));

			MotorRun(AP_OBJ::MOTOR_HIGH, pose_data.cmd_pos, pose_data.cmd_vel);
			//m_cfg.p_motors->Move(AP_OBJ::MOTOR_HIGH, pose_data.cmd_pos, pose_data.cmd_vel);

			m_step.SetStep(STEP_DO_READY_MOVE_HIGH_WAIT);
		}
		break;

		case STEP_DO_READY_MOVE_HIGH_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsMotorMoveCplt(AP_OBJ::MOTOR_HIGH))
			{
				m_step.SetStep(STEP_DO_READY_MOVE_HIGH_END);
				break;
			}
			else
			{

				if (IsMotorRun(AP_OBJ::MOTOR_HIGH))
				{
					break;
				}
				else
				{
					if (m_step.retry_cnt++ < RETRY_CNT_MAX)
					{
						m_step.SetStep(STEP_DO_READY_MOVE_HIGH_START);
						break;
					}
					else
					{
						// error
						m_step.retry_cnt = 0;
						log_dat::head_t head_inf;
						head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
						head_inf.obj_id = AP_OBJ::MOTOR_HIGH;
						head_inf.step_no = STEP_DO_READY_MOVE_HIGH_WAIT;
						PROC_ALARM(&head_inf,"ready_pos move timeout");
						m_step.SetStep(STEP_DO_READY_MOVE_HIGH_START);
					}
				}
			}
			//if
		}
		break;

		case STEP_DO_READY_MOVE_HIGH_END:
		{
			m_step.SetStep(STEP_DO_READY_MOVE_ROLL);
		}
		break;

		/*######################################################
					STEP_DO_READY_MOVE_ROLL
			######################################################*/
		case STEP_DO_READY_MOVE_ROLL:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_READY_MOVE_ROLL_START);
		}
		break;

		case STEP_DO_READY_MOVE_ROLL_START:
		{
			axis_dat::dat_t pose_data = m_cfg.p_apAxisDat->ReadData(static_cast<uint8_t>(mt_roll_pos_addr::ready));
			MotorRun(AP_OBJ::MOTOR_ROLL, pose_data.cmd_pos, pose_data.cmd_vel);


			m_step.SetStep(STEP_DO_READY_MOVE_ROLL_WAIT);
		}
		break;

		case STEP_DO_READY_MOVE_ROLL_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsMotorMoveCplt(AP_OBJ::MOTOR_ROLL))
			{
				m_step.SetStep(STEP_DO_READY_MOVE_ROLL_END);
				break;
			}
			else
			{
				if (IsMotorRun(AP_OBJ::MOTOR_ROLL))
				{
					break;
				}
				else
				{
					if (m_step.retry_cnt++ < RETRY_CNT_MAX)
					{
						m_step.SetStep(STEP_DO_READY_MOVE_ROLL_START);
						break;
					}
					else
					{
						// error
						m_step.retry_cnt = 0;
						log_dat::head_t head_inf;
						head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
						head_inf.obj_id = AP_OBJ::MOTOR_ROLL;
						head_inf.step_no = STEP_DO_READY_MOVE_ROLL_WAIT;
						PROC_ALARM(&head_inf,"ready_pos move timeout");
						m_step.SetStep(STEP_DO_READY_MOVE_ROLL_START);
					}
				}
			}
			//if
		}
		break;

		case STEP_DO_READY_MOVE_ROLL_END:
		{
			m_step.SetStep(STEP_DO_READY_MOVE_JIG);
		}
		break;

		/*######################################################
					STEP_DO_READY_MOVE_JIG
			######################################################*/
		case STEP_DO_READY_MOVE_JIG:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_READY_MOVE_JIG_START);
		}
		break;

		case STEP_DO_READY_MOVE_JIG_START:
		{
			axis_dat::dat_t pose_data = m_cfg.p_apAxisDat->ReadData(static_cast<uint8_t>(mt_jig_pos_addr::ready));
			MotorRun(AP_OBJ::MOTOR_JIG, pose_data.cmd_pos, pose_data.cmd_vel);


			m_step.SetStep(STEP_DO_READY_MOVE_JIG_WAIT);
		}
		break;

		case STEP_DO_READY_MOVE_JIG_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsMotorMoveCplt(AP_OBJ::MOTOR_JIG))
			{
				m_step.SetStep(STEP_DO_READY_MOVE_JIG_END);
				break;
			}
			else
			{
				if (IsMotorRun(AP_OBJ::MOTOR_JIG))
				{
					break;
				}
				else
				{
					if (m_step.retry_cnt++ < RETRY_CNT_MAX)
					{
						m_step.SetStep(STEP_DO_READY_MOVE_JIG_START);
						break;
					}
					else
					{
						// error
						m_step.retry_cnt = 0;
						log_dat::head_t head_inf;
						head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
						head_inf.obj_id = AP_OBJ::MOTOR_JIG;
						head_inf.step_no = STEP_DO_READY_MOVE_JIG_WAIT;
						PROC_ALARM(&head_inf,"ready_pos move timeout");
						m_step.SetStep(STEP_DO_READY_MOVE_JIG_START);
					}
				}
			}
			//if
		}
		break;

		case STEP_DO_READY_MOVE_JIG_END:
		{
			m_step.SetStep(STEP_DO_READY_CYL_JIG_OPEN);
		}
		break;

		/*######################################################
					STEP_DO_READY_CYL_JIG_OPEN
			######################################################*/
		case STEP_DO_READY_CYL_JIG_OPEN:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_READY_CYL_JIG_OPEN_START);
		}
		break;

		case STEP_DO_READY_CYL_JIG_OPEN_START:
		{
			CylOpen(AP_OBJ::CYL_PHONE_OPENCLOSE, true);
			//m_cfg.p_Cyl[AP_OBJ::CYL_PHONE_OPENCLOSE].Open(true);

			m_step.SetStep(STEP_DO_READY_CYL_JIG_OPEN_WAIT);
		}
		break;

		case STEP_DO_READY_CYL_JIG_OPEN_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_Cyl[AP_OBJ::CYL_PHONE_OPENCLOSE].IsOpen())
			{
				m_step.SetStep(STEP_DO_READY_CYL_JIG_OPEN_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_READY_CYL_JIG_OPEN_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_PHONE_open_timeout);
					head_inf.obj_id = AP_OBJ::CYL_PHONE_OPENCLOSE;
					head_inf.step_no = STEP_DO_READY_CYL_JIG_OPEN_WAIT;
					PROC_ALARM(&head_inf,"phone jig open timeout");
					m_step.SetStep(STEP_DO_READY_CYL_JIG_OPEN_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_READY_CYL_JIG_OPEN_END:
		{
			m_step.SetStep(STEP_TODO);
		}
		break;

		/*######################################################
					STEP_DO_STICKY_CYL_JIG_CLOSE
			######################################################*/
		case STEP_DO_STICKY_CYL_JIG_CLOSE:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_STICKY_CYL_JIG_CLOSE_START);
		}
		break;

		case STEP_DO_STICKY_CYL_JIG_CLOSE_START:
		{
			CylGrip(AP_OBJ::CYL_PHONE_OPENCLOSE, true);

			m_step.SetStep(STEP_DO_STICKY_CYL_JIG_CLOSE_WAIT);
		}
		break;

		case STEP_DO_STICKY_CYL_JIG_CLOSE_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsGrip(AP_OBJ::CYL_PHONE_OPENCLOSE))
			{
				m_step.SetStep(STEP_DO_STICKY_CYL_JIG_CLOSE_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_STICKY_CYL_JIG_CLOSE_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_PHONE_open_timeout);
					head_inf.obj_id = AP_OBJ::CYL_PHONE_OPENCLOSE;
					head_inf.step_no = STEP_DO_STICKY_CYL_JIG_CLOSE_WAIT;
					PROC_ALARM(&head_inf,"phone jig open timeout");
					m_step.SetStep(STEP_DO_STICKY_CYL_JIG_CLOSE_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_STICKY_CYL_JIG_CLOSE_END:
		{
			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_READY);
		}
		break;

		/*######################################################
					STEP_DO_STICKY_LINK_MOVE_READY
			######################################################*/
		case STEP_DO_STICKY_LINK_MOVE_READY:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_READY_START);
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_READY_START:
		{
			link_dat::dat_t pose_data = m_cfg.p_linkPosDat->ReadData(link_dat::sticky_ready);

			LinkMove(pose_data.cmd_pos, pose_data.cmd_vel);

			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_READY_WAIT);
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_READY_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_motors->IsLinkMoveCplt())
			{
				m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_READY_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_READY_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
					head_inf.obj_id = AP_OBJ::MOTOR_MAX;
					head_inf.step_no = STEP_DO_STICKY_LINK_MOVE_READY_WAIT;
					PROC_ALARM(&head_inf,"link move timeout");
					m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_READY_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_READY_END:
		{
			m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON);
		}
		break;

		/*######################################################
					STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON
			######################################################*/
		case STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_START);
		}
		break;

		case STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_START:
		{
			axis_dat::dat_t pose_data = m_cfg.p_apAxisDat->ReadData(static_cast<uint8_t>(mt_roll_pos_addr::sticky));
			MotorRun(AP_OBJ::MOTOR_HIGH, pose_data.cmd_pos, pose_data.cmd_vel);
			VacOn(AP_OBJ::VAC_DETECT, true);


			m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_WAIT);
		}
		break;

		case STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsMotorMoveCplt(AP_OBJ::MOTOR_HIGH))
			{
				m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_END);
				break;
			}
			else
			{
				if (IsMotorRun(AP_OBJ::MOTOR_HIGH))
				{
					break;
				}
				else
				{
					if (m_step.retry_cnt++ < RETRY_CNT_MAX)
					{
						m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_START);
						break;
					}
					else
					{
						// error
						m_step.retry_cnt = 0;
						log_dat::head_t head_inf;
						head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
						head_inf.obj_id = AP_OBJ::MOTOR_HIGH;
						head_inf.step_no = STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_WAIT;
						PROC_ALARM(&head_inf,"high axis stick pos move timeout");
						m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_START);
					}
				}
			}
			//if
		}
		break;

		case STEP_DO_STICKY_MOVE_HIGH_READY_VAC_ON_END:
		{
			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW);
		}
		break;

		/*######################################################
					STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW
			######################################################*/
		case STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_START);
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_START:
		{
			link_dat::dat_t pose_data = m_cfg.p_linkPosDat->ReadData(link_dat::roll_slow);

			LinkMove(pose_data.cmd_pos, pose_data.cmd_vel);

			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_WAIT);
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_motors->IsLinkMoveCplt())
			{
				m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
					head_inf.obj_id = AP_OBJ::MOTOR_MAX;
					head_inf.step_no = STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_WAIT;
					PROC_ALARM(&head_inf,"link move slow stick timeout");
					m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_END:
		{
			m_step.SetStep(STEP_DO_STICKY_CHECK_VAC_ON_VINYL);
		}
		break;

		/*######################################################
					STEP_DO_STICKY_CHECK_VAC_ON_VINYL
			######################################################*/
		case STEP_DO_STICKY_CHECK_VAC_ON_VINYL:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_STICKY_CHECK_VAC_ON_VINYL_START);
		}
		break;

		case STEP_DO_STICKY_CHECK_VAC_ON_VINYL_START:
		{

			m_step.SetStep(STEP_DO_STICKY_CHECK_VAC_ON_VINYL_WAIT);
		}
		break;

		case STEP_DO_STICKY_CHECK_VAC_ON_VINYL_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (1/*m_cfg.p_motors->IsLinkMoveCplt()*/)
			{
				m_step.SetStep(STEP_DO_STICKY_CHECK_VAC_ON_VINYL_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_STICKY_CHECK_VAC_ON_VINYL_START);
					break;
				}
				else
				{
					// error
					/*m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
					head_inf.obj_id = AP_OBJ::MOTOR_MAX;
					head_inf.step_no = STEP_DO_STICKY_LINK_MOVE_ROLL_SLOW_WAIT;
					PROC_ALARM(&head_inf,"link move slow stick timeout");
					m_step.SetStep(STEP_DO_STICKY_CHECK_VAC_ON_VINYL_START);*/
				}
			}
			//if
		}
		break;

		case STEP_DO_STICKY_CHECK_VAC_ON_VINYL_END:
		{
			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_FAST);
		}
		break;

		/*######################################################
					STEP_DO_STICKY_LINK_MOVE_ROLL_FAST
			######################################################*/
		case STEP_DO_STICKY_LINK_MOVE_ROLL_FAST:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_START);
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_START:
		{
			link_dat::dat_t pose_data = m_cfg.p_linkPosDat->ReadData(link_dat::roll_fast);

			LinkMove(pose_data.cmd_pos, pose_data.cmd_vel);

			m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_WAIT);
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_motors->IsLinkMoveCplt())
			{
				m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
					head_inf.obj_id = AP_OBJ::MOTOR_MAX;
					head_inf.step_no = STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_WAIT;
					PROC_ALARM(&head_inf,"link move fast stick timeout");
					m_step.SetStep(STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_STICKY_LINK_MOVE_ROLL_FAST_END:
		{
			m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_UP);
		}
		break;
		/*######################################################
					STEP_DO_STICKY_MOVE_HIGH_UP
			######################################################*/
		case STEP_DO_STICKY_MOVE_HIGH_UP:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_UP_START);
		}
		break;

		case STEP_DO_STICKY_MOVE_HIGH_UP_START:
		{
			axis_dat::dat_t pose_data = m_cfg.p_apAxisDat->ReadData(static_cast<uint8_t>(mt_high_pos_addr::up_sticky));

			MotorRun(AP_OBJ::MOTOR_HIGH, pose_data.cmd_pos, pose_data.cmd_vel);
			m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_UP_WAIT);
		}
		break;

		case STEP_DO_STICKY_MOVE_HIGH_UP_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsMotorMoveCplt(AP_OBJ::MOTOR_HIGH))
			{
				m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_UP_END);
				break;
			}
			else
			{

				if (IsMotorRun(AP_OBJ::MOTOR_HIGH))
				{
					break;
				}
				else
				{
					if (m_step.retry_cnt++ < RETRY_CNT_MAX)
					{
						m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_UP_START);
						break;
					}
					else
					{
						// error
						m_step.retry_cnt = 0;
						log_dat::head_t head_inf;
						head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
						head_inf.obj_id = AP_OBJ::MOTOR_HIGH;
						head_inf.step_no = STEP_DO_STICKY_MOVE_HIGH_UP_WAIT;
						PROC_ALARM(&head_inf,"ready_pos move up timeout");
						m_step.SetStep(STEP_DO_STICKY_MOVE_HIGH_UP_START);
					}
				}
			}
			//if
		}
		break;

		case STEP_DO_STICKY_MOVE_HIGH_UP_END:
		{
			m_step.SetStep(STEP_DO_RETRUN_MOVE_JIG_OPEN);
		}
		break;

		/*######################################################
					STEP_DO_RETRUN_MOVE_JIG_OPEN
			######################################################*/
		case STEP_DO_RETRUN_MOVE_JIG_OPEN:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_RETRUN_MOVE_JIG_OPEN_START);
		}
		break;

		case STEP_DO_RETRUN_MOVE_JIG_OPEN_START:
		{
			axis_dat::dat_t pose_data = m_cfg.p_apAxisDat->ReadData(static_cast<uint8_t>(mt_jig_pos_addr::ready));
			MotorRun(AP_OBJ::MOTOR_JIG, pose_data.cmd_pos, pose_data.cmd_vel);
			CylOpen(AP_OBJ::CYL_PHONE_OPENCLOSE, true);


			m_step.SetStep(STEP_DO_RETRUN_MOVE_JIG_OPEN_WAIT);
		}
		break;

		case STEP_DO_RETRUN_MOVE_JIG_OPEN_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsMotorMoveCplt(AP_OBJ::MOTOR_JIG))
			{
				m_step.SetStep(STEP_DO_RETRUN_MOVE_JIG_OPEN_END);
				break;
			}
			else
			{
				CylOpen(AP_OBJ::CYL_PHONE_OPENCLOSE, true);

				if (IsMotorRun(AP_OBJ::MOTOR_JIG))
				{
					break;
				}
				else
				{
					if (m_step.retry_cnt++ < RETRY_CNT_MAX)
					{
						m_step.SetStep(STEP_DO_RETRUN_MOVE_JIG_OPEN_START);
						break;
					}
					else
					{
						// error
						m_step.retry_cnt = 0;
						log_dat::head_t head_inf;
						head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
						head_inf.obj_id = AP_OBJ::MOTOR_JIG;
						head_inf.step_no = STEP_DO_RETRUN_MOVE_JIG_OPEN_WAIT;
						PROC_ALARM(&head_inf,"ready_pos move return open timeout");
						m_step.SetStep(STEP_DO_RETRUN_MOVE_JIG_OPEN_START);
					}
				}
			}
			//if
		}
		break;

		case STEP_DO_RETRUN_MOVE_JIG_OPEN_END:
		{
			m_step.SetStep(STEP_DO_TRASH_MOVE_ROLL);
		}
		break;
		/*######################################################
					STEP_DO_TRASH_MOVE_ROLL
			######################################################*/
		case STEP_DO_TRASH_MOVE_ROLL:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_TRASH_MOVE_ROLL_START);
		}
		break;

		case STEP_DO_TRASH_MOVE_ROLL_START:
		{
			axis_dat::dat_t pose_data = m_cfg.p_apAxisDat->ReadData(static_cast<uint8_t>(mt_roll_pos_addr::trash));
			MotorRun(AP_OBJ::MOTOR_ROLL, pose_data.cmd_pos, pose_data.cmd_vel);


			m_step.SetStep(STEP_DO_TRASH_MOVE_ROLL_WAIT);
		}
		break;

		case STEP_DO_TRASH_MOVE_ROLL_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (IsMotorMoveCplt(AP_OBJ::MOTOR_ROLL))
			{
				m_step.SetStep(STEP_DO_TRASH_MOVE_ROLL_END);
				break;
			}
			else
			{
				if (IsMotorRun(AP_OBJ::MOTOR_ROLL))
				{
					break;
				}
				else
				{
					if (m_step.retry_cnt++ < RETRY_CNT_MAX)
					{
						m_step.SetStep(STEP_DO_TRASH_MOVE_ROLL_START);
						break;
					}
					else
					{
						// error
						m_step.retry_cnt = 0;
						log_dat::head_t head_inf;
						head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
						head_inf.obj_id = AP_OBJ::MOTOR_ROLL;
						head_inf.step_no = STEP_DO_TRASH_MOVE_ROLL_WAIT;
						PROC_ALARM(&head_inf,"ready_pos move timeout");
						m_step.SetStep(STEP_DO_TRASH_MOVE_ROLL_START);
					}
				}
			}
			//if
		}
		break;

		case STEP_DO_TRASH_MOVE_ROLL_END:
		{
			m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_UP);
		}
		break;

		/*######################################################
					STEP_DO_TRASH_REMOVE_CYL_UP
			######################################################*/
		case STEP_DO_TRASH_REMOVE_CYL_UP:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_UP_START);
		}
		break;

		case STEP_DO_TRASH_REMOVE_CYL_UP_START:
		{
			CylOpen(AP_OBJ::CYL_VINYLHOLD_UPDOWN, true);

			m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_UP_WAIT);
		}
		break;

		case STEP_DO_TRASH_REMOVE_CYL_UP_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_Cyl[AP_OBJ::CYL_VINYLHOLD_UPDOWN].IsUp())
			{
				m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_UP_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_UP_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_PHONE_open_timeout);
					head_inf.obj_id = AP_OBJ::CYL_VINYLHOLD_UPDOWN;
					head_inf.step_no = STEP_DO_TRASH_REMOVE_CYL_UP_WAIT;
					PROC_ALARM(&head_inf,"phone hold cylinder up timeout");
					m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_UP_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_TRASH_REMOVE_CYL_UP_END:
		{
			m_step.SetStep(STEP_DO_TRASH_CHECK_VINYL);
		}
		break;

		/*######################################################
					STEP_DO_TRASH_CHECK_VINYL
			######################################################*/
		case STEP_DO_TRASH_CHECK_VINYL:
		{
			m_step.retry_cnt = 0;
			if(m_cfg.p_apIo->IsOn(MCU_IO::ap_io::in_grip_vinyle_detect))
				m_step.SetStep(STEP_DO_TRASH_CHECK_VINYL_START);
			else
				m_step.SetStep(STEP_DO_TRASH_CHECK_VINYL_END);
		}
		break;

		case STEP_DO_TRASH_CHECK_VINYL_START:
		{
			m_step.SetStep(STEP_DO_TRASH_CHECK_VINYL_WAIT);
		}
		break;

		case STEP_DO_TRASH_CHECK_VINYL_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_apIo->IsOn(MCU_IO::ap_io::in_grip_vinyle_detect))
			{
				m_step.SetStep(STEP_DO_TRASH_CHECK_VINYL_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_TRASH_CHECK_VINYL_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::vinyl_detect_timeout);
					head_inf.obj_id = 0;
					head_inf.step_no = STEP_DO_TRASH_CHECK_VINYL_WAIT;
					PROC_ALARM(&head_inf,"vinyl detect timeout");
					m_step.SetStep(STEP_DO_TRASH_CHECK_VINYL_START);
				}
			}
			//if (m_cfg.p_motors->IsInPose
		}
		break;

		case STEP_DO_TRASH_CHECK_VINYL_END:
		{
			m_step.SetStep(STEP_DO_TRASH_LINK_MOVE_CLEAR);
		}
		break;

		/*######################################################
					STEP_DO_TRASH_LINK_MOVE_CLEAR
			######################################################*/
		case STEP_DO_TRASH_LINK_MOVE_CLEAR:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_TRASH_LINK_MOVE_CLEAR_START);
		}
		break;

		case STEP_DO_TRASH_LINK_MOVE_CLEAR_START:
		{
			link_dat::dat_t pose_data = m_cfg.p_linkPosDat->ReadData(link_dat::roll_clear);

			LinkMove(pose_data.cmd_pos, pose_data.cmd_vel);

			m_step.SetStep(STEP_DO_TRASH_LINK_MOVE_CLEAR_WAIT);
		}
		break;

		case STEP_DO_TRASH_LINK_MOVE_CLEAR_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_motors->IsLinkMoveCplt())
			{
				m_step.SetStep(STEP_DO_TRASH_LINK_MOVE_CLEAR_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_TRASH_LINK_MOVE_CLEAR_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
					head_inf.obj_id = AP_OBJ::MOTOR_MAX;
					head_inf.step_no = STEP_DO_TRASH_LINK_MOVE_CLEAR_WAIT;
					PROC_ALARM(&head_inf,"link move fast stick timeout");
					m_step.SetStep(STEP_DO_TRASH_LINK_MOVE_CLEAR_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_TRASH_LINK_MOVE_CLEAR_END:
		{
			m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_DOWN);
		}
		break;
		/*######################################################
					STEP_DO_TRASH_REMOVE_CYL_DOWN
			######################################################*/
		case STEP_DO_TRASH_REMOVE_CYL_DOWN:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_DOWN_START);
		}
		break;

		case STEP_DO_TRASH_REMOVE_CYL_DOWN_START:
		{
			CylClose(AP_OBJ::CYL_VINYLHOLD_UPDOWN, true);

			m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_DOWN_WAIT);
		}
		break;

		case STEP_DO_TRASH_REMOVE_CYL_DOWN_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_Cyl[AP_OBJ::CYL_VINYLHOLD_UPDOWN].IsDown())
			{
				m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_DOWN_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_DOWN_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_PHONE_open_timeout);
					head_inf.obj_id = AP_OBJ::CYL_VINYLHOLD_UPDOWN;
					head_inf.step_no = STEP_DO_TRASH_REMOVE_CYL_DOWN_WAIT;
					PROC_ALARM(&head_inf,"phone hold cylinder up timeout");
					m_step.SetStep(STEP_DO_TRASH_REMOVE_CYL_DOWN_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_TRASH_REMOVE_CYL_DOWN_END:
		{
			m_step.SetStep(STEP_DO_TRASH_PUSH_CYL_PUSH);
		}
		break;

		/*######################################################
					STEP_DO_TRASH_PUSH_CYL_PUSH
			######################################################*/
		case STEP_DO_TRASH_PUSH_CYL_PUSH:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_TRASH_PUSH_CYL_PUSH_START);
		}
		break;

		case STEP_DO_TRASH_PUSH_CYL_PUSH_START:
		{
			CylOpen(AP_OBJ::CYL_VINYL_PUSHBACK, true);

			m_step.SetStep(STEP_DO_TRASH_PUSH_CYL_PUSH_WAIT);
		}
		break;

		case STEP_DO_TRASH_PUSH_CYL_PUSH_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_Cyl[AP_OBJ::CYL_VINYL_PUSHBACK].IsForward())
			{
				m_step.SetStep(STEP_DO_TRASH_PUSH_CYL_PUSH_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_TRASH_PUSH_CYL_PUSH_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_PHONE_open_timeout);
					head_inf.obj_id = AP_OBJ::CYL_VINYL_PUSHBACK;
					head_inf.step_no = STEP_DO_TRASH_PUSH_CYL_PUSH_WAIT;
					PROC_ALARM(&head_inf,"vinyl push cylinder push timeout");
					m_step.SetStep(STEP_DO_TRASH_PUSH_CYL_PUSH_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_TRASH_PUSH_CYL_PUSH_END:
		{
			m_step.SetStep(STEP_DO_PEEL_VINYL_FINISH);
		}
		break;

		/*######################################################
					STEP_DO_PEEL_VINYL_FINISH
			######################################################*/
		case STEP_DO_PEEL_VINYL_FINISH:
		{
			m_step.retry_cnt = 0;
			m_step.SetStep(STEP_DO_PEEL_VINYL_FINISH_START);
		}
		break;

		case STEP_DO_PEEL_VINYL_FINISH_START:
		{
			CylClose(AP_OBJ::CYL_VINYL_PUSHBACK, true);

			m_step.SetStep(STEP_DO_PEEL_VINYL_FINISH_WAIT);
		}
		break;

		case STEP_DO_PEEL_VINYL_FINISH_WAIT:
		{
			if (m_step.LessThan(STEP_DELAY_WAIT))             {	break; }

			if (m_cfg.p_Cyl[AP_OBJ::CYL_VINYL_PUSHBACK].IsBackward())
			{
				m_step.SetStep(STEP_DO_PEEL_VINYL_FINISH_END);
				break;
			}
			else
			{
				if (m_step.retry_cnt++ < RETRY_CNT_MAX)
				{
					m_step.SetStep(STEP_DO_PEEL_VINYL_FINISH_START);
					break;
				}
				else
				{
					// error
					m_step.retry_cnt = 0;
					log_dat::head_t head_inf;
					head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_PHONE_open_timeout);
					head_inf.obj_id = AP_OBJ::CYL_VINYL_PUSHBACK;
					head_inf.step_no = STEP_DO_PEEL_VINYL_FINISH_WAIT;
					PROC_ALARM(&head_inf,"peel vinyl off finish timeout");
					m_step.SetStep(STEP_DO_PEEL_VINYL_FINISH_START);
				}
			}
			//if
		}
		break;

		case STEP_DO_PEEL_VINYL_FINISH_END:
		{
			m_step.SetStep(STEP_DO_READY_MOVE_HIGH);
		}
		break;
		/////////////////////////////////////////////////////////////////////
		default:
			break;
	}
	//switch(m_step.GetStep())
}






errno_t cnTasks::DoTask_JobReady(){
	if(m_step.Available()) { m_step.SetStep(STEP_DO_READY_MOVE_HIGH); return 0; };
	return -1;
}

errno_t cnTasks::DoTask_loaingPhone(){
	return 0;
}

errno_t cnTasks::DoTask_StickyVinyl(){
	return 0;
}

errno_t cnTasks::DoTask_RemoveRolling(){
	return 0;
}

errno_t cnTasks::DoTask_ReturnPhoneMoveToTrash(){
	return 0;
}

errno_t cnTasks::DoTask_CleanVinyl(){
	return 0;
}
