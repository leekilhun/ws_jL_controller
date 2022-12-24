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




void api_lcd::ThreadJob()
{

	doRunStep();
	m_cfg.ptr_comm->ReceiveProcess();

}

void api_lcd::doRunStep()
{

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

		}
		break;
		/*######################################################
		  STEP_MAIN_UPDATE
		######################################################*/
		case STEP_MAIN_UPDATE:
		{
			//SendApReg(false);
			m_waitReplyOK = true;
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


	if (m_waitReplyOK)
	{
		if(m_receiveData.type != TYPE::RPY_BKCMD_OK)
		{
			m_OkReply = true;
		}
		return;
	}

	switch (m_receiveData.type)
	{
		case TYPE::REQ_BEEP :
		{

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




