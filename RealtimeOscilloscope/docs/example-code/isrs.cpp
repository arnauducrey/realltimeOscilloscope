#include "critical/critical.h"
#include "mcu/mcu.h"
#include "board/buttonscontroller.h"
#include "main.h"

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

