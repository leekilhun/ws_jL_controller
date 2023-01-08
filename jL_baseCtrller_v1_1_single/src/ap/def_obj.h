/*
 * def_obj.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_AP_DEF_OBJ_H_
#define SRC_AP_DEF_OBJ_H_


#define AP_DEF_OBJ_BASE_MOTOR                   0x0010
#define AP_DEF_OBJ_BASE_CYLINDER                0x0100
#define AP_DEF_OBJ_BASE_VACUUM                  0x0120

#define AP_DEF_OBJ_BASE_IO                      0x0140
#define AP_DEF_START_IN_ADDR                    1000
#define AP_DEF_START_OUT_ADDR                   3000

#define AP_DEF_OBJ_MOTOR_ID_BASE                1
#define AP_DEF_OBJ_MOTOR_ID_JIG                 0
#define AP_DEF_OBJ_MOTOR_ID_ROLL                1
#define AP_DEF_OBJ_MOTOR_ID_HIGH                2
#define AP_DEF_OBJ_MOTOR_ID_MAX                 3
#define M_SetMotorId(instance_no)               AP_DEF_OBJ_MOTOR_ID_BASE + instance_no
#define M_GetMotorInstanceId(node_id)           node_id - AP_DEF_OBJ_MOTOR_ID_BASE
#define M_GetMotorObjId(obj)                    AP_DEF_OBJ_BASE_MOTOR|obj

#define AP_DEF_OBJ_CYL_ID_PHONE_OPEN_CLOSE      0
#define AP_DEF_OBJ_CYL_ID_PHONE_FOR_BACK        1
#define AP_DEF_OBJ_CYL_ID_VINYLHOLD_UP_DOWN     2
#define AP_DEF_OBJ_CYL_ID_VINYL_PUSH_BACK       3
#define AP_DEF_OBJ_CYL_ID_MAX                   4
#define M_GetCylinderObjId(obj)              AP_DEF_OBJ_BASE_CYLINDER|obj


#define AP_DEF_OBJ_VACUUM_ID_VINYL_DETECT       0
#define AP_DEF_OBJ_VACUUM_ID_MAX                1
#define GetVacObjId(obj)                     AP_DEF_OBJ_BASE_VACUUM|obj



namespace AP_OBJ
{
	enum MOTOR
	{
		MOTOR_JIG,
		MOTOR_ROLL,
		MOTOR_HIGH,
		MOTOR_MAX,

	};

	enum CYL
	{
		CYL_PHONE_OPENCLOSE,
		CYL_PHONE_FORBACK,
		CYL_VINYLHOLD_UPDOWN,
		CYL_VINYL_PUSHBACK,

		CYL_MAX,

	};

	enum VAC
	{
		VAC_DETECT,

		VAC_MAX,
	};

}














#endif /* SRC_AP_DEF_OBJ_H_ */
