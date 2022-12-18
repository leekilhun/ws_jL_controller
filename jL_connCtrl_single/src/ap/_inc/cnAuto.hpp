/*
 * cnAuto.hpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_CNAUTO_HPP_
#define SRC_AP__INC_CNAUTO_HPP_


#define CN_AUTO_STEP_RETRY_CNT_MAX               3

#define CN_AUTO_STEP_INIT                        0
#define CN_AUTO_STEP_TODO                        1
#define CN_AUTO_STEP_SW_CHECK_DOUBLE             2
#define CN_AUTO_STEP_SW_CHECK_DOUBLE_START       3
#define CN_AUTO_STEP_SW_CHECK_DOUBLE_WAIT        4
#define CN_AUTO_STEP_SW_CHECK_DOUBLE_END         5

#define CN_AUTO_STEP_TIMEOUT                     254
#define CN_AUTO_STEP_END                         255

#define AUTO_ALARM(head, msg)  AlarmAuto(head, (__FILE__), __FUNCTION__, __LINE__,  msg)

class cnAuto
{
	/****************************************************
	 *  data
	 ****************************************************/
public:
	enum state_e
	{
		ready,
		mcu_unit_err,
		seq_initial_timeout,
		emg_stop,
		error_stop,

		servo_on_err,
		servo_off_err,
		axis_origin_err,
		axis_start_timeout,
		axis_move_timeout,
		axis_stop_timeout,

		cyl_PHONE_JIG_open_timeout,
		cyl_PHONE_JIG_lock_timeout,
		cyl_PHONE_JIG_forward_timeout,
		cyl_VINYL_grip_timeout,
		cyl_VINYL_push_timeout,

		cyl_PHONE_JIG_close_timeout,
		cyl_PHONE_JIG_unlock_timeout,
		cyl_PHONE_JIG_backward_timeout,
		cyl_VINYL_ungrip_timeout,
		cyl_VINYL_back_timeout,

		vac_PHONE_JIG_on_timeout,
		vac_PHONE_JIG_off_timeout,

		cyl_interlock_State,
	};


	enum sw_e
	{
		sw_start,
		sw_stop,
		sw_reset,
		sw_max,
	};

	enum sw_event
	{
		invalid,
		short_key,
		long_key,
		double_key,
	};

	struct sw_t
	{
		sw_e type{};
		bool is_press{};
		sw_event state{};
		volatile uint32_t delta_cnt{};
		uint32_t pre_ms{};

		inline void SetType(sw_e type){
			this->type = type;
		}

		inline void init(){
			is_press = false;
			delta_cnt = 0 ;
			pre_ms = 0;
		}

		inline void eventClear(){
			state = sw_event::invalid;
		}

	};

	struct cfg_t  {
		MCU_REG::ap_reg* p_apReg;
		enOp* p_op;
		ap_log* p_apLog;
		iio * p_ApIo;

		cfg_t() = default;

		cfg_t& operator = (const cfg_t& cfg)
		{
			if (this != &cfg){
				p_apReg = cfg.p_apReg;
				p_apLog = cfg.p_apLog;
				p_ApIo = cfg.p_ApIo;
				p_op = cfg.p_op;
			}
			return *this;
		}


	};
private:
	cfg_t m_cfg;
	//Ap_reg* m_pApReg;
	//enOp* m_pOP;
	//ap_log* m_pApLog;
	//iio * m_pApIo;

	enOp::mode_e m_OpMode;
	enOp::status_e m_OpStatus;
	bool m_checkReady;
	//uint8_t m_retryCnt;
	//uint32_t  m_pre_time;
	prc_step_t  m_step;
	sw_t m_pushSw[sw_max];
	bool m_FlagStartSw;
	bool m_IsDetectedPauseSensor;
	uint32_t m_errCnt;
	/****************************************************
	 *  Constructor
	 ****************************************************/
public:
	cnAuto ():m_cfg{},m_OpMode{},m_OpStatus{}, m_checkReady{}
	, m_step{}, m_pushSw{},m_FlagStartSw{}, m_IsDetectedPauseSensor{}
	, m_errCnt{} {
		m_OpMode = enOp::mode_e::STOP;
		m_OpStatus=enOp::status_e::INIT;



	} ;

	virtual  ~cnAuto (){};

	/****************************************************
	 *  func
	 ****************************************************/
private:
	void doRunStep();

	// 모듈의 자동 운전 조건을 확인
	inline state_e checkStartRunCondition(){
		cnAuto::state_e ret = cnAuto::state_e::ready;
		MCU_REG::ap_reg* ap_reg = m_cfg.p_apReg;
		constexpr int ERR_CNT_MAX = 100;

		if (ap_reg->error_reg.no_error)
		{
			m_errCnt = 0;
			// state commication, axis 등의 구성품의 상태가 문제가 없다면
			// 구동 중 발생되는 e-stop, error stop 에러 상태는 구성품 상태는 정상
			// 구동 중 발생되는 위 상태는 alarm 비트 활성화로 통합하여 표시

			if(ap_reg->state_reg.alarm_status)
			{

				if(ap_reg->state_reg.emg_stop)
				{
					ret = cnAuto::state_e::emg_stop;
				}
				else if(ap_reg->state_reg.error_stop)
				{
					ret = cnAuto::state_e::error_stop;
				}
			}
			else if (ap_reg->state_reg.motor_on == false)
			{
				ret =cnAuto::state_e::servo_on_err;
			}
			else if (ap_reg->state_reg.system_origin_cplt == false)
			{
				ret =cnAuto::state_e::axis_origin_err;
			}
		}
		else
		{
			if(m_errCnt < ERR_CNT_MAX )
			{
				m_errCnt++;
			}
			else
			{
				m_errCnt = 0 ;
				ret =cnAuto::state_e::mcu_unit_err;
			}
		}

		return ret;
	}

public:
	inline void Init(cnAuto::cfg_t &cfg){
		m_cfg = cfg;
	}

	inline enOp::mode_e GetOPMode(){
		return m_OpMode;
	}
	inline enOp::status_e GetOPStatus(){
		return m_OpStatus;
	}
	inline void SetOPMode(enOp::mode_e mode){
		m_cfg.p_op->SetMode(mode);
		m_OpMode = mode;
	}
	inline void SetOPStatus(enOp::status_e status){
		m_cfg.p_op->SetStatus(status);
		m_OpStatus = status;
	}

	inline void StartSw(){
		if (m_pushSw[sw_start].is_press == false)
		{
			m_pushSw[sw_start].is_press = true;
			m_pushSw[sw_start].delta_cnt = 0;
			m_pushSw[sw_start].pre_ms = millis();
		}
		else
		{
			m_pushSw[sw_start].pre_ms = millis();
		}
	}
	inline void StopSw(){
		SetOPMode(enOp::mode_e::STOP);
		SetOPStatus(enOp::status_e::STEP_STOP);
		m_checkReady = false;
	}

	inline void ResetSw(){
		SetOPMode(enOp::mode_e::STOP);
		SetOPStatus(enOp::status_e::STEP_STOP);
		m_cfg.p_apReg->ClearAlarmState();
		m_checkReady = false;
		m_IsDetectedPauseSensor = false;


		uint32_t pre_ms = millis();
		while(m_cfg.p_op->GetPressed(enOp::panel_e::SW_RESET))
		{
			if(millis() - pre_ms >= (1000*3))
			{
				m_cfg.p_apReg->state_reg.request_initial = true;
				break;
			}
		}
	}

	inline void PauseStop(){
		m_IsDetectedPauseSensor = true;
	}


	inline bool IsDetectAreaSensor(){
		bool ret = false;
		if (!m_cfg.p_apReg->IsSkipSafeSensor())//not
		{
			ret = m_cfg.p_ApIo->IsOff(MCU_IO::ap_io::in_e::in_area_sensor);
			ret = m_IsDetectedPauseSensor;
		}
		return ret;
	}


	inline void ResetFlagAutoReady(){
		m_FlagStartSw = false;
		SetOPStatus(enOp::status_e::RUN_READY);
	}


	inline void UiStartSw(){
		m_FlagStartSw = true;
		m_checkReady = true;
	}

	void ThreadJob();
	// condition for auto-run
	int AutoReady();

	void AlarmAuto(cnAuto::state_e err);

	void AlarmAuto(log_dat::head_t *p_head,
								 const char* file,
								 const char* func,
								 const int line,
								 const char* msg);

};


#endif /* SRC_AP__INC_CNAUTO_HPP_ */
