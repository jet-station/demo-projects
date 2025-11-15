/*****************************************************************************
 * @file      main.c
 * @author    Jet Station
 * @brief     Main program body
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

#include <stdbool.h> /* Standard bool data types */
#include <stdint.h> /* Standard integer data types */
#include <stm32f103x6.h>

#define STM32F103xB
#define GPIO_PIN_13     ((uint16_t)0x2000)  /* Pin 13 selected   */
#define NULL			((void *)0)

typedef enum {
    GPIO_LOW  = 0U,
    GPIO_HIGH = 1U
} GpioState_et;

/* BSP Functions - LED Control */
void BSP_LedWrite(GpioState_et state);
void BSP_LedOff(void);
void BSP_LedOn(void);
void BSP_LedToggle(void);
void BSP_LedError(void);

/* BSP Functions - Temperature Sensor ADC */
bool BSP_AdcReadTemperature(int16_t *temp_out);
bool BSP_AdcReadRaw(uint16_t *adc_value);

/* BSP Functions - Hardware Initialization */
void BSP_GpioInit(void);
void BSP_AdcInit(void);

/* Temperature monitoring variables */
int16_t current_temp = 0;        /* Temperature in 0.1°C units */
uint16_t adc_raw_value = 0;      /* Raw ADC value */

#define TEMP_THRESHOLD_HIGH  400  /* 40.0°C warning threshold */
#define TEMP_THRESHOLD_ALARM 600  /* 60.0°C alarm threshold */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* Decleare & Initialize local variables */
	uint32_t tickCountMs = 0U;
	
	/* Initialize hardware peripherals */
	BSP_GpioInit();
	BSP_AdcInit();

	/* infinite loop */
	while (1)
	{
		/* Count up every loop, simluate 1 loop = 1ms */
		tickCountMs++;
		
		/* Temperature monitoring example - demonstrates if statement best practices */
		/* Technique #5: Short-circuit evaluation - cheap checks first */
		if (tickCountMs % 500 == 0) {  /* Read temperature every 500ms */
			
			/* Best Practice #5: Error handling with explicit checks */
			if (BSP_AdcReadTemperature(&current_temp) == true) {
				
				/* Pitfall #3 Solution: Use fixed-point instead of float */
				/* Practice #6: Named constants instead of magic numbers */
				
				/* Best Practice #1: Explicit comparison */
				if (current_temp >= TEMP_THRESHOLD_ALARM) {
					/* Critical temperature - trigger alarm */
					BSP_LedError();
					
				} else if (current_temp >= TEMP_THRESHOLD_HIGH) {
					/* Warning temperature - turn on LED */
					BSP_LedOn();
					
				} else {
					/* Normal temperature - LED controlled by state machine */
					/* Do nothing, let state machine handle LED */
				}
				
			} else {
				/* ADC read failed - handle error */
				/* Practice #4: Defensive programming */
				BSP_LedError();
			}
		}
		else {
			/* Do nothing */
		}
		
		/* Demonstrate raw ADC reading */
		/* Technique #5: Short-circuit - null check prevents dereference */
		if (tickCountMs % 1000 == 0 && BSP_AdcReadRaw(&adc_raw_value)) {
			/* Best Practice #3: Range validation already done in BSP function */
			/* Successfully read ADC - value available in adc_raw_value */
			
			/* Pitfall #2 Solution: Check for overflow before arithmetic */
			if (adc_raw_value < (4095 - 100)) {
				uint16_t adjusted = adc_raw_value + 100;  /* Safe addition */
				(void)adjusted;  /* Use value as needed */
			}
		}
		else {
			/* Do nothing */
		}
	}

	return 0;
}

void BSP_LedWrite(GpioState_et state) {
	/* STM32F103 Blue Pill onboard LED on PC13 (active low) */
	#ifdef STM32F103xB
		/* PC13 LED - Active LOW (0 = ON, 1 = OFF) */
		if (state == GPIO_HIGH) {
			GPIOC->BSRR = GPIO_PIN_13 << 16;  /* Reset bit - Turn ON LED */
		} else {
			GPIOC->BSRR = GPIO_PIN_13;         /* Set bit - Turn OFF LED */
		}
	#else
		/* Generic implementation for other platforms */
		(void)state;
	#endif
}

void BSP_LedOff(void) {
	BSP_LedWrite(GPIO_LOW);
}

void BSP_LedOn(void) {
	BSP_LedWrite(GPIO_HIGH);
}

void BSP_LedToggle(void) {
	#ifdef STM32F103xB
		GPIOC->ODR ^= GPIO_PIN_13;  /* Toggle PC13 */
	#endif
}

void BSP_LedError(void) {
	/* Fast blink pattern for error indication */
	for (uint8_t i = 0; i < 3; i++) {
		BSP_LedWrite(GPIO_HIGH);
		/* Simple delay ~300ms (approximate, CPU-dependent) */
		for (volatile uint32_t delay = 0; delay < 300000; delay++);
		BSP_LedWrite(GPIO_LOW);
		for (volatile uint32_t delay = 0; delay < 300000; delay++);
	}
}

/**
 * @brief Read temperature sensor via ADC
 * @param temp_out Pointer to store temperature in 0.1°C units (e.g., 253 = 25.3°C)
 * @return true if reading successful, false otherwise
 * 
 * Demonstrates:
 * - Best Practice #3: Null pointer check before dereferencing
 * - Best Practice #3: Range validation
 * - Technique #5: Short-circuit evaluation for safety
 */
bool BSP_AdcReadTemperature(int16_t *temp_out) {
	/* Best Practice #3 Solution: Always validate pointers first */
	if (temp_out == NULL) {
		return false;
	}
	
	#ifdef STM32F103xB
		/* Short-circuit evaluation: check hardware ready before reading */
		if ((ADC1->SR & ADC_SR_EOC) == 0) {
			/* Start ADC conversion if not already running */
			ADC1->CR2 |= ADC_CR2_ADON;
			
			/* Wait for conversion (with timeout to avoid infinite loop) */
			uint32_t timeout = 10000;
			while (((ADC1->SR & ADC_SR_EOC) == 0) && (timeout > 0)) {
				timeout--;
			}
			
			if (timeout == 0) {
				return false;  /* Timeout - ADC not responding */
			}
		}
		
		/* Read ADC value (12-bit: 0-4095) */
		uint16_t adc_raw = ADC1->DR & 0x0FFF;
		
		/* Best Practice #3: Range validation */
		if (adc_raw > 4095) {
			return false;  /* Invalid ADC reading */
		}
		
		/* Convert ADC to temperature using LM35 sensor formula */
		/* LM35: 10mV/°C, ADC Vref=3.3V, 12-bit (4096 levels) */
		/* Temp(°C) = (ADC * 3.3V / 4096) / 0.01V */
		/* Simplified: Temp = (ADC * 3300) / (4096 * 10) = (ADC * 33) / 41 */
		/* For 0.1°C precision: Temp_0.1C = (ADC * 330) / 41 */
		
		/* Pitfall #2 Solution: Use wider type to prevent overflow */
		uint32_t temp_calc = ((uint32_t)adc_raw * 330) / 41;
		
		/* Best Practice #3: Validate result is in reasonable range */
		if (temp_calc > 1500) {  /* Max 150°C */
			return false;
		}
		
		*temp_out = (int16_t)temp_calc;
		return true;
	#else
		/* Simulated temperature for non-STM32 platforms */
		*temp_out = 253;  /* 25.3°C */
		return true;
	#endif
}

/**
 * @brief Read raw ADC value from temperature sensor
 * @param adc_value Pointer to store raw ADC value (0-4095)
 * @return true if reading successful, false otherwise
 */
bool BSP_AdcReadRaw(uint16_t *adc_value) {
	/* Practice #5: Error handling pattern with null check */
	if (adc_value == NULL) {
		return false;
	}
	
	#ifdef STM32F103xB
		/* Technique #5: Short-circuit - check ready flag before accessing data */
		if ((ADC1->SR & ADC_SR_EOC) != 0) {
			*adc_value = ADC1->DR & 0x0FFF;
			
			/* Best Practice #3: Validate range */
			if (*adc_value > 4095) {
				return false;
			}
			
			return true;
		}
		return false;  /* ADC not ready */
	#else
		*adc_value = 2048;  /* Simulated mid-range value */
		return true;
	#endif
}

/**
 * @brief Initialize GPIO for LED
 */
void BSP_GpioInit(void) {
	#ifdef STM32F103xB
		/* Enable GPIOC clock */
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
		
		/* Configure PC13 as output (50MHz, Push-Pull) */
		/* CNF13[1:0] = 00 (General purpose output push-pull) */
		/* MODE13[1:0] = 11 (Output mode, max speed 50 MHz) */
		GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
		GPIOC->CRH |= GPIO_CRH_MODE13;
		
		/* Initial state: LED OFF (PC13 high for active-low LED) */
		GPIOC->BSRR = GPIO_PIN_13;
	#endif
}

/**
 * @brief Initialize ADC for temperature sensor
 */
void BSP_AdcInit(void) {
	#ifdef STM32F103xB
		/* Enable ADC1 and GPIOA clock */
		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;
		
		/* Configure PA0 as analog input */
		GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);  /* Analog mode */
		
		/* ADC Configuration */
		/* Set sample time for channel 0 (239.5 cycles for accurate reading) */
		ADC1->SMPR2 |= ADC_SMPR2_SMP0;
		
		/* Set regular sequence: 1 conversion, channel 0 */
		ADC1->SQR1 = 0;  /* 1 conversion */
		ADC1->SQR3 = 0;  /* Channel 0 */
		
		/* Enable ADC */
		ADC1->CR2 |= ADC_CR2_ADON;
		
		/* Wait for ADC to stabilize (at least 1μs) */
		for (volatile uint32_t delay = 0; delay < 1000; delay++);
		
		/* Start calibration */
		ADC1->CR2 |= ADC_CR2_CAL;
		while (ADC1->CR2 & ADC_CR2_CAL);  /* Wait for calibration */
		
		/* Start first conversion */
		ADC1->CR2 |= ADC_CR2_ADON;
	#endif
}