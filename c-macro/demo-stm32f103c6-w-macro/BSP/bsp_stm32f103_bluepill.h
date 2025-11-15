/*****************************************************************************
 * @file      bsp_stm32f103_bluepill.h
 * @author    Ho Thien Ai
 * @brief     Board Support Package for STM32F103 BluePill Development Board
 * @date      [2025-09-20]
 *
 * Contact:
 *   @website   https://hothienai.github.io/
 *   @github    https://github.com/hothienai
 *   @linkedin  https://www.linkedin.com/in/thien-ai-ho/
 *   @email     thienaiho95@gmail.com
 *
 * @copyright  Copyright (c) 2025 Ho Thien Ai. All rights reserved.
 *****************************************************************************/

#ifndef __BSP_STM32F103_BLUEPILL_H__
#define __BSP_STM32F103_BLUEPILL_H__

typedef enum
{
	BAD = 0U,
	GOOD
} BoardStatus;

void BSP_Stm32f103BluePill_turnOnBoardLedsOn(void);

#endif