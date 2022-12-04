/*
 * cnJob.hpp
 *
 *  Created on: 2022. 12. 4.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_CNJOB_HPP_
#define SRC_AP__INC_CNJOB_HPP_


class cnJob
{
  /****************************************************
   *  data
   ****************************************************/
public:
  struct cfg_t
  {
    cnAuto *p_AutoManger;
  };

private:


public:


  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnJob() {};
  virtual ~cnJob(){};

  /****************************************************
   *  func
   ****************************************************/
private:
  // for auto run
  void doRunStep();

public:
  int Init(cnJob::cfg_t &cfg);

};



#endif /* SRC_AP__INC_CNJOB_HPP_ */
