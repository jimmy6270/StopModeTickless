/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NFC_DECT_Pin GPIO_PIN_13
#define NFC_DECT_GPIO_Port GPIOC
#define BUTTON_Pin GPIO_PIN_0
#define BUTTON_GPIO_Port GPIOA
#define SENSOR_PWR_EN_Pin GPIO_PIN_4
#define SENSOR_PWR_EN_GPIO_Port GPIOA
#define MOTION_PWR_EN_Pin GPIO_PIN_5
#define MOTION_PWR_EN_GPIO_Port GPIOA
#define WL_DECT_Pin GPIO_PIN_6
#define WL_DECT_GPIO_Port GPIOA
#define NFC_PWR_EN_Pin GPIO_PIN_7
#define NFC_PWR_EN_GPIO_Port GPIOA
#define NB_RESET_Pin GPIO_PIN_0
#define NB_RESET_GPIO_Port GPIOB
#define MOTION_INT2_Pin GPIO_PIN_1
#define MOTION_INT2_GPIO_Port GPIOB
#define MOTION_INT1_Pin GPIO_PIN_2
#define MOTION_INT1_GPIO_Port GPIOB
#define NB_PSM_WAKE_Pin GPIO_PIN_12
#define NB_PSM_WAKE_GPIO_Port GPIOB
#define NB_PWR_KEY_Pin GPIO_PIN_13
#define NB_PWR_KEY_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOB
#define VBAT_PWR_EN_Pin GPIO_PIN_15
#define VBAT_PWR_EN_GPIO_Port GPIOB
#define USB_DECT_Pin GPIO_PIN_8
#define USB_DECT_GPIO_Port GPIOA
#define WL_PWR_EN_Pin GPIO_PIN_15
#define WL_PWR_EN_GPIO_Port GPIOA
#define MCS_PWR_EN_Pin GPIO_PIN_3
#define MCS_PWR_EN_GPIO_Port GPIOB
#define MCS_SOURCE_Pin GPIO_PIN_4
#define MCS_SOURCE_GPIO_Port GPIOB
#define NB_PWR_EN_Pin GPIO_PIN_8
#define NB_PWR_EN_GPIO_Port GPIOB
#define MCS_DECT_Pin GPIO_PIN_9
#define MCS_DECT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
