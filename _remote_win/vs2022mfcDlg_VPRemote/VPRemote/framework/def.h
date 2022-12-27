#pragma once

#include <array>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <iomanip>
#include <algorithm>

// 프로젝트 전역
 /****************************************************
  *	prject define
  ****************************************************/

#define PROJECT_MAIN_HEADER             "VPRemote.h"
#define PROJECT_MAIN_DLG_HEAdER         "VPRemoteDlg.h"

#define PRJ_DATA_FILE_PATH_CONFIG       "./Data/config_dat.json"
#define PRJ_DATA_FILE_PATH_AXIS         "./Data/axis_dat.json"
#define PRJ_DATA_FILE_PATH_MOTION       "./Data/motion_dat.json"
#define PRJ_DATA_FILE_PATH_CYLINDER     "./Data/cylinder_dat.json"
#define PRJ_DATA_FILE_PATH_VACUUM       "./Data/vacuum_dat.json"
#define PRJ_DATA_FILE_PATH_ERR_DESC     "./Data/error_description.json"

#define PRJ_CFG_USE_CSTRING
#ifdef PRJ_CFG_USE_CSTRING
#define PRJ_CF_USE_STRING CString
#else
#define PRJ_CF_USE_STRING std::string
#endif

  //typedef const int def_t;

#define PRJ_NAME_STR_LENGTH_MAX 			  26
#define PRJ_FILE_DIR_STR_MAX			      260
#define PRJ_DATA_MSG_STR_MAX				  256

//#define PRJ_USE_FUNC_CLI
#ifdef PRJ_USE_FUNC_CLI
  #define PRJ_CLI_CMD_LIST_MAX 16 
  #define PRJ_CLI_CMD_NAME_MAX 16 
  
  #define PRJ_CLI_LINE_HIS_MAX 8
  #define PRJ_CLI_LINE_BUF_MAX 64
#endif

//9자리로 구성되며 object Type Code [xxx] + instance no [xxx] + 정의된 error no [xxx]
typedef errno_t errcode;
//9자리로 구성되면 Object ID [xxx] errcode [xxx+xxx] 조합한 코드
//objid[xxx]+ type[xxx] + error no[xxx]
typedef int obj_errno;


typedef void (*evt_cb)(void* obj, void* w_parm, void* l_parm);
typedef void (*init_cb)(void); //void(*cd_increase)(void)
typedef bool (*openExe_cb)(void); //void(*cd_increase)(void)
typedef void (*swTimer_cb)(void* obj); // 콜백 타입 정의





