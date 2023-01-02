/*
 * s_curve.h
 *
 *  Created on: Jan 2, 2023
 *      Author: gns2l
 */

#ifndef S_CURVE_H_
#define S_CURVE_H_


#include <stdint.h>

// Initialize s_curve algorithm
void init_s_curve(void);

// Update s_curve values for given number of pins
void update_s_curve_values(uint8_t* values, uint8_t num_pins);


#endif /* S_CURVE_H_ */
