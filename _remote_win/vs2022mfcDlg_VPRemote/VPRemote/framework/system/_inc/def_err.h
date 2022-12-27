#pragma once


constexpr errno_t  ERR_SUCCESS = 0;
constexpr errno_t  ERR_FAIL = -1;

using namespace AP_OBJ;

auto def_err_code = [](auto obj, auto inst, auto code)
{
	return ((static_cast<int>(obj) * 1'000'000) + (static_cast<int>(inst) * 1'000) + code);
};


auto def_err_ret_obj = [](auto err_code)
{
	std::array<int, 3> ret{0,};
	auto temp = err_code;

	div_t errorcode = div(temp, 1'000);
	ret[0] = errorcode.rem;
	temp = (temp - errorcode.rem)/1'000;
	errorcode = div(temp, 1'000);
	ret[1] = errorcode.rem;
	temp = (temp - errorcode.rem) / 1'000;
	ret[2] = temp;

	return ret;
};


/************************************************************************/
/* IO Module								                            */
/************************************************************************/
// IO Error(Component Code : 100)
constexpr errcode ERR_IO_MODULE_OPEN						= def_err_code(ObjType::HL_IO, 0 , 1);//(static_cast<int>(ObjType::HL_IO) * 1000) + 1;
constexpr errcode ERR_IO_OUTPUT_OUT_OF_RANGE				= def_err_code(ObjType::HL_IO, 0 , 2);
constexpr errcode ERR_IO_NO_MATCH							= def_err_code(ObjType::HL_IO, 0,  3);
constexpr errcode ERR_IO_MODULE_IRQ_SET						= def_err_code(ObjType::HL_IO, 0,  4);
constexpr errcode ERR_IO_MODULE_NO_RESPONSE					= def_err_code(ObjType::HL_IO, 0,  5);
constexpr errcode ERR_IO_INVALID_ADDRESS_NUMVER				= def_err_code(ObjType::HL_IO, 0,  6);



/************************************************************************/
/* Cyliner Module								                        */
/************************************************************************/
// Cylinder Error(Component Code : 101)
constexpr errcode ERR_CYLINDER_MOVE_TIMEOUT	    			= def_err_code(ObjType::HL_CYLINDER, CYLINDER_OBJ::_max, 1); // Cylinder 동작시간을 초과하였습니다.
constexpr errcode ERR_CYLINDER_INVALID_POSITION				= def_err_code(ObjType::HL_CYLINDER, CYLINDER_OBJ::_max, 2);
constexpr errcode ERR_CYLINDER_FAIL_LOADING_PARAM			= def_err_code(ObjType::HL_CYLINDER, CYLINDER_OBJ::_max, 3);
constexpr errcode ERR_CYLINDER_FAIL_SAVING_PARAM			= def_err_code(ObjType::HL_CYLINDER, CYLINDER_OBJ::_max, 4);
constexpr errcode ERR_CYLINDER_IO_NOT_ASSIGN				= def_err_code(ObjType::HL_CYLINDER, CYLINDER_OBJ::_max, 5); // io 객체가 assign 되지 않았습니다


#if 1


/************************************************************************/
/* Vacuum Module								                        */
/************************************************************************/
// Vacuum Module(Component Code : 102)
constexpr errcode ERR_VACUUM_MISS_MATCH_IN_OUT_SIGNAL		= def_err_code(ObjType::HL_VACUUM, VACUUM_OBJ::_max, 0);
constexpr errcode ERR_VACUUM_TIMEOUT						= def_err_code(ObjType::HL_VACUUM, VACUUM_OBJ::_max, 1);
constexpr errcode ERR_VACUUM_FAIL_IN_LOADING_PARAM			= def_err_code(ObjType::HL_VACUUM, VACUUM_OBJ::_max, 2);
constexpr errcode ERR_VACUUM_FAIL_IN_SAVING_PARAM			= def_err_code(ObjType::HL_VACUUM, VACUUM_OBJ::_max, 3);
constexpr errcode ERR_VACUUM_IO_NOT_ASSIGN					= def_err_code(ObjType::HL_VACUUM, VACUUM_OBJ::_max, 4);





/************************************************************************/
/* Motion Axis Module			   		                                */
/************************************************************************/
// Motion Axis Error(Component Code : 105)

constexpr errcode ERR_MAXIS_AMP_FAULT						= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 8);    
constexpr errcode ERR_MAXIS_TIMEOUT_MOVE_TIME				= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 21);   // Motion Time Out Error, 모터가 정해진 시간동안 동작하지 않습니다.
constexpr errcode ERR_MAXIS_NO_EXIST_MOTION_LIBRARY_OBJECT	= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 24);   // ERR_MAXES_NO_EXIST_MOTION_LIBRARY_OBJECT
constexpr errcode ERR_MAXIS_INVALID_POINTER					= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 28);   // ERR_MAXES_INVALID_POINTER
constexpr errcode ERR_MAXIS_INVALID_AXIS_ID					= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 34);   // ERR_MAXES_INVALID_AXIS_ID
constexpr errcode ERR_MAXIS_HOME_SWITCH_AXIS_SOURCE			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 36);   // ERR_MAXES_HOME_SWITCH_AXIS_SOURCE
constexpr errcode ERR_MAXIS_POSITIVE_LIMIT_AXIS_SOURCE		= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 37);   // ERR_MAXES_POSITIVE_LIMIT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_NEGATIVE_LIMIT_AXIS_SOURCE		= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 38);   // ERR_MAXES_NEGATIVE_LIMIT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_AMP_FAULT_AXIS_SOURCE			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 39);   // ERR_MAXES_AMP_FAULT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_ACCELERATE_LIMIT_AXIS_SOURCE	= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 40);   // ERR_MAXES_ACCELERATE_LIMIT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_VELOCITY_LIMIT_AXIS_SOURCE		= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 41);   // ERR_MAXES_VELOCITY_LIMIT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_X_NEGATIVE_LIMIT_AXIS_SOURCE	= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 42);   // ERR_MAXES_X_NEGATIVE_LIMIT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_X_POSITIVE_LIMIT_AXIS_SOURCE	= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 43);   // ERR_MAXES_X_POSITIVE_LIMIT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_ERROR_LIMIT_AXIS_SOURCE			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 44);   // ERR_MAXES_ERROR_LIMIT_AXIS_SOURCE
constexpr errcode ERR_MAXIS_PC_COMMAND_AXIS_SOURCE			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 45);   // ERR_MAXES_PC_COMMAND_AXIS_SOURCE
constexpr errcode ERR_MAXIS_OUT_OF_FRAMES_AXIS_SOURCE		= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 46);   // ERR_MAXES_OUT_OF_FRAMES_AXIS_SOURCE
constexpr errcode ERR_MAXIS_AMP_POWER_ON_OFF_AXIS_SOURCE	= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 47);   // ERR_MAXES_AMP_POWER_ON_OFF_AXIS_SOURCE
constexpr errcode ERR_MAXIS_RUN_STOP_COMMAND_AXIS_SOURCE	= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 48);   // ERR_MAXES_RUN_STOP_COMMAND_AXIS_SOURCE
constexpr errcode ERR_MAXIS_COLLISION_STATE_AXIS_SOURCE		= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 49);   // ERR_MAXES_COLLISION_STATE_AXIS_SOURCE
constexpr errcode ERR_MAXIS_NOT_RETURNED_ORIGIN				= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 50);   // 원점 복귀가 수행되지 않았습니다.
constexpr errcode ERR_MAXIS_ORIGIN_RETURNED_FAILURE			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 51);   // 원점복귀 실패.
constexpr errcode ERR_MAXIS_ORIGIN_RETURNED_TIMEOUT			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 53);   // 원점복귀 제한 시간을 초과하였습니다.
constexpr errcode ERR_MAXIS_INVALID_MOVE_TYPE				= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 54);   // 유효한 Move Type이 아닙니다.
constexpr errcode ERR_MAXIS_ESTOP							= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 58);   // E-Stop Event가 발생하였습니다.
constexpr errcode ERR_MAXIS_OVER_SW_POSITIVE_LIMIT			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 60);   // ERR_MAXES_OVER_SW_POSITIVE_LIMIT
constexpr errcode ERR_MAXIS_OVER_SW_NAGATIVE_LIMIT			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 61);   // ERR_MAXES_OVER_SW_NAGATIVE_LIMIT
constexpr errcode ERR_MAXIS_ORIGIN_STOP_BY_USER				= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 62);   // ERR_MAXES_ORIGIN_STOP_BY_USER
constexpr errcode ERR_MAXIS_NO_RESPONSE_COOLMUSCLE			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 63);   // Cool Muscle Motor로부터 응답이 없습니다.
constexpr errcode ERR_MAXIS_INTERLOCK_CHECKED				= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 66);   // ERR_MAXES_INTERLOCK_CHECKED
constexpr errcode ERR_MAXIS_NOT_IN_POSITION					= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 67);   // Motor가 Inposition아 아닙니다.
constexpr errcode ERR_MAXIS_FAIL_LOAD_PARAM					= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 118);  // ERR_MAXES_PARAMETER_LOAD_FAIL;
constexpr errcode ERR_MAXIS_FAIL_SAVE_PARAM					= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 119);  // ERR_MAXES_PARAMETER_SAVE_FAIL
constexpr errcode ERR_MAXIS_INPUT_STOP_BUTTON				= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 200);  // 정지 버튼이 감지되었습니다.
constexpr errcode ERR_MAXIS_SAFETY_SENSOR_INTERLOCK			= def_err_code(ObjType::HL_MOTION, AXIS_OBJ::_max, 201);  // Safety Fence Interlock Checked

#endif
/************************************************************************/
/* Motion Lib Module			   		                                */
/************************************************************************/
// Motion Lib Error(Component Code : 106)
constexpr errcode ERR_MLIB_INVALID_STOP_TYPE				= def_err_code(ObjType::HL_MOTION_LIB, MOTION_LIB_OBJ::_max, 77);	// ERR_MLIB_INVALID_STOP_TYPE
constexpr errcode ERR_MLIB_INVALID_POINTER					= def_err_code(ObjType::HL_MOTION_LIB, MOTION_LIB_OBJ::_max, 90);	// ERR_MLIB_INVALID_POINTER
constexpr errcode ERR_MLIB_INVALID_AXIS_ID					= def_err_code(ObjType::HL_MOTION_LIB, MOTION_LIB_OBJ::_max, 109);	// ERR_MLIB_INVALID_AXIS_ID

/************************************************************************/
/* Serial Communication Module								            */
/************************************************************************/
// Serial Module(Component Code : 108)
constexpr errcode ERR_SERAIL_TIMEOUT						= def_err_code(ObjType::HL_SERIAL, SERIAL_OBJ::_max,0);


/************************************************************************/
/* Net Socket Communication Module								            */
/************************************************************************/
// Net Socket Module(Component Code : 109)
constexpr errcode ERR_ETHERNET_TIMEOUT						= def_err_code(ObjType::HL_ETHERNET, NET_OBJ::_max, 0);


/************************************************************************/
/* Distance Sensor Module								            */
/************************************************************************/
// Distance Senso Module(Component Code : 110)
constexpr errcode ERR_DIST_SENSOR_TIMEOUT					= def_err_code(ObjType::HL_DIST_SENSOR, DISTSENSOR_OBJ::_max, 0);







/************************************************************************/
/* OP PANEL Module			   		                                    */
/************************************************************************/
// OP PANEL(Component Code : 200)
constexpr errcode ERR_OPPANEL_TIMEOUT						= def_err_code(ObjType::ML_OP_PANEL, 0, 0);


/************************************************************************/
/* Mechanical - TapeCut  	   		                                    */
/************************************************************************/
// ML_COMPONENTS TapeCut(Component Code : 201)



/************************************************************************/
/* Mechanical - TapeLoaderUnload  	   		                                    */
/************************************************************************/
// ML_COMPONENTS TapeLoaderUnload(Component Code : 201)




/************************************************************************/
/* autoManager Module			   		                                */
/************************************************************************/
// autoManager (Component Code : 400)
constexpr errcode ERR_AUTO_MANAGER_TIMEOUT					= def_err_code(ObjType::PL_AUTO_MANAGER,0 ,0);

