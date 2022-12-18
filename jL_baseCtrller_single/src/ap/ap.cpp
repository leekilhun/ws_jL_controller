/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "ap.h"



void updateApReg();
void updateErr();
void eventOpPanel();
void updateLamp();

uint16_t refresh_time = 1000;
uint32_t lamp_ms = 0;
uint32_t test_ms =0;

/****************************************************
  0. mcu data and register
 ****************************************************/
MCU_REG::ap_reg mcu_reg;
MCU_IO::ap_io mcu_io;
ap_dat apCfg_data;
axis_dat axis_data;
cyl_dat cyl_data;
vac_dat vac_data;
seq_dat seq_data;
ap_log mcu_log;
/****************************************************
  1. ap에서 생성할 객체
 ****************************************************/
// engine layer (물리 계층)
MOTOR::uart_moons moons_comm;


MOTOR::enMotor_moons moons_motors[AP_DEF_OBJ_MOTOR_ID_MAX]
																	{M_SetMotorId(AP_DEF_OBJ_MOTOR_ID_JIG),2,3};
ACT::enCyl cyl[AP_DEF_OBJ_CYL_ID_MAX];
ACT::enVac vac[AP_DEF_OBJ_VACUUM_ID_MAX];
enOp op_panel;


// control  (로직 계층)
cnAuto autoManager;
cnJob process;
cnTasks tasks;
MOTOR::cnMotors motors;

// user interface.
NXLCD::uart_nextion nextion_lcd;
RCTRL::uart_remote remote_ctrl;

api_remote remote_pc;
api_lcd op_lcd;

void apInit(void)
{

#if 0

	/* manual operating initial */
	{
		cnDo::cfg_t do_cfg = {0, };
		do_cfg.p_apReg = &mcu_reg;
		do_cfg.p_apIo = &mcu_io;
		do_cfg.p_Fm = &fastech_motor;
		do_cfg.p_Cyl = cyl;
		do_cfg.p_Vac = vac;
		do_cfg.p_Op = &op_panel;
		do_cfg.p_AutoManger = &autoManager;
		do_cfg.p_apAxisDat = &axis_data;
		do_cfg.p_apCylDat = &cyl_data;
		do_cfg.p_apVacDat = &vac_data;
		do_cfg.p_apCfgDat = &apCfg_data;
		do_cfg.p_apSeqDat = &seq_data;
		doJob.Init(&do_cfg);
	}

	/* sequence process initial */
	{
		cnJob::cfg_t prc_cfg = {0, };
		prc_cfg.p_apReg = &mcu_reg;
		prc_cfg.p_apIo = &mcu_io;
		prc_cfg.p_Fm = &fastech_motor;
		prc_cfg.p_Cyl = cyl;
		prc_cfg.p_Vac = vac;
		prc_cfg.p_Op = &op_panel;
		prc_cfg.p_AutoManger = &autoManager;
		prc_cfg.p_apAxisDat = &axis_data;
		prc_cfg.p_apCylDat = &cyl_data;
		prc_cfg.p_apVacDat = &vac_data;
		prc_cfg.p_apCfgDat = &apCfg_data;
		prc_cfg.p_apSeqDat = &seq_data;
		process.Init(&prc_cfg);
	}

#endif


	// uart
	{
		using namespace MOTOR;
		uart_moons::cfg_t cfg{};
		cfg.ch = _DEF_UART1;
		cfg.baud = 115200;
		moons_comm.Init(cfg);
	}
	{
		using namespace NXLCD;
		uart_nextion::cfg_t cfg{};
		cfg.ch =_DEF_UART3;
		cfg.baud = 115200;
		nextion_lcd.Init(cfg);

	}
	{
		using namespace RCTRL;
		uart_remote::cfg_t cfg{};
		cfg.ch = _DEF_UART2;
		cfg.baud = 115200;
		remote_ctrl.Init(cfg);
	}

	/* operating panel sw initial */
	{
		enOp::cfg_t cfg ={0,};
		cfg.sw_pin_start = _GPIO_OP_SW_START;
		cfg.sw_pin_stop  = _GPIO_OP_SW_STOP;
		cfg.sw_pin_reset = _GPIO_OP_SW_RESET;
		cfg.sw_pin_estop = _GPIO_OP_SW_ESTOP;

		cfg.lamp_pin_start = _GPIO_OP_LAMP_START;
		cfg.lamp_pin_stop = _GPIO_OP_LAMP_STOP;
		cfg.lamp_pin_reset = _GPIO_OP_LAMP_RESET;
		op_panel.Init(cfg);
	}

	/* cylinder initial */
	{
		using namespace ACT;

		/*phone jig open close*/
		enCyl::cfg_t cyl_cfg = {0, };
		cyl_cfg.cyl_id = AP_DEF_OBJ_CYL_ID_PHONE_OPEN_CLOSE;
		cyl_cfg.cyl_type = enCyl::type_e::open_close;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::phone_clamp_cyl);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_1_on;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_1_off;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_1_on;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_1_off;

		cyl[AP_DEF_OBJ_CYL_ID_PHONE_OPEN_CLOSE].SetConfigData(cyl_cfg);

		/*phone jig for align gripper*/
		cyl_cfg.cyl_id = AP_DEF_OBJ_CYL_ID_PHONE_LOCK_UNLOCK;
		cyl_cfg.cyl_type = enCyl::type_e::lock_unlock;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::drum_updown_cyl);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_2_on;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_2_off;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_2_on;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_2_off;

		cyl[AP_DEF_OBJ_CYL_ID_PHONE_LOCK_UNLOCK].SetConfigData(cyl_cfg);

		/*phone jig forward-backward for loading*/
		cyl_cfg.cyl_id = AP_DEF_OBJ_CYL_ID_PHONE_FOR_BACK;
		cyl_cfg.cyl_type = enCyl::type_e::forward_backward;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::drum_updown_cyl);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_2_on;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_2_off;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_2_on;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_2_off;

		cyl[AP_DEF_OBJ_CYL_ID_PHONE_FOR_BACK].SetConfigData(cyl_cfg);


		/*vinyl gripper for detach at roller*/
		cyl_cfg.cyl_id = AP_DEF_OBJ_CYL_ID_VINYL_GRIP_UNGRIP;
		cyl_cfg.cyl_type = enCyl::type_e::lock_unlock;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::drum_updown_cyl);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_2_on;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_2_off;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_2_on;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_2_off;

		cyl[AP_DEF_OBJ_CYL_ID_VINYL_GRIP_UNGRIP].SetConfigData(cyl_cfg);


		/*vinyl gripper for detach at roller*/
		cyl_cfg.cyl_id = AP_DEF_OBJ_CYL_ID_VINYL_PUSH;
		cyl_cfg.cyl_type = enCyl::type_e::forward_backward;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::drum_updown_cyl);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_2_on;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_2_off;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_2_on;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_2_off;

		cyl[AP_DEF_OBJ_CYL_ID_VINYL_PUSH].SetConfigData(cyl_cfg);

	}

	/* vacuum initial */
	{
		using namespace ACT;

		/*drum head vacuum*/
		enVac::cfg_t vac_cfg = { 0, };
		vac_cfg.vac_id = AP_DEF_OBJ_VACUUM_ID_PHONE_JIG;
		vac_cfg.vac_type = enVac::type_e::suction_blow;
		vac_cfg.pApIo = (iio *)&mcu_io;
		vac_cfg.pVacDat = vac_data.GetData(vac_dat::addr_e::peel_drum_vac);
		vac_cfg.sensor_io = MCU_IO::ap_io::in_drum_vac_on;
		vac_cfg.sol_io[enVac::on_suction] = MCU_IO::ap_io::out_vac_1_on;
		vac_cfg.sol_io[enVac::on_blow] = MCU_IO::ap_io::out_vac_1_off;

		vac[AP_DEF_OBJ_VACUUM_ID_PHONE_JIG].SetConfigData(vac_cfg);
	}

	/* motor initial */
	{
		using namespace MOTOR;

		enMotor_moons::cfg_t cfg = {0, };
		cfg.instance_no =AP_OBJ::MOTOR_JIG;
		cfg.p_apReg = &mcu_reg;
		cfg.p_apCfgDat = &apCfg_data;
		cfg.p_apAxisDat = &axis_data;
		cfg.p_comm = &moons_comm;
		cfg.motor_param.Init();
		moons_motors[AP_OBJ::MOTOR_JIG].Init(cfg);


		cfg = {0, };
		cfg.instance_no =AP_OBJ::MOTOR_ROLL;
		cfg.p_apReg = &mcu_reg;
		cfg.p_apCfgDat = &apCfg_data;
		cfg.p_apAxisDat = &axis_data;
		cfg.p_comm = &moons_comm;
		cfg.motor_param.Init();
		moons_motors[AP_OBJ::MOTOR_ROLL].Init(cfg);


		cfg = {0, };
		cfg.instance_no =AP_OBJ::MOTOR_HIGH;
		cfg.p_apReg = &mcu_reg;
		cfg.p_apCfgDat = &apCfg_data;
		cfg.p_apAxisDat = &axis_data;
		cfg.p_comm = &moons_comm;
		cfg.motor_param.Init();
		moons_motors[AP_OBJ::MOTOR_HIGH].Init(cfg);
	}

	/* control motors */
	{
		using namespace MOTOR;

		cnMotors::cfg_t cfg = {0,};
		cfg.p_motor = moons_motors;
		cfg.p_apAxisDat =  &axis_data;
		cfg.p_comm = &moons_comm;
		cfg.p_apCfgDat = &apCfg_data;
		cfg.p_apIo = &mcu_io;
		cfg.p_apReg = &mcu_reg;
		motors.Init(cfg);
	}


	/* automanager initial */
	{
		cnAuto::cfg_t auto_cfg = {0, };
		auto_cfg.p_apReg = &mcu_reg;
		auto_cfg.p_apLog = &mcu_log;
		auto_cfg.p_ApIo = (iio *)&mcu_io;;
		autoManager.Init(auto_cfg);
	}

	/* manual operating initial */
	{
		cnTasks::cfg_t cfg = {0, };
		cfg.p_apReg = &mcu_reg;
		cfg.p_apIo = &mcu_io;
		cfg.p_motors = &motors;
		cfg.p_Cyl = cyl;
		cfg.p_Vac = vac;
		cfg.p_Op = &op_panel;
		cfg.p_AutoManger = &autoManager;
		cfg.p_apAxisDat = &axis_data;
		cfg.p_apCylDat = &cyl_data;
		cfg.p_apVacDat = &vac_data;
		cfg.p_apCfgDat = &apCfg_data;
		cfg.p_apSeqDat = &seq_data;
		tasks.Init(cfg);
	}

	/* sequence process initial */
	{

	}

	/*user display object*/
	{
		api_lcd::cfg_t cfg{};
		cfg.ptr_comm = &nextion_lcd;

		op_lcd.Init(cfg);
	}

	/*remote control and monitor*/
	{
		api_remote::cfg_t cfg{};
		cfg.ptr_comm = &remote_ctrl;

		remote_pc.Init(cfg);
	}



#if 0
	/****************************/
	/* loading eeprom data*/
	//apCfg_data.ClearRomData();
	apCfg_data.LoadRomData();
	delay(5);
	//axis_data.ClearRomData();
	axis_data.LoadRomData();
	delay(5);
	//cyl_data.ClearRomData();
	cyl_data.LoadRomData();
	delay(5);
	//vac_data.ClearRomData();
	vac_data.LoadRomData();
	delay(5);
	//seq_data.ClearRomData();
	seq_data.LoadRomData();

	/****************************/
	mcu_reg.status[AP_REG_BANK_SETTING][AP_REG_USE_BEEP] = true;
	/****************************/

	/****************************/
	process.SetAutoSpeed(seq_data.GetMaxSpeed());
	/****************************/
	/*Assign Obj */
	mcu_io.assignObj((iio *)&fastech_motor);
#endif


}


void apMain(void)
{


	uint8_t motor_err_state{};
	uint32_t pre_main_ms = millis();
	while (1)
	{
		if (millis() - pre_main_ms >= refresh_time)
		{
			ledToggle(_DEF_LED1);
			pre_main_ms = millis();
		}

		// 1. request moons motor state data;
		{
			motor_err_state = motors.UpdateMotorsState();
			mcu_reg.SetMotorErrState(motor_err_state);

			//mcu_reg.error_reg


		}


		moons_comm.ReceiveProcess();
		nextion_lcd.ReceiveProcess();
		remote_ctrl.ReceiveProcess();

		updateErr();
		updateApReg();
		updateLamp();

	}

}




void eventOpPanel()
{
	if (op_panel.GetPressed(enOp::panel_e::SW_ESTOP))
	{
		mcu_reg.state_reg.emg_stop = true;
		return;
	}
	else
	{
		mcu_reg.state_reg.emg_stop = false;
	}

	if (op_panel.GetPressed(enOp::panel_e::SW_START))
	{
		autoManager.StartSw();
	}
	else if (op_panel.GetPressed(enOp::panel_e::SW_STOP))
	{
		mcu_reg.state_reg.auto_running = false;
		autoManager.StopSw();
	}
	else if (op_panel.GetPressed(enOp::panel_e::SW_RESET))
	{
		mcu_reg.state_reg.alarm_status = false;
		autoManager.ResetSw();
	}

}

void updateLamp()
{
	enOp::status_e op_state = autoManager.GetOPStatus();

	switch (op_state)
	{
		case enOp::INIT:
			if (mcu_reg.IsProcessInitialCplt())
			{
				autoManager.SetOPStatus(enOp::STEP_STOP);
			}
			if (millis() - lamp_ms >= 1000*2)
			{
				lamp_ms = millis();
				op_panel.LampToggle(enOp::LAMP_START);
			}
			break;

		case enOp::ERR_STOP:
			if (millis() - lamp_ms >= 300)
			{
				lamp_ms = millis();
				op_panel.LampToggle(enOp::LAMP_STOP);
			}
			op_panel.LampOnOff(enOp::LAMP_START,false);
			op_panel.LampOnOff(enOp::LAMP_RESET,false);
			mcu_reg.state_reg.error_stop = true;
			break;

		case enOp::STEP_STOP:
			op_panel.LampOnOff(enOp::LAMP_START,false);
			op_panel.LampOnOff(enOp::LAMP_STOP,true);
			op_panel.LampOnOff(enOp::LAMP_RESET,false);
			mcu_reg.state_reg.error_stop = true;
			break;

		case enOp::RUN_READY:
			if (millis() - lamp_ms >= 1000)
			{
				lamp_ms = millis();
				op_panel.LampToggle(enOp::LAMP_START);
			}
			op_panel.LampOnOff(enOp::LAMP_STOP,false);
			op_panel.LampOnOff(enOp::LAMP_RESET,false);
			mcu_reg.state_reg.auto_ready = true;
			break;

		case enOp::RUN:
			op_panel.LampOnOff(enOp::LAMP_START,true);
			mcu_reg.state_reg.auto_running = true;
			break;

		default:
			break;
	}

}

void updateErr()
{


	// Check the error status of the constructed unit
	// auto run 상태의 process-step 운영에서 발생되는 에러 정지는 포함하지 않는다
	/* if (mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_MOT])
  {
    if (errCnt  >= AP_DEF_ERROR_CNT_MAX)
    {
      errCnt = 0;
      mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_CLEAR] = false;
      refresh_time = 100;
    }
    else
    {
      errCnt++;
    }
  }
  else if (mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_LCD])
  {
    if (errCnt  >= AP_DEF_ERROR_CNT_MAX)
    {
      errCnt = 0;
      mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_CLEAR] = false;
      refresh_time = 300;
    }
    else
    {
      errCnt++;
    }
  }
  else
  {
    errCnt = 0;
    mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_CLEAR] = true;
    refresh_time = 1000;
  }


  //process 에러 체크
  if (mcu_reg.GetAlarmState() != 1)
  {
    mcu_reg.SetRunState(AP_REG_ALARM_STATUS, true);
  }
  else
  {
    mcu_reg.SetRunState(AP_REG_ALARM_STATUS, false);
  }*/


}


void updateApReg()
{
	mcu_io.Update_io();

	if (autoManager.IsDetectAreaSensor()) //
	{
		mcu_reg.state_reg.detect_safe_sensor = true;
	}
	else
	{
		mcu_reg.state_reg.detect_safe_sensor = false;
	}

}




