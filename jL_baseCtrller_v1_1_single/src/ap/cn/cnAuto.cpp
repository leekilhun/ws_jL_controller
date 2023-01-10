/*
 * cnAuto.cpp
 *
 *  Created on: 2022. 12. 4.
 *      Author: gns2l
 */

#include "ap.h"
#include "cnAuto.hpp"

//#define CN_AUTO_ERR_CNT_MAX       255


constexpr int CN_AUTO_ARG_TBL_CNT_MAX   = 10;
constexpr int CN_AUTO_MSG_STR_MAX       = 20;
constexpr int CN_AUTO_FILE_STR_MAX      = 40;

void cnAuto::ThreadJob()
{

}
// condition for auto-run
int cnAuto::AutoReady()
{
	return 0;
}

void cnAuto::AlarmAuto(cnAuto::state_e err)
{
	switch (err)
	{
		case ready:
			break;
		case mcu_unit_err:
			__attribute__((fallthrough));
		case seq_initial_timeout:
		case emg_stop:
		case error_stop:
			break;

		case servo_on_err:
			__attribute__((fallthrough));
		case servo_off_err:
		case axis_origin_err:
			m_cfg.p_apReg->error_reg.not_ready_motor = true;
			break;

		case axis_start_timeout:
			__attribute__((fallthrough));
		case axis_move_timeout:
		case axis_stop_timeout:
			m_cfg.p_apReg->error_reg.motor_timeout = true;
			break;

		case cyl_PHONE_open_timeout:
			__attribute__((fallthrough));
		case cyl_PHONE_close_timeout:
		case cyl_PHONE_for_timeout:
		case cyl_PHONE_back_timeout:
		case cyl_VINYLHOLD_hold_timeout:
		case cyl_VINYLHOLD_release_timeout:
		case cyl_VINYL_push_timeout:
		case cyl_VINYL_back_timeout:
		case cyl_timeout:
			m_cfg.p_apReg->error_reg.cylinder_timeout = true;
			break;

		case vac_deletc_on_timeout:
			__attribute__((fallthrough));
		case vac_timeout:
			m_cfg.p_apReg->error_reg.vacuum_timeout = true;
			break;

		case cyl_interlock_State:
			m_cfg.p_apReg->error_reg.cylinder_interlock = true;
			break;

		default:
			break;
	}
	//switch (err)

	SetOPStatus(enOp::status_e::ERR_STOP);
}

void cnAuto::AlarmAuto(log_dat::head_t *p_head,
											 const char* file,
											 const char* func,
											 const int line,
											 const char* msg)
{

	cnAuto::state_e err = static_cast<cnAuto::state_e>(p_head->error_no);

	char tmp_str[CN_AUTO_FILE_STR_MAX] = { 0, };
	strcpy(tmp_str, file);
	char* arg_tbl[CN_AUTO_ARG_TBL_CNT_MAX] = {};
	uint8_t arg_cnt = trans::SplitArgs(tmp_str, arg_tbl, "/", CN_AUTO_ARG_TBL_CNT_MAX);

	m_cfg.p_apLog->apLogWrite(p_head, "file[%s],func[%s],msg[%s]",arg_tbl[arg_cnt - 1],func,msg);

	AlarmAuto(err);

}

