/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f0xx_hal.h"

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
#define Encoder_CLEAR_Pin GPIO_PIN_13
#define Encoder_CLEAR_GPIO_Port GPIOC
#define X_encoderA_Pin GPIO_PIN_0
#define X_encoderA_GPIO_Port GPIOA
#define X_encoderB_Pin GPIO_PIN_1
#define X_encoderB_GPIO_Port GPIOA
#define Y_positive_Pin GPIO_PIN_2
#define Y_positive_GPIO_Port GPIOA
#define Y_negative_Pin GPIO_PIN_3
#define Y_negative_GPIO_Port GPIOA
#define Y_encoderA_Pin GPIO_PIN_6
#define Y_encoderA_GPIO_Port GPIOA
#define Rest_Pin GPIO_PIN_7
#define Rest_GPIO_Port GPIOA
#define limit_x_up_Pin GPIO_PIN_15
#define limit_x_up_GPIO_Port GPIOB
#define limit_x_up_EXTI_IRQn EXTI4_15_IRQn
#define limit_x_down_Pin GPIO_PIN_6
#define limit_x_down_GPIO_Port GPIOC
#define limit_x_down_EXTI_IRQn EXTI4_15_IRQn
#define Y_encoderB_Pin GPIO_PIN_7
#define Y_encoderB_GPIO_Port GPIOC
#define X_positive_Pin GPIO_PIN_8
#define X_positive_GPIO_Port GPIOA
#define X_negative_Pin GPIO_PIN_9
#define X_negative_GPIO_Port GPIOA
#define limit_y_up_Pin GPIO_PIN_2
#define limit_y_up_GPIO_Port GPIOD
#define limit_y_down_Pin GPIO_PIN_3
#define limit_y_down_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
