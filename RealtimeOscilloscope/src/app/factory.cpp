#include "mcu/mcu.h"
#include "xf/xf.h"
#include "factory.h"
#include "board/interface/buttonscontrollercallbackprovider.h"

extern "C" TIM_HandleTypeDef htim1;         // Defined in main.c
extern "C" ADC_HandleTypeDef hadc3;         // Defined in main.c

oscilloscope::Controller Factory::_oscilloscopeController;
oscilloscope::Gui Factory::_gui;
external::FrequencyGenerator Factory::_fgen;
oscilloscope::FreqGenController Factory::_fgenctrl;
ButtonsController Factory::_bc;

void Factory_initialize()
{
    Factory::initialize();

}

void Factory_build()
{
    Factory::build();
}

Factory::Factory()
{

}

// static
void Factory::initialize()
{
    Trace::out("---------------------------------------------");
    Trace::out("Initializing...");

    // TODO: Uncomment code line below in order to call OscilloscopeController's initialize() method
//	getOscilloscopeController().initialize(getGui(), adcValuesBuffer, ADC_VALUES_BUFFER_SIZE);
    getFrequencyGenerator().initialize();
    getFreqGenController().initialize(getGui());
#if (TOUCHGFX_BAREMETAL != 0)
    getTouchGfxTask().initialize();
#endif  // TOUCHGFX_BAREMETAL
}

// static
void Factory::build()
{
    Trace::out("Building...");

    HAL_ADC_Start_IT(&hadc3);                     // Start ADC conversion
    HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);   // Start TIM1 with trigger channel 1

    getOscilloscopeController().start();
    getGui().start();
    getButtonsController().registerCallback(&getFreqGenController(),
        								    (interface::ButtonsControllerCallbackProvider::CallbackMethod)&oscilloscope::FreqGenController::onButtonChanged);

    getFreqGenController().start();
    getButtonsController().start();
#if (TOUCHGFX_BAREMETAL != 0)
    getTouchGfxTask().start();
#endif  // TOUCHGFX_BAREMETAL

    Trace::out("Running...");
}

oscilloscope::Controller & Factory::getOscilloscopeController()
{
    return oscilloscope::Controller::getInstance();
}

//static
oscilloscope::Gui & Factory::getGui()
{
    return _gui;
}

//static
ButtonsController& Factory::getButtonsController() {
	return _bc;
}

//static
oscilloscope::FreqGenController& Factory::getFreqGenController() {
	return _fgenctrl;
}


#if (TOUCHGFX_BAREMETAL != 0)
// static
TouchGfxTask & Factory::getTouchGfxTask()
{
    static TouchGfxTask touchGfxTask;
    return touchGfxTask;
}
#endif  // TOUCHGFX_BAREMETAL

FrequencyGenerator& Factory::getFrequencyGenerator() {
	return _fgen;
}
