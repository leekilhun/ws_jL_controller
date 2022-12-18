/*
 * api_lcd.hpp
 *
 *  Created on: 2022. 12. 13.
 *      Author: gns2.lee
 */

#ifndef SRC_AP__INC_API_LCD_HPP_
#define SRC_AP__INC_API_LCD_HPP_



#define UI_LCD_MAX_BUFFER_LENGTH                     (40+1)

class api_lcd
{
	/****************************************************
	 *  data
	 ****************************************************/
public:
	struct cfg_t
	{
		NXLCD::uart_nextion* ptr_comm{};
		cnTasks* ptr_task{};

		cfg_t() = default;

		cfg_t& operator = (const cfg_t& cfg){
			if (this != &cfg){
				ptr_comm = cfg.ptr_comm;
				ptr_task = cfg.ptr_task;
			}
			return *(this);
		}

	};


private:
	bool m_IsInit;
	cfg_t m_cfg;
	NXLCD::uart_nextion::rx_packet_t m_receiveData;
	NXLCD::uart_nextion::page_e m_currPage;


public:

	/****************************************************
	 *  Constructor
	 ****************************************************/
public:
	api_lcd(): m_IsInit{}, m_cfg{},m_receiveData{}
	,m_currPage{}
	{

	};

	virtual ~api_lcd(){};

	/****************************************************
	 *  func
	 ****************************************************/
private:
	void doRunStep();

	void retGetValue();

	// callback function
	static void receiveDataFunc(void* obj, void* w_parm, void* l_parm) {
		api_lcd* ptr_this = (api_lcd*)obj;
		if (w_parm == nullptr && obj == nullptr && l_parm == nullptr)
			return;
		//uint8_t instance_no = *((uint8_t*)w_parm);
		ptr_this->ProcessCmd((NXLCD::uart_nextion::rx_packet_t*)l_parm) ;
	}



public:
	inline int Init(api_lcd::cfg_t &cfg){
		m_cfg = cfg;
		cfg.ptr_comm->AttCallbackFunc(this, receiveDataFunc);
		m_IsInit = true;
		return ERROR_SUCCESS;
	}


	// 통신 연결을 재 시도한다
	inline void CommRecovery(){
		constexpr int recovery_limit = 10;
		if (m_cfg.ptr_comm->GetErrCnt() < recovery_limit)
			return;
		m_cfg.ptr_comm->Recovery();
	}

	// step machine을 통해 nonblock으로 처리된다.
	void ThreadJob();

	// PC에서 Command 수신 처리, 수신 ack 신호를 리턴한다
	void ProcessCmd(NXLCD::uart_nextion::rx_packet_t* ptr_data);

	errcode ChangePage(NXLCD::uart_nextion::page_e page)
	{
		int go_page = 0;
	  bool result = m_cfg.ptr_comm->SendCmdRxResp(NXLCD::TXCMD::LCD_CHANGE_PAGE
																	,(uint8_t *)&go_page
																	,1
																	,100);
	  if (result)
	  	return ERROR_SUCCESS;
	  CommRecovery();
	  return ERR_API_LCD_COMM_TIMEOUT;
	}



	// mcu 상태 정보를 보낸다
	inline void LcdUpdate(){
		using PAGE = NXLCD::uart_nextion::page_e;

//		if (m_pApReg->IsAutoReadyMode() || m_pApReg->IsAutoRunning())
//		{
//			if (m_currPage != PAGE::MAIN)
//			{
				 ChangePage(PAGE::MAIN);
//			}
//		}

		switch (m_currPage)
		{
			case PAGE::INIT:
				//SendDataAll();
				break;

			case PAGE::MAIN:
				//SendApReg();
				break;

			case PAGE::IO:
				//SendApIoReg();
				break;

			case PAGE::MANUAL:
				//SendAxisIoData();
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


};



#endif /* SRC_AP__INC_API_LCD_HPP_ */
