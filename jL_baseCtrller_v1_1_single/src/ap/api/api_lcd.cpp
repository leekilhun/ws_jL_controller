/*
 * api_remote.cpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */


#include "ap.h"
#include "api_lcd.hpp"

constexpr uint8_t STEP_INIT 							= 0x00;
constexpr uint8_t	STEP_TODO 							= 0x01;
constexpr uint8_t	STEP_TIMEOUT						= 0x02;
constexpr uint8_t	STEP_WAIT_RETURN				= 0x03;
constexpr uint8_t	STEP_MAIN_UPDATE				= 0x04;
constexpr uint8_t	STEP_MAIN_UPDATE_START	= 0x05;
constexpr uint8_t	STEP_MAIN_UPDATE_WAIT 	= 0x06;
constexpr uint8_t	STEP_MAIN_UPDATE_END 	  = 0x07;
constexpr uint8_t	STEP_IO_UPDATE					= 0x08;
constexpr uint8_t	STEP_IO_UPDATE_START		= 0x09;
constexpr uint8_t	STEP_IO_UPDATE_WAIT 		= 0x0a;
constexpr uint8_t	STEP_IO_UPDATE_END 	  	= 0x0b;



constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_READY 			= (1 << 0);
constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_START 			= (1 << 1);
constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_STOP 			= (1 << 2);
constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_RESET 			= (1 << 3);
constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_RESERVED4 	= (1 << 4);
constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_RESERVED5 	= (1 << 5);
constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_RESERVED6 	= (1 << 6);
constexpr uint8_t RXCMD_LCD_SW_KEY_EVENT_RESERVED7 	= (1 << 7);


void api_lcd::ThreadJob()
{

	doRunStep();
	m_cfg.ptr_comm->ReceiveProcess();

}

void api_lcd::doRunStep()
{
	if(m_step.MoreThan(200))
	{
		if (m_waitReplyOK == false)
		{
			m_waitReplyOK = true;
			m_cfg.ptr_comm->SendCmd(NXLCD::TXCMD::LCD_REQUSET_PAGE,nullptr, 0);
			m_step.SetStep(STEP_WAIT_RETURN);
		}
	}


	switch(m_step.GetStep())
	{
		case STEP_INIT:
		{

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
			m_step.wait_resp = true;
			m_waitReplyOK = false;
			m_step.SetStep(STEP_TODO);
		}
		break;
		/*######################################################
		  wait return
		######################################################*/
		case STEP_WAIT_RETURN:
		{
			if (m_step.LessThan(50))
				break;

			if (m_waitReplyOK)
				m_step.SetStep(STEP_TIMEOUT);
			else
				m_step.SetStep(STEP_TODO);
		}
		break;
		/*######################################################
		  STEP_MAIN_UPDATE
		######################################################*/
		case STEP_MAIN_UPDATE:
		{
			//SendApReg(false);
			m_waitReplyOK = true;
			m_step.wait_resp = true;
			uint8_t ser_reparse = 1;
			m_cfg.ptr_comm->SendCmd(NXLCD::TXCMD::LCD_START_REPARSEMODE
															, &ser_reparse, 1);
			m_step.SetStep(STEP_MAIN_UPDATE_START);
		}
		break;

		case STEP_MAIN_UPDATE_START:
		{
			if (m_step.LessThan(50))
				break;
			SendApReg(false, false);
			m_OkReply = false;
			m_step.SetStep(STEP_MAIN_UPDATE_WAIT);

		}
		break;
		case STEP_MAIN_UPDATE_WAIT:
		{
			if (m_OkReply == false)
				break;

			m_step.SetStep(STEP_MAIN_UPDATE_END);

		}
		break;
		case STEP_MAIN_UPDATE_END:
		{
			m_waitReplyOK = false;
			m_cfg.ptr_comm->SendCmd(NXLCD::TXCMD::LCD_END_REPARSEMODE
															, nullptr, 0);

			m_step.SetStep(STEP_TODO);
		}
		break;
		/*######################################################
		  STEP_IO_UPDATE
		######################################################*/
		case STEP_IO_UPDATE:
		{
			//SendApReg(false);
			m_waitReplyOK = true;
			uint8_t ser_reparse = 1;
			m_cfg.ptr_comm->SendCmd(NXLCD::TXCMD::LCD_START_REPARSEMODE
															, &ser_reparse, 1);
			m_step.SetStep(STEP_IO_UPDATE_START);
		}
		break;

		case STEP_IO_UPDATE_START:
		{
			if (m_step.LessThan(50))
				break;
			SendApIoReg(false, false);
			m_OkReply = false;
			m_step.SetStep(STEP_IO_UPDATE_WAIT);

		}
		break;
		case STEP_IO_UPDATE_WAIT:
		{
			if (m_OkReply == false)
				break;

			m_step.SetStep(STEP_IO_UPDATE_END);

		}
		break;
		case STEP_IO_UPDATE_END:
		{
			m_waitReplyOK = false;
			m_cfg.ptr_comm->SendCmd(NXLCD::TXCMD::LCD_END_REPARSEMODE
															, nullptr, 0);

			m_step.SetStep(STEP_TODO);
		}
		break;


		default:
			break;
	}
	// end of switch

}


void api_lcd::ProcessCmd(NXLCD::uart_nextion::rx_packet_t* ptr_data){
	m_receiveData = ptr_data;
	using TYPE = NXLCD::RXCMD;

	m_step.wait_resp = false;

	if (m_waitReplyOK)
	{
		m_waitReplyOK = false;
		//if(m_receiveData.type != TYPE::RPY_BKCMD_OK)
		{
			m_OkReply = true;
		}
		return;
	}

	switch (m_receiveData.type)
	{
		case TYPE::REQ_BEEP :
		{
			if (m_cfg.ptr_mcu_reg->option_reg.use_beep)
			{
				buzzerBeep(1, 2);
			}
		}
		break;

		case TYPE::WRITE_OUT_REG:
		{

		}
		break;

		case TYPE::CURR_PAGE_NO:
		{
			m_currPage = m_receiveData.page_no;
			LcdUpdate();
		}
		break;

		case TYPE::UPATE_MCU_REG:
		{
			switch (m_receiveData.data[0])
			{
				case RXCMD_LCD_SW_KEY_EVENT_READY:
					m_cfg.ptr_auto->UiStartSw();
					break;

				case RXCMD_LCD_SW_KEY_EVENT_START:
					m_cfg.ptr_auto->UiStartSw();
					break;

				case RXCMD_LCD_SW_KEY_EVENT_STOP:
					m_cfg.ptr_auto->StopSw();
					//m_pApReg->SetRunState(AP_REG_AUTO_RUNNING, false);
					//m_pAuto->StopSw();
					break;

				case RXCMD_LCD_SW_KEY_EVENT_RESET:
					//m_pAuto->ResetSw();
					break;

				default:
					break;
			}

		}
		break;

		case TYPE::CTRL_MOTOR:
		{

		}
		break;

		case TYPE::SAVE_MOTOR_DATA:
		{

		}
		break;

		case TYPE::REQ_MOTOR_DATA:
		{

		}
		break;

		case TYPE::SAVE_AP_CFG:
		{

		}
		break;

		case TYPE::REQ_AP_CFG:
		{

		}
		break;

		case TYPE::SAVE_CYL_DATA:
		{

		}
		break;

		case TYPE::REQ_CYL_DATA:
		{

		}
		break;

		case TYPE::SAVE_VAC_DATA:
		{

		}
		break;

		case TYPE::REQ_VAC_DATA:
		{

		}
		break;
		case TYPE::SAVE_SEQ_DATA:
		{

		}
		break;
		case TYPE::REQ_SEQ_DATA:
		{

		}
		break;
		case TYPE::REQ_SEQ_INIT:
		{

		}
		break;
		case TYPE::CTRL_CYL:
		{

		}
		break;
		case TYPE::CTRL_VAC:
		{

		}
		break;

		case TYPE::RESP_BKCMD:
		{

		}
		break;


		default:
			break;
	}// switch (m_receiveData.type)

}




// mcu 상태 정보를 보낸다
void api_lcd::LcdUpdate(){
	using PAGE = NXLCD::uart_nextion::page_e;

	if (m_cfg.ptr_mcu_reg->IsAutoReadyMode())
	{
		if (m_currPage !=PAGE::MAIN)
		{
			ChangePage(PAGE::MAIN);
		}
	}

	switch (m_currPage)
	{
		case PAGE::INIT:
			if(SendDataAll() != ERROR_SUCCESS)
			{

			}
			break;

		case PAGE::MAIN:
			m_step.SetStep(STEP_MAIN_UPDATE);
			break;

		case PAGE::IO:
			m_step.SetStep(STEP_IO_UPDATE);
			break;

		case PAGE::MANUAL:
			//SendAxisIoData(); m_step.SetStep(STEP_MAIN_UPDATE);
			break;

		case PAGE::TEACH:
			//SendAxisIoData();
			break;

		case PAGE::DATA:
			//SendDataAll();
			break;

		case PAGE::LOG:
		{
			//  if(m_pLog->Available())
			//  {
			//    SendLogData();
			//  }
		}
		break;
		default:
			break;
	}

	//m_IsRespCplt = false;


}




