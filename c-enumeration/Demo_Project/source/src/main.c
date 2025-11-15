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

#define TIMER_TIMEOUT_CNT_INIT (0U)
#define TIMER_TIMEOUT_CNT_MAX (20U)

typedef enum {
    GPIO_LOW  = 0U,
    GPIO_HIGH = 1U
} GpioState_et;

typedef enum {
	TIMER_INIT = 0U,
	TIMER_TIMEOUT_1000MS = 1000U
} TimerValue_et;

typedef enum {
	TIMER_NO_EVT,
	TIMER_TIMEOUT_EVT,
	TIMER_SHUTDOWN_EVT
} TimerEvent_et;

typedef enum {
	LED_INIT_STATE, 	/* Initial state 		*/
	LED_OFF_STATE, 		/* Running state - OFF	*/
	LED_ON_STATE, 		/* Running state - ON 	*/
	LED_STOP_STATE, 	/* Stop state 			*/
	LED_ERROR_STATE		/* Error state 			*/
} LedState_et;

void Timer_EventProcessor(void);		/* Timer Event Processor 		*/
void Timer_EventReset(void);			/* Timer Reset Event 			*/
void Led_SateMachineHandler(void); 		/* LED State Machine Handler 	*/

void BSP_LedWrite(GpioState_et state);
void BSP_LedOff(void);
void BSP_LedOn(void);
void BSP_LedError(void);

uint32_t tickCountMs = TIMER_INIT;
LedState_et ledState = LED_INIT_STATE;
TimerEvent_et timerEvt = TIMER_NO_EVT;
uint8_t timerTimeoutEvtCnt = TIMER_TIMEOUT_CNT_INIT;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* infinite loop */
	while (1)
	{
		/* Count up every loop, simluate 1 loop = 1ms */
		tickCountMs++;
		
		/* Update timer event */
		Timer_EventProcessor();
		
		/* Process based on current LED state & Timer event */
		Led_SateMachineHandler();
	}

	return 0;
}

/* Timer Events Processor */
void Timer_EventProcessor(void) {

	if (timerTimeoutEvtCnt <= TIMER_TIMEOUT_CNT_MAX) {
		if (tickCountMs >= TIMER_TIMEOUT_1000MS) {
			timerEvt = TIMER_TIMEOUT_EVT;
			timerTimeoutEvtCnt++;
			tickCountMs = TIMER_INIT;
		}
		else {
			/* do nothing */
		}
	}
	else { 	/* Shutdown timer when exceeds threshold */
		timerEvt = TIMER_SHUTDOWN_EVT;
		tickCountMs = TIMER_INIT;
	}
}

/* Timer reset event */
void Timer_EventReset(void) {
	timerEvt = TIMER_NO_EVT;
}

/* LED State Machine Handler */
void Led_SateMachineHandler() {
	/* Event safety check before processing it */
	if ((timerEvt >= TIMER_NO_EVT) && (timerEvt <= TIMER_SHUTDOWN_EVT))
	{
		/* Special handling for init state */
		if (ledState == LED_INIT_STATE) {
			BSP_LedOff();
			ledState = LED_OFF_STATE;
			return;
		}
		else { /* Not initial state => go to State Handler below */
		}
		
		switch (ledState) {
			case LED_OFF_STATE: {
				switch (timerEvt) {
					case TIMER_TIMEOUT_EVT: {
						BSP_LedOn();
						ledState = LED_ON_STATE;
						Timer_EventReset();
						break;
					}
					default: {
						break;
					}
				}
				break;
			}
			case LED_ON_STATE: {
				switch (timerEvt) {
					case TIMER_TIMEOUT_EVT: {
						BSP_LedOff();
						ledState = LED_OFF_STATE;
						Timer_EventReset();
						break;
					}
					case TIMER_SHUTDOWN_EVT: {
						BSP_LedOff();
						ledState = LED_STOP_STATE;
					}
					default: {
						break;
					}
				}
				break;
			}
			case LED_STOP_STATE: {
				/* No action => Need to reset to restart the sequence */
				break;
			}
			default: { /* invalid state => turn on notification */
				BSP_LedError();
				ledState = LED_ERROR_STATE;
				break;
			}
		}
	}
	else {
		/* Invalid event => turn on notification */
		BSP_LedError();
		ledState = LED_ERROR_STATE;
	}
}

void BSP_LedWrite(GpioState_et state) {
	/* Todo: Actual implementation */
	(void)state;
}
void BSP_LedOff(void) {
	BSP_LedWrite(GPIO_LOW);
}
void BSP_LedOn(void) {
	BSP_LedWrite(GPIO_HIGH);
}
void BSP_LedError(void) {
	BSP_LedWrite(GPIO_HIGH);
	/* Delay(300ms) */
	BSP_LedWrite(GPIO_LOW);
	/* Delay(300ms) */
}