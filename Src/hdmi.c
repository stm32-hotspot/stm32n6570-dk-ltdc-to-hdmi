/**
  ******************************************************************************
  * @file    hdmi.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "hdmi.h"

#include <assert.h>
#include <stdio.h>

#include "stm32n6570_discovery_bus.h"

#if defined(DEBUG)
#define PRINTF(...)    printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif /* defined(DEBUG) */

#define ADV7513_I2C_ADDR 0x7a

static void HDMI_read_modify_write(uint16_t addr, uint8_t data, uint8_t mask)
{
  uint8_t reg;
  int32_t ret;

  ret = BSP_I2C2_ReadReg(ADV7513_I2C_ADDR, addr, &reg, 1);
  assert(ret == 0);

  reg &= ~mask;
  reg |= (data & mask);

  ret = BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, addr, &reg, 1);
  assert(ret == 0);
}

static void HDMI_clear_bits(uint16_t addr, uint8_t bit_to_clear)
{
  HDMI_read_modify_write(addr, 0, bit_to_clear);
}

static void HDMI_set_bits(uint16_t addr, uint8_t bit_to_set)
{
  HDMI_read_modify_write(addr, bit_to_set, bit_to_set);
}

int32_t HDMI_Detect(void)
{
  uint8_t reg;
  int32_t ret;

  BSP_I2C2_Init();

  /* Read chip revision */
  ret = BSP_I2C2_ReadReg(ADV7513_I2C_ADDR, 0x00, &reg, 1);
  if (ret != 0)
  {
    return 0;
  }
  BSP_I2C2_DeInit();

  return reg == 0x13;
}

void HDMI_Init(void)
{
  uint8_t reg;
  int32_t ret;

  BSP_I2C2_Init();

  /* Read chip revision */
  ret = BSP_I2C2_ReadReg(ADV7513_I2C_ADDR, 0x00, &reg, 1);
  assert(ret == 0);
  assert(reg == 0x13);

  PRINTF("Plug hdmi cable to monitor\n");
  /* Poll cable is connected (read HPD pin status) */
  do {
    HAL_Delay(100);
    ret = BSP_I2C2_ReadReg(ADV7513_I2C_ADDR, 0x42, &reg, 1);
    assert(ret == 0);
  } while ((reg & (1 << 6)) == 0);
  PRINTF("Cable plugged detected\n");
  HAL_Delay(1000); // wait some time to be stable

  /* Power up */
  HDMI_clear_bits(0x41, 1 << 6);

  /* Fixed registers that must be set on power-up */
  reg = 0x03;
  BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, 0x98, &reg, 1);
  HDMI_set_bits(0x9a, 7 << 5);
  reg = 0x30;
  BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, 0x9c, &reg, 1);
  HDMI_read_modify_write(0x9d, 1, 3);
  reg = 0xa4;
  BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, 0xa2, &reg, 1);
  reg = 0xa4;
  BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, 0xa3, &reg, 1);
  reg = 0xd0;
  BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, 0xe0, &reg, 1);
  reg = 0x00;
  BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, 0xf9, &reg, 1);

  /* Setup input mode */
  /* input : 24 bits rgb 4:4:4 with separate syncs */
  reg = 0x00;
  BSP_I2C2_WriteReg(ADV7513_I2C_ADDR, 0x15, &reg, 1);
  /* input : 8 bit color depth */
  HDMI_read_modify_write(0x16, 3 << 4, 3 << 4);
  /* input : aspect ratio 4/3 */ // TODO: Enable 19/9 aspect ratio support
  HDMI_read_modify_write(0x17, 0 << 1, 1 << 1);

  /* Setup output mode */
  /* output : 4:4:4 */
  HDMI_read_modify_write(0x16, 0 << 6, 3 << 6);
  /* output : disable color space converter */
  HDMI_read_modify_write(0x18, 0 << 7, 1 << 7);
  /* output : dvi mode */
  HDMI_read_modify_write(0xaf, 0 << 1, 1 << 1);
  BSP_I2C2_DeInit();
}
