/*
 * cnMotors.cpp
 *
 *  Created on: 2022. 12. 23.
 *      Author: gns2l
 */


#include "ap.h"
#include "cnMotors.hpp"


constexpr uint8_t STEP_INIT 							= 0x00;
constexpr uint8_t	STEP_TODO 							= 0x01;
constexpr uint8_t	STEP_TIMEOUT						= 0x02;
constexpr uint8_t	STEP_RESET_COMM_ALARM		= 0x03;
constexpr uint8_t	STEP_STATE_UPDATE				= 0x04;
constexpr uint8_t	STEP_STATE_UPDATE_START	= 0x05;
constexpr uint8_t	STEP_STATE_UPDATE_WAIT 	= 0x06;
constexpr uint8_t	STEP_STATE_UPDATE_END 	= 0x07;

constexpr uint8_t	COMM_TIMEOUT_MAX 				= 100;
constexpr uint8_t COMM_ALARM_CNT_MAX      = 2;
namespace MOTOR
{

	void cnMotors::UpdateMotorsState()
	{
		m_step.SetStep(STEP_STATE_UPDATE);
	}

	void cnMotors::ThreadJob()
	{
		doRunStep();

#if 0
		constexpr int wait_response_timeout = 100;
		if (m_cfg.p_comm->IsAvailableComm())
		{
			if (m_cfg.p_motor[1].GetMotorData()== ERROR_SUCCESS)
			{
				++m_requestMotor_idx;
				m_request_ms = millis();
			}
		}
		else
		{
			if (millis() - m_request_ms > wait_response_timeout)
			{
				m_cfg.p_comm->Recovery();
			}

		}
#endif
		/*
		if (m_cfg.p_comm->IsAvailableComm())
		{
			// check response time
			if (millis() - m_request_ms < wait_response_timeout)
			{
				// status bit recovery
				m_commStatus.comm_err &= ~(1 << (m_requestMotor_idx-1));
			}

			// idx limit
			m_requestMotor_idx = m_requestMotor_idx % AP_OBJ::MOTOR_MAX;
			if (m_cfg.p_motor[m_requestMotor_idx].GetMotorData()== ERROR_SUCCESS)
			{
				++m_requestMotor_idx;
				m_request_ms = millis();
			}
		}
		else
		{
			if (millis() - m_request_ms > wait_response_timeout)
			{
				m_commStatus.comm_err |= (1 << (m_requestMotor_idx-1));
				m_cfg.p_comm->Recovery();
			}
		}

		if (m_commStatus.idx_0_comm_err
				&& m_commStatus.idx_1_comm_err
				&& m_commStatus.idx_2_comm_err)
		{
			Recovery();
		}

	  */

		m_cfg.p_comm->ReceiveProcess();

	}

	void cnMotors::doRunStep()
	{
		if(m_step.MoreThan(200))
			UpdateMotorsState();


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
				m_step.SetStep(STEP_STATE_UPDATE);
			}
			break;
			/*######################################################
			  timeout
			######################################################*/
			case STEP_TIMEOUT:
			{
				m_commStatus.comm_err |= (1 << (m_requestMotor_idx));
				m_cfg.p_motor[m_requestMotor_idx].AddErrCnt();

				if (m_commStatus.idx_0_comm_err
						&& m_commStatus.idx_1_comm_err
						&& m_commStatus.idx_2_comm_err)
				{
					Recovery();
				}
				++m_requestMotor_idx;

				m_step.SetStep(STEP_TODO);
			}
			break;
			/*######################################################
			  comm alarm reset
			######################################################*/
			case STEP_RESET_COMM_ALARM:
			{
				MOTOR::enMotor_moons* ptr_motor = &m_cfg.p_motor[m_requestMotor_idx];
				uint8_t* alarm_cnt = &m_motorCommErrCnt[m_requestMotor_idx];
				m_requestMotor_idx++;

				if (*alarm_cnt < COMM_ALARM_CNT_MAX)
				{
					m_step.SetStep(STEP_TODO);
					break;
				}
				else
				{
					*alarm_cnt = 0;
					ptr_motor->ClearState();
				}
				m_step.SetStep(STEP_TODO);
			}
			break;
			/*######################################################
			  STEP_STATE_UPDATE
			######################################################*/
			case STEP_STATE_UPDATE:
			{
				m_requestMotor_idx = m_requestMotor_idx % AP_OBJ::MOTOR_MAX;

				m_step.SetStep(STEP_STATE_UPDATE_START);
			}
			break;

			case STEP_STATE_UPDATE_START:
			{
				if (m_cfg.p_comm->IsAvailableComm() == false)
				{
					if (m_step.LessThan(COMM_TIMEOUT_MAX))
						break;

					m_step.SetStep(STEP_TIMEOUT);
					break;
				}

				if (m_cfg.p_motor[m_requestMotor_idx].GetMotorData()== ERROR_SUCCESS)
					m_step.SetStep(STEP_STATE_UPDATE_WAIT);
				else
					m_step.SetStep(STEP_TIMEOUT);
			}
			break;

			case STEP_STATE_UPDATE_WAIT:
			{
				if (m_cfg.p_comm->IsAvailableComm() == false)
				{
					if (m_step.LessThan(COMM_TIMEOUT_MAX))
						break;

					m_step.SetStep(STEP_TIMEOUT);
					break;
				}

				m_step.SetStep(STEP_STATE_UPDATE_END);
			}
			break;

			case STEP_STATE_UPDATE_END:
			{
				if(m_step.LessThan(50))
					break;

				m_commStatus.comm_err &= ~(1 << (m_requestMotor_idx));

				if (m_cfg.p_motor[m_requestMotor_idx].IsCommAlarm())
				{
					m_motorCommErrCnt[m_requestMotor_idx]++;
					m_step.SetStep(STEP_RESET_COMM_ALARM);
					break;
				}

				++m_requestMotor_idx;
				m_step.SetStep(STEP_TODO);
			}
			break;


			default:
				break;
		}
		// end of switch

	}


}
//end of namespace
