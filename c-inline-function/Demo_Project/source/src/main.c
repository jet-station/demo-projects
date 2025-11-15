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
#include "stm32f103x6.h"

typedef struct {
	uint32_t macroFunc;
	uint32_t inlineFunc;
	uint32_t regFunc;
} FuncExecCycles;

uint32_t testVarA = 100U;
uint32_t testVarB = 250U;
uint32_t testResult = 0U;

FuncExecCycles exeCycles = {.macroFunc = 0U, 
						   .inlineFunc = 0U, 
						   .regFunc = 0U};
		
/* Function-like macro */						   
#define MACRO_MAXFUNC(a, b) ((a) > (b) ? (a) : (b))
						   						  
/* Regular function */
uint32_t Reg_MaxFunc(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

/* Force inline even if compiler doesn't want to */
__attribute__((always_inline)) 
static inline uint32_t Inline_MaxFunc(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

/* Wrapper function to test function-like macro */
void Test_callMacroFunc(void)
{
	testResult = MACRO_MAXFUNC(testVarA, testVarB);
}

/* Wrapper function to test regular function */
void Test_callRegFunc(void) {
    testResult = Reg_MaxFunc(testVarA, testVarB);
}

/* Wrapper function to test inline function */
void Test_callInlineFunc(void) {
    testResult = Inline_MaxFunc(testVarA, testVarB);
}

/* Function to measure the execution time of functions */
void Test_execTiming(void) {
    volatile uint32_t start, end;
	
	/* Mearsure function-like macro execution timing */
    start = DWT->CYCCNT;
    Test_callMacroFunc();
    end = DWT->CYCCNT;
    exeCycles.macroFunc = end - start;
    
    /* Mearsure inline function execution timing */
    start = DWT->CYCCNT;
    Test_callInlineFunc();
    end = DWT->CYCCNT;
    exeCycles.inlineFunc = end - start;
    
	/* Mearsure regular function execution timing */
    start = DWT->CYCCNT;
    Test_callRegFunc();
    end = DWT->CYCCNT;
    exeCycles.regFunc = end - start;
}

/* Data Watchpoint and Trace intialization */
void DWT_init(void) {
    /* Enable trace and debug */
    CoreDebug->DEMCR |= (1U << 24);  // TRCENA bit
    
    /* Reset cycle counter */
    DWT->CYCCNT = 0U;
    
    /* Enable cycle counter */
    DWT->CTRL |= (1U << 0);  // CYCCNTENA bit
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* Initialize DWT first */
	DWT_init();

	Test_callMacroFunc();
	Test_callRegFunc();
	Test_callInlineFunc();
	
	Test_execTiming();
	
	/* infinite loop */
	while (1)
	{
	}

	return 0;
}