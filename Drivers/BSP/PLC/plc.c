#include "plc.h"

void plc_change_mode(uint8_t mode) { // mode => 1: send; 0: receive
  switch (mode)
  {
  case 1:
    HAL_GPIO_WritePin(PLC_MODE_GPIO_Port, PLC_MODE_Pin, GPIO_PIN_SET);
    break;
  case 0:
    HAL_GPIO_WritePin(PLC_MODE_GPIO_Port, PLC_MODE_Pin, GPIO_PIN_RESET);
  default:
    break;
  }
}
