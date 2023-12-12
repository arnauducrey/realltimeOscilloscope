#include "stm32f7xx_hal.h"
#include "main.h"

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
	volatile uint32_t value = HAL_ADC_GetValue(hadc);
}
