/*****************************************************************************
 * @file      macro_demo.c
 * @author    Ho Thien Ai
 * @brief     Demonstrations of macro usages
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
 
#include <stdint.h>
#include <stdbool.h>
#include "macro_demo.h"

#define BOARD_STM32F103C6_BLUEPILL
#define DEBUG_ENABLED

#if defined (BOARD_STM32F103C6_BLUEPILL)
	#include "bsp_stm32f103_bluepill.h"
#elif defined (BOARD_EK_TM4C123GXL)
	#include "bsp_ektm4c123gxl.h"
#else
	#error "Development Board is not specified"
#endif

/* Macro for maximum value of ticks */
#define TICKS_MAX_VALUE (4294967295U)

/* Macro for reset value of ticks */
#define TICKS_RESET_VALUE (0U)

/* Macro for ticks per second */
#define TICKS_PER_SECOND (1000U)

/* Macro to convert milliseconds to ticks */
#define MS_TO_TICKS(ms) ((ms) * TICKS_PER_SECOND / 1000U)

/* Macro to check if ticks have elapsed */
#define TICKS_ELAPSED(current, start, interval) (((current) - (start)) >= (interval))

/* Macro to convert and concat string */
#define TO_STRING(x) #x
#define CONCAT(a, b) a##b

/* extern tick variables */
extern uint32_t g_tickCount;
extern uint32_t g_lastEventTick;

#ifdef DEBUG_ENABLED
static void MacroDemo_printDbgLog(void *msg)
{
	(void)msg;
}
#endif

static void MacroDemo_sendNotificationToUser(void)
{
#if defined (BOARD_STM32F103C6_BLUEPILL)
	BSP_Stm32f103BluePill_turnOnBoardLedsOn();
#elif defined (BOARD_EK_TM4C123GXL)
	BSP_Ektm4c123gxl_turnOnBoardLedsOn();
#else
	#error "Development Board is not specified"
#endif
	
#ifdef DEBUG_ENABLED
	/* This code only compiles if DEBUG is defined */
	MacroDemo_printDbgLog("Ticks is elapsed!");
#endif
	
}

void MacroDemo_tickCountUp(void) {
		
	/* roll over handling, keep the diff-time */
	if (g_tickCount == TICKS_MAX_VALUE)
	{
		g_tickCount = g_tickCount - g_lastEventTick;
		g_lastEventTick = TICKS_RESET_VALUE;
	}
	else
	{
	}
	
	/* Simulate tick increment */
	g_tickCount++;

	/* Check if 500ms have passed since last event */
	uint32_t interval = MS_TO_TICKS(500U);
	if (TICKS_ELAPSED(g_tickCount, g_lastEventTick, interval)) {
		g_lastEventTick = g_tickCount;
		
		/* Send notifications to user */
		MacroDemo_sendNotificationToUser();
	}
	else
	{
	}
}