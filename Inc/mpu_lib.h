/******************************************************************************
 * @file     mpu_lib.h
 * @brief    Header file of custom functions library
 * @version  V1.0
 * @date     17 June 2022
 * @author	 De Vivo Domenico, Libretti Vittorio, Ricciardi Armando (group 8)
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MPU_LIB_H
#define __MPU_LIB_H

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>				//implementazione della printf a scopo di debug

#include <stdint.h>				//definizioni dei tipi standard C

#include "stm32f4xx_hal.h"		//libreria HAL di STM per usare LED e UserButton

#include <cmsis_compiler.h>		//libreria CMSIS di ARM per usare la MPU
#include <cmsis_gcc.h>			//libreria CMSIS di ARM per usare la MPU
#include <cmsis_version.h>		//libreria CMSIS di ARM per usare la MPU
#include <core_cm4.h>			//libreria CMSIS di ARM per usare la MPU
#include <mpu_armv7.h>			//libreria CMSIS di ARM per usare la MPU

/* Exported constants --------------------------------------------------------*/
#define LEDDELAY    1000000

/* Functions declaration -----------------------------------------------------*/

extern void initialise_monitor_handles();

void GPIO_Init(void);

uint32_t MPU_is_present(void);

void MPU_Configuration(uint32_t permessi_accesso);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

void delay(volatile unsigned long s);

#endif /* __MPU_LIB_H */


