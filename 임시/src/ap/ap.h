/*
 * ap.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_H_
#define SRC_AP_AP_H_


#include "ap_def.h"


#include "def_obj.h"

/* ap object*/
#include "ap_util.h"
#include "ap_dat.h"
#include "ap_log.h"


/* interface object*/
#include "iio.h"

/* ap object */
/* register -> communication -> basic unit(engine) -> combined unit(engine) -> control */
//register + io_manager
#include "ap_reg.h"
#include "ap_io.h"

//basic
#include "uart_moons.hpp"
#include "uart_nextion.hpp"
#include "uart_remote.hpp"

#include "enOp.hpp"
#include "enCyl.hpp"
#include "enVac.hpp"
#include "enMotor_moons.hpp"


struct prc_step_t
{
  uint8_t curr_step{};
  uint8_t pre_step{};
  uint8_t wait_step{};
  uint32_t pretime_ms{};
  uint32_t pass_ms{};

  inline void SetStep(uint8_t step){
    pass_ms = pass_ms - millis();
  	pre_step = curr_step;
    curr_step = step;
  }

  inline uint8_t GetStep() const{
    return curr_step;
  }

} ;
//control
#include "cnAuto.hpp"
#include "cnMotors.hpp"
#include "cnTasks.hpp"
#include "cnJob.hpp"


#include "api_lcd.hpp"
#include "api_remote.hpp"



void apInit(void);
void apMain(void);



#endif /* SRC_AP_AP_H_ */
