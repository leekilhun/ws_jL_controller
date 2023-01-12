/*
 * api_lcd.hpp
 *
 *  Created on: 2022. 12. 13.
 *      Author: gns2.lee
 */

#ifndef SRC_AP__INC_API_LCD_HPP_
#define SRC_AP__INC_API_LCD_HPP_



#define UI_LCD_MAX_BUFFER_LENGTH                     (32+1)

class api_lcd
{
	/****************************************************
	 *  data
	 ****************************************************/



	enum data_e:uint8_t
	{
		TYPE_MOTOR_POS_1_H    = 0x01,
		TYPE_MOTOR_POS_1_L  ,
		TYPE_MOTOR_POS_2_H  ,
		TYPE_MOTOR_POS_2_L  ,
		TYPE_MOTOR_POS_3_H  ,
		TYPE_MOTOR_POS_3_L  ,
		TYPE_AP_CFG_DAT     ,
		TYPE_CYL_DAT        ,
		TYPE_VAC_DAT        ,
		TYPE_SEQ_DAT        ,
		TYPE_IO             ,
		TYPE_AP_REG         ,
		TYPE_AXIS_STATE     ,
	};
public:
	struct cfg_t
	{
		NXLCD::uart_nextion* ptr_comm{};
		MCU_REG::ap_reg* ptr_mcu_reg{};
		cnTasks* ptr_task{};
		MCU_IO::ap_io* ptr_io{};
		ap_dat* ptr_cfg_data{};
		axis_dat* ptr_axis_data{};
		cyl_dat* ptr_cyl_data{};
		vac_dat* ptr_vac_data{};
		seq_dat* ptr_sequence_data{};
		ap_log* ptr_log{};
		cnAuto* ptr_auto{};

		cfg_t() = default;

		cfg_t& operator = (const cfg_t& cfg){
			ptr_mcu_reg 		= cfg.ptr_mcu_reg;
			ptr_comm 				= cfg.ptr_comm;
			ptr_task 				= cfg.ptr_task;
			ptr_io 					= cfg.ptr_io;
			ptr_cfg_data 		= cfg.ptr_cfg_data;
			ptr_cyl_data 		= cfg.ptr_cyl_data;
			ptr_vac_data 		= cfg.ptr_vac_data;
			ptr_sequence_data = cfg.ptr_sequence_data;
			ptr_log 				= cfg.ptr_log;
			ptr_auto 				= cfg.ptr_auto;
			return *(this);
		}

	};


private:
	bool m_IsInit;
	cfg_t m_cfg;
	NXLCD::uart_nextion::rx_packet_t m_receiveData;
	NXLCD::uart_nextion::page_e m_currPage;

	uint8_t m_txBuf[UI_LCD_MAX_BUFFER_LENGTH];
	bool m_waitReplyOK;
	bool m_OkReply;

public:
	prc_step_t m_step;

	/****************************************************
	 *  Constructor
	 ****************************************************/
public:
	api_lcd(): m_IsInit{}, m_cfg{},m_receiveData{}
	,m_currPage{}, m_txBuf{}, m_waitReplyOK{}, m_OkReply{},
	m_step{}
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
		m_cfg.ptr_comm->Recovery();
	}


	inline bool IsConnected(){
		//return m_cfg.ptr_comm->IsConnected();
		constexpr uint8_t rimit_err_cnt = 10;
	  if(m_cfg.ptr_comm->IsConnected())
	  	return (m_cfg.ptr_comm->GetErrCnt() > rimit_err_cnt) ? false: true;
	  return false;
	}

	// step machine을 통해 nonblock으로 처리된다.
	void ThreadJob();

	// PC에서 Command 수신 처리, 수신 ack 신호를 리턴한다
	void ProcessCmd(NXLCD::uart_nextion::rx_packet_t* ptr_data);

	inline errcode ChangePage(NXLCD::uart_nextion::page_e page)
	{
		uint8_t go_page = (uint8_t)page;
		//bool result = m_cfg.ptr_comm->SendCmdRxResp(NXLCD::TXCMD::LCD_CHANGE_PAGE
		//															,(uint8_t *)&go_page ,1, 200 );
		bool result = m_cfg.ptr_comm->SendCmd(NXLCD::TXCMD::LCD_CHANGE_PAGE
																					,&go_page ,1);
		if (result)
			return ERROR_SUCCESS;
		CommRecovery();
		return ERR_API_LCD_COMM_TIMEOUT;
	}

	inline void SetReparseMode(bool set = true){
		if (set)
		{
			m_waitReplyOK = true;
			uint8_t ser_reparse = 1;
			m_cfg.ptr_comm->SendCmdRxResp(NXLCD::TXCMD::LCD_START_REPARSEMODE
																		, &ser_reparse, 1);
		}
		else
		{
			m_cfg.ptr_comm->SendCmdRxResp(NXLCD::TXCMD::LCD_END_REPARSEMODE
																		, nullptr, 0);
		}

		m_waitReplyOK = false;

	}

	inline errcode SendMotorData(bool set_reparse = true){
		axis_dat::dat_t data{};
		uint8_t idx = 0;
		uint32_t length = sizeof(axis_dat::dat_t);

		if (set_reparse)
			SetReparseMode();
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_MOTOR_POS_1_H;
		data = m_cfg.ptr_axis_data->ReadData(0);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(1);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(2);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(3);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			goto err_out;

		idx = 0;
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_MOTOR_POS_1_L;
		data = m_cfg.ptr_axis_data->ReadData(4);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(5);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(6);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(7);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			goto err_out;

		idx = 0;
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_MOTOR_POS_2_H;
		data = m_cfg.ptr_axis_data->ReadData(8);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(9);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(10);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(11);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			goto err_out;

		idx = 0;
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_MOTOR_POS_2_L;
		data = m_cfg.ptr_axis_data->ReadData(12);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(13);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(14);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(15);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			goto err_out;

		idx = 0;
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_MOTOR_POS_3_H;
		data = m_cfg.ptr_axis_data->ReadData(16);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(17);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(18);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(19);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			goto err_out;

		idx = 0;
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_MOTOR_POS_3_L;
		data = m_cfg.ptr_axis_data->ReadData(20);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(21);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(22);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);
		data = m_cfg.ptr_axis_data->ReadData(23);
		memcpy(&m_txBuf[1 + (idx++ * length)], (uint8_t *)&data, length);

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			goto err_out;

		if (set_reparse)
			SetReparseMode(false);
		return ERROR_SUCCESS;

		err_out:
		if (set_reparse)
			SetReparseMode(false);
		return ERR_API_LCD_COMM_TIMEOUT;

	}


	inline errcode SendApIoReg(bool wait_ret = true, bool set_reparse = true){
		errcode ret = ERROR_SUCCESS;
		if (set_reparse)
			SetReparseMode();


		uint8_t idx = 0;
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[idx++] = data_e::TYPE_IO;

		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_in.data >> (8 * 0));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_in.data >> (8 * 1));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_in.data >> (8 * 2));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_in.data >> (8 * 3));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_out.data >> (8 * 0));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_out.data >> (8 * 1));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_out.data >> (8 * 2));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_io->m_out.data >> (8 * 3));


		if (wait_ret)
		{
			if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0], UI_LCD_MAX_BUFFER_LENGTH) == false)
				ret = ERR_API_LCD_COMM_TIMEOUT;
		}
		else
		{
			m_cfg.ptr_comm->SendData(&m_txBuf[0], UI_LCD_MAX_BUFFER_LENGTH);
		}


		if (set_reparse)
			SetReparseMode(false);
		return ret;

	}


	inline errcode SendApReg(bool wait_ret = true, bool set_reparse = true){
		errcode ret = ERROR_SUCCESS;
		if (set_reparse)
			SetReparseMode();


		uint8_t idx = 0;
		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[idx++] = data_e::TYPE_AP_REG;
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->option_reg.ap_option >> (8 * 0));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->option_reg.ap_option >> (8 * 1));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->state_reg.ap_state >> (8 * 0));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->state_reg.ap_state >> (8 * 1));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->error_reg.ap_error >> (8 * 0));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->error_reg.ap_error >> (8 * 1));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->error_reg.ap_error >> (8 * 2));
		m_txBuf[idx++] = (uint8_t)(m_cfg.ptr_mcu_reg->error_reg.ap_error >> (8 * 3));
		m_txBuf[idx++] = (uint8_t)100; // GetAutoSpeed();
		m_txBuf[idx++] = (uint8_t)1;// GetStep();
		m_txBuf[idx++] = (uint8_t)100;// loop count();


		if (wait_ret)
		{
			if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0], UI_LCD_MAX_BUFFER_LENGTH) == false)
				ret = ERR_API_LCD_COMM_TIMEOUT;
		}
		else
		{
			m_cfg.ptr_comm->SendData(&m_txBuf[0], UI_LCD_MAX_BUFFER_LENGTH);
		}


		if (set_reparse)
			SetReparseMode(false);
		return ret;

	}

	inline errcode SendApCfgData(bool set_reparse = true){
		errcode ret = ERROR_SUCCESS;
		ap_dat::dat_t data{};
		uint32_t length = sizeof(ap_dat::dat_t);
		if (set_reparse)
			SetReparseMode();

		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_AP_CFG_DAT;
		for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++)
		{
			data = m_cfg.ptr_cfg_data->ReadData(static_cast<ap_dat::addr_e>(i));
			memcpy(&m_txBuf[(1 + (i * length)) % UI_LCD_MAX_BUFFER_LENGTH], (uint8_t *)&data, length);
		}

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			ret = ERR_API_LCD_COMM_TIMEOUT;

		if (set_reparse)
			SetReparseMode(false);
		return ret;
	}

	inline errcode SendCylinderData(bool set_reparse = true){
		cyl_dat::dat_t data{};
		uint32_t length = sizeof(cyl_dat::dat_t);
		errcode ret = ERROR_SUCCESS;
		if (set_reparse)
			SetReparseMode();

		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_CYL_DAT;
		for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++)
		{
			data = m_cfg.ptr_cyl_data->ReadData(static_cast<cyl_dat::addr_e>(i));
			memcpy(&m_txBuf[(1 + (i * length)) % UI_LCD_MAX_BUFFER_LENGTH], (uint8_t *)&data, length);
		}

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			ret = ERR_API_LCD_COMM_TIMEOUT;

		if (set_reparse)
			SetReparseMode(false);
		return ret;
	}



	inline errcode SendVacuumData(bool set_reparse = true){
		vac_dat::dat_t data{};
		uint32_t length = sizeof(vac_dat::dat_t);
		errcode ret = ERROR_SUCCESS;
		if (set_reparse)
			SetReparseMode();

		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_VAC_DAT;
		for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++)
		{
			data = m_cfg.ptr_vac_data->ReadData(static_cast<vac_dat::addr_e>(i));
			memcpy(&m_txBuf[(1 + (i * length)) % UI_LCD_MAX_BUFFER_LENGTH], (uint8_t *)&data, length);
		}

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			ret = ERR_API_LCD_COMM_TIMEOUT;

		if (set_reparse)
			SetReparseMode(false);
		return ret;
	}

	inline errcode SendSequenceData(bool set_reparse = true){
		seq_dat::dat_t data{};
		uint32_t length = sizeof(seq_dat::dat_t);
		errcode ret = ERROR_SUCCESS;
		if (set_reparse)
			SetReparseMode();

		memset(&m_txBuf[0], 0x00, UI_LCD_MAX_BUFFER_LENGTH);
		m_txBuf[0] = data_e::TYPE_SEQ_DAT;
		for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++)
		{
			data = m_cfg.ptr_sequence_data->ReadData(static_cast<seq_dat::addr_e>(i));
			memcpy(&m_txBuf[(1 + (i * length)) % UI_LCD_MAX_BUFFER_LENGTH], (uint8_t *)&data, length);
		}

		if (m_cfg.ptr_comm->SendDataRxResp(&m_txBuf[0] , UI_LCD_MAX_BUFFER_LENGTH ) == false)
			ret = ERR_API_LCD_COMM_TIMEOUT;

		if (set_reparse)
			SetReparseMode(false);
		return ret;
	}


	errcode SendLogData();
	void LcdUpdate();

	inline errcode SendDataAll(){
		errcode ret = ERROR_SUCCESS;

		SetReparseMode();

		//delay(10);

		if (SendMotorData(false) != ERROR_SUCCESS)
			goto err_out;

		if(SendApCfgData(false) != ERROR_SUCCESS)
			goto err_out;

		if(SendCylinderData(false) != ERROR_SUCCESS)
			goto err_out;

		if(SendVacuumData(false) != ERROR_SUCCESS)
			goto err_out;

		if(SendSequenceData(false) != ERROR_SUCCESS)
			goto err_out;

		err_out:
		SetReparseMode(false);
		return ret;
	}


};



#endif /* SRC_AP__INC_API_LCD_HPP_ */
