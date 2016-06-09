/*
 * gpiocfg.h
 *
 *  Created on: Feb 28, 2016
 *      Author: ivan
 */
#ifndef STCUBE_TOOLS_GPIOCFG_H_
#define STCUBE_TOOLS_GPIOCFG_H_

#include "stm32f1xx_hal_conf.h"
#include "stm32f1xx_hal.h"

#include <stdexcept>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>

/** @defgroup GPIO_Private_Constants GPIO Private Constants
  * @{
  */

#define GPIO_MODE             ((uint32_t)0x00000003)
#define EXTI_MODE             ((uint32_t)0x10000000)
#define GPIO_MODE_IT          ((uint32_t)0x00010000)
#define GPIO_MODE_EVT         ((uint32_t)0x00020000)
#define RISING_EDGE           ((uint32_t)0x00100000)
#define FALLING_EDGE          ((uint32_t)0x00200000)
#define GPIO_OUTPUT_TYPE      ((uint32_t)0x00000010)
#define GPIO_NUMBER           ((uint32_t)16)

/* Definitions for bit manipulation of CRL and CRH register */
#define  GPIO_CR_MODE_INPUT         ((uint32_t)0x00000000) /*!< 00: Input mode (reset state)  */
#define  GPIO_CR_CNF_ANALOG         ((uint32_t)0x00000000) /*!< 00: Analog mode  */
#define  GPIO_CR_CNF_INPUT_FLOATING ((uint32_t)0x00000004) /*!< 01: Floating input (reset state)  */
#define  GPIO_CR_CNF_INPUT_PU_PD    ((uint32_t)0x00000008) /*!< 10: Input with pull-up / pull-down  */
#define  GPIO_CR_CNF_GP_OUTPUT_PP   ((uint32_t)0x00000000) /*!< 00: General purpose output push-pull  */
#define  GPIO_CR_CNF_GP_OUTPUT_OD   ((uint32_t)0x00000004) /*!< 01: General purpose output Open-drain  */
#define  GPIO_CR_CNF_AF_OUTPUT_PP   ((uint32_t)0x00000008) /*!< 10: Alternate function output Push-pull  */
#define  GPIO_CR_CNF_AF_OUTPUT_OD   ((uint32_t)0x0000000C) /*!< 11: Alternate function output Open-drain  */

/**
  * @}
  */

#endif /* STCUBE_TOOLS_GPIOCFG_H_ */
