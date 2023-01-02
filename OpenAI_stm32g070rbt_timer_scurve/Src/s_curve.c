/*
 * s_curve.c
 *
 *  Created on: Jan 2, 2023
 *      Author: gns2l
 */




#include "s_curve.h"

// S-curve acceleration and deceleration rates
#define ACCEL_RATE 0.1
#define DECEL_RATE 0.2

// S-curve start and end speeds
#define START_SPEED 0.2
#define END_SPEED 0.8

// S-curve start and end positions
#define START_POS 0
#define END_POS 100

// Current s-curve position
static int32_t pos = START_POS;

// Current s-curve speed
static float speed = START_SPEED;

// Initialize s_curve algorithm
void init_s_curve(void)
{
  pos = START_POS;
  speed = START_SPEED;
}

// Update s_curve values for given number of pins
void update_s_curve_values(uint8_t* values, uint8_t num_pins)
{
  // Calculate next position
  int32_t next_pos = pos + (int32_t)(speed * num_pins);

  // Check if we have reached the end position
  if (next_pos >= END_POS) {
    // Calculate number of steps until end position
    int32_t steps = END_POS - pos;

    // Set remaining values to end position
    for (int i = 0; i < steps; i++) {
      values[i] = END_SPEED * 255;
    }

    // Reset to start position
    init_s_curve();
  } else {
    // Update values with current speed
    for (int i = 0; i < num_pins; i++) {
      values[i] = speed * 255;
    }

    // Update position and speed
    pos = next_pos;
    if (pos < (END_POS / 2)) {
      speed += ACCEL_RATE;
    } else {
      speed -= DECEL_RATE;
    }
  }
}
