#ifndef __NRF24L01P_H__
#define __NRF24L01P_H__

#include "main.h"
#include "spi.h"
// register address macro
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR_REG 0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17

// instruction macro
#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0x2A
#define NOP 0xFF

#define RX_OK 0x40
#define TX_OK 0x20
#define MAX_TX 0x10

#define NRF_CE(x)   do{ x ? \
                              HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_RESET); \
                          }while(0)       /* nrf24L01 CE */

#define NRF_CS(x)   do{ x ? \
                              HAL_GPIO_WritePin(NRF_CS_GPIO_Port, NRF_CS_Pin, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(NRF_CS_GPIO_Port, NRF_CS_Pin, GPIO_PIN_RESET); \
                          }while(0)       /* nrf24L01 CS */

#define NRF_IRQ     HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin) /* nrf24l01 IRQ input*/

void nrf_spi_init();
void nrf_init();
uint8_t nrf_check(void);
uint8_t nrf_write_reg(uint8_t reg, uint8_t value);
uint8_t nrf_read_reg(uint8_t reg);
uint8_t nrf_write_buf(uint8_t reg, uint8_t *pbuf, uint8_t len);
uint8_t nrf_read_buf(uint8_t reg, uint8_t *pbuf, uint8_t len);
uint8_t nrf_tx_packet(uint8_t *pbuf);
uint8_t nrf_rx_packet(uint8_t *pbuf);
void nrf_tx_mode(void);
void nrf_rx_mode(void);



#endif
