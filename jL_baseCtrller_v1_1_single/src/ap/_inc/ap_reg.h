
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

	/*

	 ## 상태 정의

	 01. AUTO_RUNNING : 자동 실행
	 02. EMG_STOP : 비상정지
	 03. AUTO_READY : 자동 실행이 가능한 지 체크하고, 문제 없다면 시작 신호를 대기
	 04. AUTO_STOP : 자동 실행 중 정지된 상태
	 05. DETECT_AREA_SEN : 자동 동작이 정지되어야 한다.
	 06. MOTOR_ON : 모터가 정상적으로 enable 됨
	 07. ORG_COMPLETED : 시스템 초기화가 완료되어 자동 운전이 가능한 상태
	 08. ALARM_STATUS : 시스템에 리셋할 에러가 있다.

	 11. STATE_10,
	 12. STATE_11,
	 13. STATE_12,
	 14. STATE_13,
	 15. STATE_14,
	 16. STATE_15,
	 17. REQUEST_INITAL,
	 18. ALL_CHECK_OK,







	 */


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
			AUTO_STOP,
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
			REQUEST_INITAL,
			ALL_CHECK_OK,
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
				unsigned auto_stop:1;
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

		inline uint16_t SetReg_State(MCU_REG::ap_reg::state_e state, bool on_off = true) {
			AP_REG_LOCK_BEGIN;
			switch (state) {
				case AUTO_RUNNING:
				{

				}
				break;

				case EMG_STOP:
				{
					if (on_off)
						state_reg.emg_stop = true;
					else
						state_reg.emg_stop = false;
				}
				break;

				case AUTO_READY:
				{

				}
				break;

				case AUTO_STOP:
				{

				}
				break;

				case DETECT_AREA_SEN:
				{
					if (on_off)
						state_reg.detect_safe_sensor = true;
					else
						state_reg.detect_safe_sensor = false;
				}
				break;

				case MOTOR_ON:
				{

					if (on_off)
						state_reg.motor_on = true;
					else
						state_reg.motor_on = false;
				}
				break;

				case ORG_COMPLETED:
				{

				}
				break;

				case ALARM_STATUS:
				{
					if (on_off)
						state_reg.alarm_status = true;
					else
						state_reg.alarm_status = false;

				}
				break;

				case STATE_10:
				{

				}
				break;
				case STATE_11:
				{

				}
				break;
				case STATE_12:
				{

				}
				break;
				case STATE_13:
				{

				}
				break;
				case STATE_14:
				{

				}
				break;
				case STATE_15:
				{

				}
				break;
				case REQUEST_INITAL:
				{
					state_reg.request_initial = on_off;
				}
				break;
				case ALL_CHECK_OK:
				{
					state_reg.all_check_ok = on_off;
				}
				break;
				default:
					break;
			}
			// end of switch (state)


			AP_REG_LOCK_END;
			return state_reg.ap_state;
		}


		inline void SetErrRegister(ap_reg::err_e state, bool set = true){

			switch (state) {
				case err_e::no_response_lcd:
					error_reg.no_resp_lcd = set;
					break;

				case err_e::no_response_mot:
					error_reg.no_resp_mot = set;
					break;

				case err_e::reserved_03:
					break;

				case err_e::reserved_04:
					break;

				case err_e::reserved_05:
					break;

				case err_e::reserved_06:
					break;

				case err_e::motor_not_ready:
					error_reg.not_ready_motor = set;
					break;

				case err_e::motor_limit:
					error_reg.limit_err_motor = set;
					break;

				case err_e::reserved_11:
					break;

				case err_e::reserved_12:
					break;

				case err_e::reserved_13:
					break;

				case err_e::cyl_timeout:
					error_reg.cylinder_timeout = set;
					break;

				case err_e::vac_timeout:
					error_reg.vacuum_timeout = set;
					break;

				case err_e::move_timout:
					error_reg.motor_timeout = set;
					break;

				case err_e::seq_timeout:
					error_reg.sequnce_timeout = set;
					break;
				default:
					break;
			}


			if (error_reg.ap_error > 1)
			{
				error_reg.no_error = false;
			}



		}

		inline void SetMotorErrState(uint8_t motor_state){
			union motor_comm
			{
				uint8_t comm_err{};
				struct
				{
					unsigned	idx_0_comm_err : 1;       // = 0x00000001;
					unsigned	idx_1_comm_err : 1;		    // = 0x00000002;
					unsigned	idx_2_comm_err : 1;			  // = 0x00000004;
					unsigned	idx_3_comm_err : 1;	      // = 0x00000008;
					unsigned	idx_4_comm_err : 1;       // = 0x00000010;
					unsigned	idx_5_comm_err : 1;		    // = 0x00000020;
					unsigned	idx_6_comm_err : 1;	 	    // = 0x00000040;
					unsigned	idx_7_comm_err : 1;	      // = 0x00000080;
				};
			};
			motor_comm state{motor_state};

			if (state.idx_0_comm_err
					||state.idx_1_comm_err
					||state.idx_2_comm_err)
			{
				SetErrRegister(err_e::no_response_mot);
			}
			else
			{
				SetErrRegister(err_e::no_response_mot, false);
			}

		}


	};


}




#endif /* SRC_AP_AP_REG_H_ */
