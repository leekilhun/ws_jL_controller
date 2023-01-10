/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "ap.h"


constexpr uint8_t COMM_RECOVERY_LIMIT_COUNT  = 10;

void updateApReg();
void updateErr();
void eventOpPanel();
void updateLamp();
void routineFunc();

std::array <uint8_t, 32> lcd_data_arry = {0,};
uint16_t refresh_time = 1000;
uint32_t loop_ms = 0;
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
link_dat linkPose_data;
ap_log mcu_log;
/****************************************************
  1. ap에서 생성할 객체
 ****************************************************/
// engine layer (물리 계층)
MOTOR::uart_moons moons_comm;


MOTOR::enMotor_moons moons_motors[AP_OBJ::MOTOR_MAX]
																{M_SetMotorId(AP_OBJ::MOTOR_JIG)
																,M_SetMotorId(AP_OBJ::MOTOR_ROLL)
																,M_SetMotorId(AP_OBJ::MOTOR_HIGH)};
ACT::enCyl cyl[AP_OBJ::CYL_MAX];
ACT::enVac vac[AP_OBJ::VAC_MAX];
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
		cfg.ptr_databuff = &lcd_data_arry;
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
		cyl_cfg.cyl_id = AP_OBJ::CYL_PHONE_OPENCLOSE;
		cyl_cfg.cyl_type = enCyl::type_e::open_close;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::phone_jig_open_close);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_jig_open;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_jig_close;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_jig_open;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_jig_close;

		cyl[AP_OBJ::CYL_PHONE_OPENCLOSE].SetConfigData(cyl_cfg);

		/*phone jig forward backward*/
		cyl_cfg.cyl_id = AP_OBJ::CYL_PHONE_FORBACK;
		cyl_cfg.cyl_type = enCyl::type_e::forward_backward;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::phone_jig_for_back);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_jig_open;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_jig_close;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_jig_open;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_jig_close;

		cyl[AP_OBJ::CYL_PHONE_FORBACK].SetConfigData(cyl_cfg);

		/*vinyl holder up down*/
		cyl_cfg.cyl_id = AP_OBJ::CYL_VINYLHOLD_UPDOWN;
		cyl_cfg.cyl_type = enCyl::type_e::up_down;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::vinylhold_up_down);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_vinylhold_up;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_vinylhold_down;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_vinylhold_up;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_vinylhold_down;

		cyl[AP_OBJ::CYL_VINYLHOLD_UPDOWN].SetConfigData(cyl_cfg);

		/*vinyl gripper for detach at roller*/
		cyl_cfg.cyl_id = AP_OBJ::CYL_VINYL_PUSHBACK;
		cyl_cfg.cyl_type = enCyl::type_e::forward_backward;
		cyl_cfg.sol_type = enCyl::two;
		cyl_cfg.pApIo = (iio *)&mcu_io;
		cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::vinyl_push_back);
		cyl_cfg.sensor_io[enCyl::dir_e::up] = MCU_IO::ap_io::in_cyl_vinyl_push;
		cyl_cfg.sensor_io[enCyl::dir_e::down] = MCU_IO::ap_io::in_cyl_vinyl_back;
		cyl_cfg.sol_io[enCyl::dir_e::up] = MCU_IO::ap_io::out_cyl_vinyl_push;
		cyl_cfg.sol_io[enCyl::dir_e::down] = MCU_IO::ap_io::out_cyl_vinyl_back;

		cyl[AP_OBJ::CYL_VINYL_PUSHBACK].SetConfigData(cyl_cfg);

	}

	/* vacuum initial */
	{
		using namespace ACT;

		/*drum head vacuum*/
		enVac::cfg_t vac_cfg = { 0, };
		vac_cfg.vac_id = AP_OBJ::VAC_DETECT;
		vac_cfg.vac_type = enVac::type_e::suction_blow;
		vac_cfg.pApIo = (iio *)&mcu_io;
		vac_cfg.pVacDat = vac_data.GetData(vac_dat::addr_e::vinyl_detect);
		vac_cfg.sensor_io = MCU_IO::ap_io::in_grip_vinyle_detect;
		vac_cfg.sol_io[enVac::on_suction] = MCU_IO::ap_io::out_vac_detect_on;
		vac_cfg.sol_io[enVac::on_blow] = MCU_IO::ap_io::out_vac_detect_blow;

		vac[AP_OBJ::VAC_DETECT].SetConfigData(vac_cfg);
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
		cfg.p_linkPosDat = &linkPose_data;
		tasks.Init(cfg);
	}

	/* sequence process initial */
	{

	}

	/*user display object*/
	{
		api_lcd::cfg_t cfg{};
		cfg.ptr_comm = &nextion_lcd;
		cfg.ptr_mcu_reg = &mcu_reg;
		cfg.ptr_task = &tasks;
		cfg.ptr_io = &mcu_io;
		cfg.ptr_cyl_data = &cyl_data;
		cfg.ptr_vac_data = &vac_data;
		cfg.ptr_sequence_data = &seq_data;
		cfg.ptr_log = &mcu_log;
		cfg.ptr_auto =&autoManager;

		op_lcd.Init(cfg);
	}

	/*remote control and monitor*/
	{
		api_remote::cfg_t cfg{};
		cfg.ptr_comm = &remote_ctrl;
		cfg.ptr_mcu_reg = &mcu_reg;
		cfg.ptr_task = &tasks;
		cfg.ptr_io = &mcu_io;
		cfg.ptr_cyl_data = &cyl_data;
		cfg.ptr_vac_data = &vac_data;
		cfg.ptr_sequence_data = &seq_data;
		cfg.ptr_linkPose_data = &linkPose_data;
		cfg.ptr_log = &mcu_log;
		cfg.ptr_motors = &motors;
		cfg.ptr_auto =&autoManager;
		cfg.ptr_axis_data = &axis_data;
		cfg.ptr_cfg_data = &apCfg_data;

		remote_pc.Init(cfg);
	}



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

	linkPose_data.LoadRomData();

	/****************************/
	//mcu_reg.status[AP_REG_BANK_SETTING][AP_REG_USE_BEEP] = true;
	mcu_reg.option_reg.use_beep = true;
	/****************************/

	/****************************/
	//process.SetAutoSpeed(seq_data.GetMaxSpeed());
	/****************************/
	/*Assign Obj */
	mcu_io.Init();

}


void apMain(void)
{
	uint32_t pre_main_ms = millis();
	uint32_t pre_loop = millis();
	//uint8_t io_idx = 0;
	while (1)
	{
		loop_ms = millis() - pre_loop;
		pre_loop = millis();

		/* set led blink time */
		if (mcu_reg.error_reg.no_error)
			refresh_time = 1000;
		else
			refresh_time =300;
		if (millis() - pre_main_ms >= refresh_time)
		{
			//mcu_io.OutputToggle(MCU_IO::ap_io::out_cyl_jig_for + (io_idx++ % 16));

			ledToggle(_DEF_LED1);
			pre_main_ms = millis();
		}

		// non-block코드.
		motors.ThreadJob();

		// non-block코드.
		op_lcd.ThreadJob();

		// non-block코드
		remote_pc.ThreadJob();
		//remote_ctrl.ReceiveProcess();

		eventOpPanel();

		updateErr();
		updateApReg();
		updateLamp();
		routineFunc();

	}

}



/*
 * operating sw 이벤트 처리
 */
void eventOpPanel()
{
	/*check request initial*/
	if (mcu_reg.state_reg.request_initial)
	{
		mcu_reg.state_reg.request_initial = false;
		tasks.Initialize();
	}
	/* op key*/
	if (op_panel.GetPressed(enOp::panel_e::SW_ESTOP))
	{
		mcu_reg.SetReg_State(MCU_REG::ap_reg::state_e::EMG_STOP, true);
		return;
	}
	else
	{
		mcu_reg.SetReg_State(MCU_REG::ap_reg::state_e::EMG_STOP, false);
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


/*
 * operating 램프
 */
void updateLamp()
{

	switch (autoManager.GetOPStatus())
	{
		case enOp::status_e::INIT:
		{
			if (mcu_reg.IsProcessInitialCplt())
			{
				autoManager.SetOPStatus(enOp::STEP_STOP);
			}

			if (millis() - lamp_ms >= 1000*2)
			{
				lamp_ms = millis();
				op_panel.LampToggle(enOp::LAMP_START);
			}
		}
		break;

		case enOp::status_e::STEP_STOP:
		{
			op_panel.LampOnOff(enOp::LAMP_START,false);
			op_panel.LampOnOff(enOp::LAMP_STOP,true);
			op_panel.LampOnOff(enOp::LAMP_RESET,false);
		}
		break;

		case enOp::status_e::RUN_READY:
		{
			if (millis() - lamp_ms >= 1000)
			{
				lamp_ms = millis();
				op_panel.LampToggle(enOp::LAMP_START);
			}
			op_panel.LampOnOff(enOp::LAMP_STOP,false);
			op_panel.LampOnOff(enOp::LAMP_RESET,false);
		}
		break;

		case enOp::status_e::RUN:
		{
			op_panel.LampOnOff(enOp::LAMP_START,true);
			switch (autoManager.GetOPMode())
			{
				case enOp::mode_e::AUTORUN:
					break;

				case enOp::mode_e::DRY_RUN:
					break;

				case enOp::mode_e::READY:
					break;

				case enOp::mode_e::STOP:
					break;

				default:
					break;
			}
			// end of switch (autoManager.GetOPMode())
		}
		break;

		case enOp::status_e::ERR_STOP:
		{
			if (millis() - lamp_ms >= 300)
			{
				lamp_ms = millis();
				op_panel.LampToggle(enOp::LAMP_STOP);
			}
			op_panel.LampOnOff(enOp::LAMP_START,false);
			op_panel.LampOnOff(enOp::LAMP_RESET,false);

		}
		break;

		default:
			break;
	}
	// end of switch (autoManager.GetOPStatus())
}




/*
 * mcu register update
 */
void updateApReg()
 {
	 /* 1. io register */
	 mcu_io.Update_io();

	 /* 2. mcu register */
	 using reg = MCU_REG::ap_reg::state_e;

	 mcu_reg.state_reg.detect_safe_sensor = autoManager.IsDetectAreaSensor();
	 mcu_reg.state_reg.motor_on = motors.IsMotorOn();
	 mcu_reg.state_reg.system_origin_cplt = !(tasks.m_IsInit);


	 /* mode */
	 switch (autoManager.GetOPStatus())
	 {
		 case enOp::status_e::INIT:
		 {

		 }
		 break;

		 case enOp::status_e::STEP_STOP:
		 {
			 mcu_reg.SetReg_State(reg::AUTO_STOP, true);
			 //mcu_reg.state_reg.error_stop = true;
		 }
		 break;

		 case enOp::status_e::RUN_READY:
		 {
			 mcu_reg.SetReg_State(reg::AUTO_READY, true);
			 //mcu_reg.state_reg.auto_ready = true;
		 }
		 break;

		 case enOp::status_e::RUN:
		 {
			 mcu_reg.SetReg_State(reg::AUTO_RUNNING, true);
			 //mcu_reg.state_reg.auto_running = true;
			 switch (autoManager.GetOPMode())
			 {
				 case enOp::mode_e::AUTORUN:
					 break;

				 case enOp::mode_e::DRY_RUN:
					 break;

				 case enOp::mode_e::READY:
					 break;

				 case enOp::mode_e::STOP:
					 break;

				 default:
					 break;
			 }
		 }
		 // end of switch (autoManager.GetOPMode())
		 break;

		 case enOp::status_e::ERR_STOP:
		 {
			 mcu_reg.SetReg_State(reg::AUTO_STOP, true);
			 //mcu_reg.state_reg.error_stop = true;
		 }
		 break;

		 default:
			 break;
	 }
	 // end of switch (autoManager.GetOPStatus())

 }



/*
 * system error register, communication error and try to recovery
 */
 void updateErr()
 {

	 /* 1. error register */

	 //using state_reg = MCU_REG::ap_reg::state_e;
	 using error_reg = MCU_REG::ap_reg::err_e;

	 //process 에러 체크
	 /*if (mcu_reg.GetAlarmState() != 1)
		 mcu_reg.SetReg_State(state_reg::ALARM_STATUS, true);
	 else
		 mcu_reg.SetReg_State(state_reg::ALARM_STATUS, false);*/
	 mcu_reg.error_reg.no_error = !(mcu_reg.GetAlarmState() > 1);

   /* 2. lcd  통신 체크  */
	 if (op_lcd.IsConnected() == false)
	 {
		 if (nextion_lcd.GetErrCnt() >= COMM_RECOVERY_LIMIT_COUNT)
		 {
			 mcu_reg.SetErrRegister(error_reg::no_response_lcd, true);
			 op_lcd.CommRecovery();
		 }
	 }
	 else
	 {
		 mcu_reg.SetErrRegister(error_reg::no_response_lcd, false);
	 }


	 /* 3. motor communication */
	 //mcu_reg.error_reg.no_resp_mot

	 if (motors.GetCommStatus() == 0)
	 {
		 motors.m_errCnt = 0;
		 mcu_reg.SetErrRegister(error_reg::no_response_mot, false);
	 }
	 else
	 {
		 // recovery는 motors객체에서 전체 모터 연결이 안되면 실시
			mcu_reg.SetErrRegister(error_reg::no_response_mot, (++motors.m_errCnt >= COMM_RECOVERY_LIMIT_COUNT));
	 }

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



 void routineFunc()
 {
	 static uint32_t time_ms = 0;
	 if (millis() - time_ms >= 1000)
	 {
		 time_ms = millis();
		 //op_lcd.ChangePage(NXLCD::uart_nextion::page_e::MAIN);
	 }

 }




