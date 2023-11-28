#ifndef APP_FACTORY_H
#define APP_FACTORY_H

//
// What is seen only by the C++ compiler
//
#ifdef __cplusplus

#include "main.h"
#include "oscilloscopecontroller.h"
#include "gui.h"
#include "config/touchgfx-config.h"
#if (TOUCHGFX_BAREMETAL != 0)
	#include "touchgfxtask.h"
#endif  // TOUCHGFX_BAREMETAL
#include "mdw/ext-freqgen/frequencygenerator.h"
#include "app/freqgencontroller.h"
#include "board/buttonscontroller.h"
using external::FrequencyGenerator;

/**
 * @brief Factory creating all objects/components and relations between them.
 */
class Factory
{
public:
    Factory();                          ///< Constructor

    static void initialize();           ///< Initializes the factory
    static void build();                ///< Creates components and initializes relations

#if (TOUCHGFX_BAREMETAL != 0)
    static TouchGfxTask & getTouchGfxTask();
#endif  // TOUCHGFX_BAREMETAL


    static oscilloscope::Controller & getOscilloscopeController();
    static oscilloscope::Gui & getGui();
    static ButtonsController& getButtonsController();
    static FrequencyGenerator& getFrequencyGenerator();
    static oscilloscope::FreqGenController& getFreqGenController();

protected:
    static oscilloscope::Controller _oscilloscopeController;
    static oscilloscope::Gui _gui;
    static external::FrequencyGenerator _fgen;
    static oscilloscope::FreqGenController _fgenctrl;
    static ButtonsController _bc;
};

#endif // __cplusplus

//
// What is seen by the C and C++ compiler
//
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void Factory_initialize();
void Factory_build();

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // APP_FACTORY_H
