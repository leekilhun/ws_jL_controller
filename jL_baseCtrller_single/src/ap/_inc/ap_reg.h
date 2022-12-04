
/*
 * ap_reg.h
 *
 *  Created on: 2022. 2. 22.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_REG_H_
#define SRC_AP_AP_REG_H_

namespace MCU_REG
{


#ifdef _USE_HW_RTOS
#define AP_REG_LOCK_BEGIN osMutexWait(ap_reg_mutex_id, osWaitForever)
#define AP_REG_LOCK_END   osMutexRelease(ap_reg_mutex_id)
#else
#define AP_REG_LOCK_BEGIN
#define AP_REG_LOCK_END
#endif


struct ap_reg
{
  enum err_e
	{
    no_error,
    no_response_lcd,
    no_response_mot,
    reserved_03,
    reserved_04,
    reserved_05,
    reserved_06,
    motor_not_ready,

    motor_limit,
    reserved_11,
    reserved_12,
    reserved_13,
    cyl_timeout,
    vac_timeout,
    move_timout,
    seq_timeout,
    _max
  };
  enum state_e
	{
  	AUTO_RUNNING,
  	EMG_STOP,
  	AUTO_READY,
  	ERROR_STOP,
  	DETECT_AREA_SEN,
  	MOTOR_ON,
  	ORG_COMPLETED,
  	ALARM_STATUS,

		STATE_10,
		STATE_11,
		STATE_12,
		STATE_13,
		STATE_14,
		STATE_15,
		STATE_16,
		STATE_17,
	};

  union AP_ERR_REG
  {
  	uint32_t ap_error{};
  	struct
		{
  		unsigned no_error:1;
  		unsigned no_resp_lcd:1;
  		unsigned no_resp_mot:1;
  		unsigned err_03:1;
  		unsigned err_04:1;
  		unsigned err_05:1;
  		unsigned err_state_motor:1;
  		unsigned not_ready_motor:1;

  		unsigned limit_err_motor:1;
  		unsigned cylinder_interlock:1;
  		unsigned err_12:1;
  		unsigned err_13:1;
  		unsigned cylinder_timeout:1;
  		unsigned vacuum_timeout:1;
  		unsigned motor_timeout:1;
  		unsigned sequnce_timeout:1;

  		unsigned err_20:1;
  		unsigned err_21:1;
  		unsigned err_22:1;
  		unsigned err_23:1;
  		unsigned err_24:1;
  		unsigned err_25:1;
  		unsigned err_26:1;
  		unsigned err_27:1;

  		unsigned err_30:1;
  		unsigned err_31:1;
  		unsigned err_32:1;
  		unsigned err_33:1;
  		unsigned err_34:1;
  		unsigned err_35:1;
  		unsigned err_36:1;
  		unsigned err_37:1;
		};
  };


  union AP_OPT_REG
  {
  	uint16_t ap_option{};
  	struct
		{
  		unsigned use_beep:1;
  		unsigned use_dryrun:1;
  		unsigned skip_doorsensor:1;
  		unsigned skip_safesensor:1;
  		unsigned option_04:1;
  		unsigned option_05:1;
  		unsigned option_06:1;
  		unsigned option_07:1;

  		unsigned option_10:1;
  		unsigned option_11:1;
  		unsigned option_12:1;
  		unsigned option_13:1;
  		unsigned option_14:1;
  		unsigned option_15:1;
  		unsigned option_16:1;
  		unsigned option_17:1;
		};
  };

  union AP_STATE_REG
  {

  	uint16_t ap_state{};
  	struct{
  		unsigned auto_running:1;
  		unsigned emg_stop:1;
  		unsigned auto_ready:1;
  		unsigned error_stop:1;
  		unsigned detect_safe_sensor:1;
  		unsigned motor_on:1;
  		unsigned system_origin_cplt:1;
  		unsigned alarm_status:1;

  		unsigned state_10:1;
  		unsigned state_11:1;
  		unsigned state_12:1;
  		unsigned state_13:1;
  		unsigned state_14:1;
  		unsigned state_15:1;
  		unsigned request_initial:1;
  		unsigned all_check_ok:1;
  	};
  };



#ifdef _USE_HW_RTOS
  osMutexId ap_reg_mutex_id;
#endif

  AP_ERR_REG error_reg;
  AP_OPT_REG option_reg;
  AP_STATE_REG state_reg;

  /*constructor*/
  ap_reg(): error_reg{},option_reg{}, state_reg{} {
#ifdef _USE_HW_RTOS
    osMutexDef(ap_reg_mutex_id);
    ap_reg_mutex_id = osMutexCreate (osMutex(ap_reg_mutex_id));
#endif
  }

  inline bool IsProcessInitialCplt() const {
      return state_reg.system_origin_cplt;//status[AP_REG_BANK_RUN_STATE][AP_REG_ORG_COMPLETED];
    }

  inline bool IsSkipSafeSensor() const {
    return option_reg.skip_safesensor;//status[AP_REG_BANK_SETTING][AP_REG_SKIP_AREA_SENSOR];
  }

  inline bool IsAllCheckOk() const {
     return state_reg.all_check_ok;//status[AP_REG_BANK_SETTING][AP_REG_ALL_CEHCK_OK];
   }

  inline bool IsUseDryRun() const {
    return option_reg.use_dryrun;//status[AP_REG_BANK_SETTING][AP_REG_USE_DRY_RUN];
  }

  inline void SetUseDryRun(bool on_off){
  	option_reg.use_dryrun = on_off;//status[AP_REG_BANK_SETTING][AP_REG_USE_DRY_RUN] = on_off;
  }

  inline bool IsAutoReadyMode() const {
    return state_reg.auto_ready; //status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_READY];
  }

  inline bool IsAutoRunning(){
    return state_reg.auto_running; //status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_RUNNING];
  }

  inline void SetConfigRegister(uint16_t data){
  	state_reg.ap_state = data;
  }

  inline uint32_t GetAlarmState(){
    return error_reg.ap_error;
  }

  inline void ClearAlarmState(){
  	 error_reg.ap_error = 0;
  }

  inline void SetReg_AutoRunning(bool on_off = true){
  	state_reg.auto_running = on_off;
  }

  inline void SetReg_State(state_e state, bool on_off = true){
    AP_REG_LOCK_BEGIN;
    uint16_t tmp {};
    if (on_off)
    {
    	tmp = 1 << uint16_t(state);
      state_reg.ap_state |= tmp;
    }
    else
    {
    	tmp = ~(1 << uint16_t(state));
    	state_reg.ap_state &= tmp;
    }
    AP_REG_LOCK_END;
  }


};


}




#endif /* SRC_AP_AP_REG_H_ */
