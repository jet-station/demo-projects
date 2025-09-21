/*****************************************************************************
 * @file      bsp_stm32f103_bluepill.c
 * @author    Jet Station
 * @brief     Board Support Package for STM32F103 BluePill Development Board
 * @date      [2025-09-20]
 *
 * Contact:
 *   @website   https://jet-station.github.io/
 *   @github    https://github.com/jet-station
 *   @linkedin  https://www.linkedin.com/in/thien-ai-ho/
 *   @email     thienaiho95@gmail.com
 *
 * @copyright  Copyright (c) 2025 Jet Station. All rights reserved.
 *****************************************************************************/

#include "bsp_stm32f103_bluepill.h"

static void BSP_Stm32f103BluePill_turnLedPc13On(void)
{
	/* TODO: Implement actual code */
}

void BSP_Stm32f103BluePill_turnOnBoardLedsOn(void)
{
	BSP_Stm32f103BluePill_turnLedPc13On();
}