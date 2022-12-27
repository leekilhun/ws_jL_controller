#pragma once

/*
 * system_def.h
 * 작성자 : 이길훈
 * 22.04.04 - system define 및 include
 */

/* 시스템 전역에 접근할 헤더를 include*/
/* 아래 순서대로 절차를 진행 */
/*

      hal_def.h   <-  def_obj.h   <-   system_def.h   
                                           |
                                      system_obj.h



*/


#include "hal.h"


#include "def_obj.h"
#include "def_err.h"
#include "def_io.h"



/****************************************************
 *	system config
 ****************************************************/
#define USE_IO_DATA_FORMAT_JSON
//#define USE_IO_DATA_FORMAT_DAT

