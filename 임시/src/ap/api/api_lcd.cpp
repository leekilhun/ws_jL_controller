/*
 * api_remote.cpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */


#include "ap.h"
#include "api_lcd.hpp"


void api_lcd::ProcessCmd(NXLCD::uart_nextion::rx_packet_t* ptr_data){
	m_receiveData = ptr_data;
	using TYPE = NXLCD::RXCMD;
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




