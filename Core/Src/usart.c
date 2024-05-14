/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "../../Drivers/BSP/PLC/plc.h"


// 串口重定�????
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
 HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
 return ch;
}

// USART3
#define USART3_END (g_usart3_rx_sta & 0x3fff)

uint8_t g_usart3_rx_buf[USART_RX_LEN];

/*  接收状�??
 *  bit15�????      接收完成标志
 *  bit14�????      接收�????0x0d
 *  bit13~0�????    接收到的有效字节数目
*/
uint16_t g_usart3_rx_sta = 0;

uint8_t g_usart3_hal_rx_buffer[USART_RX_BUFFER_SIZE];  /* HAL库使用的串口接收缓冲 */

// USART1
#define USART1_END (g_usart1_rx_sta & 0x3fff)

uint8_t g_usart1_rx_buf[USART_RX_LEN];

/*  接收状�??
 *  bit15�????      接收完成标志
 *  bit14�????      接收�????0x0d
 *  bit13~0�????    接收到的有效字节数目
*/
uint16_t g_usart1_rx_sta = 0;

uint8_t g_usart1_hal_rx_buffer[USART_RX_BUFFER_SIZE];  /* HAL库使用的串口接收缓冲 */

void USART1_RX_USART3_TX();
void USART3_RX_USART1_TX();
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)g_usart1_hal_rx_buffer, USART_RX_BUFFER_SIZE);
  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  HAL_UART_Receive_IT(&huart3, (uint8_t *)g_usart3_hal_rx_buffer, USART_RX_BUFFER_SIZE);
  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART3) {                                  // USART3(PC)
		if ((g_usart3_rx_sta & 0x8000) == 0) {                          // 接收未完�???? 
			if (!(g_usart3_rx_sta & 0x4000)) {                            // 未接收到0x0D（回�????)
				if (g_usart3_hal_rx_buffer[0] == 0x0d) {                     // 接收�????0x0D
					g_usart3_rx_sta |= 0x4000;                                // 回车标志�???? �????1
				} else {
          g_usart3_rx_buf[USART3_END] = g_usart3_hal_rx_buffer[0];  // 将接收到的字符放入rx_buf�????
          g_usart3_rx_sta += 1;
          if (USART3_END > USART_RX_LEN - 1) {
            g_usart3_rx_sta = 0;                                    // 接收数据大于200，重新接�????
          }
				}
			} else {
        if (g_usart3_hal_rx_buffer[0] == 0x0a) {
          g_usart3_rx_sta |= 0x8000;                                // 接收完成
        } else {
          g_usart3_rx_sta = 0;                                      // 接收错误，重新接�????
        }
      }
		}
    HAL_UART_Receive_IT(&huart3, (uint8_t *)g_usart3_hal_rx_buffer, USART_RX_BUFFER_SIZE);
	} else if (huart->Instance == USART1){                                                          // USART1(PLC)
    if ((g_usart1_rx_sta & 0x8000) == 0) {                          // 接收未完�???? 
			if (!(g_usart1_rx_sta & 0x4000)) {                            // 未接收到0x0D（回�????)
				if (g_usart1_hal_rx_buffer[0] == 0x0d) {                     // 接收�????0x0D
					g_usart1_rx_sta |= 0x4000;                                // 回车标志�???? �????1
				} else {
          g_usart1_rx_buf[USART1_END] = g_usart1_hal_rx_buffer[0];  // 将接收到的字符放入rx_buf�????
          g_usart1_rx_sta += 1;
          if (USART1_END > USART_RX_LEN - 1) {
            g_usart1_rx_sta = 0;                                    // 接收数据大于200，重新接�????
          }
				}
			} else {
        if (g_usart1_hal_rx_buffer[0] == 0x0a) {
          g_usart1_rx_sta |= 0x8000;                                // 接收完成
        } else {
          g_usart1_rx_sta = 0;                                      // 接收错误，重新接�????
        }
      }
		}
    HAL_UART_Receive_IT(&huart1, (uint8_t *)g_usart1_hal_rx_buffer, USART_RX_BUFFER_SIZE);
  }
}

// id == 1: USART1(PLC) reveive info and send to USART3(PC)
// id == 3: USART3(PC) reveive info and send to USART1(PLC)
void USART_RX_TX(uint8_t id) {
  if (id == 1) {
    USART1_RX_USART3_TX();
  } else if (id == 3) {
    USART3_RX_USART1_TX();
  }
}

void USART1_RX_USART3_TX() { // USART1(PLC) reveive info and send to USART3(PC)
  if (g_usart1_rx_sta & 0x8000) { // USART1 receive PLC info
    uint32_t len;
    len = g_usart1_rx_sta & 0x3fff;
    // send message to USART3(PC)
    printf("\r\n PLC message: \r\n");
    HAL_UART_Transmit(&huart3, (uint8_t*)g_usart1_rx_buf, len, 1000);
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET); // wait until sending end
    printf("\r\n PLC end \r\n");
    g_usart1_rx_sta = 0;
  }
}

void USART3_RX_USART1_TX() { // USART3(PC) reveive info and send to USART1(PLC)
  if (g_usart3_rx_sta & 0x8000) { // USART3 receive PC info
    uint32_t len;
    len = g_usart3_rx_sta & 0x3fff;
    if (g_usart3_rx_buf[0] == 0x01) { // first Byte is 1, send PC message to PLC
      // send message to USART1(PLC)
      HAL_UART_Transmit(&huart1, (uint8_t*)(&g_usart3_rx_buf[1]), len - 1, 1000);
      while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET);
    } else if (g_usart3_rx_buf[0] == 0x00) { // first Byte is 0, change PLC mode
      plc_change_mode(g_usart3_rx_buf[1]);
      printf("\r\n\r\n");
    }
    g_usart3_rx_sta = 0;
  }
}
/* USER CODE END 1 */
