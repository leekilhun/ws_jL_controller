/*
 * cnTasks.hpp
 *
 *  Created on: 2022. 12. 4.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_CNTASKS_HPP_
#define SRC_AP__INC_CNTASKS_HPP_

/*
   cnTasks  [manual 동작 완성]
      |
      |----------------
      |      |        |
   enCyl  enMotor   enVac

 */

class cnTasks
{
	/****************************************************
	 *  data
	 ****************************************************/
public:
	struct cfg_t
	{
		MCU_REG::ap_reg *p_apReg{};
		iio *p_apIo{};
		axis_dat *p_apAxisDat{};
		cyl_dat *p_apCylDat{};
		vac_dat *p_apVacDat{};
		ap_dat *p_apCfgDat{};
		seq_dat *p_apSeqDat{};
		enOp* p_Op{};
		ACT::enCyl *p_Cyl{};
		ACT::enVac *p_Vac{};
		cnMotors *p_motors{};
		cnAuto *p_AutoManger;

		cfg_t() = default;
		inline cfg_t& operator = (const cfg_t& cfg)
		{
			if (this != & cfg){
				p_apReg = cfg.p_apReg;
				p_apIo = cfg.p_apIo;
				p_apAxisDat = cfg.p_apAxisDat;
				p_apCylDat = cfg.p_apCylDat;
				p_apVacDat = cfg.p_apVacDat;
				p_apCfgDat = cfg.p_apCfgDat;
				p_apSeqDat = cfg.p_apSeqDat;
				p_Op = cfg.p_Op;
				p_Cyl = cfg.p_Cyl;
				p_Vac = cfg.p_Vac;
				p_motors = cfg.p_motors;
				p_AutoManger = cfg.p_AutoManger;
			}
			return *this;
		}

	};

private:
	cfg_t m_cfg;

public:

	/****************************************************
	 *  Constructor
	 ****************************************************/
public:
	cnTasks():m_cfg{}{

	};
	virtual ~cnTasks(){};

	/****************************************************
	 *  func
	 ****************************************************/
private:
	// for auto run
	void doRunStep();

public:
	inline int Init(cnTasks::cfg_t &cfg){
		m_cfg = cfg;
		return ERROR_SUCCESS;
	}

	inline errno_t Initialize(){
		return ERROR_SUCCESS;
	}

	inline errno_t MotorOrigin(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
		errno_t ret = ERROR_SUCCESS;
		if (m_cfg.p_AutoManger->IsDetectAreaSensor())
			return -1;
		if (m_cfg.p_motors->IsMotorOn(motor_id) == false)
			return -2;

		if (m_cfg.p_motors->Origin(motor_id) != OK)
		{
			ret = -3;
		}
		return ret;
	}

	inline errno_t MotorOnOff(bool on_off, AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){

		return ERROR_SUCCESS;
	}

	inline bool IsMotorOn(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
		return false;
	}

	inline bool IsMotorRun(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
		return false;
	}

	inline bool IsMotorMoveCplt(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
		return false;
	}

	inline errno_t MotorStop (AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
		return ERROR_SUCCESS;
	}

	/* only check inpose or busy state */
	inline errno_t MotorRun(AP_OBJ::MOTOR motor_id, int cmd_pos){

		return ERROR_SUCCESS;
	}

	inline errno_t RelMove(AP_OBJ::MOTOR motor_id, int cmd_dist){

		return ERROR_SUCCESS;
	}

	// is floating state
	inline bool IsGrip(AP_OBJ::CYL cyl_id){
		bool ret = false;
		ret |= m_cfg.p_Cyl[cyl_id].IsOpen();
		ret |= m_cfg.p_Cyl[cyl_id].IsClose();
		return !(ret);
	}

	inline errno_t Grip(AP_OBJ::CYL cyl_id){
		errno_t ret = ERROR_SUCCESS;
		ret = m_cfg.p_Cyl[cyl_id].Close(true);
		uint32_t pre_ms = millis();
		constexpr int timeout = 100;
		while (!IsGrip(cyl_id))
		{
			if ( millis()-pre_ms >timeout)
			{
				uint8_t err = (uint8_t)(cnAuto::state_e::cyl_PHONE_JIG_close_timeout + cyl_id);
				m_cfg.p_AutoManger->AlarmAuto(static_cast<cnAuto::state_e>(err));
				ret = -1;
				break;
			}
		}
		return ret;
	}

	inline errno_t CylOpen(AP_OBJ::CYL cyl_id, bool skip_sensor = false){
		errno_t ret = ERROR_SUCCESS;
		ret = m_cfg.p_Cyl[cyl_id].Open(skip_sensor);
		if (ret != 0)
		{
			uint8_t err = (uint8_t)(cnAuto::state_e::cyl_PHONE_JIG_open_timeout + cyl_id);
			m_cfg.p_AutoManger->AlarmAuto(static_cast<cnAuto::state_e>(err));
			ret = -1;
		}
		return ret;
	}

	inline errno_t CylClose(AP_OBJ::CYL cyl_id, bool skip_sensor = false){
		errno_t ret = ERROR_SUCCESS;
		ret = m_cfg.p_Cyl[cyl_id].Close(skip_sensor);
		if (ret != 0)
		{
			uint8_t err = (uint8_t)(cnAuto::state_e::cyl_PHONE_JIG_close_timeout + cyl_id);
			m_cfg.p_AutoManger->AlarmAuto(static_cast<cnAuto::state_e>(err));
			ret = -1;
		}
		return ret;
	}

	inline errno_t VacOn(AP_OBJ::VAC vac_id, bool skip_sensor = false){
		errno_t ret = ERROR_SUCCESS;
		ret = m_cfg.p_Vac[vac_id].On(skip_sensor);
		if (ret != 0)
		{
			uint8_t err = (uint8_t)(cnAuto::state_e::vac_PHONE_JIG_on_timeout + vac_id);
			m_cfg.p_AutoManger->AlarmAuto(static_cast<cnAuto::state_e>(err));
			ret = -1;
		}
		return ret;
	}

	inline errno_t VacOff(AP_OBJ::VAC vac_id, bool skip_sensor = false){
		errno_t ret = ERROR_SUCCESS;
		ret = m_cfg.p_Vac[vac_id].Off(skip_sensor);
		if (ret != 0)
		{
			uint8_t err = (uint8_t)(cnAuto::state_e::vac_PHONE_JIG_off_timeout + vac_id);
			m_cfg.p_AutoManger->AlarmAuto(static_cast<cnAuto::state_e>(err));
			ret = -1;
		}
		return ret;
	}







};



#endif /* SRC_AP__INC_CNTASKS_HPP_ */
