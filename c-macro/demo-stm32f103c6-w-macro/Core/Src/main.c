/*****************************************************************************
 * @file      main.c
 * @author    Ho Thien Ai
 * @brief     Main program body
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

#include <stdbool.h> /* Standard bool data types */
#include <stdint.h> /* Standard integer data types */
#include "macro_demo.h"
#include "bsp_stm32f103_bluepill.h"
#include "bsp_ektm4c123gxl.h"

#define MACRO_USED

/* global tick variables */
uint32_t g_tickCount = 0U;
uint32_t g_lastEventTick = 0U;

//volatile BoardStatus boardStatus = GOOD;

#if !defined(MACRO_USED)
/* type definition */
typedef enum {
	BOARD_STM32F103C6_BLUEPILL = 0U,
	BOARD_EK_TM4C123GXL,
	UNKNOWN
} BoardSelection;

typedef enum {
	DISABLED = 0U,
	ENABLED
} DebugControl;

/* Configurations */
static const BoardSelection selectedBoard = BOARD_STM32F103C6_BLUEPILL;
static const DebugControl dbgCtrl = ENABLED;

static void Demo_printDbgLog(void *msg)
{
	(void)msg;
}

static void Demo_printError(void *msg)
{
	(void)msg;
}

static void Demo_sendNotificationToUserWoMacro(void)
{
	if (selectedBoard == BOARD_STM32F103C6_BLUEPILL)
	{
		BSP_Stm32f103BluePill_turnOnBoardLedsOn();
	}
	else if (selectedBoard == BOARD_EK_TM4C123GXL)
	{
		BSP_Ektm4c123gxl_turnOnBoardLedsOn();
	}
	else
	{
		Demo_printError("Development Board is not specified");
	}
	
	if (dbgCtrl == ENABLED)
	{
		/* This code is alway complied */
		Demo_printDbgLog("Ticks is elapsed!");
	}
}

bool Ticks_elapsed(uint32_t tickCount, uint32_t lastEventTick, uint32_t interval)
{
	bool isElapsed = false;
	
	if ((tickCount - lastEventTick) >= interval)
	{
		isElapsed = true;
	}
	else
	{
	}
	
	return isElapsed;
}

void Demo_tickCountUpWoMacro(void) {
	
	/* roll over handling, keep the diff-time */
	if (g_tickCount == 4294967295U)
	{
		g_tickCount = g_tickCount - g_lastEventTick;
		g_lastEventTick = 0U;
	}
	else
	{
	}
	
	/* Simulate tick increment */
	g_tickCount++;
	
	/* ticks per second = 1000 */
	/* Check if 500ms have passed since last event */
	uint32_t interval = (500U) * (1000U) / (1000U); /* Steps: 500ms * 1000 ticks per second / 1000ms per sec */
	if (Ticks_elapsed(g_tickCount, g_lastEventTick, interval)) {
		g_lastEventTick = g_tickCount;
		
		/* Send notifications to user */
		Demo_sendNotificationToUserWoMacro();
	}
	else
	{
	}
}

#endif

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* infinite loop */
	while (1)
	{
		MacroDemo_tickCountUp();
	}

	return 0;
}