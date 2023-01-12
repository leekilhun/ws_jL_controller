/*
 * cnMotors.hpp
 *
 *  Created on: 2022. 12. 4.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_CNMOTORS_HPP_
#define SRC_AP__INC_CNMOTORS_HPP_

namespace MOTOR
{

	constexpr uint8_t CN_MOTORS_VELOCITY_RPS_MAX  = 50;

	class cnMotors
	{
		/****************************************************
		 *  data
		 ****************************************************/
	public:
		struct cfg_t  {
			axis_dat* p_apAxisDat;
			MCU_REG::ap_reg* p_apReg;
			MOTOR::enMotor_moons *p_motor;
			MOTOR::uart_moons *p_comm;
			iio *p_apIo;
			ap_dat* p_apCfgDat;

			cfg_t() = default;

			inline cfg_t& operator = (const cfg_t& cfg)
			{
				if (this != & cfg){
					p_apReg = cfg.p_apReg;
					p_apIo = cfg.p_apIo;
					p_apAxisDat = cfg.p_apAxisDat;
					p_apCfgDat = cfg.p_apCfgDat;
					p_motor = cfg.p_motor;
					p_comm = cfg.p_comm;
				}
				return *this;
			}
		};

   union CN_MOTORS_COMM
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

	private:
		cfg_t m_cfg;
		uint8_t m_requestMotor_idx;
		uint32_t m_request_ms;
		CN_MOTORS_COMM m_commStatus;
		uint8_t m_motorCommErrCnt[AP_OBJ::MOTOR_MAX];

	public:
		prc_step_t m_step;
		uint8_t m_errCnt;
		/****************************************************
		 *  Constructor
		 ****************************************************/
	public:
		cnMotors () : m_cfg{},m_requestMotor_idx{}, m_request_ms{}
		,m_commStatus{}, m_motorCommErrCnt{}
		,m_step{}, m_errCnt{}
		{

		}

		virtual  ~cnMotors (){}
		/****************************************************
		 *  func
		 ****************************************************/
	private:
		void doRunStep();

	public:
		inline int Init(cnMotors::cfg_t &cfg)  {
			m_cfg = cfg;
			return 0;
		}

		inline bool Recovery() {
			m_commStatus.comm_err = 0;

			m_cfg.p_motor[AP_OBJ::MOTOR_JIG].Recovery();
			m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].Recovery();
			m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].Recovery();

			return m_cfg.p_comm->Recovery();
		}

		inline bool IsConnected() {
			constexpr uint8_t rimit_err_cnt = 10;
			if(m_cfg.p_comm->IsConnected())
				return  (m_cfg.p_comm->GetErrCnt() > rimit_err_cnt) ? false: true;
			return false;
		}

		inline uint8_t GetCommStatus() const{
			return m_commStatus.comm_err;
		}

		inline MOTOR::enMotor_moons* GetMotorObject(AP_OBJ::MOTOR motor_id){
			if (motor_id < AP_OBJ::MOTOR::MOTOR_MAX)
				return &m_cfg.p_motor[motor_id];
			return nullptr;
		}

		// step machine을 통해 nonblock으로 처리된다.
		void ThreadJob();

		void UpdateMotorsState();

		inline int SetParamDataMove(AP_OBJ::MOTOR motor_id, uint32_t vel, uint32_t acc, uint32_t dec){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].SetParamDataMove(constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].SetParamDataMove(constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].SetParamDataMove(constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				return ret;
			}
			return m_cfg.p_motor[motor_id].SetParamDataMove(constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
		}

		inline int SetParamDataInit(AP_OBJ::MOTOR motor_id, MOTOR::enMotor_moons::origin_param_t& param){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].SetOriginParam(param);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].SetOriginParam(param);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].SetOriginParam(param);
				return ret;
			}
			return m_cfg.p_motor[motor_id].SetOriginParam(param);
		}

		inline bool IsLinkMoveCplt(){
			bool ret = true;
			ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsInPose()
					 &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsInPose();
			return ret;
		}
		inline int LinkMove(int cmd_pos, uint32_t vel){
			int ret = 0;
			ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveRelative( cmd_pos, constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), 100, 100);
			ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveRelative( cmd_pos, constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), 100, 100);

			return ret;
		}

		inline int Move(AP_OBJ::MOTOR motor_id, int cmd_pos){
			/*must assign motor id*/
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveAbsolutive(cmd_pos);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveAbsolutive(cmd_pos);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveAbsolutive(cmd_pos);
				return ret;
			}
			return m_cfg.p_motor[motor_id].MoveAbsolutive(cmd_pos);
		}
		inline int Move(AP_OBJ::MOTOR motor_id, int cmd_pos, uint32_t vel, uint32_t acc = 100, uint32_t dec = 100){
			/*must assign motor id*/
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveAbsolutive( cmd_pos, constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveAbsolutive( cmd_pos, constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveAbsolutive( cmd_pos, constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				return ret;
			}
			return m_cfg.p_motor[motor_id].MoveAbsolutive( cmd_pos, constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
		}

		inline int RelMove(AP_OBJ::MOTOR motor_id, int cmd_pos){
			/*must assign motor id*/
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveRelative(cmd_pos);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveRelative(cmd_pos);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveRelative(cmd_pos);
				return ret;
			}
			return m_cfg.p_motor[motor_id].MoveRelative(cmd_pos);
		}

		inline int RelMove(AP_OBJ::MOTOR motor_id, int cmd_pos, uint32_t vel, uint32_t acc = 100, uint32_t dec = 100){
			/*must assign motor id*/
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveRelative( cmd_pos,constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveRelative( cmd_pos,constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveRelative( cmd_pos,constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
				return ret;
			}
			return m_cfg.p_motor[motor_id].MoveAbsolutive( cmd_pos, constrain(vel,1,CN_MOTORS_VELOCITY_RPS_MAX), acc, dec);
		}

		inline int Origin(AP_OBJ::MOTOR motor_id =AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].OriginMotor();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].OriginMotor();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].OriginMotor();
				return ret;
			}
			return m_cfg.p_motor[motor_id].OriginMotor();
		}


		inline int MotorOnOff(bool on_off = true, AP_OBJ::MOTOR motor_id =AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MotorOnOff(on_off);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MotorOnOff(on_off);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MotorOnOff(on_off);
				return ret;
			}
			return m_cfg.p_motor[motor_id].MotorOnOff(on_off);
		}

		inline int MotorClearAlarm(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].ClearState();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].ClearState();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].ClearState();
				return ret;
			}
			return m_cfg.p_motor[motor_id].ClearState();
		}

		inline int MotorZeroEncode(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].ClearEncoder();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].ClearEncoder();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].ClearEncoder();
				return ret;
			}
			return m_cfg.p_motor[motor_id].ClearEncoder();
		}

		inline int MotorJogMoveStop(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].JogStop();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].JogStop();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].JogStop();
				return ret;
			}
			return m_cfg.p_motor[motor_id].JogStop();
		}

		inline int MotorMoveStop(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveStop();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveStop();
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveStop();
				return ret;
			}
			return m_cfg.p_motor[motor_id].MoveStop();
		}

		inline int MotorJogMove(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX, bool is_cw = true){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].JogMove(is_cw);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].JogMove(is_cw);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].JogMove(is_cw);
				return ret;
			}
			return m_cfg.p_motor[motor_id].JogMove(is_cw);
		}

		inline int MotorJogMove(AP_OBJ::MOTOR motor_id, uart_moons::speed_t& param, bool is_cw){
			param.speed = constrain(param.speed,1,CN_MOTORS_VELOCITY_RPS_MAX);
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				int ret = 0;
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].JogMove(param,is_cw);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].JogMove(param,is_cw);
				ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].JogMove(param,is_cw);
				return ret;
			}
			return m_cfg.p_motor[motor_id].JogMove(param,is_cw);
		}


		inline bool IsConnected() const{
			return m_cfg.p_comm->IsConnected();
		}

		inline bool IsMotorOn(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				bool ret = true;
				ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsMotorOn()
				     &m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsMotorOn()
						 &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsMotorOn();
				return ret;
			}
			return m_cfg.p_motor[motor_id].IsMotorOn();
		}

		inline bool IsMotorRun(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
			{
				bool ret = true;
				ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsMotorRun()
					   &m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsMotorRun()
						 &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsMotorRun();
				return ret;
			}
			return m_cfg.p_motor[motor_id].IsMotorRun();
		}

		inline bool IsInPose(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
			if (motor_id == AP_OBJ::MOTOR_MAX)
						{
							bool ret = true;
							ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsInPose()
								   &m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsInPose()
									 &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsInPose();
							return ret;
						}
						return m_cfg.p_motor[motor_id].IsInPose();
		}


#if 0
		inline bool IsMotorOn(uint8_t id = static_cast<uint8_t>(cnMotors::mot_Id::_max)){
			bool ret = false;

			if (id < static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[id].IsMotorOn();
			}
			else if (id == static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_JIG].IsMotorOn();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_ROLL].IsMotorOn();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_HIGH].IsMotorOn();
			}
			return ret;
		}

		inline bool IsOriginOK(uint8_t id = static_cast<uint8_t>(cnMotors::mot_Id::_max)){
			bool ret = false;

			if (id < static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[id].IsOriginOK();
			}
			else if (id == static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_JIG].IsOriginOK();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_ROLL].IsOriginOK();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_HIGH].IsOriginOK();
			}
			return ret;
		}

		inline bool IsBusy(uint8_t id = static_cast<uint8_t>(cnMotors::mot_Id::_max)){
			bool ret = false;

			if (id < static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[id].IsBusy();
			}
			else if (id == static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_JIG].IsBusy();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_ROLL].IsBusy();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_HIGH].IsBusy();
			}
			return ret;
		}

		inline bool IsInpose(uint8_t id = static_cast<uint8_t>(cnMotors::mot_Id::_max)){
			/*must assign motor id*/
			bool ret = false;
			if (id < static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[id].IsInpose();
			}
			else
			{
				ret = m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_JIG].IsInpose();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_ROLL].IsInpose();
				ret &= m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_HIGH].IsInpose();
			}
			return ret;
		}

		inline int EStop (uint8_t id = static_cast<uint8_t>(cnMotors::mot_Id::_max)){
			int ret = 0;
			if (id < static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[id].EStop();
			}
			else if (id == static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_JIG].EStop();
				ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_ROLL].EStop();
				ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_HIGH].EStop();

			}
			return ret;
		}


		inline int Stop (uint8_t id = static_cast<uint8_t>(cnMotors::mot_Id::_max)){
			int ret = 0;
			if (id < static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret = m_Cfg.p_motor[id].Stop();
			}
			else if (id == static_cast<uint8_t>(cnMotors::mot_Id::_max))
			{
				ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_JIG].Stop();
				ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_ROLL].Stop();
				ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_HIGH].Stop();
			}
			return ret;
		}


#endif


	};
}// end of namespace MOTOR



#endif /* SRC_AP__INC_CNMOTORS_HPP_ */
