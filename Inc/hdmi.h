/**
  ******************************************************************************
  * @file    hdmi.h
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

#ifndef HDMI_H
#define HDMI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int32_t HDMI_Detect(void);
void HDMI_Init(void);

#ifdef __cplusplus
}
#endif

#endif
