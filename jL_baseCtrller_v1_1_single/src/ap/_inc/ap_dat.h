/*
 * ad_dat.h
 *
 *  Created on: 2022. 2. 24.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_DAT_H_
#define SRC_AP_AP_DAT_H_

/**
 * @brief Define the address of eeprom
 *
 * st24c64c - 32byte 256 page  0x0000  ~ 0x2000 *
 */

#define APDAT_START_ADDRESS               0x0000

#define APDAT_START_ADDR                  APDAT_START_ADDRESS
#define APDAT_LENGTH                      4
#define APDAT_ADDR(x)                     APDAT_START_ADDR + ((x) * APDAT_LENGTH)
#define APDAT_CNT_MAX                     8
#define APDAT_END_ADDR                    APDAT_START_ADDR + (APDAT_LENGTH * APDAT_CNT_MAX)

/*
 * motor 1  8 cnt data
 * motor 2  8 cnt data
 * motor 3  8 cnt data
 * total  24 cnt
 * */
#define APDAT_AXIS_POS_START_ADDR         APDAT_END_ADDR
#define APDAT_AXIS_POS_DATA_LENGTH        8
#define APDAT_AXIS_POS_DATA_ADDR(x)       APDAT_AXIS_POS_START_ADDR + ((x) * APDAT_AXIS_POS_DATA_LENGTH)
#define APDAT_AXIS_POS_DATA_CNT_MAX       24
#define APDAT_AXIS_POS_END_ADDR           APDAT_AXIS_POS_START_ADDR + (APDAT_AXIS_POS_DATA_LENGTH * APDAT_AXIS_POS_DATA_CNT_MAX)





#define APDAT_CYL_ACT_START_ADDR          APDAT_AXIS_POS_END_ADDR
#define APDAT_CYL_ACT_DATA_LENGTH         4
#define APDAT_CYL_ACT_DATA_ADDR(x)        APDAT_CYL_ACT_START_ADDR + ((x) * APDAT_CYL_ACT_DATA_LENGTH)
#define APDAT_CYL_ACT_DATA_CNT_MAX        8
#define APDAT_CYL_ACT_END_ADDR            APDAT_CYL_ACT_START_ADDR + (APDAT_CYL_ACT_DATA_LENGTH * APDAT_CYL_ACT_DATA_CNT_MAX)

#define APDAT_VAC_ACT_START_ADDR          APDAT_CYL_ACT_END_ADDR
#define APDAT_VAC_ACT_DATA_LENGTH         4
#define APDAT_VAC_ACT_DATA_ADDR(x)        APDAT_VAC_ACT_START_ADDR + ((x) * APDAT_VAC_ACT_DATA_LENGTH)
#define APDAT_VAC_ACT_DATA_CNT_MAX        8
#define APDAT_VAC_ACT_END_ADDR            APDAT_VAC_ACT_START_ADDR + (APDAT_VAC_ACT_DATA_LENGTH * APDAT_VAC_ACT_DATA_CNT_MAX)

#define APDAT_SEQ_START_ADDR              APDAT_VAC_ACT_END_ADDR
#define APDAT_SEQ_DATA_LENGTH             4
#define APDAT_SEQ_DATA_ADDR(x)            APDAT_SEQ_START_ADDR + ((x) * APDAT_SEQ_DATA_LENGTH)
#define APDAT_SEQ_DATA_CNT_MAX            8
#define APDAT_SEQ_END_ADDR                APDAT_SEQ_START_ADDR + (APDAT_SEQ_DATA_LENGTH * APDAT_SEQ_DATA_CNT_MAX)


#define APDAT_LINK_START_ADDR              APDAT_SEQ_END_ADDR
#define APDAT_LINK_DATA_LENGTH             8
#define APDAT_LINK_DATA_ADDR(x)            APDAT_LINK_START_ADDR + ((x) * APDAT_LINK_DATA_LENGTH)
#define APDAT_LINK_DATA_CNT_MAX            8
#define APDAT_LINK_END_ADDR                APDAT_LINK_START_ADDR + (APDAT_LINK_DATA_LENGTH * APDAT_LINK_DATA_CNT_MAX)



#define APDAT_LOG_START_ADDR              APDAT_LINK_END_ADDR
#define APDAT_LOG_DATA_LENGTH             64
#define APDAT_LOG_DATA_ADDR(x)            APDAT_LOG_START_ADDR + ((x) * APDAT_LOG_DATA_LENGTH)
#define APDAT_LOG_DATA_CNT_MAX            32
#define APDAT_LOG_END_ADDR                APDAT_LOG_START_ADDR + (APDAT_LOG_DATA_LENGTH * APDAT_LOG_DATA_CNT_MAX)


#define UI_ROM_DATA_TYPE_MOTOR_POS    0x10
#define UI_ROM_DATA_TYPE_AP           0x11
#define UI_ROM_DATA_TYPE_CYL          0x12
#define UI_ROM_DATA_TYPE_VAC          0x13
#define UI_ROM_DATA_TYPE_SEQ          0x14
#define UI_ROM_DATA_TYPE_LINK         0x15



#define AP_DAT_MOTOR_CFG_BASE        static_cast<uint8_t>(ap_dat::addr_e::ap_mot_1_cfg)
#define MotorCfgAddr(x)              ((AP_DAT_MOTOR_CFG_BASE)-1) + (x)

struct ap_dat
{
  struct dat_t
  {
    uint16_t  parm1;
    uint16_t  parm2;
  };

  enum addr_e:uint8_t //
  {
    mt_jig_turn_dist, mt_roll_turn_dist, mt_high_turn_dist, cfg_4 ,sycn_rate, cfg_6, cfg_7, cfg_8,
    _max
  };

  dat_t apcfg_dat[APDAT_CNT_MAX];
                  /*{
    {100,100},{10000,1800},{10000,7200},{10000,150},{0,0},{0,0},{0,0},{0,0}
  };*/

  inline void WriteData(addr_e addr, dat_t data){
    apcfg_dat[addr]=data;
    uint16_t rcv_data[2] = {data.parm1, data.parm2};
    at24c64Write(APDAT_ADDR(addr), (uint8_t *)&rcv_data[0], APDAT_LENGTH);
  }

  inline dat_t ReadData(addr_e addr){
    return apcfg_dat[addr];
  }

  inline dat_t LoadData(addr_e addr){
    uint16_t rcv_data[2] = {0,};
    uint8_t idx = addr;
    at24c64Read(APDAT_ADDR(idx), (uint8_t*)&rcv_data, APDAT_LENGTH);
    apcfg_dat[idx].parm1 =rcv_data[0];
    apcfg_dat[idx].parm2 =rcv_data[1];
    return apcfg_dat[idx];
  }
  inline dat_t* GetData(addr_e addr){
    return &apcfg_dat[addr];
  }

  inline bool LoadRomData(){
    uint16_t rcv_data[2] = {0,};
    bool ret;
    for (uint8_t i = 0; i < APDAT_CNT_MAX ;  i++) {
      ret = at24c64Read(APDAT_ADDR(i), (uint8_t*)&rcv_data, APDAT_LENGTH);
      apcfg_dat[i].parm1 =rcv_data[0];
      apcfg_dat[i].parm2 =rcv_data[1];
    }
    return ret;
  }

  inline bool ClearRomData(){
    uint8_t data[APDAT_LENGTH] = {0,};
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_ADDR(i), (uint8_t*)&data, APDAT_LENGTH);
    }
    return ret;
  }
/*
  inline void Write_MotCfg(dat_t data){
    apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::ap_mot_move)]=data;
    uint16_t rcv_data[2] = {data.parm1, data.parm2};
    at24c64Write(APDAT_ADDR(static_cast<uint8_t>(ap_dat::addr_e::ap_mot_move)), (uint8_t *)&rcv_data[0], APDAT_LENGTH);
  }

  inline dat_t Read_MotCfg(){
    return apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::ap_mot_move)];
  }
  */
};




/*
 * motor pose data address
 */

enum class mt_jig_pos_addr
{
	ready,
	sticky,
	trash,
	pos_3,
	pos_4,
	pos_5,
	pos_6,
	pos_7,
	_max
};

enum class mt_roll_pos_addr
{
	ready = static_cast<uint8_t>(mt_jig_pos_addr::_max),
	sticky,//
  trash,//
	pos_3,
	pos_4,
	pos_5,
	pos_6,
	pos_7,
	_max
};

enum class mt_high_pos_addr
{
	ready = static_cast<uint8_t>(mt_roll_pos_addr::_max),
	sticky,//
  up_sticky,//
  trash,//
  pos_4,//
  pos_5,
  pos_6,
  pos_7,
  _max
};


struct axis_dat
{
  struct dat_t
  {
    int       cmd_pos{};
    uint32_t  cmd_vel{};
  };

  /*multiply by data length 8*/
/*  enum class addr_e
  {
    ready_pos, pos_start_peel, pos_vinyl_cplt, pos_destination,
    pos_reattach, pos_reattach_cplt, pos_6, pos_7,
    _max
  };*/

  dat_t cmd_pos_dat[APDAT_AXIS_POS_DATA_CNT_MAX];
/*   {
    {3000,1000},{92000,1000},{250000,500},{300000,1000},
    {262000,1000},{90000,500},{3000,500},{0,0},

    {1000,500},{15000,1000},{180000,500},{0,0},
    {0,0},{0,0},{0,0},{0,0},

    {5000,1500},{28600,1500},{28000,500},{31500,1500},
    {0,0},{0,0},{0,0},{0,0},
   };*/


  inline void WriteData(uint8_t addr, dat_t data){
    cmd_pos_dat[addr]=data;
    uint32_t rcv_data[2] = {(uint32_t)data.cmd_pos,data.cmd_vel};
    at24c64Write(APDAT_AXIS_POS_DATA_ADDR(addr),(uint8_t *)&rcv_data[0], APDAT_AXIS_POS_DATA_LENGTH);
  }

  inline dat_t* GetData(uint8_t addr){
    return &cmd_pos_dat[addr];
  }
  inline dat_t ReadData(uint8_t addr){
    return cmd_pos_dat[addr];
  }

  inline dat_t LoadData(uint8_t addr){
    uint32_t rcv_data[2] = {0,};
    at24c64Read(APDAT_AXIS_POS_DATA_ADDR(addr), (uint8_t*)&rcv_data, APDAT_AXIS_POS_DATA_LENGTH);
    cmd_pos_dat[addr] = {(int)rcv_data[0], rcv_data[1]};
    return cmd_pos_dat[addr];
  }
  inline bool LoadRomData(){
    bool ret;
    uint32_t rcv_data[2] = {0,};
    for (uint8_t i = 0; i < APDAT_AXIS_POS_DATA_CNT_MAX ;  i++) {
      ret = at24c64Read(APDAT_AXIS_POS_DATA_ADDR(i), (uint8_t*)&rcv_data, APDAT_AXIS_POS_DATA_LENGTH);
      cmd_pos_dat[i] = {(int)rcv_data[0], rcv_data[1]};
    }
    return ret;
  }

  inline bool ClearRomData(){
    uint8_t data[APDAT_AXIS_POS_DATA_LENGTH] = {0,};
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_AXIS_POS_DATA_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_AXIS_POS_DATA_ADDR(i), (uint8_t*)&data, APDAT_AXIS_POS_DATA_LENGTH);
    }
    return ret;
  }

};

struct cyl_dat{
  struct dat_t
  {
    /** ?????? ?????? ?????? ms */
    uint16_t timeout;

    /** ?????? ??? ????????? ?????? ms*/
    uint16_t on_settling;
  };

  /* multiply by length 4*/
  enum addr_e : uint8_t //
  {
  	phone_jig_open_close,
  	phone_jig_for_back,
		vinylhold_up_down,
		vinyl_push_back,

		cyl4, cyl_5,cyl_6,cyl_7,
  	_max
  };

  dat_t cyl_act_dat[APDAT_CYL_ACT_DATA_CNT_MAX];

  inline void WriteData(addr_e addr, dat_t data){
    cyl_act_dat[addr]=data;
    uint16_t rcv_data[2] = {data.timeout, data.on_settling};
    at24c64Write(APDAT_CYL_ACT_DATA_ADDR(addr), (uint8_t *)&rcv_data[0], APDAT_CYL_ACT_DATA_LENGTH);
  }

  inline dat_t* GetData(addr_e addr){
    return &cyl_act_dat[addr];
  }

  inline dat_t ReadData(addr_e addr){
    return cyl_act_dat[addr];
  }

  inline dat_t LoadData(addr_e addr){
    uint16_t rcv_data[2] = {0,};
    uint8_t idx = addr;
    at24c64Read(APDAT_CYL_ACT_DATA_ADDR(idx), (uint8_t*)&rcv_data, APDAT_CYL_ACT_DATA_LENGTH);
    cyl_act_dat[idx] = {rcv_data[0], rcv_data[1]};
    return cyl_act_dat[idx];
  }

  inline bool LoadRomData(){
    uint16_t rcv_data[2] = {0,};
    bool ret;
    for (uint8_t i = 0; i < APDAT_CYL_ACT_DATA_CNT_MAX ;  i++) {
      ret = at24c64Read(APDAT_CYL_ACT_DATA_ADDR(i), (uint8_t*)&rcv_data, APDAT_CYL_ACT_DATA_LENGTH);
      cyl_act_dat[i].timeout =rcv_data[0];
      cyl_act_dat[i].on_settling =rcv_data[1];
    }
    return ret;
  }

  inline bool ClearRomData(){
    uint8_t data[APDAT_CYL_ACT_DATA_LENGTH] = {0,};
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_CYL_ACT_DATA_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_CYL_ACT_DATA_ADDR(i), (uint8_t*)&data, APDAT_CYL_ACT_DATA_LENGTH);
    }
    return ret;
  }




};

struct vac_dat{
  struct dat_t
  {
    /** ?????? ?????? ?????? */
    uint16_t timeout;
    /** ON ?????? ??? ????????? ?????? */
    uint16_t  on_settling;
  };

  enum addr_e:uint8_t //
  {
    vinyl_detect,
		vac_1, vac_2, vac_3,
    vac_4, vac_5, vac_6, vac_7,
    _max
  };

  dat_t vac_act_dat[APDAT_VAC_ACT_DATA_CNT_MAX];

  inline void WriteData(addr_e addr, dat_t data){
    vac_act_dat[addr]=data;
    uint16_t rcv_data[2] = {data.timeout, data.on_settling};
    at24c64Write(APDAT_VAC_ACT_DATA_ADDR(addr), (uint8_t *)&rcv_data[0], APDAT_VAC_ACT_DATA_LENGTH);
  }

  inline dat_t* GetData(addr_e addr){
    return &vac_act_dat[addr];
  }

  inline dat_t ReadData(addr_e addr){
    return vac_act_dat[addr];
  }

  inline dat_t LoadData(addr_e addr){
    uint16_t rcv_data[2] = {0,};
    uint8_t idx = addr;
    at24c64Read(APDAT_VAC_ACT_DATA_ADDR(idx), (uint8_t*)&rcv_data, APDAT_VAC_ACT_DATA_LENGTH);
    vac_act_dat[idx] = {rcv_data[0], rcv_data[1]};
    return vac_act_dat[idx];
  }

  inline bool LoadRomData(){
    uint16_t rcv_data[2] = {0,};
    bool ret;
    for (uint8_t i = 0; i < APDAT_VAC_ACT_DATA_CNT_MAX ;  i++) {
      ret = at24c64Read(APDAT_VAC_ACT_DATA_ADDR(i), (uint8_t*)&rcv_data, APDAT_VAC_ACT_DATA_LENGTH);
      vac_act_dat[i].timeout =rcv_data[0];
      vac_act_dat[i].on_settling =rcv_data[1];
    }
    return ret;
  }

  inline bool ClearRomData(){
    uint8_t data[APDAT_VAC_ACT_DATA_LENGTH] = {0,};
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_VAC_ACT_DATA_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_VAC_ACT_DATA_ADDR(i), (uint8_t*)&data, APDAT_VAC_ACT_DATA_LENGTH);
    }
    return ret;
  }

};



#define SEQ_DAT_DEF_MAX_SPEED          101
#define SEQ_DAT_DEF_MAX_LOOP_CNT       1001
struct seq_dat{
  struct dat_t
  {
    uint16_t  parm1;
    uint16_t  parm2;
  };

  enum addr_e:uint8_t //
  {
  	seq_1, seq_2, seq_3, seq_4,
    seq_5, seq_6, seq_7, seq_8,
    _max
  };

  dat_t sequencing_dat[APDAT_SEQ_DATA_CNT_MAX];

  inline void WriteData(addr_e addr, dat_t data){
    sequencing_dat[addr]=data;
    uint16_t rcv_data[2] = {data.parm1, data.parm2};
    at24c64Write(APDAT_SEQ_DATA_ADDR(addr), (uint8_t *)&rcv_data[0], APDAT_SEQ_DATA_LENGTH);
  }

  inline dat_t* GetData(addr_e addr){
    return &sequencing_dat[addr];
  }

  inline dat_t ReadData(addr_e addr){
    return sequencing_dat[addr];
  }

  inline dat_t LoadData(addr_e addr){
    uint16_t rcv_data[2] = {0,};
    uint8_t idx = addr;
    at24c64Read(APDAT_SEQ_DATA_ADDR(idx), (uint8_t*)&rcv_data, APDAT_SEQ_DATA_LENGTH);
    sequencing_dat[idx] = {rcv_data[0], rcv_data[1]};
    return sequencing_dat[idx];
  }

  inline bool LoadRomData(){
    uint16_t rcv_data[2] = {0,};
    bool ret;
    for (uint8_t i = 0; i < APDAT_SEQ_DATA_CNT_MAX ;  i++) {
      ret = at24c64Read(APDAT_SEQ_DATA_ADDR(i), (uint8_t*)&rcv_data, APDAT_SEQ_DATA_LENGTH);
      sequencing_dat[i].parm1 =rcv_data[0];
      sequencing_dat[i].parm2 =rcv_data[1];
    }
    return ret;
  }

  inline bool ClearRomData(){
    uint8_t data[APDAT_SEQ_DATA_LENGTH] = {0,};
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_SEQ_DATA_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_SEQ_DATA_ADDR(i), (uint8_t*)&data, APDAT_SEQ_DATA_LENGTH);
    }
    return ret;
  }

};


struct link_dat{
  struct dat_t
  {
    int       cmd_pos{};
    uint32_t  cmd_vel{};
  };


  enum addr_e:uint8_t //
  {
    sticky_ready,//0,
    roll_slow,//1,
    roll_fast,//2,
		roll_clear,//3,
    link_4,//4,
    link_5,//5,
    link_6,//6,
    link_7,
    _max
  };

  dat_t linkpose_dat[APDAT_LINK_DATA_CNT_MAX];

  inline void WriteData(addr_e addr, dat_t data){
    linkpose_dat[addr]=data;
    uint32_t rcv_data[2] = {(uint32_t)data.cmd_pos, data.cmd_vel};
    at24c64Write(APDAT_LINK_DATA_ADDR(addr), (uint8_t *)&rcv_data[0], APDAT_LINK_DATA_LENGTH);
  }

  inline dat_t* GetData(addr_e addr){
    return &linkpose_dat[addr];
  }

  inline dat_t ReadData(addr_e addr){
    return linkpose_dat[addr];
  }

  inline dat_t LoadData(addr_e addr){
    uint32_t rcv_data[2] = {0,};
    uint8_t idx = addr;
    at24c64Read(APDAT_LINK_DATA_ADDR(idx), (uint8_t*)&rcv_data, APDAT_LINK_DATA_LENGTH);
    linkpose_dat[idx] = {(int)rcv_data[0], rcv_data[1]};
    return linkpose_dat[idx];
  }

  inline bool LoadRomData(){
    uint32_t rcv_data[2] = {0,};
    bool ret;
    for (uint8_t i = 0; i < APDAT_LINK_DATA_CNT_MAX ;  i++) {
      ret = at24c64Read(APDAT_LINK_DATA_ADDR(i), (uint8_t*)&rcv_data, APDAT_LINK_DATA_LENGTH);
      linkpose_dat[i] = {(int)rcv_data[0] , rcv_data[1]};
    }
    return ret;
  }

  inline bool ClearRomData(){
    uint8_t data[APDAT_LINK_DATA_LENGTH] = {0,};
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_LINK_DATA_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_LINK_DATA_ADDR(i), (uint8_t*)&data, APDAT_LINK_DATA_LENGTH);
    }
    return ret;
  }

};





#define AP_LOG_DAT_BUFF_SIZE               60
#define AP_LOG_DAT_HEADER                  0x11 // for nextion_lcd display
struct log_dat{
  struct head_t
  {
    uint8_t  header=AP_LOG_DAT_HEADER;
    uint8_t  error_no{};
    uint8_t  obj_id{};
    uint8_t  step_no{};

    head_t& operator = (const head_t& dat){
    	header = dat.header;
    	error_no = dat.error_no;
    	obj_id = dat.obj_id;
    	step_no = dat.step_no;
    	return *this;
    }

  };

  struct dat_t
  {
    head_t head{};
    std::array<char, AP_LOG_DAT_BUFF_SIZE> log{};
    //char  log[AP_LOG_DAT_BUFF_SIZE]{};

    dat_t(){init();}
    inline void init(){
      head.header = AP_LOG_DAT_HEADER;
      head.error_no = 0;
      head.obj_id = 0;
      head.step_no = 0;
      log.fill(0);
    }

    dat_t& operator = (const dat_t& dat){
    	head = dat.head;
    	log = dat.log;
    /*	for (uint8_t i = 0; i < AP_LOG_DAT_BUFF_SIZE; i++) {
    		this->log[i]=dat.log[i];
    	}*/
    	return *this;
    }
    /*
    inline void operator = (const dat_t* p_dat){
      this->head =  p_dat->head;
      for (uint8_t i = 0; i < AP_LOG_DAT_BUFF_SIZE; i++) {
        this->log[i]=p_dat->log[i];
      }
    }
    */

  };

  enum addr_e:uint8_t //
  {
    log_00, log_01, log_02, log_03, log_04, log_05, log_06, log_07,
    log_10, log_11, log_12, log_13, log_14, log_15, log_16, log_17,
    log_20, log_21, log_22, log_23, log_24, log_25, log_26, log_27,
    log_30, log_31, log_32, log_33, log_34, log_35, log_36, log_37,
    _max
  };

  dat_t log_buff;

  inline char* GetBuffer(){
    return &log_buff.log[0];
  }

  inline uint8_t GetBufferLen(){
    return (uint8_t)log_buff.log.size();
  }
  inline void WriteData(addr_e addr, dat_t& ref_data){
    at24c64Write(APDAT_LOG_DATA_ADDR(addr), (uint8_t *)&ref_data, APDAT_LOG_DATA_LENGTH);
  }

  inline dat_t* GetData(addr_e addr){
    at24c64Read(APDAT_LOG_DATA_ADDR(addr), (uint8_t*)&log_buff, APDAT_LOG_DATA_LENGTH);
    return &log_buff;
  }

/*  inline uint8_t* Get(addr_e addr){
    at24c64Read(APDAT_LOG_DATA_ADDR(addr), (uint8_t*)&log_buff, APDAT_LOG_DATA_LENGTH);
    return (uint8_t*)&log_buff;
  }*/

 /* inline bool LoadRomData(){
    bool ret;
    for (uint8_t i = 0; i < static_cast<uint8_t>(log_dat::addr_e::_max) ;  i++) {
      ret = at24c64Read(APDAT_LOG_DATA_ADDR(i), (uint8_t*)&rcv_data, APDAT_LOG_DATA_LENGTH);
    }
    return ret;
  }*/


  inline bool ClearRomData(){
    log_buff.init();
    bool ret = false;
    for (uint8_t i = 0; i < APDAT_LOG_DATA_CNT_MAX ;  i++) {
      ret = at24c64Write(APDAT_LOG_DATA_ADDR(i), (uint8_t*)&log_buff, APDAT_LOG_DATA_LENGTH);
    }
    return ret;
  }
};




#endif /* SRC_AP_AP_DAT_H_ */
