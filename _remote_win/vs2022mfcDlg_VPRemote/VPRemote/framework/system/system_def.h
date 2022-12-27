#pragma once

/*
 * system_def.h
 * �ۼ��� : �̱���
 * 22.04.04 - system define �� include
 */

/* �ý��� ������ ������ ����� include*/
/* �Ʒ� ������� ������ ���� */
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

