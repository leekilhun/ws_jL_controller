/*
 * cnMotors.hpp
 *
 *  Created on: 2022. 12. 4.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_CNMOTORS_HPP_
#define SRC_AP__INC_CNMOTORS_HPP_




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


private:
  cfg_t m_cfg;


  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnMotors () : m_cfg{}{

  }

  virtual  ~cnMotors (){}
  /****************************************************
   *  func
   ****************************************************/
private:

public:
  inline int Init(cnMotors::cfg_t &cfg)  {
    m_cfg = cfg;
    return 0;
  }

  inline bool Recovery() {
    m_cfg.p_motor[AP_OBJ::MOTOR_JIG].Recovery();
    m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].Recovery();
    m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].Recovery();

    return m_cfg.p_comm->Recovery();
  }

  inline void UpdateMotorsState(){

  }

  inline int Move(AP_OBJ::MOTOR motor_id, int cmd_pos){
    /*must assign motor id*/
    int ret = 0;
    return ret;
  }

  inline int RelMove(AP_OBJ::MOTOR motor_id, int cmd_pos){
    /*must assign motor id*/
    int ret = 0;
    return ret;
  }

  inline int Origin(AP_OBJ::MOTOR motor_id =AP_OBJ::MOTOR_MAX){
    int ret = 0;//enFaxis::err_e::success;

    return ret;
  }


  inline int MotorOnOff(bool on_off = true, AP_OBJ::MOTOR motor_id =AP_OBJ::MOTOR_MAX){
    int ret = 0;

    return ret;
  }


  inline bool IsConnected() const{
    return m_cfg.p_comm->IsConnected();
  }

  inline bool IsMotorOn(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
  		return false;
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




#endif /* SRC_AP__INC_CNMOTORS_HPP_ */
