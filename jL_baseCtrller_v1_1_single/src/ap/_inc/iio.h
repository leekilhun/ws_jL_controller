/*
 * iio.h
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */

#ifndef SRC_AP_INTERFACE_IIO_H_
#define SRC_AP_INTERFACE_IIO_H_



class iio
{
public:

  virtual bool IsOn(uint32_t addr) = 0;
  virtual bool IsOff(uint32_t addr) = 0;
  virtual int OutputOn(uint32_t addr) = 0;
  virtual int OutputOff(uint32_t addr) = 0;
  virtual int OutputToggle(uint32_t addr) = 0;
};



#endif /* SRC_AP_INTERFACE_IIO_H_ */
