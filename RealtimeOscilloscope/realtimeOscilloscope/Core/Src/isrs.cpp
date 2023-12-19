#include "stm32f7xx_hal.h"
#include "main.h"
#include "board/buttonscontroller.h"

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
	adcValuesBuffer[adcBufferIndex++] = HAL_ADC_GetValue(hadc);
	if(adcBufferIndex > ADC_VALUES_BUFFER_SIZE){
		adcBufferIndex = 0;
	}
	//volatile uint32_t value = HAL_ADC_GetValue(hadc);
}
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
      switch (GPIO_Pin)
      {
        case BUTTON0_Pin:
        case BUTTON1_Pin:
        case BUTTON2_Pin:
        case BUTTON3_Pin:
            ButtonsController::getInstance().onIrq();
            break;
        default:
            break;
      }
}
