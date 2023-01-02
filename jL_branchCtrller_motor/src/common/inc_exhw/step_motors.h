/*
 * step_motors.h
 *
 *  Created on: 2023. 1. 1.
 *      Author: gns2l
 */

#ifndef SRC_COMMON_INC_EXHW_STEP_MOTORS_H_
#define SRC_COMMON_INC_EXHW_STEP_MOTORS_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "exhw_def.h"


#ifdef _USE_EXHW_STEP_MOTORS


  bool step_motorsInit(void);
  //bool step_motorStart(axis_dir axis);

  //void step_motorEnable(axis_dir axis);
  //void step_motorDisable(axis_dir axis);

  //void step_motorMoveRel( axis_dir axis, int steps, uint32_t speed);
  void step_motorsSetPosition(axis_dir axis, int32_t steps);
  void step_motorsRunAndWait(void);

#endif


#ifdef __cplusplus
}
#endif




#endif /* SRC_COMMON_INC_EXHW_STEP_MOTORS_H_ */
