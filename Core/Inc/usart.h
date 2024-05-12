/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
#define USART_RX_LEN 200            // �?大接收字�?200
#define USART_RX_BUFFER_SIZE 1      // buffer大小1
extern uint8_t g_usart3_rx_buf[USART_RX_LEN]; // 接收缓冲
extern uint16_t g_usart3_rx_sta; 	// 接收状�?�标�?
extern uint8_t g_usart3_hal_rx_buffer[USART_RX_BUFFER_SIZE]; // HAL �? USART 接收 Buffer

extern uint8_t g_usart1_rx_buf[USART_RX_LEN]; // 接收缓冲
extern uint16_t g_usart1_rx_sta; 	// 接收状�?�标�?
extern uint8_t g_usart1_hal_rx_buffer[USART_RX_BUFFER_SIZE]; // HAL �? USART 接收 Buffer
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void USART_RX_TX(uint8_t id);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

