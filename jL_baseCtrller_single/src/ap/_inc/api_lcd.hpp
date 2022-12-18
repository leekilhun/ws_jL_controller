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
	enum ret_e
	{
		cplt = 0x00,
		wait,
		timeout,
	};

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
public:

	/****************************************************
	 *  Constructor
	 ****************************************************/
public:
	api_lcd(): m_IsInit{}, m_cfg{},m_receiveData{}{

	};

	virtual ~api_lcd(){};

	/****************************************************
	 *  func
	 ****************************************************/
private:
	void doRunStep();
	void response(api_lcd::ret_e ret_value = ret_e::cplt);
	void retGetValue();

	//eeprom 데이터를 pc로 보냄
	api_lcd::ret_e sendData(uint8_t type);

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

	// step machine을 통해 nonblock으로 처리된다.
	void ThreadJob();

	// PC에서 Command 수신 처리, 수신 ack 신호를 리턴한다
	void ProcessCmd(NXLCD::uart_nextion::rx_packet_t* ptr_data);

	// mcu 상태 정보를 보낸다
	void PcUpdate();

	// 통신 연결을 재 시도한다
	void CommRecovery();

};



#endif /* SRC_AP__INC_API_LCD_HPP_ */
