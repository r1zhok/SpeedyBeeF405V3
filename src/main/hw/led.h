/*
 * led.h
 *
 *  Created on: Dec 6, 2020
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_LED_H_
#define SRC_COMMON_HW_INCLUDE_LED_H_


#include "hw.h"


#ifdef _USE_HW_LED

#define LED_MAX_CH          HW_LED_MAX_CH

typedef enum
{
  ST1 = 0u,
  ST2,
} LED_Type;


bool ledInit(void);
void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_LED_H_ */
