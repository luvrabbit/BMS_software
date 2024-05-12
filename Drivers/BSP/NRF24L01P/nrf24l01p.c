#include "nrf24l01p.h"

uint8_t TX_ADDR[5] = {0xF0, 0xF0, 0xF0, 0xF0};
uint8_t RX_ADDR[5] = {0xF0, 0xF0, 0xF0, 0xF0};

void nrf_spi_init() {
    __HAL_SPI_DISABLE(&hspi1);
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    HAL_SPI_Init(&hspi1);
    __HAL_SPI_ENABLE(&hspi1);
}

void nrf_init() {
	MX_SPI1_Init();				/* initialize SPI2 */
    nrf_spi_init();             /* modify SPI setting */
    NRF_CE(0);                  /* enable nrf24l01 */
    NRF_CS(1);                  /* SPI CS cancel */
}

uint8_t nrf_check(void) {
    uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    uint8_t i;
    spi1_set_speed(5);                                    /* spi 7.5Mhz */
    nrf_write_buf(W_REGISTER + TX_ADDR_REG, buf, 5);  /* 写入5个字节的地址. */
    nrf_read_buf(R_REGISTER + TX_ADDR_REG, buf, 5);                   /* 读出写入的地址 */

    for (i = 0; i < 5; i++)
    {
        if (buf[i] != 0XA5) break;
    }
    
    if (i != 5) return 1;   /* 检测24L01错误 */

    return 0;               /* 检测到24L01 */
}

/**
 * @brief       NRF24L01 write register
 * @param       reg   : register address 
 * @param       value : value to be written 
 * @retval      status register value 
 */
uint8_t nrf_write_reg(uint8_t reg, uint8_t value) {
    uint8_t status;
    NRF_CS(0);                          /* SPI select nrf*/
    status = spi1_read_write_byte(reg); /* instruction */
    spi1_read_write_byte(value);        /* write value */
    NRF_CS(1);                          /* SPI cancel select nrf */
    return status;                      /* return status */
}

/**
 * @brief       NRF24L01 read register
 * @param       reg   : register address 
 * @retval      register value;
 */
uint8_t nrf_read_reg(uint8_t reg) {
    uint8_t reg_val;
    NRF_CS(0);                             /* SPI select nrf */
    spi1_read_write_byte(reg);             /* send reg address */
    reg_val = spi1_read_write_byte(NOP);   /* read value */
    NRF_CS(1);                             /* SPI cancel select nrf */
    return reg_val;
}

uint8_t nrf_write_buf(uint8_t reg, uint8_t *pbuf, uint8_t len) {
    uint8_t status, i;
    NRF_CS(0);                          /* SPI select nrf */
    status = spi1_read_write_byte(reg); /* send register, and read status */

    for (i = 0; i < len; i++) {
        spi1_read_write_byte(*pbuf++);  /* write data */
    }
    
    NRF_CS(1);                          /* SPI cancel select nrf */
    return status;                      /* return status */
}

uint8_t nrf_read_buf(uint8_t reg, uint8_t *pbuf, uint8_t len) {
    uint8_t status, i;
    NRF_CS(0);                                  /* SPI select nrf */
    status = spi1_read_write_byte(reg);

    for (i = 0; i < len; i++) {
        pbuf[i] = spi1_read_write_byte(0XFF);   /* read data into pbuf */
    }
    
    NRF_CS(1);                                  /* SPI cancel select nrf */
    return status;                              /* return status */
}

void nrf_rx_mode(void) {
    NRF_CE(0);
    nrf_write_buf(W_REGISTER + RX_ADDR_P0, (uint8_t *)RX_ADDR, 5);    /* RX address */
    nrf_write_reg(W_REGISTER + CONFIG, 0x0f);       /* receive mode */
    nrf_write_reg(W_REGISTER + EN_AA, 0x01);        /* enable channel 0 auto answer */
    nrf_write_reg(W_REGISTER + RF_CH, 00);          /* RF freq 2.4Ghz */
    nrf_write_reg(W_REGISTER + RX_PW_P0, 32);       /* channel 0 rx data width */
    nrf_write_reg(W_REGISTER + EN_RXADDR, 0x01);    /* enable channel 0 rx */
    nrf_write_reg(W_REGISTER + RF_SETUP, 0x0f);     /* 设置TX发射参数,0db增益,2Mbps */
    NRF_CE(1);                                           /* CE为高,进入接收模式 */
}

void nrf_tx_mode(void) {
    NRF_CE(0);
    nrf_write_buf(W_REGISTER + TX_ADDR_REG, (uint8_t *)TX_ADDR, 5);       /* 写TX节点地址 */
    nrf_write_buf(W_REGISTER + RX_ADDR_P0, (uint8_t *)RX_ADDR, 5);    /* 设置RX节点地址,主要为了使能ACK */
    nrf_write_reg(W_REGISTER + CONFIG, 0x0e);       /* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断 */
    nrf_write_reg(W_REGISTER + EN_AA, 0x01);        /* 使能通道0的自动应答 */
    nrf_write_reg(W_REGISTER + EN_RXADDR, 0x01);    /* 使能通道0的接收地址 */
    nrf_write_reg(W_REGISTER + SETUP_RETR, 0x1a);   /* 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 */
    nrf_write_reg(W_REGISTER + RF_CH, 00);          /* 设置RF通道为40 */
    nrf_write_reg(W_REGISTER + RF_SETUP, 0x0f);     /* 设置TX发射参数,0db增益,2Mbps */
    NRF_CE(1); /* CE为高,10us后启动发送 */
}

uint8_t nrf_rx_packet(uint8_t *pbuf) {
    uint8_t sta;
    uint8_t rval = 1;
    
    sta = nrf_read_reg(R_REGISTER + STATUS); /* 读取状态寄存器的值 */
    nrf_write_reg(W_REGISTER + STATUS, sta); /* 清除RX_OK中断标志 */

    if (sta & RX_OK)    /* 接收到数据 */
    {
        nrf_read_buf(R_RX_PAYLOAD, pbuf, 32); /* 读取数据 */
        nrf_write_reg(FLUSH_RX, NOP); /* 清除RX FIFO寄存器 */
        rval = 0;       /* 标记接收完成 */
    }

    return rval;    /* 返回结果 */
}

uint8_t nrf_tx_packet(uint8_t *pbuf) {
    uint8_t sta;
    uint8_t rval = 0XFF;
    
    NRF_CE(0);
    nrf_write_buf(W_TX_PAYLOAD, pbuf, 32);    /* 写数据到TX BUF  TX_PLOAD_WIDTH个字节 */
    NRF_CE(1); /* 启动发送 */

    while (NRF_IRQ != 0);          /* 等待发送完成 */

    sta = nrf_read_reg(R_REGISTER + STATUS);    /* 读取状态寄存器的值 */
    nrf_write_reg(W_REGISTER + STATUS, sta);    /* 清除TX_DS或MAX_RT中断标志 */

    if (sta & MAX_TX) {
        nrf_write_reg(FLUSH_TX, 0xff); /* 清除TX FIFO寄存器 */
        rval = 1;
    }

    if (sta & TX_OK) {
        rval = 0;   /* 标记发送成功 */
    }

    return rval;    /* 返回结果 */
}
