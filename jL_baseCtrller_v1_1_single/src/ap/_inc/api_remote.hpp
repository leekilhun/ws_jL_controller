/*
 * api_remote.hpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_API_REMOTE_HPP_
#define SRC_AP__INC_API_REMOTE_HPP_


#define UI_RCTRL_MAX_BUFFER_LENGTH                     (64)

class api_remote
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
		RCTRL::uart_remote* ptr_comm{};
		MCU_REG::ap_reg* ptr_mcu_reg{};
		cnTasks* ptr_task{};
		MCU_IO::ap_io* ptr_io{};
		ap_dat* ptr_cfg_data{};
		axis_dat* ptr_axis_data{};
		cyl_dat* ptr_cyl_data{};
		vac_dat* ptr_vac_data{};
		seq_dat* ptr_sequence_data{};
		link_dat* ptr_linkPose_data{};
		ap_log* ptr_log{};
		cnAuto* ptr_auto{};
		MOTOR::cnMotors* ptr_motors{};

		cfg_t() = default;

		cfg_t& operator = (const cfg_t& cfg){
			ptr_mcu_reg 					= cfg.ptr_mcu_reg;
			ptr_comm 							= cfg.ptr_comm;
			ptr_task 							= cfg.ptr_task;
			ptr_io 								= cfg.ptr_io;
			ptr_cfg_data 					= cfg.ptr_cfg_data;
			ptr_cyl_data 					= cfg.ptr_cyl_data;
			ptr_vac_data 					= cfg.ptr_vac_data;
			ptr_sequence_data 		= cfg.ptr_sequence_data;
			ptr_linkPose_data     = cfg.ptr_linkPose_data;
			ptr_axis_data 				= cfg.ptr_axis_data;
			ptr_log 							= cfg.ptr_log;
			ptr_auto 							= cfg.ptr_auto;
			ptr_motors 						= cfg.ptr_motors;
			return *(this);
		}


	};


private:
	bool m_IsInit;
	cfg_t m_cfg;
	RCTRL::uart_remote::rx_packet_t m_receiveData;
	//uint8_t m_txBuffer[UI_RCTRL_MAX_BUFFER_LENGTH];
	std::array<uint8_t, UI_RCTRL_MAX_BUFFER_LENGTH> m_txBuffer;
	AP_OBJ::MOTOR m_idxMotor;
	bool m_waitReplyOK;
	bool m_OkReply;
public:
	prc_step_t m_step;
  uint32_t m_elaps;
	/****************************************************
	 *  Constructor
	 ****************************************************/
public:
	api_remote(): m_IsInit{}, m_cfg{},m_receiveData{}, m_txBuffer{}, m_idxMotor{}
	, m_waitReplyOK{}, m_OkReply{}
	, m_step{}, m_elaps{}{

	};

	virtual ~api_remote(){};

	/****************************************************
	 *  func
	 ****************************************************/
private:
	void doRunStep();
	inline void ok_Response(){
		m_cfg.ptr_comm->SendData((uint8_t)RCTRL::TX_TYPE::TX_OK_RESPONSE, nullptr, 0);

	}
	//void retGetValue();


	// callback function
	inline static void receiveDataFunc(void* obj, void* w_parm, void* l_parm) {
		api_remote* ptr_this = (api_remote*)obj;
		if (w_parm == nullptr && obj == nullptr && l_parm == nullptr)
			return;
		//uint8_t instance_no = *((uint8_t*)w_parm);
		ptr_this->ProcessCmd((RCTRL::uart_remote::rx_packet_t*)l_parm) ;
	}



public:
	inline int Init(api_remote::cfg_t &cfg){
		m_cfg = cfg;
		cfg.ptr_comm->AttCallbackFunc(this, receiveDataFunc);
		m_IsInit = true;
		return ERROR_SUCCESS;
	}

	// step machine??? ?????? nonblock?????? ????????????.
	void ThreadJob();

	// PC?????? Command ?????? ??????, ?????? ack ????????? ????????????
	void ProcessCmd(RCTRL::uart_remote::rx_packet_t* ptr_data);

	// mcu ?????? ????????? ?????????
	void PcUpdate();

	// ?????? ????????? ??? ????????????
	void CommRecovery();

};



#endif /* SRC_AP__INC_API_REMOTE_HPP_ */
